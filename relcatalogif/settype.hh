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
 *   The SetType class represents the type for sets of MDD
 *   objects.
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _SETTYPE_HH_
#define _SETTYPE_HH_

class SetType;

#include <iostream>
#include "collectiontype.hh"
#include "catalogmgr/ops.hh"
class OId;

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:
  The SetType class represents the type for sets of MDD
  objects.
*/

/**
  * \ingroup Relcatalogifs
  */
class SetType : public CollectionType
{
public:
    SetType(const OId& id) throw (r_Error);

    virtual char* getTypeStructure() const;

    /// constructor receiving pointer to an MDDType (or subclass).
    SetType(const char* newTypeName, const MDDType* newMDDType);
    /// default constructor, cannot be used.
    SetType();
    /// copy constructor.
    SetType(const SetType& old);
    /// assignment operator.
    SetType& operator=(const SetType& old);

    /// virtual destructor.
    virtual ~SetType();

protected:

    virtual void deleteFromDb() throw (r_Error);

    virtual void insertInDb() throw (r_Error);

    virtual void readFromDb() throw (r_Error);
};

#endif
