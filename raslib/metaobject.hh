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
 * INCLUDE: metaobject.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Meta_Object
 *
 * COMMENTS:
 *
*/

#ifndef _D_META_OBJECT_
#define _D_META_OBJECT_

#include <iostream>

#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <sstream> // for istrstream
#endif

#include "raslib/error.hh"

//@ManMemo: Module: {\bf raslib}

/*@Doc:
  This class the superclass for all classes in the ODMG conformant
  representation of the RasDaMan type system.
*/

class r_Meta_Object
{
public:
    /// default constructor.
    r_Meta_Object();
    /// constructor getting name of type.
    r_Meta_Object( const char* newTypeName );
    /// copy constructor
    r_Meta_Object( const r_Meta_Object& oldObj );
    /// assignment operator.
    const r_Meta_Object& operator=( const r_Meta_Object& oldObj );
    /// destructor.
    virtual ~r_Meta_Object();

    /// retrieve name of the type.
    const char* name() const;

    /// writes state of object to specified stream
    virtual void print_status( std::ostream& s = std::cout ) const = 0;

protected:
    char* typeName;
};

#endif
