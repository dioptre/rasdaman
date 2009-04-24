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
 *   creates Tiles and does ops
 *
 *
 * COMMENTS:
 *   none
 *
 ************************************************************/

static const char rcsid[] = "@(#)cachetamgr,test_tile: $Id: test_tile.cc,v 1.26 2002/08/30 11:51:22 coman Exp $";

#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "ulongtype.hh"
#include "booltype.hh"
#include "chartype.hh"
#include "structtype.hh"

#ifndef bool
  #include <bool.h>
  #define OS_OMIT_BOOL
#endif

#include "adminif.hh"
#include "databaseif.hh"
#include "transactionif.hh"
#include "tilemgr/perstile.hh"
#include "tilemgr/transtile.hh"
#include "typefactory.hh"
#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"

// global variable for AdminIf because of O2 d_Session::begin()
extern char* myExecArgv0 = "";

RMINITGLOBALS('C')

// perhaps used later
static char O2BenchDBName[256] = "NorbertBase";
static char O2BenchSchemaName[256] = "NorbertSchema";

void
printTime()
{
  struct timespec cur_time;
  clock_gettime(CLOCK_REALTIME, &cur_time);

  cout << asctime(localtime(&(cur_time.tv_sec)));
}

void
testConstructors()
{
  // for for loops
  unsigned long i;
  // Tile used for tests
  Tile* myTile;
  // base types for tiles
  BaseType* ulongType = TypeFactory::mapType("ULong");
  // domain of all Tiles
  r_Minterval dom = 
    r_Minterval(3) << r_Sinterval(1L,10L) << r_Sinterval(11L,20L) 
                   << r_Sinterval(5L,7L);

  // testing constructor executing section operation
  // first create a tile with the whole domain
  // char* containing data for one ULong Tile
  char* tileData = (char*)mymalloc(dom.cell_count() * ulongType->getSize());
  for(i = 0; i < dom.cell_count() * ulongType->getSize(); i++)
  {
    if(i % ulongType->getSize() == 3 )
      tileData[i] = 10;
    else
      tileData[i] = 0;
  }

  // transient with value
  cout << "Creating transient ULong Tile containing 10 with domain " 
       << dom << ":" << endl;
  myTile = new TransTile( dom, ulongType, tileData );

  // now project it
  set<r_Dimension, less<r_Dimension> >* projSet = 
    new set<r_Dimension, less<r_Dimension> >;
  projSet->insert(1);

  r_Minterval projDom = 
    r_Minterval(3) << r_Sinterval(1l, 10l) << r_Sinterval(15l, 15l) 
		   << r_Sinterval(5l, 7l);
  
  Tile* projectedTile = new TransTile(myTile, projDom, projSet);

  projectedTile->printStatus();
  
  delete projSet;
  delete projectedTile;
}

void
testCompression()
{
  // for for loops
  unsigned long i;
  // Tile used for tests
  Tile* myTile;
  // Tile used for inititializing tiles with OP_IDENTITY
  Tile* myContentTile;
  // for contents of tiles
  char* tileData;
  // base types for tiles
  BaseType* ulongType = TypeFactory::mapType("ULong");
  // domain of all Tiles
  r_Minterval dom = 
    r_Minterval(3) << r_Sinterval(1L,10L) << r_Sinterval(11L,20L) 
                   << r_Sinterval(5L,7L);

  cout << "Testing Constructors for Tiles" << endl;
  cout << "------------------------------" << endl << endl;
 
  // ------------------------------------------------------------
  // uncompressed TransTile
  // ------------------------------------------------------------

  // char* containing data for one ULong Tile
  tileData = (char*)mymalloc(dom.cell_count() * ulongType->getSize());
  for(i = 0; i < dom.cell_count() * ulongType->getSize(); i++)
  {
    if(i % ulongType->getSize() == 3 )
      tileData[i] = 10;
    else
      tileData[i] = 0;
  }

  // transient with value
  cout << "Creating transient ULong Tile containing 10 with domain " 
       << dom << ":" << endl;
  myTile = new TransTile( dom, ulongType, tileData );

  cout << "  Printing created Tile: " << endl;
  myTile->printStatus();

  cout << "  Deleting created Tile" << endl << endl;
  delete myTile;

  // ------------------------------------------------------------
  // uncompressed PersTile
  // ------------------------------------------------------------

  // char* containing data for one ULong Tile
  tileData = (char*)mymalloc(dom.cell_count() * ulongType->getSize());
  for(i = 0; i < dom.cell_count() * ulongType->getSize(); i++)
  {
    if(i % ulongType->getSize() == 3 )
      tileData[i] = 10;
    else
      tileData[i] = 0;
  }

  // persistent with value
  cout << "Creating persistent ULong Tile containing 10 with domain " 
       << dom << ":" << endl;
  myTile = new PersTile( dom, ulongType, tileData );

  cout << "  Printing created Tile: " << endl;
  myTile->printStatus();

  cout << "  Deleting created Tile" << endl << endl;
  delete myTile;

  // ------------------------------------------------------------
  // uncompressed TransTile
  // ------------------------------------------------------------

  // char* containing data for one ULong Tile
  tileData = (char*)mymalloc(dom.cell_count() * ulongType->getSize());
  for(i = 0; i < dom.cell_count() * ulongType->getSize(); i++)
  {
    if(i % ulongType->getSize() == 3 )
      tileData[i] = 10;
    else
      tileData[i] = 0;
  }

  // transient with value
  cout << "Creating transient ULong Tile containing 10 with domain " 
       << dom << ":" << endl;
  myContentTile = new TransTile( dom, ulongType, tileData );
  myTile = new TransTile( dom, ulongType );
  myTile->execUnaryOp( Ops::OP_IDENTITY, dom, myContentTile, dom );

  cout << "  Printing created Tile: " << endl;
  myTile->printStatus();

  cout << "  Deleting created Tile" << endl << endl;
  delete myTile;

  // ------------------------------------------------------------
  // uncompressed PersTile
  // ------------------------------------------------------------

  // char* containing data for one ULong Tile
  tileData = (char*)mymalloc(dom.cell_count() * ulongType->getSize());
  for(i = 0; i < dom.cell_count() * ulongType->getSize(); i++)
  {
    if(i % ulongType->getSize() == 3 )
      tileData[i] = 10;
    else
      tileData[i] = 0;
  }

  // persistent with value
  cout << "Creating persistent ULong Tile containing 10 with domain " 
       << dom << ":" << endl;
  myContentTile = new PersTile( dom, ulongType, tileData );
  myTile = new PersTile( dom, ulongType, r_ZLib );
  myTile->execUnaryOp( Ops::OP_IDENTITY, dom, myContentTile, dom );

  cout << "  Printing created Tile: " << endl;
  myTile->printStatus();

  cout << "  Deleting created Tile" << endl << endl;
  delete myTile;

  // ------------------------------------------------------------
  // compressed TransTile
  // ------------------------------------------------------------

  // char* containing data for one ULong Tile
  tileData = (char*)mymalloc(dom.cell_count() * ulongType->getSize());
  for(i = 0; i < dom.cell_count() * ulongType->getSize(); i++)
  {
    if(i % ulongType->getSize() == 3 )
      tileData[i] = 10;
    else
      tileData[i] = 0;
  }

  // transient with value
  cout << "Creating compressed transient ULong Tile containing 10 with domain " 
       << dom << ":" << endl;
  myTile = new TransTile( dom, ulongType, r_ZLib );
  myTile->setContents(tileData);

  cout << "  Physical Size: " << myTile->getPhysicalSize() << endl;

  cout << "  Printing created Tile: " << endl;
  myTile->printStatus();

  cout << "  Physical Size: " << myTile->getPhysicalSize() << endl;

  cout << "  Deleting created Tile" << endl << endl;
  delete myTile;

  // ------------------------------------------------------------
  // compressed PersTile
  // ------------------------------------------------------------

  // char* containing data for one ULong Tile
  tileData = (char*)mymalloc(dom.cell_count() * ulongType->getSize());
  for(i = 0; i < dom.cell_count() * ulongType->getSize(); i++)
  {
    if(i % ulongType->getSize() == 3 )
      tileData[i] = 10;
    else
      tileData[i] = 0;
  }

  // persistent with value
  cout << "Creating compressed persistent ULong Tile containing 10 with domain " 
       << dom << ":" << endl;
  myTile = new PersTile( dom, ulongType, r_ZLib );
  myTile->setContents(tileData);

  cout << "  Physical Size: " << myTile->getPhysicalSize() << endl;

  cout << "  Printing created Tile: " << endl;
  myTile->printStatus();

  cout << "  Physical Size: " << myTile->getPhysicalSize() << endl;

  cout << "  Deleting created Tile" << endl << endl;
  delete myTile;

  // ------------------------------------------------------------
  // compressed TransTile
  // ------------------------------------------------------------

  // char* containing data for one ULong Tile
  tileData = (char*)mymalloc(dom.cell_count() * ulongType->getSize());
  for(i = 0; i < dom.cell_count() * ulongType->getSize(); i++)
  {
    if(i % ulongType->getSize() == 3 )
      tileData[i] = 10;
    else
      tileData[i] = 0;
  }

  // transient with value
  cout << "Creating compressed transient ULong Tile containing 10 with domain " 
       << dom << ":" << endl;
  myContentTile = new TransTile( dom, ulongType, tileData );
  myTile = new TransTile( dom, ulongType, r_ZLib );
  myTile->execUnaryOp( Ops::OP_IDENTITY, dom, myContentTile, dom );

  cout << "  Physical Size: " << myTile->getPhysicalSize() << endl;

  cout << "  Printing created Tile: " << endl;
  myTile->printStatus();

  cout << "  Physical Size: " << myTile->getPhysicalSize() << endl;

  cout << "  Deleting created Tile" << endl << endl;
  delete myTile;

  // ------------------------------------------------------------
  // compressed PersTile
  // ------------------------------------------------------------

  // char* containing data for one ULong Tile
  tileData = (char*)mymalloc(dom.cell_count() * ulongType->getSize());
  for(i = 0; i < dom.cell_count() * ulongType->getSize(); i++)
  {
    if(i % ulongType->getSize() == 3 )
      tileData[i] = 10;
    else
      tileData[i] = 0;
  }

  // persistent with value
  cout << "Creating compressed persistent ULong Tile containing 10 with domain " 
       << dom << ":" << endl;
  myContentTile = new PersTile( dom, ulongType, tileData );
  myTile = new PersTile( dom, ulongType, r_ZLib );
  myTile->execUnaryOp( Ops::OP_IDENTITY, dom, myContentTile, dom );

  cout << "  Physical Size: " << myTile->getPhysicalSize() << endl;

  cout << "  Printing created Tile: " << endl;
  myTile->printStatus();

  cout << "  Physical Size: " << myTile->getPhysicalSize() << endl;

  cout << "  Deleting created Tile" << endl << endl;
  delete myTile;
}

void
testUnaryOps()
{
  // base types for tiles
  BaseType* ulongType = TypeFactory::mapType("ULong");
  BaseType* boolType = TypeFactory::mapType("Bool");;
  BaseType* charType = TypeFactory::mapType("Char");;

  // default cell for filling ULong Tiles
  unsigned long longVal = 4294966295;
  char* ulongCell = (char*)&longVal;

  // default cell for filling Bool Tiles
  unsigned char boolVal = 1;
  char* boolCell = (char*)&boolVal;

  // domain of all Tiles
  r_Minterval dom = 
    r_Minterval(2) << r_Sinterval(1L,10L) << r_Sinterval(11L,20L);

  r_Minterval domOp = 
    r_Minterval(2) << r_Sinterval(1L,5L) << r_Sinterval(11L,15L);

  // domain of one dimensional result
  r_Minterval res1D = 
    r_Minterval(2) << r_Sinterval(1L,10L);

  unsigned char charVal = 20;
  char* charCell = (char*)&charVal;

  // Tiles used for tests
  Tile* transULongTile;
  Tile* transBoolTile;
  Tile* persULongTile;
  Tile* persBoolTile;
  Tile* transCharTile;
  Tile* transCharRes;

  cout << "Testing Unary Operations on Tiles" << endl;
  cout << "---------------------------------" << endl << endl;

  // transient with value
  cout << "Creating Tiles for Unary Operations with domain " 
       << dom << ":" << endl;
  transULongTile = new TransTile( dom, ulongType, ulongCell );
  transBoolTile = new TransTile( dom, boolType, boolCell );
  persULongTile = new PersTile( dom, ulongType, ulongCell );
  persBoolTile = new PersTile( dom, boolType, boolCell );
  transCharTile = new TransTile( dom, charType, charCell );
  transCharRes = new TransTile( domOp, charType, boolCell );


  cout << "Operations with ULong:" << endl << endl;

  cout << "Carrying out NOT on TransTile (should be 1000)" << endl;
  transULongTile->execUnaryOp(Ops::OP_NOT, dom, transULongTile, dom);

  cout << "Contents after operation: " << endl;
  transULongTile->printStatus();

  cout << "Carrying out IDENTITY on TransTile (should be 1000)" << endl;
  transULongTile->execUnaryOp(Ops::OP_IDENTITY, dom, transULongTile, dom);

  cout << "Contents after operation: " << endl;
  transULongTile->printStatus();

  cout << "Carrying out NOT on PersTile (should be 1000)" << endl;
  persULongTile->execUnaryOp(Ops::OP_NOT, dom, persULongTile, dom);

  cout << "Contents after operation: " << endl;
  persULongTile->printStatus();

  cout << "Carrying out IDENTITY on PersTile (should be 1000)" << endl;
  persULongTile->execUnaryOp(Ops::OP_IDENTITY, dom, persULongTile, dom);

  cout << "Contents after operation: " << endl;
  persULongTile->printStatus();

  cout << "Operations with Bool:" << endl << endl;

  cout << "Carrying out NOT on TransTile (should be FALSE)" << endl;
  transBoolTile->execUnaryOp(Ops::OP_NOT, dom, transBoolTile, dom);

  cout << "Contents after operation: " << endl;
  transBoolTile->printStatus();

  cout << "Carrying out IDENTITY on TransTile (should be FALSE)" << endl;
  transBoolTile->execUnaryOp(Ops::OP_IDENTITY, dom, transBoolTile, dom);

  cout << "Contents after operation: " << endl;
  transBoolTile->printStatus();

  cout << "Carrying out NOT on PersTile (should be FALSE)" << endl;
  persBoolTile->execUnaryOp(Ops::OP_NOT, dom, persBoolTile, dom);

  cout << "Contents after operation: " << endl;
  persBoolTile->printStatus();

  cout << "Carrying out IDENTITY on PersTile (should be FALSE)" << endl;
  persBoolTile->execUnaryOp(Ops::OP_IDENTITY, dom, persBoolTile, dom);

  cout << "Contents after operation: " << endl;
  persBoolTile->printStatus();

  cout << "Carrying out IDENTITY on subset of char tile" << endl;
  transCharRes->execUnaryOp(Ops::OP_IDENTITY, domOp, transCharTile, domOp);

  cout << "Contents after operation: " << endl;
  transCharRes->printStatus();

  // Trying to get more interesting values into tile
  unsigned char val = 0;
  for(int i = 0; i < transCharTile->getSize(); i++) {
    transCharTile->setCell(i, (const char*)&val);
    val++;
  }

  cout << "Tile with more interesting values: " << endl;
  transCharTile->printStatus();

  cout << "Carrying out IDENTITY on subset of char tile" << endl;
  transCharRes->execUnaryOp(Ops::OP_IDENTITY, domOp, transCharTile, domOp);

  cout << "Contents after operation: " << endl;
  transCharRes->printStatus();

  // Operation with result of a different dimensionality

  // domain of all Tiles
  r_Minterval horSlice = 
    r_Minterval(2) << r_Sinterval(1L,10L) << r_Sinterval(15L,15L);
  r_Minterval verSlice = 
    r_Minterval(2) << r_Sinterval(5L,5L) << r_Sinterval(11L,20L);
  Tile* transRes1D = new TransTile( res1D, charType, charCell );

  cout << "Carrying out IDENTITY to receive a 1D vertical slice" << endl;
  transRes1D->execUnaryOp(Ops::OP_IDENTITY, res1D, transCharTile, horSlice);

  cout << "Contents after operation: " << endl;
  transRes1D->printStatus();

  cout << "Carrying out IDENTITY to receive a 1D horizontal slice" << endl;
  transRes1D->execUnaryOp(Ops::OP_IDENTITY, res1D, transCharTile, verSlice);

  cout << "Contents after operation: " << endl;
  transRes1D->printStatus();

  cout << "Deleting created Tiles ..." << endl << endl;

  delete transULongTile;
  delete transBoolTile;
  delete persULongTile;
  delete persBoolTile;
  delete transCharRes;
  delete transCharTile;
  delete transRes1D;
}

void
testBinaryOps()
{
  // base types for tiles
  BaseType* ulongType = TypeFactory::mapType("ULong");
  BaseType* boolType = TypeFactory::mapType("Bool");;

  // default cell for filling ULong Tiles
  unsigned long longVal = 1000;
  char* ulongCell = (char*)&longVal;

  // default cell for filling Bool Tiles
  unsigned char boolVal = 1;
  char* boolCell = (char*)&boolVal;

  // domain of all Tiles
  r_Minterval dom = 
    r_Minterval(3) << r_Sinterval(1L,10L) << r_Sinterval(11L,20L) 
		   << r_Sinterval(5L,7L);

  // Tiles used for tests
  Tile* transULongTile;
  Tile* transBoolTile;
  Tile* persULongTile;
  Tile* persBoolTile;
  Tile* constULongTile;
  Tile* constBoolTile;

  cout << "Testing Binary Operations on Tiles" << endl;
  cout << "---------------------------------" << endl << endl;

  cout << "Creating Tiles for Binary Operations with domain " 
       << dom << ":" << endl;
  transULongTile = new TransTile( dom, ulongType, ulongCell );
  transBoolTile = new TransTile( dom, boolType, boolCell );
  persULongTile = new PersTile( dom, ulongType, ulongCell );
  persBoolTile = new PersTile( dom, boolType, boolCell );

  cout << "Creating a TransTile for the two operands..." << endl;
  TransTile ttileOp = TransTile(dom, ulongType, ulongCell);

  cout << "Contents: " << endl;
  ttileOp.printStatus();

  cout << "Creating a TransTile for the result..." << endl;
  TransTile ttileRes = TransTile(dom, ulongType, ulongCell);

  cout << "Carrying out binary operation..." << endl;
  ttileRes.execBinaryOp(Ops::OP_PLUS, dom, &ttileOp, dom, &ttileOp, dom);

  cout << "Contents after operation: " << endl;
  ttileRes.printStatus();

  // testing binary operation with Bool as result
  long bval = 10000;

  // default cell for filling all Tiles
  char* bvalCell = (char*)&bval;

  cout << "Creating a TransTile for the two operands..." << endl;
  TransTile btileOp = TransTile(dom, ulongType, boolCell);

  cout << "Contents: " << endl;
  btileOp.printStatus();

  cout << "Creating a TransTile for the result..." << endl;
  TransTile btileRes = TransTile(dom, boolType, boolCell);

  cout << "Carrying out binary operation..." << endl;
  btileRes.execBinaryOp(Ops::OP_EQUAL, dom, &btileOp, dom, &btileOp, dom);

  cout << "Contents after operation: " << endl;
  btileRes.printStatus();

  cout << "Carrying out PLUS with TransTile and constant (should be 2000)"
       << endl;

  transULongTile->execConstOp(Ops::OP_PLUS, dom, transULongTile, dom,
			      ulongCell, ulongType);

  cout << "Contents after operation: " << endl;
  transULongTile->printStatus();

  delete transULongTile;
  delete transBoolTile;
  delete persULongTile;
  delete persBoolTile;
  delete constULongTile;
  delete constBoolTile;
}

void
testCompTile()
{
//   // size of tiles in one direction
//   const long testSize = 10;
//   // base types for tiles
//   BaseType* ulongType = TypeFactory::mapType("ULong");

//   // default cell for filling ULong Tiles
//   unsigned long longVal = 1000;
//   char* ulongCell = (char*)&longVal;

//   // domain of all Tiles
//   r_Minterval dom = 
//     r_Minterval(2) << r_Sinterval(1L,testSize) << r_Sinterval(1L,testSize);

//   // Tiles used for tests
//   Tile* compTile1;
//   Tile* compTile2;
//   TransTile* transTile;

//   cout << "Testing CompTile" << endl;
//   cout << "----------------" << endl << endl;

//   cout << "Creating Tiles with domain " 
//        << dom << ":" << endl;
//   compTile1 = new CompTile( dom, ulongType, Tile::COMP_ZLIB, ulongCell );
//   compTile2 = new CompTile( dom, ulongType, Tile::COMP_ZLIB, ulongCell );
//   transTile = new TransTile( dom, ulongType, ulongCell );

  // initialize with random values
//   long val;
//   for(int i = 0; i < testSize; i++) {
//     for(int j = 0; j < testSize; j++) {
//       val = (long)(LONG_MIN + ((double)LONG_MAX-LONG_MIN)*drand48());
//       compTile1->setCell(i*testSize+j, (char*)&val);
//       val = (long)(LONG_MIN + ((double)LONG_MAX-LONG_MIN)*drand48());
//       compTile2->setCell(i*testSize+j, (char*)&val);
//     }
//   }

  // initialize with incrementing values.
//   long val;
//   for(int i = 0; i < testSize; i++) {
//     for(int j = 0; j < testSize; j++) {
//       val = i*testSize+j;
//       compTile1->setCell(i*testSize+j, (char*)&val);
//       compTile2->setCell(i*testSize+j, (char*)&val);
//     }
//   }

  // initialize with incrementing values, simulate struct
//   long val;
//   for(int i = 0; i < testSize; i++) {
//     for(int j = 0; j < testSize; j += 3) {
//       val = i*testSize+j;

//       compTile1->setCell(i*testSize+j, (char*)&val);
//       if(j+1 < testSize) 
// 	compTile1->setCell(i*testSize+j+1, (char*)&val+1000000);
//       if(j+2 < testSize) 
// 	compTile1->setCell(i*testSize+j+2, (char*)&val+2000000);

//       compTile2->setCell(i*testSize+j, (char*)&val);
//       if(j+1 < testSize) 
// 	compTile2->setCell(i*testSize+j+1, (char*)&val+1000000);
//       if(j+2 < testSize) 
// 	compTile2->setCell(i*testSize+j+2, (char*)&val+2000000);
//     }
//   }

//   cout << "Contents transTile: " << endl;
//   if(transTile->getSize() <= 1000)
//     transTile->printStatus();

//   cout << "Contents compTile1: " << endl;
//   if(transTile->getSize() <= 1000)
//     compTile1->printStatus();

//   cout << "Contents compTile2: " << endl;
//   if(transTile->getSize() <= 1000)
//     compTile2->printStatus();

  // does only work if compress() is not protected.
//   cout << "Compressing compTiles" << endl;
//   ((CompTile*)compTile1)->compress();
//   ((CompTile*)compTile2)->compress();

//   cout << "Carrying out binary operation..." << endl;
//   transTile->execBinaryOp(Ops::OP_PLUS, dom, compTile1, dom, compTile2, dom);

//   cout << "Contents after operation: " << endl;
//   if(transTile->getSize() <= 1000)
//     transTile->printStatus();

//   delete compTile1;
//   delete compTile2;
//   delete transTile;
}

void
testOpsWithConsts()
{
  // base types for tiles
  BaseType* ulongType = TypeFactory::mapType("ULong");
  BaseType* charType = TypeFactory::mapType("Char");

  StructType* structType = new StructType("TestStruct", 2);
  structType->addElement("Band1", "ULong");
  structType->addElement("Band2", "Char");

  // default cell for filling ULong Tiles
  unsigned long uLongVal = 1000;
  char* ulongCell = (char*)&uLongVal;
  // default cell for filling Char Tiles
  unsigned char charVal = 112;
  char* charCell = (char*)&charVal;
  // default cell for filling Struct Tiles
  unsigned char structCell[5];
  structCell[0] = ulongCell[0];
  structCell[1] = ulongCell[1];
  structCell[2] = ulongCell[2];
  structCell[3] = ulongCell[3];
  structCell[4] = charCell[0];

  // domain of all Tiles
  r_Minterval dom = 
    r_Minterval(2) << r_Sinterval(1L,5L) << r_Sinterval(1L,5L);

  // Tiles used for tests
  cout << "Creating Tiles with domain " 
       << dom << ":" << endl;
  Tile* charTile = new TransTile(dom, charType, charCell);
  Tile* ulongTile = new TransTile(dom, ulongType, ulongCell);
  Tile* structTile = new TransTile(dom, structType, (char*)structCell);

  cout << "Contents of Tiles: " << endl;
  charTile->printStatus();
  ulongTile->printStatus();
  structTile->printStatus();

  cout << "Result Type for Struct and Char: " 
       << (Ops::getResultType(Ops::OP_PLUS, structType, charType))->getTypeName() << endl;

  cout << "Carrying out binary operation with constant on structTile ..." 
       << endl;
  structTile->execConstOp(Ops::OP_PLUS, dom, structTile, dom, charCell, 
			  charType);

  cout << "Contents after operation: " << endl;
  structTile->printStatus();

  cout << "Carrying out binary operation with charTile on structTile ..." 
       << endl;
  structTile->execBinaryOp(Ops::OP_PLUS, dom, structTile, dom, charTile, dom);

  cout << "Contents after operation: " << endl;
  structTile->printStatus();

  cout << "Carrying out binary operation with two structTiles ..." 
       << endl;
  structTile->execBinaryOp(Ops::OP_PLUS, dom, structTile, dom, structTile, dom);

  cout << "Contents after operation: " << endl;
  structTile->printStatus();

  delete charTile;
  delete ulongTile;
  delete structTile;
}

void
testCellAccess()
{
  // base types for tiles
  BaseType* ulongType = TypeFactory::mapType("ULong");
  BaseType* charType = TypeFactory::mapType("Char");

  // default cell for filling ULong Tiles
  unsigned long uLongVal = 1000;
  char* ulongCell = (char*)&uLongVal;
  // default cell for filling Char Tiles
  unsigned char charVal = 112;
  char* charCell = (char*)&charVal;

  // domain of all Tiles
  r_Minterval dom = 
    r_Minterval(2) << r_Sinterval(1L,10L) << r_Sinterval(1L,10L);

  // Tiles used for tests
  cout << "Creating Tiles with domain " 
       << dom << ":" << endl;
  Tile* charTile = new TransTile(dom, charType, charCell);
  Tile* ulongTile = new TransTile(dom, ulongType, ulongCell);

  cout << "Accessing cell by point: ";
  r_Point a = r_Point(2) << 5 << 5;
  cout << (int)*(charTile->getCell(a)) << " "
       << *(unsigned long*)ulongTile->getCell(a) << endl;
  
  delete charTile;
  delete ulongTile;
}

void
testScaleOp(void)
{
  r_Dimension dim = 2;
  r_Dimension i;
  BaseType* charType = TypeFactory::mapType("Char");
  r_Minterval dom = r_Minterval(dim);
  r_Minterval nextDom = r_Minterval(dim);
  r_Point origin(dim);
  r_Point offset(dim);
  unsigned long fullSize;
  float scaleBy, incrementBy, eps;

  for (i=0; i<dim; i++)
  {
    origin[i] = (r_Range)0;
    offset[i] = (r_Range)64;
    dom << r_Sinterval(origin[i] + offset[i], (r_Range)511 + offset[i]);
    nextDom << r_Sinterval((r_Range)512 + offset[i], (r_Range)1023 + offset[i]);
  }

  fullSize = dom.cell_count();
  char *tileValues = (char*)mymalloc(fullSize);
  Tile *charTile = new TransTile(dom, charType, tileValues, fullSize);
  Tile *nextTile = new TransTile(dom, charType);
  memset(charTile->getContents(), 0, dom.cell_count());

  vector<float> scaleFactors(dim);

  incrementBy = 0.05; eps = 1e-2;
  for (scaleBy=incrementBy; scaleBy <= 8-eps; scaleBy += incrementBy)
  {
    vector<float>::iterator iter = scaleFactors.begin();

    cout << "Scale " << dom << " by " << scaleBy << endl;

    for (i=0; i<dim; i++, iter++)
      *iter = scaleBy;

    r_Minterval scaledDom;
    if (charTile->scaleGetDomain(dom, origin, scaleFactors, scaledDom))
    {
      unsigned long scaledSize = scaledDom.cell_count();
      char *scaledValues = (char*)mymalloc(scaledSize);
      Tile *scaledTile = new TransTile(scaledDom, charType, scaledValues, scaledSize);

      r_Minterval nextScaledDom;
      if (nextTile->scaleGetDomain(nextDom, origin, scaleFactors, nextScaledDom))
      {
	cout << "scaledDom: " << scaledDom << ", nextScaledDom: " << nextScaledDom << endl;
	for (i=0; i<dim; i++)
	{
	  if (nextScaledDom[i].low() != scaledDom[i].high() + 1)
	  {
	    cout << "NO SEAMLESS MATCH IN DIM " << i << '!' << endl;
	  }
	}
      }

      scaledTile->execScaleOp(charTile, dom, origin, scaleFactors);

      delete scaledTile;
    }
    else
    {
      cout << "Tile scaled to zero domain" << endl;
    }

    incrementBy = 0.05 * (1 << (int)((scaleBy + eps) / 2));
  }

  delete charTile;
  delete nextTile;
}


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
main( int argc , char* argv[] )
{
  int i;

  i = 1;
  while (i < argc)
  {
    if (argv[i][0] == '-')
    {
      switch (argv[i][1])
      {
	case 'd':
	  strcpy(O2BenchDBName, argv[++i]); break;
	case 's':
	  strcpy(O2BenchSchemaName, argv[++i]); break;
	case 'h':
	  cout << "Use: " << argv[0] << " [-d dbname -s schemaname -h]" << endl;
	  exit(0);
	default:
	  break;
      }
    }
    i++;
  }

  // variables representing O2 database, ta and session
  DatabaseIf database;
  TransactionIf ta;

  // don't forget to initialize before using AdminIf!
  myExecArgv0 = argv[0];
  AdminIf* myAdmin = AdminIf::instance();

  cout << "RManDebug: " << RManDebug << endl;

  // connect to the database
  cout << "Connecting to database " << O2BenchDBName 
       << "..." << endl;
  database.open( O2BenchDBName );

  ta.begin(&database);

  cout << "These tests were done with:" << endl 
       << "  " << rcsid << endl;
  cout << "This protocol was created on:" << endl << "  ";
  printTime();
  cout << endl;

  // testConstructors();
  // testCompression();
  // testUnaryOps();
  // testBinaryOps();
  // testCompTile();
  // testOpsWithConsts();
  // testAlgebraOps();
  // testCellAccess();
  // testScaleOp();
  testScaleOp();

  ta.commit();

  cout << "Ending O2 session..." << endl;
  database.close();
  delete myAdmin;
}
