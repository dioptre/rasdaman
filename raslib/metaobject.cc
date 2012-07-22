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

#include "mymalloc/mymalloc.h"
static const char rcsid[] = "@(#)raslib, r_Meta_Object: $Header: /home/rasdev/CVS-repository/rasdaman/raslib/metaobject.cc,v 1.7 2003/12/27 23:01:21 rasdev Exp $";

#include "raslib/metaobject.hh"

#include <stdlib.h> // OSF1 has the definition for malloc here
#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif
#include <string.h>

r_Meta_Object::r_Meta_Object()
    : typeName(NULL)
{
}

r_Meta_Object::r_Meta_Object( const char* newTypeName )
{
    typeName = (char*)mymalloc(strlen(newTypeName) + 1);
    strcpy(typeName, newTypeName);
}

r_Meta_Object::r_Meta_Object( const r_Meta_Object& oldObj )
    : typeName(NULL)
{
    if( oldObj.typeName )
    {
        typeName = (char*)mymalloc(strlen(oldObj.typeName) + 1);
        strcpy(typeName, oldObj.typeName);
    }
}

const r_Meta_Object&
r_Meta_Object::operator=( const r_Meta_Object& oldObj )
{
    // Gracefully handle self assignment
    if (this == &oldObj) return *this;

    free(typeName);
    typeName = NULL;
    if( oldObj.typeName )
    {
        typeName = (char*)mymalloc(strlen(oldObj.typeName) + 1);
        strcpy(typeName, oldObj.typeName);
    }

    return *this;
}

r_Meta_Object::~r_Meta_Object()
{
    if(typeName)
        free(typeName);
}

const char*
r_Meta_Object::name() const
{
    return typeName;
}
