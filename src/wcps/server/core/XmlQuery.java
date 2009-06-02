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
    private BooleanScalarExpr where;
    private IRasNode coverageExpr;
    private IDynamicMetadataSource meta;
    private ArrayList<CoverageIterator> dynamicIterators;

    public String getMimeType()
    {
        return mime;
    }

    public XmlQuery(IDynamicMetadataSource source)
    {
        super();
        this.meta = source;
    }

    public void startParsing(Node node) throws WCPSException
    {
        System.err.println("Processing XML Request: " + node.getNodeName());

        Node x = node.getFirstChild();
        iterators = new ArrayList<CoverageIterator>();
        dynamicIterators = new ArrayList<CoverageIterator>();

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
                iterators.add(new CoverageIterator(x, this));
            }
            else if (x.getNodeName().equals("where"))
            {
                where = new BooleanScalarExpr(x.getFirstChild(), this);
            }
            else if (x.getNodeName().equals("encode"))
            {
                EncodeDataExpr encode;

                encode = new EncodeDataExpr(x, this);
                coverageExpr = encode;
                mime = encode.getMime();
            }
            else
            {
                // It has to be a scalar Expr 
                coverageExpr = new ScalarExpr(x, this);
                mime = "text/plain";
            }

            x = x.getNextSibling();
        }
    }

    public XmlQuery(Node node) throws WCPSException
    {
        this.startParsing(node);
    }

    public Boolean isIteratorDefined(String iteratorName)
	{
		Iterator<CoverageIterator> it = iterators.iterator();
		while (it.hasNext())
		{
			CoverageIterator tmp = it.next();
			if (iteratorName.equals(tmp.getIteratorName()))
				return true;
		}

        it = dynamicIterators.iterator();
        while (it.hasNext())
        {
			CoverageIterator tmp = it.next();
			if (iteratorName.equals(tmp.getIteratorName()))
				return true;
        }

		return false;
	}

    /* Stores information about dynamically created iterators.
     * For example, from a Construct Coverage expression.
     */
    public void addDynamicIterator(CoverageIterator i)
    {
        dynamicIterators.add(i);
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

        for (int i = 0; i < dynamicIterators.size(); ++i)
		{
			if (dynamicIterators.get(i).getIteratorName().equals(iteratorName))
			{
				return dynamicIterators.get(i).getCoverages();
			}
		}

		throw new WCPSException("Iterator " + iteratorName + " not defined");
	}

    public boolean isDynamicCoverage(String coverageName)
    {
        for (int i = 0; i < dynamicIterators.size(); ++i)
		{
			Iterator<String> iterator =
                    ((CoverageIterator)dynamicIterators.get(i)).getCoverages();
            while (iterator.hasNext())
			{
                if (iterator.next().equals(coverageName))
                    return true;
			}
		}

        return false;
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

    public IDynamicMetadataSource getMetadataSource()
	{
		return meta;
	}
}
