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
 * INCLUDE: dirtiling.hh
 *
 * MODULE:  rasodmg
 * CLASS:   r_Dir_Tiling
 *
 * COMMENTS:
 *		None
*/

#ifndef _R_DIRTILING_HH_
#define _R_DIRTILING_HH_

// Include statements 
class r_Dir_Decompose;
class r_Dir_Tiling;

#include "rasodmg/tiling.hh"

// Class definition

//@ManMemo: Module: {\bf rasodmg}

/*@Doc:
  This class implements the "Directional Tiling" tiling method. In this
  method the decomposition is done along certain directions of the 
  multi-dimensional cube. The user uses {\tt r_Dir_Decompose} to inform
  the system on how the decomposition is done in each dimension.
*/

/**
  * \ingroup Rasodmgs
  */
class r_Dir_Tiling : public r_Dimension_Tiling
{
  // ******************* PUBLIC SECTION *******************

  public:

    /// Constants that specify if subtiling will occur inside the blocks
    enum SubTiling { 
    	 WITHOUT_SUBTILING = 0, 
    	 WITH_SUBTILING = 1,
    	 NUMBER = 2 
    	 };
    /// read everything from encoded string
    /// e.g. "3;[0,2,4,5],[*],[0,10,15];100;NOSUBTILING"
    r_Dir_Tiling(const char* encoded) throw (r_Error);

    /// Class constructor
    r_Dir_Tiling(r_Dimension dims,
		 const std::vector<r_Dir_Decompose>& decomp, 
                 r_Bytes ts = RMInit::clientTileSize, 
		 SubTiling sub = WITH_SUBTILING) throw (r_Error);
    /**
    The user has to give the number of dimensions of the space and the 
    decomposition wanted for that space. Note that the number of elements of 
    decomp must be the same as the number of dimensions of the space. 
    See {\tt r_Dir_Decompose} for details on how to indicate a decomposition.
    SubTiling indicates if subtiling should occur inside the blocks specified 
    by the user.
    Throws an exception when the decomp.size is not equal to the specified
    dimension.
    */

    virtual ~r_Dir_Tiling();

    virtual std::vector<r_Minterval>* compute_tiles(const r_Minterval& obj_domain, r_Bytes cell_size) const throw (r_Error);
    
    virtual bool is_compatible(const r_Minterval& obj_domain, r_Bytes type_len) const;

    virtual void print_status(std::ostream& os) const;

    virtual r_Tiling* clone() const;
    
    virtual r_Tiling_Scheme get_tiling_scheme() const;     
    
    //@ManMemo: Module: {\bf raslib}
    /**
     Get a tilesize limit for a tilisize limit name
    */
    static r_Dir_Tiling::SubTiling get_subtiling_from_name(const char* name);
    //@ManMemo: Module: {\bf raslib}
    /**
     Get a tilisize limit name for a tilesize limit
    */
    static const char*      get_name_from_subtiling(SubTiling st);

    static const char* description;
  
  protected: // data

    /// The decomposition to be used
    std::vector<r_Dir_Decompose> dim_decomp;

    /// If sub-tiling should occour
    SubTiling sub_tile;
    
    //@ManMemo: Module: {\bf raslib}
    /**
     The names of all subtiling types, to avoid redundant storage and inconsistencies.
     The variable name convention is the prefix subtiling_name_ followed by the name
     of the data format in lower case, i.e. for WITH_SUBTILING subtiling_name_withsubtiling.
     In addition there's an array of names all_subtiling_names  where the subtiling
     can be used as index to get the name.
    */
    static const char* subtiling_name_withoutsubtiling;
    static const char* subtiling_name_withsubtiling;
                                                
    static const char* all_subtiling_names[r_Dir_Tiling::NUMBER];
};

#endif

