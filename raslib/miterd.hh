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
 * INCLUDE: miterd.hh
 *
 * MODULE:  raslib
 * CLASS:   r_MiterDirect
 *
*/

#ifndef _R_MITERD_
#define _R_MITERD_

#include "raslib/mddtypes.hh"
#include "raslib/odmgtypes.hh"

#include <iosfwd>

class r_Minterval;

class r_miter_direct_data;

//@ManMemo: Module {\bf raslib}

/*@Doc:
  r_MiterDirect is similar to r_Miter, but allows stepping by more
  than one cell in each direction, arbitrary order of dimensions in
  the iteration and has a lot of its internal state variables as public
  members.
  It should be used in low-level, very time-critical code like
  folding operations which would otherwise require construction
  of a new iterator for each cell when only position and base
  address need to change.
*/

class r_MiterDirect
{
  public:
  /// constructor
  r_MiterDirect(void *data, const r_Minterval &total, const r_Minterval &iter,
		r_Bytes tlen, unsigned int step=1);
  /**
     constructor getting the data, the total domain, the iteration
     domain, the base type length and the number of steps per
     iteration.
  */

  /// destructor
  ~r_MiterDirect(void);

  /// increment the iterator in the default order, i.e. last dimension first
  inline r_MiterDirect &operator++(void);
  /// increment in user-specified order
  inline r_MiterDirect &iterateUserOrder(const r_Dimension *order, const unsigned int *step);
  /**
     increment the iterator in a user-specified order. order points to an array
     defining the order of the dimensions during iteration, e.g. for a 3D
     iteration 0,1,2 would iterate over the first dimension first and the
     last dimension last wheres 2,1,0 is equivalent to operator++(). step
     is the number of steps to do in each dimension.
  */
  /// increment or decrement in user-specified order
  inline r_MiterDirect &iterateUserOrder(const unsigned int *order, const int *step);
  /**
     see the other incrementUserOrder method for more details
  */

  /// returns != 0 if iteration is finished.
  inline bool isDone(void) const;
  /// returns pointer to data during normal iteration.
  inline void* getData(void);
  /// return pointer to data for non-standard iteration order
  inline void* getData(unsigned int *order);
  /**
     returns pointer to data during user-defined iteration; order is as
     defined in iterateUserOrder().
  */
  /// returns number of bytes to step in dimension d in one iteration
  inline r_Range getDimStep(r_Dimension d) const;
  /// returns number of bytes to step in dimension d when pos changes by 1.
  inline r_Bytes getDimBaseStep(r_Dimension d) const;
  /// returns extent in dimension d
  inline r_Range getExtent(r_Dimension d) const;
  /// notify that the position was changed and internal variables need to be recalculated
  inline void posChanged( void );
  /// reset the iterator (pos to low and data to baseAddress + offset)
  void reset(void);
  /// print the position
  void print_pos(std::ostream &str) const;

  bool done;
  r_miter_direct_data* id;
  void* baseAddress;

  private:

  /// if this data should change you must construct a new iterator,
  /// therefore no public access.
  r_Dimension dim;
  r_ULong length;
};



/*@Doc:
  r_miter_direct_data encapsulates data for each dimension.
  It's an auxiliary class for r_MiterDirect. The only reason
  not to make it a simple struct was data protection.
 */

class r_miter_direct_data
{
  friend class r_MiterDirect;

  public:

  r_miter_direct_data();
  ~r_miter_direct_data();

  /// Data concerning the iteration position and domain. May
  /// be changed by the user.
  void *data;
  r_Range pos;
  r_Range low;
  r_Range high;

  private:

  /// Data concerning the domain of the source object. Is fixed
  /// in the constructor.
  r_Range step;
  r_Range baseStep;
  r_Range extent;
  r_Range origin;
};


/// overloaded stream operator
extern std::ostream &operator<<(std::ostream &str, const r_MiterDirect &iter);



#include "raslib/miterd.icc"

#endif
