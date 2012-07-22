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
 * SOURCE: test_lookup.cc
 *
 * MODULE: rasodmg
 *
 * COMMENTS:
 *      None
 */

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include <iostream>
#include <string.h>

#include "raslib/type.hh"

#ifdef __VISUALC__
#define __EXECUTABLE__
#include "include/basictypes.hh"
#undef  __EXECUTABLE__
#else
#include "include/basictypes.hh"
#endif

const int MAX_STR_LEN = 255;
enum prg_mode
{
    USE_INVALID = 0,
    USE_COLLNAME,
    USE_OIDSTR
};

using namespace std;

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



void printColl( r_Ref< r_Set< r_Ref< r_GMarray > > >& image_set, int output, int rgbOutput, int hexOutput )
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

    if( output )
    {
        r_Iterator< r_Ref< r_GMarray > > iter = image_set->create_iterator();

        std::cout << std::endl;

        for ( int i=1 ; iter.not_done(); iter++, i++ )
        {
            std::cout << "Image " << i << std::endl;

            if( !rgbOutput )
                (*iter)->print_status( std::cout, hexOutput );
            else
            {
                RGBImage image( **iter );
                r_Point  pt(0,0);

                std::cout << "[0,0] = {" << (int)image[pt].red << ","
                          << (int)image[pt].green << ","
                          << (int)image[pt].blue << "}" << std::endl;
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}

void
printUsage(char* name)
{
    std::cout << name << "v1.1 systemtest lookup utility " << std::endl << std::endl;
    std::cout << "Description: Systemtest lookup utility for retrival of set / marray from RasDaMan " << std::endl << std::endl;
    std::cout << "Usage:   " << name << " [options]" << std::endl << std::endl;
    std::cout << "Options: -h                                      ... this help" << std::endl;
    std::cout << "         -server                 <srvname>       ... name of server.(mandatory)" << std::endl;
    std::cout << "         -port                   <nnnn>          ... port of server.(default 7001)" << std::endl;
    std::cout << "         -base                   <dbname>        ... name of database.(madatory)" << std::endl;
    std::cout << "         -setname                <setname>       ... name of collection for retrival by name.(madatory)" << std::endl;
    std::cout << "         -oid                    <oidset>        ... oid of marray or set as string for retrival by oid.(mandatory)" << std::endl;
    std::cout << "         -user                   <usrname>       ... user name.(default rasguest)" << std::endl;
    std::cout << "         -passwd                 <usrpasswd>     ... user password.(default rasguest)" << std::endl;
    std::cout << "         -transferformat         <format>        ... transfer format.(default Array)" << std::endl;
    std::cout << "         -transferformatparams   <formatparams>  ... transfer format parameters.(default NULL)" << std::endl;
    std::cout << "         -storageformat          <format>        ... storage format.(default Array)" << std::endl;
    std::cout << "         -storageformatparams    <formatparams>  ... storage format parameters.(default NULL)" << std::endl;
    std::cout << "         -ia                                     ... interactive control for multiple collection retrieval.(default off)" << std::endl;
    std::cout << "         -hex                                    ... output in hex.(default off)" << std::endl;
    std::cout << "         -rgb                                    ... output is rgb struct.(default off)" << std::endl;
    std::cout << "         -nooutput                               ... no output of MDD content.(default on)" << std::endl;
    std::cout << "         -testbed                                ... turn on output for testbed.(default off)" << std::endl;

    std::cout << std::endl  << std::endl;

    std::cout << "Report bugs to <support@active­knowledge.com>" << std::endl;
}

int main( int argc, char** argv )
{
    int  interactive=0;
    int  output=0;
    int  optionValueIndex=0;
    char key = 'a';
    int  hexOutput = 0;
    int  rgbOutput = 0;
    prg_mode mode=USE_INVALID;
    int  testbed = 0;

    r_Data_Format transferFormat=r_Array;
    char *transferFormatParams = NULL;
    r_Data_Format storageFormat=r_Array;
    char *storageFormatParams = NULL;

    char *serverName="";
    r_ULong serverPort = 7001;
    char *baseName="";

    char *collName="";
    char *oidString="";

    char *userName="rasguest";
    char *userPasswd="rasguest";

#ifdef TEST_LOOKUP
    r_GMarray*       transImage = 0;
    r_Ref<r_GMarray> transImageRef = 0;
#endif

    if( checkArguments( argc, argv, "-h", optionValueIndex ) )
    {
        printUsage(argv[0]);
        return EXIT_SUCCESS;
    }

    if( checkArguments( argc, argv, "-server", optionValueIndex ) && optionValueIndex )
        serverName=argv[optionValueIndex];

    if( checkArguments( argc, argv, "-base", optionValueIndex ) && optionValueIndex )
        baseName=argv[optionValueIndex];

    if( checkArguments( argc, argv, "-setname", optionValueIndex ) && optionValueIndex )
    {
        collName=argv[optionValueIndex];
        mode=USE_COLLNAME;
    }

    if( checkArguments( argc, argv, "-oid", optionValueIndex ) && optionValueIndex )
    {
        oidString=argv[optionValueIndex];
        if(mode != USE_INVALID)
        {
            std::cerr << "Use -setname or -oid, not both in the same time" << std::endl;
            printUsage(argv[0]);
            return EXIT_FAILURE;
        }
        mode=USE_OIDSTR;
    }

    if(!strcmp(serverName, "") ||
            !strcmp(baseName,   "") ||
            (mode == USE_INVALID) )
    {
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

    if( checkArguments( argc, argv, "-transferformat", optionValueIndex ) && optionValueIndex )
    {
        transferFormat = get_data_format_from_name( argv[optionValueIndex] );
        if(transferFormat == r_Data_Format_NUMBER)
        {
            std::cerr << "Invalid transfer format '" << argv[optionValueIndex] << "' switched to " << r_Array << std::endl;
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
            std::cerr << "Invalid storage format '" << argv[optionValueIndex] << "' switched to " << r_Array << std::endl;
            storageFormat = r_Array;
        }
    }

    if( checkArguments( argc, argv, "-storageformatparams", optionValueIndex ) && optionValueIndex )
        storageFormatParams = argv[optionValueIndex] ;

    interactive =  checkArguments( argc, argv, "-ia", optionValueIndex );
    output      = !checkArguments( argc, argv, "-nooutput", optionValueIndex );
    hexOutput   =  checkArguments( argc, argv, "-hex", optionValueIndex );
    rgbOutput   =  checkArguments( argc, argv, "-rgb", optionValueIndex );
    testbed     =  checkArguments( argc, argv, "-testbed", optionValueIndex );

    std::cout << std::endl << std::endl;
    std::cout << "ODMG conformant lookup of Marrays" << std::endl;
    std::cout << "=================================" << std::endl << std::endl;

    r_Database db;
    r_Transaction ta;



    try
    {
        std::cout << "Opening database '" << baseName << "' on  server '" << serverName << "':" << serverPort
                  << " using user '" << userName << "':'" << userPasswd << "' ... " << std::flush;
        db.set_servername( serverName, serverPort );
        db.set_useridentification( userName, userPasswd );
        db.open( baseName );
        std::cout << "OK" << std::endl;

        while( key != 'q' )
        {
            std::cout << "Starting Read Only Transaction ... " << std::flush;
            ta.begin( r_Transaction::read_only );
            std::cout << "OK" << std::endl;

            if( mode == USE_COLLNAME )
            {
                r_Ref< r_Set< r_Ref< r_GMarray > > >  image_set;

                std::cout << "Looking up collection by name " << collName << " ..." << std::flush;
                image_set = db.lookup_object( collName );
                std::cout << "OK" << std::endl;

                printColl( image_set, output, rgbOutput, hexOutput );
            }
            else
            {
                r_Ref<r_Object> persObject;
                r_OId           oid( oidString );

                std::cout << "Looking up object by oid " << oid << " ..." << std::flush;
                persObject = db.lookup_object( oid );
                std::cout << "OK" << std::endl;

                std::cout << "Object type: " << persObject->get_type_structure() << std::endl;

                if( strstr( persObject->get_type_structure(), "set" ) )
                {
                    r_Ref< r_Set< r_Ref< r_GMarray > > >  image_set = (r_Ref_Any) persObject;

                    printColl( image_set, output, rgbOutput, hexOutput );
                }
                else
                {
                    r_Ref<r_GMarray> marray = (r_Ref_Any) persObject;

                    std::cout << "MDD object with domain " << marray->spatial_domain()
                              << " and type " << marray->get_type_name() << std::endl;

                    if( output )
                        marray->print_status( std::cout );

#ifdef TEST_LOOKUP
                    // make a copy for use after ta
                    transImage    = new r_GMarray( (const r_GMarray&)*marray );
                    transImageRef = new("type") r_GMarray( (const r_GMarray&)*marray );
#endif
                }

                std::cout << std::endl;
            }

            std::cout << "Commiting Transaction ... " << std::flush;
            ta.commit();
            std::cout << "OK" << std::endl;

            if( interactive )
            {
                std::cout << "Command (q=quit, a=again): ";
                cin >> key;
            }
            else
                key = 'q';

        }

        std::cout << "Closing Database ... " << std::flush;
        db.close();
        std::cout << "OK" << std::endl;

    }
    catch( r_Error& errorObj )
    {
        ta.abort();
        db.close();
        std::cout << "Error " << errorObj.get_errorno() << " : " << errorObj.what() << std::endl;
        return EXIT_FAILURE;
    }

#ifdef TEST_LOOKUP
    if( transImage )
    {
        std::cout << "Transient MDD object with domain " << transImage->spatial_domain()
                  << " and type " << transImage->get_type_name() << std::endl;

        transImage->print_status( std::cout, hexOutput );

        delete transImage;
    }

    if( transImageRef != 0 )
    {
        std::cout << "Transient MDD object with domain " << transImageRef->spatial_domain()
                  << " and type " << transImageRef->get_type_name() << std::endl;

        transImageRef->print_status( std::cout, hexOutput );

        transImageRef.destroy();
    }
#endif

    return EXIT_SUCCESS;
}

