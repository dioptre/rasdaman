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
 * Copyright 2003 - 2011 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
/*************************************************************
 *
 * PURPOSE:
 * initializes a 2D rasgeo map by writing a lower left and upper right tile.
 *
 * COMMENTS:
 * - NOTE: probably does not work with RPC and HTTP server mode due to "unofficial" fct use!
 * - reinvented after rginitmap whose source wasn't found
 * - output kept as close as possible to rginitmap for eventual scripts based on it
 * - index choice not yet implemented
 *
 ************************************************************/

static const char rcsid[] = "@(#)inittools,initpyramid.cc: $Id: initpyramid.cc,v 1.10 2009/01/26 16:25:58 rasdev Exp $";

// this program's version number
#define INITGEO_VERSION "1.1"

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <errno.h>

#ifdef SOLARIS
#include <strings.h>
#endif

using std::ostringstream;

// includes needed for the rasdaman types
#include <string>

// for floor()
#include <math.h>

#include "include/globals.hh"	// DEFAULT_*

#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "rasodmg/gmarray.hh"
#include "raslib/primitive.hh"
#include "raslib/rminit.hh"
#include "raslib/storageman.hh"
#include "rasodmg/storagelayout.hh"

// for access to the unpublished interface functions introduced by W.Schatz :[
#include <akgnetwork.hh>
#include "rnprotocol/rnpclientcomm.hh"

#include "cmlparser.hh"

#include "rasgeo_signal.hh"
#include "rasgeo_error.hh"

// debug facility; relies on -DDEBUG at compile time
#define DEBUG_MAIN
#undef DEBUG_HH
#include "debug/debug.hh"

/*
 * --- these values determine the pyramid, it must be consistent wih all other rasgeo modules -------
 */

// pixel coordinates start at (0/0):
// but don't think that you can freely change this -- the pix coordinate computation in getPixelBbox() relies on it!
#define MAP_ORIGIN_X	0L
#define MAP_ORIGIN_Y	0L

// separator between base layer collection name and numbering
#define PYRAMID_SEPARATOR "_"

// separator between levels input parameter
#define LEVEL_SEPARATOR ':'

// Global variables
#define RMANVERSION '8.3'
#define COMPDATE '08 May 2012'

/*
 * --------------------------------------------------------------------------------------------------
 */

// buffer size for numbering - will never occupy more than two bytes + padding zero
#define NUMSTRINGSIZE 10

// default cell pattern for initialization
// (attention: don't mess with it, it is really just one byte, this is repeated for multibyte cell types)
#define DEFAULT_CELL_VALUE 0

// indentation for pyramid layer output
#define INDENT "   "

// command line parameters, options, defaults

#define FLAG_HELP	'h'
#define PARAM_HELP	"help"
#define HELP_HELP	"show command line switches"

#define FLAG_SERV	's'
#define PARAM_SERV	"server"
#define HELP_SERV	"<host-name> rasdaman server"
#define DEFAULT_SERV	DEFAULT_HOSTNAME

#define FLAG_PORT	'p'
#define PARAM_PORT	"port"
#define HELP_PORT	"<p> rasmgr port number"

#define FLAG_DB		'd'
#define PARAM_DB	"database"
#define HELP_DB		"<db-name> name of rasdaman database"

#define PARAM_USER	"user"
#define HELP_USER	"<user-name> name of rasdaman user"

#define PARAM_PASSWD	"passwd"
#define HELP_PASSWD	"<user-passwd> password of rasdaman user"

#define PARAM_MAPNAME	"mapname"
#define HELP_MAPNAME	"<name> name for base collection in database (pyramid layer names will be derived from this)"

#define PARAM_MAPTYPE	"maptype"
#define DEFAULT_MAPTYPE	"gray8"
#define HELP_MAPTYPE	"<maptype> map type, one of: gray8 (or GREY), rgb24 (or RGB), dem32 (or DEM), tk32 (or TK), tk8"

#define FLAG_XMIN	'x'
#define PARAM_XMIN	"startx"
#define HELP_XMIN	"<pixel-coord> bounding box minimum x pixel coordinate"

#define FLAG_XMAX	'X'
#define PARAM_XMAX	"endx"
#define HELP_XMAX	"<pixel-coord> bounding box maximum x pixel coordinate"

#define FLAG_YMIN	'y'
#define PARAM_YMIN	"starty"
#define HELP_YMIN	"<pixel-coord> bounding box minimum y pixel coordinate"

#define FLAG_YMAX	'Y'
#define PARAM_YMAX	"endy"
#define HELP_YMAX	"<pixel-coord> bounding box maximum y pixel coordinate"

#define PARAM_LEVELS	"levels"
#define HELP_LEVELS	"<list> colon separated values giving the scaling factors (float numbers) at which pyramid layers should be generated or 'auto' for dynamic adjustment; pyramid collection names will be suffixed with '_i' for i=[1..n]"

#define AUTO_LEVEL	-1
#define AUTO_LEVEL_STRING "auto"
#define AUTO_LEVEL_FACTOR 2

#define PARAM_TILEEDGE	"tileedge"
#define DEFAULT_TILEEDGE 512
#define HELP_TILEEDGE	"<p> lower left and upper right tiles generated will have size pxp pixels"

#define PARAM_HINDEX	"Hindex"
#define HELP_HINDEX	"use hierarchic index instead of RCindex"

#define PARAM_DEBUG	"debug"
#define HELP_DEBUG	"generate diagnostic output"

// admissible parameter values for the map types 
// (double values for compatibility with the variations done at AK)
const char* TYPE_DOP_GREY_1 ="gray8";
const char* TYPE_DOP_GREY_2 ="GREY";
const char* TYPE_DOP_RGB_1  ="rgb24";
const char* TYPE_DOP_RGB_2  ="RGB";
const char* TYPE_TK32_1	    ="tk32";
const char* TYPE_TK32_2	    ="TK";
const char* TYPE_TK8	    ="tk8";
const char* TYPE_DEM_1	    ="dem32";
const char* TYPE_DEM_2	    ="DEM";

// corresponding rasdaman MDD types (cf. ~rasdaman/examples/rasdl/basictypes.dl)
const char* MDD_DOP_GREY= "GreyImage";
const char* MDD_DOP_RGB	= "RGBImage";
const char* MDD_TK32	= "ULongImage";
const char* MDD_TK8	= "GreyImage";
const char* MDD_DEM	= "DoubleImage";

// corresponding rasdaman MDD set types
const char* SET_DOP_GREY= "GreySet";
const char* SET_DOP_RGB	= "RGBSet";
const char* SET_TK32	= "ULongSet";
const char* SET_TK8	= "GreySet";
const char* SET_DEM	= "DoubleSet";

// cell size for each type (in bytes)
const unsigned int SIZE_DOP_GREY= 1;
const unsigned int SIZE_DOP_RGB	= 3;
const unsigned int SIZE_TK32	= 4;
const unsigned int SIZE_TK8	= 1;
const unsigned int SIZE_DEM	= 8;


// global variables and default settings
// -------------------------------------

r_Database db;
r_Transaction ta;

bool dbIsOpen = false;
bool taIsOpen = false;

int  optionValueIndex=0; 

// bbox coordinates / pixel
long pixXmin = 0;
long pixXmax = 0;
long pixYmin = 0;
long pixYmax = 0;

unsigned long tileEdge = DEFAULT_TILEEDGE;

const char *mapName = NULL;
const char *mapType = DEFAULT_MAPTYPE;

const char *serverName = DEFAULT_SERV;
r_ULong serverPort = DEFAULT_PORT;  
const char *dbName = DEFAULT_DBNAME;
  
const char *user = DEFAULT_USER;
const char *passwd = DEFAULT_PASSWD;
  

// number of pyramid levels (excl base layer), or AUTO_LEVEL for dynamic pyramids
int levels;
vector <double> levelValues;

// use hierarchic index?
bool useHindex = false;

// debug output wanted?
bool debug = false;

void parseParams(int argc, char** argv) throw (InitError, r_Error)
{
	ENTER( "parseParams" );

	//program interface
	CommandLineParser    &cmlInter     	= CommandLineParser::getInstance();

	CommandLineParameter &cmlHelp           = cmlInter.addFlagParameter( FLAG_HELP, PARAM_HELP, HELP_HELP );

	CommandLineParameter &cmlXmin		= cmlInter.addStringParameter( FLAG_XMIN, PARAM_XMIN, HELP_XMIN );
	CommandLineParameter &cmlXmax		= cmlInter.addStringParameter( FLAG_XMAX, PARAM_XMAX, HELP_XMAX );
	CommandLineParameter &cmlYmin		= cmlInter.addStringParameter( FLAG_YMIN, PARAM_YMIN, HELP_YMIN );
	CommandLineParameter &cmlYmax		= cmlInter.addStringParameter( FLAG_YMAX, PARAM_YMAX, HELP_YMAX );

	CommandLineParameter &cmlMapname	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_MAPNAME, HELP_MAPNAME );
	CommandLineParameter &cmlMaptype	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_MAPTYPE, HELP_MAPTYPE, DEFAULT_MAPTYPE );
	CommandLineParameter &cmlLevels		= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_LEVELS, HELP_LEVELS );
	CommandLineParameter &cmlTileedge	= cmlInter.addLongParameter(CommandLineParser::noShortName, PARAM_TILEEDGE, HELP_TILEEDGE, DEFAULT_TILEEDGE );
	CommandLineParameter &cmlSrvName   	= cmlInter.addStringParameter( FLAG_SERV, PARAM_SERV, HELP_SERV, DEFAULT_SERV );
	CommandLineParameter &cmlSrvPort   	= cmlInter.addLongParameter(CommandLineParser::noShortName, PARAM_PORT, HELP_PORT,  DEFAULT_PORT );
	CommandLineParameter &cmlDbName    	= cmlInter.addStringParameter( FLAG_DB, PARAM_DB, HELP_DB, DEFAULT_DBNAME );
	CommandLineParameter &cmlUsrName   	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_USER, HELP_USER, DEFAULT_USER );
	CommandLineParameter &cmlUsrPasswd 	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_PASSWD, HELP_PASSWD, DEFAULT_PASSWD );

#if 0 // not yet implemented
	CommandLineParameter &cmlHindex		= cmlInter.addFlagParameter(CommandLineParser::noShortName, PARAM_HINDEX, HELP_HINDEX );
#endif

#ifdef DEBUG
	CommandLineParameter &cmlDebug          = cmlInter.addFlagParameter(CommandLineParser::noShortName, PARAM_DEBUG, HELP_DEBUG );
#endif

	try
	{
                cmlInter.processCommandLine(argc, argv);

                if (cmlInter.isPresent( FLAG_HELP ) || argc == 1)
                {
                        cout << "options:" << endl;
                        cmlInter.printHelp();
                        exit(2);                        // 2 is bash ret code after help; FIXME: exit no good style!!
                }

#ifdef DEBUG
		// evaluate optional parameter debug --------------------------------------
		debug = cmlInter.isPresent( PARAM_DEBUG );
		SET_OUTPUT( debug );
#endif

		// check mandatory parameters ====================================================

		// evaluate mandatory parameter mapname  --------------------------------------
		if (cmlInter.isPresent( PARAM_MAPNAME ))
			mapName = cmlInter.getValueAsString( PARAM_MAPNAME );
		else
			throw InitError( NOCOLLNAME );

		// evaluate mandatory parameter xmin --------------------------------------
		if ( cmlInter.isPresent( PARAM_XMIN ) )
		{
			const char *nptr = cmlInter.getValueAsString( PARAM_XMIN );
			if (nptr == NULL)
				throw InitError( INVALIDFLOAT );
			char *endptr = NULL;		// used by strtod(), receives indicator on success
			pixXmin = ceil(strtod( nptr, &endptr ));
			if (endptr == nptr || errno == ERANGE)	// strtod() conversion error
			{
				throw InitError( INVALIDFLOAT );
			}
		}
		else
			throw InitError( NOXMIN );

		// evaluate mandatory parameter xmax  --------------------------------------
		if ( cmlInter.isPresent( PARAM_XMAX ) )
		{
			const char *nptr = cmlInter.getValueAsString( PARAM_XMAX );
			if (nptr == NULL)
				throw InitError( INVALIDFLOAT );
			char *endptr = NULL;		// used by strtod(), receives indicator on success
			pixXmax = ceil(strtod( nptr, &endptr ));
			if (endptr == nptr || errno == ERANGE)	// strtod() conversion error
			{
				throw InitError( INVALIDFLOAT );
			}
		}
		else
			throw InitError( NOXMAX );

		// evaluate mandatory parameter ymin  --------------------------------------
		if ( cmlInter.isPresent( PARAM_YMIN ) )
		{
			const char *nptr = cmlInter.getValueAsString( PARAM_YMIN );
			if (nptr == NULL)
				throw InitError( INVALIDFLOAT );
			char *endptr = NULL;		// used by strtod(), receives indicator on success
			pixYmin = ceil(strtod( nptr, &endptr ));
			if (endptr == nptr || errno == ERANGE)	// strtod() conversion error
			{
				throw InitError( INVALIDFLOAT );
			}
		}
		else
			throw InitError( NOYMIN );

		// evaluate optional parameter levels --------------------------------------
		
		if ( cmlInter.isPresent( PARAM_LEVELS ) )
		{
			const char* levelsString = cmlInter.getValueAsString( PARAM_LEVELS );
					
			if (strncasecmp( AUTO_LEVEL_STRING, levelsString, sizeof(AUTO_LEVEL_STRING) ) == 0)
				levels = AUTO_LEVEL;
			else
			{		
				const char *i;
				char *end;
				i = levelsString;
				do 
				{
					double val = strtod(i, &end);					
					if (*end==LEVEL_SEPARATOR)
						i = end+1;
					else 
						if (*end != '\0')
							throw InitError( INVALIDLEVEL );					
					if (val < 1)
					{
						cout << "Error: level must be >= 1, but is: " << val << std::endl;
						throw InitError( INVALIDLEVEL );
					}
					levelValues.push_back(val);
				} while (*end!='\0');

				levels = levelValues.size();
				if (levels < 0)
					throw InitError( INVALIDLEVEL );
			}
		} else
		{
			levels = 1;
			levelValues.push_back(1);
		}
			

		// evaluate mandatory parameter ymax --------------------------------------
		if ( cmlInter.isPresent( PARAM_YMAX ) )
		{
			const char *nptr = cmlInter.getValueAsString( PARAM_YMAX );
			if (nptr == NULL)
				throw InitError( INVALIDFLOAT );
			char *endptr = NULL;		// used by strtod(), receives indicator on success
			pixYmax = ceil(strtod( nptr, &endptr ));
			if (endptr == nptr || errno == ERANGE)	// strtod() conversion error
			{
				throw InitError( INVALIDFLOAT );
			}
		}
		else
			throw InitError( NOYMAX );

		// check optional parameters ====================================================

		// evaluate optional parameter maptype  --------------------------------------
		if (cmlInter.isPresent( PARAM_MAPTYPE ))
			mapType = cmlInter.getValueAsString( PARAM_MAPTYPE );

		// evaluate optional parameter tileedge --------------------------------------
		if ( cmlInter.isPresent( PARAM_TILEEDGE ) )
		{
			tileEdge = cmlInter.getValueAsLong( PARAM_TILEEDGE );
			if (tileEdge <= 0)
				throw InitError( INVALIDTILEEDGE );
		}

#if 0 // not yet implemented
		// evaluate optional parameter Hindex --------------------------------------
		useHindex = cmlInter.isPresent( PARAM_HINDEX );
#endif

		// evaluate optional parameter server --------------------------------------
		if (cmlInter.isPresent( PARAM_SERV ))
			serverName = cmlInter.getValueAsString( PARAM_SERV );

		// evaluate optional parameter port --------------------------------------
		if (cmlInter.isPresent( PARAM_PORT ))
			serverPort = cmlInter.getValueAsLong( PARAM_PORT );

		// evaluate optional parameter database --------------------------------------
		if (cmlInter.isPresent( PARAM_DB ))
			dbName = cmlInter.getValueAsString( PARAM_DB );

		// evaluate optional parameter user --------------------------------------
		if (cmlInter.isPresent( PARAM_USER ))
			user = cmlInter.getValueAsString( PARAM_USER );

		// evaluate optional parameter passwd --------------------------------------
		if (cmlInter.isPresent( PARAM_PASSWD ))
			passwd = cmlInter.getValueAsString( PARAM_PASSWD );
	}
	catch(CmlException& err)
	{
		cout << err.what() << endl;
		throw InitError( ERRORPARSINGCOMMANDLINE );
	}


	// plausi check of parameters
if (pixXmin > pixXmax || pixYmin > pixYmax)
    {
		throw InitError( INVALIDBBOX );
    }

	TALK( "server=" << serverName << ", port=" << serverPort << ", database=" << dbName << ", user=" << user << ", passwd=" << passwd );
	TALK( "mapname=" << mapName << ", levels=" << levels << ", tileEdge=" << tileEdge << ", useHindex=" << useHindex );
	TALK( "xmin=" << pixXmin << ", xmax=" << pixXmax << ", ymin=" << pixYmin << ", ymax=" << pixYmax );

	LEAVE( "parseParams");
	return;
} // parseParams()


void
openDatabase() throw (r_Error)
{
        ENTER( "openDatabase" );

	if (! dbIsOpen)
	{
		cout << "opening database " <<  dbName  << " at " << serverName << ":" << serverPort << "..." << flush;
        	db.set_servername(serverName, serverPort);
        	db.set_useridentification( user, passwd);
        	TALK( "database was closed, opening database=" <<  dbName  << ", server=" << serverName << ", port=" << serverPort << ", user=" <<  user << ", passwd=" << passwd << "..." );
        	db.open( dbName );
		TALK( "done" );
		dbIsOpen = true;
		cout << "ok" << endl << flush;
	}

        LEAVE( "openDatabase" );
}

void
closeDatabase() throw (r_Error)
{
        ENTER( "closeDatabase" );

	if (dbIsOpen)
	{
        	TALK( "database was open, closing it" );
		db.close();
		dbIsOpen = false;
	}

        LEAVE( "closeDatabase" );
        return;
}

void
openTransaction(bool readwrite) throw (r_Error)
{
        ENTER( "openTransaction, readwrite=" << (readwrite ? "rw" : "ro" ) );

	if (! taIsOpen)
	{
		if (readwrite)
		{
			TALK( "transaction was closed, opening rw..." );
			ta.begin(r_Transaction::read_write);
			TALK( "done" );
		}
		else
		{
			TALK( "transaction was closed, opening ro..." );
			ta.begin(r_Transaction::read_only);
			TALK( "done" );
		}

		TALK( "setting transfer format to r_Array" );
		db.set_transfer_format( r_Array, "" );

		TALK( "setting storage format to r_ZLib" );
		db.set_storage_format( r_ZLib, "" );

		taIsOpen = true;
	}

        LEAVE( "openTransaction" );
}

void 
closeTransaction( bool doCommit ) throw (r_Error)
{
        ENTER( "closeTransaction, doCommit=" << doCommit );

	if (taIsOpen)
	{
		if (doCommit)
		{
			TALK( "transaction was open, committing it..." );
			ta.commit();
			TALK( "done" );
		}
		else
		{
			TALK( "transaction was open, aborting it..." );
			ta.abort();
			TALK( "done" );
		}
		taIsOpen = false;
	}

        LEAVE( "closeTransaction" );
	return;
}


// determine database mdd and set type from input parameter
void
getType( const char* mapType, const char** mddTypeP, const char** setTypeP, size_t* cellSizeP ) throw (InitError)
{
	ENTER( "getTypeName, mapType=" << mapType );

	if (strcasecmp( mapType, TYPE_DOP_GREY_1 ) == 0 || strcasecmp( mapType, TYPE_DOP_GREY_2 ) == 0)
	{
		*mddTypeP = MDD_DOP_GREY;
		*setTypeP = SET_DOP_GREY;
		*cellSizeP = SIZE_DOP_GREY;
	}
	else if (strcasecmp( mapType, TYPE_DOP_RGB_1 ) == 0 || strcasecmp( mapType, TYPE_DOP_RGB_2 ) == 0)
	{
		*mddTypeP = MDD_DOP_RGB;
		*setTypeP = SET_DOP_RGB;
		*cellSizeP = SIZE_DOP_RGB;
	}
	else if (strcasecmp( mapType, TYPE_TK32_1 ) == 0 || strcasecmp( mapType, TYPE_TK32_2 ) == 0)
	{
		*mddTypeP = MDD_TK32;
		*setTypeP = SET_TK32;
		*cellSizeP = SIZE_TK32;
	}
	else if (strcasecmp( mapType, TYPE_TK8 ) == 0)
	{
		*mddTypeP = MDD_TK8;
		*setTypeP = SET_TK8;
		*cellSizeP = SIZE_TK8;
	}
	else if (strcasecmp( mapType, TYPE_DEM_1 ) == 0 || strcasecmp( mapType, TYPE_DEM_2 ) == 0)
	{
		*mddTypeP = MDD_DEM;
		*setTypeP = SET_DEM;
		*cellSizeP = SIZE_DEM;
	}
	else
		throw InitError( ILLEGALMAPTYPE );

	LEAVE( "getTypeName" );
}

// extends current dimension to be a multiple of tileEdge
void extendToFit(long &dim) 
{
	dim ++;
	dim += (tileEdge - dim % tileEdge) % tileEdge;
	dim --;
}

// bring everything to origin
void normalize (long& pixXminP, long& pixXmaxP, long& pixYminP, long& pixYmaxP ) 
{
	pixXmax -= pixXmin;
	pixYmax -= pixYmin;
	pixXmin = pixYmin = 0;	
	extendToFit(pixXmax);
	extendToFit(pixYmax);
}


void createPyramids( const char* mddTypeName, const char* setTypeName, size_t cellSize, long pixXmin, long pixXmax, long pixYmin, long pixYmax ) throw (r_Error, InitError)
{
	ENTER( "createPyramids, mddTypeName=" << mddTypeName << ", setTypeName=" << setTypeName );
	long currentXmin;
	long currentYmin;
	long currentXmax;
	long currentYmax;
	currentXmin = pixXmin;
	currentXmax = pixXmax;
	currentYmin = pixYmin;
	currentYmax = pixYmax;

	// tile domain, equals lower left domain
	r_Minterval tileDomain = r_Minterval( 2 )
		<< r_Sinterval( (r_Range) currentXmin, (r_Range) (currentXmin+tileEdge-1) )
		<< r_Sinterval( (r_Range) currentYmin, (r_Range) (currentYmin+tileEdge-1) );

	// query object preparations, including MDD object array
	string queryBuffer;
	unsigned long bufferSize = tileEdge * tileEdge * cellSize;
	char *arrayBuffer = new char[ bufferSize ];
	if (arrayBuffer == NULL)
		throw InitError( ALLOCFAIL );
	memset( arrayBuffer, DEFAULT_CELL_VALUE, bufferSize );

	// walk through the pyramid levels
	for ( int currentLevel = 0;
		// termination criterion:
		// - if auto: look at tile size underflow
	      (  (levels==AUTO_LEVEL && (currentXmax-currentXmin>tileEdge || (currentYmax-currentYmin)>tileEdge) )
		// - if fixed level: count levels
	      || (levels!=AUTO_LEVEL && currentLevel < levels ) );
              currentLevel++ )
	{
		
		if (levels != AUTO_LEVEL) 
		{
			currentXmax = (long)ceil(pixXmax / levelValues[currentLevel]);
			currentYmax = (long)ceil(pixYmax / levelValues[currentLevel]);
			extendToFit(currentXmax);
			extendToFit(currentYmax);		
		}

		// prepare collection name for this layer
		string currentMapName;
		if (currentLevel == 0)			// base layer, no suffix
			currentMapName = string(mapName);
		else
		{
			char numberString[NUMSTRINGSIZE];
			memset( numberString, NUMSTRINGSIZE, '\0' );
			sprintf( numberString, "%d", currentLevel );
			currentMapName = string(mapName) + PYRAMID_SEPARATOR + numberString;
		}
		TALK( "generating layer " << currentLevel << ", name is " << currentMapName );

		// domain describing complete area of MDD
		r_Minterval definitionDomain  = r_Minterval( 2 )
			<< r_Sinterval( (r_Range) currentXmin, (r_Range) currentXmax )
			<< r_Sinterval( (r_Range) currentYmin, (r_Range) currentYmax );
		const char* definitionDomainString = definitionDomain.get_string_representation();
		// domain describing tile size, also lower left tile
		r_Minterval tileDomain  = r_Minterval( 2 )
			<< r_Sinterval( (r_Range) MAP_ORIGIN_X, (r_Range) (tileEdge-1) )
			<< r_Sinterval( (r_Range) MAP_ORIGIN_Y, (r_Range) (tileEdge-1) );
		const char* tileDomainString = tileDomain.get_string_representation();

		// report current layer
		// don't change the "Creating collection: " string, init_wms.sh greps for it
		cout << INDENT << "Creating collection: " << currentMapName 
			<< " with type=" << setTypeName
			<< " and raster domain=" << definitionDomainString << endl;
#ifndef TEST
		// generate pyramid collection
		queryBuffer = string( "CREATE COLLECTION " ) + currentMapName + " " + setTypeName;

		r_OQL_Query createQuery( queryBuffer.c_str() );
		TALK( "creating collection: " << createQuery.get_query() );
		r_oql_execute( createQuery );

		// now use "unofficial" interface to create MDD tile
		// - get internal communication object
		ClientComm *cc = db.getComm();
		// - create MDD with proper index
		r_OId newOId = ((RnpClientComm*)cc)->createMDD( currentMapName.c_str(), mddTypeName, definitionDomainString, tileDomainString, !useHindex );
		// - perform tiling of new object
		// ((RnpClientComm*)cc)->extendMDD( newOId, tileDomainString, definitionDomainString );

		closeTransaction(true);
		closeDatabase();
		openDatabase();
		openTransaction(true);

		cout << INDENT << "Mdd with oid= " << newOId.get_string_representation() << ", tiledomain=" << tileDomain << " and " << (useHindex ? "Hindex" : "RCIndex" ) << "..." << flush;
#else
		cout << INDENT << "Mdd with tiledomain=" << tileDomain << " and " << (useHindex ? "Hindex" : "RCIndex" ) << "..." << flush;
#endif // TEST

		cout << "ok" << endl;

		if (levels == AUTO_LEVEL)
		{
			currentXmax = (long)ceil( (double) currentXmax / AUTO_LEVEL_FACTOR);
			currentYmax = (long)ceil( (double) currentYmax / AUTO_LEVEL_FACTOR);
			extendToFit(currentXmax);
			extendToFit(currentYmax);		
		}

		// prepare next iteration: compute next smaller domain
	}

	LEAVE( "createPyramids" );
	return;
}

int main(int argc, char** argv)
{
	SET_OUTPUT( false );            // inhibit unconditional debug output, await cmd line evaluation

	int retval = EXIT_SUCCESS;

	cout << argv[0] << ": rasdaman map initialization tool v1.0. rasdaman v" << RMANVERSION/1000 << " -- generated on " << COMPDATE << ".\n";

	try
	{
		parseParams( argc, argv );             	 // evaluate cmd line, allows to override defaults

#ifndef TEST
		// open database and transaction
		openDatabase();
		cout << "opening read-write transaction..." << flush;
		openTransaction( true );
		cout << "ok" << endl << flush;
#endif // TEST

		// determine MDD and collection characteristics
		const char *mddTypeName = "XXX"; // NULL;		// MDD name corresponding to map type indicated
		const char *setTypeName = "GreySet"; // NULL;		// set name corresponding to map type indicated
		size_t cellSize = 0;		// MDD cell size in bytes
		getType( mapType, &mddTypeName, &setTypeName, &cellSize );
		TALK( "mddTypeName=" << mddTypeName << ", setTypeName=" << setTypeName << ", cellSize=" << cellSize );

		cout << "Creating map pyramid " << mapName << " of type " << mddTypeName;
		if (levels==AUTO_LEVEL)
			cout << " with auto scale levels" << endl;
		else
			cout << " with " << levels << " scale levels" << endl;
		printf( "Pixel domain: [%ld:%ld,%ld:%ld] \n", pixXmin, pixXmax, pixYmin, pixYmax );

		normalize(pixXmin, pixXmax, pixYmin, pixYmax);
		// now do it: create pyramids, fill in objects and tiles
		createPyramids( mddTypeName, setTypeName, cellSize, pixXmin, pixXmax, pixYmin, pixYmax );
    

#ifndef TEST
		cout << "committing and closing..." << flush;
		closeTransaction( true );
		closeDatabase();
		cout << "ok" << endl;
#endif // TEST

		retval = EXIT_SUCCESS;
	}
	catch (InitError& e)
	{
		cout << argv[0] << ": " << e.what() << endl;
		retval = EXIT_FAILURE;
	}
	catch (const r_Error& e)
	{
		cout << argv[0] << ": rasdaman error " << e.get_errorno() << ": " << e.what() << endl;
		retval = EXIT_FAILURE;
	}
	catch (...)
	{
		cout << argv[0] << ": panic: unexpected internal exception." << endl;
		retval = EXIT_FAILURE;
	}

	if (retval != EXIT_SUCCESS)
	{       // abort transaction and close database, don't care for any further exceptions
#ifndef TEST
		closeTransaction( false );
		closeDatabase();
#endif // TEST
	}

	cout<< argv[0] << " done." << endl;
	return retval;
} // main()

