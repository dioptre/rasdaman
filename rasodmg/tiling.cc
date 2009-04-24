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
 * SOURCE: tiling.cc
 *
 * MODULE: rasodmg
 * CLASS:	r_Tiling
 *
 * COMMENTS:
 *	 		None
 *
*/

//for max min
#include <algorithm>

//for pow
#include <math.h>
#include <iostream>
#include "rasodmg/tiling.hh"
#include "raslib/rmdebug.hh"
#include "raslib/minterval.hh"
#include "rasodmg/gmarray.hh"
#include "raslib/dlist.hh"

const char*
r_Tiling::ASTERIX   = "*";
const char*
r_Tiling::COLON     = ";";
const char*
r_Tiling::COMMA     = ",";
const char*
r_Tiling::LSQRBRA   = "[";
const char*
r_Tiling::RSQRBRA   = "]";
const long
r_Tiling::DefaultBase = 10;

r_Tiling::~r_Tiling()
	{
	}
	
const char*
r_No_Tiling::description = "no parameters";

r_No_Tiling::r_No_Tiling(const char* encoded) throw(r_Error)
	{
	//we don't use encoded string, it is present in order to have
	//uniform interface "char* constructor" for every tiling strategy
	}
	
r_No_Tiling::r_No_Tiling()
	{
	}	

r_No_Tiling::~r_No_Tiling()
	{
	}

void
r_No_Tiling::print_status(std::ostream& os) const
	{
	os << "r_No_Tiling[ ]";
	}

bool
r_No_Tiling::is_compatible(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const
	{
	return true;
	}

std::vector<r_Minterval>*
r_No_Tiling::compute_tiles(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const throw (r_Error)
	{
	RMDBGENTER(4, RMDebug::module_rasodmg, "r_No_Tiling", "compute_tiles(" << obj_domain << ", " << cellTypeSize << ")")
	std::vector<r_Minterval>* result = new std::vector<r_Minterval>; 
	result->push_back(obj_domain);
	RMDBGEXIT(4, RMDebug::module_rasodmg, "r_No_Tiling", "compute_tiles() " << *result)
	return result;
	}

r_Tiling*
r_No_Tiling::clone() const
	{
	r_Tiling* clo = new r_No_Tiling();
	return clo;
	}

r_Tiling_Scheme
r_No_Tiling::get_tiling_scheme() const
	{
	return r_NoTiling;
	}

std::ostream&
operator<<(std::ostream& os, const r_Tiling& t)
	{
	t.print_status(os);
	return os;
	}

const char*
r_Size_Tiling::description = "tile configuration or tile dimension and tile size (in bytes) (ex: \"[0:9,0:9];100\" or \"2;100\")";

r_Size_Tiling::r_Size_Tiling(const char* encoded) throw (r_Error)
	:	tile_size(0)
{
  if(!encoded) {
	 RMInit::logOut << "r_Size_Tiling::r_Size_Tiling(" << (encoded?encoded:"NULL") << ")" << endl;
	 throw r_Error(TILINGPARAMETERNOTCORRECT);  
  }
  
  r_Bytes tileS=strtol(encoded, (char**)NULL, DefaultBase);
  if(tileS<=0) {
	 RMInit::logOut << "r_Size_Tiling::r_Size_Tiling(" << encoded << "): Error decoding tile size." << endl;
	 throw r_Error(TILINGPARAMETERNOTCORRECT);
  }
  tile_size = tileS;
}

r_Size_Tiling::r_Size_Tiling(r_Bytes ts) :
	tile_size(ts)
	{
	}

r_Size_Tiling::~r_Size_Tiling()
	{
	}

r_Bytes
r_Size_Tiling::get_tile_size() const
	{
	return tile_size;
	}

void
r_Size_Tiling::print_status(std::ostream& os) const
	{
	os << "r_Size_Tiling[ tile size = " << tile_size << " ]";
	}

bool
r_Size_Tiling::is_compatible(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const
	{
	return ((cellTypeSize <= tile_size) && (obj_domain.dimension() != 0));
	}

std::vector<r_Minterval>*
r_Size_Tiling::compute_tiles(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const throw (r_Error)
	{
	RMDBGENTER(4, RMDebug::module_rasodmg, "r_Size_Tiling", "compute_tiles(" << obj_domain << ", " << cellTypeSize << ")")
	if (cellTypeSize > tile_size)
		{
		RMInit::logOut << "r_Size_Tiling::compute_tiles(" << obj_domain << ", " << cellTypeSize << ") tile size (" << tile_size << ") is smaller than type length (" << cellTypeSize << ")" << endl;
		throw r_Error(TILESIZETOOSMALL);
		}
	std::vector<r_Minterval>* result = new std::vector<r_Minterval>; 
	r_Minterval bigDom = obj_domain;
	r_Dimension dim = bigDom.dimension();
	r_Minterval tileDom(dim);
	// compute the domain of the small tiles
	// tiles are n-dimensional cubes with edge length n-th root of max tile size
	RMDBGMIDDLE(4, RMDebug::module_rasodmg, "r_Size_Tiling", "tile size " << get_tile_size())	 
	r_Range edgeLength = (r_Range)std::max((r_Range)floor(pow(get_tile_size()/cellTypeSize, 1/(double)dim)), 1);
	r_Dimension dimcnt = 0;
	for (dimcnt = 0; dimcnt < dim; dimcnt++)
		tileDom << r_Sinterval((r_Range)0, edgeLength - 1);

	r_Minterval currDom(dim);
	r_Point cursor(dim);
	r_Point tileSize;
	r_Point origin;
	bool done = 0;
	
	// initialize cursor
	for (dimcnt = 0; dimcnt < dim; dimcnt++)
		cursor[dimcnt] = 0;

	// calculate size of Tiles
	tileSize = tileDom.get_extent();

	// origin of bigTile
	origin = bigDom.get_origin();

	// initialize currDom
	for (dimcnt=0; dimcnt < dim; dimcnt++)
		currDom << r_Sinterval((r_Range)origin[dimcnt], (r_Range)(origin[dimcnt] + tileSize[dimcnt] - 1));
	// resets tileDom to lower left side of bigTile
	tileDom = currDom;
	
	// intersect with bigTile
	currDom.intersection_with(bigDom);

	// iterate with smallTile over bigTile
	while (!done)
		{
		currDom.intersection_with(bigDom);
		// create new smallTile
		r_Minterval smallTile(dim);
		
		smallTile = currDom;
		// insert tile in set
		result->push_back(smallTile);

		// increment cursor, start with highest dimension
		r_Dimension i = cursor.dimension() - 1;
		cursor[i] += tileSize[i];
		// move cursor
		currDom = tileDom.create_translation(cursor);
		while (!(currDom.intersects_with( bigDom )))
			{
			cursor[i] = 0;
			if (i == 0)
				{
				done = true;
				break;
				}
			i--;
			cursor[i] += tileSize[i];
			// move cursor
			currDom = tileDom.create_translation(cursor);
			}
		}
	RMDBGEXIT(4, RMDebug::module_rasodmg, "r_Size_Tiling", "compute_tiles() " << *result)
	return result;
	}

r_Tiling*
r_Size_Tiling::clone() const
	{
	r_Tiling* clo = new r_Size_Tiling(tile_size);
	return clo;
	}

r_Tiling_Scheme
r_Size_Tiling::get_tiling_scheme() const
	{
	return r_SizeTiling;
	}

/*
std::ostream&
operator<<(std::ostream& os, const r_Size_Tiling& t)
	{
	t.print_status(os);
	return os;
	}
*/

r_Dimension_Tiling::r_Dimension_Tiling(r_Dimension dim, r_Bytes ts)
	:	r_Size_Tiling(ts),
		dimension(dim)
	{
	}

r_Dimension_Tiling::~r_Dimension_Tiling()
	{
	}

r_Dimension
r_Dimension_Tiling::get_dimension() const
	{
	return dimension;
	}

void
r_Dimension_Tiling::print_status(std::ostream& os) const
	{
	os << "r_Dimension_Tiling[ ";
	r_Size_Tiling::print_status(os);
	os << " dimension = " << dimension << " ]";
	}

bool
r_Dimension_Tiling::is_compatible(const r_Minterval& obj_domain, r_Bytes cellTypeSize) const
	{
	return ((obj_domain.dimension() == dimension) && r_Size_Tiling::is_compatible(obj_domain, cellTypeSize));
	}

/*
std::ostream&
operator<<(std::ostream& os, const r_Dimension_Tiling& t)
	{
	t.print_status(os);
	return os;
	}
*/


