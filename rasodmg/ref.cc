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
/
/**
 * SOURCE:   ref.cc
 *
 * MODULE:   rasodmg
 * CLASS:    r_Ref
 *
 * COMMENTS:
 *      None
*/

static const char rcsidref[] = "@(#)rasodmg, r_Ref: $Id: ref.cc,v 1.28 2002/08/28 12:10:18 coman Exp $";

#include "rasodmg/database.hh"
#include "rasodmg/ref.hh"

#include "raslib/rmdebug.hh"

#ifdef __VISUALC__
#include <sstrea.h>
#else
#include <sstream>
#endif

#include "rasodmg/transaction.hh"
#include "clientcomm/clientcomm.hh"

// forward declaration needed because of EARLY_TEMPLATE
class r_Transaction;

// In case of early templates take care non-template code is only
// compiled once.
#ifndef __EXECUTABLE__

r_Ref_Any::r_Ref_Any()
    : memptr(0), oid()
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "r_Ref_Any()")
}



r_Ref_Any::r_Ref_Any( const r_Ref_Any& obj )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "r_Ref_Any( const r_Ref_Any& )")
    memptr       = obj.memptr;
    oid          = obj.oid;
}



r_Ref_Any::r_Ref_Any( const r_OId& initOId )
    : memptr(0)
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "r_Ref_Any( const r_OId& )")
    oid = initOId;
}



r_Ref_Any::r_Ref_Any( r_Object* ptr )
    : oid(), memptr( ptr )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "r_Ref_Any( r_Object* )")
}



r_Ref_Any::r_Ref_Any( void* ptr )
    : oid(), memptr( ptr )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "r_Ref_Any( void* )")
}


r_Ref_Any::r_Ref_Any( const r_OId &initOId, r_Object* ptr )
    : oid( initOId ), memptr( ptr )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "r_Ref_Any( const r_OId &oid, const r_Object* )")
}



r_Ref_Any::~r_Ref_Any()
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "~r_Ref_Any()")

    // object should not be delete from databse when reference destructor is called
    //  if( memptr ){
    //    delete memptr;
    //    memptr = 0;
    //  }
}



r_Ref_Any&
r_Ref_Any::operator=( const r_Ref_Any& objptr )
{
    memptr    = objptr.memptr;
    oid       = objptr.oid;

    return *this;
}



r_Ref_Any&
r_Ref_Any::operator=( r_Object* ptr )
{
    memptr    = ptr;
    oid       = r_OId();

    return *this;
}



void
r_Ref_Any::destroy()
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "destroy()")

    if( memptr && !oid.is_valid() )
    {
        delete memptr;
        memptr = 0;
    }
}


void
r_Ref_Any::delete_object()
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "delete_object()")

    if( memptr )
    {
        delete memptr;
        memptr = 0;
    }
}



r_Ref_Any::operator const void*() const
{
    return memptr;
}



r_Ref_Any::operator void*()
{
    return memptr;
}



r_Ref_Any::operator r_Point*()
{
    return (r_Point*)memptr;
}



r_Ref_Any::operator r_Sinterval*()
{
    return (r_Sinterval*)memptr;
}



r_Ref_Any::operator r_Minterval*()
{
    return (r_Minterval*)memptr;
}



r_Ref_Any::operator r_OId*()
{
    return (r_OId*)memptr;
}



r_Ref_Any::operator r_Scalar*()
{
    return (r_Scalar*)memptr;
}



r_Ref_Any::operator r_Primitive*()
{
    return (r_Primitive*)memptr;
}



r_Ref_Any::operator r_Structure*()
{
    return (r_Structure*)memptr;
}



int
r_Ref_Any::operator!() const
{
    return !is_null();
}



int
r_Ref_Any::is_null() const
{
    return (memptr==0) && !oid.is_valid();
}



int
r_Ref_Any::operator==( const r_Ref_Any& ref ) const
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "operator==( const r_Ref_Any& )")
    return // both refs are not valid or ...
        ( is_null() && ref.is_null() ) ||
        // both oids are valid and the same or ...
        ( oid.is_valid() && oid == ref.oid ) ||
        // both oids are not valid and memory pointers are the same
        ( !oid.is_valid() && !ref.oid.is_valid() && memptr == ref.memptr );
}



int
r_Ref_Any::operator!=( const r_Ref_Any& ref ) const
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "operator!=( const r_Ref_Any& )")
    return !operator==( ref );
}



int
r_Ref_Any::operator==( const r_Object* ptr ) const
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "operator==( const r_Object* )")
    return memptr == (void*)ptr;
}



int
r_Ref_Any::operator!=( const r_Object* ptr ) const
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref_Any", "operator!=( const r_Object* )")
    return !operator==( ptr );
}




void*
r_Ref_Any::get_memory_ptr() const
{
    return memptr;
}

#endif  // __EXECUTABLE__



template<class T>
r_Ref<T>::r_Ref()
    : memptr(0), oid()
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "r_Ref()")
}



template<class T>
r_Ref<T>::r_Ref( const r_Ref<T>& obj )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "r_Ref( const r_Ref<T>& )")
    memptr       = obj.memptr;
    oid          = obj.oid;
}



template<class T>
r_Ref<T>::r_Ref( const r_OId& initOId )
    : memptr(0)
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "r_Ref( const r_OId& )")
    oid = initOId;
}



template<class T>
r_Ref<T>::r_Ref( const r_Ref_Any& obj )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "r_Ref( const r_Ref_Any& )")

    memptr    = (T*)obj.get_memory_ptr();
    oid       = obj.get_oid();
}



template<class T>
r_Ref<T>::r_Ref( T* ptr )
    : oid(), memptr( ptr )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "r_Ref( const T* )")
}



template<class T>
r_Ref<T>::r_Ref( const r_OId &initOId, T* ptr )
    : oid( initOId ), memptr( ptr )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "r_Ref( const r_OId &oid, const T* )")
}



template<class T>
r_Ref<T>::~r_Ref()
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "~r_Ref()")

    // object should not be delete from databse when reference destructor is called
    //  if( memptr ){
    //    delete memptr;
    //    memptr = 0;
    //  }
}



template<class T>
r_Ref<T>::operator r_Ref_Any() const
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "operator r_Ref_Any()")
    return r_Ref_Any( oid, (r_Object *)memptr );
}


/*
template<class T>
r_Ref<T>::operator const r_Ref_Any() const
{
  RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "operator const r_Ref_Any()")
  return r_Ref_Any( oid, memptr );
}
*/

template<class T>
r_Ref<T>&
r_Ref<T>::operator=( const r_Ref_Any& ptr )
{
    memptr    = (T*)ptr.get_memory_ptr();
    oid       = ptr.get_oid();

    return *this;
}



template<class T>
r_Ref<T>&
r_Ref<T>::operator=( T* ptr )
{
    memptr    = ptr;
    oid       = r_OId();

    return *this;
}



/*
template<class T>
r_Ref<T>&
r_Ref<T>::operator=( r_Ref<T>& objptr )
{
  memptr    = objptr.memptr;
  oid       = objptr.oid;

  return *this;
}
*/


template<class T>
r_Ref<T>&
r_Ref<T>::operator=( const r_Ref<T>& objptr )
{
    memptr    = objptr.memptr;
    oid       = objptr.oid;

    return *this;
}



template<class T>
const T&
r_Ref<T>::operator*() const throw( r_Error )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "operator*()")
    if( !memptr )
        load_object();

    if( !memptr )
    {
        r_Error err = r_Error( r_Error::r_Error_RefNull );
        throw err;
    }

    return *memptr;
}


template<class T>
T&
r_Ref<T>::operator*() throw( r_Error )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "operator*()")
    if( !memptr )
        load_object();

    if( !memptr )
    {
        r_Error err = r_Error( r_Error::r_Error_RefNull );
        throw err;
    }

    return *memptr;
}



template<class T>
const T*
r_Ref<T>::operator->() const throw( r_Error )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "operator->()")
    if( !memptr )
        load_object();

    if( !memptr )
    {
        r_Error err = r_Error( r_Error::r_Error_RefNull );
        throw err;
    }

    return memptr;
}

template<class T>
T*
r_Ref<T>::operator->() throw( r_Error )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "operator->()")
    if( !memptr )
        load_object();

    if( !memptr )
    {
        r_Error err = r_Error( r_Error::r_Error_RefNull );
        throw err;
    }

    return memptr;
}



template<class T>
const T*
r_Ref<T>::ptr() const throw( r_Error )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "ptr()")
    if( !memptr )
        load_object();

    return memptr;
}


template<class T>
T*
r_Ref<T>::ptr() throw( r_Error )
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "ptr()")
    if( !memptr )
        load_object();

    return memptr;
}



template<class T>
int
r_Ref<T>::operator!() const
{
    return !is_null();
}



template<class T>
int
r_Ref<T>::is_null() const
{
    return (memptr==0) && !oid.is_valid();
}



template<class T>
int
r_Ref<T>::operator==( const r_Ref<T>& refR ) const
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "operator==( const r_Ref<T>& )")
    return // both refs are not valid or ...
        ( is_null() && refR.is_null() ) ||
        // both oids are valid and the same or ...
        ( oid.is_valid() && oid == refR.oid ) ||
        // both oids are not valid and memory pointers are the same
        ( !oid.is_valid() && !refR.oid.is_valid() && memptr == refR.memptr );
}



template<class T>
int
r_Ref<T>::operator!=( const r_Ref<T>& refR ) const
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "operator!=( const r_Ref<T>& )")
    return !operator==( refR );
}



template<class T>
int
r_Ref<T>::operator==( const T* ptr ) const
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "operator==( const T* )")
    return memptr == ptr;
}



template<class T>
int
r_Ref<T>::operator!=( const T* ptr ) const
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "operator!=( const T* )")
    return !operator==( ptr );
}


template<class T>
void
r_Ref<T>::destroy()
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "destroy()")

    if( memptr && !oid.is_valid() )
    {
        delete memptr;
        memptr = 0;
    }
}



template<class T>
void
r_Ref<T>::delete_object()
{
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Ref", "delete_object()")

    if( memptr )
    {
        delete memptr;
        memptr = 0;
    }
}



template<class T>
T*
r_Ref<T>::get_memory_ptr() const
{
    return memptr;
}

#ifdef DEF_TRANSACTION

template<class T>
void
r_Ref<T>::load_object() const
{
    if( oid.is_valid() )
    {
        if( r_Database::actual_database == 0 || r_Database::actual_database->get_status() == r_Database::not_open )
        {
            r_Error err = r_Error( r_Error::r_Error_DatabaseClosed );
            throw err;
        }

        if( r_Transaction::actual_transaction == 0 ||
                r_Transaction::actual_transaction->get_status() != r_Transaction::active )
        {
            r_Error err = r_Error( r_Error::r_Error_TransactionNotOpen );
            throw err;
        }

        // load object and take its memory pointer
        r_Ref<T> ref = r_Transaction::actual_transaction->load_object( oid );
        memptr       = ref.get_memory_ptr();
    }
}

#endif
