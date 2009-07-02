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
 * INCLUDE: dirdecomp.hh
 *
 * MODULE:  rasodmg
 * CLASS:   r_Dir_Decompose
 *
 * COMMENTS:
 *		None
*/

#ifndef _R_DIRDECOMPOSE_HH_
#define _R_DIRDECOMPOSE_HH_

// Include statements 

#include <iostream>
#include <vector>
using std::cout;
using std::vector;

#include "raslib/error.hh"
#include "raslib/sinterval.hh"

//@ManMemo: Module {\bf rasodmg}

/*@Doc:

  The {\tt r_Dir_Decompose} class is used to specify a decomposition on 
  an n-dimensional cube (for use in {\tt r_Dir_Tiling}). For instance, to 
  specify a tiling restriction on a dimension with the form: [0, 2, 4, 5], 
  the following code would apply:

    r_Dir_Decompose decomp;
    
    decomp << 0 << 2 << 4 << 5;

  Note that the first and the last elements input into the object must be 
  the origin and limit of that dimension or else a cross-section of the domain 
  will occur (as if the elements outside the specification wouldn't mind). 

  If one dimension is considered to be a prefered access direction, then
  the r_Dir_Decompose should be empty, this is, no restriction should be
  entered.
*/

/**
  * \ingroup Rasodmgs
  */
class r_Dir_Decompose
{
  public:

    r_Dir_Decompose();

    virtual ~r_Dir_Decompose();
		
    /// Copy constructor
    r_Dir_Decompose(const r_Dir_Decompose& other);

    /// Assigment operator
    const r_Dir_Decompose& operator=(const r_Dir_Decompose& other);
    
    /// Reads a new limit for the current dimension
    r_Dir_Decompose& operator<<(r_Range limit);

    /// Gets the number of intervals the dimension is to be split into
    int get_num_intervals() const;

    /// Gets a restriction
    r_Range get_partition(int number) const throw (r_Eindex_violation);

    /// Prints the current status of the object
    virtual void print_status(std::ostream& os = cout ) const;

protected:
   r_Sinterval get_total_interval( );

    /// Initial number of intervals of the buffer
    const static r_Dimension DEFAULT_INTERVALS;

    /// The number of intervals that this object can currently suport
    r_Dimension num_intervals;

    /// The current interval that is being used for input
    r_Dimension current_interval;

    /// The buffer that holds the information
    r_Range* intervals;
};

//@ManMemo: Module: {\bf rasodmg}
/**
    Prints the status of an r_Dir_Decompose object to a stream
*/
extern std::ostream& operator<<(std::ostream& os, const r_Dir_Decompose& d);
extern std::ostream& operator<<(std::ostream& os, const std::vector<r_Dir_Decompose>& vec);

#endif
