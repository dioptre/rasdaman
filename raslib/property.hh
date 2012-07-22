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
 * INCLUDE: property.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Property
 *
 * COMMENTS:
 *
*/

#ifndef _D_PROPERTY_
#define _D_PROPERTY_

#include "metaobject.hh"


class r_Base_Type;
class r_Type_Id;

//@ManMemo: Module: {\bf raslib}

/*@Doc:
  This class the superclass for properties of classes in the ODMG
  conformant representation of the RasDaMan type system.
*/

class r_Property : public r_Meta_Object
{
public:

    /// constructor getting name and type of property.
    r_Property( const char* newTypeName, const r_Base_Type& newType );

    /// copy constructor.
    r_Property( const r_Property& oldObj );

    /// assignment operator.
    const r_Property& operator=( const r_Property& oldObj );

    /// destructor.
    virtual ~r_Property();

    /// retrieve type of property.
    const r_Base_Type& type_of() const;

protected:

    r_Base_Type* myType;

    /// default constructor.
    r_Property();

};

#endif
