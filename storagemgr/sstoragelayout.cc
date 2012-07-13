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
 * SOURCE: sstoragelayout.cc
 *
 * MODULE: indexmgr
 * CLASS:	StorageLayout
 *
 * CHANGE HISTORY (append further entries):
 * when				 who				what
 * -----------------------------------------------------------------------
 * 10-Sep-97		furtado		creation of preliminary version.
 * 09-Oct-97		sivan			set, get functions
 * 13-Oct-97		furtado		extended functionality, class hierarchy
 * 4-Nov-98		furtado		added RegDirIx< >.
 * 13-Nov-00		hoefner		startet to do something with this class
 * 07-Jan-09            Shams           add tiling set to the getLayout method 
 * 07-Jan-09            Shams           add some methods for supporting tiling
 * COMMENTS:
 *	 none
 *
 ****************************************************************************/

#include "sstoragelayout.hh"
#include <stdlib.h>
#include "raslib/rmdebug.hh"
#include "rasodmg/alignedtiling.hh"
#include "rasodmg/dirtiling.hh"
#include "rasodmg/stattiling.hh"

const r_Bytes	StorageLayout::DBSPageSize = 4096; 

r_Bytes		StorageLayout::DefaultMinimalTileSize = DBSPageSize;

r_Bytes		StorageLayout::DefaultPCTMax = 2 * DBSPageSize;

r_Bytes		StorageLayout::DefaultTileSize = 32 * DBSPageSize;

unsigned int		StorageLayout::DefaultIndexSize = 0;

r_Index_Type		StorageLayout::DefaultIndexType = r_RPlus_Tree_Index; // DirTilesIx; // AutoIx; 

r_Tiling_Scheme		StorageLayout::DefaultTilingScheme = r_DirectionalTiling;

r_Minterval		StorageLayout::DefaultTileConfiguration("[0:511,0:511]");

r_Data_Format		StorageLayout::DefaultDataFormat = r_Array;

StorageLayout::StorageLayout(r_Index_Type ixType)
	:	myLayout(new DBStorageLayout())
	{
	setIndexType(ixType);
	extraFeatures = new StgMddConfig();
	RMDBGONCE(4, RMDebug::module_storagemgr, "StorageLayout", "StorageLayout(" << ixType << ")")
	}

StorageLayout::StorageLayout()
	:	myLayout(new DBStorageLayout())
	{
	extraFeatures = new StgMddConfig();
	RMDBGONCE(4, RMDebug::module_storagemgr, "StorageLayout", "StorageLayout()")
	}

StorageLayout::StorageLayout(const DBStorageLayoutId& id) 
	:	myLayout(id)	
	{
	extraFeatures = new StgMddConfig();
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

void
StorageLayout::setBBoxes(const vector<r_Minterval>& input) {
    extraFeatures->setBBoxes(input);
}

void
StorageLayout::setSubTiling()
{extraFeatures->setSubTiling();}

void
StorageLayout::resetSubTiling()
{extraFeatures->resetSubTiling();}

void
StorageLayout::setInterestThreshold(double i){
    extraFeatures->setInterestThreshold(i);
}

void
StorageLayout::setBorderThreshold(unsigned int b){
    extraFeatures->setBorderThreshold(b);
}

void
StorageLayout::setCellSize(int i){
    extraFeatures->setCellSize(i);
}


void
StorageLayout::setDirDecomp(vector<r_Dir_Decompose>* dir){
    vector<r_Dir_Decompose> dec;
    for(int i = 0 ; i < dir->size() ; ++i){
        dec.push_back(dir->at(i));
    }
    extraFeatures->setDirDecompose(dec);
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
                    retval = calcInterestLayout(tileDomain);
                    RMInit::logOut << "Interest Tiling chosen Everything is OK" << endl;
                    RMDBGMIDDLE(0, RMDebug::module_storagemgr, "StorageLayout", "getLayout(" << tileDomain << ") Interest Tiling");
                break;
            case r_StatisticalTiling:
                    retval = calcStatisticLayout(tileDomain);
                    RMInit::logOut << "Statistical Tiling chosen" << endl;
                break;
            case r_AlignedTiling:
                if (myLayout->getTileConfiguration().dimension() == tileDomain.dimension())
                    retval = calcAlignedLayout(tileDomain);
                else {
                    RMInit::logOut << "Aligned Tiling chosen without Tiling Domain!\nPlease specify a Tiling Domain.\nNow using no tiling." << endl;
                    RMDBGMIDDLE(0, RMDebug::module_storagemgr, "StorageLayout", "getLayout(" << tileDomain << ") Aligned Tiling without Tiling Domain");
                    retval.push_back(tileDomain);
                }
                break;

            case r_DirectionalTiling:
//                if (myLayout->getTileConfiguration().dimension() == tileDomain.dimension())
                    retval = calcDirectionalLayout(tileDomain);
//                else {
                    RMInit::logOut << "Directional Tiling chosen. Everything is OK." << endl;
//                    RMDBGMIDDLE(0, RMDebug::module_storagemgr, "StorageLayout", "getLayout(" << tileDomain << ") Directional Tiling without Tiling Domain");
//                    retval.push_back(tileDomain);
  //              }
                break;

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
        RMInit::logOut << "CalcReg Tiling: tile number: " << retval.size() << endl;
	return retval;
	}

std::vector< r_Minterval >
StorageLayout::calcInterestLayout(const r_Minterval& tileDomain) {
    RMInit::logOut << "Entering CalcInterest Tiling" << endl;
    r_Interest_Tiling* tiling = new r_Interest_Tiling
            (tileDomain.dimension(), extraFeatures->getBBoxes(), myLayout->getTileSize(), r_Interest_Tiling::SUB_TILING);
    std::vector<r_Minterval> ret;
    std::vector<r_Minterval>* ret1 = tiling->compute_tiles
            (tileDomain, extraFeatures->getCellSize());
    RMInit::logOut << "CalcInterest Tiling: tile number: " << ret1->size() << endl;
    for (int i = 0; i < ret1->size(); i++) {
        ret.push_back(ret1->at(i));
    }
    RMInit::logOut << "CalcInterest Tiling: tile number2: " << ret.size() << endl;
    return ret;
}

std::vector< r_Minterval >
StorageLayout::calcAlignedLayout(const r_Minterval& tileDomain) {
    RMInit::logOut << "Entering CalcAligned Tiling" << endl;
    r_Aligned_Tiling* tiling = new r_Aligned_Tiling
            (myLayout->getTileConfiguration(),myLayout->getTileSize());
    std::vector<r_Minterval> ret;
    std::vector<r_Minterval>* ret1 = tiling->compute_tiles
    (tileDomain, extraFeatures->getCellSize());
    RMInit::logOut << "CalcAligned Tiling: tile number: " << ret1->size() << endl;
    for (int i = 0; i < ret1->size(); i++) {
        ret.push_back(ret1->at(i));
    }
    RMInit::logOut << "CalcAligned Tiling: tile number2: " << ret.size() << endl;
    return ret;
}

std::vector< r_Minterval >
StorageLayout::calcDirectionalLayout(const r_Minterval& tileDomain){
    RMInit::logOut << "Entering CalcDir Tiling" << endl;
    r_Dir_Tiling* dirTile ;
    if(!extraFeatures->getSubTiling())
        dirTile = new r_Dir_Tiling(tileDomain.dimension(),
                extraFeatures->getDirDecompose(),myLayout->getTileSize(),
                r_Dir_Tiling::WITHOUT_SUBTILING);
    else
        dirTile = new r_Dir_Tiling(tileDomain.dimension(),
            extraFeatures->getDirDecompose(),myLayout->getTileSize(),
            r_Dir_Tiling::WITH_SUBTILING);

    std::vector<r_Minterval> ret;
    std::vector<r_Minterval>* ret1 = dirTile->compute_tiles
    (tileDomain, extraFeatures->getCellSize());
    RMInit::logOut << "CalcDir Tiling: tile number: " << ret1->size() << endl;
    for (int i = 0; i < ret1->size(); i++) {
        ret.push_back(ret1->at(i));
    }
    RMInit::logOut << "End of dirTiling Method Tile numbers = "<< ret.size();
    return ret;
}

std::vector< r_Minterval >
StorageLayout::calcStatisticLayout(const r_Minterval& tileDomain){
    RMInit::logOut << "Entering CalcStatistic Tiling" << endl;
    std::vector<r_Minterval> ret;
    RMInit::logOut << "ret is created" << endl;
    RMInit::logOut << "bboxes are " << extraFeatures->getBBoxes().size() << endl;
    vector<r_Minterval> temp = extraFeatures->getBBoxes();
    RMInit::logOut << "bboxes are ok " << temp.size() << endl;
    RMInit::logOut << "Statistic areas are "<< endl;
    std::vector<r_Access> accesses;
    RMInit::logOut << "Beginning of For"<< endl;
    for(int i = 0 ; i < temp.size() ; ++i){
        r_Minterval area = temp.at(i);
        r_Access ac(area);
        RMInit::logOut << ac;
        accesses.push_back(ac);
    }
    RMInit::logOut << "\nStatistic End of for"<<endl;
    unsigned int borderT;
    double interestT;
    if(extraFeatures->getBorderThreshold() < 0)
        borderT = r_Stat_Tiling::DEF_BORDER_THR;
    else
        borderT = extraFeatures->getBorderThreshold();
    if(extraFeatures->getInterestThreshold() < 0)
        interestT = r_Stat_Tiling::DEF_INTERESTING_THR;
    else
        interestT = extraFeatures->getInterestThreshold();
    RMInit::logOut << "Object is : " <<tileDomain.dimension() <<" "<< accesses.size() << " "<<myLayout->getTileSize()<<" "<< borderT<< " "<< interestT <<endl;
    r_Stat_Tiling* stat = new r_Stat_Tiling(tileDomain.dimension(),accesses,
            myLayout->getTileSize(), borderT,
            interestT);
    RMInit::logOut << "Object created" << endl;
    std::vector<r_Minterval>* ret1 = stat->compute_tiles(tileDomain,extraFeatures->getCellSize());
    RMInit::logOut << "End of CalcStatistic Tiling tile numbers = "<< ret.size() << endl;
    for (int i = 0; i < ret1->size(); i++) {
        ret.push_back(ret1->at(i));
    }

    return ret;
}
