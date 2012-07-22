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
 *   The RealType class is the superclass for floating point
 *   types (Float, Double) describing the type of a
 *   cell
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _REALTYPE_HH_
#define _REALTYPE_HH_

#include "atomictype.hh"

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:
RealType is the abstract base class for floating point
\Ref{BaseType} subclasses, \Ref{Float} or \Ref{Double}.
It provides conversions to/from long and unsigned long
It's subclasses must implement conversions to/from double
*/

/**
  * \ingroup Relcatalogifs
  */
class RealType : public AtomicType
{
public:
    RealType(unsigned int newSize):
        AtomicType(newSize) {}
    /*@Doc:
    constructor.
    */

    RealType(const RealType& old):
        AtomicType(old) {}
    /*@Doc:
    copy constructor.
    */

    RealType(const OId& id) throw (r_Error):
        AtomicType(id) {}
    /*@Doc:
    */

    virtual ~RealType() {}
    /*@Doc:
    */

    virtual r_ULong* convertToCULong(const char*, r_ULong*) const;
    virtual char* makeFromCULong(char*, const r_ULong*) const;

    virtual r_Long* convertToCLong(const char*, r_Long*) const;
    virtual char* makeFromCLong(char*, const r_Long*) const;

protected:
    RealType(const char* name, unsigned int newSize):
        AtomicType(name, newSize) {}
    /*@Doc:
    */
};

#include "realtype.icc"

#endif


