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

using namespace std;

static const char rcsid[] = "@(#)rasgeo, InitError: $Id: rasgeo_error.cc,v 1.1 2003/12/27 21:57:18 rasdev Exp $";

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif


#include <exception>
#include <string>

// for sprintf():
#include <stdio.h>

#include "rasgeo_error.hh"

// debug facility; relies on -DDEBUG at compile time
#include "debug/debug.hh"

/// error object, carrying int error code
InitError::InitError( unsigned int e )
{
    TALK( "Exception: " << e << " for this=" << this );
    errorCode = e;
}

/// default destructor
InitError::~InitError()
{
}

/// print error message (including error code)
const char*
InitError::what()
{
    char *errorMsg;
    TALK( "error code is: " << errorCode );
    switch (errorCode)
    {
    case NOCOLLNAME:
        errorMsg = "Mandatory parameter --mapname missing.";
        break;
    case INVALIDFLOAT:
        errorMsg = "Invalid floating point number";
        break;
    case NOXMIN:
        errorMsg = "Mandatory parameter -x missing.";
        break;
    case NOXMAX:
        errorMsg = "Mandatory parameter -X missing.";
        break;
    case NOYMIN:
        errorMsg = "Mandatory parameter -y missing.";
        break;
    case NOYMAX:
        errorMsg = "Mandatory parameter -Y missing.";
        break;
    case NORES:
        errorMsg = "Mandatory pararmeter --res missing.";
        break;
    case ERRORPARSINGCOMMANDLINE:
        errorMsg = "Error parsing command line.";
        break;
    case INVALIDBBOX:
        errorMsg = "Coordinates do not outline a valid bounding box.";
        break;
    case INVALIDLEVEL:
        errorMsg = "Invalid level parameter, must be greater or equal to zero.";
        break;
    case ILLEGALMAPTYPE:
        errorMsg = "Illegal map type, use -h for choices available.";
        break;
    case NOMAPTYPE:
        errorMsg = "Mandatory parameter --maptype missing.";
        break;
    case INVALIDTILEEDGE:
        errorMsg = "Invalid tileedge parameter, must be greater than zero.";
        break;
    case ALLOCFAIL:
        errorMsg = "Cannot allocate tile buffer.";
        break;
    case NOTINSERTED:
        errorMsg = "Cannot insert object into collection.";
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
#define MODULE_TAG "IN"
#define ERROR_TEXT " Error: "

    // check for buffer overflow
    if (strlen(MODULE_TAG) + 3 + strlen(ERROR_TEXT) + strlen(errorMsg) + 1 > ERRTEXT_BUFSIZ)
        sprintf( errorText, "%s%03d%s", MODULE_TAG, errorCode, "(error message too long, cannot display)" );
    else
        sprintf( errorText, "%s%03d%s%s", MODULE_TAG, errorCode, ERROR_TEXT, errorMsg );

    return errorText;
} // what()


