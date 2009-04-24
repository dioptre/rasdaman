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
 * SOURCE: test_bmark_dir1.cc
 *
 * MODULE: rasodmg
 *
 * PURPOSE: benchmark dirtiling
 *
 * COMMENTS:
 *   This program creates 3D and 5D datacubes for benchmarking
 *   the directional tiling method
 *
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "rasodmg/ref.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "rasodmg/set.hh"
#include "rasodmg/marray.hh"
#include "raslib/odmgtypes.hh"
#include "rasodmg/tiling.hh"
#include "rasodmg/dirtiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "rasodmg/storagelayout.hh"
#include "raslib/oid.hh"

#define YEARS               (2L)

#define TOTAL_DAYS          (365L * 2L)
#define TOTAL_PRODUCTS      (60L)
#define TOTAL_STORES        (100L)


#define MAX_SALES           500

#define S_32K               (32  * 1024L)
#define S_64K               (64  * 1024L)
#define S_128K              (128 * 1024L)
#define S_256K              (256 * 1024L)

#define TOTAL_CUBES         10


char* server_name;
char* dbase_name;
char* colect_name;

void parse(int argc, char* argv[])
{
  if (argc != 4)
  {
    cout << "Usage: " << argv[0] << " [server name] [db name] [colection name]"
         << endl;

    exit(0);
  }

  server_name = argv[1];
  dbase_name  = argv[2];
  colect_name = argv[3];
}

r_ULong init(const r_Point& pnt )
{
  return (long)(rand() % MAX_SALES);
}

void insert_datacube()
{

  r_Ref< r_Set< r_Ref< r_Marray<r_ULong> > > >  cube_set;
  // r_Ref< r_Marray<r_ULong> >                 cube[TOTAL_CUBES];
  r_Minterval                                   domain, block_config;
  r_Domain_Storage_Layout*                      dsl[TOTAL_CUBES];
  r_OId                                         oid[TOTAL_CUBES];
  
  domain = r_Minterval(3);
  domain << r_Sinterval(1L, TOTAL_DAYS)
         << r_Sinterval(1L, TOTAL_PRODUCTS)
         << r_Sinterval(1L, TOTAL_STORES);


  block_config = r_Minterval(3);
  block_config << r_Sinterval(0L, TOTAL_DAYS)
               << r_Sinterval(0L, TOTAL_PRODUCTS)
               << r_Sinterval(0L, TOTAL_STORES);
  
  // Each storage object must have an own dynamic tiling obj or else the client 
  // ( and server ) crashes because memory is released for a non heap memory free 
  // r_Marray become responsible for managing the memory allocated for the
  // tiling object.
  r_Aligned_Tiling* til_reg_32k = new r_Aligned_Tiling(block_config, S_32K);
  r_Aligned_Tiling* til_reg_64k = new r_Aligned_Tiling(block_config, S_64K);
  r_Aligned_Tiling* til_reg_128k = new r_Aligned_Tiling(block_config, S_128K);
  r_Aligned_Tiling* til_reg_256k = new r_Aligned_Tiling(block_config, S_256K);


  // For directional tiling

  r_Dir_Decompose decomp[3];
    
  //                Jan  Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dez
  unsigned int daysMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int ix = 0; 
  int year;
  int month;
  decomp[0] << 1;
  for ( year = 0; year < YEARS ; year++)
  {
    for (month = 0; month < 12; month++ )
    {
      decomp[0] << ix + daysMonth[month];
      ix += daysMonth[month];  
    }  
  }

  decomp[1] << 1 << 60;

  // << 1 << 1 + 26 << 27 + 8 << 35 + 6 << 41 + 18 << 59 + 14 << 73 + 16 << 89 + 8 << 97 + 3;
  decomp[2] << 1 <<  27 <<  35 << 41 << 59 << 73 << 89 << 97 << 100;
  

  r_Dir_Tiling* til_dir_32k = new r_Dir_Tiling(3, decomp, S_32K);
  r_Dir_Tiling* til_dir_64k = new r_Dir_Tiling(3, decomp, S_64K);
  r_Dir_Tiling* til_dir_128k = new r_Dir_Tiling(3, decomp, S_128K);
  r_Dir_Tiling* til_dir_256k = new r_Dir_Tiling(3, decomp, S_256K);


  r_Dir_Decompose decomp1[3];

  ix = 0;
  decomp1[0] << 1;
  for ( year = 0; year < YEARS ; year++)
  {
    for (month = 0; month < 12; month++ )
    {
      decomp1[0] << ix + daysMonth[month];
      ix += daysMonth[month];  
    }  
  }

  // Products 
  // << 1<< 1+ 26 << 27 + 15 << 42 + 28
  decomp1[1] << 1 <<  27 <<  42 << 60;

  // Stores 
  // << 1 << 1 + 26 << 27 + 8 << 35 + 6 << 41 + 18 << 59 + 14 << 73 + 16 << 89 + 8 << 97 + 3;
  decomp1[2] << 1 <<  27 <<  35 << 41 << 59 << 73 << 89 << 97 << 100;

  r_Dir_Tiling* til_dir1_32k = new r_Dir_Tiling(3, decomp1, S_32K);
  r_Dir_Tiling* til_dir1_64k = new r_Dir_Tiling(3, decomp1, S_64K);

  // Domain storage layouts

  dsl[0] = new r_Domain_Storage_Layout(domain, til_reg_32k);
  dsl[1] = new r_Domain_Storage_Layout(domain, til_reg_64k);
  dsl[2] = new r_Domain_Storage_Layout(domain, til_reg_128k);
  dsl[3] = new r_Domain_Storage_Layout(domain, til_reg_256k);

  dsl[4] = new r_Domain_Storage_Layout(domain, til_dir_32k);
  dsl[5] = new r_Domain_Storage_Layout(domain, til_dir_64k);
  dsl[6] = new r_Domain_Storage_Layout(domain, til_dir_128k);
  dsl[7] = new r_Domain_Storage_Layout(domain, til_dir_256k);

  dsl[8] = new r_Domain_Storage_Layout(domain, til_dir1_32k);
  dsl[9] = new r_Domain_Storage_Layout(domain, til_dir1_64k);

  
for (int i= 0 ; i< TOTAL_CUBES ; i++)
{
  r_Database db;
  r_Transaction trans;
  // The main phase of the database creation
  r_Ref< r_Marray<r_ULong> > cube1;
   
  db.set_servername(server_name);
  
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

      cube_set = new(&db, "ULong_3D_Set") r_Set< r_Ref< r_Marray<r_ULong> > >;
      db.set_object_name(*cube_set, colect_name);
    } 

    cout << "Ok" << endl;
    cout << "Creating the datacube... " << flush;

    // for (int i=0; i<TOTAL_CUBES; i++)
    //  {
    // cube[i] =
      cout << "domain == " << domain << endl;
      cube1 =
          new(&db, "ULong_3D_Cube") r_Marray<r_ULong>(domain, 1L/* &init */, dsl[i]);

      // cube_set->insert_element(cube[i]);
      // oid[i] = cube[i].get_oid();
      cube_set->insert_element(cube1);
      oid[i] = cube1->get_oid();
      
      cout << "*" << flush;


    cout << " ... Ok" << endl;
    
    cout << "  Cube[" << i+1 << "]:  " << oid[i] << endl;
    cout << "  Spatial domain: " << cube1->spatial_domain( ) << endl;
    cout << "  Type length: " << cube1->get_type_length( ) << endl;
    cout << "  Storage Layout:  "; 
    if( i < 4 )
     cout << "regular; tile size " << dsl[i]->get_tile_size( ) << endl;
    else 
    {
     cout << "directional; tile size " << dsl[i]->get_tile_size( ) << endl;
     cout << "Dir decompose:  ";              
     for ( int j = 0; j < 3 ; j++ )
     {
       for ( int k = 0; k < 3 ; k++)
       {
         if ( i < 8 ) 
           decomp[k].print_status(cout );
         else 
           decomp1[k].print_status( cout );
       }
     }
     cout << endl;
    }
		    
    cout << "Commiting transaction... " << flush;    

    trans.commit();
    
    cout << "Ok" << endl; 

    cout << "Closing database... " << flush;

    db.close();
     
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

    cout << "Ok [******************]" << endl << flush;

    cout << endl;
    cout << "Inserted data resume" << endl;
    cout << "====================" << endl;
}

int main(int argc, char* argv[])
{
  parse(argc, argv);
  insert_datacube();
  
  return 0;
}








