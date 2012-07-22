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

static const char rcsid[] = "@(#)raslib, r_Oid_Type: $Header: /home/rasdev/CVS-repository/rasdaman/raslib/oidtype.cc,v 1.6 2003/12/27 23:01:21 rasdev Exp $";

#include "raslib/oidtype.hh"

r_Oid_Type::r_Oid_Type()
    : r_Type()
{
}

bool
r_Oid_Type::isOidType() const
{
    return true;
}

r_Oid_Type::r_Oid_Type( const r_Oid_Type& oldObj )
    : r_Type(oldObj)
{
}

r_Type*
r_Oid_Type::clone() const
{
    return new r_Oid_Type( *this );
}

r_Type::r_Type_Id
r_Oid_Type::type_id() const
{
    return OIDTYPE;
}

void
r_Oid_Type::convertToLittleEndian(char* cells, r_Area noCells) const
{
}

void
r_Oid_Type::convertToBigEndian(char* cells, r_Area noCells) const
{
}

void
r_Oid_Type::print_status( std::ostream& s ) const
{
    s << "oid";
}

r_Oid_Type::~r_Oid_Type()
{
}

std::ostream &operator<<( std::ostream &str, const r_Oid_Type &type )
{
    type.print_status(str);
    return str;
}
