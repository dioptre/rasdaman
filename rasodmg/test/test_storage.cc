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
 * SOURCE: test_storage.cc
 *
 * MODULE: rasodmg
 *
 * COMMENTS:
 *          None
*/

#include <iostream>
#include <string.h>

#ifdef __VISUALC__
#define __EXECUTABLE__
#endif
#include "rasodmg/ref.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "rasodmg/set.hh"
#include "rasodmg/marray.hh"
#include "raslib/odmgtypes.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/alignedtiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "rasodmg/dirtiling.hh"

#ifdef __VISUALC__
#undef  __EXECUTABLE__
#endif

static int INIT = 0;

r_ULong initWithCounter( const r_Point& /*pt*/ )
{
    return INIT++;
}


r_ULong initWithCrossfoot( const r_Point& pt )
{
    r_ULong value=0;

    for( r_Dimension i=0; i< pt.dimension(); i++ )
        value += pt[i];

    return value;
}


r_ULong initWithCoordinates( const r_Point& pt )
{
    r_ULong value=0;
    int     factor=1;

    for( int i=pt.dimension()-1; i >= 0; i-- )
    {
        value  += factor * pt[i];
        factor *= 100;
    }

    return value;
}



int checkArguments( int argc, char** argv, const char* searchText, int& optionValueIndex )
{
    int found = 0;
    int i=1;

    while( !found && i<argc )
        found = !strcmp( searchText, argv[i++] );

    if( found && i<argc && !strchr(argv[i],'-') )
        optionValueIndex = i;
    else
        optionValueIndex = 0;

    return found;
}




int main( int argc, char** argv )
{
    char serverName[255];
    char baseName[255];
    char collName[255];
    int  optionValueIndex;

    if( argc < 4 || checkArguments( argc, argv, "-h", optionValueIndex ) )
    {
        cout << "Usage:   test_storage server_name base_name collection_name [options]" << endl << endl;
        cout << "Options: -h  ... this help" << endl;
        //    cout << "         -nooutput  ... no output of MDD content" << endl;
        //    cout << "         -hex       ... output in hex" << endl;
        cout << endl;
        return 0;
    }

    strcpy( serverName, argv[1] );
    strcpy( baseName, argv[2] );
    strcpy( collName, argv[3] );

    cout << endl << endl;
    cout << "ODMG conformant insertion of Marrays" << endl;
    cout << "====================================" << endl << endl;

    r_Database db;
    r_Transaction ta;
    r_Ref< r_Set< r_Ref< r_Marray<r_ULong> > > > image_set;
    r_Ref< r_Marray<r_ULong> >                   image1, image2, image3, image4;
    r_Minterval                                  domain, domain2;

    domain  = r_Minterval(2) << r_Sinterval( 0, 100 ) << r_Sinterval( 0, 100 );

    // Default storage layout
    r_Storage_Layout* sl1 = new r_Storage_Layout;
    cout << "sl1 " << *sl1 <<endl;

    // Default tiling with a different tile size than the previous one:
    r_Size_Tiling* til2 = new r_Size_Tiling( 40000 );
    r_Storage_Layout* sl2 = new r_Storage_Layout( til2 );
    //, r_Storage_Layout::No_Compression );
    cout << "sl2 " << *sl2 <<endl;

    // Aligned Tiling
    r_Aligned_Tiling* til3 = new r_Aligned_Tiling( r_Minterval("[0:1,0:2]"), 3200 );
    // cout << *til3;
    r_Storage_Layout* sl3 = new r_Storage_Layout( til3 );
    cout << "sl3 " << *sl3 <<endl;

    // Directional Tiling
    vector<r_Dir_Decompose> decompVec;
    r_Dir_Decompose decompI, decompII;
    decompI << 0 << 10 << 20 << 40 << 60 << 70 << 100;
    decompII << 0 << 10 << 20 << 40 << 60 << 70 << 100;
    decompVec.push_back(decompI);
    decompVec.push_back(decompII);
    r_Dir_Tiling* til4 = new r_Dir_Tiling( 2, decompVec );
    r_Storage_Layout* sl4 = new r_Storage_Layout( til4 );
    cout << "sl4 " << *sl4 <<endl;

    // FIXME adapt this to test also statistical, interest and no tiling
    /*
      // Interest Tiling
      vector<r_Dir_Decompose> decompVec;
      r_Dir_Decompose decompI, decompII;
      decompI << 0 << 10 << 20 << 40 << 60 << 70 << 100;
      decompII << 0 << 10 << 20 << 40 << 60 << 70 << 100;
      decompVec.push_back(decompI);
      decompVec.push_back(decompII);
      r_Dir_Tiling* til4 = new r_Dir_Tiling( 2, decompVec );
      r_Storage_Layout* sl4 = new r_Storage_Layout( til4 );
      cout << "sl4 " << *sl4 <<endl;

      // Statistic Tiling
      vector<r_Dir_Decompose> decompVec;
      r_Dir_Decompose decompI, decompII;
      decompI << 0 << 10 << 20 << 40 << 60 << 70 << 100;
      decompII << 0 << 10 << 20 << 40 << 60 << 70 << 100;
      decompVec.push_back(decompI);
      decompVec.push_back(decompII);
      r_Dir_Tiling* til4 = new r_Dir_Tiling( 2, decompVec );
      r_Storage_Layout* sl4 = new r_Storage_Layout( til4 );
      cout << "sl4 " << *sl4 <<endl;

      // No Tiling
      vector<r_Dir_Decompose> decompVec;
      r_Dir_Decompose decompI, decompII;
      decompI << 0 << 10 << 20 << 40 << 60 << 70 << 100;
      decompII << 0 << 10 << 20 << 40 << 60 << 70 << 100;
      decompVec.push_back(decompI);
      decompVec.push_back(decompII);
      r_Dir_Tiling* til4 = new r_Dir_Tiling( 2, decompVec );
      r_Storage_Layout* sl4 = new r_Storage_Layout( til4 );
      cout << "sl4 " << *sl4 <<endl;
    */


    db.set_servername( serverName );

    try
    {
        cout << "Opening Database " << baseName << " on " << serverName << "... " << flush;
        db.open( baseName );
        cout << "OK" << endl;

        cout << "Starting Transaction ... " << flush;
        ta.begin();
        cout << "OK" << endl;

        cout << "Opening the set ... " << flush;

        try
        {
            image_set = db.lookup_object( collName );
        }
        catch( r_Error &obj )
        {
            cout << "FAILED" << endl;
            // cout << obj.what() << endl;

            //
            // set doesn't exist -> create the set
            //

            cout << "Create the set ... " << flush;

            // create the set
            image_set = new( &db, "ULongSet" ) r_Set< r_Ref< r_Marray<r_ULong> > >;

            // create a name for the persistent set in order to be able to look it up again
            db.set_object_name( *image_set, collName );
        }

        cout << "OK" << endl;

        cout << "Creating four images ... " <<  flush;

        // create first image
        image1 = new( &db, "ULongImage" ) r_Marray<r_ULong>( domain, (r_ULong)0, sl1 );

        // create second image
        image2 = new( &db, "ULongImage" ) r_Marray<r_ULong>( domain, &initWithCrossfoot, sl2 );

        // create third image
        image3 = new( &db, "ULongImage" ) r_Marray<r_ULong>( domain, &initWithCounter, sl3 );

        // create fourth image
        image4 = new( &db, "ULongImage" ) r_Marray<r_ULong>( domain, &initWithCoordinates, sl4 );

        cout << "OK" << endl;

        cout << "Inserting them into the set " << collName << " ... " << flush;

        // insert the images
        image_set->insert_element( image1 );
        image_set->insert_element( image2 );
        image_set->insert_element( image3 );
        image_set->insert_element( image4 );

        cout << "OK" << endl;

        cout << "Commiting Transaction ... " << flush;
        ta.commit();
        cout << "OK" << endl;

        cout << "Closing Database ... " << flush;
        db.close();
        cout << "OK" << endl;
    }
    catch( r_Error& errorObj )
    {
        cerr << errorObj.what() << endl;
        return -1;
    }

    return 0;
}
