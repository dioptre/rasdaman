/*
* This file is part of rasdaman community.
*
* Rasdaman community is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Rasdaman community is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann / rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
/

/**
* rasqlError
*
* Provides a error checking for the rasql queries
* options for storing results to file(s) or displaying them
*
* COMMENTS:
*
* 		No comments
*/

using namespace std;

static const char rcsid[] = "@(#)raslib, RasqlError: $Id: rasql_error.cc,v 1.1 2003/12/27 19:30:23 rasdev Exp $";

#include <exception>
#include <cstring>

// for sprintf():
#include <stdio.h>

#include "rasql_error.hh"

// debug facility; relies on -DDEBUG at compile time
#include "debug-clt.hh"

/// error object, carrying int error code
RasqlError::RasqlError( unsigned int e )
{
	TALK( "Exception: " << e );
	errno = e;
}

/// default destructor
RasqlError::~RasqlError()
{
}

/// print error message (including error code)
/// NB: not all messages can occur
const char*
RasqlError::what()
{
	const char *errorMsg;
	switch (errno)
	{
		case  NOQUERY:
			errorMsg = "Mandatory parameter '--query' missing.";
			break;
		case  ERRORPARSINGCOMMANDLINE:
			errorMsg = "Command line syntax error.";
			break;
		case  ILLEGALOUTPUTTYPE:
			errorMsg = "Illegal output type specifier, must be one of none, file, formatted, string, hex.";
			break;
		case  FILEINACCESSIBLE:
			errorMsg = "Cannot read input file.";
			break;
		case  UNABLETOCLAIMRESOURCEFORFILE:
			errorMsg = "Cannot allocate memory for file read.";
			break;
		case  NOVALIDDOMAIN:
			errorMsg = "Syntax error in mdddomain specification, must be [x0:x1,y0:y1] (forgot to quote or escape?)";
			break;
		case  MDDTYPEINVALID:
			errorMsg = "MDD type invalid.";
			break;
		case  FILESIZEMISMATCH:
			errorMsg = "Input file size does not correspond with MDD domain specified.";
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
		sprintf( errorText, "%s%03d%s", MODULE_TAG, errno, "(error message too long, cannot display)" );
	else
		sprintf( errorText, "%s%03d%s%s", MODULE_TAG, errno, ERROR_TEXT, errorMsg );

	return errorText;
} // what()

 
