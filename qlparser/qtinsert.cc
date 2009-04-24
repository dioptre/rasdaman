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
/*************************************************************
 *
 *
 * PURPOSE:
 *
 *
 * COMMENTS:
 *
 ************************************************************/

static const char rcsid[] = "@(#)qlparser, QtInsert: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtinsert.cc,v 1.27 2003/12/27 20:40:21 rasdev Exp $";

#include "qlparser/qtinsert.hh"
#include "qlparser/qtdata.hh"
#include "qlparser/qtmdd.hh"

#include "tilemgr/tile.hh"
#include "mddmgr/mddcoll.hh"
#include "mddmgr/mddobj.hh"

#include <iostream>

#include "servercomm/servercomm.hh"

extern ServerComm::ClientTblElt* currentClientTblElt;

const QtNode::QtNodeType QtInsert::nodeType = QtNode::QT_INSERT;


QtInsert::QtInsert( const std::string& initCollectionName, QtOperation* initSource )
  : QtExecute(), collectionName( initCollectionName ), source( initSource )
{
  source->setParent( this );
}



QtInsert::~QtInsert()
{
  if( source )
  {
    delete source;
    source=NULL;
  }
}



int
QtInsert::evaluate()
{
  RMDBCLASS( "QtInsert", "evaluate()", "qlparser", __FILE__, __LINE__ )

  // empty data list for evaluation of insert expression including constant
  QtNode::QtDataList* nextTupel = new QtNode::QtDataList(0);

  // get the operands
  QtData* sourceData = source->evaluate( nextTupel );

  if( sourceData )
  {
    QtMDD*  sourceMDD = (QtMDD*) sourceData;
    MDDObj* sourceObj = sourceMDD->getMDDObject();

    MDDColl* persColl = NULL;
    MDDColl* almost = NULL;

    try
    {
      almost = MDDColl::getMDDCollection( collectionName.c_str() );
    }
    catch(...)
    {
      
      RMInit::logOut << "Error: QtInsert::evaluate() - collection name not found" << std::endl;

      // delete the operand
      if( sourceData ) sourceData->deleteRef();

      parseInfo.setErrorNo(355);
      throw parseInfo;
    }
if (!almost->isPersistent())
	{
	RMInit::logOut << "QtInsert: User tries to insert into system table" << std::endl;
      if( sourceData ) sourceData->deleteRef();

      parseInfo.setErrorNo(355);
      throw parseInfo;
	}
else	{
	persColl = (MDDColl*)almost;
	}

    //
    // check MDD and collection type for compatibility
    //

RMDBGIF(3, RMDebug::module_qlparser, "QtInsert", \
      char* collTypeStructure = persColl->getCollectionType()->getTypeStructure(); \
      char* mddTypeStructure  = sourceObj->getMDDBaseType()->getTypeStructure(); \
      RMInit::dbgOut << std::endl << "Collection type structure.: " << collTypeStructure << std::endl \
                             << "MDD type structure........: " << mddTypeStructure << std::endl \
                             << "MDD domain................: " << sourceObj->getDefinitionDomain() << std::endl; \
      free( collTypeStructure ); collTypeStructure=NULL; \
      free( mddTypeStructure ); mddTypeStructure=NULL; )

    // bug fix: "insert into" found claimed non-existing type mismatch -- PB 2003-aug-25, based on fix by K.Hahn
    // if( !persColl->getCollectionType()->compatibleWith( (Type*) sourceObj->getMDDBaseType() ) )
    if( !((MDDType*) sourceObj->getMDDBaseType())->compatibleWith( persColl->getCollectionType()->getMDDType() ) )
    {
      // free resources
      persColl->releaseAll();
      delete persColl;
      persColl=NULL;
      if( sourceData ) sourceData->deleteRef();   // delete the operand

      // return error
      RMInit::logOut << "Error: QtInsert::evaluate() - MDD and collection types are incompatible" << std::endl;
      parseInfo.setErrorNo(959);
      throw parseInfo;
    }

    if( !persColl->getCollectionType()->getMDDType()->compatibleWithDomain( &(sourceObj->getDefinitionDomain()) ) )
    {
      // free resources
      persColl->releaseAll();
      delete persColl;
      persColl=NULL;
      if( sourceData ) sourceData->deleteRef();   // delete the operand

      // return error
      RMInit::logOut << "Error: QtInsert::evaluate() - MDD and collection domains are incompatible" << std::endl;
      parseInfo.setErrorNo(959);
      throw parseInfo;
    }

    //
    // convert a transient MDD object to a persistent one
    //

    // allocate a new oid within the current db
    OId oid;
#ifdef BASEDB_O2
    if( !OId::allocateMDDOId( &oid ) )
    {
#else
    OId::allocateOId( oid, OId::MDDOID );
#endif
      // get all tiles
      vector<Tile*>* sourceTiles = sourceObj->getTiles();

      // get a persistent type pointer
      MDDBaseType* persMDDType = (MDDBaseType*)TypeFactory::ensurePersistence( (Type*)sourceObj->getMDDBaseType() );

      if( !persMDDType )
      {
        RMInit::logOut << "Error: QtInsert::evaluate() - type not persistent" << std::endl;

        // delete dynamic data
        if( sourceData ) sourceData->deleteRef();
        delete sourceTiles;
        sourceTiles=NULL;
        delete nextTupel;
        nextTupel=NULL;
        persColl->releaseAll();
        delete persColl;
        persColl=NULL;
        parseInfo.setErrorNo(964);
        throw parseInfo;        
      }

      // create a persistent MDD object
      // need a StorageLayout here
      StorageLayout tempStorageLayout;
      tempStorageLayout.setTileSize(StorageLayout::DefaultTileSize);
      tempStorageLayout.setIndexType(StorageLayout::DefaultIndexType);
      tempStorageLayout.setTilingScheme(StorageLayout::DefaultTilingScheme);
      if (sourceObj->getDefinitionDomain().dimension() == 
	  StorageLayout::DefaultTileConfiguration.dimension())
	tempStorageLayout.setTileConfiguration(StorageLayout::DefaultTileConfiguration);
      MDDObj*  persMDDObj = new MDDObj( persMDDType, sourceObj->getDefinitionDomain(), oid, 
						tempStorageLayout );

      // iterate over source tiles
      for( vector<Tile*>::iterator sourceIt = sourceTiles->begin(); sourceIt != sourceTiles->end(); sourceIt++ )
      {
        // create a new persistent tile, copy the transient data, and insert it into the target mdd object
        Tile* newPersTile = new Tile( (*sourceIt)->getDomain(), persMDDType->getBaseType(), (*sourceIt)->getDataFormat() );
        newPersTile->copyTile( (*sourceIt)->getDomain(), *sourceIt, (*sourceIt)->getDomain() );
        persMDDObj->insertTile( newPersTile );
      }

      // delete tile vector
      delete sourceTiles;
      sourceTiles=NULL;
      persColl->insert( persMDDObj );
#ifdef BASEDB_O2
    }
    else
    {
      RMInit::logOut << "Error: QtInsert::evaluate() - allocation of oid failed" << std::endl;

      // delete dynamic data
      if( sourceData ) sourceData->deleteRef();
      delete nextTupel;
      nextTupel=NULL;
      persColl->releaseAll();
      delete persColl;
      persColl=NULL;
      parseInfo.setErrorNo(958);
      throw parseInfo;
    }
#else
#endif
    // free transient memory
    persColl->releaseAll();
    delete persColl;
    persColl=NULL;
  }
  else
    RMInit::logOut << "Error: QtInsert::evaluate() - insert data is invalid." << std::endl;

  // delete source operand
  if( sourceData ) sourceData->deleteRef();

  // delete dummy tupel vector 
  delete nextTupel;
  nextTupel=NULL;

  return 0;
}



QtNode::QtNodeList*
QtInsert::getChilds( QtChildType flag )
{
  QtNodeList* resultList=NULL;

    RMDBGENTER(0, RMDebug::module_qlparser, "QtInsert", "QtInsert::getChilds()")

  if( source )
  {
    // allocate resultList
    if( flag == QT_DIRECT_CHILDS );
      resultList = new QtNodeList();

    if( flag == QT_LEAF_NODES || flag == QT_ALL_NODES )
      resultList = source->getChilds( flag );

    // add the nodes of the current level
    if( flag == QT_DIRECT_CHILDS || flag == QT_ALL_NODES )
      resultList->push_back( source );
  }
  else
    resultList = new QtNodeList();


    RMDBGEXIT(0, RMDebug::module_qlparser, "QtInsert", "QtInsert::getChilds()")
  return resultList;
}



void
QtInsert::printTree( int tab, std::ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtInsert Object" << std::endl;

  if( mode != QtNode::QT_DIRECT_CHILDS )
  {
    if( source )
    {
      s << SPACE_STR(tab).c_str() << "source : " << std::endl;
      source->printTree( tab + 2, s );
    }
    else
      s << SPACE_STR(tab).c_str() << "no source" << std::endl;

    s << std::endl;
  }
}



void
QtInsert::printAlgebraicExpression( std::ostream& s )
{
  s << "insert<";

  if( source )
    source->printAlgebraicExpression( s );
  else
    s << "<no source>";

  s << ">";
}


QtOperation*
QtInsert::getSource()
{
	return source;
}


/*
void
QtInsert::preOptimize()
{
  if( source )
    source->optimizeLoad( new QtNode::QtTrimList );
}
*/


void
QtInsert::checkType()
{
  RMDBCLASS( "QtInsert", "checkType()", "qlparser", __FILE__, __LINE__ )

  // check operand branches
  if( source )
  {

  // get input type
  const QtTypeElement& inputType = source->checkType(); 

  if( inputType.getDataType() != QT_MDD )
  {
    RMInit::logOut << "Error: QtInsert::checkType() - insert expression must be of type r_Marray<T>" << std::endl;
    parseInfo.setErrorNo(960);
    throw parseInfo;
  }
  }
  else
    RMInit::logOut << "Error: QtInsert::checkType() - operand branch invalid." << std::endl;
}
