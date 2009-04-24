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
 * SOURCE: test_insert.cc
 *
 * MODULE: rasodmg
 *
 * COMMENTS:
 * 		None
*/

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include <iostream>
#include <string.h>

#ifdef __VISUALC__
  #define __EXECUTABLE__
#endif

#include "include/basictypes.hh"

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
    cout << "Usage:   test_insert server_name base_name collection_name [options]" << endl << endl;
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
  r_Ref< r_Marray<r_ULong> >                   image1, image2, image3, image4, 
                                               image5, image6, transImage;
  r_Minterval                                  domain, domain2;
  
  domain  = r_Minterval(2) << r_Sinterval( 0, 10 ) << r_Sinterval( 0, 10 );
  
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

    try{
      image_set = db.lookup_object( collName );
    }
    catch( r_Error& /*obj*/ )
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

    cout << "Create image1 with constant 0 ... " <<  flush;  
    image1 = new( &db, "ULongImage" ) ULongImage( domain, (r_ULong)0 );
    cout << "OK" << endl;
  
    cout << "Create image2 with copy constructor from image1 ... " <<  flush;  
    image2 = new( &db, "ULongImage" ) ULongImage( (const ULongImage&) *image1 );
    cout << "OK" << endl;

    cout << "Create transient image with constant 1 ... " <<  flush;  
    transImage = new ULongImage( domain, 1ul );
    cout << "OK" << endl;

    cout << "Create image3 with copy constructor from transient image ... " <<  flush;  
    image3 = new( &db, "ULongImage" ) ULongImage( (const ULongImage&) *transImage );
    cout << "OK" << endl;

    transImage.destroy();

    // image2->initialize_oid( db.get_new_oid(1) );

    cout << "Create image4 with cross foot ... " <<  flush;  
    image4 = new( &db, "ULongImage" ) r_Marray<r_ULong>( domain, &initWithCrossfoot );
    cout << "OK" << endl;
  
    cout << "Create image5 with counter ... " <<  flush;  
    image5 = new( &db, "ULongImage" ) r_Marray<r_ULong>( domain, &initWithCounter );
    cout << "OK" << endl;
  
    cout << "Create image6 with coordinates ... " <<  flush;  
    image6 = new( &db, "ULongImage" ) r_Marray<r_ULong>( domain, &initWithCoordinates );
    cout << "OK" << endl;

    cout << "Insert images into the set " << collName << " ... " << flush;    

    // insert the images
    image_set->insert_element( image1 );
    image_set->insert_element( image2 );
    image_set->insert_element( image3 );
    image_set->insert_element( image4 );
    image_set->insert_element( image5 );
    image_set->insert_element( image6 );
    
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
    ta.abort();
    db.close();
    cerr << "Error " << errorObj.get_errorno() << " : " << errorObj.what() << endl;
    return -1;
  }
    
  return 0;
}
