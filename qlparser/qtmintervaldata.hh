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

#ifndef _QTMINTERVALDATA_
#define _QTMINTERVALDATA_

#include "qlparser/qtdata.hh"

#include "raslib/minterval.hh"
#include "raslib/point.hh"

#include <string>
#include <vector>
#include <iostream>

//@ManMemo: Module: {\bf qlparser}

/*@Doc:
  The class encapsulates an minterval.
*/

class QtMintervalData : public QtData
{
public:
    /// constructor getting the minterval
    QtMintervalData( const r_Minterval& minterval, vector<bool>* initTrimFlags = NULL );

    /// virtual destructor
    virtual ~QtMintervalData();

    //@Man: Read/Write methods:
    //@{
    ///

    ///
    inline const r_Minterval&  getMintervalData() const;
    ///
    inline void                setMintervalData( const r_Minterval& interval );
    //
    // inline void                setMintervalData( const r_Point& point );
    ///
    inline const vector<bool>* getTrimFlags() const;

    /// returns a null-terminated string describing the type structure
    virtual char* getTypeStructure() const;
    /**
      The string pointer has to be free using free() by the caller.
    */

    ///
    //@}

    /// returns {\tt QT_INTERVAL}
    virtual QtDataType getDataType() const;

    /// compares data content
    virtual bool equal( const QtData* obj ) const;

    /// returns content dependent string representation
    virtual std::string getSpelling() const;

    /// print status of the object to the specified stream
    virtual void printStatus( std::ostream& stream = std::cout ) const;

private:
    /// prevents from using the default constructor
    QtMintervalData() {};

    /// attribute storing the minterval
    r_Minterval mintervalData;

    /// bitvector indicating real projections
    vector<bool>* trimFlags;
};

#include "qlparser/qtmintervaldata.icc"

#endif




