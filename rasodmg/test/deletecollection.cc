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
 * SOURCE: deletecollection.cc
 *
 * MODULE: rasodmg
 *
 * PURPOSE:
 *
 *
 ************************************************************/

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include <iostream>
#include <iomanip.h>
#include <string.h>
#include <fstream.h>

#include <math.h>

#ifdef __VISUALC__
  #define __EXECUTABLE__
#endif

#include "include/basictypes.hh"
#include "raslib/type.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/alignedtiling.hh"  

#ifdef __VISUALC__
  #undef  __EXECUTABLE__
#endif

static int INIT = 0;

static long minNo = 0;
static long maxNo = 0;
static ifstream fileStream;

r_ULong initWithCounter( const r_Point& /*pt*/ )
{
  return INIT++;
}


r_Char initWithCounterChar( const r_Point& /*pt*/ )
{
  return (r_Char) INIT++;
}


r_UShort initWithCounterUShort( const r_Point& /*pt*/ )
{
  return (r_UShort) INIT++;
}



r_ULong initWithCrossfoot( const r_Point& pt )
{
  r_ULong value=0;
  
  for( r_Dimension i=0; i< pt.dimension(); i++ )
    value += (r_ULong)pt[i];
  
  return value;
}


r_ULong initWithCoordinates( const r_Point& pt )
{
   r_ULong value=0;
   r_ULong factor=1;
  
  for( int i=pt.dimension()-1; i >= 0; i-- )
  {   
    value  += (r_ULong)(factor * pt[i]);
    factor *= 100;
  }
    
  return value;
}



r_Char initWithCurve( const r_Point& pt )
{
  r_ULong value=0;
  
  for( r_Dimension i=0; i < pt.dimension(); i++ )
    value  += (r_Char)( 255* sin( .25 * (float)pt[i] ) );
    
  return (r_Char)value;
}



r_ULong initWithColorCube( const r_Point& pt )
{
   r_ULong value=0;
  
  if( pt.dimension() == 3 )
    value = ((unsigned long)(pt[0]) << 16) + ((unsigned long)(pt[1]) << 8) + (unsigned long)(pt[2]);
    
  return value;
}


r_Char initWithRandomChar( const r_Point& /*pt*/ )
{
  // rand creates numbers between 0 and 32767
  unsigned long number = rand();
  double        factor = (maxNo - minNo)/32767.0;

  return (r_Char)(minNo + (number * factor));
}

r_ULong initWithRandomULong( const r_Point& /*pt*/ )
{
  // rand creates numbers between 0 and 32767
  unsigned long number = rand();
  double        factor = (maxNo - minNo)/32767.0;

  return (r_ULong)(minNo + (number * factor));
}


r_Char initFromFile( const r_Point& /*pt*/ )
{
  char ch;

  if( fileStream.get(ch) )
    return ch;
  else
    return 0;
}

/*
r_Ref< r_Marray<r_ULong> > readFile( r_Database& db )
{
  r_Ref< r_Marray<r_ULong> > image;
  r_Minterval            domain(3);
  long                   dx, dy, dz, depth, cx, cy, cz;
  int                    i;
  
  ifstream fileStream( "hw8.full.vol" );
  if( !fileStream )
  {
    cout << "Error: File not found." << endl;
    exit(-1);
  }
 
  // read the first 16 bytes
  char charDummy;
  for( i=0; i<16; i++ )
    fileStream >> charDummy;
  
  dx    = 256;
  dy    = 256;
  dz    = 20;
  depth = 8;

  domain << r_Sinterval( 0l, (long)(dx-1) ) << r_Sinterval( 0l, (long)(dy-1) ) << r_Sinterval( 0l, (long)(dz-1) );

  cout << domain << "... "; cout.flush();
  
  image = new( &db ) r_Marray<r_ULong>( domain );  

  unsigned long* dataPtr = (unsigned long*)image->get_array();
  
  r_Point pt(3);
  
  //  for( i=0; i<dx*dy*(221-dz); i++ )
  //    fileStream >> charDummy;    
  
  for( cz = 0; cz < dz; cz++ )
  {
    pt[2] = cz;
    for( cy = 0; cy < dy; cy++ )
    {
      pt[1] = cy;
      for( cx = 0; cx < dx; cx++ )
      {
        unsigned char ch;
        fileStream.get(ch);

	pt[0] = cx;	
        dataPtr[domain.cell_offset(pt)] = ((unsigned long)ch << 16) +
                                          ((unsigned long)ch << 8) +
                                          ((unsigned long)ch);
      }
    }
  }
  
  fileStream.close();
  
  return image;
}
*/	       
		       
		       
void printColl( r_Ref< r_Set< r_Ref< r_GMarray > > >& image_set, int output, int hexOutput )
{
  cout << "Collection" << endl;
  cout << "  Oid...................: " << image_set->get_oid() << endl;
  cout << "  Type Name.............: " << image_set->get_object_name() << endl;
  cout << "  Type Structure........: " 
       << ( image_set->get_type_structure() ? image_set->get_type_structure() : "<nn>" ) << endl;
  cout << "  Type Schema...........: " << flush;
  if( image_set->get_type_schema() )
    image_set->get_type_schema()->print_status( cout );
  else
    cout << "<nn>" << flush;
  cout << endl;
  cout << "  Number of entries.....: " << image_set->cardinality() << endl;
  cout << "  Element Type Schema...: " << flush;
  if( image_set->get_element_type_schema() )
    image_set->get_element_type_schema()->print_status( cout );
  else
    cout << "<nn>" << flush;
  cout << endl;

  r_Iterator< r_Ref< r_GMarray > > iter = image_set->create_iterator();
  
  cout << endl;
  for ( int i=1 ; iter.not_done(); iter++, i++ )
  {
    // do not dereference the object
    cout << "Image " << i << " oid: " << (*iter).get_oid() << endl;

    if( output )
    {
      (*iter)->print_status( cout, hexOutput );   
      cout << endl;
    }
  }
  cout << endl;
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
  int  optionValueIndex=0; 
  int  testbed = 0;
  unsigned long tileSize=0;
  r_Minterval   tileConfig;
  int  compressed=0;

  char serverName[255];
  char baseName[255];
  char collName[255];
  char setTypeName[255] = "";
  char mddTypeName[255] = "";
  char fileName[255]="";
   
  if( argc < 4 || checkArguments( argc, argv, "-h", optionValueIndex ) )
  {
    cout << "Usage:   test_insert3 server_name base_name collection_name [options]" << endl << endl;
    cout << "Options: -h                  ... this help" << endl;
    cout << "         -settype <typename> ... type name used for creation of a new set." << endl;
    cout << "         -mddtype <typename> ... type name used for creation of a new mdd object." << endl;
    cout << "         -testbed            ... turn on output for testbed" << endl;
    cout << "         -tileconf           ... tile configuration" << endl;
    cout << "         -tilesize           ... tile size" << endl;
    cout << "         -file <filename>    ... file name used to read data from a file" << endl;
    cout << "         -compressed         ... inserts MDD in compressed format" << endl;
    cout << endl;
    return 0;
  }

  strcpy( serverName, argv[1] );
  strcpy( baseName, argv[2] );
  strcpy( collName, argv[3] );

  if( checkArguments( argc, argv, "-settype", optionValueIndex ) && optionValueIndex )
    strcpy( setTypeName, argv[optionValueIndex] );

  if( checkArguments( argc, argv, "-mddtype", optionValueIndex ) && optionValueIndex )
    strcpy( mddTypeName, argv[optionValueIndex] );

  testbed   =  checkArguments( argc, argv, "-testbed", optionValueIndex );

  if( checkArguments( argc, argv, "-tileconf", optionValueIndex ) && optionValueIndex )
    tileConfig = r_Minterval( argv[optionValueIndex] );

  if( checkArguments( argc, argv, "-tilesize", optionValueIndex ) && optionValueIndex )
    tileSize = strtoul( argv[optionValueIndex], (char **)NULL, 10 ) ;

  if( checkArguments( argc, argv, "-file", optionValueIndex ) && optionValueIndex )
    strcpy( fileName, argv[optionValueIndex] );

  compressed =  checkArguments( argc, argv, "-compressed", optionValueIndex );

  cout << endl << endl;
  cout << "Insertion of one MDD into the database" << endl;
  cout << "======================================" << endl << endl;
    
  r_Database db;
  r_Transaction ta;
  r_Ref< r_Set< r_Ref< r_GMarray > > >  image_set;
  r_Ref< r_GMarray >                    image;
  r_Minterval                           domain;
  r_Dimension                           dim;
  r_Range                               low, high;
  unsigned long                         initMode, initValue;

  RGBPixel                              rgbValue = { 1, 2, 3 };

  db.set_servername( serverName );
  
  try
  {
    /*
    cout << "MDD Initialization:  0 - Marray<r_ULong> with constant" << endl;
    cout << "                     1 - Marray<r_ULong> with counter" << endl;
    cout << "                     2 - Marray<r_ULong> with coordinates" << endl;
    cout << "                     3 - Marray<r_ULong> with crossfoot" << endl;
    cout << "                     4 - Marray<r_ULong> with color cube" << endl << endl;
    
    cout << "                     5 - Marray<r_Char> with constant" << endl;
    cout << "                     6 - insert RGBImage with constant" << endl;
    cout << "                     7 - Marray<r_Char>  with 255*( sin(.25*x1) + ... + sin(.25*xn) )" << endl;
    
    cout << "                     8 - Marray<r_ULong> read a file (hw8.full.vol)" << endl;
    cout << "                     9 - Create just an empty collection of type Marray<r_ULong>" << endl;
    cout << "                    10 - Delete collection of type GMarray" << endl;
    cout << "                    11 - Delete an object of collection" << endl;

    cout << "                    13 - Marray<r_Char> with counter" << endl;
    cout << "                    14 - Marray<r_Char> with random numbers" << endl;
    cout << "                    15 - Marray<r_UShort> with counter" << endl;
    cout << "                    16 - Marray<r_ULong>  with random numbers" << endl;
    cout << "                    17 - Marray<r_Boolean>" << endl;
    cout << "                    18 - Marray<r_Char> from file" << endl;

    cin  >> initMode;
    cout << endl;
    */
    initMode = 10;

    if( initMode == 18 )
    {
      fileStream.open( fileName );
      if( !fileStream )
      {
        cout << "Error: File " << fileName << " not found." << endl;
        exit(-1);
      }
    }

    if( initMode == 0 || initMode == 5 || initMode == 17 )
    {
      cout << "Constant value : ";
      cin  >> initValue;
      cout << endl;
    }
    if( initMode <= 7 || initMode >= 13 )
    {
      cout << "Number of dimensions : ";
      cin  >> dim;
      cout << endl;
    
      domain = r_Minterval(dim);
    
      for( r_Dimension i = 1; i<=dim ; i++ )
      {
        cout << "Dimension " << setw(2) << i << " lower bound ";
        cin  >> low;
        cout << "             upper bound ";
        cin  >> high;
      
        domain << r_Sinterval( low, high );
      }
    
      cout << endl;
    }
     
    if( initMode == 14 || initMode == 16 )
    {
      cout << "Minimum number : ";
      cin  >> minNo;
      cout << endl;

      cout << "Maximum number : ";
      cin  >> maxNo;
      cout << endl;
    }

    cout << "Opening Database " << baseName << " on " << serverName << "... "; cout.flush();
    db.open( baseName );
    cout << "OK" << endl;
  
    cout << "Starting Transaction ... " << flush;
    ta.begin();
    cout << "OK" << endl;

    cout << "Opening the set ... " << flush;

    if( initMode == 10 || initMode == 11 )
    {
      //
      // get the set
      //

      try{
        image_set = db.lookup_object( collName );
      }
      catch( r_Error &obj )
      {
        cout << "FAILED" << endl;
        cout << obj.what() << endl;
        return -1;
      }

      cout << "OK" << endl;

      if( initMode == 10 )
      {
        // delete the set

        if( !image_set.is_null() )
	  image_set.delete_object();
        // delete image_set; (not implemented yet)
      }
      else
      {
        int imageNo=0;

        printColl( image_set, 0, 0 );

        cout << "Please enter the image number to delete: ";
        cin  >> imageNo;
        cout << endl;

        r_Iterator< r_Ref< r_GMarray > > iter = image_set->create_iterator();
  
        for ( int i=1; iter.not_done() && i<imageNo; iter++, i++ );
 
        if( imageNo && iter.not_done() )
	{
          image_set->remove_element( *iter );
	  cout << "MDD removed." << endl;
	}
        else
          cout << "Number not valid." << endl << endl;
      }
    }
    else
    {    
      //
      // get set
      //

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

        if( initMode == 5 || initMode == 7 || initMode == 13 || initMode == 14 || initMode == 18 )
	{
          if( !strlen( setTypeName ) )
            strcpy( setTypeName, "GreySet" );

          image_set = new( &db, setTypeName  ) r_Set< r_Ref< r_Marray<r_Char> > >;
        }
        else if( initMode == 6 )
	{
          if( !strlen( setTypeName ) )
            strcpy( setTypeName, "RGBSet" );

          image_set = new( &db, setTypeName  ) r_Set< r_Ref< r_Marray<RGBPixel> > >;
        }
        else if( initMode == 15 )
	{
          if( !strlen( setTypeName ) )
            strcpy( setTypeName, "UShortSet" );

          image_set = new( &db, setTypeName  ) r_Set< r_Ref< r_Marray<r_UShort> > >;
        }
        else if( initMode == 17 )
        {
          if( !strlen( setTypeName ) )
            strcpy( setTypeName, "BoolSet" );
     
          image_set = new( &db, setTypeName ) r_Set< r_Ref< r_Marray<r_Boolean> > >;
        }
        else
	{       
          if( !strlen( setTypeName ) )
            strcpy( setTypeName, "ULongSet" );

          image_set = new( &db, setTypeName  ) r_Set< r_Ref< r_Marray<r_ULong> > >;
        }

        // create a name for the persistent set in order to be able to look it up again
        db.set_object_name( *image_set, collName );
      }

      cout << " with type name " << setTypeName << " ... OK" << endl;

      cout << "OId of the set is " << image_set->get_oid() << " ... " << endl;

      if( initMode <= 9 || initMode >= 13 )
      {
        cout << "Creating an image ..." << flush;

        // create storage layout object
        r_Storage_Layout* stl = 0;
        if( tileSize )
          stl = new r_Storage_Layout( new r_Aligned_Tiling( tileConfig, tileSize ) );

        if( compressed )
	{
          if( !stl )
	    stl = new r_Storage_Layout();

          db.set_storage_format( r_ZLib );
        }

        // create the image
        switch( initMode )
        {
          case 0:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "ULongImage" );

            image = new( &db, mddTypeName  ) r_Marray<r_ULong>( domain, (r_ULong)initValue, stl );
            break;
          case 1:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "ULongImage" );

            image = new( &db, mddTypeName  ) r_Marray<r_ULong>( domain, &initWithCounter, stl );
            break;
          case 2:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "ULongImage" );

            image = new( &db, mddTypeName  ) r_Marray<r_ULong>( domain, &initWithCoordinates, stl );
            break;
          case 3:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "ULongImage" );

            image = new( &db, mddTypeName  ) r_Marray<r_ULong>( domain, &initWithCrossfoot, stl );
            break;
          case 4:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "ULongImage" );

            image = new( &db, mddTypeName  ) r_Marray<r_ULong>( domain, &initWithColorCube, stl );
            break;
          case 5:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "GreyImage" );

            image = new( &db, mddTypeName  ) r_Marray<r_Char>( domain, (r_Char)initValue, stl );
            break;
          case 6:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "RGBImage" );

            image = new( &db, mddTypeName  )  RGBImage( domain, rgbValue, stl );
            break;
          case 7:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "GreyImage" );

            image = new( &db, mddTypeName  ) r_Marray<r_Char>( domain, &initWithCurve, stl );
            break;
          case 13:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "GreyImage" );

            image = new( &db, mddTypeName  ) r_Marray<r_Char>( domain, &initWithCounterChar, stl );
            break;
          case 14:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "GreyImage" );

            image = new( &db, mddTypeName  ) r_Marray<r_Char>( domain, &initWithRandomChar, stl );
            break;
          case 15:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "UShortImage" );

            image = new( &db, mddTypeName  ) r_Marray<r_UShort>( domain, &initWithCounterUShort, stl );
            break;
          case 16:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "ULongImage" );

            image = new( &db, mddTypeName  ) r_Marray<r_ULong>( domain, &initWithRandomULong, stl );
            break;
          case 17:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "BoolImage" );

            image = new( &db, mddTypeName ) r_Marray<r_Boolean>( domain, (r_Boolean)initValue, stl );
            break;
          case 18:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "GreyImage" );

            image = new( &db, mddTypeName ) r_Marray<r_Char>( domain, &initFromFile, stl );
            break;
          default:
            if( !strlen( mddTypeName ) )
              strcpy( mddTypeName, "ULongImage" );

            image = new( &db, mddTypeName  ) r_Marray<r_ULong>( domain, (r_ULong)0, stl );	 
        }
    
        cout << "OId of the new image is " << image->get_oid() << endl;

        cout << "Inserting one image with domain " << domain << " into collection " << collName << "..." << flush;
        // put in into the persistent list
        image_set->insert_element( image );      
        cout << "OK" << endl << endl;

        if( testbed )
        {
          cout << endl << "Testbed output:" << endl;
          cout << "-- Testbed: set_oid=" << image_set->get_oid() << endl;
          cout << "-- Testbed: image_oid=" << image->get_oid() << endl;
          cout << endl;
        }
  
      }
    }
       
    try
    {
      cout << "Committing Transaction ... " << flush;
      ta.commit();
      cout << "OK" << endl;
    }
    catch( r_Error &obj )
    {
      cerr << obj.what() << endl;
      return -1;
    }
   
    cout << "Closing Database ... " << flush;
    db.close();
    cout << "OK" << endl; 

    if( initMode == 18 )
      fileStream.close();
  }
  catch( r_Error& errorObj )
  {
     cerr << errorObj.what() << endl;
     return -1;
  }
    
  return 0;
}



