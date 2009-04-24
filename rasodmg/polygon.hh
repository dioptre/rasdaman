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
 * INCLUDE: polygon.hh
 *
 * MODULE:  rasodmg
 * CLASS:   r_Polygon
 *
 * COMMENTS:
 *		None
*/

#ifndef _R_POLYGON_
#define _R_POLYGON_

#include <iostream>
#include <vector>
#include <string>

#include "raslib/point.hh"
#include "raslib/minterval.hh"

class r_GMarray;

//@ManMemo: Module: {\bf rasodmg}

/*@Doc:

  The class r_Edge is used to represent edges of 2-D polygons
  represented in class r_Polygon. Edges cannot be modified. This does
  not make sense, as a polygon always has to consist out of connecting
  edges. Modifications should always be done on the polygon.
 
*/

class r_Edge
{
public:
  /// constructor getting a 2-D start and end point.
  r_Edge(const r_Point& newStart, const r_Point& newEnd);

  /// retrieve 2-D start point of edge.
  const r_Point& getStart() const;

  /// retrieve 2-D end point of edge.
  const r_Point& getEnd() const;

  /// calculate inverse slope of the edge. Note: may throw exception due to division by 0.
  double getInvSlope() const;

  /// calculate slope of the edge. Note: may throw exception due to division by 0.
  double getSlope() const;

  /// retrieve x for a given y on a line with the slope of the edge. Calls getInvSlope().
  double getCurrX(r_Range y) const;

  /// retrieve y for a given x on a line with the slope of the edge. Calls getSlope().
  double getCurrY(r_Range x) const;

  /// print start and end point of the edge.
  void print_status( std::ostream& s = std::cout ) const;
  
  /// returns true if the edge is parallel to the first axis
  bool isHorizontal() const;
  
private:
  /// start point of the edge.
  r_Point start;

  /// end point of the edge.
  r_Point end;
};

/*@Doc:

  The class r_Polygon is used to represent 2-D polygons in
  RasDaMan. Polygons are always constructed by specifying a set of
  points. This class is mainly used for doing polygonal cutouts on
  r_Marray objects, it offers the relevant methods for that purpose.

  The construction of a r_Polygon with addPoint has to finish with a
  call to close(). A open polygon is not a legal polygon! Currently no
  exceptions are thrown if a polygon is open and methods besides
  addPointXY or close are called. The results are undefined, if the
  polygon is not closed.
 
*/

class r_Polygon
{
public:

  /// FIXME current we support only 2 dimensional polygon
  static const r_Dimension polyPointDim;

  /// enum used to clasify one polygon
  enum r_Polygon_Type{
       UNKNOWN,
       CONCAVE,
       CONVEX
       };
       
  /// constructor to initialize polygon from a string
  r_Polygon(const char* init) throw (r_Error);
  
  /// constructor getting x and y of the first point in the polygon.
  r_Polygon(r_Range x, r_Range y);
  
  /// copy constructor 
  r_Polygon(const r_Polygon&);

  /// default constructor.
  r_Polygon();
  
  /// asignment opertor
  r_Polygon& operator=(const r_Polygon&);
  
  /// add a point to the polygon.
  void addPoint(const r_Point& newPoint) throw(r_Error);
  
  /// add a point to the polygon specifying x and y.
  void addPointXY(r_Range x, r_Range y) throw(r_Error);
  
  /// close a polygon after creation with addPointXY.
  void close();
  
  /// retrieve the set of all edges of the polygon.
  const std::vector<r_Edge>& getEdges() const throw(r_Error);
  
  /// determine the polygon type for an polygon in 2D
  r_Polygon::r_Polygon_Type detectPolygonType() const throw(r_Error);
  /**
      It is assumed that the polygon is simple (does not intersect itself or have holes)
      Returns UNKNOWN for incomputables eg: polygon with colinear points,  polygon with less then 3 points
  */
  
  /// retrieve a vector of all points in the polygon.
  std::vector<r_Point> getPoints() const throw(r_Error);
  /**
   Each point is connected with its successor. The last point is connected to the first one.
  */
  
  /// print all edges of the polygon.
  void print_status( std::ostream& s = std::cout ) const;
  
  /// Fill the 2-D array myArray according to the polygon.
  void fillMArray( r_GMarray& myArray, bool fillInside = false, const std::string& bgr = "") const throw(r_Error);
  /** The polygon has to be completely in the domain of the array. Should this not be the case, 
      then the polygon must be clipped according to the domain of the array. Filling is done 
      so that the data in the array is overwritten byte by byte with 0 which is not inside
      the polygon.
  */
  
  /// retrieve the bounding box of the polygon.
  r_Minterval getBoundingBox() const throw(r_Error);
  
  /// clip the polygon according to the bounding box specified in clipDom.
  void clip(const r_Minterval& clipDom) throw(r_Error);  
  /** Note that the r_Polygon object is modified! So after calling clip you will generally
      have a different polygon represented by your object. */
      
  /// scale the points of the polygon according to scaleFactor.
  void scale(const r_Point& origin, const r_Minterval& mddDom,
	     const r_Minterval& clipDom, const double& scaleFactor) throw(r_Error);
  /** This function is used when using a polygon to extract part of an image retrieved
      with r_Fast_Scale. The scaling is done like in r_Fast_Base_Scale::get_scaled_domain(). 
      origin is the point of origin for the scaling. mddDom is the domain of the MDD which
      will later be filled with the polygon. Problem is, that the domain of this MDD is
      currently not created by simply scaling the domain of the original MDD. Instead it
      is made sure that the origin of the scaled domain stays the same. clipDom is used
      to do the same when scaling the polygon, it contains the scaled domain of the MDD
      without shifting it to origin of the domain of the MDD before scaling. It is a
      bit complicated, I know. scaleFactor is trivially the scaleFactor used. */
      
  /// scale the points of the polygon according to scaleFactor.
  void scale(const r_Point& origin, const double& scaleFactor) throw(r_Error);
  /** This function is used used when we scale a polygon to extract part of an image retrieved
      with r_Fast_Scale. The scaling is done like in r_Fast_Base_Scale::get_scaled_domain(). 
      origin is the point of origin for the scaling. scaleFactor is the scale factor used. */

  /// mirrors a polygon along the y-axes point by point.
  void mirror(const r_Minterval& mddDom) throw(r_Error);
  /** The mirroring is done along the middle of mddDom. It is done like that to be coherent
      with the mirroring commonly done when inserting TIFF image, e.g. in insertlva.cc. */

  /// get "middle" point by averaging all x and y values.
  r_Point getMiddle() const throw(r_Error);

  /// "shrink" polygon by moving all points towards the middle by pixelCount pixels.
  void shrinkPoly(int pixelCount) throw(r_Error);

  ///  returns the number of edges
  int  countEdges() const;
  
  ///  returns the number of horizontal edges (used by polygon cut out)
  int  countHorizontalEdges() const;
   
private:

  /// check if the current point is the first point to close the polygon
  void checkFistPoint();

  /// clipping is done side by side. This enum is used to tell functions which side to clip.
  enum Side { 
    top, bottom, left, right
  };

  /// overwrite this with a polygon create from a vector of points.
  void fromPoints(const std::vector<r_Point>& newPoints) throw(r_Error);

  /// erase the area in myArray outside of the polygon for one scanline.
  void eraseLine( r_Range x1, r_Range x2, r_Range y, r_GMarray& myArray, const std::string& bgr ) const throw(r_Error);

  /// return the polygon clipped on the specified side as a list of points.
  std::vector<r_Point> clip1Side(const r_Minterval& b, r_Polygon::Side s);

  /// determine if a point is inside a bounding line on a certain side.
  bool inside(const r_Minterval& b, const r_Point& p, r_Polygon::Side s);

  /// intersect an edge with a bounding line on a certain side.
  r_Point intersect(const r_Minterval& b, const r_Edge& e, r_Polygon::Side s);

  /// flag if the polygon is closed.
  bool closed;

  /// flag if firstPoint has been set.
  bool firstPointSet;

  /// vector of all edges of the polygon.
  std::vector<r_Edge> edges;
  /// remember first point. Used when constructing the polygon.

  r_Point firstPoint;

  /// remember last added point. Used when constructing the polygon.
  r_Point currPoint;
};

extern std::ostream& operator<<( std::ostream& s, const r_Polygon& d );

// The following classes are STL function objects which can be used to make
// sorted collection of r_Edge. In the current implementation only the first
// one is needed.

class EdgeSortCriterion
{
public:
  // This is needed for keeping a sorted set of edges (sorted by start coordinate y,
  // then start coordinate x).
  bool operator() (const r_Edge& e1, const r_Edge& e2) const {
    return e1.getStart()[1] < e2.getStart()[1] || 
      (!(e2.getStart()[1] < e1.getStart()[1]) && e1.getStart()[0] < e2.getStart()[0]);
  }
};

class ActiveEdgeSortCriterion
{
public:
  // This is needed for keeping a sorted set of active edges (sorted by start coordinate x,
  // then start coordinate y).
  bool operator() (const r_Edge& e1, const r_Edge& e2) const {
    return e1.getStart()[0] < e2.getStart()[0] || 
      (!(e2.getStart()[0] < e1.getStart()[0]) && e1.getStart()[1] < e2.getStart()[1]);
  }
};

#endif
