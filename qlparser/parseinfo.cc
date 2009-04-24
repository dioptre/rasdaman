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

static const char rcsid[] = "@(#)qlparser, ParseInfo: $Id: parseinfo.cc,v 1.6 2002/06/19 14:36:23 coman Exp $";

#include <iostream>
using namespace std;

#include "qlparser/parseinfo.hh"


ParseInfo::ParseInfo()
  : errorNo( 0 ),
    token( "" ),
    lineNo( 0 ),
    columnNo( 0 )
{
}

ParseInfo::ParseInfo(const ParseInfo& old)
  : errorNo( 0 ),
    token( "" ),
    lineNo( 0 ),
    columnNo( 0 )
{
  errorNo=old.errorNo;
  lineNo=old.lineNo;
  columnNo=old.columnNo;
  token=old.token;
}

ParseInfo::ParseInfo( const char* initToken, unsigned int initLineNo, unsigned initColumnNo )
  : errorNo( 0 ),
    token( "" ),
    lineNo( initLineNo ),
    columnNo( initColumnNo )
{
 if(initToken) {
  token=initToken;
 }
}



ParseInfo::ParseInfo( unsigned long initErrorNo, const char* initToken, unsigned int initLineNo, unsigned initColumnNo )
  : errorNo( initErrorNo ),
    token( "" ),
    lineNo( initLineNo ),
    columnNo( initColumnNo )
{
 if(initToken) {
  token=initToken;
 }
}

ParseInfo& 
ParseInfo::operator=(const ParseInfo& old)
{
 if(this != &old) {
   errorNo=old.errorNo;
   lineNo=old.lineNo;
   columnNo=old.columnNo;
   token=old.token;
 }
 return *this;
}

void
ParseInfo::printStatus( ostream& s ) const
{
  s << "Error number: " << errorNo << " Token: " << token.c_str() << " Line: " << lineNo << " Column: " << columnNo << endl;
}

