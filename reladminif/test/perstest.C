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
#include "mymalloc/mymalloc.h"
#define TEST_PROTECTED 
#define TEST_PRIVATE

#include <stdlib.h>
#include <iostream.h>

#include "ulongtype.hh"
#include "cachetamgr/persmddcoll.hh"
#include "cachetamgr/persmddobj.hh"
#include "cachetamgr/perstile.hh"
#include "cachetamgr/persmddcolliter.hh"

#include "adminif.hh"
#include "databaseif.hh"
#include "transactionif.hh"
#include "oidif.hh"

#include "raslib/rminit.hh"
#include "typefactory.hh"

#include "dbmddobj.hh"
#include "dbmddcoll.hh"

#include "mddbasetype.hh"
#include "mdddomaintype.hh"
#include "settype.hh"

#include "externs.h"



/* 
  Global Variables 
*/

RMINITGLOBALS('C')

static char* O2DBName;
char *collName;
char defaultCollName[]= "ObjsContainer";
OId globalOId1, globalOId2;
OId globalCollOId;
EOId globalEOId[4];
int numObjsCreated;
TransactionIf ta;
int ExitNo;

/* 
  Functions
*/

int 
getOption( )
{
   unsigned int result;
   cout << endl;
   cout << "Choose Option : " << endl;
   cout << "-----------------------------------------------------------------"<<endl;
   cout << "  1 - Create MDD collection with name .. " << collName << endl;
   cout << "  2 - Populate collection with MDD objects (testConstructors) " << endl;
   cout << "  3 - Access MDD collection given name (testAccessing) " << endl;
   cout << "  4 - Access MDD collection given OId (testAccessing)" << endl;
   cout << "  5 - Access MDD object given OId (testAccessingMDDObj) " << endl;
   cout << "  6 - Remove MDD object from the collection given OId (testRemove) "<<endl;
   cout << "  7 - Test PersMDDObj::intersect( ) (testSearch) " <<endl;
   cout << "  8 - Test PersMDDColl::getCardinality( ) "<<endl;
   cout << "  9 - Test PersMDDObj get functions "<< endl;
   cout << " 10 - Remove MDD Collection given OId " << endl;
   cout << " 11 - Insert MDD given OId in a second collection "<< endl;
   cout << " 12 - Insert tile in MDD given OId "<< globalOId1 <<endl;
   cout << " 13 - Test removeTile from MDD "<< globalOId1 << endl;
   ExitNo = 14;
   cout << " " << ExitNo << " - Exit " << endl; 
   cout << "------------------------------------------------------------------"<<endl;
   cout << "Enter option:  ";
   cin >> result;
   switch (result)
   	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		return result;
		break;
	default:
		return getOption();
	}
}
      
// 1 - Create MDD collection
static int createMDDColl( const char* collName, DatabaseIf* db );

// 2 - Populate collection with MDD objects
static void testConstructors( char* cn );

// 3 - Retrieves an MDD collection with name cn and prints contents:
static void testAccessing( char* cn );

// 4 - Retrieves an MDD collection with OId o and prints contents:
static void testAccessing( OId o );

// 5 - Retrieves an MDD object with OId o and prints contents:
static void testAccessingMDDObj( OId o );

// Removes one of the MDD objects in the collection
static void testRemove( );

// 6 - Removes MDD object with OId o in the collection
static void testRemove( OId o );

// 7 - Test PersMDDObj::intersect( ) objects of the collection
static void testSearch( );

// 8 - Test PersMDDColl::getCardinality( )
static void testGetCardinality( char* cn);

// 9 - Scans collection and tests PersMDDObj get functions
static void testGetFunctions( );

//10 - Remove MDD collection given OId

//11 - Insert MDD given OId in a second collection
static void testInsertMDDObjColl( OId o, char* cn );

//12 - Test later insertion of tile in PersMDDObj
static void testLaterInsert( OId o );

//13 - Test removeTile from an MDD obj
static void testRemoveTile( OId o );

// Tries accessing several OIds, including ilegal ones, to test 
// several error conditions.
static void testAccessingOId( );


/*************************************************************
 * Function name.: int main( int argc, char** argv)
 *
 * Arguments.....: 
 *   argc: number of arguments given to program
 *   argv: array of char* with arguments
 * Return value..: exit status
 * Description...: none
 ************************************************************/
int
main( int argc, char** argv)
{
RManDebug = 10;
RManModule = 0;
RMInit::logOut = cout.rdbuf();
RMInit::dbgOut = cout.rdbuf();
RMInit::tileSize = 12;
MDDStorage::DefaultIxType = MDDStorage::RPlusTreeNode;

  // variables representing O2 database, ta and session
  DatabaseIf database;

  if( argc < 2 ) {
    cout << "Usage: test_persmddcoll <database> [collName]" << endl;
    return -1;
  }
  O2DBName = strdup( argv[1] );
  if ( argc == 3 ) collName = strdup( argv[2] );
  else 
    collName = defaultCollName;

  // don't forget to initialize before using AdminIf!
  AdminIf* myAdmin = AdminIf::instance();
  
  
  // connect to the database
  cout << "Connecting to database " << O2DBName 
       << "..." << endl;
  int errorDBOpen;
  try{
    errorDBOpen =  database.open( O2DBName );
  }
  catch( ...) 
  {
     cout << "Caught Exception " << endl;
     errorDBOpen = -6;
  }
  if ( errorDBOpen < 0 )
  {
    cout << "Database doesn't exist. Create it new ... " << endl;
    cout << "Creating new database " << O2DBName
       << "..." << endl;
    database.create( O2DBName, "TestSMSchema" );
    cout << "Connecting to database " << O2DBName 
       << "..." << endl;
    try{
      errorDBOpen =  database.open( O2DBName );
    }
    catch(...)
    {
       errorDBOpen = -6;
    }
  }    
  if ( errorDBOpen < 0 )
  {
    cout << "Failed at opening newly created database " << errorDBOpen << endl;
    cout << "Exiting " << endl;
    return errorDBOpen;
  }
  
  ta.begin( &database );
  cout << endl << "Deleting root object from the database ..." <<endl;
  int i = PersMDDColl::destroyRoot( collName, &database );
  cout << " i == " << i << endl;
  // cout << " &database = " << long( &database) << endl;
  ta.commit( );
  
  char c;
  int error;
  
  for( unsigned opt = getOption( ); opt != ExitNo ; opt = getOption( ) )
  {
    cout <<"Transaction begin ... " << endl;
    ta.begin( &database );
    switch ( opt )
    {
      case 1: // 1 - Create MDD collection with name 
	// create root collection
        cout << endl << "Creating mdd types and root collection..." << endl;
        if ( createMDDColl( collName, &database ) != 0 )
        {
           cout <<"Error caught ................."<< endl;
           cout << endl << "Ending O2 session..." << endl;
           ta.commit( );
           database.close( );
           delete myAdmin;
           exit( 1 );
        }
        break;
	
      case 2: //  2 - Populate collection with MDD objects (testConstructors)
	// create objects and put them in the collection 
        cout << endl << "Populate collection ..." << endl;
        testConstructors( collName );
        break;
	
      case 3: //  3 - Access MDD collection given name (testAccessing)
        // read coll and print contents 
        cout << endl << "Read collection " << collName << " and print contents..." << endl;
        testAccessing( collName );
	break;
	
      case 4: //  4 - Access MDD collection given OId (testAccessing)
	cout << endl << "Read collection " << globalCollOId << " and print contents..." << endl;
        testAccessing( globalCollOId );
        break;
	
      case 5: //  5 - Access MDD object given OId (testAccessingMDDObj)
	cout << endl << "Test Accessing MDD with OId " << globalOId1 << " ... " << endl;
        testAccessingMDDObj( globalOId1 );
        break;
	
      case 6: //  6 - Remove MDD object from the collection given OId (testRemove)	
        cout<< endl << "Remove MDD with OId " << globalOId1 << " ..." << endl;
        testRemove( globalOId1 );
        break;
      
      case 7: //  7 - Test PersMDDObj::intersect( ) (testSearch)
        cout << endl << "Test region search ..." << endl;
        testSearch( );
	break;

      case 8: //  8 - Test PersMDDColl::getCardinality( )
	cout << endl << "Get cardinality of collection" << collName <<" ..." << endl;
        testGetCardinality( collName );
        break;
	
      case 9: //  9 - Test PersMDDObj get functions
	cout << endl <<"Test PersMDDObj get functions " << endl;
	testGetFunctions( );
	break;
	
      case 10: //  10 - Remove MDD Collection given OId
	cout << endl << "Remove MDD collection with OId ";
	cout << globalCollOId << " ..." << endl;
	error = PersMDDColl::destroyRoot( globalCollOId, &database );
        if (error ) 
          cout << " Error destroying root " << endl;
	break;
	
      case 11: // 11 - Insert MDD given OId in a second collection
	cout << endl << "Insert Object with OId " << globalOId1;
	cout << " in collection Coleccao1 " << endl;
	cout << "First, create collection" << endl;
	if ( createMDDColl( "Coleccao1", &database ) != 0 )
        {
           cout <<"Error caught ................."<< endl;
           cout << endl << "Ending O2 session..." << endl;
           ta.commit( );
           database.close( );
           delete myAdmin;
           exit( 1 );
        }
	cout << "Then insert object with OId "<< globalOId1 << endl;
	testInsertMDDObjColl( globalOId1 , "Coleccao1");
	cout << endl;
	break;
	
      case 12: // 12 - Insert new tiles in the MDD object 
        cout << endl << "Insert Tile in object with OId " << globalOId1;
        testLaterInsert( globalOId1 );
        cout << endl;
	break;
	
     case 13: // 13 - Remove a tile from the MDD object 
        cout << endl << "Remove Tile from object with OId " << globalOId1;
        testRemoveTile( globalOId1 );
        cout << endl;
	break;
	
      default:
	break;
    }	
    cout <<"Transaction abort (A/a) or commit (default)? ";
    cin >> c;
    if ( c == 'A' || c == 'a' )
    {
      ta.abort( );
      cout <<"End of Transaction Abort..."<<endl;
    }
    else
    	{
	ta.commit( );
      cout <<"End of Transaction Commit..."<<endl;
	}
    if ( opt == 6 )
    {
       cout<<"Garbage ? ( y /n ) ";
       cin >> c;
       if (c =='y' || c == 'Y' )
       {
	 cout <<"Garbaging ..."<< endl;
         ta.begin( &database );
         database.garbage( );
         ta.commit( );
       }
    }
    cout <<"End of transaction commit... "<<endl;
  } // for opt 
 

  cout << endl << "Ending O2 session..." << endl;
  database.close( );
  delete myAdmin;
  
  free( O2DBName );
  if ( collName != defaultCollName ) free( collName );
  return 0;
 
}

/*************************************************************
 * Functions......: 
 *
 *    static void 
 *    testInsertMDDObjColl( OId o, char* cn )
 *    
 *    static void 
 *    testConstructors( char* collName )
 *
 ************************************************************/
static void 
testInsertMDDObjColl( OId o, char* cn )
{
  cout << "....testInsertMDDObjColl "<< o <<","<< cn << endl;
  PersMDDObj* obj = new PersMDDObj( O2DBName,o );
  PersMDDColl objsSet( cn );
  objsSet.insert( obj );
  delete obj;
  
}

static void testConstructors( char* collName )
{

  const BaseType* ulongTypeObj = TypeFactory::mapType("ULong");
  const BaseType* boolTypeObj = TypeFactory::mapType("Bool");
  const MDDBaseType* mType1 = (const MDDBaseType* ) TypeFactory::mapMDDType("TestSMDomainType2D"); 
  const MDDBaseType* mType2 = (const MDDBaseType* ) TypeFactory::mapMDDType("TestSMDomainType3D"); 

  cout << "....testConstructors"<< endl;

  OId oid1; 
  if ( OId::allocateMDDOId( &oid1 ) == 0)
    cout << "Successfull allocation  of OId " << oid1 <<endl; 
  else
  {
    cout << "Error by allocation of OId" <<endl;
    exit(1);
  }  
  OId oid2;
  if ( OId::allocateMDDOId( &oid2 ) == 0)
   cout << "Successfull allocation  of OId " << oid2 <<endl; 
  else
  {
    cout << "Error by allocation of OId" <<endl;
    exit(1);
  }
  
  globalOId1 = oid1;
  globalOId2 = oid2;

  // read root object

  PersMDDColl objsSet(collName);

 
  // create MDD Object 1

  cout << "Creating mddObj1" << endl;
  cout << "tile 1 = nil, 10-12, 20-24 "<< endl;
  r_Sinterval limits1Obj1(10l,12l);
  r_Sinterval limits2Obj1(20l,24l);
  r_Minterval dom(2);
  dom << limits1Obj1 << limits2Obj1;

  r_Minterval tmpInt =  *( ( MDDDomainType* ) mType1 )->getDomain( );
  PersMDDObj* MDDObj1 = new PersMDDObj( mType1, tmpInt, O2DBName, oid1 );

  char* test = (char*)mymalloc((dom.cell_count() + 1) * 4);
  memset(test, 0, (dom.cell_count() + 1) * 4);
  cout << "created new persmddobj" << endl;
  PersTile* tile1Obj1 = new PersTile( dom, ulongTypeObj, test );
  cout << "created new perstile" << endl;
  MDDObj1->insertTile(tile1Obj1);
  cout << "inserted tile" << endl;

  cout << "tile 2 = nil, 0-400, 22-24 "<< endl;
  dom[0].set_interval(0l,400l);
  dom[1].set_interval(22l,24l);
  test = (char*)mymalloc((dom.cell_count() + 1) * 4);
  memset(test, 0, (dom.cell_count() + 1) * 4);
  cout << "created new perstile" << endl;
  PersTile* tile2Obj1 = new PersTile( dom, ulongTypeObj, test );
  cout << "insert tile" << endl;
  MDDObj1->insertTile(tile2Obj1);
  cout << "inserted tile" << endl;
		      
  cout << "tile 3 = nil, 0-600, 10-1000 "<< endl;
  dom[0].set_interval(0l,600l);
  dom[1].set_interval(10l,1000l);
  test = (char*)mymalloc((dom.cell_count() + 1) * 4);
  memset(test, 0, (dom.cell_count() + 1) * 4);
  cout << "created new perstile" << endl;
  PersTile* tile3Obj1 = new PersTile( dom, ulongTypeObj, test );  
  cout << "insert tile" << endl;
  MDDObj1->insertTile(tile3Obj1);
  cout << "inserted tile" << endl;

  cout << "MDDObj1 == isPersistent:" << MDDObj1->isPersistent( )<< ";" <<endl; 
  MDDObj1->printStatus( );
  cout << endl;

  objsSet.insert(MDDObj1);

  // create MDD Object
  cout << "Creating mddObj2 "<< endl;
  cout << "tile 1 = nil, 0-19, 20-59, 30-59 "<< endl;
  r_Sinterval limits1Obj2(0l,19l);
  r_Sinterval limits2Obj2(20l,59l);
  r_Sinterval limits3Obj2(30l,59l);
  r_Minterval dom2(3);
  dom2 << limits1Obj2 << limits2Obj2 << limits3Obj2;

  tmpInt = *( ( MDDDomainType* ) mType2 )->getDomain( );
  PersMDDObj* MDDObj2 = new PersMDDObj( mType2, tmpInt, O2DBName, oid2 );

  test = (char*)mymalloc((dom.cell_count() + 1) * 1);
  memset(test, 0, (dom.cell_count() + 1) * 1);
  PersTile* tile1Obj2 = new PersTile( dom2, boolTypeObj, test);
  MDDObj2->insertTile( tile1Obj2 );  
  
  cout << "tile 2 = nil, 20-39, 60-79, 60-89 "<< endl;
  dom2[0].set_interval(20l,39l);
  dom2[1].set_interval(60l,79l);
  dom2[2].set_interval(60l,89l); 
  test = (char*)mymalloc((dom.cell_count() + 1) * 1);
  memset(test, 0, (dom.cell_count() + 1) * 1);
  PersTile* tile2Obj2 = new PersTile( dom2, boolTypeObj, test);
  
  MDDObj2->insertTile(tile2Obj2);
  

  cout << "MDDObj2 == isPersistent:" << MDDObj2->isPersistent( )<< ";" <<endl; 
  MDDObj2->printStatus( );
  cout << endl;
  
/*
  // This program doesn't work if the TA is aborted when OIds are 
  // allocated, even if all allocated OIds are binded. Question: is it
  // because of dangling handles? This little test was done to 
  // check that.
  // This without oids works with ta.abort
  // conclusion: the problem with abort is not due to the handles
  Handle hd;
  d_Ref<DBMDDObj> refObj = MDDObj2->getDBMDDObjId();
  hd = refObj.o2_get_handle();
*/


  objsSet.insert(MDDObj2);
  numObjsCreated = 2;

  cout << "Release all " << endl;
  
  //( ( PersMDDColl ) objsSet ).releaseAll( );  

  cout << "Release all END" << endl;
}


/*************************************************************
 * Function......: testAccessing( char* cn )
 *                 testAccessing( OId o )
 ************************************************************/

static void testAccessing( char* cn )
{
  PersMDDObj* accessedObj;
  
  cout << "....testAccessing collection "<< cn << endl;
  
  try{
    PersMDDColl objsSet( cn );

    // To test PersMDDColl::printStatus( )
    // objsSet.printStatus( );
  
    // To test PersMDDObj::printStatus( ), MDDCollIter::createIterator( ) and
    // MDDCollIter methods :

    cout << "Iterating through the collection with PersMDDCollIter " << endl;
    MDDCollIter* objsIt = objsSet.createIterator( );
  
    for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
    {
      cout << i<<". MDD object in set:" << endl;
      accessedObj = (PersMDDObj*) objsIt->getElement();
      accessedObj->printStatus();
      EOId eoid;
      if ( accessedObj->getEOId( &eoid ) ==0 )
       cout <<"EOId: " << eoid;
       // old version cout <<"EOId: " << eoid.getSystemName( ) << eoid.getBaseName( ) << eoid.getOId( );
      cout << endl << endl;
      accessedObj->getEOId( &globalEOId[i-1] );
    }
    delete objsIt;
    objsSet.releaseAll( );
  }
  catch ( r_Error& errObj)
  {
    cout <<"Error caught ................."<< endl;
  } 
}

static void testAccessing( OId o )
{
  PersMDDObj* accessedObj;
  
  cout << "....testAccessing collection "<< o << endl;
  
  try {
    PersMDDColl objsSet( o, O2DBName );
     
    OId o;
    if ( objsSet.getOId( &o ) == 0 )
       cout <<"getOId " << o << endl;
    else 
       cout <<"Error getOId " << endl;
    
    EOId eo;
    if ( objsSet.getEOId( &eo ) == 0 )
      cout << "getEOId " << eo <<endl;
        // cout << "getEOId " << eo.getSystemName( ) <<":"<<eo.getBaseName( )<<":"<<eo.getOId( ) << endl;
    else
       cout <<"Error getEOId " << endl;
    
    // To test PersMDDColl::printStatus( )
    objsSet.printStatus( );
  
    // To test PersMDDObj::printStatus( ), MDDCollIter::createIterator( ) and
    // MDDCollIter methods :
 
    cout << "Iterating through the collection with PersMDDCollIter " << endl;
    MDDCollIter* objsIt = objsSet.createIterator( );
  
    for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
    {
      cout << i<<". MDD object in set:" << endl;
      accessedObj = (PersMDDObj*) objsIt->getElement();
      accessedObj->printStatus();
      EOId eoid;
      if ( accessedObj->getEOId( &eoid ) ==0 )
       cout <<"EOId: " << eoid ;
       // cout <<"EOId: " << eoid.getSystemName( ) << eoid.getBaseName() << eoid.getOId( );
      cout << endl << endl;
      accessedObj->getEOId( &globalEOId[i-1] );
    }
    delete objsIt;
    objsSet.releaseAll( );
  }
  catch ( r_Error& errObj)
  {
    cout <<"Error caught ................."<< endl;
  } 
 
}

/*************************************************************
 * Function......: testAccessingMDDObj(OId o ) 
 *                 testAccessingOId( )
 *
 ************************************************************/
static void testAccessingMDDObj(OId o)
{
  PersMDDObj *mObj;

  cout << "....testAccessingMDDObj"<<endl;

  try{
    mObj = new PersMDDObj( O2DBName, o);
    mObj->printStatus( );
    delete mObj;
  }
  catch (...)
  {
     cout <<" Object not found..." << endl;
     
  }
}

static void testAccessingOId()
{
  PersMDDObj *mObj1, *mObj2, *mObj;
  int result;
  OId o(70000); 
  OId o1(5010); 
  OId o2(0);
  OId o3(5);
 
  cout << "....testAccessingOId"<<endl;

  
  cout << "Test OIdIf::getType( )..."<<endl;
  cout << "1.st MDDObj " << endl;
  cout << "getType " << globalOId1.getType( O2DBName ) << endl;
  
  cout << "2.nd MDDObj " << endl;
  cout << "getType " << globalOId2.getType( O2DBName ) << endl; 

  // This crashes
  // cout << "OId == 0 " << endl;
  // cout << "getType " << o2.getType( O2DBName ) << endl;

  cout << "OId == 5 " << endl;
  cout << "getType " << o3.getType( O2DBName ) << endl;

  cout << "MDDCollection " << endl;
  cout << "getType " << globalCollOId.getType( O2DBName ) << endl;
  // mObj2 = new PersMDDObj( O2DBName, globalOId2);
  // mObj2->printStatus( );

  cout << "Not used OId " << endl;
  cout << "getType " << o1.getType( O2DBName ) << endl;

  cout << "Nonexistent OId " << endl;
  cout << "getType " << o.getType( O2DBName ) << endl;
  // mObj2 = new PersMDDObj( O2DBName, globalOId2);
  // mObj2->printStatus( );

  cout <<"Loading PersMDDObjs from OIds " << endl;
  mObj1 = new PersMDDObj( O2DBName, globalOId1);
  mObj1->printStatus( );
  mObj2 = new PersMDDObj( O2DBName, globalOId2);
  mObj2->printStatus( );

  delete mObj1;
  delete mObj2;
  
  
  for ( int i = 0; i < numObjsCreated; i++ )
  {
    cout << "Reading with " << i+1<< ".th EOId " ; 
    cout << globalEOId[i]; 
    // cout << globalEOId[i].getSystemName( );   
    // cout << " ; "<< globalEOId[i].getBaseName( ) << ";" << globalEOId[i].getOId( );
    cout << endl;
 
    
    // result = o2_externalNameGetObject( &globalEOId[i], &hdObj );
/*   
    result = globalEOId[i].getObject( &hdObj );
    if ( result == 0 ) 
    {
      DBMDDObjId  obj1(hdObj);
      mObj = new PersMDDObj(obj1);
      mObj->printStatus( );
    } 
    else 
      cout << "No such object!!" << endl;
    // o2_unref_handle( );
    delete mObj;
*/   
  }
 
}

/*************************************************************
 * Function......: testLaterInsert()
 *
 ************************************************************/

static void testLaterInsert( OId o )
{
  PersMDDObj* accessedObj;

  cout << "....testLaterInsert"<<endl;

  try{
    accessedObj = new PersMDDObj( O2DBName, o);
  }
  catch (...)
  {
     cout <<" Object not found..." << endl;
     return;
  }

  cout << "Current status of MDD object : " << endl;
  accessedObj->printStatus( );
  cout << endl << "Inserting new Tile ..."<< endl; 

  PersTile *t, *t2, *t3;
  switch( accessedObj->getDimension( ) ) 
  {
     case 2 : 
       t2 = new PersTile( r_Minterval("[40:60,80:1200]"), 
                                    accessedObj->getCellType( ) );
       t = t2;
       break;
     case 3 : 
       t3 = new PersTile(r_Minterval("[40:60,80:100,0:20]"), 
                                    accessedObj->getCellType( )  ); 
       t = t3;
       break;
     default: 
       cout << "Error Dimensionality not expected" << endl;
       break;
    }
  accessedObj->insertTile(t);
  cout << " New status after insertion:" << endl;
  accessedObj->printStatus();
  cout << endl << endl;
 
  delete accessedObj;
}


/*************************************************************
 * Function......: testSearch()
 *
 ************************************************************/ 
static void testSearch()
{
  
  MDDObj* accessedObj;
  
  cout << "....testSearch"<<endl;
    
  PersMDDColl objsSet(collName);
  
  MDDCollIter* objsIt = objsSet.createIterator( );
  
  for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
  {
    accessedObj =  objsIt->getElement();
   
    cout << "Accessed Object " << endl; 
    accessedObj->printStatus( );
    cout << endl; 
    
    if (i == 1 || i == 2)
    {
      r_Minterval searchInt1(2); 
      r_Minterval searchInt2(3); 
      vector< Tile* >* entriesList;
      
      cout << "    -- " << i << ". MDD object in list. Search for:";
      switch (i) {
      case 1: searchInt1[0].set_interval(10l,20l);
	      searchInt1[1].set_interval(10l,30l);
	      cout << " 10-20, 10-30" << endl;
              entriesList = accessedObj->intersect(searchInt1);
              break;
      case 2: searchInt2[0].set_interval(10l,20l);
	      searchInt2[1].set_interval(10l,30l);
	      searchInt2[2].set_interval(40l,50l);
	      cout << " 10-20, 10-30, 40-50" <<endl;
	      entriesList = accessedObj->intersect(searchInt2);
	      break;
      default: break;
      }
      cout << "    -- Search result: " << endl; 
      vector<Tile*>::iterator entryIt = entriesList->begin();
          
      while (entryIt !=  entriesList->end())
      {
	 // (*entryIt)->printStatus();
	 r_Minterval tileInterval = (*entryIt)->getDomain();
	 int dimensionality = tileInterval.dimension();
	 
	 cout << "   PersTile printStatus";
         cout << "    domain == " << dimensionality << ": ";
         for (int i = 0; i <dimensionality; i++)
             cout << tileInterval[i].low() << "-" 
                  << tileInterval[i].high() <<", ";
         cout << endl;

	 entryIt++;
      }
      delete entriesList;
    }
  }
  delete objsIt;
  // objsSet.releaseAll( );
}

/*************************************************************
 * Function......: testRemoveTile(OId o ) 
 *
 ************************************************************/
static void testRemoveTile(OId o)
{
  PersMDDObj *mObj;

  cout << "....testRemoveTile from MDD Obj "<< o << endl;

  try{
    mObj = new PersMDDObj( O2DBName, o);
    mObj->printStatus( );
  }
  catch (...)
  {
     cout <<" Object not found..." << endl;
     return;
  }
  
  vector<Tile*>* tiles = mObj->getTiles( );
  
  if ( tiles->size( ) == 0 )
  {
    cout <<"MDD object has no tiles !! "<< endl;
    return;
  }
  else
  {
    int ix = tiles->size( )/2;
  
    cout << "Removing "<< ix <<". tile from MDD Obj "<< endl;
    cout << "Tile Description: " << (*tiles)[ix]->getDomain( ) << endl;
  
    PersTile* t =  (PersTile*) (*tiles)[ix];
  
    mObj->removeTile( (*tiles)[ix] );
  
    delete tiles;
    delete mObj;
  }
}

/*************************************************************
 * Function......: testGetCardinality( const char* cn )
 *                 testGetFunctions()
 *
 ************************************************************/
static void testGetCardinality( char* cn )
{
  cout << "....testGetCardinality( "<< cn << " )" << endl;
  
  try{
   PersMDDColl objsSet( cn );
   cout<< "Cardinality of collection " << objsSet.getCardinality( ) <<endl;
  }
  catch(  r_Error& errObj)
  {
    cout <<"Error caught ................."<< endl;
  }
}

static void testGetFunctions()
{
  PersMDDObj* accessedObj;
  
  cout << "....testGetFunctions"<<endl;
    
  PersMDDColl objsSet(collName);
  
  MDDCollIter* objsIt = objsSet.createIterator( );
  
  vector< Tile* >* entriesList;
  
  for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
  {
    r_Minterval currDom;
    r_Minterval defDom;
    
    cout << "  " << i << ". Object" << endl;
    accessedObj =  (PersMDDObj*) objsIt->getElement();
    
    
    defDom = accessedObj->getDefinitionDomain( );
    cout << "     GetDefinitionDomain result: ";
    defDom.print_status( );
    cout << endl;

    currDom = accessedObj->getCurrentDomain( );
    cout << "     GetCurrentDomain result:  ";
    currDom.print_status( );
    cout << endl;
    
    entriesList = accessedObj->getTiles( );
    cout << "     -- GetTiles result: " << endl; 
    vector<Tile*>::iterator entryIt = entriesList->begin();
          
    while (entryIt !=  entriesList->end())
    {
        // (*entryIt)->printStatus();
       r_Minterval tileInterval = (*entryIt)->getDomain();
       int dimensionality = tileInterval.dimension();
	 
       cout << "    PersTile ";
       cout << " domain == " << dimensionality << ": ";
       for (int i = 0; i <dimensionality; i++)
         cout << tileInterval[i].low() << "-" << tileInterval[i].high() <<", ";
       cout << endl;
       entryIt++;
    }
    delete entriesList;
  }
  delete objsIt;
  objsSet.releaseAll( );
}

/*************************************************************
 * Function......: testRemove()
 *                 testRemove( OId o )
 *
 ************************************************************/

static void testRemove()
{
  PersMDDObj* accessedObj;
  
  cout << "....testRemove"<<endl;
    
  PersMDDColl objsSet(collName);
  // PersMDDColl objsSet("Qualquercoisa");
  // To test PersMDDColl::printStatus and PersMDDColl::remove
  
  
  MDDCollIter* objsIt = objsSet.createIterator( );
  
  cout << "-- Remove second element from collection " << endl;
  
  for( int i = 1 ; objsIt->notDone( ) && i < 2; i++, objsIt->advance( ))
  {
    accessedObj = (PersMDDObj*) objsIt->getElement();
  }
  cout << "Delete of objsIt:" << endl; 
  delete objsIt;
  cout << "Finished Delete of objsIt." << endl; 
  cout << "Remove accessedObj:" << endl; 
  objsSet.remove( accessedObj );
  cout << "Finished Remove accessedObj." << endl; 
}

static void testRemove( OId o )
{
 
  cout << "....testRemove( OId == " << o <<" ) "<< endl;

  PersMDDColl objsSet(collName);
  objsSet.remove( o, O2DBName ); 
}


/*************************************************************
 * Function......: createMDDColl( )
 ************************************************************/
static int
createMDDColl( const char* collName, DatabaseIf* db )
{
cout << "createMDDColl(" << collName << ", " << db->getName() << ")" << endl;
  MDDDomainType* mType1 = 0;
  MDDDomainType* mType2 = 0;
  MDDType* mt = 0;
  CollectionType* collType1 = 0;
     
  const BaseType* ulongTypeObj = TypeFactory::mapType("ULong");
 
  const MDDDomainType* cmType1 =
    ( MDDDomainType* ) TypeFactory::mapMDDType( "TestSMDomainType2D" );
    if (cmType1)
    	cout << "cmType1 " << cmType1->getName() << endl;
  const MDDDomainType* cmType2 =
    ( MDDDomainType* ) TypeFactory::mapMDDType( "TestSMDomainType3D" );
    if (cmType2)
    	cout << "cmType1 " << cmType2->getName() << endl;

  const CollectionType* collType =
    (CollectionType*)TypeFactory::mapSetType( "ObjsContainerType" );
    if (collType)
    	cout << "collType " << collType->getName() << endl;


  if( !cmType1 || !cmType2 || !collType )
  {
  cout << "types not found!" << endl;
    char name1[] = "TestSMDomainType2D";
    char name2[] = "TestSMDomainType3D";

    r_Sinterval limits1Obj1(0l,1000l);
    r_Sinterval limits2Obj1(0l,800l);
    r_Minterval dom1(2);
    dom1 << limits1Obj1 << limits2Obj1;
    cout << "MDD Type 1 , domain "<< dom1 << endl;

    cout << "       tile 1 = nil, 0-19, 20-59, 30-59 "<< endl;
    r_Sinterval limits1Obj2(0l,19l);
    r_Sinterval limits2Obj2(20l,59l);
    r_Sinterval limits3Obj2(30l,59l);
    r_Minterval dom2(3);
    dom2 << limits1Obj2 << limits2Obj2 << limits3Obj2;
    cout << "MDD Type 2 , domain "<< dom2 << endl;

    // MDDDomainType* mType1 =
    mType1 = 
      new  MDDDomainType((char*) name1, ( BaseType* ) ulongTypeObj, dom1 );
    // MDDDomainType* mType2 =
    mType2 = 
      new  MDDDomainType((char*) name2, ( BaseType* ) ulongTypeObj, dom2 );
 
    cout << "MDD Type1 == ";
    mType1->print_status( cout  );
    cout << endl;
    cout << "MDD Type2 == ";
    mType2->print_status( cout );
    cout << endl;

    TypeFactory::addMDDType( mType1 );
    TypeFactory::addMDDType( mType2 );
    
    if ( !collType )
    {
      cout << "Collection type newly created " << endl;
      // MDDType* mt = new MDDType( );
      mt = new MDDType( );
      cout << "MDDtype created "<< endl;
      collType1 = new SetType(  "ObjsContainerType", mType1 );
      cout << "Set Type created ... ";
      collType = collType1;
      TypeFactory::addSetType( (SetType*)  collType );
      cout <<" and added "<< endl;

    }

  }
  else
  	cout << "types were found!" << endl;
  //ta.commit( );

  //ta.begin( );

  cout << "Creating root collection" << endl;

  // cout << " &database = " << long( db ) << endl;

  OId oColl;
  if( OId::allocateMDDCollOId(  &oColl ) == 0 )
    cout <<"Successfully allocated OId for collection " << oColl << endl;
  else
    cout <<"Error allocating OId for collection " << endl;
  globalCollOId = oColl;
  
  PersMDDColl* col;
  try {
  // CollectionType* ct = TypeFactory::mapSetType( "ObjsContainerType" );
  col = PersMDDColl::createRoot( collName, oColl, collType, db );
  }
  catch (...)
  {
    cout <<"Error caught ................."<< endl;
    return -1;
  }
  
  cout << "Committing TA ..."<< endl;
  ta.commit( );
  
  /*
  
  cout <<"Freeing types ... "<< endl; 
  
  // if ( mType1 ) delete mType1;
  cout << "Type 1 freed . "<< endl;
  // if ( mType2 ) delete mType2;
  cout << "Type 2 freed . "<< endl;
  // if ( mt ) delete mt;
  cout << "Type mt freed . "<< endl;
  // if ( collType1 ) delete collType1;
  cout << "Types collType1 freed . "<< endl;
  */
  if ( col ) delete col;
  cout << "Col freed . "<< endl;
  
  
  ta.begin( db );
  return 0;
}
