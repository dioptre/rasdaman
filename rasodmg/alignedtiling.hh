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
  * INCLUDE: alignedtiling.hh
 *
 * MODULE:  rasodmg
 * CLASS:   r_Aligned_Tiling, r_Default_Tiling
 *
 * COMMENTS:
 *		None
*/

#ifndef _R_ALIGNEDTILING_HH_
#define _R_ALIGNEDTILING_HH_

class r_Aligned_Tiling;

#include "rasodmg/tiling.hh"
#include "raslib/minterval.hh"

//@ManMemo: Module: {\bf rasodmg}

/*@Doc:
    
    The {\tt r_Aligned_Tiling} class is used to express the options 
    for aligned tiling of {\tt r_Marray} objects. 
    
    The following options may be set:
    
    \begin{itemize}
    \item {\bf Tile configuration}
    
         describes which format tiles should have. The tile configuration 
	 is expressed using a multidimensional interval {\tt r_Minterval}.
	 This interval should have null lower limits and 
	 must have the same dimensionality as that of the 
	 objects to which it is to be applied. Its lengths along each 
	 direction are interpreted relative to the others. 
	 
	 For example, if tile configuration is {\tt[ 0:9, 0:9, 0:19]}, tiles 
	 will be three dimensional arrays with two sides of equal length and 
	 double that length along the third direction. 
	 
	 If a fixed tile is required, tile configuration and 
	 tile size should be set in such a way that the size of a tile 
	 with the given configuration is equal to the specified tile size.
	 For example, if the tile configuration is {\tt [ 0:29, 0:39, 0:59]} 
	 and cell size is 2, then the tile size should be set to 
	 144000. This will also result in more efficient computation of 
	 the tiling since the given tile configuration is used unchanged if
	 
	   {\tt 90% * tile_size < size of tile_config  < tile_size}
	 
	 (i.e., no computation is necessary). This applies equally to tile 
	 configurations with non-fixed limits.
	 
	 Tiles with non-fixed limits are used to express preferential 
	 directions for tiling. For example, {\tt [ 0:9 , 0:* ]} expresses that 
	 tiles should be done along the first direction, i.e., they
	 should have domains :
	 
	 \begin{verbatim}
     [  0 :  9 , 0 : marray.domain[1].high() ]
     [ 10 : 19 , 0 : marray.domain[1].high() ]
     ...
	 \end{verbatim}
	 
	 assuming this results in a tile with the given tile size. If not,
	 the limits in the first direction are changed. The higher dimensions 
	 are given preference in that tiles will be preferably
	 extended along a higher dimension than a lower one if two or
	 more limits are open. 
	 
	 The default configuration corresponds to an interval with equal
	 lengths along all directions.
	
    \item {\bf Tile size }  
    
         describes the size for tiles of the object in characters. 
	 Tiling is done so that tiles are as big as possible but wit a 
	 smaller size than this one.
	 The default tile size is the size specified for the RasDaMan client. 
  
  Notice: the tiling options are invalid if the rasdaman client is running 
          with the option notiling. In that case, no tiling is done, 
	  independently of the storage layout chosen.
*/
/**
  * \defgroup Rasodmgs Rasodmg Classes
  */

/**
  * \ingroup Rasodmgs
  */
class r_Aligned_Tiling : public r_Dimension_Tiling
{
  public:
  /// read everything from encoded string
  /// (e.g. "[0:9,0:9];100" or "2;100")
  r_Aligned_Tiling(const char* encoded) throw (r_Error);
  
  /// dimension and tile size.
  r_Aligned_Tiling(r_Dimension dim, r_Bytes ts = RMInit::clientTileSize) throw (r_Error);
    
  /// dimension and tile size will be taken from RMInit::clientTileSize.
  //r_Aligned_Tiling(r_Dimension dim) throw (r_Error);
    
  /// tile configuration and tile size.
  r_Aligned_Tiling(const r_Minterval& tc, r_Bytes ts = RMInit::clientTileSize) throw (r_Error);
    
  virtual r_Tiling* clone() const;
  
  virtual ~r_Aligned_Tiling();
  
  /// returns the current value for the tile configuration option
  const r_Minterval& get_tile_config() const;
  
  std::vector<r_Minterval>* compute_tiles(const r_Minterval& obj_domain, r_Bytes cell_size) const throw (r_Error);
  
  char* get_string_representation() const;
  /**
    The string representation delivered by this method is allocated using 
    {\tt malloc()} and has to be freed using {\tt free()} in the end. 
  */
    
  /// writes the state of the object to the specified stream
  void print_status(std::ostream& s = cout) const;
  
  virtual r_Tiling_Scheme get_tiling_scheme() const;     

  static const char* description;
  
  protected:
  
  /// determines the individual tiles domains
  r_Minterval compute_tile_domain(const r_Minterval& dom, r_Bytes cell_size) const;
  /**
     Determines the individual tiles domains for aligned tiling, 
     using the options expressed in this object.
     Takes into account the tile size and the tile configuration, 
     as well as the cell size given by {\ttcell_size}. 
     
     Returns the domain for tiles in such a way that the tile 
     configuration is as close to {\tttile_config} set in this object and
     the size is lower than {\tt tile_size}.
     
     The origin of the returned interval is the same as that from
     {\ttthis->tile_config}.
     
     The data to partition has domain {\tt dom } and cells with size 
     {\tt cell_size}.
     To be used before splitting a tile with domain {\tt dom} (typically,
     containing all the cells belonging to an {\ttr_Marray} object).
  */
  
  ///  tile configuration
  r_Minterval tile_config;
  
  /// 
  r_Bytes get_min_opt_tile_size() const;
  
  ///
  r_Minterval get_opt_size(const r_Minterval& tile_domain, r_Bytes cell_size) const;
};

//@ManMemo: Module: {\bf rasodmg}

/*@Doc:
    
    The {\tt r_Default_Tiling} class is used to express the default tiling 
    of {\tt r_Marray} objects. According to this algorithm, tiles are divided 
    into equal sized multidimensional blocks qith equal lengths along
    all directions of the spatial domain.

    The following parameter may be set:
    
    \item {\bf Tile size }  
    
         describes the size for tiles of the object in characters. 
	 Tiling is done so that tiles have a smaller size than this one.
	 The default tile size is the size specified for the RasDaMan client.
	 In bytes.
*/


//@ManMemo: Module: {\bf rasodmg }
/**
  Output stream operator for objects of type {\tt const} 
  \Ref{r_Aligned_Tiling}.
*/
//extern std::ostream& operator<<(std::ostream& s, const r_Aligned_Tiling& at);

#endif




