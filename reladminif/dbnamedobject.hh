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
#ifndef _DBNAMEDOBJECT_HH_
#define _DBNAMEDOBJECT_HH_

class DBObject;
class DBNamedObject;

#include "dbobject.hh"

//@ManMemo: Module: {\bf reladminif}.
/*@Doc:
Has functionality for setting the name of itsself from VARCHAR structures.
Takes care of too long names.
Implements set/getName functionality.
*/
/**
  * \ingroup Reladminifs
  */
class DBNamedObject : public DBObject
{
public:
    DBNamedObject();
    /*@Doc:
    sets Name to defaultName
    */

    DBNamedObject(const OId& id) throw (r_Error);
    /*@Doc:
    only initializes itself
    */

    DBNamedObject(const DBNamedObject& old);
    /*@Doc:
    sets myName to the name of the old object
    */

    DBNamedObject(const char* name);
    /*@Doc:
    sets myName to name
    */

    DBNamedObject(const OId& id, const char* name);
    /*@Doc:
    sets myName to name and calls DBObject(OId).  this is needed by MDDSet.
    */

    virtual ~DBNamedObject();
    /*@Doc:
    frees myName
    */

    const char* getName() const;
    /*@Doc:
    returns a pointer to myName.
    */

    static short MAXNAMELENGTH;
    /*@Doc:
    the maximum length of a name.
    */

    DBNamedObject& operator=(const DBNamedObject& old);
    /*@Doc:
    takes care of the name
    */

    virtual r_Bytes getMemorySize() const;
    /*@Doc:
    Should be revised not to include attribute sizes
    */

    virtual void printStatus(unsigned int level = 0, std::ostream& stream = std::cout) const;
    /*@Doc:
    prints the status of DBObject + Name: myName
    */

protected:

    void setName(const char* newname);
    /*@Doc:
    renames the object
    */

    void setName(const short length, const char* data);
    /*@Doc:
    sets the name from a VARCHAR structure
    */

    char* myName;
    /*@Doc:
    the name of the object
    */

    unsigned short myNameSize;
    /*@Doc:
    the size of the name
    */

    static const char* defaultName;
};

#endif
