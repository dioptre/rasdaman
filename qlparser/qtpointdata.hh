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

#ifndef _QTPOINTDATA_
#define _QTPOINTDATA_

#include "qlparser/qtdata.hh"

#include "raslib/point.hh"

#include <iostream>
#include <string>
#include <vector>

// removed deprecated ostrsteam -- PB 2005-jan-14
// #include <strstream.h>

//@ManMemo: Module: {\bf qlparser}

/*@Doc:
  The class encapsulates a point of type \Ref{r_Point}.
*/

class QtPointData : public QtData
{
public:
    /// constructor getting a point
    QtPointData( const r_Point& point );

    /// virtual destructor
    virtual ~QtPointData();

    //@Man: Read/Write methods:
    //@{
    ///

    ///
    inline const r_Point&  getPointData() const;
    ///
    inline void            setPointData( const r_Point& point );

    /// returns a null-terminated string describing the type structure
    virtual char* getTypeStructure() const;
    /**
      The string pointer has to be free using free() by the caller.
    */

    ///
    //@}

    /// returns {\tt QT_POINT}
    virtual QtDataType getDataType() const;

    /// compares data content
    virtual bool equal( const QtData* obj ) const;

    /// returns content dependent string representation
    virtual std::string getSpelling() const;

    /// print status of the object to the specified stream
    virtual void printStatus( std::ostream& stream = std::cout ) const;

private:
    /// attribute storing the minterval
    r_Point pointData;
};

#include "qlparser/qtpointdata.icc"

#endif





