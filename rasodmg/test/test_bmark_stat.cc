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
 * SOURCE: test_bmark_stat.cc
 *
 * MODULE: rasodmg
 *
 * PURPOSE: Load database for statistic tiling benchmark
 *
 * COMMENTS:
 *      None
*/

/*
   ATENTION: The format of the input file for using with this program is:

     border_threshold accesses_threshold tilesize domain
     access1
     access2
     accessXXX
     ...

   Example:

     50 0.20 1000 [0:799, 0:599]
     [10:20, 30:40]
     [12:20, 35:39]
     [100:300, 300:400]
     [120:300, 310:410]
     [200:500, 350:500]
     [200:510, 350:500]
*/


#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include "raslib/oid.hh"
#include "raslib/dlist.hh"
#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/set.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/tiling.hh"
#include "rasodmg/database.hh"
#include "rasodmg/odmgtypes.hh"
#include "rasodmg/stattiling.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/storagelayout.hh"
#include "RGBCube.hh"

#define S_32K               (32  * 1024L)
#define S_64K               (64  * 1024L)
#define S_128K              (128 * 1024L)
#define S_256K              (256 * 1024L)

#define TOTAL_CUBES         8
#define SIZE_X              120L
#define SIZE_Y              159L
#define SIZE_Z              119L

char* filename;
char* server_name;
char* dbase_name;
char* colect_name;
int   cube_i;

DList<r_Access>  stat_info;
unsigned int     border_threshold;
double           interesting_threshold;
unsigned long    tile_size;
r_Minterval*     domain;

void parse(int argc, char* argv[])
{
    if (argc != 6)
    {
        cout << "Usage: " << argv[0] << " server db collec ptfile cube" << endl;
        cout << endl;
        cout << "  server ............. server to use" << endl;
        cout << "  db ................. database" << endl;
        cout << "  collec ............. collection" << endl;
        cout << "  ptfile ............. pattern file" << endl;
        cout << "  cube ............... datacube to create" << endl;

        exit(1);
    }

    server_name = argv[1];
    dbase_name  = argv[2];
    colect_name = argv[3];
    filename    = argv[4];
    cube_i      = atoi(argv[5]);

    if ((cube_i<0) || (cube_i>7))
    {
        cout << "Invalid datacube. Must be in 0..7" << endl;
        exit(0);
    }
}

void read_data()
{
    const unsigned int BUF_SIZE = 256;
    char buf[BUF_SIZE], buf2[BUF_SIZE];

    cout << "Opening " << filename << " for reading... ";

    ifstream is(filename, ios::in);
    if (!is)
    {
        cout << "Couldn't open!!!" << endl;
        exit(1);
    }
    else
        cout << "done." << endl;

    cout << "Reading parameters... ";

    is >> border_threshold;
    is >> interesting_threshold;
    is >> tile_size;

    is.getline(buf, BUF_SIZE);
    domain = new r_Minterval(buf);

    cout << "done." << endl;
    cout << "Geting the accesses... ";

    unsigned long count = 0;

    while (!is.eof())
    {
        is.getline(buf, BUF_SIZE);
        if (sscanf(buf, "%s", buf2) == 1)
        {
            r_Minterval inter(buf);
            stat_info += inter;
            ++count;

            cout << "*";
        }
    }

    is.close();

    cout << endl;
    cout << "Geting the accesses... done." << endl << endl;

    cout << "Border threshold      = " << border_threshold << endl;
    cout << "Interesting threshold = " << interesting_threshold << endl;
    cout << "Tile size             = " << tile_size << endl;
    cout << "Domain                = " << *domain << endl;
    cout << "Number of accesses    = " << count << endl << endl;
}


void insert_datacube()
{

    r_Ref< r_Set< r_Ref< r_Marray<r_ULong> > > >  cube_set;
    r_Minterval                                   domain;

    domain = r_Minterval(3);
    domain << r_Sinterval(0L, SIZE_X)
           << r_Sinterval(0L, SIZE_Y)
           << r_Sinterval(0L, SIZE_Z);


    // For alligned tiling (Regular tiling)

    r_Minterval block_config(3);
    block_config << r_Sinterval(0L, SIZE_X)
                 << r_Sinterval(0L, SIZE_Y)
                 << r_Sinterval(0L, SIZE_Z);

    unsigned long ts;
    switch (cube_i)
    {
    case 0:
        ts = S_32K;
        break;
    case 1:
        ts = S_64K;
        break;
    case 2:
        ts = S_128K;
        break;
    case 3:
        ts = S_256K;
        break;
    default:  // Irrelevant, not used.
        ts = S_32K;
        break;
    }

    r_Aligned_Tiling til_reg(block_config, ts);
    r_Stat_Tiling til_stat(border_threshold, interesting_threshold, tile_size);

    if (cube_i > 3)
    {
        til_stat.update_stat_information(stat_info);
    }


    // Domain storage layout

    // This is a hack due to problems with the pointers
    r_Domain_Storage_Layout* dsl[2];
    r_Domain_Storage_Layout* use;

    dsl[0] = new r_Domain_Storage_Layout(domain, &til_reg);
    dsl[1] = new r_Domain_Storage_Layout(domain, &til_stat);

    if (cube_i<4)
        use = dsl[0];
    else
        use = dsl[1];

    // Create cube

    r_Database db;
    db.set_servername(server_name);
    r_Transaction trans;
    r_Ref< r_Marray<r_ULong> > cube;

    try
    {
        cout << "Opening database " << dbase_name << " on " << server_name
             << "... " << flush;

        db.open(dbase_name);

        cout << "Ok" << endl;
        cout << "Starting transaction... " << flush;

        trans.begin();

        cout << "Ok" << endl;
        cout << "Opening the set... " << flush;

        try
        {
            cube_set = db.lookup_object(colect_name);
        }
        catch (...)
        {
            cout << "*Failed*" << endl;
            cout << "Creating the set... " << flush;

            cube_set =
                new(&db, "RGB_3D_Set") r_Set< r_Ref< r_Marray<RGBPixel> > >;

            db.set_object_name(*cube_set, colect_name);
        }

        cout << "Ok" << endl;
        cout << "Creating the datacube... " << flush;

        cube =
            new(&db, "RGB_3D_Cube") r_Marray<RGBPixel>(domain, use);

        cube_set->insert_element(cube);

        cout << "*" << flush;
        cout << " ... Ok" << endl;
        cout << "Commiting transaction... " << flush;

        trans.commit();

        cout << "Ok" << endl;
        cout << "Closing database... " << flush;

        db.close();

        cout << " Ok" << endl << flush;
    }
    catch (r_Error& e)
    {
        cout << e.what() << endl;
        exit(0);
    }
    catch (...)
    {
        cout << "Undefined error..." << endl;
        exit(0);
    }
}


int main(int argc, char* argv[])
{
    parse(argc, argv);
    read_data();
    insert_datacube();

    return 0;
}











