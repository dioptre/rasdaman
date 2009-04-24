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
 * Example program for computing the avg cell value 
 * for each image in a given 2-D RGB collection.
 *
 *
 ************************************************************/

#include <iostream>

// Linux needs this for template instantiation
#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif


// rasdl generated data type file:
// (contains predefined RGB pixel structure)
#include "basictypes.hh"


int main( int ac, char** av )
{   
  char rasmgrName[255];
  int  rasmgrPort;
  char baseName[255];
  char collName[255];
  char userName[255];
  char userPass[255];
  
  double sum;
   
  if( ac != 7 )
  {
    cout << "Usage: avg-cell-red rasmgr_name rasmgr_port base_name collection_name user_name user_passwd" << endl;
    return -1;
  }

  strcpy( rasmgrName, av[1] );
  rasmgrPort = strtoul( av[2], NULL, 0);
  strcpy( baseName,   av[3] );
  strcpy( collName,   av[4] );
  strcpy( userName,   av[5] );
  strcpy( userPass,   av[6] );

  r_Database                           database;
  r_Transaction                        transaction;
  r_Ref< r_Set< r_Ref< r_Marray< RGBPixel > > > > image_set;
  r_Iterator< r_Ref< r_Marray< RGBPixel > > > iter;
  r_Ref< r_Marray< RGBPixel > >               image;
  r_Range                              i,j;
  long 	                               pixelcount;

  try
  {
    database.set_servername( rasmgrName, rasmgrPort );
    database.set_useridentification( userName, userPass );
  
    cout << "Opening database " << baseName
         << " on " << rasmgrName << "... " << flush;
    database.open( baseName );
    cout << "OK" << endl;
  
    cout << "Starting read-only transaction ... " << flush;
    transaction.begin( r_Transaction::read_only );
    cout << "OK" << endl;
  
    cout << "Looking up collection " << collName << " ..." << flush;
    image_set = database.lookup_object( collName );
    cout << "OK" << endl;
 
    cout << "Collection contains " << image_set->cardinality() 
         << " entries" << endl;

    iter = image_set->create_iterator();
    for( iter.reset(); iter.not_done(); iter++ )
    {
      RGBPixel pixel;
      image = *iter;
      sum = 0.0;
      for ( i=image->spatial_domain()[0].low(); i<=image->spatial_domain()[0].high(); i++ )
      {
        for ( j=image->spatial_domain()[1].low(); j<=image->spatial_domain()[1].high(); j++ )
        {
          pixel = (*image)[ r_Point(i,j) ];
          sum += pixel.red;
        }
      }
      pixelcount = 
        ( image->spatial_domain()[0].high() - image->spatial_domain()[0].low() + 1 )
      * ( image->spatial_domain()[1].high() - image->spatial_domain()[1].low() + 1 );
      cout << "  avg over " << pixelcount
           << " red pixels is "
           << sum / pixelcount
           << endl << flush;   
    }

    cout << "Committing transaction ... " << flush;
    transaction.commit();
    cout << "OK" << endl;
  
    cout << "Closing database ... " << flush;
    database.close();
    cout << "OK" << endl;
  }
  catch( r_Error& errorObj )
  {
    cerr << errorObj.what() << endl;
    return -1;
  }
    
  return 0;
}

/* 
 * end of avg-cell-red.cc
 */
