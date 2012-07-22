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
 *   DBRef is a smart pointer for managing objects derived from
 *   the DbObject class.
 *
 *
 * COMMENTS:
 *
 ************************************************************/


#ifndef _DBREF_HH_
#define _DBREF_HH_

class DBHierIndex;
class DBRCIndexDS;
class DBTCIndex;
class BLOBTile;
class InlineTile;
class DBTile;
class OId;
class DBObject;
class r_Error;
class IndexDS;
class HierIndexDS;
class DBMDDObj;

template <class T> class DBRef;

#include "oidif.hh"

//@ManMemo: Module: {\bf reladminif}.
/*@Doc:
DBRef is a smart pointer class operating on classes derived from DbObject. A smart
pointer to an object with a known id is created using DBRef<T>(id). The object
managed by a given smart pointer can be changed (rebinding) by using the assignment
operator.
All access methods may throw database related r_Errors.
*/

#define DBOBJID_NONE    OId()
/**
  * \ingroup Reladminifs
  */
template <class T>
class DBRef
{
public:

    DBRef(void);
    /*@Doc:
    Default constructor. Object must be assigned a value before the first dereferencing.
    */

    DBRef(const OId &id);
    /*@Doc:
    Id-constructor, binds smart pointer to object with the given id (must only be unique
    within class T, not within all classes derived from DbObject).
    */

    DBRef(OId::OIdPrimitive id);
    /*@Doc:
    Id-constructor, binds smart pointer to object with the given id (must only be unique
    within class T, not within all classes derived from DbObject).
    */

    DBRef(const DBRef<T> &src);
    /*@Doc:
    Copy-constructor, binds smart pointer to the same object src is bound to.
    */

    DBRef(T *ptr);
    /*@Doc:
    Object-constructor, binds smart pointer explicitly to the object ptr.
    */

    ~DBRef(void);
    /*@Doc:
    Destructor: decrements reference count for the object that was managed by this
    smart pointer.
    */

    bool operator<(const DBRef<T>& other) const;
    /*@Doc:
    Returns true if me.operator==(other) returns -1
    */

    int operator==(const DBRef<T> &src) const;
    /*@Doc:
    Comparison operator:
    Returns:
        -1 if this is not initialised and src is initialised
        -1 if persistent and objId < src.objId
        -1 if transient and src is persistent
        -1 if transient and object < src.object
        0 if persistent and src persistent and myOId == src.myOId
        0 if transient and src transient and object == src.object
        0 if this is not initialised and src is not initialised
        +1 if persistent and objId > src.objId
        +1 if persistent and src is transient
        +1 if transient and object > src.object
        -1 if this is initialised and src is not initialised
    */

    DBRef<T> &operator=(const DBRef<T> &src);
    /*@Doc:
    Assignment operator: removes old binding and rebinds to the same object managed by src.
    */

    DBRef<T> &operator=(T *ptr);
    /*@Doc:
    Assignment operator: removes old binding and rebinds to object ptr.
    */

    T *operator->(void) throw (r_Error);
    /*@Doc:
    Dereferencing operator -> for accessing the managed object's members.
    */

    const T *operator->(void) const throw (r_Error);
    /*@Doc:
    Dereferencing operator -> for accessing the managed object's members.
    */

    T &operator*(void) throw (r_Error);
    /*@Doc:
    Dereferencing operator * for accessing the managed object.
    */

    const T &operator*(void) const throw (r_Error);
    /*@Doc:
    Dereferencing operator * for accessing the managed object.
    */

#ifndef __GNUG__
    T &operator[](int idx) const throw (r_Error);
    /*@Doc:
    Dereferencing operator [] for accessing array objects.
    */
#endif

    T *ptr(void) throw (r_Error);
    /*@Doc:
    Returns pointer to managed object.
    */

    const T *ptr(void) const throw (r_Error);
    /*@Doc:
    Returns pointer to managed object.
    */

    OId getOId(void) const;
    /*@Doc:
    Returns id of managed object
    */

    void delete_object(void);
    /*@Doc:
    deletes the object from database if it is valid else throws an exception.
    */

    bool is_null(void) const;
    /*@Doc:
    Returns false if valid binding exists, true otherwise.
    this method may instantiate an object from the database
    */

    bool is_valid(void) const;
    /*@Doc:
    Returns true if valid binding exists, false otherwise
    */

    bool isInitialised() const;
    /*@Doc:
    Returns true if OId is valid or the pointer is valid, false otherwise
    */

    void release();
    /*@Doc:
    releases this DBRef pointer and refcount to its object
    */

    operator DBRef<DBObject>() const;
    /*@Doc:
    cast operator. works allways.
    */

    operator DBRef<BLOBTile>() const throw (r_Error);
    /*@Doc:
    cast operator.  checks it the objects type is of OId::BLOBOID.
    */

    operator DBRef<DBTile>() const throw (r_Error);
    /*@Doc:
    cast operator.  checks it the objects type is of OId::BLOBOID or OId::INLINETILEOID.
    */

    operator DBRef<InlineTile>() const throw (r_Error);
    /*@Doc:
    cast operator.  checks it the objects type is of OId::INLINETILEOID.
    */

    operator DBRef<DBHierIndex>() const throw (r_Error);
    /*@Doc:
    cast operator.  checks it the objects type is of OId::MDDHIERIXOID.
    */

    operator DBRef<DBTCIndex>() const throw (r_Error);
    /*@Doc:
    cast operator.  checks it the objects type is of OId::INLINEIXOID.
    */

    operator DBRef<DBRCIndexDS>() const throw (r_Error);
    /*@Doc:
    cast operator.  checks it the objects type is of OId::MDDRCIXOID.
    */

    operator IndexDS*() const throw (r_Error);
    /*@Doc:
    cast operator.  checks it the objects type is of any valid index.
    */

    operator HierIndexDS*() const throw (r_Error);
    /*@Doc:
    cast operator.  checks it the objects type is of any valid hierarchical index.
    */

    operator T*() throw (r_Error);
    /*@Doc:
    */

    operator const T*() const throw (r_Error);
    /*@Doc:
    */

    static void setPointerCaching(bool useIt);
    /*@Doc:
    Make the dbref store and use pointers.
    If set to false the DBRef will always ask the objectbroker.
    May be set at any time to false.
    Only between transactions may it be set to true.
    */

    static bool getPointerCaching();
    /*@Doc:
    returns pointerCaching
    */

private:


    mutable T *object;
    /*@Doc:
    Pointer to the managed object or 0 if no binding exists.
    */

    OId objId;
    /*@Doc:
    id of managed object.
    */

    bool pointerValid;
    /*@Doc:
    whenever a smartpointer is initiaised by a pointer, this attribute is set to true.
    this is neccessary for disabled pointer caching.
    */

    static bool pointerCaching;
};

template <class T> bool operator< (const DBRef<T> &me, const DBRef<T> &him);

#ifdef EARLY_TEMPLATE
#ifdef __EXECUTABLE__
#include "dbref.cc"
#endif
#endif

#endif
