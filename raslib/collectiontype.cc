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

static const char rcsid[] = "@(#)raslib, r_Collection_Type: $Header: /home/rasdev/CVS-repository/rasdaman/raslib/collectiontype.cc,v 1.6 2003/12/27 23:01:21 rasdev Exp $";

#include "raslib/collectiontype.hh"
#include "raslib/rminit.hh"
#include "raslib/error.hh"


r_Collection_Type::r_Collection_Type()
    :   r_Type(),
        elementType(NULL)
{
}

r_Collection_Type::r_Collection_Type(r_Type& newElementType)
    :   r_Type(),
        elementType(newElementType.clone())
{
}

r_Collection_Type::r_Collection_Type(const r_Collection_Type& oldObj) throw (r_Error)
    :   r_Type(oldObj),
        elementType(NULL)
{
    if (oldObj.elementType)
        elementType = oldObj.elementType->clone();
    else
    {
        RMInit::logOut << "r_Collection_Type::r_Collection_Type( oldObj ) the element type is NULL." << endl;
        throw r_Error(COLLECTIONTYPEHASNOELEMENTTYPE);
    }
}

bool
r_Collection_Type::isCollectionType() const
{
    return true;
}

const r_Collection_Type&
r_Collection_Type::operator=(const r_Collection_Type& oldObj) throw (r_Error)
{
    // Gracefully handle self assignment
    if (this == &oldObj)
        return *this;

    r_Type::operator=(oldObj);
    delete elementType;
    elementType = NULL;
    if (oldObj.elementType)
        elementType = oldObj.elementType->clone();
    else
    {
        RMInit::logOut << "r_Collection_Type::operator=( oldObj ) the element type is NULL." << endl;
        throw r_Error(COLLECTIONTYPEHASNOELEMENTTYPE);
    }
    return *this;
}

const r_Type&
r_Collection_Type::element_type() const throw (r_Error)
{
    if (elementType == NULL)
    {
        RMInit::logOut << "r_Collection_Type::element_type() the element type is NULL." << endl;
        throw r_Error(COLLECTIONTYPEHASNOELEMENTTYPE);
    }
    return *elementType;
}

r_Type*
r_Collection_Type::clone() const
{
    return new r_Collection_Type(*this);
}

r_Collection_Type::r_Kind
r_Collection_Type::kind() const
{
    return SET;
}

r_Type::r_Type_Id
r_Collection_Type::type_id() const
{
    return COLLECTIONTYPE;
}

void
r_Collection_Type::convertToLittleEndian(char* cells, r_Area noCells) const
{
}

void
r_Collection_Type::convertToBigEndian(char* cells, r_Area noCells) const
{
}

void
r_Collection_Type::print_status(std::ostream& s) const
{
    s << "set< ";
    elementType->print_status(s);
    s << " >";
}



r_Collection_Type::~r_Collection_Type()
{
    if (elementType)
        delete elementType;
    elementType = NULL;
}

std::ostream &operator<<( std::ostream &str, const r_Collection_Type &type )
{
    type.print_status(str);
    return str;
}
