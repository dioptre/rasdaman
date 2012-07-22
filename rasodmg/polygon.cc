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
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
/
/**
 * SOURCE: polygon.cc
 *
 * MODULE: rasodmg
 * CLASS:  r_Polygon
 *
 * PURPOSE:
 *  This class maintains 2-D polygon sequences.
 *  It knows about them being closed or not.
 *  Input syntax (see constructor) is:
 *      polygon ::= point+
 *      point   ::= '[' int ',' int ']'
 *      int ::= ASCII-integer
 *
 * COMMENTS:
 *      - see comment in shrinkPoly() about a potential error source
 *      - r_Error::r_Error_General should be replaced with more specific exception
 *
*/

#include "rasodmg/polygon.hh"

#include <set>
#include <algorithm>
#include <math.h>

#if defined(SOLARIS)
#include <strings.h>
#endif

using std::sort;
//causes problems compiling on old red hat
//using std::iterator;

#include "raslib/miter.hh"
#include "rasodmg/marray.hh"

#include "debug/debug.hh"

static const char rcsid[] = "@(#)rasodmg, r_Polygon: $Header: /home/rasdev/CVS-repository/rasdaman/rasodmg/polygon.cc,v 1.28 2003/12/27 23:02:56 rasdev Exp $";

#ifndef LONG_MAX
const int LONG_MAX = (1<<31) - 1;
#endif
#ifndef LONG_MIN
const int LONG_MIN = (1<<31);  // due to overflow
#endif

// ------------------------------------------------------------------
// r_Edge
// ------------------------------------------------------------------

r_Edge::r_Edge(const r_Point& newStart, const r_Point& newEnd) :
    start(newStart), end(newEnd)
{
}

const r_Point&
r_Edge::getStart() const
{
    return start;
}

const r_Point&
r_Edge::getEnd() const
{
    return end;
}

double
r_Edge::getInvSlope() const
{
    return (((double)end[0] - start[0]) / (end[1] - start[1]));
}

double
r_Edge::getSlope() const
{
    return (((double)end[1] - start[1]) / (end[0] - start[0]));
}

double
r_Edge::getCurrX(r_Range y) const
{
    double currX=0.0;
    if(end[1]==start[1])
        currX=end[0];
    else
        currX=getInvSlope()*(y - start[1]) + start[0];
    return currX;
}

double
r_Edge::getCurrY(r_Range x) const
{
    double currY=0.0;
    if(end[0]==start[0])
        currY=end[1];
    else
        currY=getSlope()*(x - start[0]) + start[1];
    return currY;
}

void
r_Edge::print_status( std::ostream& s ) const
{
    start.print_status(s);
    s << "->";
    end.print_status(s);
}


bool
r_Edge::isHorizontal() const
{
    return start[1] == end[1] ? true:false;
}

// ------------------------------------------------------------------
// r_Polygon
// ------------------------------------------------------------------

const r_Dimension r_Polygon::polyPointDim=2;

r_Polygon::r_Polygon(const char* init) throw (r_Error)
    :   closed(false),
        firstPointSet(false)
{
    ENTER( "r_Polygon::r_Polygon, init=" << init );

    if (init ==  NULL)
    {
        TALK( "r_Polygon::r_Polygon(" << (init?init: "NULL") << ")" );
        RMInit::logOut << "r_Polygon::r_Polygon(" << (init?init: "NULL") << ")" << std::endl;
        throw r_Error(POLYGONWRONGINITSTRING);
    }
    const int POINTBUFFERLEN=512;
    const char* endPos = NULL;
    size_t pointLen = 0;
    char pointBuffer[POINTBUFFERLEN];
    const char* startPos = index(init, '[');
    if (startPos == NULL)
    {
        TALK( "r_Polygon::r_Polygon(" << init << ") the init string has to start with a '['" );
        RMInit::logOut << "r_Polygon::r_Polygon(" << init << ") the init string has to start with a '['" << std::endl;
        throw r_Error(POLYGONWRONGINITSTRING);
    }

    // while (true)
    do
    {
        endPos = index(startPos, ']');
        if (endPos == NULL)
        {
            TALK( "r_Polygon::r_Polygon(" << init << ") the init string has to contain valid r_Point definitions" );
            RMInit::logOut << "r_Polygon::r_Polygon(" << init << ") the init string has to contain valid r_Point definitions" << std::endl;
            throw r_Error(POLYGONWRONGINITSTRING);
        }
        pointLen = endPos - startPos;
        if (pointLen >= POINTBUFFERLEN)
        {
            TALK( "r_Polygon::r_Polygon(" << init << ") the definition of one r_Point is too long, only 2 dimensions are allowed" );
            RMInit::logOut << "r_Polygon::r_Polygon(" << init << ") the definition of one r_Point is too long, only 2 dimensions are allowed" << std::endl;
            throw r_Error(POLYGONWRONGINITSTRING);
        }
        memset(pointBuffer, 0, POINTBUFFERLEN);
        strncpy(pointBuffer, startPos, pointLen + 1);
        addPoint(r_Point(pointBuffer));
        startPos = index(endPos, '[');
        // was an endless loop with break, changed it to a 'nice' loop -- PB 2003-sep-12
        // if (startPos == NULL)
        //  break;
    }
    while( startPos != NULL);

    LEAVE( "r_Polygon::r_Polygon" );
}

r_Polygon::r_Polygon(r_Range x, r_Range y) : closed(false), firstPointSet(true)
{
    firstPoint = r_Point(x, y);
    currPoint = firstPoint;
}

r_Polygon::r_Polygon() : closed(false), firstPointSet(false)
{
}

r_Polygon::r_Polygon(const r_Polygon& old)
{
    closed=old.closed;
    firstPointSet=old.firstPointSet;
    firstPoint=old.firstPoint;
    currPoint=old.currPoint;

    edges=old.edges;
}

r_Polygon&
r_Polygon::operator=(const r_Polygon& old)
{
    if(this!=&old)
    {
        closed=old.closed;
        firstPointSet=old.firstPointSet;
        firstPoint=old.firstPoint;
        currPoint=old.currPoint;

        edges=old.edges;
    }
    return *this;
}

void
r_Polygon::addPoint(const r_Point& newPoint) throw(r_Error)
{
    if (newPoint.dimension() != polyPointDim)
    {
        RMInit::logOut << "r_Polygon::addPoint(" << newPoint << ") only " << polyPointDim << " dimensional r_Points allowed" << std::endl;
        throw r_Error(POLYGONWRONGPOINTDIMENSION);
    }

    if(closed)
    {
        RMInit::logOut << "r_Polygon::addPoint(" << newPoint << ") polygon closed" << std::endl;
        throw r_Error(r_Error::r_Error_General);
    }

    if(firstPointSet)
    {
        // add an edge from currentPoint to newPoint
        edges.push_back(r_Edge(currPoint, newPoint));
        currPoint = newPoint;

        //check if we have the first point
        checkFistPoint();
    }
    else
    {
        firstPoint = newPoint;
        currPoint = newPoint;
        firstPointSet = true;
    }
}

void
r_Polygon::addPointXY(r_Range x, r_Range y) throw(r_Error)
{
    r_Point newPoint(x, y);
    addPoint(newPoint);
}

void
r_Polygon::close()
{
    if (closed)
        return;

    // add the final edge from currentPoint to firstPoint
    edges.push_back(r_Edge(currPoint, firstPoint));
    closed = true;
}

const std::vector<r_Edge>&
r_Polygon::getEdges() const throw(r_Error)
{
    // if the polygon is not closed we raise an exception.
    if(!closed)
    {
        // TO DO: This should be an internal error sometimes.
        RMInit::logOut << "r_Polygon::getEdges() polygon opened" << std::endl;
        throw(r_Error(r_Error::r_Error_General));
    }

    return edges;
}

std::vector<r_Point>
r_Polygon::getPoints() const throw(r_Error)
{
    if(!closed)
    {
        // TO DO: This should be an internal error sometimes.
        RMInit::logOut << "r_Polygon::getPoints() polygon opened" << std::endl;
        throw(r_Error(r_Error::r_Error_General));
    }

    std::vector<r_Point> retVal;
    std::vector<r_Edge>::const_iterator iter, iterEnd;
    for(iter = edges.begin(), iterEnd=edges.end(); iter != iterEnd; ++iter)
    {
        retVal.push_back((*iter).getStart());
    }
    return retVal;
}

r_Polygon::r_Polygon_Type
r_Polygon::detectPolygonType() const throw(r_Error)
{
    const unsigned int minNoEdges=3;
    std::vector<r_Point> points=getPoints();
    unsigned int i=0,j=0, k=0, n=0;
    unsigned int flag = 0;
    double z;

    n=points.size();

    //check if is at least a  triangle
    if (n < minNoEdges)
        return r_Polygon::UNKNOWN;

    //check if is a polygon in 2D
    for (i=0; i<n; i++)
        if (points[i].dimension()!=polyPointDim)
            return r_Polygon::UNKNOWN;

    //check sign of vertice
    for (i=0; i<n; i++)
    {
        j = (i + 1) % n;
        k = (i + 2) % n;
        z  = (points[j][1] - points[i][1]) * (points[k][0] - points[j][0]);
        z -= (points[j][0] - points[i][0]) * (points[k][1] - points[j][1]);
        if (z < 0)
            flag |= 1;
        else if (z > 0)
            flag |= 2;
        if (flag == 3)
            return r_Polygon::CONCAVE;
    }

    if (flag != 0)
        return r_Polygon::CONVEX;
    else
        return r_Polygon::UNKNOWN;
}

void
r_Polygon::checkFistPoint()
{
    if (firstPoint == currPoint)
        closed = true;
    else
        closed = false;
}

void
r_Polygon::print_status( std::ostream& s ) const
{

    std::vector<r_Edge>::const_iterator iter = edges.begin();
    std::vector<r_Edge>::const_iterator iterEnd = edges.end();
    s << "{";
    while (iter!=iterEnd)
    {
        iter->print_status(s);
        s << ", ";
        ++iter;
    }
    s << "} ";
    if (closed)
        s << "closed";
    else //does not matter, because open will crash ...
        s << "opened";

}

std::ostream& operator<<( std::ostream& s, const r_Polygon& d )
{
    d.print_status( s );
    return s;
}

void
r_Polygon::fillMArray( r_GMarray& myArray, bool fillInside, const std::string& bgr ) const throw(r_Error)
{
    if(!closed)
    {
        // TO DO: This should be an internal error sometimes.
        RMInit::logOut << "r_Polygon::fillMArray(...) polygon opened" << std::endl;
        throw(r_Error(r_Error::r_Error_General));
    }

    // This code currently is not optimised. For every scanline all edges are checked.
    // Normally you would keep a list of currently relevant edges and manage deletes
    // and additions to this table by presorting. Anyway, focus was on correctnes
    // for the time being and even that was not really easy.

    // all edges of the poly except for horizontal ones.
    std::set<r_Edge, EdgeSortCriterion> sortedEdges;
    // the X values of the edges in the current scanline.
    std::vector<double> currXVals;
    // just to save some typing.
    r_Minterval myDom = myArray.spatial_domain();
    unsigned long typeSize = myArray.get_type_length();

    // build sortedEdges (sorted after startPoint y, startPoint x).
    std::vector<r_Edge>::const_iterator iter, iterEnd;
    for(iter = edges.begin(), iterEnd=edges.end(); iter != iterEnd; ++iter)
        sortedEdges.insert(*iter);

    // now the actual filling is done. Remember that we only draw the outside!
    // we iterate through the whole y range
    for(r_Range y = myDom[1].low(); y <= myDom[1].high(); y++)
    {
        // update the currXVals by iterating through all edges.
        for(std::multiset<r_Edge, EdgeSortCriterion>::const_iterator itera = sortedEdges.begin(); itera != sortedEdges.end(); itera++)
        {
            if( (*itera).getStart()[1] <= y && y <= (*itera).getEnd()[1] ||
                    (*itera).getEnd()[1] <= y && y <= (*itera).getStart()[1] )
            {
                currXVals.push_back((*itera).getCurrX(y));
            }
        }
        // sort them.
        sort(currXVals.begin(), currXVals.end());
        // currently we can only draw concave polygons anyway (see below).
        // So this is heavily simplified! Check version 1.3 for a
        // blueprint of how it should look like.
        if(fillInside)
        {
            if(currXVals.size() >= 1)
            {
                // currXVals is sorted, so just draw from the first to the last.
                eraseLine(rint(currXVals.front()), rint(currXVals.back()), y, myArray, bgr);
            }
        }
        else
        {
            // currXVals is sorted, so just draw from low to the first and
            // from the last to high.
            // This only works correctly if the polygon is clipped
            // to the area of the image.
            if(currXVals.size() >= 1)
            {
                eraseLine(myDom[0].low(), rint(currXVals.front())-1.0, y, myArray, bgr);
                eraseLine(rint(currXVals.back())+1.0, myDom[0].high(), y, myArray, bgr);
            }
            else
            {
                eraseLine(myDom[0].low(), myDom[0].high(), y, myArray, bgr);
            }
        }
        // Note:
        // Couldn't get it working with an even/odd inside rule. Reason: If you
        // want to do this correctly you have to classify two edges meeting into
        // different categories to decide if they have to be put into currXVals
        // once or twice. Otherwise you get problems. With this simplified version
        // only convex polygons are filled correctly!

        // delete the old currXVals
        currXVals.clear();
    }
}

r_Minterval
r_Polygon::getBoundingBox() const throw(r_Error)
{
    if(!closed)
    {
        // TO DO: This should be an internal error sometimes.
        TALK( "r_Polygon::getBoundingBox() polygon opened" );
        RMInit::logOut << "r_Polygon::getBoundingBox() polygon opened" << std::endl;
        throw(r_Error(r_Error::r_Error_General));
    }

    r_Minterval retVal(2);
    r_Range minX = LONG_MAX;
    r_Range maxX = LONG_MIN;
    r_Range minY = LONG_MAX;
    r_Range maxY = LONG_MIN;
    r_Range currMinX=0, currMaxX=0, currMinY=0, currMaxY=0;
    std::vector<r_Edge>::const_iterator iter, iterEnd;

    for(iter = edges.begin(), iterEnd=edges.end(); iter != iterEnd; ++iter)
    {
        currMinX = (*iter).getStart()[0] < (*iter).getEnd()[0] ? (*iter).getStart()[0] : (*iter).getEnd()[0];
        currMaxX = (*iter).getStart()[0] > (*iter).getEnd()[0] ? (*iter).getStart()[0] : (*iter).getEnd()[0];
        currMinY = (*iter).getStart()[1] < (*iter).getEnd()[1] ? (*iter).getStart()[1] : (*iter).getEnd()[1];
        currMaxY = (*iter).getStart()[1] > (*iter).getEnd()[1] ? (*iter).getStart()[1] : (*iter).getEnd()[1];
        minX = currMinX < minX ? currMinX : minX;
        maxX = currMaxX > maxX ? currMaxX : maxX;
        minY = currMinY < minY ? currMinY : minY;
        maxY = currMaxY > maxY ? currMaxY : maxY;
    }
    retVal << r_Sinterval(minX, maxX) << r_Sinterval(minY, maxY);
    return retVal;
}

void
r_Polygon::clip(const r_Minterval& clipDom) throw(r_Error)
{
    if(!closed)
    {
        // TO DO: This should be an internal error sometimes.
        TALK( "r_Polygon::getBoundingBox() polygon opened" );
        RMInit::logOut << "r_Polygon::getBoundingBox() polygon opened" << std::endl;
        throw(r_Error(r_Error::r_Error_General));
    }

    std::vector<r_Point> pointList;

    // Disjunct polygons used to crash the program.
    // check if the bounding box of the polygon overlaps the clipDom
    if(clipDom.intersects_with(getBoundingBox()))
    {
        // We just clip all 4 edges
        for(int s = r_Polygon::top; s <= r_Polygon::right; ++s)
        {
            pointList=clip1Side(clipDom, (r_Polygon::Side)s);
            if(pointList.empty()) // do we have intersection points?
            {
                // return a polygon with one line only. This should delete everything.
                pointList.push_back(r_Point(clipDom[0].low(), clipDom[1].low()));
                pointList.push_back(r_Point(clipDom[0].low(), clipDom[1].high()));
            }
            fromPoints(pointList);
            pointList.clear();
        }
    }
    else
    {
        // return a polygon with one line only. This should delete everything.
        pointList.push_back(r_Point(clipDom[0].low(), clipDom[1].low()));
        pointList.push_back(r_Point(clipDom[0].low(), clipDom[1].high()));
        fromPoints(pointList);
    }
}


void
r_Polygon::scale(const r_Point& origin, const r_Minterval& mddDom,
                 const r_Minterval& clipDom, const double& scaleFactor) throw(r_Error)
{
    r_Dimension dim = origin.dimension();
    std::vector<r_Point> oldPoints = getPoints();
    std::vector<r_Point>::const_iterator iter = oldPoints.begin();
    std::vector<r_Point>::const_iterator iterEnd = oldPoints.end();
    std::vector<r_Point> newPoints;
    r_Point tmpPoint(dim);
    r_Range coord=0;

    // iterate through all points
    while( iter != iterEnd )
    {
        // currently only 2-D, but who knows
        for (int i=0; i<dim; i++)
        {
            coord = (*iter)[i];
            // This is yet another copy of the code in tile.cc (another one is
            // in fastscale.cc). Hope it is exact enough if I do not do the
            // correction for seamless tiling as done in Tile::getScaleDomain().
            coord = (r_Range)(origin[i] + floor((coord - origin[i]) * scaleFactor));
            // This domain thing is driving me crazy. Ok, now we still have to
            // shift the domain so that it coincides with the origin of the
            // MInterval used for clipping later (i.e. the domain of the MDD
            // object which will be later filled using the polygon. See
            // fastscale.cc, r_Fast_Scale<T>::get_scaled_image().
            coord = coord + mddDom[i].high() - clipDom[i].high();
            tmpPoint[i] = coord;
        }
        newPoints.push_back(tmpPoint);
        ++iter;
    }
    fromPoints(newPoints);
}


void
r_Polygon::scale(const r_Point& origin, const double& scaleFactor) throw(r_Error)
{
    r_Dimension dim = origin.dimension();
    std::vector<r_Point> oldPoints = getPoints();
    std::vector<r_Point>::const_iterator iter = oldPoints.begin();
    std::vector<r_Point>::const_iterator iterEnd = oldPoints.end();
    std::vector<r_Point> newPoints;
    r_Point tmpPoint(dim);
    r_Range coord=0;

    //std::cout << "Polygon bounding box " << getBoundingBox() << std::endl;

    // iterate through all points
    while( iter != iterEnd )
    {
        // currently only 2-D, but who knows
        for (int i=0; i<dim; i++)
        {
            coord = (*iter)[i];
            // scaling is done in this way:
            // translate to 0, scale and translate back
            coord = origin[i]+(r_Range)floor((coord-origin[i]) * scaleFactor);
            tmpPoint[i] = coord;
        }
        newPoints.push_back(tmpPoint);
        ++iter;
    }
    fromPoints(newPoints);

    //std::cout << "Polygon bounding box " << getBoundingBox() << std::endl;
}

void
r_Polygon::mirror(const r_Minterval& mddDom) throw(r_Error)
{
    r_Dimension dim = mddDom.dimension();
    std::vector<r_Point> oldPoints = getPoints();
    std::vector<r_Point>::const_iterator iter = oldPoints.begin();
    std::vector<r_Point>::const_iterator iterEnd = oldPoints.end();
    std::vector<r_Point> newPoints;
    r_Point tmpPoint(dim);
    r_Range y=0;

    // iterate through all points
    while( iter != iterEnd )
    {
        y = mddDom[1].high() - (*iter)[1];
        tmpPoint = (*iter);
        tmpPoint[1] = y;
        newPoints.push_back(tmpPoint);
        ++iter;
    }
    fromPoints(newPoints);
}

void
r_Polygon::fromPoints(const std::vector<r_Point>& newPoints) throw(r_Error)
{
    std::vector<r_Point>::const_iterator iter, iterEnd;

    if(newPoints.empty())
    {
        TALK( "r_Polygon::fromPoinst(....) newPoints is empty" );
        RMInit::logOut << "r_Polygon::fromPoinst(....) newPoints is empty" << std::endl;
        throw r_Error(r_Error::r_Error_General);
    }

    iter = newPoints.begin();
    iterEnd = newPoints.end();
    edges.clear();

    firstPoint = *iter;
    currPoint = *iter;
    while( ++iter != iterEnd )
    {
        edges.push_back(r_Edge(currPoint, *iter));
        currPoint = *iter;
    }
    edges.push_back(r_Edge(currPoint, firstPoint));
    closed = true;
}

void
r_Polygon::eraseLine( r_Range x1, r_Range x2, r_Range y, r_GMarray& myArray, const std::string& bgr ) const throw(r_Error)
{
    // Do nothing in that case (may happen due to rounding problems)
    if(x2 < x1)
        return;

    r_Minterval eraseDom(2);
    eraseDom << r_Sinterval(x1, x2) << r_Sinterval(y, y);
    r_Minterval arrayDom = myArray.spatial_domain();
    r_Bytes typeSize = myArray.get_type_length();
    r_Bytes bgrSize = bgr.size();
    const char *bgrContent=bgr.c_str();
    char *currCell=NULL;


    // Grrr. In RasDaMan the y are stored close together. So the whole fillPolygon
    // should have been organised by x instead of y. Well, for now I just use an
    // r_Miter here.
    r_Miter eraseIter( &eraseDom, &arrayDom, typeSize, myArray.get_array() );
    while( !eraseIter.isDone() )
    {
        currCell = eraseIter.nextCell();
        // FIXME This potentially wont work for all types. I just set every byte to 0.
        if(bgr.empty())
            memset(currCell, 0, typeSize);
        else
        {
            if( typeSize != bgrSize)
                throw r_Error( r_Error::r_Error_TypeInvalid );
            memmove(currCell, bgrContent, bgrSize);
        }
    }
}

std::vector<r_Point>
r_Polygon::clip1Side(const r_Minterval& b, r_Polygon::Side s)
{
    // This routine clips a polygon against one (endless) edge of a bounding box.
    // It is an implementation of the Sutherland-Hodgman algorithm geared more
    // towards readability than efficiency. The algorithm classifies edges into
    // four cases:
    // Case 1: both ends are inside. Then add the end point to the list of points.
    // Case 2: start inside, end outside. And only the intersection of the
    //         bounding box edge and the polygon edge.
    // Case 3: completely outside. Add no points.
    // Case 4: start outside, end inside. Add end and the intersection of the
    //         bounding box edge and the polygon edge.

    std::vector<r_Point> out;
    std::vector<r_Edge>::const_iterator iter, iterEnd;

    // iterate through all edges
    for(iter = edges.begin(),iterEnd=edges.end(); iter != iterEnd; ++iter)
    {
        if(inside(b, (*iter).getEnd(), s))
        {
            if(inside(b, (*iter).getStart(), s))
            {
                // case 1
                out.push_back((*iter).getEnd());
            }
            else
            {
                // case 4
                out.push_back(intersect(b, *iter, s));
                out.push_back((*iter).getEnd());
            }
        }
        else
        {
            if(inside(b, (*iter).getStart(), s))
            {
                // case 2
                out.push_back(intersect(b, *iter, s));
            }
        }
        // do nothing for case 3
    }
    return out;
}

bool
r_Polygon::inside(const r_Minterval& b, const r_Point& p, r_Polygon::Side s)
{
    switch(s)
    {
    case top:
        return p[1] <= b[1].high();
    case bottom:
        return p[1] >= b[1].low();
    case right:
        return p[0] <= b[0].high();
    case left:
        return p[0] >= b[0].low();
    default:
        return false;
    }
}

r_Point
r_Polygon::intersect(const r_Minterval& b, const r_Edge& e, r_Polygon::Side s)
{
    switch(s)
    {
    case top:
        return r_Point( e.getCurrX(b[1].high()), b[1].high() );
    case bottom:
        return r_Point( e.getCurrX(b[1].low()), b[1].low() );
    case right:
        return r_Point( b[0].high(), e.getCurrY(b[0].high()) );
    default: //case left:
        return r_Point( b[0].low(), e.getCurrY(b[0].low()) );
    }
}

r_Point
r_Polygon::getMiddle() const throw(r_Error)
{
    // Note that the summing up done here is a bit risky since overflows
    // give incorrect results.

    r_Point retVal(2);
    double xSum = 0;
    double ySum = 0;
    int pointCount = 0;
    std::vector<r_Point> myPoints = getPoints();
    std::vector<r_Point>::const_iterator iter = myPoints.begin();
    std::vector<r_Point>::const_iterator iterEnd = myPoints.end();

    while( iter != iterEnd )
    {
        ySum += (*iter)[1];
        xSum += (*iter)[0];
        ++pointCount;
        ++iter;
    }
    retVal[0] = rint((xSum / pointCount));
    retVal[1] = rint((ySum / pointCount));

    return retVal;
}

void
r_Polygon::shrinkPoly(int pixelCount) throw(r_Error)
{
    // Ok now, we move all points towards the middle. Since we store edges we
    // have to use this somewhat clumsy form with using points in between.
    // Note that there is quite a bit of potential for error (e.g. points
    // coinciding after moving them), Anyway, this was programmed as a quick
    // hack for a problem at BLVA.
    r_Point middle = getMiddle();
    r_Dimension dim = middle.dimension();
    std::vector<r_Point> oldPoints = getPoints();
    std::vector<r_Point>::const_iterator iter = oldPoints.begin();
    std::vector<r_Point>::const_iterator iterEnd = oldPoints.end();
    std::vector<r_Point> newPoints;
    r_Point tmpPoint(dim);
    r_Range coord=0;

    // iterate through all points
    while( iter != iterEnd )
    {
        // currently only 2-D, but who knows
        for (int i=0; i<dim; i++)
        {
            coord = (*iter)[i];
            if(coord > middle[i])
            {
                coord = coord - pixelCount;
            }
            else
            {
                if(coord < middle[i])
                {
                    coord = coord + pixelCount;
                }
            }
            tmpPoint[i] = coord;
        }
        newPoints.push_back(tmpPoint);
        ++iter;
    }
    fromPoints(newPoints);
}


int
r_Polygon::countEdges() const
{
    return edges.size();
}

int
r_Polygon::countHorizontalEdges() const
{
    int counter = 0;

    std::vector<r_Edge>::const_iterator iter = edges.begin();

    for(int i=0; i<edges.size(); i++,iter++)
    {
        counter += iter->isHorizontal() ? 1:0;
    }
    return counter;
}

