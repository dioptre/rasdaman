/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2012 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package secore;

import java.util.List;
import java.util.HashSet;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.HashMap;
import secore.util.SecoreException;
import secore.util.ExceptionCode;
import secore.util.Pair;
import secore.util.StringUtil;
import secore.util.XMLUtil;
import java.util.ArrayList;
import java.util.regex.Pattern;
import javax.script.ScriptException;
import net.n3.nanoxml.IXMLElement;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import static secore.ParameterizedCrsHandler.*;
import static secore.util.Constants.*;

/**
 * Handle parameterized CRSs. An example of a parameterized CRS is the 
 * auto universal transverse mercator layer CRS (AUTO:42001):
 * 
  <ParameterizedCRS>
    <parameters>
        <parameter name="lon"/>
        <parameter name="lat">
            <value>0.0</value>
        </parameter>
        <parameter name="zone">
            <value>min( floor( (${lon} + 180.0) / 6.0 ) + 1, 60 )</value>
        </parameter>
        <parameter name="central_meridian">
            <value>-183.0 + ${zone} * 6.0</value>
            <target>//greenwichLongitude</target>
        </parameter>
        <parameter name="false_northing">
            <value>(${lat} >= 0.0) ? 0.0 : 10000000.0</value>
            <target>//falseNorthing</target>
        </parameter>
    </parameters>
    <identifier>http://www.opengis.net/def/crs/AUTO/1.3/42001</identifier>
    <targetCRS xlink:href="http://www.opengis.net/def/crs/EPSG/0/4326"/>
  </ParameterizedCRS>

 * targetCRS holds the CRS that this template refers to.
 * <p>
 * This class first delegates the handling to the {@link GeneralHandler} and then
 * continues if the result is a ParameterizedCRS or returns.
 *
 * @author Dimitar Misev
 */
public class ParameterizedCrsHandler extends GeneralHandler {
  
  private static Logger log = LoggerFactory.getLogger(ParameterizedCrsHandler.class);
  
  // element names in a parameterized CRS GML definition
  public static final String PARAMETERIZED_CRS = "ParameterizedCRS";
  public static final String PARAMETERS = "parameters";
  public static final String PARAMETER = "parameter";
  public static final String PARAMETER_NAME = "name";
  public static final String PARAMETER_VALUE = "value";
  public static final String PARAMETER_TARGET = "target";
  public static final String TARGET_CRS = "targetCRS";
  public static final String TARGET_CRS_HREF = "href";

  @Override
  public boolean canHandle(ResolveRequest request) {
    return request.getOperation() != null
        && request.getOperation().equals(OP_CRS) && request.getParams().size() > 3;
  }

  @Override
  public GmlResponse handle(ResolveRequest request) throws SecoreException {
    log.debug("Handling resolve request...");
    
    // first resolve the parameterized CRS
    ResolveRequest req = new ResolveRequest(request.getOperation(),
        request.getService(), request.getFullUri());
    int i = 0;
    for (Pair<String, String> p : request.getParams()) {
      if (p.snd == null) { // it's REST
        req.addParam(p.fst, p.snd);
        ++i;
      } else {
        if (p.fst.equals(CODE_KEY) || p.fst.equals(VERSION_KEY) || p.fst.equals(AUTHORITY_KEY)) {
          req.addParam(p.fst, p.snd);
          ++i;
        }
      }
      if (i == 3) {
        break;
      }
    }
    if (req.getParams().size() != 3) {
      throw new SecoreException(ExceptionCode.InvalidRequest, "Invalid Parameterized CRS request");
    }
    GmlResponse gml = resolveRequest(req, 0);
    
    // check if the result is a ParameterizedCRS
    String rootElementName = StringUtil.getRootElementName(gml.getData());
    if (!PARAMETERIZED_CRS.equals(rootElementName)) {
      return gml;
    }
    
    // handling of parameterized CRS
    IXMLElement root = XMLUtil.parse(gml.getData());
    
    // extract needed data from the XML, doing various validity checks
    Parameters parameters = new Parameters();
    String identifier = null;
    String targetCRS = null;
    for (int k = 0; k < root.getChildrenCount(); k++) {
      IXMLElement c = root.getChildAtIndex(k);
      if (c.getName().equals(IDENTIFIER_LABEL)) {
        identifier = c.getContent();
      } else if (c.getName().equals(TARGET_CRS)) {
        targetCRS = c.getAttribute("xlink:" + TARGET_CRS_HREF, null);
      } else if (c.getName().equals(PARAMETERS)) {
        parameters.parse(c);
      } else {
        throw new SecoreException(ExceptionCode.XmlNotValid.locator(c.getName()),
            "Unexpected element name " + c.getName() + ", expected one of " +
            "parameters, " + IDENTIFIER_LABEL + ", or targetCRS");
      }
    }
    if (identifier == null) {
      throw new SecoreException(ExceptionCode.XmlNotValid,
          "Mandatory IDENTIFIER_LABEL missing from the GML definition");
    }
    if (targetCRS == null) {
      throw new SecoreException(ExceptionCode.XmlNotValid,
          "Mandatory target CRS missing from the GML definition");
    }
    
    // parse the query parameters, and accordingly update the template parameters
    for (Pair<String, String> p : request.getParams()) {
      String name = p.fst;
      String value = p.snd;
      if (!name.equals(AUTHORITY_KEY) && !name.equals(CODE_KEY) &&
          !name.equals(VERSION_KEY) && value != null) {
        Parameter parameter = parameters.get(name);
        if (parameter == null) {
          throw new SecoreException(ExceptionCode.InvalidRequest.locator(name),
              "Specified parameter not supported by this Parameterized CRS.");
        }
        parameter.setValue(value);
      }
    }
    
    // do the actual work
    parameters.evaluateParameters();
    
    // get the target CRS
    ResolveRequest targetCRSRequest = StringUtil.buildRequest(targetCRS);
    
    // get (URN, URL) for the target CRS
    Pair<String, String> p = parseRequest(targetCRSRequest);
    
    // extract parameters with targets
    List<Parameter> params = new ArrayList<Parameter>();
    for (Parameter parameter : parameters.values()) {
      if (parameter.getTarget() != null) {
        params.add(parameter);
      }
    }
    
    // try to resolve a URN first
    String res = null;
    try {
      res = resolve(IDENTIFIER_LABEL, p.fst, 2, params);
    } catch (SecoreException ex) {
      if (ExceptionCode.NoSuchDefinition.equals(ex.getExceptionCode())) {
        // URN resolution failed, try with a REST now
        res = resolve(IDENTIFIER_LABEL, p.snd, 2, params);
      } else {
        throw ex;
      }
    }
    
    return new GmlResponse(res);
  }
  
}

class Parameters extends HashMap<String, Parameter> {
  
  private static Logger log = LoggerFactory.getLogger(Parameters.class);
  
  // used to detect circular references
  private Set<String> stack = new HashSet<String>();

  /**
   * @param xml a parameters XML fragment
   */
  public void parse(IXMLElement xml) throws SecoreException {
    for (int i = 0; i < xml.getChildrenCount(); i++) {
      IXMLElement parameter = xml.getChildAtIndex(i);
      if (!parameter.getName().equals(PARAMETER)) {
        throw new SecoreException(ExceptionCode.XmlNotValid,
            "Expected element name " + PARAMETER + ", got " + parameter.getName());
      }
      String name = parameter.getAttribute(PARAMETER_NAME, null);
      if (name == null) {
        throw new SecoreException(ExceptionCode.XmlNotValid,
            "Missing mandatory attribute " + PARAMETER_NAME);
      }
      if (containsKey(name)) {
        throw new SecoreException(ExceptionCode.XmlNotValid,
            "Duplicate parameter " + name);
      }
      
      String value = null;
      String target = null;
      for (int j = 0; j < parameter.getChildrenCount(); j++) {
        IXMLElement c = parameter.getChildAtIndex(j);
        if (c.getName().equals(PARAMETER_VALUE)) {
          value = c.getContent();
        } else if (c.getName().equals(PARAMETER_TARGET)) {
          target = c.getContent();
        } else {
          throw new SecoreException(ExceptionCode.XmlNotValid,
              "Unknown element name " + c.getName() + ", expected "
              + PARAMETER_VALUE + " or " + PARAMETER_TARGET);
        }
      }
      
      put(name, new Parameter(name, value, target));
    }
  }
  
  /**
   * Substitute the value expression with computed values.
   * @throws SecoreException in case of an error while evaluating a value
   *  expression
   */
  public void evaluateParameters() throws SecoreException {
    stack.clear();
    for (Parameter parameter : values()) {
      evaluateParameter(parameter);
    }
  }
  
  /**
   * 
   * @param parameter
   * @throws SecoreException 
   */
  private void evaluateParameter(Parameter parameter) throws SecoreException {
    if (stack.contains(parameter.getName())) {
      throw new SecoreException(ExceptionCode.XmlNotValid.locator(parameter.getName()),
          "Parameter value leads to circular evaluation.");
    }
    if (parameter.isEvaluated()) {
      return;
    }
    
    stack.add(parameter.getName());
    
    // substitute references
    Set<String> refs = parameter.getRefs();
    for (String refName : refs) {
      Parameter refParameter = get(refName);
      evaluateParameter(refParameter);
      parameter.substituteReference(refName, refParameter.getValue());
    }
    
    // now evaluate this parameter's value
    parameter.evaluateValue();
    
    stack.remove(parameter.getName());
  }
}

/**
 * Class representing a paramater. Holds the parameters name, value, and target
 * XPath expression.
 * 
 * @author Dimitar Misev
 */
class Parameter {
  
  private static Logger log = LoggerFactory.getLogger(Parameter.class);
  
  public static final Pattern PATTERN = Pattern.compile("\\$\\{([^\\}]+)\\}");
  
  private final String name;
  private String value;
  private final String target;
  
  // parameters referenced from a value
  private final Set<String> refs;
  
  // flags whether value has been evaluated, in order to avoid re-evaluations
  private boolean evaluated;

  public Parameter(String name) {
    this(name, null);
  }

  public Parameter(String name, String value) {
    this(name, value, null);
  }

  public Parameter(String name, String value, String target) {
    this.name = name;
    this.value = value;
    this.target = target;
    this.refs = new HashSet<String>();
    
    // extract referenced parameters
    if (value != null) {
      value = value.replaceAll("&gt;", ">").replaceAll("&lt;", "<").replaceAll("&amp;", "&");
      Matcher matcher = PATTERN.matcher(value);
      while (matcher.find()) {
        try {
          refs.add(matcher.group(1));
        } catch (Exception ex) {
          log.warn(ex.getMessage());
        }
      }
    }
  }
  
  public void substituteReference(String name, String value) {
    this.value = this.value.replaceAll("\\$\\{" + name + "\\}", value);
  }
  
  /**
   * Update the value of the parameter.
   * 
   * @throws SecoreException in case of an error while evaluating the value
   *  expression
   */
  public void evaluateValue() throws SecoreException {
    if (evaluated) {
      return;
    }
    try {
      this.value = StringUtil.evaluate(value);
      evaluated = true;
    } catch (ScriptException ex) {
      throw new SecoreException(ExceptionCode.InvalidParameterValue.locator(name),
          "Failed evaluating the parameter value: " + value);
    }
  }

  public String getName() {
    return name;
  }

  public Set<String> getRefs() {
    return refs;
  }

  public String getTarget() {
    return target;
  }

  public String getValue() {
    return value;
  }

  public boolean isEvaluated() {
    return evaluated;
  }

  public void setValue(String value) {
    this.value = value;
  }
}