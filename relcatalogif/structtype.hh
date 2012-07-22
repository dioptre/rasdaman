// -*-C++-*- (for Emacs)

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
 *   The StructType class is the superclass for all classes
 *   describing the type of a cell
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _STRUCTTYPE_HH_
#define _STRUCTTYPE_HH_

#include <iostream>
#include <vector>

#include "compositetype.hh"
#include "catalogmgr/ops.hh"

class BaseType;

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:
StructType is the base type used for user defined structures. It
stores the names and BaseTypes of the elements. The size of a
StructType in chars depends on the elements.

StructType now uses alignment for structures with elements of
different sizes. One byte types are aligned to one byte, two byte
types to two, all other types to four. The Size is padded to 4 byte
alignment except for types with only chars (one byte alignment) or
type with only shorts (two byte alignment). Structs as elements of
structs are aligned with the minimum byte alignment needed for the
struct.

*/

/**
  * \ingroup Relcatalogifs
  */
class StructType : public CompositeType
{
    //  friend ostream& operator << (ostream& stream, StructType& b);

public:
    virtual void printCell(ostream& stream, const char* cell) const;

    virtual char* getTypeStructure() const;

    /// add new element to struct
    unsigned int addElement(const char* elemName, const char* elemType);

    /// add new element to struct using pointer to BaseType
    unsigned int addElement(const char* elemName, const BaseType* elemType);

    /// get offset for an element by name of element.
    unsigned int getOffset(const char* elemName) const;

    /// get offset for an element by number of element (0 based).
    unsigned int getOffset(unsigned int num) const;

    /// get type of an element by name of element.
    const BaseType* getElemType(const char* elemName) const;

    /// get name of an element by number of element (0 based).
    const char* const getElemName(unsigned int num) const;

    /// get type of an element by number of element (0 based).
    const BaseType* getElemType(unsigned int num) const;

    /// get number of elements.
    unsigned int getNumElems() const;

    /// get alignment needed for structure to be embedded in another structure.
    unsigned int getAlignment() const;

    /// checks if a certain StructType is contained in this StructType
    int contains(const StructType* aStruct) const;

    StructType(const OId& structtypeid) throw (r_Error);

    /// default constructor, sets type name to "".
    StructType();

    /// constructor getting type name and number of elements.
    StructType(const char* newTypeName, unsigned int numElem );

    /// copy constructor.
    StructType( const StructType& old );

    /// assignment operator.
    StructType& operator=( const StructType& old );

    /// virtual destructor.
    virtual ~StructType();

    virtual int compatibleWith(const Type* aType) const;

    virtual r_Bytes getMemorySize() const;

private:
    // those inherited from BaseType aren't useful at all for StructType
    // made them private to preven calling them
    virtual r_ULong* convertToCULong(const char* cell, r_ULong* value) const;
    virtual char* makeFromCULong(char* cell, const r_ULong* value) const;
    virtual r_Long* convertToCLong(const char* cell, r_Long* value) const;
    virtual char* makeFromCLong(char* cell, const r_Long* value) const;
    virtual double* convertToCDouble(const char* cell, double* value) const;
    virtual char* makeFromCDouble(char* cell, const double* value) const;


protected:

    virtual void insertInDb() throw (r_Error);

    virtual void deleteFromDb() throw (r_Error);

    virtual void readFromDb() throw (r_Error);

    // moves back one step all elements all elements behind pos
    void moveBack(int pos);

    // calculates and sets current size of type with alignment
    void calcSize();

    /// Array containing references to base types of elements.
    std::vector< const BaseType* > elements;

    /// Array containing names of elements.
    std::vector< char* > elementNames;

    /// Array containing offsets to elements
    std::vector< unsigned int > elementOffsets;

    /// The number of elements.
    unsigned int numElems;

    /// Alignment needed for structure if embedded in other structures.
    unsigned int align;

    /// add new element to struct using pointer to BaseType
    /// does the actuall adding.  the public method will not let a persitent type
    /// be changed
    unsigned int addElementPriv(const char* elemName, const BaseType* elemType);
};

#include "structtype.icc"

#endif
