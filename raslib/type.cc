/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
*/


static const char rcsid[] = "@(#)raslib, r_Type: $Header: /home/rasdev/CVS-repository/rasdaman/raslib/type.cc,v 1.17 2003/12/27 23:01:21 rasdev Exp $";

#include <ctype.h>   //  for isalpha()
#include <string.h> //  for strncmp()

#include "raslib/type.hh"
#include "raslib/collectiontype.hh"
#include "raslib/primitivetype.hh"
#include "raslib/complextype.hh"
#include "raslib/structuretype.hh"
#include "raslib/marraytype.hh"
#include "raslib/sintervaltype.hh"
#include "raslib/mintervaltype.hh"
#include "raslib/pointtype.hh"
#include "raslib/oidtype.hh"
#include "raslib/attribute.hh"
#include "raslib/error.hh"
#include "raslib/rminit.hh"

r_Type::r_Type()
    : r_Meta_Object()
{
}

r_Type::r_Type(const char* newTypeName)
    : r_Meta_Object(newTypeName)
{
}

r_Type::r_Type(const r_Type& oldObj)
    : r_Meta_Object(oldObj)
{
}

const r_Type&
r_Type::operator=(const r_Type& oldObj)
{
    // Gracefully handle self assignment
    if (this == &oldObj) return *this;

    r_Meta_Object::operator=(oldObj);

    return *this;
}

r_Type::~r_Type()
{
}

bool
r_Type::isStructType() const
{
    return false;
}

bool
r_Type::isComplexType() const
{
    return false;
}

bool
r_Type::isBaseType() const
{
    return false;
}

bool
r_Type::isCollectionType() const
{
    return false;
}

bool
r_Type::isMarrayType() const
{
    return false;
}

bool
r_Type::isPrimitiveType() const
{
    return false;
}

bool
r_Type::isSintervalType() const
{
    return false;
}

bool
r_Type::isMintervalType() const
{
    return false;
}

bool
r_Type::isPointType() const
{
    return false;
}

bool
r_Type::isOidType() const
{
    return false;
}

r_Type*
r_Type::get_any_type(const char* type_string)
{
    char*   pos         = (char*)type_string;
    char*   identifier  = NULL;
    r_Type* returnValue = NULL;
    DLTOKEN token = DLUNKNOWN;

    // one token look ahead
    char* oldPos = pos;
    token = getNextToken(pos, identifier);
    pos = oldPos;

    switch(token)
    {
    case DLMARRAY:
        returnValue = getMarrayType(pos);
        break;
    case DLSET:
        returnValue = getCollectionType(pos);
        break;
    default:
        returnValue = getType(pos);
    }

    return returnValue;
}

r_Type::DLTOKEN
r_Type::getNextToken(char* &pos, char* &identifier)
{
    DLTOKEN token = DLUNKNOWN;

    while(*pos == ' ') pos ++;

    if(!strncmp(pos, "marray", 6))
    {
        token = DLMARRAY;
        pos += 6;
    }
    else if(!strncmp(pos, "set", 3))
    {
        token = DLSET;
        pos += 3;
    }
    else if(!strncmp(pos, "struct", 6))
    {
        token = DLSTRUCT;
        pos += 6;
    }
    else if(*pos == '[')
    {
        token = DLLEP;
        pos += 1;
    }
    else if(*pos == ']')
    {
        token = DLREP;
        pos += 1;
    }
    else if(*pos == '<')
    {
        token = DLLAP;
        pos += 1;
    }
    else if(*pos == '>')
    {
        token = DLRAP;
        pos += 1;
    }
    else if(*pos == '{')
    {
        token = DLLCP;
        pos += 1;
    }
    else if(*pos == '}')
    {
        token = DLRCP;
        pos += 1;
    }
    else if(*pos == ',')
    {
        token = DLCOMMA;
        pos += 1;
    }
    else if(!strncmp(pos, "octet", 5))
    {
        token = DLOCTET;
        pos += 5;
    }
    else if(!strncmp(pos, "char", 4))
    {
        token = DLCHAR;
        pos += 4;
    }
    else if(!strncmp(pos, "ulong", 5))
    {
        token = DLULONG;
        pos += 5;
    }
    else if(!strncmp(pos, "long", 4))
    {
        token = DLLONG;
        pos += 4;
    }
    else if(!strncmp(pos, "short", 5))
    {
        token = DLSHORT;
        pos += 5;
    }
    else if(!strncmp(pos, "ushort", 6))
    {
        token = DLUSHORT;
        pos += 6;
    }
    else if(!strncmp(pos, "float", 5))
    {
        token = DLFLOAT;
        pos += 5;
    }
    else if(!strncmp(pos, "double", 6))
    {
        token = DLDOUBLE;
        pos += 6;
    }
    else if(!strncmp(pos, "bool", 4))
    {
        token = DLBOOL;
        pos += 4;
    }
    else if(!strncmp(pos, "complexd", 8))
    {
        token = DLCOMPLEXTYPE2;
        pos += 8;
    }
    // the order of testing it's important here
    // (complex is a proper prefix of complexd!)
    else if(!strncmp(pos, "complex", 7))
    {
        token = DLCOMPLEXTYPE1;
        pos += 7;
    }
    else if(!strncmp(pos, "interval", 8))
    {
        token = DLINTERVAL;
        pos += 8;
    }
    else if(!strncmp(pos, "minterval", 9))
    {
        token = DLMINTERVAL;
        pos += 9;
    }
    else if(!strncmp(pos, "point", 5))
    {
        token = DLPOINT;
        pos += 5;
    }
    else if(!strncmp(pos, "oid", 3))
    {
        token = DLOID;
        pos += 3;
    }
    else
    {
        token = DLIDENTIFIER;  // identifier

        char* beginPos = pos;

        // read identifier
        while(isalnum(*pos) || *pos == '-' || *pos == '_') pos++;

        identifier = new char[pos-beginPos+1];
        strncpy(identifier, beginPos, pos-beginPos);
        identifier[pos-beginPos] = '\0';
    }

    while(*pos == ' ') pos ++;

    return token;
}

r_Collection_Type*
r_Type::getCollectionType(char* &pos)
{
    char*   identifier = NULL;
    r_Collection_Type* returnValue = NULL;
    r_Type* elementType = NULL;
    DLTOKEN token = DLUNKNOWN;

    // get 'set'
    getNextToken(pos, identifier);

    // get '<'
    if(getNextToken(pos, identifier) != DLLAP)
    {
        RMInit::logOut << "r_Type::getCollectionType(" << pos << ") expected DLLAP" << endl;
        throw r_Error(INTERNALDLPARSEERROR);
    }

    // one token look ahead
    char* oldPos = pos;
    token = getNextToken(pos, identifier);
    pos = oldPos;

    switch(token)
    {
    case DLMARRAY:
        elementType = getMarrayType(pos);
        break;
    default:
        elementType = getType(pos);
    }

    returnValue = new r_Collection_Type(*elementType);
    delete elementType;

    return returnValue;
}

r_Type*
r_Type::getType(char* &pos)
{
    DLTOKEN token = DLUNKNOWN;
    char*   identifier = 0;
    r_Type* returnValue = 0;

    // one token look ahead
    char* oldPos = pos;
    token = getNextToken(pos, identifier);
    pos = oldPos;

    if(token == DLSTRUCT)
        returnValue = getStructureType(pos);
    else if(token == DLINTERVAL)
        returnValue = getSintervalType(pos);
    else if(token == DLMINTERVAL)
        returnValue = getMintervalType(pos);
    else if(token == DLPOINT)
        returnValue = getPointType(pos);
    else if(token == DLOID)
        returnValue = getOidType(pos);
    else
        returnValue = getPrimitiveType(pos);

    return returnValue;
}

r_Marray_Type*
r_Type::getMarrayType(char* &pos)
{
    char*   identifier = NULL;
    r_Marray_Type* returnValue = NULL;
    r_Base_Type*  basetype = NULL;

    // get 'marray'
    getNextToken(pos, identifier);
    // get '<'
    if(getNextToken(pos, identifier) != DLLAP)
    {
        RMInit::logOut << "r_Type::getMarrayType(" << pos << ") expected DLLAP" << endl;
        throw r_Error(INTERNALDLPARSEERROR);
    }

    // get base type (structure or primitive type)
    basetype = getBaseType(pos);

    returnValue = new r_Marray_Type(*basetype);

    delete basetype;

    return returnValue;
}

r_Base_Type*
r_Type::getBaseType(char* &pos, int offset)
{
    DLTOKEN token = DLUNKNOWN;
    char*   identifier = NULL;
    r_Base_Type* returnValue = NULL;

    // one token look ahead
    char* oldPos = pos;
    token = getNextToken(pos, identifier);
    pos = oldPos;

    if(token == DLSTRUCT)
        returnValue = getStructureType(pos, offset);
    else
        returnValue = getPrimitiveType(pos);

    return returnValue;
}

r_Primitive_Type*
r_Type::getPrimitiveType(char* &pos)
{
    char*             dummy = NULL;
    r_Primitive_Type* returnValue = NULL;

    switch(getNextToken(pos, dummy))
    {
    case DLCHAR:
        returnValue = new r_Primitive_Type("Char", r_Type::CHAR);
        break;
    case DLOCTET:
        returnValue = new r_Primitive_Type("Octet", r_Type::OCTET);
        break;
    case DLSHORT:
        returnValue = new r_Primitive_Type("Short", r_Type::SHORT);
        break;
    case DLUSHORT:
        returnValue = new r_Primitive_Type("UShort", r_Type::USHORT);
        break;
    case DLLONG:
        returnValue = new r_Primitive_Type("Long", r_Type::LONG);
        break;
    case DLULONG:
        returnValue = new r_Primitive_Type("ULong", r_Type::ULONG);
        break;
    case DLBOOL:
        returnValue = new r_Primitive_Type("Bool", r_Type::BOOL);
        break;
    case DLFLOAT:
        returnValue = new r_Primitive_Type("Float", r_Type::FLOAT);
        break;
    case DLDOUBLE:
        returnValue = new r_Primitive_Type("Double", r_Type::DOUBLE);
        break;
    case DLCOMPLEXTYPE1:
        returnValue = new r_Complex_Type("Complex1", r_Type::COMPLEXTYPE1);
        break;
    case DLCOMPLEXTYPE2:
        returnValue = new r_Complex_Type("Complex2", r_Type::COMPLEXTYPE2);
        break;
    default:
    {
        RMInit::logOut << "r_Type::getPrimitiveType(" << pos << ") unknown token" << endl;
        throw r_Error(INTERNALDLPARSEERROR);
    }
    }

    return returnValue;
}

r_Structure_Type*
r_Type::getStructureType(char* &pos, int offset)
{
    r_Structure_Type* returnValue = NULL;
    char*             identifier = NULL;
    DLTOKEN           token = DLUNKNOWN;
    r_Attribute*      attributes = NULL;
    int               noAttributes=0;

    // get 'struct'
    getNextToken(pos, identifier);

    // get '{'
    if(getNextToken(pos, identifier) != DLLCP)
    {
        RMInit::logOut << "r_Type::getStructureType(" << pos << ", " << offset << ") expected DLLCP" << endl;
        throw r_Error(INTERNALDLPARSEERROR);
    }

    int localOffset = offset;

    while(token != DLRCP)
    {
        // get type
        r_Base_Type* type = getBaseType(pos, localOffset);

        // adjust local offset
        localOffset += type->size();

        // get optional name
        token = getNextToken(pos, identifier);

        // allocate another attribute (very inefficient)
        noAttributes++;
        r_Attribute* oldAttributes = attributes;
        attributes = new r_Attribute[noAttributes];
        for(int i=0; i < noAttributes-1; i++)
            attributes[i] = oldAttributes[i];
        if(oldAttributes)
            delete[] oldAttributes;
        oldAttributes = NULL;

        if(token == DLIDENTIFIER)
        {
            // with identifier
            attributes[noAttributes-1] = r_Attribute(identifier, *type);
            delete[] identifier;
            identifier = NULL;
            // read next token
            token = getNextToken(pos, identifier);
        }
        else
        {
            // without identifier
            attributes[noAttributes-1] = r_Attribute("", *type);
        }

        delete type;
        type = NULL;

        if(token != DLCOMMA && token != DLRCP)
        {
            RMInit::logOut << "r_Type::getStructureType(" << pos << ", " << offset << ") expected DLRCP or DLCOMMA" << endl;
            throw r_Error(INTERNALDLPARSEERROR);
        }
    }

    returnValue = new r_Structure_Type("Structure", noAttributes, attributes, offset);

    if(attributes)
        delete[] attributes;
    attributes = NULL;
    return returnValue;
}

r_Sinterval_Type*
r_Type::getSintervalType(char* &pos)
{
    char*             dummy = NULL;
    r_Sinterval_Type* returnValue = NULL;

    getNextToken(pos, dummy);

    returnValue = new r_Sinterval_Type();

    return returnValue;
}

r_Minterval_Type*
r_Type::getMintervalType(char* &pos)
{
    char*             dummy = NULL;
    r_Minterval_Type* returnValue = NULL;

    getNextToken(pos, dummy);

    returnValue = new r_Minterval_Type();

    return returnValue;
}

r_Point_Type*
r_Type::getPointType(char* &pos)
{
    char*             dummy = NULL;
    r_Point_Type* returnValue = NULL;

    getNextToken(pos, dummy);

    returnValue = new r_Point_Type();

    return returnValue;
}

r_Oid_Type*
r_Type::getOidType(char* &pos)
{
    char*             dummy = NULL;
    r_Oid_Type* returnValue = NULL;

    getNextToken(pos, dummy);

    returnValue = new r_Oid_Type();

    return returnValue;
}

std::ostream& operator<<( std::ostream& s, r_Type::r_Type_Id t )
{
    switch( t )
    {
    case r_Type::ULONG:
        s << "ulong";
        break;
    case r_Type::USHORT:
        s << "ushort";
        break;
    case r_Type::BOOL:
        s << "bool";
        break;
    case r_Type::LONG:
        s << "long";
        break;
    case r_Type::SHORT:
        s << "short";
        break;
    case r_Type::OCTET:
        s << "octet";
        break;
    case r_Type::DOUBLE:
        s << "double";
        break;
    case r_Type::FLOAT:
        s << "float";
        break;
    case r_Type::CHAR:
        s << "char";
        break;
    case r_Type::COMPLEXTYPE1:
        s << "complextype1";
        break;
    case r_Type::COMPLEXTYPE2:
        s << "complextype2";
        break;
    case r_Type::STRUCTURETYPE:
        s << "structuretype";
        break;
    case r_Type::MARRAYTYPE:
        s << "marraytype";
        break;
    case r_Type::COLLECTIONTYPE:
        s << "collectiontype";
        break;
    case r_Type::SINTERVALTYPE:
        s << "sintervaltype";
        break;
    case r_Type::MINTERVALTYPE:
        s << "mintervaltype";
        break;
    case r_Type::POINTTYPE:
        s << "pointtype";
        break;
    case r_Type::OIDTYPE:
        s << "oidtype";
        break;
    case r_Type::UNKNOWNTYPE:
        s << "unknowntype";
        break;
    default:
        s << "UNKNOWN r_Type_Id" << t;
        break;
    }

    return s;
}
