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
// This is -*- C++ -*-
/*************************************************************************
 *
 *
 * PURPOSE:
 *
 *
 * COMMENTS:
 *   uses embedded SQL
 *
 ***********************************************************************/

static const char rcsid[] = "@(#)catalogif,TypeFactory: $Header: /home/rasdev/CVS-repository/rasdaman/catalogmgr/typefactory.cc,v 1.19 2003/12/20 23:41:27 rasdev Exp $";

#include <vector>
#include <algorithm>
#include <map>
#include <utility>
#include <set>

#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"
#include "relcatalogif/alltypes.hh"
#include "typefactory.hh"
#include "reladminif/objectbroker.hh"
#include "reladminif/adminif.hh"
#include "reladminif/databaseif.hh"
#include "reladminif/sqlerror.hh"
#include "reladminif/externs.h"
#include "reladminif/dbref.hh"
#include "relcatalogif/dbminterval.hh"
#include "relmddif/mddid.hh"
#include "relmddif/dbmddobj.hh"
#include "relmddif/dbmddset.hh"

TypeFactory* TypeFactory::myInstance = 0;

//This variable is not required since any struct
//type can now be deleted. This resulted as
//the resolution of ticket #88
const short TypeFactory::MaxBuiltInId = 11;

const char* OctetType::Name = "Octet";
const char* UShortType::Name = "UShort";
const char* ShortType::Name = "Short";
const char* ULongType::Name = "ULong";
const char* LongType::Name = "Long";
const char* BoolType::Name = "Bool";
const char* CharType::Name = "Char";
const char* FloatType::Name = "Float";
const char* DoubleType::Name = "Double";
const char* ComplexType1::Name = "Complex1";
const char* ComplexType2::Name = "Complex2";

using namespace std;

// all atomic types given back by mapType()
// for managing the memory of temporary types
std::vector<Type*> *TypeFactory::theTempTypes = 0;

TypeFactory*
TypeFactory::instance()
{
    if(myInstance == 0)
    {
        myInstance = new TypeFactory;
    }
    return myInstance;
}

const BaseType*
TypeFactory::mapType(const char* typeName)
{
    RMDBGENTER(4, RMDebug::module_catalogmgr, "TypeFactory", "mapType(" << typeName << ")");
    BaseType* resultType = 0;
    resultType = (BaseType*)ObjectBroker::getObjectByName(OId::ATOMICTYPEOID, typeName);
    if (resultType == 0)
    {
        try
        {
            resultType = (BaseType*)ObjectBroker::getObjectByName(OId::STRUCTTYPEOID, typeName);
        }
        catch   (r_Error)
        {
            resultType = 0;
        }
    }
    RMDBGEXIT(4, RMDebug::module_catalogmgr, "TypeFactory", "mapType(" << typeName << ") END " << resultType);
    return resultType;
}

const StructType*
TypeFactory::addStructType(const StructType* type)
{
    RMDBGENTER(4, RMDebug::module_catalogmgr, "TypeFactory", "addStructType(" << type->getTypeName() << ")");
    StructType* persistentType = 0;
    const StructType* retval = 0;
    if (type->isPersistent())
    {
        RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "type is persistent " << type->getName() << " " << type->getOId());
        retval = type;
    }
    else
    {
        persistentType = new StructType((char*)type->getTypeName(), type->getNumElems());
        BaseType* t = 0;
        for (int i = 0; i < type->getNumElems(); i++)
        {
            switch (type->getElemType(i)->getType())
            {
            case STRUCT:
                RMDBGMIDDLE(6, RMDebug::module_catalogmgr, "TypeFactory", "element is struct type " << (char*)type->getElemName(i) << " of type " << type->getElemType(i)->getName());
                persistentType->addElement(type->getElemName(i), addStructType((const StructType*)type->getElemType(i)));
                break;
            case ULONG:
            case USHORT:
            case CHAR:
            case BOOLTYPE:
            case LONG:
            case SHORT:
            case OCTET:
            case DOUBLE:
            case FLOAT:
            case COMPLEXTYPE1:
            case COMPLEXTYPE2:
                RMDBGMIDDLE(6, RMDebug::module_catalogmgr, "TypeFactory", "element is atomic type " << (char*)type->getElemName(i) << " of type " << type->getElemType(i)->getName());
                persistentType->addElement(type->getElemName(i), (BaseType*)ObjectBroker::getObjectByOId(type->getElemType(i)->getOId()));
                break;
            default:
                persistentType = 0;
                RMDBGMIDDLE(0, RMDebug::module_catalogmgr, "TypeFactory", "addStructType(" << type->getTypeName() << ") unknown type " << type->getOId() << type->getOId().getType());
                break;
            }
        }
        RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "type is now persistent " << persistentType->getName() << " " << persistentType->getOId());
        persistentType->setCached(true);
        persistentType->setPersistent(true);
        ObjectBroker::registerDBObject(persistentType);
        retval = persistentType;
    }
    RMDBGEXIT(4, RMDebug::module_catalogmgr, "TypeFactory", "addStructType(" << retval->getTypeName() << ") END");
    return retval;
}

const SetType*
TypeFactory::mapSetType(const char* typeName)
{
    RMDBGENTER(4, RMDebug::module_catalogmgr, "TypeFactory", "mapSetType(" << typeName << ")");
    // it is a user defined type
    SetType* resultType = 0;
    try
    {
        resultType = (SetType*)ObjectBroker::getObjectByName(OId::SETTYPEOID, typeName);
    }
    catch   (r_Error)
    {
        resultType = 0;
    }
    RMDBGEXIT(4, RMDebug::module_catalogmgr, "TypeFactory", "mapSetType(" << typeName << ") END " << resultType);
    return resultType;
}

const SetType*
TypeFactory::addSetType(const SetType* type)
{
    RMDBGENTER(4, RMDebug::module_catalogmgr, "TypeFactory", "addSetType(" << type->getTypeName() << ")");
    SetType* persistentType = 0;
    const SetType* retval = 0;
    if (type->isPersistent())
    {
        RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "type is persistent " << type->getName() << " " << type->getOId());
        retval = type;
    }
    else
    {
        persistentType = new SetType((char*)type->getTypeName(), addMDDType(type->getMDDType()));
        persistentType->setPersistent(true);
        RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "type is now persistent " << type->getName() << " " << persistentType->getOId());
        ObjectBroker::registerDBObject(persistentType);
        persistentType->setCached(true);
        retval = persistentType;
    }
    RMDBGEXIT(4, RMDebug::module_catalogmgr, "TypeFactory", "addSetType(" << retval->getTypeName() << ") END");
    return retval;
}

const MDDType*
TypeFactory::mapMDDType(const char* typeName)
{
    RMDBGENTER(4, RMDebug::module_catalogmgr, "TypeFactory", "mapMDDType(" << typeName << ")");
    MDDType* resultType = 0;
    try
    {
        resultType = ObjectBroker::getMDDTypeByName(typeName);
    }
    catch   (...)
    {
        resultType = 0;
    }
    RMDBGEXIT(4, RMDebug::module_catalogmgr, "TypeFactory", "mapMDDType(" << typeName << ") END " << resultType);
    return resultType;
}

const MDDType*
TypeFactory::addMDDType(const MDDType* type)
{
    RMDBGENTER(4, RMDebug::module_catalogmgr, "TypeFactory", "addMDDType(" << type->getTypeName() << ")");
    MDDType* persistentType = 0;
    const MDDType* retval = 0;
    BaseType* t = 0;

    if (type->isPersistent())
    {
        RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "type is persistent " << type->getOId());
        retval = type;
    }
    else
    {
        RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "type is not persistent " << type->getOId());
        switch (type->getSubtype())
        {
        case MDDType::MDDONLYTYPE:
            RMDBGMIDDLE(7, RMDebug::module_catalogmgr, "TypeFactory", "is MDDONLYTYPE");
            persistentType = new MDDType(type->getTypeName());
            break;
        case MDDType::MDDBASETYPE:
            RMDBGMIDDLE(7, RMDebug::module_catalogmgr, "TypeFactory", "is MDDBASETYPE");
            persistentType = new MDDBaseType(type->getTypeName(), addStructType((StructType*)((MDDBaseType*)type)->getBaseType()));
            break;
        case MDDType::MDDDOMAINTYPE:
            RMDBGMIDDLE(7, RMDebug::module_catalogmgr, "TypeFactory", "is MDDDOMAINTYPE");
            persistentType = new MDDDomainType(type->getTypeName(), addStructType((StructType*)((MDDBaseType*)type)->getBaseType()), *((MDDDomainType*)type)->getDomain());
            break;
        case MDDType::MDDDIMENSIONTYPE:
            RMDBGMIDDLE(7, RMDebug::module_catalogmgr, "TypeFactory", "is MDDDIMENSIONTYPE");
            persistentType = new MDDDimensionType(type->getTypeName(), addStructType((StructType*)((MDDBaseType*)type)->getBaseType()), ((MDDDimensionType*)type)->getDimension());
            break;
        default:
            RMDBGMIDDLE(0, RMDebug::module_catalogmgr, "TypeFactory", "addMDDType(" << type->getName() << ") mddsubtype unknown");
            break;
        }
        if (persistentType != 0)
        {
            persistentType->setPersistent(true);
            RMDBGMIDDLE(6, RMDebug::module_catalogmgr, "TypeFactory", "adding " << persistentType->getName() << " " << persistentType->getOId());
            persistentType->setCached(true);
            ObjectBroker::registerDBObject(persistentType);
            retval = persistentType;
        }
        else
        {
            //error message was already given in switch default
        }
    }
    RMDBGEXIT(4, RMDebug::module_catalogmgr, "TypeFactory", "addMDDType(" << type->getTypeName() << ") END");
    return retval;
}

Type*
TypeFactory::addTempType(Type* type)
{
    // put in front to avoid deletion of MDDTypes still referenced
    // by an MDDBaseType.
    theTempTypes->insert(theTempTypes->begin(), type);
    return type;
}

void
TypeFactory::initialize()
{
    // to initailize the typefactory
    RMDBGENTER(4, RMDebug::module_catalogmgr, "TypeFactory", "initialize()");
    if (!theTempTypes)
        theTempTypes = new std::vector<Type*>;

    RMDBGEXIT(4, RMDebug::module_catalogmgr, "TypeFactory", "initialize() END");
}

void
TypeFactory::freeTempTypes()
{
    RMDBGENTER(4, RMDebug::module_catalogmgr, "TypeFactory", "freeTempTypes()");

    // delete all temporary types
    if (theTempTypes)
    {
        for(std::vector<Type*>::iterator iter = theTempTypes->begin(); iter != theTempTypes->end(); iter++)
        {
            delete *iter;
            *iter = 0;
        }
        delete theTempTypes;
        theTempTypes = 0;
    }
    RMDBGEXIT(4, RMDebug::module_catalogmgr, "TypeFactory", "freeTempTypes()");
}

TypeFactory::TypeFactory()
{
}

void
TypeFactory::deleteStructType(const char* typeName)
{
    RMDBGENTER(4, RMDebug::module_catalogmgr, "TypeFactory", "deleteStructType(" << typeName << ")");
    const DBObject* resultType = mapType(typeName);
    if (resultType)
    {
        bool canDelete = true;
        for (TypeIterator<MDDType> miter = createMDDIter(); miter.not_done(); miter.advance())
        {
            if (miter.get_element()->getSubtype() != MDDType::MDDONLYTYPE)
            {
                if (((MDDBaseType*)(miter.get_element().ptr()))->getBaseType() == resultType)
                {
                    RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "mdd type " << miter.get_element()->getName() << " contains " << typeName);
                    canDelete = false;
                    break;
                }
            }
        }
        if (canDelete)
        {
            DBObjectId toKill(resultType->getOId());
            toKill->setPersistent(false);
            toKill->setCached(false);
            RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "will be deleted from db");
        }
        else
        {
            RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "will not be deleted from db");
        }
    }
    else
    {
        RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "is not in map");
    }
    RMDBGEXIT(4, RMDebug::module_catalogmgr, "TypeFactory", "deleteStructType(" << typeName << ")");
}

void
TypeFactory::deleteMDDType(const char* typeName)
{
    RMDBGENTER(4, RMDebug::module_catalogmgr, "TypeFactory", "deleteMDDType(" << typeName << ")");
    const MDDType* resultType = mapMDDType(typeName);  //is ok because only short for find
    if (resultType)
    {
        bool canDelete = true;
        for (TypeIterator<SetType> miter = createSetIter(); miter.not_done(); miter.advance())
        {
            if (miter.get_element()->getMDDType() == resultType)
            {
                RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "set type " << miter.get_element()->getName() << " contains " << typeName);
                canDelete = false;
                break;
            }
        }
        if (canDelete)
        {
            if (resultType->getSubtype() != MDDType::MDDONLYTYPE)
            {
                //mdd only types can not be in mdd objects
                OIdSet* theList = ObjectBroker::getAllObjects(OId::MDDOID);
                for (OIdSet::iterator miter = theList->begin(); miter != theList->end(); miter++)
                {
                    if (DBMDDObjId(*miter)->getMDDBaseType() == resultType)
                    {
                        RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "mdd object " << *miter << " contains " << typeName);
                        canDelete = false;
                        break;
                    }
                }
                delete theList;
                theList = 0;
            }
            if (canDelete)
            {
                DBObjectId toKill(resultType->getOId());
                toKill->setPersistent(false);
                toKill->setCached(false);
                RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "will be deleted from db");
            }
            else
            {
                RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "will not be deleted from db");
            }
        }
        else
        {
            RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "will not be deleted from db");
        }
    }
    else
    {
        RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "is not in map");
    }
    RMDBGEXIT(4, RMDebug::module_catalogmgr, "TypeFactory", "deleteMDDType(" << typeName << ") END");
}

void
TypeFactory::deleteSetType(const char* typeName)
{
    RMDBGENTER(4, RMDebug::module_catalogmgr, "TypeFactory", "deleteSetType(" << typeName << ")");
    const DBObject* resultType = (SetType*)mapSetType(typeName);//is ok because only short for find
    if (resultType)
    {
        bool canDelete = true;
        OIdSet* theList = ObjectBroker::getAllObjects(OId::MDDCOLLOID);
        for (OIdSet::iterator miter = theList->begin(); miter != theList->end(); miter++)
        {
            if (DBMDDSetId(*miter)->getCollType() == resultType)
            {
                RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "set object " << *miter << " contains " << typeName);
                canDelete = false;
                break;
            }
        }
        delete theList;
        theList = 0;
        if (canDelete)
        {
            DBObjectId toKill(resultType->getOId());
            toKill->setPersistent(false);
            toKill->setCached(false);
            RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "will be deleted from db");
        }
        else
        {
            RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "will not be deleted from db");
        }
    }
    else
    {
        RMDBGMIDDLE(5, RMDebug::module_catalogmgr, "TypeFactory", "is not in map");
    }
    RMDBGEXIT(4, RMDebug::module_catalogmgr, "TypeFactory", "deleteSetType(" << typeName << ") END");
}

const Type*
TypeFactory::ensurePersistence(Type* type)
{
    std::vector<Type*>::iterator iter;
    const Type* retval = 0;
    Type* ttype = 0;

    // deleting type if it is in the list of tempTypes
    if(theTempTypes)
    {
        for(iter = theTempTypes->begin(); iter < theTempTypes->end(); iter++)
        {
            if(*iter == type)
            {
                theTempTypes->erase(iter);
            }
        }
    }
    // check if the struct type is alread in the DBMS
    switch (type->getType())
    {
    case STRUCT:
    {
        TypeIterator<StructType> ist = createStructIter();
        while (ist.not_done())
        {
            ttype = ist.get_element();
            if (ttype->compatibleWith(type))
            {
                retval = ttype;
                break;
            }
            ist.advance();
        }
        if (!retval)
            retval = addStructType((const StructType*)type);
    }
    break;
    case MDDTYPE:
    {
        TypeIterator<MDDType> imd = createMDDIter();
        while (imd.not_done())
        {
            ttype = imd.get_element();
            if (ttype->compatibleWith(type))
            {
                retval = ttype;
                break;
            }
            imd.advance();
        }
        if (!retval)
            retval = addMDDType((const MDDType*)type);
    }
    break;
    case SETTYPE:
    {
        TypeIterator<SetType> ise = createSetIter();
        while (ise.not_done())
        {
            ttype = ise.get_element();
            if (ttype->compatibleWith(type))
            {
                retval = ttype;
                break;
            }
            ise.advance();
        }
        if (!retval)
            retval = addSetType((const SetType*)type);
    }
    break;
    case ULONG:
        retval = (Type*)ObjectBroker::getObjectByOId(OId(ULONG, OId::ATOMICTYPEOID));
        break;
    case USHORT:
        retval = (Type*)ObjectBroker::getObjectByOId(OId(USHORT, OId::ATOMICTYPEOID));
        break;
    case CHAR:
        retval = (Type*)ObjectBroker::getObjectByOId(OId(CHAR, OId::ATOMICTYPEOID));
        break;
    case BOOLTYPE:
        retval = (Type*)ObjectBroker::getObjectByOId(OId(BOOLTYPE, OId::ATOMICTYPEOID));
        break;
    case LONG:
        retval = (Type*)ObjectBroker::getObjectByOId(OId(LONG, OId::ATOMICTYPEOID));
        break;
    case SHORT:
        retval = (Type*)ObjectBroker::getObjectByOId(OId(SHORT, OId::ATOMICTYPEOID));
        break;
    case OCTET:
        retval = (Type*)ObjectBroker::getObjectByOId(OId(OCTET, OId::ATOMICTYPEOID));
        break;
    case DOUBLE:
        retval = (Type*)ObjectBroker::getObjectByOId(OId(DOUBLE, OId::ATOMICTYPEOID));
        break;
    case FLOAT:
        retval = (Type*)ObjectBroker::getObjectByOId(OId(FLOAT, OId::ATOMICTYPEOID));
        break;
    case COMPLEXTYPE1:
        retval = (Type*)ObjectBroker::getObjectByOId(OId(COMPLEXTYPE1, OId::ATOMICTYPEOID));
        break;
    case COMPLEXTYPE2:
        retval = (Type*)ObjectBroker::getObjectByOId(OId(COMPLEXTYPE2, OId::ATOMICTYPEOID));
        break;
    default:
        RMDBGONCE(0, RMDebug::module_catalogmgr, "TypeFactory", "ensurePersitence() is not a STRUCT/MDDTYPE/SETTYPE/ATOMIC " << type->getName());
        break;
    }
    if (!type->isPersistent())
        delete type;
    return retval;
}

TypeIterator<SetType>
TypeFactory::createSetIter()
{
    RMDBGONCE(1, RMDebug::module_catalogmgr, "TypeFactory", "createSetIter()");
    OIdSet* t = ObjectBroker::getAllObjects(OId::SETTYPEOID);
    TypeIterator<SetType> ti(*t);
    delete t;
    t = 0;
    return ti;
}

TypeIterator<StructType>
TypeFactory::createStructIter()
{
    RMDBGONCE(1, RMDebug::module_catalogmgr, "TypeFactory", "createStructIter()");
    OIdSet* t = ObjectBroker::getAllObjects(OId::STRUCTTYPEOID);
    TypeIterator<StructType> ti(*t);
    delete t;
    t = 0;
    return ti;
}

TypeIterator<MDDType>
TypeFactory::createMDDIter()
{
    RMDBGENTER(1, RMDebug::module_catalogmgr, "TypeFactory", "createMDDIter()");
    OIdSet theMDDTypes;
    OIdSet* tempList = 0;
    OIdSet::iterator i;

    tempList = ObjectBroker::getAllObjects(OId::MDDTYPEOID);
    theMDDTypes = *tempList;
    delete tempList;

    tempList = ObjectBroker::getAllObjects(OId::MDDBASETYPEOID);
    while (!tempList->empty())
    {
        theMDDTypes.insert(*(tempList->begin()));
        tempList->erase(*(tempList->begin()));
    }
    delete tempList;

    tempList = ObjectBroker::getAllObjects(OId::MDDDIMTYPEOID);
    while (!tempList->empty())
    {
        theMDDTypes.insert(*(tempList->begin()));
        tempList->erase(*(tempList->begin()));
    }
    delete tempList;

    tempList = ObjectBroker::getAllObjects(OId::MDDDOMTYPEOID);
    while (!tempList->empty())
    {
        theMDDTypes.insert(*(tempList->begin()));
        tempList->erase(*(tempList->begin()));
    }
    delete tempList;

    RMDBGEXIT(1, RMDebug::module_catalogmgr, "TypeFactory", "createMDDIter()");
    return TypeIterator<MDDType>(theMDDTypes);
}


