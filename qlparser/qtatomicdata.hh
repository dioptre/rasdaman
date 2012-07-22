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
#ifndef _QTATOMICDATA_HH_
#define _QTATOMICDATA_HH_

#include "qlparser/qtscalardata.hh"
#include "raslib/odmgtypes.hh"

/*************************************************************
 *
 *
 * COMMENTS:
 *
 ************************************************************/

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

 The class represents an atomic value handled by the query tree.

*/

class QtAtomicData : public QtScalarData
{
public:
    /// default constructor
    QtAtomicData();

    /// constructor getting signed integer data
    QtAtomicData( r_Long value, unsigned short byteLength );
    /**
      The value for {\tt byteLength} specifies the type of the constant.

      \begin{tabular}{lll}
      1 && OCTET\\
      2 && SHORT\\
      4 && LONG\\
      \end{tabular}
    */

    /// constructor getting unsigned integer data
    QtAtomicData( r_ULong value, unsigned short byteLength );
    /**
      The value for {\tt byteLength} specifies the type of the constant.

      \begin{tabular}{lll}
      1 && CHAR\\
      2 && USHORT\\
      4 && ULONG\\
      \end{tabular}
    */

    /// constructor getting boolean data
    QtAtomicData( bool value );

    /// constructor getting float data
    QtAtomicData( double value, unsigned short byteLength );
    /**
      The value for {\tt byteLength} specifies the type of the constant.

      \begin{tabular}{lll}
      4 && FLOAT\\
      8 && DOUBLE\\
      \end{tabular}
    */

    /// constructor getting complex data
    QtAtomicData(double valRe, double valIm, unsigned short size);

    /// copy constructor
    QtAtomicData( const QtAtomicData& obj );

    /// destructor
    virtual ~QtAtomicData();

    /// returning unsigned value of an unsigned atomic type if it is one, 0 otherwise
    r_ULong getUnsignedValue() const;

    /// returning signed value of a signed atomic type if it is one, 0 otherwise
    r_Long getSignedValue() const;

    /// returning double value if it is one, 0 otherwise
    double getDoubleValue() const;

    /// print status of the object to the specified stream
    virtual void printStatus( std::ostream& stream = std::cout ) const;
};

#include "qlparser/qtatomicdata.icc"

#endif






















