#include "mymalloc/mymalloc.h"
// This is -*- C++ -*-
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
 *  code common to all database interface implementations
 */
#include "structtype.hh"
#include "catalogmgr/typefactory.hh"
#include <iomanip>
#include <stdlib.h>
#include "reladminif/sqlerror.hh"
#include "reladminif/externs.h"
#include "raslib/rmdebug.hh"
#include "reladminif/objectbroker.hh"
#include "raslib/error.hh"

#include <cstring>

/*************************************************************
 * Method name...: StructType();
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: initializes member variables for an
 *                 StructType.
 ************************************************************/

r_Bytes
StructType::getMemorySize() const
{
    r_Bytes retval = DBNamedObject::getMemorySize() + sizeof(int) + sizeof(int) + sizeof(std::vector< BaseType* >) + sizeof(std::vector< unsigned int >) + sizeof(std::vector< char* >) + sizeof(int) * numElems + sizeof(BaseType*) * numElems;
    for (int i = 0; i < numElems; i++)
        retval = retval + 1 + strlen(elementNames[i]);
    return retval;
}

StructType::StructType()
    :   CompositeType("unnamed structtype", 0),
        numElems(0),
        elements(0),
        elementNames(0),
        elementOffsets(0),
        align(1)
{
    myType = STRUCT;
    objecttype = OId::STRUCTTYPEOID;
}

StructType::StructType(const char* newTypeName, unsigned int numElem )
    :   CompositeType(newTypeName, 0),
        elements(numElem),
        elementNames(numElem),
        elementOffsets(numElem),
        numElems(0),
        align(1)
{
    myType = STRUCT;
    objecttype = OId::STRUCTTYPEOID;
}

/*************************************************************
 * Method name...: StructType(const StructType& old);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

StructType::StructType(const StructType& old)
    :   CompositeType(old)
{
    elements = old.elements;
    elementNames = old.elementNames;
    elementOffsets = old.elementOffsets;
    numElems = old.numElems;
    align = old.align;
}

StructType::StructType(const OId& structtypeid) throw (r_Error)
    :   CompositeType(structtypeid),
        elements(0),
        elementNames(0),
        elementOffsets(0),
        numElems(0),
        align(1)
{
    RMDBGENTER(4, RMDebug::module_catalogif, "StructType", "StructType(" << myOId << ")");
    myType = STRUCT;
    objecttype = OId::STRUCTTYPEOID;
    readFromDb();
    RMDBGEXIT(4, RMDebug::module_catalogif, "StructType", "StructType(" << myOId << ")");
}

/*************************************************************
 * Method name...: operator=(const StructType&);
 *
 * Arguments.....: none
 * Return value..: none
 * Description...: copy constructor
 ************************************************************/

StructType&
StructType::operator=(const StructType& old)
{
    // Gracefully handle self assignment
    //FIXME memory leak with char* in elementNames and elements
    if (this == &old)
        return *this;
    CompositeType::operator=(old);
    elements = old.elements;
    elementNames = old.elementNames;
    elementOffsets = old.elementOffsets;
    numElems = old.numElems;
    align = old.align;
    return *this;
}

StructType::~StructType()
{
    ObjectBroker::deregisterDBObject(myOId);
    validate();
    for (int i = 0; i < getNumElems(); i++)
        free((void*)elementNames[i]);//is ok because noone is using it
}

/*************************************************************
 * Method name...: void printCell( ostream& stream,
 *                                 const char* cell )
 *
 * Arguments.....:
 *   stream: stream to print on
 *   cell:   pointer to cell to print
 * Return value..: none
 * Description...: prints a cell cell in hex on stream
 *                 followed by a space.
 *                 Assumes that Struct is stored MSB..LSB
 *                 on HP.
 ************************************************************/

void
StructType::printCell( ostream& stream, const char* cell ) const
{
    int i;

    stream << "\t|";
    for(i=0; i<numElems; i++)
    {
        stream << elementNames[i] << "\t: ";
        elements[i]->printCell(stream, cell + elementOffsets[i]);
        stream << "\t ";
    }
    stream << "\t| ";
}

char*
StructType::getTypeStructure() const
{
    // this implementation is not very clever, perhaps should use
    // an intelligent string class
    char* result = (char*)mymalloc(10);
    char* newResult;
    int i;

    strcpy(result, "struct { ");
    if(numElems == 0)
    {
        newResult = (char*)mymalloc(strlen(result) + 1 + 2);
        strcpy(newResult, result);
        strcat(newResult, " }");
        free(result);
        return newResult;
    }
    for(i = 0; i < numElems; i++)
    {
        char* dummy = elements[i]->getTypeStructure();
        newResult = (char*)mymalloc(strlen(result) +
                                    strlen(elementNames[i]) +
                                    strlen(dummy) + 1 + 3 );
        strcpy(newResult, result);
        strcat(newResult, dummy);
        strcat(newResult, " ");
        strcat(newResult, elementNames[i]);
        strcat(newResult, ", ");
        free(result);
        free(dummy);
        result = newResult;
    }
    newResult = (char*)mymalloc(strlen(result) + 1 );
    strcpy(newResult, result);
    newResult[strlen(newResult) - 2] = ' ';
    newResult[strlen(newResult) - 1] = '}';
    free(result);
    result = newResult;
    return result;
}

unsigned int
StructType::addElement(const char* elemName, const char* elemType)
{
    BaseType* stuff = 0;
    stuff = (BaseType*)ObjectBroker::getObjectByName(OId::ATOMICTYPEOID, elemType);
    if (stuff == 0)
        stuff = (BaseType*)ObjectBroker::getObjectByName(OId::STRUCTTYPEOID, elemType);
    return addElement(elemName, stuff);
}

unsigned int
StructType::addElement(const char* elemName, const BaseType* newType)
{
    if (newType)
    {
        if (!_isPersistent)
        {
            addElementPriv(elemName, newType);
        }
    }
    return numElems;
}

unsigned int
StructType::addElementPriv(const char* elemName, const BaseType* newType)
{
    int currPos = 0;
    int i, j;
    char* myElemName = 0;

    myElemName = (char*)mymalloc(strlen(elemName) + 1);
    strcpy(myElemName, elemName);

    if(numElems+1 > elements.size())
    {
        BaseType* dummyB = 0;
        char* dummyN = 0;
        unsigned int dummyO = 0;
        elements.push_back(dummyB);
        elementNames.push_back(dummyN);
        elementOffsets.push_back(dummyO);
    }
    if(numElems == 0)
    {
        // first element
        elementOffsets[currPos] = 0;
        ++numElems;
    }
    else
    {
        // All cases have to set currPos and numElems correctly!
        // The array has to be ordered by offsets.
        if(newType->getType() == STRUCT)
        {
            unsigned int myAlign = ((StructType*)(newType))->getAlignment();
            if( align < myAlign )
                align = myAlign;
            // append at the end, align offset to 4 bytes.
            currPos = numElems;
            ++numElems;
            elementOffsets[currPos] = ((elementOffsets[currPos-1] +
                                        elements[currPos-1]->getSize()-1)/myAlign+1)*myAlign;
        }
        else
        {
            if(newType->getSize() >= 4)
            {
                if(align < 4)
                    align = 4;
                // append at the end, align offset to 4 bytes.
                currPos = numElems;
                ++numElems;
                elementOffsets[currPos] = ((elementOffsets[currPos-1] +
                                            elements[currPos-1]->getSize()-1)/4+1)*4;
            }
            else
            {
                if(newType->getSize() == 2)
                {
                    if(align < 2)
                        align = 2;
                    currPos = numElems;
                    numElems++;
                    elementOffsets[currPos] = ((elementOffsets[currPos-1] +
                                                elements[currPos-1]->getSize()-1)/2+1)*2;
                }
                else
                {
                    if(newType->getSize() == 1)
                    {
                        currPos = numElems;
                        numElems++;
                        elementOffsets[currPos] = elementOffsets[currPos-1] +
                                                  elements[currPos-1]->getSize();
                    }
                    else
                    {
                        RMDBGONCE(0, RMDebug::module_catalogif,"StructType", "addType() ERROR!");
                        // for debugging purposes only, shouldn't happen.
                    }
                }
            }
        }
    }
    // actually insert type and name of element
    elements[currPos] = newType;
    elementNames[currPos] = myElemName;
    // calculate current size
    calcSize();
    return numElems;
}

unsigned int
StructType::getOffset(const char* elemName) const
{
    RMDBGENTER(6, RMDebug::module_catalogif, "StructType", "getOffset(" << elemName << ") " << getName() << " " << myOId);
    int i;
    unsigned int retval = 0;
    bool found = false;
    for(i=0; i<numElems; i++)
    {
        if(strcmp(elementNames[i], elemName) == 0)
        {
            retval = elementOffsets[i];
            found = true;
            break;
        }
    }
    RMDBGIF(0, RMDebug::module_catalogif, "StructType", \
            if (found == false) \
{
    \
    RMDBGONCE(0, RMDebug::module_catalogif, "StructType", "ERROR in StructType::getOffset(" << elemName << ") name not found " << getName() << " " << myOId  << " retval " << retval); \
        throw r_Error(STRUCTTYPE_ELEMENT_UNKNOWN); \
    } )
    RMDBGEXIT(6, RMDebug::module_catalogif, "StructType", "getOffset(" << elemName << ") " << getName() << " " << myOId << " retval " << retval);
    return retval;
    // should raise exception!
}

unsigned int
StructType::getOffset(unsigned int num) const
{
    if (num >= numElems)
    {
        RMDBGONCE(0, RMDebug::module_catalogif, "StructType", "ERROR in StructType::getOffset(" << num << ") offset out of bounds " << getName() << " retval " << 0);
        RMDBGIF(0, RMDebug::module_catalogif, "StructType", throw r_Error(STRUCTTYPE_ELEMENT_OUT_OF_BOUNDS); )
        return 0;
    }
    return elementOffsets[num];
}

const BaseType*
StructType::getElemType(const char* elemName) const
{
    RMDBGENTER(6, RMDebug::module_catalogif, "StructType", "getElemType(" << elemName << ") " << getName() << " " << myOId);
    const BaseType* retval = 0;
    int i;

    for(i=0; i<numElems; i++)
    {
        if(strcmp(elementNames[i], elemName) == 0)
        {
            retval = elements[i];
            break;
        }
    }
    RMDBGIF(0, RMDebug::module_catalogif, "StructType", \
            if (retval == 0) \
{
    \
    RMDBGMIDDLE(0, RMDebug::module_catalogif, "StructType", "ERROR in StructType::getElemType(" << elemName << ") name not found " << getName() << " " << myOId  << " retval " << retval); \
        throw r_Error(STRUCTTYPE_ELEMENT_UNKNOWN); \
    } )
    RMDBGEXIT(6, RMDebug::module_catalogif, "StructType", "getElemType(" << elemName << ") " << getName() << " " << myOId << " retval " << retval);
    return retval;
}

const BaseType*
StructType::getElemType(unsigned int num) const
{
    if(!(num < numElems))
    {
        RMDBGONCE(0, RMDebug::module_catalogif, "StructType", "ERROR in StructType::getElemType(" << num << ") offset out of bounds " << getName() << " retval " << 0);
        RMDBGIF(0, RMDebug::module_catalogif, "StructType", throw r_Error(STRUCTTYPE_ELEMENT_OUT_OF_BOUNDS); )
        return 0;
    }
    return elements[num];
}

const char* const
StructType::getElemName(unsigned int num) const
{
    if(!(num < numElems))
    {
        RMDBGONCE(0, RMDebug::module_catalogif, "StructType", "ERROR in StructType::getElemName(" << num << ") offset out of bounds " << getName() << " retval " << 0);
        RMDBGIF(0, RMDebug::module_catalogif, "StructType", throw r_Error(STRUCTTYPE_ELEMENT_OUT_OF_BOUNDS); )
        return 0;
    }
    return elementNames[num];
}

unsigned int
StructType::getNumElems() const
{
    return numElems;
}

unsigned int
StructType::getAlignment() const
{
    return align;
}

int
StructType::contains(const StructType* aStruct) const
{
    int i;

    for(i=0; i<numElems; i++)
    {
        if(elements[i] == aStruct)
            return 1;
        else if(elements[i]->getType() == STRUCT)
            if(((StructType*)elements[i])->contains(aStruct))
                return 1;
    }
    return 0;
}

int
StructType::compatibleWith(const Type* aType) const
{
    RMDBGENTER(7, RMDebug::module_catalogif, "StructType", "compatibleWith(" << aType->getName() << ") " << getName());
    int retval;
    if(aType->getType() != STRUCT)
    {
        RMDBGMIDDLE(8, RMDebug::module_catalogif, "StructType", "no structtype");
        retval = 0;
    }
    else
    {
        if(elements.size() != ((StructType*)aType)->elements.size())
        {
            RMDBGMIDDLE(8, RMDebug::module_catalogif, "StructType", "not the same size");
            retval = 0;
        }
        else
        {
            const BaseType* myBaseType;
            const BaseType* otherBaseType;

            int i;

            retval = 1;
            for( i = 0; i < elements.size(); i++ )
            {
                myBaseType = elements[i];
                otherBaseType = ((StructType*)aType)->elements[i];
                if(!myBaseType->compatibleWith(otherBaseType))
                {
                    RMDBGMIDDLE(8, RMDebug::module_catalogif, "StructType", i << ". element " << otherBaseType->getName() << " does not match " << myBaseType);
                    retval = 0;
                    break;
                }
            }
        }
    }
    RMDBGEXIT(7, RMDebug::module_catalogif, "StructType", "compatibleWith(" << aType->getName() << ") " << getName() << " retval " << retval);
    return retval;
}

void
StructType::calcSize()
{
    int alignSize = 1;

    // check for alignment of size
    for(int i=0; i<numElems; i++)
    {
        if( elements[i]->getSize() == 4 || elements[i]->getType() == STRUCT )
        {
            alignSize = 4;
            break;
        }
        else
        {
            if( elements[i]->getSize() == 2 )
            {
                alignSize = 2;
            }
        }
    }
    // align size to alignSize bytes (there may be unused bytes at the end!)
    size = ((elementOffsets[numElems-1]+elements[numElems-1]->getSize()-1)/alignSize+1)*alignSize;
}

