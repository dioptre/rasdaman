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
 *   The UIntegralType class is the superclass for all unsigned
 *   integral types (Char, Short, Long) describing the type of a
 *   cell
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _UINTEGRALTYPE_HH_
#define _UINTEGRALTYPE_HH_

#include "atomictype.hh"

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:
UIntegralType is the abstract base class for all integral
\Ref{BaseType} subclasses, i.e. base types like \Ref{ULongType} or
\Ref{BoolType}. It provides conversions to/from long and
double. It's subclasses must implement conversions to/from unsigned
long.
*/

/**
  * \ingroup Relcatalogifs
  */
class UIntegralType : public AtomicType
{
public:
    UIntegralType(unsigned int newSize):
        AtomicType(newSize) {}
    /*@Doc:
    constructor.
    */

    UIntegralType(const UIntegralType& old):
        AtomicType(old) {}
    /*@Doc:
    copy constructor.
    */

    UIntegralType(const OId& id) throw (r_Error):
        AtomicType(id) {}
    /*@Doc:
    */

    virtual ~UIntegralType() {}
    /*@Doc:
    */

    virtual r_Long* convertToCLong(const char*, r_Long*) const;
    virtual char* makeFromCLong(char*, const r_Long*) const;

    virtual double* convertToCDouble(const char*, double*) const;
    virtual char* makeFromCDouble(char*, const double*) const;

protected:
    UIntegralType(const char* name, unsigned int newSize):
        AtomicType(name, newSize) {}
    /*@Doc:
    */

};

#include "uintegraltype.icc"

#endif
