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
#ifndef _PARSEINFO_
#define _PARSEINFO_

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

/*************************************************************
 *
 *
 * COMMENTS:
 *
 ************************************************************/


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

 This class holds some information about a token and its
 location in the parse string. Additionally, it can keep
 an error number which is used in the query tree to report
 error with some context information within the query string.

*/

class ParseInfo
{
public:

    /// default constructor
    ParseInfo();

    /// constructor getting error number, token and location information
    ParseInfo( unsigned long errorNo, const char* token, unsigned int lineNo, unsigned columnNo );

    /// copy constructor
    ParseInfo( const ParseInfo& old);

    /// constructor getting token and location information
    ParseInfo( const char* token, unsigned int lineNo, unsigned columnNo );

    //@Man: Read/Write methods
    //@{
    ///
    inline unsigned long getErrorNo() const;
    ///
    inline const std::string& getToken() const;
    ///
    inline unsigned int  getLineNo() const;
    ///
    inline unsigned int  getColumnNo() const;
    ///
    inline void          setErrorNo( unsigned long errorNo );
    ///
    inline void          setToken( const std::string& text );
    ///
    inline void          setToken( const char* text );
    ///
    inline void          setLineNo( unsigned int lineNo );
    ///
    inline void          setColumnNo( unsigned int columnNo );
    ///
    //@}

    void printStatus(  std::ostream& s= std::cout ) const;

    ParseInfo& operator=(const ParseInfo& old);

private:
    //@Man: Attributes
    //@{
    ///
    unsigned long errorNo;
    ///
    unsigned int lineNo;
    ///
    unsigned int columnNo;
    ///
    std::string token;
    ///
    //@}
};

#include "parseinfo.icc"

#endif
