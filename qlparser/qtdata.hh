#ifndef _QTDATA_
#define _QTDATA_

#include <iostream>

#include "qlparser/parseinfo.hh"
#include "raslib/rminit.hh"

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

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

//@Man: TypeEnum
//@Type: typedef
//@Memo: Module: {\bf qlparser}.

enum QtDataType
{
    QT_TYPE_UNKNOWN,
    QT_BOOL,
    QT_CHAR,
    QT_OCTET,
    QT_USHORT,
    QT_SHORT,
    QT_ULONG,
    QT_LONG,
    QT_FLOAT,
    QT_DOUBLE,
    QT_COMPLEXTYPE1,  // with float members
    QT_COMPLEXTYPE2,  // with double
    QT_MDD,
    QT_MDD_CONFIG,
    QT_COMPLEX,
    QT_STRING,
    QT_INTERVAL,
    QT_MINTERVAL,
    QT_POINT
};




//@ManMemo: Module: {\bf qlparser}

/*@Doc:

This class is superclass for the classes representing a
specific data type handled by the query tree.

*/

class QtData
{
public:
    // construktor
    QtData();

    // constructor
    QtData( const std::string name );

    /// copy constructor
    QtData( const QtData &obj );

    /// destructor
    virtual ~QtData();

    /// assignement operator
    const QtData& operator=( const QtData& obj );

    enum QtLifetime
    {
        QT_PERSISTENT,
        QT_TRANSIENT
    };

    /// returns type of data represented by the QtData object
    virtual QtDataType getDataType() const = 0;
    /**
      \begin{tabular}{lll}
      QtMDD&&QT_MDD\\
      QtAtomicData&&QT_BOOL\\
      &&QT_CHAR\\
      &&QT_OCTET\\
      &&QT_USHORT\\
      &&QT_SHORT\\
      &&QT_ULONG\\
      &&QT_FLOAT\\
      &&QT_DOUBLE\\
      QtComplexData&&QT_COMPLEX\\
      QtString&&QT_STRING\\
      QtInterval&&QT_INTERVAL\\
      QtMinterval&&QT_MINTERVAL\\
      QtPoint&&QT_POINT\\
      \end{tabular}
    */

    /// returns a null-terminated string describing the type structure
    virtual char* getTypeStructure() const = 0;
    /**
      The string pointer has to be free using free() by the caller.
    */

    /// determines, if the data is of type scalar
    virtual bool isScalarData() const;

    /// compares data content
    virtual bool equal( const QtData* obj ) const = 0;

    /// returns content dependent string representation
    virtual std::string getSpelling() const = 0;

    //@Man: Read/Write methods:
    //@{
    ///

    /// get the iterator name
    inline std::string const getIteratorName() const;
    ///
    inline void setIteratorName( const std::string & iteratorNameNew );
    ///
    inline QtLifetime const getLifetime() const;
    ///
    inline void setLifetime( QtLifetime flag );
    /// return number of references to the self object
    inline unsigned int getRefNo() const;
    /// increases the reference counter by one
    inline void incRef();
    /// deletes one reference and the object if it was the last one
    inline int  deleteRef();
    /**
       In case the object is deleted, 1 is returned, 0 otherwise.
    */

    ///
    inline ParseInfo getParseInfo();
    ///
    inline void      setParseInfo( const ParseInfo &info );

    ///
    //@}

    /// print status of the object to the specified stream
    virtual void printStatus( std::ostream& stream = std::cout ) const;

protected:
    /// pointer to an optional parser info
    ParseInfo* parseInfo;

private:
    /// decreases the reference counter by one and returns the new value
    inline unsigned int decRef();

    /// attribute storing the iterator name
    std::string iteratorName;

    /// attribute storing the lifetime (persistent, transient)
    QtLifetime persistent;
    /**
      Objects of subclasses of QtScalarData (QtAtomicData and QtComplexdata) are always transient.
      Objects of subclass QtMDD are persistent or transient.
    */

    /// counts the number of references to the data object
    unsigned int referenceCounter;
};

#include "qlparser/qtdata.icc"

#endif

