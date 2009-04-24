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
 * CLASS:   r_Interest_Tiling
 *
 * COMMENTS:
 *		None
*/

#ifndef _R_INTERESTTILING_HH_
#define _R_INTERESTTILING_HH_

class r_Dir_Decompose;
class r_Interest_Tiling;
#include "rasodmg/tiling.hh"

//@ManMemo: Module: {\bf rasodmg}

/*@Doc:
  This class implements the "Interesting Areas" tiling algorithm.
  The user specifies which areas are of interest to him and tiling is
  performed accordingly.

  Example of usage:
    The user says that [10:20, 50:60] and [18:50, 65:70] are of interest
    to him in the [0:1000, 0:1000] domain. Tiling should be done according
    to this setting. To specify this with this class the following code 
    would work:

      ...

      r_Minterval domain(2);
      domain << r_Sinterval(0L, 1000L) << r_Sinterval(0L, 1000L);

      r_Minteval interest1(2);
      interest1 << r_Sinterval(10L, 20L) << r_Sinterval(50L, 60L);

      r_Minterval interest2(2);
      interest2 << r_Sinterval(18L, 50L) << r_Sinterval(65L, 70L);

      std::vector<r_Minterval> interest_areas;
      interest_areas.insert_element(interest1);
      interest_areas.insert_element(interest2);

      r_Interest_Tiling(interest_areas);

      ...
*/

class r_Interest_Tiling : public r_Dimension_Tiling
{
  public:

    /// Possible strategies to limit the tilesize
    enum Tilesize_Limit {NO_LIMIT, REGROUP, SUB_TILING, REGROUP_AND_SUBTILING, NUMBER};

   /// read everything from an encoded string
   /// e.g. "2;[0:9,0:9];[100:109,0:9];100;REGROUPSUBTILING"
    r_Interest_Tiling(const char* encoded) throw (r_Error);

    r_Interest_Tiling(r_Dimension dim,
                      const std::vector<r_Minterval>& interest_areas,
		      r_Bytes ts = RMInit::clientTileSize,
		      Tilesize_Limit strat = SUB_TILING) throw (r_Error);
    /**
      It takes as parameter a list containing the areas of interest to
      the user and also the tilesize to be used.
      The constructor also takes as parameter the tilesize limitation
      strategy, that can be the following:

        NO_LIMIT:   The generated blocks can have any size.
	REGROUP:    Only when performing grouping/merging of blocks, the
	            size of the resulting block of two merges is checked
		    against tilesize. If it's bigger, they are not merged.
		    Blocks larger than tilesize may exist (for instance,
		    if the user specifies an interest area larger then
		    tilesize).
        SUB_TILING: In this strategie, regrouping is done regardless of the
	            size of the generated blocks. After all the blocks are
		    created, sub-tiling is performed on those whose size is
		    larger than tilesize.
	REGROUP_AND_SUBTILING: This combines the last two strategies. When
	            merging blocks, blocks larger than tilesize are never 
		    created and, when the final blocks are all created, 
		    sub-tiling is performed on those whose size is larger
		    then tilesize.
    An exception is thrown when the dimensions of the domains does not match the
    specified dimension.
    */

    virtual ~r_Interest_Tiling();

    std::vector<r_Minterval>* compute_tiles(const r_Minterval& obj_domain, r_Bytes cell_size) const throw (r_Error);

    virtual void print_status(std::ostream& os) const;
    
    virtual r_Tiling* clone() const;

    virtual r_Tiling_Scheme get_tiling_scheme() const;
    
    //@ManMemo: Module: {\bf raslib}
    /**
       Get a tilesize limit for a tilisize limit name
    */
    static r_Interest_Tiling::Tilesize_Limit get_tilesize_limit_from_name(const char* name);
    //@ManMemo: Module: {\bf raslib}
    /**
       Get a tilisize limit name for a tilesize limit
    */
    static const char*      get_name_from_tilesize_limit(Tilesize_Limit tsl);

    static const char* description;

  protected: // methods.

    /// Given a domain and a set of interest areas (internal) gener. partition
    std::vector<r_Dir_Decompose>* make_partition(const r_Minterval& domain) const;

    /// The Block types (A, B or C)
    enum Blocks_Type {BLOCKS_A, BLOCKS_B, BLOCKS_C };
    /**
      A Blocks ---> Blocks that belong to two or more interest zones
                    (non-groupable)
      B Blocks ---> Blocks within the same interest zone
                    (groupable by interest zone)
      C Blocks ---> Blocks outside interest zones
                    (groupable)
      */

    /// Merge as many blocks together in a list as possible
    std::vector<r_Minterval>* group(std::vector<r_Minterval>& blocks, r_Bytes typelen, Blocks_Type btype) const;
    /**
      Parameters: the block list, the lenght of the base cells and
      the type of the block (A, B or C)
      */

    /// Tilesize limitation strategie
    Tilesize_Limit ts_strat;

    /// Interest areas
    std::vector<r_Minterval> iareas;


   //@ManMemo: Module: {\bf raslib}
   /**
     The names of all tilesizelimit types, to avoid redundant storage and inconsistencies.
     The variable name convention is the prefix tilisizelimit_name_ followed by the name
     of the data format in lower case, i.e. for NOLIMIT tilisizelimit_name_nolimit.
     In addition there's an array of names all_tilesizelimit_names  where the tilesize limit
     can be used as index to get the name.
   */
   static const char* tilesizelimit_name_nolimit;
   static const char* tilesizelimit_name_regroup;
   static const char* tilesizelimit_name_subtiling;
   static const char* tilesizelimit_name_regroupandsubtiling;
   
   static const char* all_tilesizelimit_names[r_Interest_Tiling::NUMBER]; 
};

#endif
