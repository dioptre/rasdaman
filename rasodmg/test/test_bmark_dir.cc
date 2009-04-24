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
 * SOURCE: test_bmark_dir.cc
 *
 * MODULE: rasodmg
 *
 * PURPOSE: benchmark dirtiling
 *
 * COMMENTS:
 *   This program creates a 3D datacube for benchmarking
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
#include "rasodmg/odmgtypes.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/tiling.hh"
#include "rasodmg/dirtiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "rasodmg/storagelayout.hh"
#include "raslib/oid.hh"

#define TOTAL_DAYS          (365L * 2L)
#define TOTAL_PRODUCTS      (60L)
#define TOTAL_STORES        (100L)

#define MAX_SALES           500

#define S_32K               (32  * 1024L)
#define S_64K               (64  * 1024L)
#define S_128K              (128 * 1024L)
#define S_256K              (256 * 1024L)

#define TOTAL_CUBES         9


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

r_ULong init(const r_Point&)
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


  // For alligned tiling (Regular tiling)
  
  block_config = r_Minterval(3);
  block_config << r_Sinterval(0L, TOTAL_DAYS)
               << r_Sinterval(0L, TOTAL_PRODUCTS)
               << r_Sinterval(0L, TOTAL_STORES);
  
  r_Aligned_Tiling* til_reg_32k = new r_Aligned_Tiling(block_config, S_32K);
  r_Aligned_Tiling* til_reg_64k = new r_Aligned_Tiling(block_config, S_64K);
  r_Aligned_Tiling* til_reg_128k = new r_Aligned_Tiling(block_config, S_128K);
  r_Aligned_Tiling* til_reg_256k = new r_Aligned_Tiling(block_config, S_256K);


  // For directional tiling

  r_Dir_Decompose decomp[3];
  decomp[0] << 1 << 365 << 730;
  decomp[1] << 1 <<  27 <<  42 << 60;
  decomp[2] << 1 <<  27 <<  35 << 41 << 59 << 73 << 89 << 97 << 100;

  r_Dir_Tiling* til_dir_32k = new r_Dir_Tiling(3, decomp, S_32K);
  r_Dir_Tiling* til_dir_64k = new r_Dir_Tiling(3, decomp, S_64K);
  r_Dir_Tiling* til_dir_128k = new r_Dir_Tiling(3, decomp, S_128K);
  r_Dir_Tiling* til_dir_256k = new r_Dir_Tiling(3, decomp, S_256K);
  r_Dir_Tiling* til_dir_Unlk = new r_Dir_Tiling(3, decomp, S_256K, r_Dir_Tiling::WITHOUT_SUBTILING);


  // Domain storage layouts

  dsl[0] = new r_Domain_Storage_Layout(domain, til_reg_32k);
  dsl[1] = new r_Domain_Storage_Layout(domain, til_reg_64k);
  dsl[2] = new r_Domain_Storage_Layout(domain, til_reg_128k);
  dsl[3] = new r_Domain_Storage_Layout(domain, til_reg_256k);

  dsl[4] = new r_Domain_Storage_Layout(domain, til_dir_32k);
  dsl[5] = new r_Domain_Storage_Layout(domain, til_dir_64k);
  dsl[6] = new r_Domain_Storage_Layout(domain, til_dir_128k);
  dsl[7] = new r_Domain_Storage_Layout(domain, til_dir_256k);
  dsl[8] = new r_Domain_Storage_Layout(domain, til_dir_Unlk);

  
for (int i=0; i< TOTAL_CUBES ; i++)
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
      cube1 =
          new(&db, "ULong_3D_Cube") r_Marray<r_ULong>(domain, 1L /* &init */, dsl[i]);

      // cube_set->insert_element(cube[i]);
      // oid[i] = cube[i].get_oid();
      cube_set->insert_element(cube1);
      oid[i] = cube1->get_oid();
      
      cout << "*" << flush;


    cout << " ... Ok" << endl;
    
    cout << "  Cube[" << i << "]:  " << oid[i] << endl;
		    
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

// Wrong - management of memory for storage layouts passes 
//         to the marray, after being given to it.
//     for (int j=0; j<TOTAL_CUBES; j++)
//       delete dsl[j];

    cout << endl;
    cout << "Inserted data resume" << endl;
    cout << "====================" << endl;
    
    //    for (int k=0; k<TOTAL_CUBES; k++)
    //  cout << "  Cube[" << k << "]:  " << oid[k] << endl;
}

int main(int argc, char* argv[])
{
  parse(argc, argv);
  insert_datacube();
  
  return 0;
}








