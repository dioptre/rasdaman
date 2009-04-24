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
 * INCLUDE: mitera.hh
 *
 * MODULE:  raslib
 * CLASS:   r_MiterArea
 *
*/

#ifndef _D_MITERA_
#define _D_MITERA_

#include "raslib/minterval.hh"

//@ManMemo: Module: {\bf raslib}

/*@Doc:
  r_MiterArea is used for iterating r_Mintervals through larger
  r_Mintervals. It is given the domain to be iterated through and
  an Minterval specifying the shape of area to be iterated with.


  Going to the next area is done with nextArea() which returns an
  r_Minterval. Test for the end is done with isDone(). The
  iterator can be reset with reset(). Iteration starts at the
  lowest border in all dimensions. Note that if the shape of
  r_Minterval iterated does not completely fit into the
  r_Minterval iterated through the results at the border may have
  a different (smaller) shape.
*/

class r_MiterArea
{
public:
  /// constructor.
  /// An exception is thrown if newIterDom and newImgDom have different dimension
  r_MiterArea( const r_Minterval* newIterDom, 
	       const r_Minterval* newImgDom ) throw(r_Error);
  /**
    The pointers are stored, do not delete the objects as long
    as the iterator is used!
  */
  
  /// destructor.
  ~r_MiterArea();                    
  /// resets iterator to beginning.
  void reset();
  /// returns current cell and sets iterator to next cell.
  r_Minterval nextArea();
  /// returns TRUE if iteration is finished.
  bool isDone();
protected:
  // structure storing information on iteration for each dimension
  // (perhaps add dimension for reordering later)
  typedef struct {
    int repeat; // total number of repeats
    int curr;   // current repeat
  } incArrElem;
  /// area to be iterated through
  const r_Minterval* iterDom;
  /// area of tile.
  const r_Minterval* imgDom;
  /// array with increments
  incArrElem* incArrIter;
  /// flag set if iteration is finished.
  bool done;
  /// This is used for the return value in nextArea()
  r_Minterval retVal;
};

#endif
