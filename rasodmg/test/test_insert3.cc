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
 * SOURCE: test_insert3.cc
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

#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <strstream>
#endif

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include <math.h>

#ifdef __VISUALC__
  #define __EXECUTABLE__
#endif
#include "include/basictypes.hh"
#include "raslib/type.hh"

#include "rasodmg/storagelayout.hh"

#include "rasodmg/alignedtiling.hh"
#include "rasodmg/dirtiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "rasodmg/interesttiling.hh"
#include "rasodmg/stattiling.hh"


const int MAX_STR_LEN = 255;

static int INIT = 0;
static r_Long  minNo = 0;
static r_Long  maxNo = 0;
static ifstream fileStream;

using namespace std;

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
    value = ((r_ULong)(pt[0]) << 16) + ((r_ULong)(pt[1]) << 8) + (r_ULong)(pt[2]);
    
  return value;
}


r_Char initWithRandomChar( const r_Point& /*pt*/ )
{
  // rand creates numbers between 0 and 32767
  r_ULong  	number = rand();
  double        factor = (maxNo - minNo)/32767.0;

  return (r_Char)(minNo + (number * factor));
}

r_ULong initWithRandomULong( const r_Point& /*pt*/ )
{
  // rand creates numbers between 0 and 32767
  r_ULong	number = rand();
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

#ifdef QUERY_TEST
r_Ref< r_Marray<r_ULong> > readFile( r_Database& db )
{
  r_Ref< r_Marray<r_ULong> > image;
  r_Minterval            domain(3);
  long                   dx, dy, dz, depth, cx, cy, cz;
  int                    i;
  
  ifstream fileStream( "hw8.full.vol" );
  if( !fileStream )
  {
    std::cout << "Error: File not found." << std::endl;
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

  domain << r_Sinterval( (r_Long)0, (r_Long)(dx-1) ) << r_Sinterval( (r_Long)0, (r_Long)(dy-1) ) << r_Sinterval( (r_Long)0, (r_Long)(dz-1) );

  std::cout << domain << "... "<< std::flush;
  
  image = new( &db ) r_Marray<r_ULong>( domain );  

  r_ULong* dataPtr = (r_ULong*)image->get_array();
  
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
        dataPtr[domain.cell_offset(pt)] = ((r_ULong)ch << 16) +
                                          ((r_ULong)ch << 8) +
                                          ((r_ULong)ch);
      }
    }
  }
  
  fileStream.close();
  
  return image;
}
#endif
		       
		       
void printColl( r_Ref< r_Set< r_Ref< r_GMarray > > >& image_set, int output, int hexOutput )
{
  std::cout << "Collection" << std::endl;
  std::cout << "  Oid...................: " << image_set->get_oid() << std::endl;
  std::cout << "  Type Name.............: " << image_set->get_object_name() << std::endl;
  std::cout << "  Type Structure........: " 
       << ( image_set->get_type_structure() ? image_set->get_type_structure() : "<nn>" ) << std::endl;
  std::cout << "  Type Schema...........: " << std::flush;
  if( image_set->get_type_schema() )
    image_set->get_type_schema()->print_status( std::cout );
  else
    std::cout << "<nn>" << std::flush;
  std::cout << std::endl;
  std::cout << "  Number of entries.....: " << image_set->cardinality() << std::endl;
  std::cout << "  Element Type Schema...: " << std::flush;
  if( image_set->get_element_type_schema() )
    image_set->get_element_type_schema()->print_status( std::cout );
  else
    std::cout << "<nn>" << std::flush;
  std::cout << std::endl;

  r_Iterator< r_Ref< r_GMarray > > iter = image_set->create_iterator();
  
  std::cout << std::endl;
  for ( int i=1 ; iter.not_done(); iter++, i++ )
  {
    // do not dereference the object
    std::cout << "Image " << i << " oid: " << (*iter).get_oid() << std::endl;

    if( output )
    {
      (*iter)->print_status( std::cout, hexOutput );   
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}



int checkArguments( int argc, char** argv, const char* searchText, int& optionValueIndex )
{
  int found = 0;
  int i=1;

  while( !found && i<argc )
    found = !strcmp( searchText, argv[i++] );   

  if( found && i<argc && (argv[i][0] != '-') )
    optionValueIndex = i;
  else
    optionValueIndex = 0;
  
  return found;
}

void printUsage(char* name){
    std::cout << name << " v1.1 systemtest insert utility" << std::endl;
    std::cout << "Description: Systemtest insert utility for creation and update of sets in RasDaMan" << std::endl;
    std::cout << "Usage:   " << name << " [options]" << std::endl << std::endl;
    std::cout << "Options: -h                                      ... this help" << std::endl;
    std::cout << "         -server                 <srvname>       ... name of server.(mandatory)" << std::endl;
    std::cout << "         -port                   <nnnn>          ... port of server.(default 7001)" << std::endl;    
    std::cout << "         -base                   <dbname>        ... name of database.(madatory)" << std::endl;
    std::cout << "         -setname                <setname>       ... name of collection.(madatory)" << std::endl;    
    std::cout << "         -settype                <typename>      ... type name used for creation of a new set." << std::endl;
    std::cout << "         -mddtype                <typename>      ... type name used for creation of a new mdd object." << std::endl;
    std::cout << "         -user                   <usrname>       ... user name.(default rasguest)" << std::endl;
    std::cout << "         -passwd                 <usrpasswd>     ... user password.(default rasquest)" << std::endl;
    std::cout << "         -transferformat         <format>        ... transfer format.(default Array)" << std::endl;
    std::cout << "         -transferformatparams   <formatparams>  ... transfer format parameters.(default NULL)" << std::endl;    
    std::cout << "         -storageformat          <format>        ... storage format.(default Array)" << std::endl;
    std::cout << "         -storageformatparams    <formatparams>  ... storage format parameters.(default NULL)" << std::endl;    
    std::cout << "         -tiling                 <tiling>        ... tiling strategy.(default SizeTiling)" << std::endl;
    std::cout << "         -tilingparams           <tilingparams>  ... tiling strategy params.(default 131072)" << std::endl;    
    std::cout << "         -file                   <filename>      ... file name used to read data from a file" << std::endl;
    std::cout << "         -testbed                                ... turn on output for testbed.default(default off)" << std::endl;    
    std::cout << std::endl << std::endl;
    
    std::cout << "Report bugs to <support@active­knowledge.com>" << std::endl;
}

r_Tiling*
getTilingScheme(r_Tiling_Scheme& tilingS, char* tilingP){
  r_Tiling* retval=NULL;
  
  try {
   std::cout << " Creating tiling strategy ..." << std::flush;
   switch(tilingS) {
     case r_NoTiling:
            retval = new r_No_Tiling(tilingP);
     break;
     case r_AlignedTiling:
     	    retval = new r_Aligned_Tiling(tilingP);
     break;
     case r_InterestTiling:
            retval = new r_Interest_Tiling(tilingP);
     break;
     case r_DirectionalTiling:
            retval = new r_Dir_Tiling(tilingP);
     break;
     case r_StatisticalTiling:
            retval = new r_Stat_Tiling(tilingP);
     break;
     default:
            retval = new r_Size_Tiling(tilingP);
     break;
     }
    std::cout << "OK" << std::flush;     
  }
  catch(r_Error& err){
    std::cout << "FAILED" << std::endl;
    std::cout << "Error " << err.get_errorno() << " : "  << err.what() << std::endl;  
  }
  
  return retval;
}

int main( int argc, char** argv ) {   
  int  optionValueIndex=0; 
  int  testbed = 0;

  r_Tiling_Scheme tilingScheme = r_SizeTiling;
  char* tilingSchemeParams = "131072";
  
  r_Data_Format transferFormat=r_Array;
  char *transferFormatParams = NULL;  
  r_Data_Format storageFormat=r_Array;  
  char *storageFormatParams = NULL;
  
  char serverName[MAX_STR_LEN]="";
  r_ULong serverPort=7001;    
  char baseName[MAX_STR_LEN]="";
  
  char userName[MAX_STR_LEN]="rasguest";
  char userPasswd[MAX_STR_LEN]="rasguest";  
  
  char collName[MAX_STR_LEN]="";
  char setTypeName[MAX_STR_LEN] = "";
  char mddTypeName[MAX_STR_LEN] = "";
  char fileName[MAX_STR_LEN]="";
  
  
  if( checkArguments( argc, argv, "-h", optionValueIndex ) ) {
    printUsage(argv[0]);
    return EXIT_SUCCESS;
  }

  if( checkArguments( argc, argv, "-server", optionValueIndex ) && optionValueIndex )
    strcpy( serverName, argv[optionValueIndex] );      
   
  if( checkArguments( argc, argv, "-base", optionValueIndex ) && optionValueIndex )
    strcpy( baseName, argv[optionValueIndex] );   
    
  if( checkArguments( argc, argv, "-setname", optionValueIndex ) && optionValueIndex )
    strcpy( collName, argv[optionValueIndex] );    
  
  if(!strcmp(serverName, "") || 
     !strcmp(baseName,   "") ||
     !strcmp(collName,   "") ) {
    std::cerr << "Mandatory parameters are missing!" << std::endl;     
    printUsage(argv[0]);       
    return EXIT_SUCCESS;
  }

  if( checkArguments( argc, argv, "-port", optionValueIndex ) && optionValueIndex )
    serverPort = strtoul( argv[optionValueIndex], (char **)NULL, 10 ) ;
    
  if( checkArguments( argc, argv, "-user", optionValueIndex ) && optionValueIndex )
    strcpy( userName, argv[optionValueIndex] );

  if( checkArguments( argc, argv, "-passwd", optionValueIndex ) && optionValueIndex )
    strcpy( userPasswd, argv[optionValueIndex] );    
    
  if( checkArguments( argc, argv, "-transferformat", optionValueIndex ) && optionValueIndex ) {
    transferFormat = get_data_format_from_name( argv[optionValueIndex] );
    if(transferFormat == r_Data_Format_NUMBER) {
       std::cerr << "Invalid transfer format '" << argv[optionValueIndex] << "' switched to " << r_Array << std::endl;
       transferFormat = r_Array;
    }
  }

  if( checkArguments( argc, argv, "-transferformatparams", optionValueIndex ) && optionValueIndex )
    transferFormatParams = argv[optionValueIndex] ;
    
  if( checkArguments( argc, argv, "-storageformat", optionValueIndex ) && optionValueIndex ) {
     storageFormat = get_data_format_from_name(argv[optionValueIndex] );
    if(storageFormat == r_Data_Format_NUMBER) {
       std::cerr << "Invalid storage format '" << argv[optionValueIndex] << "' switched to " << r_Array << std::endl;
       storageFormat = r_Array;
    }     
  }

  if( checkArguments( argc, argv, "-storageformatparams", optionValueIndex ) && optionValueIndex )
    storageFormatParams = argv[optionValueIndex] ;

  if( checkArguments( argc, argv, "-settype", optionValueIndex ) && optionValueIndex )
    strcpy( setTypeName, argv[optionValueIndex] );

  if( checkArguments( argc, argv, "-mddtype", optionValueIndex ) && optionValueIndex )
    strcpy( mddTypeName, argv[optionValueIndex] );

  testbed   =  checkArguments( argc, argv, "-testbed", optionValueIndex );

  if( checkArguments( argc, argv, "-tiling", optionValueIndex ) && optionValueIndex ) {
     tilingScheme = get_tiling_scheme_from_name( argv[optionValueIndex] );
    if(tilingScheme == r_Tiling_Scheme_NUMBER) {
       std::cerr << "Invalid tiling scheme '" << argv[optionValueIndex] << "' switched to " << r_SizeTiling << std::endl;
       tilingScheme = r_SizeTiling;
    }
    if(tilingScheme == r_RegularTiling) {
       std::cerr << "Tiling scheme '" << argv[optionValueIndex] << "' not supported, switched to " << r_SizeTiling << std::endl;
       tilingScheme = r_SizeTiling;       
    }     
  }
    
  if( checkArguments( argc, argv, "-tilingparams", optionValueIndex ) && optionValueIndex )
    tilingSchemeParams = argv[optionValueIndex] ;    

  if( checkArguments( argc, argv, "-file", optionValueIndex ) && optionValueIndex )
    strcpy( fileName, argv[optionValueIndex] );

  std::cout << std::endl << std::endl;
  std::cout << "Insertion of one MDD into the database" << std::endl;
  std::cout << "======================================" << std::endl << std::endl;
    
  r_Database db;
  r_Transaction ta;
  r_Ref< r_Set< r_Ref< r_GMarray > > >  image_set;
  r_Ref< r_GMarray >                    image;
  r_Minterval                           domain;
  r_Dimension                           dim;
  r_Range                               low, high;
  r_ULong                         initMode, initValue;

  RGBPixel                              rgbValue = { 1, 2, 3 };

  db.set_servername( serverName, serverPort );
  db.set_useridentification( userName, userPasswd );  

  
  try
  {
    std::cout << "MDD Initialization:  0 - Marray<r_ULong> with constant" << std::endl;
    std::cout << "                     1 - Marray<r_ULong> with counter" << std::endl;
    std::cout << "                     2 - Marray<r_ULong> with coordinates" << std::endl;
    std::cout << "                     3 - Marray<r_ULong> with crossfoot" << std::endl;
    std::cout << "                     4 - Marray<r_ULong> with color cube" << std::endl << std::endl;
    
    std::cout << "                     5 - Marray<r_Char> with constant" << std::endl;
    std::cout << "                     6 - insert RGBImage with constant" << std::endl;
    std::cout << "                     7 - Marray<r_Char>  with 255*( sin(.25*x1) + ... + sin(.25*xn) )" << std::endl;
    
    std::cout << "                     8 - Marray<r_ULong> read a file (hw8.full.vol)" << std::endl;
    std::cout << "                     9 - Create just an empty collection of type Marray<r_ULong>" << std::endl;
    std::cout << "                    10 - Delete collection of type GMarray" << std::endl;
    std::cout << "                    11 - Delete an object of collection" << std::endl;

    std::cout << "                    13 - Marray<r_Char> with counter" << std::endl;
    std::cout << "                    14 - Marray<r_Char> with random numbers" << std::endl;
    std::cout << "                    15 - Marray<r_UShort> with counter" << std::endl;
    std::cout << "                    16 - Marray<r_ULong>  with random numbers" << std::endl;
    std::cout << "                    17 - Marray<r_Boolean>" << std::endl;
    std::cout << "                    18 - Marray<r_Char> from file" << std::endl;

    cin  >> initMode;
    std::cout << "Selected mode : " << initMode << std::endl;

    if( initMode == 18 )
    {
      fileStream.open( fileName );
      if( !fileStream )
      {
        std::cout << "Error: File " << fileName << " not found." << std::endl;
        exit(-1);
      }
    }

    if( initMode == 0 || initMode == 5 || initMode == 17 )
    {
      std::cout << "Constant value : ";
      cin  >> initValue;
      std::cout << initValue << std::endl;
    }
    if( initMode <= 7 || initMode >= 13 )
    {
      std::cout << "Number of dimensions : ";
      cin  >> dim;
      std::cout << dim << std::endl;
    
      domain = r_Minterval(dim);
    
      for( r_Dimension i = 1; i<=dim ; i++ )
      {
        std::cout << setw(2) << i << " Dimension( lower bound : ";
        cin  >> low;
        std::cout << low << " upper bound : ";
        cin  >> high;
        std::cout << high << " ); " ;
      
        domain << r_Sinterval( low, high );
      }
    
      std::cout << std::endl;
    }
     
    if( initMode == 14 || initMode == 16 )
    {
      std::cout << "Minimum number : ";
      cin  >> minNo;
      std::cout << minNo << std::endl;

      std::cout << "Maximum number : ";
      cin  >> maxNo;
      std::cout << maxNo << std::endl;
    }
    
    try{
      std::cout << "Opening Database " << baseName << " on " << serverName << "... "<< std::flush;
      db.open( baseName );
      std::cout << "OK" << std::endl;
      
      std::cout << "Starting Transaction ... " << std::flush;
      ta.begin();      
      std::cout << "OK" << std::endl;      
      
      std::cout << "Setting transfer and storage formats ... " << std::flush;      
      db.set_transfer_format( transferFormat, transferFormatParams );    
      db.set_storage_format( storageFormat, storageFormatParams );    
      std::cout << "OK" << std::endl;
    }
    catch(r_Error& obj){
        std::cout << "FAILED" << std::endl;
        std::cout << "Error " << obj.get_errorno() << " : " << obj.what() << std::endl;
        ta.abort();
        db.close();
        return EXIT_FAILURE;    
    }
  
    std::cout << "Opening the set " << collName << "... " << std::flush;

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
        std::cout << "FAILED" << std::endl;
        std::cout << "Error " << obj.get_errorno() << " : " << obj.what() << std::endl;        
        return EXIT_FAILURE;
      }

      std::cout << "OK" << std::endl;

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

        std::cout << "Please enter the image number to delete: ";
        cin  >> imageNo;
        std::cout << imageNo << std::endl;

        r_Iterator< r_Ref< r_GMarray > > iter = image_set->create_iterator();
  
        for ( int i=1; iter.not_done() && i<imageNo; iter++, i++ );
 
        if( imageNo && iter.not_done() )
	{
          image_set->remove_element( *iter );
	  std::cout << "MDD " << imageNo << " removed." << std::endl;
	}
        else
          std::cout << "Number not valid." << std::endl << std::endl;
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
        std::cout << "FAILED" << std::endl;
        //std::cout << "Error " << obj.get_errorno() << " : " << obj.what() << std::endl;        

        //
        // set doesn't exist -> create the set
        //

        std::cout << "Create the set " << collName << " ... " << std::flush;

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

      std::cout << " with type name " << setTypeName << " ... OK" << std::endl;

      std::cout << "OId of the set is " << image_set->get_oid() << " ... " << std::endl;

      if( initMode <= 9 || initMode >= 13 )
      {
        std::cout << "Creating the marray ..." << std::flush;

        // create storage layout object
        r_Tiling* tilingObj = getTilingScheme(tilingScheme, tilingSchemeParams);

        if(tilingObj == NULL) return EXIT_FAILURE;        
        
        r_Storage_Layout* stl = new r_Storage_Layout( tilingObj );

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
    
    	std::cout << " Marray type " << mddTypeName << " ... OK" << std::endl;
        std::cout << "OId of the new image is " << image->get_oid() << std::endl;

        std::cout << "Inserting one image with domain " << domain << " into collection " << collName << "..." << std::flush;
        // put in into the persistent list
        image_set->insert_element( image );      
        std::cout << "OK" << std::endl << std::endl;

        if( testbed )
        {
          std::cout << std::endl << "Testbed output:" << std::endl;
          std::cout << "-- Testbed: set_oid=" << image_set->get_oid() << std::endl;
          std::cout << "-- Testbed: image_oid=" << image->get_oid() << std::endl;
          std::cout << std::endl;
        }
  
      }
    }
       
    try
    {
      std::cout << "Committing Transaction ... " << std::flush;
      ta.commit();
      std::cout << "OK" << std::endl;
    }
    catch( r_Error &obj )
    {
      std::cerr << "Error " << obj.get_errorno() << " : " << obj.what() << std::endl;
      return EXIT_FAILURE;
    }
   
    std::cout << "Closing Database ... " << std::flush;
    db.close();
    std::cout << "OK" << std::endl; 

    if( initMode == 18 )
      fileStream.close();
  }
  catch( r_Error& obj )
  {
     std::cerr << "Error " << obj.get_errorno() << " : " << obj.what() << std::endl;  
     return EXIT_FAILURE;
  }
    
  return EXIT_SUCCESS;
}



