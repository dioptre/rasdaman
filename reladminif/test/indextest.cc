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
#include <stdlib.h>
#include <iostream>

#include "chartype.hh"
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
#include "mdddimensiontype.hh"
#include "settype.hh"

#ifndef BASEDB_O2
#include "externs.h"
#endif

RMINITGLOBALS('C')
#ifndef BASEDB_O2
char* O2DBName = "RASBASE";
#else
char* O2DBName = "NorbertBase";
#endif
char *collName = "testColl";
char defaultCollName[]= "ObjsContainer";

#ifdef BASEDB_O2
extern char* myExecArgv0 = "";
#endif

int
main( int argc, char** argv)
{
#ifdef BASEDB_O2
    myExecArgv0 = argv[0];
#endif
    RManDebug = 3;
#ifndef BASEDB_O2
    RManModule = 0;
#endif
    RMInit::logOut = cout.rdbuf();
    RMInit::dbgOut = cout.rdbuf();
    RMInit::tileSize = 12;
    MDDStorage::DefaultIxType = MDDStorage::RPlusTreeNode;

    // variables representing O2 database, ta and session
    DatabaseIf db;

    // don't forget to initialize before using AdminIf!
    AdminIf* myAdmin = AdminIf::instance();
    TransactionIf ta;
    // connect to the database
    cout << "Connecting to database " << O2DBName << "..." << endl;
    int errorDBOpen;
    try
    {
        errorDBOpen = db.open(O2DBName);
    }
    catch(...)
    {
        cout << "Caught Exception " << endl;
        exit(-1);
    }
    ta.begin(&db);
    int i = PersMDDColl::destroyRoot(collName, &db);
    ta.commit();
    ta.begin(&db);
    const CollectionType* greyset = TypeFactory::mapSetType("GreySet");
    MDDDimensionType* mddtype = (MDDDimensionType*)TypeFactory::mapMDDType("GreyCube");
    if (!mddtype || !greyset)
    {
        cout << "didnt find types" << endl;
        exit(-1);
    }
    OId oColl;
    if (OId::allocateMDDCollOId(&oColl) == 0)
    {
    }
    else
        cout <<"Error allocating OId for collection " << endl;
    PersMDDColl* col;
    try
    {
        // CollectionType* ct = TypeFactory::mapSetType("ObjsContainerType");
        col = PersMDDColl::createRoot(collName, oColl, greyset, &db);
    }
    catch (...)
    {
        cout << "Error creating PersMDDColl" << endl;
        exit(-1);
    }

    greyset = TypeFactory::mapSetType("GreySet3");
    mddtype = (MDDDimensionType*)TypeFactory::mapMDDType("GreyCube");

    OId oid1;
    if (OId::allocateMDDOId(&oid1) == 0)
    {

    }
    else
    {
        cout << "Error by allocation of OId" <<endl;
        exit(1);
    }

    // read root object

    PersMDDColl objsSet(collName);


    // create MDD Object 1

    r_Sinterval limits1Obj1(0l,100l);
    r_Sinterval limits2Obj1(0l,100l);
    r_Sinterval limits3Obj1(0l,100l);
    r_Minterval dom(3);
    dom << limits1Obj1 << limits2Obj1 << limits3Obj1;

    PersMDDObj* MDDObj1 = new PersMDDObj(mddtype, dom, O2DBName, oid1);
    cout << "created new persmddobj" << endl;

    char* test = (char*)mymalloc((dom.cell_count() + 1) * mddtype->getBaseType()->getSize());
    for (i = 0; i < 1000; i++)
        test[i] = i%255;

    dom[0].set_interval(0l,1l);
    dom[1].set_interval(0l,1l);
    dom[2].set_interval(0l,1l);
    PersTile* tile1Obj1 = new PersTile(dom, mddtype->getBaseType(), test);
    MDDObj1->insertTile(tile1Obj1);
    cout << "1--------------" << endl;

    dom[0].set_interval(2l,3l);
    dom[1].set_interval(0l,1l);
    dom[2].set_interval(0l,1l);
    tile1Obj1 = new PersTile(dom, mddtype->getBaseType(), test);
    MDDObj1->insertTile(tile1Obj1);
    cout << "2--------------" << endl;

    dom[0].set_interval(4l,5l);
    dom[1].set_interval(0l,1l);
    dom[2].set_interval(0l,1l);
    tile1Obj1 = new PersTile(dom, mddtype->getBaseType(), test);
    MDDObj1->insertTile(tile1Obj1);
    cout << "3--------------" << endl;

    dom[1].set_interval(0l,1l);
    dom[0].set_interval(20l,21l);
    dom[2].set_interval(0l,1l);
    tile1Obj1 = new PersTile(dom, mddtype->getBaseType(), test);
    MDDObj1->insertTile(tile1Obj1);
    cout << "4--------------" << endl;

    dom[1].set_interval(0l,1l);
    dom[0].set_interval(22l,23l);
    dom[2].set_interval(0l,1l);
    tile1Obj1 = new PersTile(dom, mddtype->getBaseType(), test);
    MDDObj1->insertTile(tile1Obj1);
    cout << "5--------------" << endl;

    dom[1].set_interval(0l,1l);
    dom[0].set_interval(24l,25l);
    dom[2].set_interval(0l,1l);
    tile1Obj1 = new PersTile(dom, mddtype->getBaseType(), test);
    MDDObj1->insertTile(tile1Obj1);
    cout << "6--------------" << endl;

    dom[1].set_interval(0l,1l);
    dom[0].set_interval(10l,11l);
    dom[2].set_interval(0l,1l);
    tile1Obj1 = new PersTile(dom, mddtype->getBaseType(), test);
    MDDObj1->insertTile(tile1Obj1);
    cout << "7--------------" << endl;

    dom[1].set_interval(0l,1l);
    dom[0].set_interval(26l,27l);
    dom[2].set_interval(0l,1l);
    tile1Obj1 = new PersTile(dom, mddtype->getBaseType(), test);
    MDDObj1->insertTile(tile1Obj1);
    cout << "8--------------" << endl;

    objsSet.insert(MDDObj1);

    r_Point po("[10,0,1]");
    char* utest = MDDObj1->pointQuery(po);
    if (utest)
        cout << "works" << endl;
    else
        cout << "failure" << endl;
    ta.commit();
    db.close();
}


