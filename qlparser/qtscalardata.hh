#ifndef _QTSCALARDATA_HH_
#define _QTSCALARDATA_HH_

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

#include "qlparser/qtdata.hh"
#include "catalogmgr/typefactory.hh"


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
 * COMMENTS:
 *
 ************************************************************/

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

 The abstract class represents a scalar data handled by the
 query tree. It can either be atomic (\Ref{QtAtomicData})
 or complex (\Ref{QtComplexData}).

*/

class QtScalarData : public QtData
{
public:
    /// default constructor
    QtScalarData();

    /// copy constructor
    QtScalarData( const QtScalarData& obj );

    /// virtual destructor
    virtual ~QtScalarData();

    //@Man: Read/Write methods:
    //@{
    ///

    /// returns data type
    virtual QtDataType getDataType() const;

    /// returns true meaning that the data is of type scalar
    virtual bool isScalarData() const;

    /// returns a pointer to the value type
    inline const BaseType* getValueType() const;

    /// sets the value type
    inline void setValueType( const BaseType* newType );

    /// returns a pointer to the internal character representation
    inline const char* getValueBuffer() const;

    /// sets the value buffer
    inline void setValueBuffer( char* buffer );

    /// returns a null-terminated string describing the type structure
    virtual char* getTypeStructure() const;
    /**
      The string pointer has to be free using free() by the caller.
    */

    ///
    //@}

    /// compares data content
    virtual bool equal( const QtData* obj ) const;

    /// returns content dependent string representation
    virtual std::string getSpelling() const;

    /// print status of the object to the specified stream
    virtual void printStatus( std::ostream& stream = std::cout ) const;

protected:
    /// pointer to the value type
    const BaseType* valueType;

    /// pointer to the value buffer
    char* valueBuffer;
};

#include "qlparser/qtscalardata.icc"

#endif






















