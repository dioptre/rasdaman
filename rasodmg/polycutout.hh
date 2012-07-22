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
 * INCLUDE: polycutout.hh
 *
 * MODULE:  rasodmg
 * CLASS:   r_PolygonCutOut
 *
 * COMMENTS:
 *
 * This class is intended to perform the polygon cut out operation
 * Except for the r_PolygonCutOut class, everything else is just
 * for internal use
 * Attention: this version works good (I hope :-) but has just a tiny geometrical problem:
 *            as usual in discrete space, lines are build up by little horiontal segments.
 *        Edges are considered to be inside, except the first little segment of a line.
 *        This is only for avoiding a bigger problem, I will develop a better algorythm
*/

/*
  This module is intended to perform polygon cut out operation. It supports the exact ESRI specifications
  regarding "clean" polygons, this means: not self intersecting, closed, if you walk from an point to the
  next of edge the inside is on the right. It also supports multiple rings, but the outer one has to have
  the "inside" inside.
  Don't forget that the rasdaman coordinates are with x left->right and y top->down, or at least
  I have considered them so because of the usual coordinates of tiffs
*/

#ifndef _R_POLYGON_CUT_OUT_HH
#define _R_POLYGON_CUT_OUT_HH

#include <iosfwd>
#include <vector>
#include <list>
#include <string>
using std::string;
using std::list;
using std::vector;
using std::ostream;

#include "raslib/point.hh"
#include "raslib/minterval.hh"
#include "rasodmg/polygon.hh"


//@ManMemo: Module: {\bf rasodmg}


/**
  * \ingroup Rasodmgs
  */
class r_SegmentIterator
{
public:
    r_SegmentIterator(r_Point&,r_Point&);
    void reset();
    r_Point next();
    bool hasMore();
    int  cosFunc(); // limited use,1000 * cos(alfa)
private:
    void  swap(r_Range&, r_Range&);
    r_Point createCurrentPoint();

    r_Point start;
    r_Point end;
    int cadran;

    r_Range dx,dy;
    r_Range cx,cy;
    int   beta;
};

/**
  * \ingroup Rasodmgs
  */
class r_Line
{
public:
    r_Line();
    r_Line(double,double,double);
    r_Line(r_Point&,r_Point&);
    double getA();
    double getB();
    double getC();
    float ecuatia(r_Point&);
private:
    double a,b,c;
    friend ostream& operator<<(ostream&,r_Line&);
};


/**
  * \ingroup Rasodmgs
  */
class r_PolygonCutOut
{
public:
    r_PolygonCutOut();
    ~r_PolygonCutOut();
    void setImageSize(r_Range width, r_Range height);

    void setMArray(r_GMarray& myArray);
    void addPolygon(const r_Polygon&);

    bool fillMArrayInside(const string& bgr = "") throw(r_Error);
    bool fillMArrayOutside(const string& bgr = "") throw(r_Error);

    // just for debugging
    void print(int onlyLine=-1);
    void printLine(r_Range line);

private:
    bool compute();
    void eraseLine( r_Range, r_Range, r_Range y, const string& bgr ) throw(r_Error);

    r_Range imgWidth,imgHeight;
    r_Range imgX,imgY; // - the origin of the mdd domain

    r_GMarray *mArray;

    std::list<r_Polygon> polygons;

    struct TablePoint
    {
        r_Range x;
        int   inside; // where the inside is, -1 left, +1 right, 0 hor. line
        int   cosFunc;
        bool operator==(TablePoint&);
    };

    r_Range tableWidth;
    r_Range tableHeight;
    TablePoint *table;
    int        *usedCount;
    TablePoint& getTP(r_Range line, r_Range column);

    bool initTable();
    void clearTables();
    int  computeTableWidth();


    int  computeInside(r_Point start, r_Point end);
    void computeOneSegment(r_Point start, r_Point end, int inside);
    void computeOneHorSegment(r_Point start, r_Point end);
    void ordonateLine(int line);
    void minimizeLine(int line);
    void replacePoint(r_Range line,r_Range col,int inside, int cosFunc);
    void addPoint(r_Range line,r_Range col,int inside, int cosFunc);

};

#endif
