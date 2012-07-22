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
 *   The ShortType class is the superclass for all classes
 *   describing the type of a cell
 *
 *
 * COMMENTS:
 *   This file is patched by O2!
 *
 ************************************************************/

#ifndef _SHORTTYPE_HH_
#define _SHORTTYPE_HH_

#include <iostream>
#include "integraltype.hh"
#include "catalogmgr/ops.hh"

class OId;

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:
ShortType is the base type used for 16bit integer cell
values. The value of a Short is stored in four chars.

*/

/**
  * \ingroup Relcatalogifs
  */
class ShortType : public IntegralType
{
public:
    ShortType(const OId& id) throw (r_Error);

    ShortType();
    /*@Doc:
    default constructor, sets type name to "Short".
    */

    ShortType(const ShortType& old);
    /*@Doc:
    copy constructor.
    */

    ShortType& operator=(const ShortType& old);
    /*@Doc:
    assignment operator.
    */

    virtual ~ShortType();
    /*@Doc:
    virtual destructor.
    */

    virtual void printCell(ostream& stream, const char* cell) const;
    /*@Doc:
    */

    virtual r_Long* convertToCLong(const char* cell, r_Long* value) const;
    /*@Doc:
    */

    virtual char* makeFromCLong(char* cell, const r_Long* value) const;
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
