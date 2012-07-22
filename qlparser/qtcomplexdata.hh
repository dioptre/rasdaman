#ifndef _QTCOMPLEXDATA_HH
#define _QTCOMPLEXDATA_HH

#include "qlparser/qtscalardata.hh"

#include <list>

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

/**

 The class represents a complex scalar value handled by the query tree.

*/

class QtComplexData : public QtScalarData
{
public:
    /// list of \Ref{QtScalarData} objects
    typedef std::list<QtScalarData*> QtScalarDataList;

    /// default constructor
    QtComplexData();

    /// constructor getting a pointer to a list of \Ref{QtScalarData} objects
    QtComplexData( QtScalarDataList* &scalarDataList );
    /**
      Construct an object with \Ref{QtScalarData} elements of the list and
      destroy the list afterwards. {\ttscalarDataList} is set to zero.
    */

    /// copy constructor
    QtComplexData( const QtComplexData& obj );

    /// print status of the object to the specified stream
    virtual void printStatus( std::ostream& stream = std::cout ) const;
};

#include "qlparser/qtcomplexdata.icc"

#endif






















