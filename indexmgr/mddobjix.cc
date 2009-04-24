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
 * SOURCE: mddobjix.cc
 *
 * MODULE: indexmgr
 * CLASS:	MDDObjIx
 *
 * COMMENTS:
 *	none
 *
*/
static const char rcsid[] = "@(#)mddobjix, MDDObjIx: $Id: mddobjix.cc,v 1.30 2002/07/24 14:33:51 hoefner Exp $";

#include <iostream>
#include <math.h>

#include "indexmgr/mddobjix.hh"
#include "raslib/rmdebug.hh"
#include "tilemgr/tile.hh"
#include "indexmgr/srptindexlogic.hh"
#include "indexmgr/sdirindexlogic.hh"
#include "indexmgr/srcindexlogic.hh"
#include "indexmgr/transdirix.hh"
#include "relindexif/hierindex.hh"
#include "relindexif/dbtcindex.hh"
#include "relindexif/dbrcindexds.hh"
#include "keyobject.hh"
#include "reladminif/lists.h"
#include "relblobif/tileid.hh"

void
MDDObjIx::setNewLastAccess(const r_Minterval& newLastAccess, const std::vector<Tile*>* newLastTiles)
	{
	lastAccess = newLastAccess;
	releasePersTiles();
	lastAccessTiles = *newLastTiles;
	}

void
MDDObjIx::setNewLastAccess(const Tile* newLastTile, bool clear)
	{
	if (clear)
		{	
		releasePersTiles();
		lastAccessTiles.erase(lastAccessTiles.begin(), lastAccessTiles.end());
		}
	if (newLastTile)
		{
		r_Minterval region = newLastTile->getDomain();
		lastAccess = region;
		lastAccessTiles.push_back((Tile*)newLastTile);
		}
	}

bool
MDDObjIx::removeTileFromLastAccesses(const Tile* tileToRemove)
	{
	bool found = false;
	std::vector<Tile*>::iterator iter; 
	for (iter = lastAccessTiles.begin(); iter != lastAccessTiles.end() ; iter++)
		{
		if (*iter == tileToRemove) 
			{
			found = true;
			lastAccessTiles.erase(iter);
			break;
			}
		}
	if (found)
		{
		r_Minterval emptyInterval;
		lastAccess = emptyInterval;
		}
	return found;
	}

std::vector< Tile* >* 
MDDObjIx::lastAccessIntersect(const r_Minterval& searchInter) const
	{
	std::vector< Tile* >* interResult = 0;
	if ((lastAccess.dimension() != 0) && (lastAccess.covers(searchInter))) 
		{
		RMDBGONCE(6, RMDebug::module_indexmgr, "MDDObjIx", "lastAccessIntersect Search in the cache ")
		interResult = new std::vector< Tile* >();
		interResult->reserve(10);
		for (int i = 0; i < lastAccessTiles.size(); i++)
			{
			if (lastAccessTiles[i]->getDomain().intersects_with(searchInter))
				interResult->push_back(lastAccessTiles[i]);
			}
		if (interResult->size() == 0)
			{
			delete interResult;
			interResult = NULL;
			}
		}
	return interResult; 
	}

Tile* 
MDDObjIx::lastAccessPointQuery(const r_Point& searchPoint) const
	{
	Tile* result = 0;
	
	if ((lastAccess.dimension() != 0) && (lastAccess.covers(searchPoint)))	
		{
		for (int i = 0; !result && i < lastAccessTiles.size(); i++)
			{
			if (lastAccessTiles[i]->getDomain().covers(searchPoint))
				{
				result = lastAccessTiles[i];
				}
			}
		}
	return (result); 
	}

void 
MDDObjIx::releasePersTiles() 
	{
	RMDBGENTER(6, RMDebug::module_indexmgr, "MDDObjIx", "releasePersTiles()")
	if (isPersistent())
		{
		Tile* t = NULL;
		for(int i = 0; i < lastAccessTiles.size(); i++)
			{
			t = lastAccessTiles[i];
			delete t;
			lastAccessTiles[i] = NULL;
			}
		lastAccessTiles.clear();
		}
	RMDBGEXIT(6, RMDebug::module_indexmgr, "MDDObjIx", "releasePersTiles()")
	}

void	
MDDObjIx::printStatus(unsigned int level, std::ostream& stream) const
	{
	stream << "MDDObjIx [ last access interval = " << lastAccess << " tile cache size = " << lastAccessTiles.size() << " index structure = ";
	actualIx->printStatus(level, stream);
	} 

DBObjectId 
MDDObjIx::getDBMDDObjIxId() const
	{
	return actualIx;
	} 

r_Minterval
MDDObjIx::getCurrentDomain() const	
	{
	return actualIx->getCoveredDomain();
	}

r_Dimension 
MDDObjIx::getDimension() const
	{
	return actualIx->getDimension();
	}

#ifdef RMANBENCHMARK
void
MDDObjIx::initializeTimerPointers() 
	{
	pointQueryTimer= new RMTimer("DirIx", "pointQuery time	");
	intersectTimer = new RMTimer("DirIx", "intersect time	");
	getTilesTimer = new RMTimer("DirIx", "getTiles time	");
	} 
#endif

MDDObjIx::MDDObjIx(const StorageLayout& sl, const r_Minterval& dim, const BaseType* bt)
	:	cellBaseType(bt),
		myStorageLayout(sl),
		actualIx(0),
		_isPersistent(true)
	{
	RMDBGENTER(10, RMDebug::module_indexmgr, "MDDObjIx", "MDDObjIx(storage, " << dim << ", type)")
	lastAccessTiles.reserve(10); 
	if (bt == NULL)
		{
		RMDBGMIDDLE(10, RMDebug::module_indexmgr, "MDDObjIx", "TransIndex")
		_isPersistent = false;
		}
	initializeLogicStructure();
	if (isPersistent())
		{
		r_Range temp;
		switch(myStorageLayout.getIndexType())
			{
			case r_RPlus_Tree_Index: 
				actualIx = (HierIndexDS*)new DBHierIndex(dim.dimension(), false, true);
				break;
			case r_Reg_Computed_Index:
				actualIx = new DBRCIndexDS(dim, SRCIndexLogic::computeNumberOfTiles(myStorageLayout, dim));
				break;
			case r_Tile_Container_Index: 
				actualIx = (HierIndexDS*)new DBTCIndex(dim.dimension(), false);
				break;
			case r_Directory_Index:
				actualIx = (HierIndexDS*)new DBHierIndex(dim.dimension(), false, true);
				break;	
			case r_Auto_Index:
			default:
				// should never get here. If Auto_Index, a specific index was 
				RMDBGONCE(0, RMDebug::module_indexmgr, "MDDObjIx", "initializeLogicStructure() Auto_Index or unknown index chosen");
				throw r_Error(UNKNOWN_INDEX_TYPE);
				break;
			} 
		}	
	else	{
		//only dirindex supports transient indexes
		actualIx = new TransDirIx(dim.dimension());
		}
#ifdef RMANBENCHMARK
	initializeTimerPointers();
#endif
	RMDBGEXIT(10, RMDebug::module_indexmgr, "MDDObjIx", "MDDObjIx(storage, " << dim << ", type)")
	}

MDDObjIx::MDDObjIx(DBObjectId newDBIx, const StorageLayout& sl, const BaseType* bt)
	:	cellBaseType(bt),
		myStorageLayout(sl),
		actualIx(newDBIx),
		_isPersistent(true)
	{
	initializeLogicStructure();
	lastAccessTiles.reserve(10); 
#ifdef RMANBENCHMARK
	initializeTimerPointers();
#endif
	}

void
MDDObjIx::initializeLogicStructure()
	{
	switch(myStorageLayout.getIndexType())
		{
		case r_RPlus_Tree_Index: 
		case r_Tile_Container_Index: 
			do_getObjs = SRPTIndexLogic::getObjects;
			do_insertObj = SRPTIndexLogic::insertObject2;
			do_pointQuery = SRPTIndexLogic::containPointQuery2;
			do_removeObj = SRPTIndexLogic::removeObject;
			do_intersect = SRPTIndexLogic::intersect2;
			break;
		case r_Reg_Computed_Index: 
			do_getObjs = SRCIndexLogic::getObjects;
			do_insertObj = SRCIndexLogic::insertObject;
			do_pointQuery = SRCIndexLogic::containPointQuery;
			do_removeObj = SRCIndexLogic::removeObject;
			do_intersect = SRCIndexLogic::intersect;
			break;
		case r_Directory_Index:
			// chosen before this
			do_getObjs = SDirIndexLogic::getObjects;
			do_pointQuery = SDirIndexLogic::containPointQuery;
			do_removeObj = SDirIndexLogic::removeObject;
			do_intersect = SDirIndexLogic::intersect;
			do_insertObj = SDirIndexLogic::insertObject;
			break;	
		default:
		case r_Auto_Index:
			// should never get here. If Auto_Index, a specific index was 
			RMInit::logOut << "MDDObjIx::initializeLogicStructure() illegal index (" << myStorageLayout.getIndexType() << ") chosen!" << endl;
			throw r_Error(ILLEGAL_INDEX_TYPE);
			break;
		} 
	}


void	
MDDObjIx::insertTile(const Tile* newTile)
	{
	if (isPersistent())
		{
		((Tile*)newTile)->setPersistent();
		}
	KeyObject t(newTile);
	do_insertObj(actualIx, t, myStorageLayout);
	setNewLastAccess(newTile, false);
	}

bool
MDDObjIx::removeTile(const Tile* tileToRemove) 
	{
	RMDBGENTER(4, RMDebug::module_indexmgr, "MDDObjIx", "removeTile(Tile)")
	bool found = false;
	// removes from cache, if it's there
	removeTileFromLastAccesses(tileToRemove);
	
	// removes from the index itself
	KeyObject t(tileToRemove);
	found = do_removeObj(actualIx, t, myStorageLayout);
	RMDBGEXIT(4, RMDebug::module_indexmgr, "MDDObjIx", "removeTile(Tile)")
	return found;
	}

vector< Tile* >* 
MDDObjIx::intersect(const r_Minterval& searchInter) const
	{
	RMDBGENTER(4, RMDebug::module_indexmgr, "MDDObjIx", "intersect(" << searchInter << ")")
#ifdef RMANBENCHMARK
	if(RManBenchmark >= 3)	intersectTimer->start();
#endif
		
	vector< Tile* >* result = lastAccessIntersect(searchInter);
	if (!result)
		{
		KeyObjectVector resultKeys;
		do_intersect(actualIx, searchInter, resultKeys, myStorageLayout);
		result = new vector< Tile* >();
		if (!resultKeys.empty())
			{
			unsigned int resSize = resultKeys.size();
			result->reserve(resSize); 
			Tile* t = NULL;
			unsigned int i = 0;
			if (isPersistent())
				{
//this checks if there are double tiles in the result
				RMDBGIF(1, RMDebug::module_indexmgr, "MDDObjIx", \
				DomainMap t; \
				DomainMap::iterator it; \
				for (i = 0; i < resSize; i++) \
					{ \
					DomainPair p(resultKeys[i].getObject().getOId(), \
						resultKeys[i].getDomain()); \
					if ((it = t.find(p.first)) != t.end()) \
						{ \
						RMDBGMIDDLE(0, RMDebug::module_indexmgr, "MDDObjIx", \
							"intersect(" << searchInter << \
							") received double tile: " << \
							resultKeys[i]) \
						for (int i = 0; i < resultKeys.size(); i++) \
							{ \
							RMInit::dbgOut << resultKeys[i] << endl; \
							} \
						throw r_Error(TILE_MULTIPLE_TIMES_RETRIEVED); \
						} \
					t.insert(p); \
					} \
				);
				for (i = 0; i < resSize; i++)
					{
					RMDBGMIDDLE(4, RMDebug::module_indexmgr, "MDDObjIx", "received entry " << resultKeys[i])
					result->push_back(new Tile(resultKeys[i].getDomain(), cellBaseType, DBTileId(resultKeys[i].getObject())));
					}
				}
			else	{
				for (i = 0; i < resSize; i++)
					{
					result->push_back(resultKeys[i].getTransObject());
					}
				}
			((MDDObjIx*) this)->setNewLastAccess(searchInter, result);
			}
		}
#ifdef RMANBENCHMARK
	if(RManBenchmark >= 3) intersectTimer->stop();
#endif		
	RMDBGEXIT(4, RMDebug::module_indexmgr, "MDDObjIx", "intersect(" << searchInter << ") " << (void*)result)
	return result;
	}
		
char*	
MDDObjIx::pointQuery(const r_Point& searchPoint)
	{
	char* result = 0;
	
	Tile* resultTile = containPointQuery(searchPoint);

	if(resultTile)
		{
		result = resultTile->getCell(searchPoint); 
		}
	return result;
	}

const char*	
MDDObjIx::pointQuery(const r_Point& searchPoint) const
	{
	const char* result = 0;
	
	Tile* resultTile = containPointQuery(searchPoint);

	if(resultTile)
		{
		result = resultTile->getCell(searchPoint); 
		}
	return result;
	}

Tile*	
MDDObjIx::containPointQuery(const r_Point& searchPoint) const
	{
	
	Tile* resultTile = 0;
 
#ifdef RMANBENCHMARK
	if(RManBenchmark >= 4) pointQueryTimer->start();
#endif		
	resultTile = lastAccessPointQuery(searchPoint);
	if (!resultTile)
		{
		KeyObject resultKey;
		do_pointQuery(actualIx, searchPoint, resultKey, myStorageLayout);
		if (resultKey.isInitialised())
			{
			if (isPersistent())
				{
				resultTile = new Tile(resultKey.getDomain(), cellBaseType, DBTileId(resultKey.getObject()));
				// for rcindex
				((DBObject*)resultKey.getObject().ptr())->setCached(false);
				}
			else	{
				resultTile = resultKey.getTransObject();
				}
			((MDDObjIx*) this)->setNewLastAccess(resultTile);
			}
		}
	
#ifdef RMANBENCHMARK
	if (RManBenchmark >= 4) pointQueryTimer->stop();
#endif			
	return resultTile;
	}

vector< Tile* >* 
MDDObjIx::getTiles() const
	{
#ifdef RMANBENCHMARK
	if(RManBenchmark >= 3)	getTilesTimer->start();
#endif
	vector< Tile* >* result = NULL;
	KeyObjectVector resultKeys;
	do_getObjs(actualIx, resultKeys, myStorageLayout);
	if (!resultKeys.empty())
		{
		result = new vector< Tile* >();
		unsigned int resSize = resultKeys.size();
		result->reserve(resSize); 
		Tile* t = 0;
		if (isPersistent())
			{
//this checks if there are double tiles in the result
			RMDBGIF(1, RMDebug::module_indexmgr, "MDDObjIx", \
			DomainMap tmap; \
			DomainMap::iterator it; \
			for (int cnt = 0; cnt < resSize; cnt++) \
				{ \
				DomainPair p(resultKeys[cnt].getObject().getOId(), \
					resultKeys[cnt].getDomain()); \
				if ((it = tmap.find(p.first)) != tmap.end()) \
					{ \
					RMDBGMIDDLE(0, RMDebug::module_indexmgr, "MDDObjIx", \
						"getTiles() received double tile: " << \
						resultKeys[cnt]) \
					for (int cnt = 0; cnt < resultKeys.size(); cnt++) \
						{ \
						RMInit::dbgOut << resultKeys[cnt] << endl; \
						} \
					throw r_Error(TILE_MULTIPLE_TIMES_RETRIEVED ); \
					} \
				tmap.insert(p); \
				} \
			);
			for (int i = 0; i < resSize; i++)
				{
				result->push_back(new Tile(resultKeys[i].getDomain(), cellBaseType, DBTileId(resultKeys[i].getObject())));
				}
			}
		else	{
			for (int i = 0; i < resultKeys.size(); i++)
				{
				result->push_back(resultKeys[i].getTransObject());
				}
			}
		r_Minterval emptyInterval;
		((MDDObjIx*) this)->setNewLastAccess(emptyInterval, result);
		}
#ifdef RMANBENCHMARK
	if(RManBenchmark >= 3)	getTilesTimer->stop();
#endif	
	return result;
	}

	
bool	
MDDObjIx::isPersistent() const
	{
	return _isPersistent;
	}

MDDObjIx::~MDDObjIx() 
	{
	releasePersTiles();
	actualIx->destroy();
	actualIx = 0;

#ifdef RMANBENCHMARK
	pointQueryTimer->setOutput(0);
	if (pointQueryTimer) delete pointQueryTimer;
	intersectTimer->setOutput(0);
	if (intersectTimer) delete intersectTimer;
	getTilesTimer->setOutput(0);
	if (getTilesTimer) delete getTilesTimer;
#endif	
	} 
