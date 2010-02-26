/*
 * This file is part of PetaScope.
 *
 * PetaScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * PetaScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with PetaScope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.PetaScope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */


package petascope.wcps.server.core;

import petascope.wcps.server.exceptions.InvalidCrsException;
import petascope.wcps.server.exceptions.WCPSException;
import petascope.wcps.server.exceptions.InvalidMetadataException;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Vector;
import org.w3c.dom.*;

public class ConstructCoverageExpr implements IRasNode, ICoverageInfo
{

    private String covName;
    private Vector<AxisIterator> iterators;
    private IRasNode values;
    private CoverageInfo info;
    private String axisIteratorString;
    private String newIteratorName;


	public ConstructCoverageExpr(Node node, XmlQuery xq)
	    throws WCPSException, InvalidCrsException
	{
        while ((node != null) && node.getNodeName().equals("#text"))
		{
			node = node.getNextSibling();
		}

        iterators = new Vector();
		System.err.println("Parsing Construct Coverage Expr: " + node.getNodeName());
        newIteratorName = xq.registerNewExpressionWithVariables();

        while (node != null)
        {
            String name = node.getNodeName();
            if (name.equals("name"))
                covName = node.getTextContent();
            else
            if (name.equals("axisIterator"))
            {
                AxisIterator it = new AxisIterator(node.getFirstChild(), xq, newIteratorName);
                iterators.add(it);
                // Top level structures need to know about these iterators
                CoverageIterator dyn = new CoverageIterator(it.getVar(), covName);
                xq.addDynamicCoverageIterator(dyn);
            }
            else
            {
                /* The iterator is probably used in the "values" section,
                 * so send the iterator to the top-level query */
                if (covName != null && iterators .size() > 0)
                    buildMetadata(xq);
                else
                    throw new WCPSException("Cannot build coverage metadata !!!");
                // And only then start parsing the "values" section 
                values = new ScalarExpr(node, xq);
            }

            node = node.getNextSibling();
            while ((node != null) && node.getNodeName().equals("#text"))
            {
                node = node.getNextSibling();
            }
        }

        buildAxisIteratorDomain();

        // Sanity check: metadata should have already been build
        if (info == null)
            throw new WCPSException("Could not build coverage metadata !!!");
	}

	public String toRasQL()
	{
		String result = "marray ";
        result += axisIteratorString;
        result += " values " + values.toRasQL();

        return result;
	}

	public CoverageInfo getCoverageInfo()
	{
		return info;
	}

    /* Concatenates all the AxisIterators into one large multi-dimensional object,
     * that will be used to build to RasQL query */
    private void buildAxisIteratorDomain()
    {
        axisIteratorString = "";
        axisIteratorString += newIteratorName + " in [";

        for (int i = 0; i < iterators.size(); i++)
        {
            if (i > 0)
                axisIteratorString += ", ";
            AxisIterator ai = iterators.elementAt(i);
            axisIteratorString += ai.getInterval();
        }

        axisIteratorString += "]";
    }

    /** Builds full metadata for the newly constructed coverage **/
    private void buildMetadata(XmlQuery xq) throws WCPSException
    {
        List<CellDomainElement> cellDomainList = new LinkedList<CellDomainElement>();
        List<RangeElement> rangeList = new LinkedList<RangeElement>();
        HashSet<String> nullSet = new HashSet<String>();
        String nullDefault = "0";
        nullSet.add(nullDefault);
        HashSet<InterpolationMethod> interpolationSet = new HashSet<InterpolationMethod>();
        InterpolationMethod interpolationDefault = new InterpolationMethod("none", "none");
        interpolationSet.add(interpolationDefault);
        String coverageName = covName;
		List<DomainElement> domainList = new LinkedList<DomainElement>();

        Iterator<AxisIterator> i = iterators.iterator();
        while (i.hasNext())
        {
            // Build domain metadata
            AxisIterator ai = i.next();
            cellDomainList.add(new CellDomainElement( ai.getLow(), ai.getHigh()));
            String axisName = ai.getVar();
            String axisType = ai.getAxisType();
            String crs = DomainElement.WGS84_CRS;
            HashSet<String> crsset = new HashSet<String>();
            crsset.add(crs);
            DomainElement domain = new DomainElement(axisName, axisType,
                    ai.getLow().doubleValue(), ai.getHigh().doubleValue(),
                    null, null, crsset, xq.getMetadataSource().getAxisNames());
            domainList.add(domain);
        }
        // "unsigned int" is default datatype
        rangeList.add(new RangeElement("dynamic_type", "unsigned int"));

        try
        {
            Metadata metadata = new Metadata(cellDomainList, rangeList, nullSet,
                    nullDefault, interpolationSet, interpolationDefault,
                    coverageName, domainList, null);
            // Let the top-level query know the full metadata about us
            xq.getMetadataSource().addDynamicMetadata(covName, metadata);
            info = new CoverageInfo(metadata);
        }
        catch (InvalidMetadataException e)
        {
            throw new WCPSException("Could not build coverage '" + covName
                    + "' metadata !", e);
        }
    }
}
