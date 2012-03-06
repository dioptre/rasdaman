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
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.wcs2.parsers;

import java.util.List;
import nu.xom.Element;
import petascope.exceptions.WCSException;
import petascope.wcs2.handlers.RequestHandler;
import static petascope.util.XMLUtil.*;
import static petascope.util.XMLSymbols.*;
import javax.xml.validation.SchemaFactory;
import javax.xml.validation.Schema;
import org.xml.sax.SAXException;
import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Validator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.io.StringBufferInputStream;
import java.net.URL;
import java.net.MalformedURLException;
import petascope.exceptions.ExceptionCode;
import java.io.IOException;
/**
 * Parse a GetCapabilities XML request.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class XMLDescribeCoverageParser extends XMLParser<DescribeCoverageRequest> {

    private Schema schema;
    private SchemaFactory schemaFactory;
    private final String SCHEMA="http://schemas.opengis.net/wcs/2.0/wcsDescribeCoverage.xsd";
    Logger log = LoggerFactory.getLogger(XMLDescribeCoverageParser.class);

    public XMLDescribeCoverageParser(){

	try{
	    schemaFactory=SchemaFactory.newInstance("http://www.w3.org/2001/XMLSchema");
	    schema=schemaFactory.newSchema(new URL(SCHEMA));

	}catch(SAXException e){
	    log.error("Could not initialize the XML Schema validator. Schema validation will be disabled.",e);
	}catch(MalformedURLException e){
	    log.error("Could not initialize the XML Schema validator. Schema validation will be disabled.",e);
	}
    }

    @Override
    public DescribeCoverageRequest parse(String input) throws WCSException {

	Source requestStream=new StreamSource(new StringBufferInputStream(input));
	Validator validator=schema.newValidator();
	try{
	    validator.validate(requestStream);
	}catch(SAXException e){
	    throw new WCSException(ExceptionCode.XmlNotValid,"The structure of the provided input is not valid.");
	}catch(NullPointerException e){
	    log.warn("The recieved XML document could not be validated.");
	}catch(IOException e){
	    throw new WCSException(ExceptionCode.WcsError,"A fatal error ocurred processing the input.");
	}
        Element root = parseInput(input);
        List<Element> coverageIds = collectAll(root, PREFIX_WCS,
                LABEL_COVERAGE_ID, CTX_WCS);
        DescribeCoverageRequest ret = new DescribeCoverageRequest();
        for (Element coverageId : coverageIds) {
            ret.getCoverageIds().add(getText(coverageId));
        }
        return ret;
    }

    @Override
    public String getOperationName() {
        return RequestHandler.DESCRIBE_COVERAGE;
    }
}
