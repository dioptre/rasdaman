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
 * SOURCE: polycutout.cc
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
 *            Edges are considered to be inside, except the first little segment of a line.
 *            This is only for avoiding a bigger problem, I will develop a better algorythm
*/

/*
This module is intended to perform polygon cut out operation. It supports the exact ESRI specifications
regarding "clean" polygons, this means: not self intersecting, closed, if you walk from an point to the
next of edge the inside is on the right. It also supports multiple rings, but the outer one has to have
the "inside" inside.
Don't forget that the rasdaman coordinates are with x left->right and y top->down, or at least
I have considered them so because of the usual coordinates of tiffs
*/

static const char rcsid[] = "@(#)rasodmg, r_PolygonCutOut: $Id: polycutout.cc,v 1.11 2002/10/09 09:58:05 hoefner Exp $";

#include "rasodmg/polycutout.hh"
#include "rasodmg/marray.hh"
#include "raslib/miter.hh"

#include <math.h>

r_SegmentIterator::r_SegmentIterator(r_Point &s,r_Point &e)
{
    start = s;
    end   = e;
    reset();
}

void r_SegmentIterator::reset()
{
    dx = end[0] - start[0];
    dy = end[1] - start[1];

    if( dx >= 0)
    {
        if( dy >= 0 )
        {
            if( dx >= dy )
            {
                cadran = 0;
            }
            else
            {
                cadran = 1;
                swap(dx,dy);
            }
        }
        else
        {
            dy = -dy;
            if( dx >= dy )
            {
                cadran = 7;
            }
            else
            {
                cadran = 6;
                swap(dx,dy);
            }
        }
    }
    else
    {
        dx = -dx;
        if( dy >= 0 )
        {
            if( dx >= dy )
            {
                cadran = 3;
            }
            else
            {
                cadran = 2;
                swap(dx,dy);
            }
        }
        else
        {
            dy = -dy;
            if( dx >= dy )
            {
                cadran = 4;
            }
            else
            {
                cadran = 5;
                swap(dx,dy);
            }
        }
    }
    cx = 0;
    cy = 0;
    beta = 0;
//     std::cout<<"reset: ["<<start<<','<<end<<"] cadran="<<cadran<<" dx="<<dx<<" dy="<<dy<<std::endl;
}

r_Point r_SegmentIterator::next()
{
    r_Point resultat = createCurrentPoint();
    //prepare next
    cx++;
    beta += dy;
    if( (beta<<1) > dx )
    {
        beta -= dx;
        cy++;
    }
    return resultat;
}

bool r_SegmentIterator::hasMore()
{
//    std::cout<<"hasMore: cx="<<cx<<" lim="<<dx+1<<std::endl;
    return (cx <= dx ) ? true : false;
}

int  r_SegmentIterator::cosFunc() // limited use,1000 * cos(alfa)
{
    // use local variables
    double ldx = end[0] - start[0];
    double ldy = end[1] - start[1];

    double num = ldx*ldx + ldy*ldy;
    if(num == 0) return 0;

    return (1000 * ldx)/sqrt(num);
}

void r_SegmentIterator::swap(r_Range &x, r_Range &y)
{
    r_Range temp = x;
    x=y;
    y=temp;
}

r_Point r_SegmentIterator::createCurrentPoint()
{
//    std::cout<<" cCP: "<<cx<<':'<<cy<<std::endl;
    r_Range rcx = cx;
    r_Range rcy = cy;
    switch(cadran)
    {
    case 0:
        break;
    case 1:
        swap(rcx,rcy);
        break;
    case 2:
        swap(rcx,rcy);
        rcx=-rcx;
        break;
    case 3:
        rcx=-rcx;
        break;
    case 4:
        rcx=-rcx;
        rcy=-rcy;
        break;
    case 5:
        swap(rcx,rcy);
        rcx=-rcx;
        rcy=-rcy;
        break;
    case 6:
        swap(rcx,rcy);
        rcy=-rcy;
        break;
    case 7:
        rcy=-rcy;
        break;
    }
    //std::cout<<"cCP: cx="<<cx<<" cy="<<cy<<" rx="<<rcx<<" ry="<<rcy<<" cadran="<<cadran<<std::endl;
    return r_Point(start[0] + rcx, start[1] + rcy);
}



r_Line::r_Line()
{
    a = b = c = 0;
}

r_Line::r_Line(double nA,double nB,double nC)
{
    a = nA;
    b = nB;
    c = nC;
}

r_Line::r_Line(r_Point &s,r_Point &e)
{
    a = e[1] - s[1];
    b = s[0] - e[0];
    c = (double)e[0]*(double)s[1] - (double)s[0]*(double)e[1];
}

double r_Line::getA()
{
    return a;
}
double r_Line::getB()
{
    return b;
}
double r_Line::getC()
{
    return c;
}

float r_Line::ecuatia(r_Point &p)
{
    return a * p[0] + b * p[1] + c;
}

ostream& operator<<(ostream& os,r_Line &l)
{
    return os<<"{L"<<l.a<<','<<l.b<<','<<l.c<<'}';
}




r_PolygonCutOut::r_PolygonCutOut()
{
    table     = 0;
    usedCount = 0;
    mArray    = 0;

    imgWidth    = 0;
    imgHeight   = 0;
    imgX = 0;
    imgY = 0;
    tableWidth  = 0;
    tableHeight = 0;
}
r_PolygonCutOut::~r_PolygonCutOut()
{
    clearTables();
}

void r_PolygonCutOut::setMArray(r_GMarray& myArray)
{
    mArray = &myArray;

    r_Minterval currDomain;
    currDomain  = mArray->spatial_domain();

    imgX = currDomain[0].low();
    imgY = currDomain[1].low();

    imgWidth  = currDomain[0].get_extent();
    imgHeight = currDomain[1].get_extent();
}

void r_PolygonCutOut::addPolygon(const r_Polygon &p)
{
    polygons.push_back(p);
}

bool r_PolygonCutOut::compute()
{
//std::cout<<"PCO: compute in"<<std::endl;
    if(!imgWidth || !imgHeight || polygons.size()==0) return false;

//std::cout<<"PCO: initTable - in"<<std::endl;
    initTable();
//std::cout<<"PCO: initTable - out"<<std::endl;

    list<r_Polygon>::iterator polyIter = polygons.begin();

    for(int i = 0; i < polygons.size(); i++, polyIter++)
    {
        r_Polygon &currPolygon = *polyIter;

        const std::vector<r_Edge>& edges    = currPolygon.getEdges();
        std::vector<r_Edge>::const_iterator edgeIterator = edges.begin();

        for(int j = 0; j < edges.size(); j++,edgeIterator++)
        {
            r_Point start = edgeIterator->getStart();
            r_Point end   = edgeIterator->getEnd();

            if(start == end) continue; //avoid such edges, causes problems

            int inside = -computeInside(start,end); // minus because rasdaman coord are flipped
            // we have to see if this is correct like this or not

            //std::cout<<"compute edge "<<j<<" ["<<start<<"-"<<end<<"] inside="<<inside<<std::endl;
            if(inside == 0 ) computeOneHorSegment(start,end);
            else             computeOneSegment(start,end,inside);
        }
    }
    for(int line=0; line<tableHeight; line++)
    {
        ordonateLine(line);
        minimizeLine(line);
    }
//print();
//    std::cout<<"PCO: compute out"<<std::endl;
    return true;
}

bool r_PolygonCutOut::initTable()
{
    clearTables();
    tableWidth = computeTableWidth();
    tableHeight= imgHeight;

//std::cout<<"PCO: initTable: "<<tableWidth<<','<<tableHeight<<std::endl;
    table = new TablePoint[tableWidth*tableHeight];
    usedCount = new int[tableHeight];

    // if(table==NULL || usedCount==NULL) std::cout<<"sunt nule"<<std::endl;

    for(int i=0; i<tableWidth*tableHeight; i++)
    {
        table[i].x =-1;
        table[i].inside= 0;
        table[i].cosFunc= 0;
    }

    for(int j=0; j<tableHeight; j++)
        usedCount[j]=0;

    return true;
}

void r_PolygonCutOut::clearTables()
{
    if(table) delete[] table;
    table = NULL;
    if(usedCount) delete[] usedCount;
    usedCount =NULL;
}
int r_PolygonCutOut::computeTableWidth()
{
    // once it was: tableWidth = poly->getCountNodes() + poly->countHorizontalEdges() + 2;

    list<r_Polygon>::iterator iter=polygons.begin();

    int nodes      = 0;
    int horizEdges = 0;

    for(int i=0; i<polygons.size(); i++, iter++)
    {
        r_Polygon &polygon = *iter;

        nodes      += polygon.countEdges();
        horizEdges += polygon.countHorizontalEdges();
    }
    return nodes + horizEdges; // niet + 2;
}

void r_PolygonCutOut::ordonateLine(int line)
{
    int count=usedCount[line];
    TablePoint *tLine = &getTP(line,0);
    bool change=false;
    do
    {
        change=false;
        for(int i=0; i<count-1; i++)
        {
            bool swap=false;

            if(tLine[i].x > tLine[i+1].x) swap=true;
            if(tLine[i].x == tLine[i+1].x &&
                    tLine[i].inside != tLine[i+1].inside &&
                    tLine[i].cosFunc > tLine[i+1].cosFunc)
                swap=true;
            if(swap)
            {
                TablePoint temp = tLine[i];
                tLine[i]   = tLine[i+1];
                tLine[i+1] = temp;
                change = true;
            }
        }
    }
    while(change);

    for(int j=0; j<count-1; j++)
    {
        if(tLine[j] == tLine[j+1])
            tLine[j].inside=0;
    }

}

void r_PolygonCutOut::minimizeLine(int line)
{
    int count=usedCount[line];
    if(count <= 2) return; //nothing to do

    int inside = 0;
    TablePoint *tLine = &getTP(line,0);

    int level=0;
    for(int i=0; i<count-1; i++)
    {
        if(level==0)
        {
            inside=tLine[i].inside;
            level++;
            continue;
        }

        if(tLine[i].inside==inside) level++;
        else                        level--;

        if(level!=0) tLine[i].inside=0; // means disabled
    }

    int s,d;
    for(s=0,d=0; s<count; s++)
    {
        if(tLine[s].inside==0) continue;
        if(s!=d)  tLine[d]=tLine[s];
        d++;
    }
    usedCount[line]=d;
}

void r_PolygonCutOut::computeOneSegment(r_Point start, r_Point end, int inside)
{
    int lastLine = -1;
    int lastX    = -1;
    r_SegmentIterator segiter(start,end);

    int cosFunc=segiter.cosFunc();
    int cosNow = cosFunc; // first point: cosFunc, lastPoint -cosFunc, rest 0

    bool firstLine = true;
    bool lastPoint = false;
    // this firstLine-lastPoint are here only top be sure that the first and the last point are in the table
    // and not replaced by neighbours
    while(segiter.hasMore())
    {
        r_Point cp = segiter.next();
        if(segiter.hasMore()==false) // last point
        {
            cosNow=-cosFunc;
            lastPoint = true;
        }

        r_Range coordX = cp[0];
        r_Range coordY = cp[1];

        int tableLine = coordY - imgY;

        if(tableLine>=0 && tableLine < tableHeight)
        {
            if(tableLine != lastLine)
            {
                addPoint(tableLine,coordX,inside,cosNow);

                if(lastLine != -1) firstLine = false;
            }
            else if(firstLine == false)
            {
                if(inside > 0 && coordX < lastX)      replacePoint(tableLine,coordX,inside,cosNow);
                else if(inside < 0 && coordX > lastX) replacePoint(tableLine,coordX,inside,cosNow);
                else if(lastPoint)                    replacePoint(tableLine,coordX,inside,cosNow);
            }
        }

        cosNow   = 0;
        lastLine = tableLine;
        lastX    = coordX;
    }
}

void r_PolygonCutOut::computeOneHorSegment(r_Point start, r_Point end)
{
    r_SegmentIterator segiter(start,end);
    int cosFunc=segiter.cosFunc();

    r_Range startX  = start[0];
    r_Range endX    = end[0];
    r_Range commonY = start[1];

    int tableLine =  commonY - imgY;

    if(tableLine>=0 && tableLine < tableHeight)
    {
        if(startX <= endX)
        {
            addPoint(tableLine,startX, 1, cosFunc);
            addPoint(tableLine,endX,  -1,-cosFunc);
        }
        else             // in this case cosFunc < 0
        {
            addPoint(tableLine,endX,   1,-cosFunc);
            addPoint(tableLine,startX,-1, cosFunc);
        }
    }
}

int r_PolygonCutOut::computeInside(r_Point start, r_Point end)
{
    r_Line line(start,end);
    r_Point control(start[0]+1,start[1]);
    float f = line.ecuatia(control);
    if(f==0) return 0;
    if(f<0 ) return -1;
    return 1;
}


r_PolygonCutOut::TablePoint& r_PolygonCutOut::getTP(int line, int column)
{
    TablePoint *tp = table + (line * tableWidth + column);
    return *tp;
}

void r_PolygonCutOut::print(int onlyLine)
{
    std::cout<<"r_PolygonCutOut::print: "<<tableWidth<<':'<<tableHeight<<std::endl;

    if(onlyLine==-1) // print all
    {
        for(int i=0; i<tableHeight; i++)
            printLine(i);
    }
    else
    {
        if(0<=onlyLine && onlyLine <tableHeight) printLine(onlyLine);
        else std::cout<<"Line "<<onlyLine<<" doesn't exist"<<std::endl;
    }
}

void r_PolygonCutOut::printLine(r_Range line)
{
    int count=usedCount[line];
    TablePoint *tp = &getTP(line,0);

    std::cout<<"line "<<line<<'('<<count<<") ";
    for(int i=0; i<count; i++)
    {
        char inside='0';
        if(tp[i].inside==-1) inside='-';
        if(tp[i].inside==1 ) inside='+';

        std::cout<<'['<<tp[i].x<<','<<inside<<','<<tp[i].cosFunc<<']';
    }
    std::cout<<std::endl;
}

void r_PolygonCutOut::addPoint(int tableLine,r_Range coordX, int inside,int cosFunc)
{
    int which = usedCount[tableLine];
    TablePoint *tp = &getTP(tableLine,which);

    tp->x       = coordX;
    tp->inside  = inside;
    tp->cosFunc = cosFunc;
    usedCount[tableLine]++;
}

void r_PolygonCutOut::replacePoint(int tableLine,r_Range coordX,int inside, int cosFunc)
{
    int which = usedCount[tableLine]-1;
    TablePoint *tp = &getTP(tableLine,which);
    tp->x       = coordX;
    tp->inside  = inside;
    tp->cosFunc = cosFunc;
}

bool r_PolygonCutOut::TablePoint::operator==(r_PolygonCutOut::TablePoint &tp)
{
    return (x==tp.x && inside==tp.inside) ? true: false;
}


// debug only int lastline = -1;
void
r_PolygonCutOut::eraseLine( r_Range x1, r_Range x2, r_Range y, const string& bgr ) throw(r_Error)
{
    // can heapen if we are outside the domain of the array
    if(x2 < x1) return;

    //if(lastline != y && x1 != 0)
    //    std::cout<<"Erasing line: y="<<y<<" ["<<x1<<","<<x2<<"]"<<std::endl;
    //lastline = y;

    r_Minterval eraseDom(2);
    eraseDom << r_Sinterval(x1, x2) << r_Sinterval(y, y);
    r_Minterval arrayDom = mArray->spatial_domain();
    r_Bytes typeSize = mArray->get_type_length();
    r_Bytes bgrSize = bgr.size();
    const char *bgrContent=bgr.c_str();
    char *currCell=NULL;


    // Grrr. In RasDaMan the y are stored close together. So the whole fillPolygon
    // should have been organised by x instead of y. Well, for now I just use an
    // r_Miter here.

    r_Miter eraseIter( &eraseDom, &arrayDom, typeSize, mArray->get_array() );
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


bool
r_PolygonCutOut::fillMArrayOutside(const string& bgr ) throw(r_Error)
{
    if( compute() == false ) return false;

    r_Minterval currDomain = mArray->spatial_domain();

    r_Range imgMinX = imgX;
    r_Range imgMaxX = currDomain[0].high();

    for(int lineY = 0; lineY < tableHeight ; lineY++)
    {
        r_Range coordY = imgY + lineY;

        int countUsed = usedCount[lineY];

        if(countUsed==0)
        {
            // polygon didn't touch this line, so all line is outside
            eraseLine(imgMinX, imgMaxX, coordY, bgr);
            continue;
        }

        TablePoint *tp = &getTP(lineY, 0);

        r_Range xLeft  = imgMinX-1;
        r_Range xRight = imgMaxX+1;
        bool erase = false;

        if( tp[countUsed-1].x < imgMinX)
        {
            // the whole polygon is on the left of this line so we have to erase the whole line
            eraseLine( imgMinX, imgMaxX, coordY, bgr);
            continue;
        }

        if( tp[0].x > imgMaxX)
        {
            // the whole polygon is on the right of this line so we have to erase the whole line
            eraseLine( imgMinX, imgMaxX, coordY, bgr);
            continue;
        }

        for(int i=0; i<countUsed; i++, tp++)
        {
            if(tp->x < imgMinX) continue;

            if(tp->inside == -1)
            {
                // inside is <-, so outside is ->
                xLeft = tp->x;
                if(xLeft > imgMaxX) break;

                if(i == countUsed-1)
                {
                    xRight = imgMaxX+1;
                    erase = true;
                }
            }

            if(tp->inside == 1)
            {
                // inside is ->, so outside is <-
                xRight = tp->x > imgMaxX ? imgMaxX+1 : tp->x;
                erase = true;
            }

            if(erase)
            {
                eraseLine( xLeft+1, xRight-1, coordY, bgr);
                erase = false;
            }
        }

    }
    clearTables();
    return true;
}

/*
  Attention, this two functions look very similar, but with have significant differences
  One of them is the absence of some -1 or +1 in the next function. This is important
  because we consider the edges to be "inside", and the edges are in the table, so
  erasing the inside is erase [start,end] and erasing the outside is erase [start+1,end-1]
*/

bool
r_PolygonCutOut::fillMArrayInside(const string& bgr ) throw(r_Error)
{
    if( compute() == false ) return false;

    r_Minterval currDomain = mArray->spatial_domain();

    r_Range imgMinX = imgX;
    r_Range imgMaxX = currDomain[0].high();

    for(int lineY = 0; lineY < tableHeight ; lineY++)
    {
        r_Range coordY = imgY + lineY;

        int countUsed = usedCount[lineY];

        if(countUsed==0)
        {
            // polygon didn't touch this line, so all line is outside
            continue;
        }

        TablePoint *tp = &getTP(lineY, 0);

        r_Range xLeft  = imgMinX;
        r_Range xRight = imgMaxX;
        bool erase = false;

        if( tp[countUsed-1].x < imgMinX)
        {
            // the whole polygon is on the left of this line so we have to erase the whole line
            continue;
        }

        if( tp[0].x > imgMaxX)
        {
            // the whole polygon is on the right of this line so we have to erase the whole line
            continue;
        }

        for(int i=0; i<countUsed; i++, tp++)
        {
            if(tp->x < imgMinX) continue;

            if(tp->inside == 1)
            {
                // inside is ->, so outside is <-
                xLeft = tp->x;
                if(xLeft > imgMaxX) break;

                if(i == countUsed-1)
                {
                    xRight = imgMaxX;
                    erase = true;
                }
            }

            if(tp->inside == -1)
            {
                // inside is <-, so outside is ->
                xRight = tp->x > imgMaxX ? imgMaxX : tp->x;
                erase = true;
            }

            if(erase)
            {
                eraseLine( xLeft, xRight, coordY, bgr);
                erase = false;
            }
        }

    }
    clearTables();
    return true;
}
