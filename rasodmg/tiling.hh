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
 * INCLUDE: tiling.hh
 *
 * MODULE:  rasodmg
 * CLASS:   r_Tiling
 *
 * COMMENTS:
 *			None
*/

#ifndef _R_TILING_HH_
#define _R_TILING_HH_

class r_Tiling;
class r_No_Tiling;
class r_Size_Tiling;
class r_Dimension_Tiling;
class r_Minterval;

// Include statements 

#include <vector>
#include <iostream>
using std::cout;

#include "raslib/rminit.hh"
#include "raslib/odmgtypes.hh"
#include "raslib/mddtypes.hh"
#include "raslib/error.hh"

//@ManMemo: Module {\bf rasodmg}

/*@Doc:

  The {\tt r_Tiling} class is used to specify in which way the tiling is done 
  by the system. The core method that does that is called {\tt decomposeMDD()}
  and must be implemented by all derived classes. It takes an object that 
  hasn't yet been split and divides it into tiles. Each derived class 
  implements a diferent decomposition method.
*/

/**
  * \ingroup Rasodmgs
  */
class r_Tiling
	{
	public:

		/// does not do anything
		virtual ~r_Tiling();	
	
		/// Prints the current status of the object
		virtual void print_status(std::ostream& os) const = 0;
		
		/// Check compatibility of object domain with this tiling
		virtual bool is_compatible(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const = 0;

		/// Decompose an object in tiles
		virtual std::vector<r_Minterval>* compute_tiles(const r_Minterval& obj_domain, r_Bytes cell_size) const throw (r_Error) = 0;
		/**
		This method provides the core funcionality of this class. All derived 
		classes must implement it. As input parameters it takes the big object to 
		be decomposed and returns a set of tiles that compose the big object.
                This method throws an exeception when the dimension specified, extend or 
                the cell_size are incompatible with the current tiling.  You can check
                compatibility by invoking is_compatible.
		*/

		/// Clones this object
		virtual r_Tiling* clone() const = 0;
		/**
		This method is similar to a copy constructor, this is, is returns a copy of
		the current object. Derived classes must explicitly implement this method.
		*/

		virtual r_Tiling_Scheme get_tiling_scheme() const = 0;
		/**
		return the type of tiling scheme
		*/

		static const char* ASTERIX;
		static const char* TCOLON;
		static const char* TCOMMA;
		static const char* LSQRBRA;
		static const char* RSQRBRA;
		static const long DefaultBase;
 	};

/**
  * \ingroup Rasodmgs
  */
class r_Size_Tiling :	public r_Tiling
	{
	public:
		/// Constructor that reads everything from a string
		/// e.g."100"
		r_Size_Tiling(const char* encoded) throw (r_Error);

		/// Constructor for this object (Takes tile size as parameter)
		r_Size_Tiling(r_Bytes ts = RMInit::clientTileSize);

		/// does not do anything
		virtual ~r_Size_Tiling();

		/// Gets the current tile size
		r_Bytes get_tile_size() const;

		virtual void print_status(std::ostream& os) const;
		
		/// returns true if the cellTypeSize is smaller or equal to the tile size and obj_domain has more than 0 dimensions
		virtual bool is_compatible(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const;

		virtual std::vector<r_Minterval>* compute_tiles(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const throw (r_Error);

		virtual r_Tiling* clone() const;

		virtual r_Tiling_Scheme get_tiling_scheme() const;

		static const char* description;

	protected:

		/// Tile size
		r_Bytes tile_size;
 	};


/**
  * \ingroup Rasodmgs
  */
class r_Dimension_Tiling :	public r_Size_Tiling
	{
	public:
		/// Constructor for this object (Takes dim (no of dimension) and tile size as parameter)
		r_Dimension_Tiling(r_Dimension dim, r_Bytes ts = RMInit::clientTileSize);

		/// does not do anything
		virtual ~r_Dimension_Tiling();

		/// Gets the current dimension
		r_Dimension get_dimension() const;

		virtual void print_status(std::ostream& os) const;
		
		virtual bool is_compatible(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const;
		/// returns true if the cellTypeSize is smaller or equal to the tile size and the dimension fits the obj_domain

		virtual std::vector<r_Minterval>* compute_tiles(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const throw (r_Error) = 0;

		virtual r_Tiling* clone() const = 0;

	protected:

		/// dimension the mdd must have
		r_Dimension dimension;
 	};


/**
  * \ingroup Rasodmgs
  */
class r_No_Tiling :	public r_Tiling
	{
	public:
		/// Constructor that reads everything from a string e.g."100"
		/// This string is ignored in the constructor, it is present 
		/// in order to have an uniform interface 
		r_No_Tiling(const char* encoded) throw (r_Error);	
		/// Constructor for this object 
		r_No_Tiling();

		/// does not do anything
		virtual ~r_No_Tiling();

		/// Prints the current status of the object
		virtual void print_status(std::ostream& os) const;
		
		/// Check compatibility of object domain with this tiling
		virtual bool is_compatible(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const;
		/// returns true

		/// Decompose an object in tiles
		virtual std::vector<r_Minterval>* compute_tiles(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const throw (r_Error);
		/// returns obj_domain

		virtual r_Tiling* clone() const;

		virtual r_Tiling_Scheme get_tiling_scheme() const;

		static const char* description;
 	};


/*@Doc:
    Prints the status of a Tiling object to a stream
*/
//extern std::ostream& operator<<(std::ostream& os, const r_Size_Tiling& t);

/*@Doc:
    Prints the status of a Tiling object to a stream
*/
//extern std::ostream& operator<<(std::ostream& os, const r_No_Tiling& t);

/*@Doc:
    Prints the status of a Tiling object to a stream
*/
extern std::ostream& operator<<(std::ostream& os, const r_Tiling& t);

#include "rasodmg/tiling.hh"
#if (defined(__VISUALC__) && !defined(__EXECUTABLE__))
  #define __EXECUTABLE__
  #include "raslib/dlist.hh"
  #undef  __EXECUTABLE__
#else
  #include "raslib/dlist.hh"
#endif

#endif
