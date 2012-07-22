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
 * SOURCE: test_transmddcoll.cc
 *
 * MODULE: test for transmddobj of cachetamgr
 *
 * COMMENTS:
 *   none
 *
*/

#include <stdlib.h>
#include <iostream>
#include <vector.h>                  // STL
#include "tilemgr/transtile.hh"
#include "tilemgr/transmddobj.hh"
#include "ulongtype.hh"              // from catalogif
#include "raslib/minterval.hh"
#include "tilemgr/transmddcoll.hh"
#include "tilemgr/mddcolliter.hh"
#include "adminif.hh"

// Needed by Adminif. Adminif has to be instantiated because
// of cell base types.
extern char* myExecArgv0 = "";
#include "raslib/rminit.hh"
RMINITGLOBALS('C')


/*************************************************************
 * Function name.: int main( int argc, char** argv)
 *
 * Arguments.....:
 *   argc: number of arguments given to program
 *   argv: array of char* with arguments
 * Return value..: exit status
 * Description...:
 *    The cellType of the MDD objects in this program is
 *    not set correctly because no database is open and no
 *    transaction is started.
 ************************************************************/

int
main( int argc, char** argv)
{
    myExecArgv0 = argv[0];

    const int numObjs = 10;
    const int numTilesObj = 10;
    r_Sinterval domSinterval;

    // In order to work with cell base types, AdminIf has to be
    // instantiated.
    // cout << " Adminif::instance " << endl;
    AdminIf* myAdmin = AdminIf::instance();

    ULongType anyType;

    char anyCell[4];
    int i;
    MDDColl* tCollMDDObjs = new TransMDDColl( );

    cout << " Allocating new TransMDDColl ..."<<endl;
    tCollMDDObjs = new TransMDDColl( );
    cout << " new TransMDDColl allocated..."<<endl;

    cout << "Creating transient tiles for the MDD objects ... " << endl;
    vector<TransTile*>* tilesVectsArr[numObjs];

    // initialize array of vectors of tiles
    for (i=0; i<numObjs; i++)
    {
        tilesVectsArr[i] = new vector<TransTile*>(numTilesObj);
        for (int j=0; j<numTilesObj ; j++)
        {
            r_Minterval dom(2);
            domSinterval.set_interval( r_Range(j* 10), r_Range((j+1)*10-1) );
            dom <<  domSinterval << domSinterval;
            (*tilesVectsArr[i])[j] = new TransTile( dom, &anyType, anyCell );
        }
    }


    cout << "Creating transient MDD objects ... " << endl;
    TransMDDObj* MDDObjsArr[numObjs];

    for ( i=0; i<numObjs; i++ )
    {
        r_Minterval dom(2);
        domSinterval.set_interval( r_Range( i ), r_Range( 100 + i*10 ) );
        if( i == 3 || i == 5 || i == 7 )
            domSinterval.set_low( '*' );
        if( i == 2 || i == 4 || i == 6 )
            domSinterval.set_high( '*' );
        dom << domSinterval << domSinterval;
        MDDObjsArr[i] = new TransMDDObj( dom, "ULong" );
        for( int j=0; j < numTilesObj; j++)
        {
            vector<TransTile*>* pTilesVec = tilesVectsArr[i];
            MDDObjsArr[i]->insertTile( (*pTilesVec)[j] );
        }
    }

    cout << "Printing contents of created objects ... " << endl;
    for( i = 0; i<  numObjs; i++)
    {
        cout << "- " << i << ". Transient MDD Object contents: " <<endl;
        MDDObjsArr[i]->printStatus( );
        cout << endl;
    }

    cout << "Creating a transient collection of objects ... " << endl;
    for( i = 0; i<  numObjs; i++)
    {
        // cout << "- " << i << ". Transient MDD Object contents: " <<endl;
        tCollMDDObjs->insert(MDDObjsArr[i]);
        // cout << endl;
    }
    cout << "Contents of the Transient Collection: ... : " << endl;
    tCollMDDObjs->printStatus( );

    cout << "Testing TransMDDCollIter ... : " << endl;
    MDDCollIter* transIter = tCollMDDObjs->createIterator( );
    MDDObj* currObj;

    for ( i=0; transIter->notDone( ) ; transIter->advance( ), i++)
    {
        cout << "- " << i << ". Trans. MDD Object returned by Iterator contents: " <<endl;
        currObj = transIter->getElement( );
        currObj->printStatus( );
    }

    // delete iterator from TransMDDColl
    delete transIter;

    // releases all contents from the collection. It should free all the MDD
    // objects and tiles created in this program.
    tCollMDDObjs->releaseAll( );

    // delete transient MDD Collection
    delete tCollMDDObjs;

    /*
    This shouldn't be needed if realeaseAll from the TransMDDColl works fine
    for ( i = 0 ; i < numObjs; i++ )
    {
      delete MDDObjsArr[i];
    }
    */


    // delete dynamically allocated vectors of tiles (not the tiles themselves,
    // which should have been freed by releaseAll of TransMDDColl)
    for (i = 0; i < numObjs; i++)
        delete tilesVectsArr[i];

    delete myAdmin;

}
