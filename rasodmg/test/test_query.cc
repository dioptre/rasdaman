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
 * SOURCE: test_query.cc
 *
 * MODULE: rasodmg
 *
 * COMMENTS:
 *          None
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

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <fstream>

using namespace std;

#ifdef __VISUALC__
#define __EXECUTABLE__
#endif

#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"

#include "rasodmg/ref.hh"

#include "rasodmg/set.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/iterator.hh"
#include "rasodmg/oqlquery.hh"

#include "raslib/type.hh"

#include "raslib/minterval.hh"

#include "raslib/primitive.hh"
#include "raslib/complex.hh"
#include "raslib/structure.hh"

#include "raslib/structuretype.hh"
#include "raslib/primitivetype.hh"

#include "rasodmg/storagelayout.hh"

#include "rasodmg/alignedtiling.hh"
#include "rasodmg/dirtiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "rasodmg/interesttiling.hh"
#include "rasodmg/stattiling.hh"

#ifdef __VISUALC__
#undef __EXECUTABLE__
#endif

const int MAX_STR_LEN = 255;
const int MAX_QUERY_LEN = 10240;

bool printText = false;

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


void printScalar( const r_Scalar& scalar )
{
    switch( scalar.get_type()->type_id() )
    {
    case r_Type::BOOL:
        std::cout << ( ((r_Primitive*)&scalar)->get_boolean() ? "T" : "F" ) << std::flush;
        break;

    case r_Type::CHAR:
        std::cout << (int)((r_Primitive*)&scalar)->get_char() << std::flush;
        break;

    case r_Type::OCTET:
        std::cout << (int)((r_Primitive*)&scalar)->get_octet() << std::flush;
        break;

    case r_Type::SHORT:
        std::cout << ((r_Primitive*)&scalar)->get_short() << std::flush;
        break;

    case r_Type::USHORT:
        std::cout << ((r_Primitive*)&scalar)->get_ushort() << std::flush;
        break;

    case r_Type::LONG:
        std::cout << ((r_Primitive*)&scalar)->get_long() << std::flush;
        break;

    case r_Type::ULONG:
        std::cout << ((r_Primitive*)&scalar)->get_ulong() << std::flush;
        break;

    case r_Type::FLOAT:
        std::cout << ((r_Primitive*)&scalar)->get_float() << std::flush;
        break;

    case r_Type::DOUBLE:
        std::cout << ((r_Primitive*)&scalar)->get_double() << std::flush;
        break;

    case r_Type::COMPLEXTYPE1:
    case r_Type::COMPLEXTYPE2:
        std::cout << "(" << ((r_Complex*)&scalar)->get_re() << ", " << ((r_Complex*)&scalar)->get_im() << ")" << std::flush;
        break;

    case r_Type::STRUCTURETYPE:
    {
        r_Structure* structValue = (r_Structure*)&scalar;

        std::cout << "{ " << std::flush;

        for( int i=0; i<structValue->count_elements(); i++ )
        {
            printScalar( (*structValue)[i] );

            if( i < structValue->count_elements()-1 ) std::cout << ", " << std::flush;
        }

        std::cout << " }" << std::endl;
    }
    break;
    default:
        std::cout << "scalar type " << scalar.get_type()->type_id() <<  "  not supported!" << std::endl;
        break;
    }
}

void printUsage(char* name)
{
    std::cout << name << "v1.1 systemtest query utility " << std::endl;
    std::cout << "Description: Systemtest query utility for query execution in RasDaMan" << std::endl;
    std::cout << "Usage:   " << name << " [options]" << std::endl << std::endl;
    std::cout << "Options: -h                                      ... this help" << std::endl;
    std::cout << "         -server                 <srvname>       ... name of server.(mandatory)" << std::endl;
    std::cout << "         -port                   <nnnn>          ... port of server.(default 7001)" << std::endl;
    std::cout << "         -base                   <dbname>        ... name of database.(madatory)" << std::endl;
    std::cout << "         -file                   <filename>      ... file name used to read query from a file" << std::endl;
    std::cout << "         -user                   <usrname>       ... user name.(default rasguest)" << std::endl;
    std::cout << "         -passwd                 <usrpasswd>     ... user password.(default rasguest)" << std::endl;
    std::cout << "         -transferformat         <format>        ... transfer format.(default Array)" << std::endl;
    std::cout << "         -transferformatparams   <formatparams>  ... transfer format parameters.(default NULL)" << std::endl;
    std::cout << "         -storageformat          <format>        ... storage format.(default Array)" << std::endl;
    std::cout << "         -storageformatparams    <formatparams>  ... storage format parameters.(default NULL)" << std::endl;
    std::cout << "         -mdddomain              <domain>        ... MDD domain." << std::endl;
    std::cout << "         -tiling                 <tiling>        ... tiling strategy, only for update query.(default SizeTiling)" << std::endl;
    std::cout << "         -tilingparams           <tilingparams>  ... tiling strategy params, only for update query.(default 131072)" << std::endl;
    std::cout << "         -hex                                    ... output in hex.(default off)" << std::endl;
    std::cout << "         -nooutput                               ... no output of MDD content.(default on)" << std::endl;
    std::cout << "         -testbed                                ... turn on output for testbed.(default off)" << std::endl;
    std::cout << "         -text                                   ... print textual output.(default off)" << std::endl;
    std::cout << std::endl << std::endl;

    std::cout << "Report bugs to <support@active-knowledge.com>" << std::endl;
}

r_Tiling*
getTilingScheme(r_Tiling_Scheme& tilingS, char* tilingP)
{
    r_Tiling* retval=NULL;

    try
    {
        std::cout << "Creating tiling strategy ..." << std::flush;
        switch(tilingS)
        {
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
    catch(r_Error& err)
    {
        std::cout << "FAILED" << std::endl;
        std::cout << "Error " << err.get_errorno() << " : "  << err.what() << std::endl;
    }

    return retval;
}

int main( int argc, char** argv )
{
    int  optionValueIndex=0;

    char serverName[MAX_STR_LEN] = "";
    r_ULong  serverPort = 7001;
    char baseName[MAX_STR_LEN] = "";

    char userName[MAX_STR_LEN] = "rasguest";
    char userPasswd[MAX_STR_LEN] = "rasguest";

    char fileName[MAX_STR_LEN] = "";

    r_Data_Format transferFormat = r_Array;
    char *transferFormatParams = NULL;
    r_Data_Format storageFormat = r_Array;
    char *storageFormatParams = NULL;

    r_Tiling_Scheme tilingScheme = r_SizeTiling;
    char* tilingSchemeParams = "131072";


    int  output = 0;
    int  hexOutput = 0;
    int  testbed = 0;

    r_ULong  tileSize=0;
    r_Minterval   tileConfig;
    r_Minterval   mddDomain;

    if( checkArguments( argc, argv, "-h", optionValueIndex ) )
    {
        printUsage(argv[0]);
        return EXIT_SUCCESS;
    }

    if( checkArguments( argc, argv, "-server", optionValueIndex ) && optionValueIndex )
        strcpy( serverName, argv[optionValueIndex] );

    if( checkArguments( argc, argv, "-base", optionValueIndex ) && optionValueIndex )
        strcpy( baseName, argv[optionValueIndex] );

    if( checkArguments( argc, argv, "-file", optionValueIndex ) && optionValueIndex )
        strcpy( fileName, argv[optionValueIndex] );

    if(!strcmp(serverName, "") ||
            !strcmp(baseName,   "") ||
            !strcmp(fileName,   "") )
    {
        std::cerr << "Mandatory parameters are missing!" << std::endl;
        printUsage(argv[0]);
        return EXIT_SUCCESS;
    }

    hexOutput =  checkArguments( argc, argv, "-hex", optionValueIndex );
    printText =  checkArguments( argc, argv, "-text", optionValueIndex );
    output    = !checkArguments( argc, argv, "-nooutput", optionValueIndex );
    testbed   =  checkArguments( argc, argv, "-testbed", optionValueIndex );

    if( checkArguments( argc, argv, "-mdddomain", optionValueIndex ) && optionValueIndex )
        mddDomain = r_Minterval( argv[optionValueIndex] );

    if( checkArguments( argc, argv, "-tileconf", optionValueIndex ) && optionValueIndex )
        tileConfig = r_Minterval( argv[optionValueIndex] );

    if( checkArguments( argc, argv, "-tilesize", optionValueIndex ) && optionValueIndex )
        tileSize = strtoul( argv[optionValueIndex], (char **)NULL, 10 ) ;

    if( checkArguments( argc, argv, "-port", optionValueIndex ) && optionValueIndex )
        serverPort = strtoul( argv[optionValueIndex], (char **)NULL, 10 ) ;

    if( checkArguments( argc, argv, "-user", optionValueIndex ) && optionValueIndex )
        strcpy( userName, argv[optionValueIndex] );

    if( checkArguments( argc, argv, "-passwd", optionValueIndex ) && optionValueIndex )
        strcpy( userPasswd, argv[optionValueIndex] );

    if( checkArguments( argc, argv, "-transferformat", optionValueIndex ) && optionValueIndex )
    {
        transferFormat = get_data_format_from_name( argv[optionValueIndex] );
        if(transferFormat == r_Data_Format_NUMBER)
        {
            std::cerr << "Invalid transfer format '" << argv[optionValueIndex] << "' switched to " << r_Array  << std::endl;
            transferFormat = r_Array;
        }
    }

    if( checkArguments( argc, argv, "-transferformatparams", optionValueIndex ) && optionValueIndex )
        transferFormatParams = argv[optionValueIndex] ;

    if( checkArguments( argc, argv, "-storageformat", optionValueIndex ) && optionValueIndex )
    {
        storageFormat = get_data_format_from_name(argv[optionValueIndex] );
        if(storageFormat == r_Data_Format_NUMBER)
        {
            std::cerr << "Invalid storage format '" << argv[optionValueIndex] << "' switched to " << r_Array  << std::endl;
            storageFormat = r_Array;
        }
    }

    if( checkArguments( argc, argv, "-storageformatparams", optionValueIndex ) && optionValueIndex )
        storageFormatParams = argv[optionValueIndex] ;

    if( checkArguments( argc, argv, "-tiling", optionValueIndex ) && optionValueIndex )
    {
        tilingScheme = get_tiling_scheme_from_name( argv[optionValueIndex] );
        if(tilingScheme == r_Tiling_Scheme_NUMBER)
        {
            std::cerr << "Invalid tiling scheme '" << argv[optionValueIndex] << "' switched to " << r_SizeTiling << std::endl;
            tilingScheme = r_SizeTiling;
        }
        if(tilingScheme == r_RegularTiling)
        {
            std::cerr << "Tiling scheme '" << argv[optionValueIndex] << "' not supported, switched to " << r_SizeTiling << std::endl;
            tilingScheme = r_SizeTiling;
        }
    }

    if( checkArguments( argc, argv, "-tilingparams", optionValueIndex ) && optionValueIndex )
        tilingSchemeParams = argv[optionValueIndex] ;


#ifdef __VISUALC__
    std::ifstream fileStream( fileName, ios::nocreate );
    if( !fileStream.is_open() )
#else
    std::ifstream fileStream( fileName );
    if( !fileStream )
#endif
    {
        std::cout << "Error: File not found." << std::endl;
        return EXIT_FAILURE;
    }

    std::ostringstream queryStream;

    queryStream << "-- " << fileName << std::endl;

    char ch;
    while( fileStream.get(ch) ) queryStream.put(ch);

    queryStream << std::ends;

    std::cout << std::endl << std::endl;

    r_Database db;
    r_Transaction ta;

    db.set_servername( serverName, serverPort );
    db.set_useridentification( userName, userPasswd );

    try
    {
        std::cout << "Creating query string ..."<< std::flush;
        r_OQL_Query q1( queryStream.str().c_str() );
        std::cout << "OK" << std::endl;
        std::cout << "The query is: " << std::endl;

        std::cout << q1.get_query() << std::endl;

        std::cout << "Opening Database " << baseName << " on " << serverName << "... "<< std::flush;
        db.open( baseName );
        std::cout << "OK" << std::endl;

        if( q1.is_update_query() )
        {
            std::cout << "Starting Update Transaction ... "<< std::flush;
            ta.begin();
            std::cout << "OK" << std::endl;

            std::cout << "Setting transfer and storage formats ... " << std::flush;
            db.set_transfer_format( transferFormat, transferFormatParams );
            db.set_storage_format( storageFormat, storageFormatParams );
            std::cout << "OK" << std::endl;

            r_Marray<r_ULong>* mddConst = NULL;
            r_Tiling* tilingObj = NULL;
            r_Storage_Layout* stl = NULL;


            std::cout << "Executing the update query ..."<< std::flush;

            if( strstr( queryStream.str().c_str(), "$" ) )
            {
                r_Minterval domain;
                if( mddDomain.dimension() )
                    domain = mddDomain;
                else
                    domain = r_Minterval("[0:10,0:10]");

                // create storage layout object

                try
                {
                    tilingObj = getTilingScheme(tilingScheme, tilingSchemeParams);

                    stl = new r_Storage_Layout( tilingObj );

                    // mddConst = new( "GreyCube" ) r_Marray<r_Char>( domain, (unsigned char)0, stl );
                    mddConst = new( "ULongImage" ) r_Marray<r_ULong>( domain, (r_ULong)9, stl );
                    // mddConst = new( "FloatCube4" ) r_Marray<r_Float>( domain, 9ul, stl );

                    q1 << *mddConst;
                }
                catch(r_Error& errorObj)
                {
                    std::cout << "FAILED" << std::endl;
                    std::cout << "Error " << errorObj.get_errorno() << " : "  << errorObj.what() << std::endl;

                    if( testbed )
                    {
                        std::cout << "-- Testbed line: error_no=" << errorObj.get_errorno() << std::endl;
                        std::cout << std::endl;
                    }

                    std::cout << "Aborting Transaction ... "<< std::flush;
                    ta.abort();
                    std::cout << "OK" << std::endl;

                    std::cout << "Closing Database ... "<< std::flush;
                    db.close();

                    std::cout << "OK" << std::endl;

                    if( mddConst ) delete mddConst;
                    else if( stl ) delete stl;
                    else    if(tilingObj) delete tilingObj;

                    return EXIT_FAILURE;
                }
            }

            try
            {
                r_oql_execute( q1 );
            }
            catch( r_Error& errorObj )
            {
                std::cout << "FAILED" << std::endl;
                std::cout << "Error " << errorObj.get_errorno() << " : "  << errorObj.what() << std::endl;

                if( testbed )
                {
                    std::cout << "-- Testbed line: error_no=" << errorObj.get_errorno() << std::endl;
                    std::cout << std::endl;
                }

                std::cout << "Aborting Transaction ... " <<  std::flush;
                ta.abort();
                std::cout << "OK" << std::endl;

                std::cout << "Closing Database ... " << std::flush;
                db.close();

                std::cout << "OK" << std::endl;

                if( mddConst ) delete mddConst;

                return EXIT_FAILURE;
            }
            std::cout << "OK" << std::endl;

            if( mddConst ) delete mddConst;
        }
        else
        {
            std::cout << "Starting Read Only Transaction ... " << std::flush;
            ta.begin( r_Transaction::read_only );
            std::cout << "OK" << std::endl;

            std::cout << "Setting transfer and storage formats ... " << std::flush;
            db.set_transfer_format( transferFormat, transferFormatParams );
            db.set_storage_format( storageFormat, storageFormatParams );
            std::cout << "OK" << std::endl;

            r_Set< r_Ref_Any > result_set;

            std::cout << "Executing the retrieval query ..." << std::flush;

            try
            {
                r_oql_execute( q1, result_set );
            }
            catch( r_Error& errorObj )
            {
                std::cout << "FAILED" << std::endl;
                std::cout << "Error " << errorObj.get_errorno() << " : "  << errorObj.what() << std::endl;

                if( testbed )
                {
                    std::cout << "-- Testbed line: error_no=" << errorObj.get_errorno() << std::endl;
                    std::cout << std::endl;
                }

                std::cout << "Aborting Transaction ... " << std::flush;
                ta.abort();
                std::cout << "OK" << std::endl;

                std::cout << "Closing Database ... " << std::flush;
                db.close();
                std::cout << "OK" << std::endl;

                return EXIT_FAILURE;
            }

            std::cout << "OK" << std::endl;
            std::cout << "Collection" << std::endl;
            std::cout << "  Oid...................: " << result_set.get_oid() << std::endl;
            std::cout << "  Type Structure........: "
                      << ( result_set.get_type_structure() ? result_set.get_type_structure() : "<nn>" ) << std::endl;
            std::cout << "  Type Schema...........: " << std::flush;
            if( result_set.get_type_schema() )
                result_set.get_type_schema()->print_status( std::cout );
            else
                std::cout << "<nn>" << std::flush;
            std::cout << std::endl;
            std::cout << "  Number of entries.....: " << result_set.cardinality() << std::endl;
            std::cout << "  Element Type Schema...: " << std::flush;
            if( result_set.get_element_type_schema() )
                result_set.get_element_type_schema()->print_status( std::cout );
            else
                std::cout << "<nn>" << std::flush;
            std::cout << std::endl;

            if( testbed )
            {
                std::cout << "-- Testbed line: result_type=" << result_set.get_type_structure() << std::endl;
                std::cout << "-- Testbed line: result_elements=" << result_set.cardinality() << std::endl;
                std::cout << std::endl;
            }

            /* The following can be used, if the type is known and the element type is not atomic.

            r_Set< r_Ref< r_Point > >* set2 = (r_Set< r_Ref< r_Point > >*)&result_set;
            r_Iterator< r_Ref<r_Point> > iter2 = set2->create_iterator();
            for( iter2.reset(); iter2.not_done(); iter2++ )
              std::cout << **iter2 << std::endl;
            */

            if( output || testbed )
            {
                r_Iterator< r_Ref_Any > iter = result_set.create_iterator();

                std::cout << std::endl;

                if( testbed )
                    std::cout << "-- Testbed start block:" << std::endl;

                for ( int i=1 ; iter.not_done(); iter++, i++ )
                {
                    switch( result_set.get_element_type_schema()->type_id() )
                    {
                    case r_Type::MARRAYTYPE:
                    {
                        const char *defExt;
                        r_Data_Format mafmt = r_Ref<r_GMarray>(*iter)->get_current_format();

                        // special treatment only for DEFs
                        switch (mafmt)
                        {
                        case r_TIFF:
                            defExt = "tif";
                            break;
                        case r_JPEG:
                            defExt = "jpg";
                            break;
                        case r_HDF:
                            defExt = "hdf";
                            break;
                        case r_PNG:
                            defExt = "png";
                            break;
                        case r_BMP:
                            defExt = "bmp";
                            break;
                        case r_VFF:
                            defExt = "vff";
                            break;
                        default:
                            defExt = NULL;
                        }

                        if( defExt == NULL )
                        {
                            std::cout << "Image " << i << std::endl;
                            if (printText)
                            {
                                int numCells = r_Ref<r_GMarray>(*iter)->get_array_size();
                                const char* theStuff = r_Ref<r_GMarray>(*iter)->get_array();
                                for (int cnt = 0; cnt < numCells; cnt++)
                                    std::cout << theStuff[cnt];
                                std::cout << std::endl;
                            }
                            else
                            {
                                r_Ref<r_GMarray>(*iter)->print_status( std::cout, hexOutput );
                            }
                            std::cout << std::endl;
                        }
                        else
                        {
                            char defFileName[256];

                            sprintf( defFileName, "image%d.%s", i, defExt );
                            std::cout << "Image " << i << " written to " << defFileName << std::endl;

                            FILE *tfile = fopen( defFileName, "wb" );
                            fwrite((void*)r_Ref<r_GMarray>(*iter)->get_array(), 1,
                                   r_Ref<r_GMarray>(*iter)->get_array_size(), tfile );
                            fclose(tfile);
                        }
                    }
                    break;

                    case r_Type::POINTTYPE:
                        std::cout << "Element " << i << ": " << *(r_Ref<r_Point>(*iter)) << std::endl;
                        break;

                    case r_Type::SINTERVALTYPE:
                        std::cout << "Element " << i << ": " << *(r_Ref<r_Sinterval>(*iter)) << std::endl;
                        break;

                    case r_Type::MINTERVALTYPE:
                        std::cout << "Element " << i << ": " << *(r_Ref<r_Minterval>(*iter)) << std::endl;
                        break;

                    case r_Type::OIDTYPE:
                        std::cout << "Element " << i << ": " << *(r_Ref<r_OId>(*iter)) << std::endl;
                        break;

                    default:
                        std::cout << "Element " << i << ": " << std::flush;
                        printScalar( *(r_Ref<r_Scalar>(*iter)) );
                        std::cout << std::endl;
                        // or simply
                        // r_Ref<r_Scalar>(*iter)->print_status( std::cout );
                    }
                }

                if( testbed )
                    std::cout << "-- Testbed end block:" << std::endl;

                std::cout << std::endl;
            }

        } // retrieval query

        std::cout << "Commiting Transaction ... " << std::endl;
        ta.commit();
        std::cout << "OK" << std::endl;

        std::cout << "Closing Database ... " << std::flush;
        db.close();
        std::cout << "OK" << std::endl;
    }
    catch( r_Error& errorObj )
    {
        std::cout << "FAILED" << std::endl;
        std::cout << "Error " << errorObj.get_errorno() << " : "  << errorObj.what() << std::endl;
        ta.abort();
        db.close();
        if( testbed )
        {
            std::cout << "-- Testbed line: error_no=" << errorObj.get_errorno() << std::endl;
            std::cout << std::endl;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

