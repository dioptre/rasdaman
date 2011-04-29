#include "mymalloc/mymalloc.h"
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

using namespace std;

#include "server/createinitmdd.hh"
#include <iomanip>
#include "reladminif/objectbroker.hh"
#include "raslib/mitera.hh"

// #undef DEBUG_HH
// #include "debug.hh"
#define ENTER(a)	RMInit::logOut << "ENTER " << a << endl << flush;
#define LEAVE(a)	RMInit::logOut << "LEAVE " << a << endl << flush;

//#include <akgtime.hh>



FastCollectionCreator::FastCollectionCreator(const char *collName, const char *collTypeName)
  {
     ENTER( "FastCollectionCreator::FastCollectionCreator(" << collName << "," << collTypeName << ")" );
     collectionName     = collName;
     
     collectionTypeName = collTypeName;
     LEAVE( "FastCollectionCreator::FastCollectionCreator()" );
   }
      
r_OId FastCollectionCreator::createCollection()
  {
    ENTER( "FastCollectionCreator::createCollection()" );

    verifyName(collectionName);
    
    // allocate a new OId 
    EOId eOId;  EOId::allocateEOId( eOId, OId::MDDCOLLOID );
    r_OId oid = r_OId( eOId.getSystemName(), eOId.getBaseName(), eOId.getOId() );

    CollectionType* collType = (CollectionType*)TypeFactory::mapSetType( collectionTypeName );

    RMInit::logOut<<"Creating collection "<<collectionName<<" with type "<<collectionTypeName<<"...";
    if( collType )
    {
      try
      {
	MDDColl* coll = MDDColl::createMDDCollection(collectionName, OId(oid.get_local_oid()), collType);
        delete coll;
        RMInit::logOut << "OK" << std::endl;
      }
      catch( r_Error& obj )
      {
	if (obj.get_kind() == r_Error::r_Error_NameNotUnique)
	  {
            RMInit::logOut << "collection name exists already... FAILED" << std::endl;
	   }
	else
	  { 
	    RMInit::logOut << obj.get_errorno() << " " << obj.what() << std::endl;
	   }
	throw;   
      }
    }
    else
    {
      RMInit::logOut << "collection type not found... FAILED" << std::endl;
      throw r_Error(COLLTYPE_NULL);
    }
    
    ENTER( "FastCollectionCreator::createCollection()" );
    return oid;
   }

void FastCollectionCreator::verifyName( const char* name ) throw(r_Error)
  {
    ENTER( "FastCollectionCreator::verifyName(" << (long) name << ")" );

    if(!name) 
      { 
        RMInit::logOut << "FastCollectionCreator::verifyName() name is null!" << std::endl;
        throw r_Error(INVALIDOBJECTNAME);
       }
  
  const char* cptr=name;
  
  //check if the name contains only [a-zA-Z0-9_]
  while(*cptr) {
   if( ((*cptr >= 'a') && (*cptr <='z')) ||
       ((*cptr >= 'A') && (*cptr <='Z')) ||
       ((*cptr >= '0') && (*cptr <='9')) ||       
       (*cptr == '_') 			 )
       cptr++;
   else 
       break;
   }
  
  if(*cptr) {
   //invalid character in object name
   RMInit::logOut << "FastCollectionCreator::verifyName(" << name << ") invalid name!" << std::endl;
   throw r_Error(INVALIDOBJECTNAME);
  }

    LEAVE( "FastCollectionCreator::verifyName(" << name << ") -> ok" );
 }


//###################################################################################################

FastMDDCreator::FastMDDCreator()
  {    
    ENTER( "FastMDDCreator::FastMDDCreator()" );

    comprData  = 0;
    
    storageFormat = r_TMC;
    formatParams  = NULL;

    LEAVE( "FastMDDCreator::FastMDDCreator()" );
   }
   
FastMDDCreator::~FastMDDCreator()
  {
    ENTER( "FastMDDCreator::~FastMDDCreator()" );

    if(comprData) free(comprData);

    LEAVE( "FastMDDCreator::~FastMDDCreator()" );
   }      

void FastMDDCreator::setCollectionName(const char* collName)
  {
    ENTER( "FastMDDCreator::setCollectionName(" << collName << ")" );

    collectionName = collName;

    LEAVE( "FastMDDCreator::setCollectionName()" );
   }
   
void FastMDDCreator::setMDDTypeName(const char* _mddTypeName)
  {
    ENTER( "FastMDDCreator::setMDDTypeName(" << _mddTypeName << ")" );

    mddTypeName = _mddTypeName;

    LEAVE( "FastMDDCreator::setMDDTypeName()" );
   }
         
void FastMDDCreator::verifyCompatibility(MDDColl* collection) throw (r_Error)
  {
    ENTER( "FastMDDCreator::verifyCompatibility(_)" );

    if (collection->isPersistent())
      {
        //RMInit::logOut<<"OK, colection exists and is  persistent"<<endl;
       } 
    else
      {
       throw r_Error(SYSTEM_COLLECTION_NOT_WRITABLE);
      }
   
   char* collTypeStructure = collection->getCollectionType()->getTypeStructure(); 
   //RMInit::logOut<<"collTypeStructure="<<collTypeStructure<<endl;
   
   const MDDType* mddType = TypeFactory::mapMDDType( mddTypeName.c_str() );
   if(mddType == NULL) throw r_Error(MDDTYPE_NULL);
   
   char* mddTypeStructure  = mddType->getTypeStructure(); 
   //RMInit::logOut<<"mddTypeStructure="<<mddTypeStructure<<endl;
   
   
   if(mddType->compatibleWithDomain( &definitionInterval ))
     {
       //RMInit::logOut<<"compatibil with domain: "<<definitionInterval<<endl;
      }
   else throw r_Error(r_Error::r_Error_CollectionElementTypeMismatch);
    //RMInit::logOut<<"incompatibil with domain"<<endl;   
   
   if(collection->getCollectionType()->compatibleWith( mddType ))
     {
       //RMInit::logOut<<"compatibil with collection"<<endl;
      }
   else
     throw r_Error(r_Error::r_Error_CollectionElementTypeMismatch);
     //RMInit::logOut<<"incompatibil with collection"<<endl;   
     
   free( collTypeStructure ); 
   free( mddTypeStructure );   

    LEAVE( "FastMDDCreator::verifyCompatibility()" );
   }

r_OId FastMDDCreator::createMDD(const char *domain)
  {
    ENTER( "FastMDDCreator::createMDD(" << domain << ")" );

    definitionInterval = r_Minterval(domain);
    
    MDDColl* collection = MDDColl::getMDDCollection( collectionName.c_str() );
    
    verifyCompatibility(collection);
    
    const MDDType* mddType    = TypeFactory::mapMDDType( mddTypeName.c_str() );
    MDDBaseType*  mddBaseType = (MDDBaseType*)mddType;

    //allocate oid-ul;
    EOId eOId; EOId::allocateEOId( eOId, OId::MDDOID );
   
    mddOId = r_OId( eOId.getSystemName(), eOId.getBaseName(), eOId.getOId() );
   
    StorageLayout ms;
    ms.setTileSize(StorageLayout::DefaultTileSize);
    ms.setIndexType(StorageLayout::DefaultIndexType);
    ms.setTilingScheme(StorageLayout::DefaultTilingScheme);
    if (definitionInterval.dimension() == StorageLayout::DefaultTileConfiguration.dimension())
	ms.setTileConfiguration(StorageLayout::DefaultTileConfiguration);
   
    mymdd = new MDDObj(mddBaseType, definitionInterval, eOId.getOId(), ms);
   
    cellSize = mymdd->getCellType()->getSize();
   
    collection->insert( mymdd );
    
    collection->releaseAll();
       
    delete collection;
    
    LEAVE( "FastMDDCreator::createMDD() -> " << mddOId );
    return mddOId;
   }
   
r_OId FastMDDCreator::createRCxMDD(const char *domain, const char *tileDomain)
  {
    ENTER( "FastMDDCreator::createRCxMDD(" << domain << "," << tileDomain << ")" );

    definitionInterval = r_Minterval(domain);
    r_Minterval tileInterval = r_Minterval(tileDomain);
    
    MDDColl* collection = MDDColl::getMDDCollection( collectionName.c_str() );
    
    verifyCompatibility(collection);
    
    const MDDType* mddType    = TypeFactory::mapMDDType( mddTypeName.c_str() );
    MDDBaseType*  mddBaseType = (MDDBaseType*)mddType;

    //allocate oid-ul;
    EOId eOId; EOId::allocateEOId( eOId, OId::MDDOID );
   
    mddOId = r_OId( eOId.getSystemName(), eOId.getBaseName(), eOId.getOId() );
   
    StorageLayout ms;
    ms.setTileSize(tileInterval.cell_count() * mddBaseType->getBaseType()->getSize());
    ms.setDataFormat(r_ZLib);
    ms.setIndexType(r_Reg_Computed_Index);
    ms.setTilingScheme(r_RegularTiling);
    ms.setTileConfiguration(tileInterval);
    mymdd = new MDDObj(mddBaseType, definitionInterval, eOId.getOId(), ms);
   
         
    cellSize = mymdd->getCellType()->getSize();
   
    collection->insert( mymdd );
    
    collection->releaseAll();
       
    delete collection;
    
    LEAVE( "FastMDDCreator::createRCxMDD() -> " << mddOId );
    return mddOId;
   }

vector<r_Minterval> FastMDDCreator::getTileDomains(r_OId mddOId, const char *stripeDomain)
  {
    ENTER( "FastMDDCreator::getTileDomains(" << mddOId << "," << stripeDomain << ")" );

    mymdd = new MDDObj( OId(mddOId.get_local_oid()) );
    
    r_Minterval stripeInterval(stripeDomain);

    vector< Tile* >* tiles = mymdd->intersect(stripeInterval);
    
    vector<r_Minterval> result;
    
    for(int i=0; i < tiles->size(); i++)
      {
        result.push_back( (*tiles)[i]->getDomain());
       }
    delete mymdd;

    LEAVE( "FastMDDCreator::getTileDomains()" );
    return result;
   }

void FastMDDCreator::addStripe(r_OId _mddOId, const char *stripeDomain, const char *tileDomain)
  {
    ENTER( "FastMDDCreator::addStripe(" << _mddOId << "," << stripeDomain << "," << tileDomain << ")" );

    mddOId = _mddOId;
    
    r_Minterval stripeInterval(stripeDomain);
    r_Minterval tileInterval(tileDomain);
    
    
    mymdd = new MDDObj( OId(mddOId.get_local_oid()) );
    cellSize = mymdd->getCellType()->getSize();
    const BaseType* baseType = mymdd->getMDDBaseType()->getBaseType();
    
    
    r_MiterArea iter(&tileInterval, &stripeInterval);
    
    while (!iter.isDone())
     {//iterate through the partitions in the search domain
	r_Minterval currentSlInterval = iter.nextArea();
        //RMInit::logOut<<"inserting tile: "<< currentSlInterval<<endl;
        
	createCompressedTileData(currentSlInterval, baseType);
   
        Tile* tile = new Tile( currentSlInterval, baseType, comprData, true, comprDataSize, storageFormat);
        tile->setParameters(formatParams);				
        tile->setPersistent(true);

        mymdd->insertTile( tile );
      }
    
    delete mymdd;    

    LEAVE( "FastMDDCreator::addStripe()" );
   }


void FastMDDCreator::createCompressedTileData(r_Minterval& tileInterval, const BaseType* baseType)
  {
    ENTER( "FastMDDCreator::createCompressedTileData(" << tileInterval << "," << baseType << ")" );

    static int lastSize = 0;
    int uncompressedSize = tileInterval.cell_count() * cellSize;
    
    if(comprData)
     {
       if(lastSize == uncompressedSize) 
         {
	   return;
          }	
        else { free(comprData); comprData = 0; }
      }	


    r_Data_Format comprMode = storageFormat; ;
			    			    
    char* dataPtr = (char*)mymalloc(uncompressedSize);  memset(dataPtr,0,uncompressedSize);

    r_Base_Type *compType = (r_Base_Type*)(r_Type::get_any_type(baseType->getTypeStructure()));
    
    r_Tile_Compression *engine = r_Tile_Compression::create(comprMode, tileInterval, compType);

    r_ULong newSize = uncompressedSize; 
    comprData = (char*)(engine->compress(dataPtr, newSize, formatParams));
    comprDataSize = newSize;
    delete engine;
    //RMInit::logOut<<"Compression of "<<uncompressedSize<<" of zeroes resulted in "<<comprDataSize<<" bytes"<<endl;   
        
    free(dataPtr);
    delete compType;
    
    lastSize= uncompressedSize;

    LEAVE( "FastMDDCreator::createCompressedTileData()" );
   }

