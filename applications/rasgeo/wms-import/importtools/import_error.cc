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
 * SOURCE: import_error.cc
 *
 * MODULE: 
 * CLASS:	ImportError
 *
 * PURPOSE:
 *
 * CHANGE HISTORY (append further entries):
 * when		 who	what
 * ----------------------------------------------------------
 * 2003-jul-03   PB	created (cloned from raslib/error.cc)
 * 2003-dec-05   PB	file renamed from rasgeo_error.?? to export_error.??
 * 2004-mar-17   PB	new error codes: BASETYPEMISMATCH, DOMAINEXTENTMISMATCH
 * 2005-apr-28   PB     errno -> importErrno (name clash with system macro in gcc3)
 *
 * COMMENTS:
 *
 ************************************************************/

using namespace std;

static const char rcsid[] = "@(#)raslib, ImportError: $Id: import_error.cc,v 1.3 2005/07/06 23:21:44 rasdev Exp $";

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif


#include <exception>
#include <string>

// for sprintf():
#include <stdio.h>

#include "import_error.hh"

// debug facility; relies on -DDEBUG at compile time
#include "debug.hh"

/// error object, carrying int error code
ImportError::ImportError( unsigned int e )
{
	TALK( "Exception: " << e );
	importErrno = e;
}

/// default destructor
ImportError::~ImportError()
{
	TALK( "ImportError destructor for error " << importErrno );
}

/// print error message (including error code)
/// NB: not all messages can occur
const char*
ImportError::what()
{
	char *errorMsg;
	switch (importErrno)
	{
		case  EXCEPTIONEXECUTEQUERY:
			errorMsg = "Query execution error.";
			break;
		case  TILINGPATAMETERSINCORRECT:
			errorMsg = "Illegal tiling parameters.";
			break;
		case  UNKNOWNTILINGTYPE:
			errorMsg = "Illegal tiling type.";
			break;
		case  TILINGPARAMETERSMISSING:
			errorMsg = "Tiling option requires additional parameters, use --tilingparameter.";
			break;
		case CONVERSIONEXCEPTION:
			errorMsg = "Conversion failed.";
			break;
		case CONVERSIONNOTSUPPORTED:
			errorMsg = "Conversion format not supported.";
			break;
		case NOQUERY:
			errorMsg = "Cannot read query from file";
			break;
		case ARGUMENTSMISSING:
			errorMsg = "Argument sequence error (--arg(i) only if --arg(i-1) is present).";
			break;
		case ARGUMENTSINCOMPLETE:
			errorMsg = "Missing arguments (--arg? --arg?domain --arg?type where ? can be 1, 2, 3 or 4).";
			break;
		case CREATEANDNOCREATE:
			errorMsg = "You specified both create and no create.";
			break;
		case NOVALIDDOMAIN:
			errorMsg = "Invalid domain.";
			break;
		case FILEINACCESSIBLE:
			errorMsg = "Cannot access file.";
			break;
		case FILENAMETOOLONG:
			errorMsg = "File name too long.";
			break;
		case NOCREATECREATECOLL:
			errorMsg = "You specified to not create a collection, but actually want to create one.";
			break;
		case NOCOLLTYPE:
			errorMsg = "No collection type specified.";
			break;
		case COLLOIDANDCOLLNAME:
			errorMsg = "Only one of collection name and collection oid can be spcified.";
			break;
		case CREATECOLLWITHOID:
			errorMsg = "Cannot take user specified oid to create collection.";
			break;
		case CREATEMDDWITHOID:
			errorMsg = "Cannot take user specified oid to create marray.";
			break;
		case INVALIDTILESIZE:
			errorMsg = "Invalid tile size.";
			break;
		case DOMAINDATAMISMATCH:
			errorMsg = "Domain extent does not match extent of input file.";
			break;
		case MDDDOMAINNOTSPECIFIED:
			errorMsg = "No domain for marray specified.";
			break;
		case FILENAMENOTSPECIFIED:
			errorMsg = "No file name specified.";
			break;
		case NOCOLLNAMENOCOLLOID:
			errorMsg = "Neither collection name nor oid specified.";
			break;
		case MDDTYPEINVALID:
			errorMsg = "Marray type not valid.";
			break;
		case NOBASETYPE:
			errorMsg = "No base type in marray type.";
			break;
		case EXCEPTIONCREATECOLL:
			errorMsg = "Cannot create collection.";
			break;
		case COLLECTIONINACCESSIBLE:
			errorMsg = "Collection not accessible.";
			break;
		case OIDINVALID:
			errorMsg = "Illegal oid (syntax is: \"<dbms>|<database>|<number>\".";
			break;
		case MDDINACCESSIBLE:
			errorMsg = "Object not accessible.";
			break;
		case MDDOIDANDCOLL:
			errorMsg = "Both MDD oid and collection specified, can update either collection or single object.";
			break;
		case CONVERSIONTYPENOTABASETYPE:
			errorMsg = "Conversion type must be a base type.";
			break;
		case CONVERSIONRETURNEDWRONGTYPE:
			errorMsg = "Internal error: conversion module returned a type that was not a base type.";
			break;
		case POLYGONCREATIONFAILED:
			errorMsg = "Illegal polygon string.";
			break;
		case OVERLAYDOMAINSDONOTMATCH:
			errorMsg = "MDD domains do not match for overlaying.";
			break;
		case OVERLAYTYPESIZESDONOTMATCH:
			errorMsg = "MDD type sizes do not match for overlaying.";
			break;
		case POLYGONDOMAINTOOLARGE:
			errorMsg = "MDD domain does not cover polygon bounding box.";
			break;
		case NOCOLLNAMEDEFINED:
			errorMsg = "No collection name defined.";
			break;
		case NOMDDTYPEDEFINED:
			errorMsg = "No MDD type defined.";
			break;
		case MDDTYPEOFGMARRAYNOTINITIALISED:
			errorMsg = "MDD type of GMarray not initialised.";
			break;
		case SCALEDOMAINISNOTCORRECT:
			errorMsg = "Illegal scale domain.";
			break;
		case SCALELEVELSINCORRECT:
			errorMsg = "Illegal scale levels.";
			break;
		case GMARRAYSARENOTEQUAL:
			errorMsg = "Maps are not equal in their extent.";
			break;
		case UNKNOWNSCALEFUNCTION:
			errorMsg = "Unknown scale function.";
			break;
		case UNABLETOCLAIMRESOURCEFORFILE:
			errorMsg = "Unable to claim memory for the input files.";
			break;
		case ERRORPARSINGCOMMANDLINE:
			errorMsg = "Error parsing command line.";
			break;
		case TILINGTYPENOTALLOWED:
			errorMsg = "Illegal tiling type.";
			break;
		case LAYERSNOTCORRECT:
			errorMsg = "Illegal layer parameter.";
			break;
		case TOOMANYLAYERS:
			errorMsg = "Too many layers.";
			break;
		case OVERLAYDOMAINDOESNOTMATCH:
			errorMsg = "Overlay domain does not cover input file domain.";
			break;
		case TILEDUPDATEANDFILE:
			errorMsg = "Option tiledupdate and file not simultaneously.";
			break;
		case NOCOMPAREDESTINATION:
			errorMsg = "Need either a file or a collection name to get source data for compare operation.";
			break;
		case CONVERSIONTYPEMISSING:
			errorMsg = "Conversion type missing.";
			break;
		case MDDOIDMISSING:
			errorMsg = "Missing OId.";
			break;
		case INVALIDALIGN:
			errorMsg = "Invalid align value.";
			break;
		case SIGNALCAUGHT:
			errorMsg = "Signal caught, have to terminate.";
			break;
		case DOMAINNOTTWODIMENSIONAL:
			errorMsg = "Domain specification must be two-dimensional.";
			break;
		case INVALIDSAMPLINGMETHOD:
			errorMsg = "Invalid sampling method specifier.";
			break;
		case UNKNOWNBASETYPE:
			errorMsg = "Unknown base type, cannot handle.";
			break;
		case SCALEDOMAINPOSSIBLEUNDERFLOW:
			errorMsg = "Illegal scale domain, possibly interval underflow due to large scale factor.";
			break;
		case INVALIDSCALENAME:
			errorMsg = "Illegal scalelevel parameter: empty scale name.";
			break;
		case INVALIDSCALEFACTOR:
			errorMsg = "Illegal scalelevel parameter: scale factor not numeric or not > 1.0.";
			break;
		case INVALIDCOLLNAMECHAR:
			errorMsg = "Illegal character in collection name.";
			break;
		case INVALIDFLOAT:
			errorMsg = "Illegal float parameter.";
			break;
		case INVALIDDATAFORMAT:
			errorMsg = "Illegal transfer/storage parameter.";
			break;
		case INVALIDRESOLUTION:
			errorMsg = "Illegal resolution value.";
			break;
		case INVALIDSKIP:
			errorMsg = "Skip value must be nonegative.";
			break;
		case INVALIDHEX:
			errorMsg = "Illegal hexadecimal string.";
			break;
		case INVALIDBUFSIZ:
			errorMsg = "Illegal buffer size.";
			break;
		case LAYERLISTNOTDEFINED:
			errorMsg = "No thematic layers specified for import.";
			break;
		case TOOMANYWILDCARDS:
			errorMsg = "File name contains more than one wildcard characters.";
			break;
		case NOWILDCARD:
			errorMsg = "File name does not contain a wildcard character for layer substitution.";
			break;
		case TKLAYERMISMATCH:
			errorMsg = "Layer list does not fit into map type.";
			break;
		case UNSUPPORTEDTYPESIZE:
			errorMsg = "Map pixel type is not an atomic type; this is currently not supported.";
			break;
		case ILLEGALIMPORTTYPE:
			errorMsg = "Internal error, illegal import type.";
			break;
		case CANNOTACCESSCOLL:
			errorMsg = "Cannot access collection.";
			break;
		case CANNOTUPDATECOLL:
			errorMsg = "Cannot update collection.";
			break;
		case BLOCKEXTENTOFLO:
			errorMsg = "Block (TIFF stripe) extent overflow - try with larger buffer size.";
			break;
		case DOMAINRETRIEVAL:
			errorMsg = "Internal error, database domain mismatch.";
			break;
		case USERNOTSPECIFIED:
			errorMsg = "Mandatory parameter --user missing.";
			break;
		case PASSWDNOTSPECIFIED:
			errorMsg = "Mandatory parameter --passwd missing.";
			break;
		case COORDMISSING:
			errorMsg = "Mandatory coordinate parameter missing.";
			break;
		case RESMISSING:
			errorMsg = "Mandatory resolution parameter missing.";
			break;
		case TIFF_FILENAME:
			errorMsg = "Cannot read TIFF file.";
			break;
		case TIFF_TAGS:
			errorMsg = "Cannot access TIFF width/length tags.";
			break;
		case TIFF_BITMAPSONLY:
			errorMsg = "Unsupported TIFF type (only bitmaps).";
			break;
		case TIFF_OUTPUTBYTES:
			errorMsg = "Unsupported pixel size.";
			break;
		case BASETYPEMISMATCH:
			errorMsg = "Database and input pixel size don't match.";
			break;
		case DOMAINEXTENTMISMATCH:
			errorMsg = "Database and input extents don't match.";
			break;
		default :
			errorMsg = "Unknown error code.";
			break;
		case  ALLDONE:
		case 0:
			errorMsg = "No errors.";
	}

// size of error text buffer below
#define ERRTEXT_BUFSIZ 200

	static char errorText[ERRTEXT_BUFSIZ];

// text constants for error msg
#define MODULE_TAG "IO"
#define ERROR_TEXT " Error: "

	// check for buffer overflow
	if (strlen(MODULE_TAG) + 3 + strlen(ERROR_TEXT) + strlen(errorMsg) + 1 > ERRTEXT_BUFSIZ)
		sprintf( errorText, "%s%03d%s", MODULE_TAG, importErrno, "(error message too long, cannot display)" );
	else
		sprintf( errorText, "%s%03d%s%s", MODULE_TAG, importErrno, ERROR_TEXT, errorMsg );

	return errorText;
} // what()

 
