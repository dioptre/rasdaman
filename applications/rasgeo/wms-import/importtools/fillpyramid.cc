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
 * COMMENTS:
 * - built from old rasodmg/test/system_update
 *
 * BUGS:
 * - --align without --tiledupdate goes wild
 * - drop mddtype, deduce from collection type
 * - new option --transpcolor 0xXXXXXX to set transparency
 ************************************************************/

static const char rcsid[] = "@(#)rasodmg/test,ImportOrtho: $Id: fillpyramid.cc,v 1.9 2007/05/09 12:39:14 rasdev Exp $";

#ifndef RMANVERSION
#error "Please specify RMANVERSION variable!"
#endif

#ifndef COMPDATE
#error "Please specify the COMPDATE variable!"
/*
COMPDATE=`date +"%d.%m.%Y %H:%M:%S"`

and -DCOMPDATE="\"$(COMPDATE)\"" when compiling
*/
#endif


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include <unistd.h>
#include <math.h>
#include <string>


// this must use the rasdaman internal module
// as there is no official type retrieval interface yet,
// access to private function Database::getDComm() is required
#include "raslib/odmgtypes.hh"

// includes needed for the rasdaman types
#include "raslib/rminit.hh"
#include "raslib/mitera.hh"
#include "raslib/type.hh"
#include "raslib/marraytype.hh"
#include "raslib/minterval.hh"
#include "raslib/primitivetype.hh"
#include "raslib/storageman.hh"
#include "raslib/mddtypes.hh"

#include "rasodmg/marray.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/set.hh"
#include "rasodmg/database.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/partinsert.hh"
#include "rasodmg/gmarray.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/dirtiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "rasodmg/interesttiling.hh"
#include "rasodmg/alignedtiling.hh"
#include "rasodmg/stattiling.hh"
#include "rasodmg/oqlquery.hh"
#include "rasodmg/polygon.hh"
#include "rasodmg/polycutout.hh"

#include "cmlparser.hh"
#include "hexcodec.hh"
#include "rasgeo_globals.hh"
#include "rasgeo_util.hh"
#include "import_error.hh"

// debug facility; relies on -DDEBUG at compile time
// tell debug that here is the place for the variables
#define DEBUG_MAIN
#include "debug/debug.hh"


/*
 * parse cmd line, fill all program variables
 * this is invoked by rasgeo_util/doStuff()
 */
void
parseParams(int argc, char** argv) throw (ImportError, r_Error)
{
        ENTER( "parseParams" );

	//program interface
	CommandLineParser    &cmlInter     	= CommandLineParser::getInstance();

	CommandLineParameter &clp_help		= cmlInter.addFlagParameter( PARAM_HELP_FLAG, PARAM_HELP, HELP_HELP );

	CommandLineParameter &clp_mddType	= cmlInter.addStringParameter( PARAM_MDDTYPE_FLAG, PARAM_MDDTYPE, HELP_MDDTYPE );
	CommandLineParameter &clp_collName	= cmlInter.addStringParameter( PARAM_COLL_FLAG, PARAM_COLL, HELP_COLL );
	CommandLineParameter &clp_mddDomain	= cmlInter.addStringParameter( CommandLineParser::noShortName, PARAM_DOMAIN, HELP_DOMAIN );
	CommandLineParameter &clp_file		= cmlInter.addStringParameter( PARAM_FILE_FLAG, PARAM_FILE, HELP_FILE );
	CommandLineParameter &clp_read		= cmlInter.addStringParameter( PARAM_READ_FLAG, PARAM_READ, HELP_READ );

	CommandLineParameter &clp_server	= cmlInter.addStringParameter( PARAM_SERV_FLAG, PARAM_SERV, HELP_SERV, DEFAULT_SERV );
	CommandLineParameter &clp_port		= cmlInter.addLongParameter( PARAM_PORT_FLAG, PARAM_PORT, HELP_PORT, DEFAULT_PORT);
	CommandLineParameter &clp_database	= cmlInter.addStringParameter( PARAM_DB_FLAG, PARAM_DB, HELP_DB, DEFAULT_DBNAME );
	CommandLineParameter &clp_user		= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_USER, HELP_USER );
	CommandLineParameter &clp_passwd	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_PASSWD, HELP_PASSWD );

	CommandLineParameter &clp_mddoid	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_MDDOID, HELP_MDDOID, DEFAULT_MDDOID );

	// CommandLineParameter &clp_overlayDomain	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_OVERLAY, HELP_OVERLAY, DEFAULT_OVERLAY );

	// CommandLineParameter &clp_conversiontypename	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_CONV, HELP_CONV, DEFAULT_CONV );

	CommandLineParameter &clp_align		= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_ALIGN, HELP_ALIGN );
	CommandLineParameter &clp_taSplit       = cmlInter.addFlagParameter(CommandLineParser::noShortName, PARAM_TASPLIT, HELP_TASPLIT );

	CommandLineParameter &clp_input		= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_DATAFORMAT, help_input.c_str(), DEFAULT_DATAFORMAT_STR );
	CommandLineParameter &clp_inputParams	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_DATAFORMAT_P, HELP_DATAFORMAT_P );

	CommandLineParameter &clp_tiledupdate	= cmlInter.addFlagParameter(CommandLineParser::noShortName, PARAM_TILED, HELP_TILED );
	CommandLineParameter &clp_transparent	= cmlInter.addFlagParameter( PARAM_TRANSP_FLAG, PARAM_TRANSP, HELP_TRANSP );

	CommandLineParameter &clp_polygon	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_POLYGON, HELP_POLYGON );
	CommandLineParameter &clp_shrink	= cmlInter.addLongParameter(CommandLineParser::noShortName, PARAM_SHRINK, HELP_SHRINK, DEFAULT_SHRINK);

	CommandLineParameter &clp_scalelevels	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_SCALE, HELP_SCALE );

// confusing, not tested, doubtful use -- so omit it
#ifdef SOMETIME_LATER
	CommandLineParameter &clp_tiling	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_TILING, HELP_TILING, DEFAULT_TILING_STR );
	CommandLineParameter &tilingParams	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_TILING_P, HELP_TILING_P, DEFAULT_TILING_STR );
#endif

	CommandLineParameter &clp_storage	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_STOR, HELP_STOR, DEFAULT_STOR_STR_ORTHO );
	CommandLineParameter &clp_storageParams	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_STOR_P, HELP_STOR_P );

	CommandLineParameter &clp_transfer	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_TRANS, HELP_TRANS, DEFAULT_TRANS_STR );
	CommandLineParameter &clp_transferParams= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_TRANS_P, HELP_TRANS_P );

	CommandLineParameter &clp_fillinside	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_INPAT, HELP_INPAT );
	CommandLineParameter &clp_filloutside	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_OUTPAT, HELP_OUTPAT );

	CommandLineParameter &clp_updatebuffer	= cmlInter.addLongParameter(CommandLineParser::noShortName, PARAM_BUF, HELP_BUF, DEFAULT_BUF );

	CommandLineParameter &clp_colltype	= cmlInter.addStringParameter(CommandLineParser::noShortName, PARAM_COLLTYPE, HELP_COLLTYPE );

	CommandLineParameter &clp_verbose       = cmlInter.addFlagParameter(PARAM_V_FLAG, PARAM_VERBOSE, HELP_VERBOSE );

#ifdef DEBUG
	CommandLineParameter &clp_debug	        = cmlInter.addFlagParameter(CommandLineParser::noShortName, PARAM_DEBUG, HELP_DEBUG );
#endif

	try
	{
		cmlInter.processCommandLine(argc, argv);

		if (cmlInter.isPresent( PARAM_HELP_FLAG ) || argc == 1)
		{
			cout << "options:" << endl;
			cmlInter.printHelp();
			cout << "Note: don't forget to escape shell sensitive characters in parameters, such as ';', '|'" << endl;
			cout << "Note2: see the rasdaman and rasgeo documentation set for details on the parameters." << endl;
			cout << "(*) functionality currently in beta, do not yet use under production conditions." << endl;
			exit( EXIT_USAGE );		//  FIXME: exit no good style!!
		}

		// check mandatory parameters ====================================================

                // evaluate mandatory parameter user --------------------------------------
                if ( cmlInter.isPresent( PARAM_USER ) )
			RasgeoUtil::userName = cmlInter.getValueAsString( PARAM_USER );
		else
                        throw ImportError( USERNOTSPECIFIED );

                // evaluate mandatory parameter passwd --------------------------------------
		if (cmlInter.isPresent( PARAM_PASSWD ))
			RasgeoUtil::passwd = cmlInter.getValueAsString( PARAM_PASSWD );
		else
                        throw ImportError( PASSWDNOTSPECIFIED );

		// evaluate mandatory parameter collection --------------------------------------
		if (cmlInter.isPresent( PARAM_COLL ))
			RasgeoUtil::collName = cmlInter.getValueAsString( PARAM_COLL );
		else
                	throw ImportError( NOCOLLNAMEDEFINED );

		// evaluate mandatory parameter MDD type name --------------------------------------
		if (cmlInter.isPresent( PARAM_MDDTYPE ))
			RasgeoUtil::mddTypeName = cmlInter.getValueAsString( PARAM_MDDTYPE );
		else
                	throw ImportError( NOMDDTYPEDEFINED );

		// evaluate mandatory parameter domain --------------------------------------
		if ( cmlInter.isPresent( PARAM_DOMAIN ) )
		{
			try
			{
				RasgeoUtil::mddDomain = r_Minterval(cmlInter.getValueAsString( PARAM_DOMAIN ));
			}
			catch ( r_Error& e )		// Minterval constructor had syntax problems
			{
				throw ImportError( NOVALIDDOMAIN );
			}
		}
		else
			throw ImportError( MDDDOMAINNOTSPECIFIED );

		// check optional parameters ====================================================

		// evaluate optional parameter overlaydomain --------------------------------------
		// overlayDomainDef = cmlInter.isPresent( PARAM_OVERLAY );
		// if (overlayDomainDef)
			// overlayDomain = r_Minterval(cmlInter.getValueAsString( PARAM_OVERLAY ));
		// else
			// overlaydomain is adapted automagically from other parameters if necessary
			RasgeoUtil::overlayDomain = RasgeoUtil::mddDomain;

		// evaluate optional parameter MDD oid --------------------------------------
		RasgeoUtil::mddOIdDef = cmlInter.isPresent( PARAM_MDDOID );
		if (RasgeoUtil::mddOIdDef)
			RasgeoUtil::mddOId = r_OId(cmlInter.getValueAsString( PARAM_MDDOID ));

#ifdef SOMETIME_LATER
		// evaluate optional parameters for tiling --------------------------------------
		if (cmlInter.isPresent( PARAM_TILING ))
		{
			r_Tiling_Scheme ts = get_tiling_scheme_from_name(cmlInter.getValueAsString( PARAM_TILING ));
			// depending on tiling param value, further parameters are required
			switch (ts)
			{
				case r_NoTiling:
					RasgeoUtil::theTiling = new r_No_Tiling();
					break;
				case r_StatisticalTiling:	// requires further parameters
					if (cmlInter.isPresent( PARAM_TILING_P ))
						RasgeoUtil::theTiling = new r_Stat_Tiling(cmlInter.getValueAsString( PARAM_TILING_P ));
					else
                				throw ImportError( TILINGPARAMETERSMISSING );
					break;
				case r_InterestTiling:	// requires further parameters
					if (cmlInter.isPresent( PARAM_TILING_P ))
						RasgeoUtil::theTiling = new r_Interest_Tiling(cmlInter.getValueAsString( PARAM_TILING_P ));
					else
                				throw ImportError( TILINGPARAMETERSMISSING );
					break;
				case r_AlignedTiling:	// requires further parameters
					if (cmlInter.isPresent( PARAM_TILING_P ))
						RasgeoUtil::theTiling = new r_Aligned_Tiling(cmlInter.getValueAsString( PARAM_TILING_P ));
					else
                				throw ImportError( TILINGPARAMETERSMISSING );
					break;
				case r_DirectionalTiling:	// requires further parameters
					if (cmlInter.isPresent( PARAM_TILING_P ))
						RasgeoUtil::theTiling = new r_Dir_Tiling(cmlInter.getValueAsString( PARAM_TILING_P ));
					else
                				throw ImportError( TILINGPARAMETERSMISSING );
					break;
				case r_SizeTiling:	// requires further parameters
					if (cmlInter.isPresent( PARAM_TILING_P ))
						RasgeoUtil::theTiling = new r_Size_Tiling(cmlInter.getValueAsString( PARAM_TILING_P ));
					else
                				throw ImportError( TILINGPARAMETERSMISSING );
					break;
				case r_RegularTiling:
					throw ImportError( TILINGTYPENOTALLOWED );
				default:
					throw ImportError( UNKNOWNTILINGTYPE );
			}
		}
		else	// set default: size tiling
#endif SOMETIME_LATER
		{
			stringstream s;
			s << DEFAULT_TILING;
			RasgeoUtil::theTiling = new r_Size_Tiling( s.str().c_str() );
		}

		// evaluate optional parameter transparent --------------------------------------
		RasgeoUtil::transparent = cmlInter.isPresent( PARAM_TRANSP );

		// evaluate optional parameter scale levels --------------------------------------
		if (cmlInter.isPresent( PARAM_SCALE ))
			RasgeoUtil::readScaleLevels(cmlInter.getValueAsString( PARAM_SCALE ));

		// evaluate optional parameter server --------------------------------------
		if (cmlInter.isPresent( PARAM_SERV ))
			RasgeoUtil::serverName = cmlInter.getValueAsString( PARAM_SERV );

		// evaluate optional parameter port --------------------------------------
		if (cmlInter.isPresent( PARAM_PORT ))
			RasgeoUtil::serverPort = cmlInter.getValueAsLong( PARAM_PORT );

		// evaluate optional parameter database --------------------------------------
		if (cmlInter.isPresent( PARAM_DB ))
			RasgeoUtil::baseName = cmlInter.getValueAsString( PARAM_DB );

		// evaluate optional parameter storageformat --------------------------------------
		if (cmlInter.isPresent( PARAM_STOR ))
		{
			RasgeoUtil::storageFormat = get_data_format_from_name(cmlInter.getValueAsString( PARAM_STOR ));
			if (cmlInter.isPresent( PARAM_STOR_P))
			{				// may appear only if previous param is present
				RasgeoUtil::storageFormatParams = cmlInter.getValueAsString( PARAM_STOR_P ); 
        		}
			else
				RasgeoUtil::storageFormatParams = NULL;
        	}

		// evaluate optional parameter transformat --------------------------------------
		if (cmlInter.isPresent( PARAM_TRANS ))
		{
			RasgeoUtil::transferFormat = get_data_format_from_name(cmlInter.getValueAsString( PARAM_TRANS ));
			if (cmlInter.isPresent( PARAM_TRANS_P ))
			{				// may appear only if previous param is present
				RasgeoUtil::transferFormatParams = cmlInter.getValueAsString( PARAM_TRANS_P );
        		}
        	}

		// evaluate optional parameter inputformat --------------------------------------
		if (cmlInter.isPresent( PARAM_DATAFORMAT ))
		{
			RasgeoUtil::inputFormat = get_data_format_from_name(cmlInter.getValueAsString( PARAM_DATAFORMAT ));
			if (cmlInter.isPresent( PARAM_DATAFORMAT_P ))
			{				// may appear only if previous param is present
				RasgeoUtil::inputFormatParams = cmlInter.getValueAsString( PARAM_DATAFORMAT_P );
        		}
        	}

		// evaluate optional parameter polygon --------------------------------------
		RasgeoUtil::polygonDefined = cmlInter.isPresent( PARAM_POLYGON );
		if (RasgeoUtil::polygonDefined)
		{
			try
			{
				r_Polygon p(cmlInter.getValueAsString( PARAM_POLYGON ));
				RasgeoUtil::polygon.addPolygon(p);
			}
			catch (r_Error& err)
			{
				throw ImportError( POLYGONCREATIONFAILED );
			}
		}

		// evaluate optional parameter polygonshrinker --------------------------------------
		RasgeoUtil::polygonShrinker = cmlInter.getValueAsLong( PARAM_SHRINK );

		// evaluate optional parameter outsidepattern --------------------------------------
		RasgeoUtil::outsidePatternDef = cmlInter.isPresent( PARAM_OUTPAT );
		if (RasgeoUtil::outsidePatternDef)
			RasgeoUtil::outsidePattern = HexCodec::convertTo(cmlInter.getValueAsString( PARAM_OUTPAT ));

		// evaluate optional parameter insidepattern --------------------------------------
		RasgeoUtil::insidePatternDef = cmlInter.isPresent( PARAM_INPAT );
		if (RasgeoUtil::insidePatternDef)
			RasgeoUtil::insidePattern = HexCodec::convertTo(cmlInter.getValueAsString( PARAM_INPAT ));

		// evaluate optional parameter align --------------------------------------
		if (cmlInter.isPresent( PARAM_ALIGN ))
		{
			RasgeoUtil::align = cmlInter.getValueAsLong( PARAM_ALIGN );
			if (RasgeoUtil::align <= 0)
				throw ImportError( INVALIDALIGN );

			TALK( "overlaydomain was: " << RasgeoUtil::overlayDomain );
			RasgeoUtil::overlayDomainDef = true;
			for (r_Dimension dim = 0; dim < RasgeoUtil::overlayDomain.dimension(); dim++)
			{	// round off lower bound, round up upper bound so that we cover all tiles of <align> size

				r_Range high = RasgeoUtil::overlayDomain[dim].high();
				r_Range highd = high% RasgeoUtil::align;
				if (high < 0)
					high = high - highd - 1;
				else
					high = high +  RasgeoUtil::align - highd - 1;
	
				r_Range low =  RasgeoUtil::overlayDomain[dim].low();
				r_Range lowd =  RasgeoUtil::overlayDomain[dim].low()% RasgeoUtil::align;
				if (low < 0)
					low = low -  RasgeoUtil::align - lowd;
				else
					low = low - lowd;

				RasgeoUtil::overlayDomain[dim].set_interval(low, high);
			}
			TALK( "Alignment reconsidered overlay domain to " << RasgeoUtil::overlayDomain );
		}
	
		// evaluate optional parameter read/file; file wins --------------------------------
		if ( cmlInter.isPresent("read") )
			RasgeoUtil::fileName = cmlInter.getValueAsString("read");
		if ( cmlInter.isPresent("file") )
			RasgeoUtil::fileName = cmlInter.getValueAsString("file");

		// evaluate optional parameter tiledupdate --------------------------------------
		RasgeoUtil::tiledImport = cmlInter.isPresent("tiledupdate");

		// evaluate optional parameter tasplit  --------------------------------------
		RasgeoUtil::taSplit = cmlInter.isPresent( PARAM_TASPLIT );

		// evaluate optional parameter conversiontype --------------------------------------
		// if ( cmlInter.isPresent("conversiontype") )
		// 	conversionTypeName = cmlInter.getValueAsString("conversiontype");

		// evaluate optional parameter scalefunction --------------------------------------
		// if ( cmlInter.isPresent("scalefunction") )
		// 	scaleFunction = get_scale_function_from_name(cmlInter.getValueAsString("scalefunction"));

		// evaluate optional parameter buffersize --------------------------------------
		if ( cmlInter.isPresent("buffersize") )
			RasgeoUtil::updateBufferSize = cmlInter.getValueAsLong("buffersize");

		// evaluate optional parameter verbose --------------------------------------
		RasgeoUtil::verbose = cmlInter.isPresent("verbose");

#ifdef DEBUG
		// evaluate optional parameter debug --------------------------------------
		RasgeoUtil::debug = cmlInter.isPresent("debug");
		SET_OUTPUT( RasgeoUtil::debug );
#endif

		// plausi check parameter combinations ====================================================

		// buffer size at least 1k
		if (RasgeoUtil::updateBufferSize < 1024)
			throw ImportError( INVALIDBUFSIZ );

		// domain spec must be 2D
		if (RasgeoUtil::mddDomain.dimension() != 2)
                	throw ImportError( DOMAINNOTTWODIMENSIONAL );
		if (RasgeoUtil::overlayDomainDef && RasgeoUtil::overlayDomain.dimension() != 2)
                	throw ImportError( DOMAINNOTTWODIMENSIONAL );

#if 0
		// either file name or transparency must be present (why this combination?)
		// I consider this as a bug and change to the 2nd alternative, see also error texts -- PB 2003-jul-05
		// if (fileName == NULL && ! transparent)
		if (RasgeoUtil::fileName == NULL && ! RasgeoUtil::transparent)
                	throw ImportError( FILENAMENOTSPECIFIED );
#endif

// lets test it...ok, seemed to work out with many examples
#if 0
		// tiledImport and file import not simultaneously, as tiledupdate changes overlay domain
		if (RasgeoUtil::tiledImport && RasgeoUtil::fileName != NULL)
			throw ImportError( TILEDUPDATEANDFILE );
#endif
	}
	catch(CmlException& err)
	{
		cout << err.what() << endl;
		throw ImportError( ERRORPARSINGCOMMANDLINE );
	}

	TALK( "server=" << RasgeoUtil::serverName << ", port=" << RasgeoUtil::serverPort << ", db=" << RasgeoUtil::baseName << ", user=" << RasgeoUtil::userName << ", pwd=" << RasgeoUtil::passwd );
	TALK( "fileName=" << RasgeoUtil::fileName << ", tiledImport=" << RasgeoUtil::tiledImport << ", taSplit=" << RasgeoUtil::taSplit );
	TALK( "transparent=" << RasgeoUtil::transparent << ", collname=" << RasgeoUtil::collName << ", mddOid=" << RasgeoUtil::mddOId << ", mddTypeName=" << RasgeoUtil::mddTypeName );
	TALK( "mddDomain=" << RasgeoUtil::mddDomain << ", align=" << RasgeoUtil::align << ", overlayDomain=" << RasgeoUtil::overlayDomain );
	TALK( "polygonDefined=" << RasgeoUtil::polygonDefined << ", polygonShrinker=" << RasgeoUtil::polygonShrinker );
	TALK( "polygon=(n.a.)" ); // this is missing, we don't have that: polygon.print_status();
	TALK( "outsidePatternDef=" << RasgeoUtil::outsidePatternDef << ", outsidePattern=" << HexCodec::convertFrom(RasgeoUtil::outsidePattern) );
	TALK( "insidePatternDef=" << RasgeoUtil::insidePatternDef << ", insidePattern=" << HexCodec::convertFrom(RasgeoUtil::insidePattern) );
	TALK( "storageFormat=" << RasgeoUtil::storageFormat << ", storageFormatParams=" << RasgeoUtil::storageFormatParams );
	TALK( "transferFormat=" << RasgeoUtil::transferFormat << ", transferFormatParams=" << RasgeoUtil::transferFormatParams );
	TALK( "inputFormat=" << RasgeoUtil::inputFormat << ", inputFormatParams=" << RasgeoUtil::inputFormatParams );
	TALK( "updateBufferSize=" << RasgeoUtil::updateBufferSize << ", scaleFunction=" << RasgeoUtil::scaleFunction );
	TALK( "theTiling=" << RasgeoUtil::theTiling ); 
	TALK( "verbose=" << RasgeoUtil::verbose ); 
#ifdef DEBUG
	if (RasgeoUtil::debug)
		RasgeoUtil::theTiling->print_status( cout );
#endif
	TALK( "" );

        LEAVE( "parseParams" );
	return;
}


/*
 * returns 0 on success, -1 on error
 */
int main(int argc, char** argv)
{
	SET_OUTPUT( false );		// inhibit unconditional debug output, await cmd line evaluation

	int retval = EXIT_SUCCESS;

	cout << argv[0] << ": rasdaman map image import tool v2.0. rasdaman v" << RMANVERSION/1000 << " -- generated on " << COMPDATE << ".\n";

	try
	{
		// set map type specific defaults
		RasgeoUtil::storageFormat = DEFAULT_STOR_ORTHO;
		RasgeoUtil::conversionTypeName = DEFAULT_CONV_ORTHO;

		parseParams( argc, argv );
		RasgeoUtil::doStuff( argc, argv, IMPORTTYPE_ORTHO );

		retval = EXIT_SUCCESS;
	}
	catch (ImportError& e)
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
	{
		RasgeoUtil::closeTransaction( false );	// abort transaction and close database, ignore any further exceptions
		RasgeoUtil::closeDatabase();
	}
	
	cout<< argv[0] << " done." << endl;
	return retval;
} // main()
