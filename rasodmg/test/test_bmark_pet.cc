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
 * SOURCE: test_bmark_pet.cc
 *
 * MODULE: rasodmg
 *
 * PURPOSE: benchmark interest tiling
 *
 * COMMENTS:
 *   This program is used to load the database with information
 *   for benchmarking interesting tiling
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
#include "include/basictypes.hh"

#define S_32K               (32  * 1024L)
#define S_64K               (64  * 1024L)
#define S_128K              (128 * 1024L)
#define S_256K              (256 * 1024L)

#define TOTAL_CUBES         7
#define SIZE_X              185L  
#define SIZE_Y              150L  
#define SIZE_Z              141L  

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

void insert_datacube( )
{

  r_Ref< r_Set< r_Ref< r_Marray<r_Short> > > >  cube_set;
  r_Minterval                                   domain[TOTAL_CUBES];
  r_Domain_Storage_Layout*                      dsl[TOTAL_CUBES];
  r_OId                                         oid[TOTAL_CUBES];
  
  for (int i = 0; i < TOTAL_CUBES; i++)
  {
    domain[i] = r_Minterval(3);
    domain[i] << r_Sinterval(0L, SIZE_X - 1 )
              << r_Sinterval(0L, SIZE_Y - 1)
              << r_Sinterval(0L, SIZE_Z - 1);
  }

  // For aligned tiling (Regular tiling)
  
  r_Minterval block_config(3);
  block_config << r_Sinterval(0L, SIZE_X)
               << r_Sinterval(0L, SIZE_Y)
               << r_Sinterval(0L, SIZE_Z);
  
  r_Aligned_Tiling* til_reg_32k = new r_Aligned_Tiling(block_config, S_32K);
  r_Aligned_Tiling* til_reg_64k = new r_Aligned_Tiling(block_config, S_64K);
  r_Aligned_Tiling* til_reg_128k = new r_Aligned_Tiling(block_config, S_128K);
  r_Aligned_Tiling* til_reg_256k = new r_Aligned_Tiling(block_config, S_256K);
  r_Aligned_Tiling* til_reg_64k1 = new r_Aligned_Tiling(block_config, S_64K);


  // For areas of interest  tiling
  r_Minterval interest1_1("[23:170,4:75,12:138]");
  r_Minterval interest1_2("[61:149,70:135,8:130]");
  r_Minterval interest2_1("[28:175,2:73,8:134]");
  r_Minterval interest2_2("[63:151,75:140,11:133]");

  
  DList<r_Minterval> areas1;
  areas1 += interest1_1;
  areas1 += interest1_2;
  DList<r_Minterval> areas2;
  areas2 += interest2_1;
  areas2 += interest2_2;

  r_Interest_Tiling* til_int1 = 
     new r_Interest_Tiling(areas1, r_Interest_Tiling::NO_LIMIT);
  r_Interest_Tiling* til_int2 = 
     new r_Interest_Tiling(areas2, r_Interest_Tiling::NO_LIMIT);
  
  // Domain storage layouts
  
  dsl[0] = new r_Domain_Storage_Layout(domain[0], til_reg_32k);
  dsl[1] = new r_Domain_Storage_Layout(domain[1], til_reg_64k);
  dsl[2] = new r_Domain_Storage_Layout(domain[2], til_reg_128k);
  dsl[3] = new r_Domain_Storage_Layout(domain[3], til_reg_256k);

  dsl[4] = new r_Domain_Storage_Layout(domain[4], til_int1);
  dsl[5] = new r_Domain_Storage_Layout(domain[5], til_int2);
  dsl[6] = new r_Domain_Storage_Layout(domain[6], til_reg_64k1);
  
     
  // Create cubes

  r_Database db;
  db.set_servername(server_name);
  
  for ( i=0; i< TOTAL_CUBES ; i++)
  {
    r_Transaction trans;

    r_Ref< r_Marray<r_Short> > cube;
   
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
            new(&db, "ShortSet3") r_Set< r_Ref< r_Marray<r_Short> > >;

        db.set_object_name(*cube_set, colect_name);
      } 

      cout << "Ok" << endl;
      cout << "Creating the datacube... " << flush;
      r_Minterval newDomain( domain[i]);
      cube =
          new(&db, "ShortCube") r_Marray<r_Short>(newDomain, dsl[i]);

      cube_set->insert_element(cube);

      cout << "Cube[" << i+1 << "]:  " << cube->get_oid() << endl;
      cout << "Spatial domain: " << cube->spatial_domain( ) <<endl;
      cout << "Storage Layout " << endl;
      dsl[i]->print_status( ); 
     
      cout << "*" << flush;
      cout << " ... Ok" << endl;
      cout << "Commiting transaction... " << flush;    

      trans.commit();
    
      cout << "Ok" << endl; 
      // cout << "Destroying cube... " <<flush;
      // cube.destroy( ); 
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
}

int main(int argc, char* argv[])
{
  parse(argc, argv);
  insert_datacube( );
  return 0;
}








