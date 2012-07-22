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
// -*-C++-*- (for Emacs)

/*************************************************************
 *
 *
 * PURPOSE:
 *   The CharType class represents unsigned char.
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _CHARTYPE_HH_
#define _CHARTYPE_HH_

#include <iostream>
#include "raslib/odmgtypes.hh"
#include "uintegraltype.hh"
#include "catalogmgr/ops.hh"

class OId;

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:

CharType is the base type used for unsigned char cell values (e.g.
result of comparison operations, see \Ref{Ops}). The value of a Char
is stored in one char. CharType is a persistence capable class.
*/

/**
  * \ingroup Relcatalogifs
  */
class CharType : public UIntegralType
{
public:
    CharType();
    /*@Doc:
    default constructor, no initialization needed for CharType.
    */

    CharType(const OId& id) throw (r_Error);
    /*@Doc:
    */

    CharType(const CharType& old);
    /*@Doc:
    copy constructor.
    */

    CharType& operator=(const CharType& old);
    /*@Doc:
    assignment operator.
    */

    virtual ~CharType();
    /*@Doc:
    virtual destructor.
    */

    virtual void printCell(ostream& stream, const char* cell) const;
    /*@Doc:
    */

    virtual r_ULong* convertToCULong(const char* cell, r_ULong* value) const;
    /*@Doc:
    */

    virtual char* makeFromCULong(char* cell, const r_ULong* value) const;
    /*@Doc:
    */

    static const char* Name;

protected:

    virtual void readFromDb() throw (r_Error);
    /*@Doc:
    initializes the attributes of this type.
    there is no database activity.  this is hard coded.
    */

};

#endif
