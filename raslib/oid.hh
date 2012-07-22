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
 * INCLUDE: oid.hh
 *
 * MODULE:  raslib
 * CLASS:   r_OId
 *
 * COMMENTS:
 *      The class represents an object identifier (OId).
 *
*/

#ifndef _D_OID_
#define _D_OID_

#include <iosfwd>

//@ManMemo: Module: {\bf raslib}

/*@Doc:

 Class \Ref{r_OId} represents an object identifier.

*/

class r_OId
{
public:
    /// default constructor
    r_OId();

    /// constructs an OId from the string representation
    r_OId( const char* );

    /// constructor getting oid parts
    r_OId( const char* initSystemName, const char* initBaseName, double initLocalOId );

    /// copy constructor
    r_OId( const r_OId& );

    /// destructor
    virtual ~r_OId();

    /// it is called when an object leaves transient memory
    virtual void r_deactivate();

    /// debug output
    void print_status(std::ostream& s) const;

    /// operator for assigning an oid
    const r_OId& operator= ( const r_OId& );

    //@Man: Comparison operators:
    //@{
    ///

    /// operator for equality
    bool operator==( const r_OId& ) const;

    /// operator for not equal
    bool operator!=( const r_OId& ) const;

    /// operator for greater than
    bool operator> ( const r_OId& ) const;

    /// operator for less than
    bool operator< ( const r_OId& ) const;

    /// operator for greater or equal than
    bool operator>=( const r_OId& ) const;

    /// operator for less than or equal
    bool operator<=( const r_OId& ) const;

    ///
    //@}

    /// gets the oid's string representation
    inline const char* get_string_representation() const;

    /// get system name
    inline const char* get_system_name() const;

    /// get base name
    inline const char* get_base_name() const;

    /// get local oid
    inline double get_local_oid() const;

    /// determines if oid is valid
    inline bool is_valid() const;

private:
    /// string representation
    char* oidString;

    /// system name
    char* systemName;

    /// base name
    char* baseName;

    /// local oid
    double localOId;
};



//@ManMemo: Module: {\bf raslib}
/**
  Output stream operator for objects of type {\tt const} \Ref{r_Oid}.
*/
extern std::ostream& operator<<( std::ostream& s, const r_OId& oid );

#include "raslib/oid.icc"
#endif
