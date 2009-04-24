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
 *
 *
 * COMMENTS:
 *
 ************************************************************/

using namespace std;

static const char rcsid[] = "@(#)raslib, RasdlError: $Id: rasdl_error.cc,v 1.1 2003/12/19 17:11:59 rasdev Exp $";

#include <exception>
#include <cstring>

// for sprintf():
#include <stdio.h>

#include "rasdl_error.hh"

// debug facility; relies on -DDEBUG at compile time
#include "debug.hh"

/// error object, carrying int error code
RasdlError::RasdlError( unsigned int e )
{
	TALK( "Exception: " << e );
	errno = e;
}

/// default destructor
RasdlError::~RasdlError()
{
}

/// print error message (including error code)
/// NB: not all messages can occur
const char*
RasdlError::what()
{
	const char *errorMsg;
	switch (errno)
	{
		case  CANNOTALLOC:
			errorMsg = "Cannot allocate memoery.";
			break;
		case  CANNOTWRITEHDR:
			errorMsg = "Cannot write header file.";
			break;
		case  NOCONNECTION:
			errorMsg = "Cannot connect to database.";
			break;
		case  EMPTYTYPENAME:
			errorMsg = "Typename is empty.";
			break;
		case  ILLEGALREADCOMBI:
			errorMsg = "Parameter -r only in conjunction with -i or --hh.";
			break;
		case  ODLFILEFAILED:
			errorMsg = "Cannot access type definition file.";
			break;
		case  ODLPARSEERROR:
			errorMsg = "Syntax error in type definition file.";
			break;
		case  ILLEGALHHCOMBI:
			errorMsg = "Parameter -hh requires -r.";
			break;
		case  ILLEGALINSERTCOMBI:
			errorMsg = "Parameter -i requires -r.";
			break;
		case  CMDLINE:
			errorMsg = "Syntax error in command line.";
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
#define MODULE_TAG "DL"
#define ERROR_TEXT " Error: "

	// check for buffer overflow
	if (strlen(MODULE_TAG) + 3 + strlen(ERROR_TEXT) + strlen(errorMsg) + 1 > ERRTEXT_BUFSIZ)
		sprintf( errorText, "%s%03d%s", MODULE_TAG, errno, "(error message too long, cannot display)" );
	else
		sprintf( errorText, "%s%03d%s%s", MODULE_TAG, errno, ERROR_TEXT, errorMsg );

	return errorText;
} // what()

 
