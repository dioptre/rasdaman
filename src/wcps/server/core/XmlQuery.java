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

package wcps.server.core;


import org.w3c.dom.*;


import java.util.ArrayList;
import java.util.Iterator;

/**
 *
 * @author Andrei Aiordachioaie
 */
class XmlQuery implements IRasNode
{
    private String mime;
    private ArrayList<CoverageIterator> iterators;
    private BooleanScalarExprType where;
    private IRasNode coverageExpr;

    public String getMimeType()
    {
        return mime;
    }

    public XmlQuery()
    {
        super();
    }

    public void startParsing(Node node, ProcessCoveragesRequest pcr) throws WCPSException
    {
        System.err.println("Processing XML Request: " + node.getNodeName());

        Node x = node.getFirstChild();
        iterators = new ArrayList<CoverageIterator>();

        while (x != null)
        {
            if (x.getNodeName().equals("#text"))
            {
                x = x.getNextSibling();
                continue;
            }

            System.err.println("The current node is: " + x.getNodeName());

            if (x.getNodeName().equals("coverageIterator"))
            {
                iterators.add(new CoverageIterator(x, pcr));
            }
            else if (x.getNodeName().equals("where"))
            {
                where = new BooleanScalarExprType(x.getFirstChild(), pcr);
            }
            else if (x.getNodeName().equals("encode"))
            {
                EncodeDataExprType encode;

                encode = new EncodeDataExprType(x, pcr);
                coverageExpr = encode;
                mime = encode.getMime();
            }
            else
            {
                // It has to be a scalar Expr Type
                coverageExpr = new ScalarExprType(x, pcr);
                mime = "text/plain";
            }

            x = x.getNextSibling();
        }
    }

    public XmlQuery(Node node, ProcessCoveragesRequest pcr) throws WCPSException
    {
        startParsing(node, pcr);
    }

    public Boolean isIteratorDefined(String iteratorName)
	{
		Iterator<CoverageIterator> it = iterators.iterator();

		while (it.hasNext())
		{
			CoverageIterator tmp = it.next();

			if (iteratorName.equals(tmp.getIteratorName()))
			{
				return true;
			}
		}

		return false;
	}

    public Iterator<String> getCoverages(String iteratorName) throws WCPSException
	{
		for (int i = 0; i < iterators.size(); ++i)
		{
			if (iterators.get(i).getIteratorName().equals(iteratorName))
			{
				return iterators.get(i).getCoverages();
			}
		}

		throw new WCPSException("Iterator " + iteratorName + " not defined");
	}

    public String toRasQL()
    {
        String result = "select " + coverageExpr.toRasQL() + " from ";
        Iterator<CoverageIterator> it = iterators.iterator();
        boolean first = true;

        while (it.hasNext())
        {
            if (first)
            {
                first = false;
            }
            else
            {
                result += ", ";
            }

            result += it.next().toRasQL();
        }

        if (where != null)
        {
            result += " where " + where.toRasQL();
        }

        return result;
    }
}
