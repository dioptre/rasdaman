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
*/
/****************************************************************************
 *
 *
 *
 * COMMENTS:
 *	 none
 *
 ****************************************************************************/

#include "sstoragelayout.hh"
#include <stdlib.h>
#include "raslib/rmdebug.hh"

const r_Bytes	StorageLayout::DBSPageSize = 4096; 

r_Bytes		StorageLayout::DefaultMinimalTileSize = DBSPageSize;

r_Bytes		StorageLayout::DefaultPCTMax = 2 * DBSPageSize;

r_Bytes		StorageLayout::DefaultTileSize = 32 * DBSPageSize;

unsigned int		StorageLayout::DefaultIndexSize = 0;

r_Index_Type		StorageLayout::DefaultIndexType = r_RPlus_Tree_Index; // DirTilesIx; // AutoIx; 

r_Tiling_Scheme		StorageLayout::DefaultTilingScheme = r_NoTiling;

r_Minterval		StorageLayout::DefaultTileConfiguration("[0:511,0:511]");

r_Data_Format		StorageLayout::DefaultDataFormat = r_Array;

StorageLayout::StorageLayout(r_Index_Type ixType)
	:	myLayout(new DBStorageLayout())
	{
	setIndexType(ixType);
	RMDBGONCE(4, RMDebug::module_storagemgr, "StorageLayout", "StorageLayout(" << ixType << ")")
	}

StorageLayout::StorageLayout()
	:	myLayout(new DBStorageLayout())
	{
	RMDBGONCE(4, RMDebug::module_storagemgr, "StorageLayout", "StorageLayout()")
	}

StorageLayout::StorageLayout(const DBStorageLayoutId& id) 
	:	myLayout(id)	
	{
	RMDBGONCE(4, RMDebug::module_storagemgr, "StorageLayout", "StorageLayout(" << id.getOId() << ")")
	}

/*
const char* 
StorageLayout::getName() const
	{
	return stName;
	} 
*/

DBStorageLayoutId
StorageLayout::getDBStorageLayout() const
	{
	return myLayout;
	}

r_Index_Type
StorageLayout::getIndexType() const
	{
	return myLayout->getIndexType();
	} 


r_Tiling_Scheme
StorageLayout::getTilingScheme() const
	{
	return myLayout->getTilingScheme();
	} 

r_Bytes
StorageLayout::getTileSize() const
	{
	return myLayout->getTileSize(); 
	}

r_Bytes
StorageLayout::getMinimalTileSize() const
	{
	return StorageLayout::DBSPageSize;
	}

r_Minterval
StorageLayout::getTileConfiguration() const
	{
	return myLayout->getTileConfiguration();
	}


void 
StorageLayout::setIndexType(r_Index_Type it)
	{
	myLayout->setIndexType(it);
	}

void 
StorageLayout::setDataFormat(r_Data_Format cs)
	{
	myLayout->setDataFormat(cs);
	}

void 
StorageLayout::setTilingScheme(r_Tiling_Scheme ts)
	{ 
	myLayout->setTilingScheme(ts);
	}

void 
StorageLayout::setTileSize(r_Bytes newSize) 
	{
	myLayout->setTileSize(newSize);
	}

void 
StorageLayout::setTileConfiguration(const r_Minterval& tc)	
	{
	myLayout->setTileConfiguration(tc);
	}

r_Data_Format
StorageLayout::getDataFormat(const r_Point& where) const
	{
	return myLayout->getDataFormat();
	}

std::vector< r_Minterval > 
StorageLayout::getLayout(const r_Minterval& tileDomain)
	{
	RMDBGENTER(5, RMDebug::module_storagemgr, "StorageLayout", "getLayout(" << tileDomain << ")");
	std::vector< r_Minterval > retval;
	if (myLayout->supportsTilingScheme())
		switch (myLayout->getTilingScheme())
			{
			case r_RegularTiling:
				if (myLayout->getTileConfiguration().dimension() == tileDomain.dimension())
					retval = calcRegLayout(tileDomain);
				else	{
					RMInit::logOut << "Regular Tiling chosen without Tiling Domain!\nPlease specify a Tiling Domain.\nNow using no tiling." << endl;
					RMDBGMIDDLE(0, RMDebug::module_storagemgr, "StorageLayout", "getLayout(" << tileDomain << ") Regular Tiling without Tiling Domain");
					retval.push_back(tileDomain);
					}
				break;
			case r_InterestTiling:
			case r_StatisticalTiling:
			case r_AlignedTiling:
			case r_DirectionalTiling:
			case r_SizeTiling:
				RMInit::logOut << "Tiling Scheme " << myLayout->getTilingScheme() << " " << (int)myLayout->getTilingScheme() << " not supported" << endl;
				RMDBGMIDDLE(0, RMDebug::module_storagemgr, "StorageLayout", "getLayout(" << tileDomain << ") of " << myLayout->getOId() << " Tiling Scheme " << myLayout->getTilingScheme() << " " << (int)myLayout->getTilingScheme() << " not supported");
				retval.push_back(tileDomain);
				break;
			default:
				RMInit::logOut << "Unknown Tiling Scheme " << myLayout->getTilingScheme() << " " << (int)myLayout->getTilingScheme() << endl;
				RMDBGMIDDLE(0, RMDebug::module_storagemgr, "StorageLayout", "getLayout(" << tileDomain << ") of " << myLayout->getOId() << " unknown Tiling Scheme " << myLayout->getTilingScheme() << " " << (int)myLayout->getTilingScheme());
			case r_NoTiling:
				retval.push_back(tileDomain);
				break;
			}
	else
		retval.push_back(tileDomain);
	RMDBGEXIT(5, RMDebug::module_storagemgr, "StorageLayout", "getLayout(" << tileDomain << ")");
	return retval;
	}

StorageLayout::~StorageLayout()
	{	
	}

std::vector< r_Minterval >
StorageLayout::calcRegLayout(const r_Minterval& tileDomain) const
	{
	RMDBGENTER(4, RMDebug::module_storagemgr, "StorageLayout", "calcRegLayout(" << tileDomain << ") " << myLayout->getOId());
	std::vector< r_Minterval > retval;
	r_Minterval base = myLayout->getTileConfiguration();
	RMDBGMIDDLE(5, RMDebug::module_storagemgr, "StorageLayout", "base: " << base)
	r_Point borigin = base.get_origin();
	r_Point bhigh = base.get_high();
	r_Point bextent = base.get_extent();
	r_Point torigin = tileDomain.get_origin();
	r_Point thigh = tileDomain.get_high();
	r_Point textent = tileDomain.get_extent();
	r_Dimension dim = base.dimension();
	r_Point transex(dim);
	r_Point transexmax(dim);
	r_Point transco(dim);
	r_Point transcotemp(dim);
	r_Minterval nextDomain(dim);
	r_Dimension counter2 = 0;
	r_Range extent = 0;
	r_Range extent2 = 0;
	r_Range origindiff = 0;
	r_Range highdiff = 0;
	for (counter2 = 0; counter2 < dim; counter2++)
		{
		origindiff = torigin[counter2] - borigin[counter2];
		highdiff = thigh[counter2] - bhigh[counter2];
		if (highdiff%bextent[counter2] > 0)
			transexmax[counter2] = highdiff/bextent[counter2] + 1;
		else
			transexmax[counter2] = highdiff/bextent[counter2];
		if (origindiff%bextent[counter2] < 0)
			transex[counter2] = origindiff/bextent[counter2] - 1;
		else
			transex[counter2] = origindiff/bextent[counter2];
		transco[counter2] = (transex[counter2]) * bextent[counter2];
		}
	for (extent = transex[dim - 1]; extent <= transexmax[dim - 1]; extent++)
		{
		transcotemp = transco;
		transcotemp[dim - 1] = bextent[dim - 1] * extent;
		nextDomain = base.create_translation(transcotemp);
		retval.push_back(nextDomain);
		
		for (counter2 = 0; counter2 < dim - 1; counter2++)
			{
			for (extent2 = transex[counter2] + 1; extent2 <= transexmax[counter2]; extent2++)
				{
				transcotemp[counter2] = bextent[counter2] * extent2;
				nextDomain = base.create_translation(transcotemp);
				retval.push_back(nextDomain);
				}
			}
		}
RMDBGIF(5, RMDebug::module_storagemgr, "StorageLayout", \
	for (std::vector< r_Minterval >::iterator i = retval.begin(); i != retval.end(); i++) \
		RMDBGMIDDLE(1, RMDebug::module_storagemgr, "StorageLayout", *i); \
);
	RMDBGEXIT(4, RMDebug::module_storagemgr, "StorageLayout", "calcRegLayout(" << tileDomain << ") " << myLayout->getOId());
	return retval;
	}

