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
 * SOURCE: collection.cc
 *
 * MODULE: rasodmg
 * CLASS:  r_Collection
 *
 * COMMENTS:
 *      None
*/

using namespace std;

static const char rcsidcollection[] = "@(#)rasodmg, r_Collection: $Id: collection.cc,v 1.53 2005/07/06 23:30:22 rasdev Exp $";

#include "raslib/rmdebug.hh"
#include "raslib/collectiontype.hh"

#include "rasodmg/collection.hh"
#include "rasodmg/iterator.hh"
#include "rasodmg/database.hh"

#include "clientcomm/clientcomm.hh"
#include <string.h>

#ifndef __GNUG__
#define NULL 0
#endif

template<class T>
r_Collection<T>::r_Collection() throw(r_Error)
    : r_Object( 2 ), card(0)
{
    init_node_list( coll );
    init_node_list( removed_objects );
}


template<class T>
r_Collection<T>::r_Collection( const r_Collection<T>& collection ) throw(r_Error)
    : r_Object( collection, 2 )
{
    CNode* nptr;
    CNode* optr;

    coll = new CNode;
    nptr = coll;
    optr = collection.coll;

    while ( optr->next != NULL )
    {
        nptr->next = new CNode;
        nptr->elem = new T;
        *(nptr->elem) = *(optr->elem);
        nptr = nptr->next;
        optr = optr->next;
    }
    if ( optr->elem != NULL )
    {
        nptr->elem = new T;
        *(nptr->elem) = *(optr->elem);
    }

    nptr->next = NULL;
    card = collection.cardinality();

    init_node_list( removed_objects );
}

template<class T>
r_Collection<T>::r_Collection( const void* node1 )
{
    // This constructor is nearly the same as a copy constrctor,
    // except that the argument is not of type r_Collection but
    // of type CNode* (the collection's internal representation).
    // Does the same as function set_internal_representation.

    // We assume that node1 is a correct CNode structure as
    // defined in collection.hh
    // copy it and determine cardinality
    CNode* nptr;
    CNode* optr;

    card = 0;
    coll = new CNode;
    nptr = coll;
    optr = (CNode*)node1;

    while ( optr->next != NULL )
    {
        nptr->next = new CNode;
        nptr->elem = new T;
        card++;
        *(nptr->elem) = *(optr->elem);
        nptr = nptr->next;
        optr = optr->next;
    }
    if ( optr->elem != NULL )
    {
        nptr->elem = new T;
        card++;
        *(nptr->elem) = *(optr->elem);
    }

    nptr->next = NULL;

    init_node_list( removed_objects );
}

template<class T>
r_Collection<T>::~r_Collection()
{
    RMDBGONCE(1, RMDebug::module_rasodmg, "r_Collection<T>", "~r_Collection")

    r_deactivate();
}



/*************************************************************
 * Method name...: r_deactivate()
 *
 * Arguments.....:
 *   none
 * Return value..:
 *   none
 * Description...: This method is called when the object leaves
 *                 the application cache. It frees all dynamic
 *                 memory allocated within the class.
 ************************************************************/
template<class T>
void
r_Collection<T>::r_deactivate()
{
    RMDBGONCE(1, RMDebug::module_rasodmg, "r_Collection<T>", "r_deactivate()")

    remove_all_nodes( coll );
    remove_all_nodes( removed_objects );
}



template<class T>
int
r_Collection<T>::contains_element( const T& element ) const
{
    CNode* ptr = coll;

    while ( *(ptr->elem) != element && ptr->next != NULL )
        ptr = ptr->next;

    if ( *(ptr->elem) == element )
        return true;
    else
        return false;
}

template<class T>
void
r_Collection<T>::insert_element( const T& element, int no_modification )
{
    // add node to list...
    add_node( coll, element );

    // increase cardinaltity
    card++;

    // ...and remove it from removed objects if it exists
    remove_node( removed_objects, element );

    if( !no_modification )
        mark_modified(); // remember modification
}

template<class T>
void
r_Collection<T>::remove_element( const T& element )
{
    // remove node from list...
    if( remove_node( coll, element ) )
    {
        // ...and add it to removed objects list
        add_node( removed_objects, element );

        // decrease cardinality
        card--;

        mark_modified(); // remember modification
    }
}

template<class T>
void
r_Collection<T>::remove_all()
{
    CNode* ptr = coll;
    CNode* ptrLast = coll;

    if ( ptr->elem != NULL )
    {
        add_node( removed_objects, *ptr->elem );
        delete ptr->elem;
        ptr->elem = NULL;
    }
    if ( ptr->next != NULL )
    {
        ptr = ptr->next;
        while ( ptr->next != NULL )
        {
            add_node( removed_objects, *ptr->elem );
            delete ptr->elem;
            ptrLast = ptr;
            ptr = ptr->next;
            delete ptrLast;
        }
        delete ptr->elem;
        delete ptr;
    }
    coll->next = NULL;
    card = 0;

    mark_modified(); // remember modification
}



template<class T>
const r_Collection<T>&
r_Collection<T>::operator=( const r_Collection<T>& collection )
{
    CNode* nptr;
    CNode* optr;

    if( this != &collection )
    {
        if( coll )
            remove_all();
        else
            coll = new CNode;

        nptr = coll;
        optr = collection.coll;

        while ( optr->next != NULL )
        {
            nptr->next = new CNode;
            nptr->elem = new T;
            *(nptr->elem) = *(optr->elem);
            nptr = nptr->next;
            optr = optr->next;
        }
        if ( optr->elem != NULL )
        {
            nptr->elem = new T;
            *(nptr->elem) = *(optr->elem);
        }

        nptr->next = NULL;
        card = collection.cardinality();
    }

    return *this;
}



template<class T>
void
r_Collection<T>::set_internal_representation( const void* node1 )
{
    // We assume that node1 is a correct CNode structure as
    // defined in collection.hh
    // copy it and determine cardinality
    CNode* nptr;
    CNode* optr;

    if( coll )
        remove_all();
    else
        coll = new CNode;

    card = 0;
    nptr = coll;
    optr = (CNode*)node1;

    while ( optr->next != NULL )
    {
        nptr->next = new CNode;
        nptr->elem = new T;
        card++;
        *(nptr->elem) = *(optr->elem);
        nptr = nptr->next;
        optr = optr->next;
    }
    if ( optr->elem != NULL )
    {
        nptr->elem = new T;
        card++;
        *(nptr->elem) = *(optr->elem);
    }

    nptr->next = NULL;
}


template<class T>
r_Iterator<T>
r_Collection<T>::create_removed_iterator()
{
    return r_Iterator<T>( *this, 1 );
}


template<class T>
r_Iterator<T>
r_Collection<T>::create_iterator()
{
    return r_Iterator<T>( *this );
}



template<class T>
void
r_Collection<T>::insert_obj_into_db()
{
    // Insert myself in database only if I have an object name. If the
    // collection doesn't have a name an exception is thrown.
    if( !object_name || !strlen( object_name ) )
    {
        r_Error err = r_Error( r_Error::r_Error_ObjectUnknown );
        throw err;
    }

    // Insert myself in database only if I have a type name, otherwise
    // an exception is thrown.
    if( !type_name || !strlen( type_name ) )
    {
        r_Error err = r_Error( r_Error::r_Error_DatabaseClassUndefined );
        throw err;
    }

    // Insert myself into the database even if i'm empty.
//  r_Database::actual_database->communication->insertColl( object_name, type_name, get_oid() );
    r_Database::actual_database->insertColl( object_name, type_name, get_oid() );
    if( !is_empty() )
    {
        r_Iterator<T> iter = create_iterator();
        for ( iter.reset(); iter.not_done(); iter++ )
            // Search for *1 for an explanation of the following cast.
            ((r_Object*)((r_Ref<r_Object>)(*iter)).ptr())->insert_obj_into_db( object_name );
    }
}



template<class T>
void
r_Collection<T>::update_obj_in_db()
{
    // Update myself in database only if I have an object name. If the
    // collection doesn't have a name an exception is thrown.
    if( !object_name || !strlen( object_name ) )
    {
        r_Error err = r_Error( r_Error::r_Error_ObjectUnknown );
        throw err;
    }

    // inspect collection elements
    if( !is_empty() )
    {
        r_Iterator<T> iter = create_iterator();
        for ( iter.reset(); iter.not_done(); iter++ )
        {
            // *1
            //
            // The following is a very ugly cast, but necessary if collection elements are not restricted
            // to r_Ref objects. Anyway, if the elements are not of type r_Ref, it is not possible to make
            // them persistent. A workaround would be to call a global function instead having a template
            // specification for our case.
            r_Ref<r_Object> ref = (r_Ref<r_Object>)(*iter);

            RMInit::logOut << "    Collection object " << ref.get_oid() << "  " << std::flush;

            // check if object is loaded
            if( ref.get_memory_ptr() != 0 )
            {
                // Search for *1 for an explanation of the following cast.
                switch( ((r_Ref<r_Object>)(*iter))->get_status() )
                {
                case r_Object::deleted:
                    RMInit::logOut << "state DELETED,  not implemented" << endl;
                    RMInit::logOut << "OK" << endl;
                    break;

                case r_Object::created:
                    RMInit::logOut << "state CREATED,  writing  ... " << std::flush;
                    // Search for *1 for an explanation of the following cast.
                    ((r_Object*)((r_Ref<r_Object>)(*iter)).ptr())->insert_obj_into_db( object_name );
                    RMInit::logOut << "OK" << endl;
                    break;

                case r_Object::modified:
                    RMInit::logOut << "state MODIFIED, not implemented" << endl;
                    break;

                case r_Object::read:
                    RMInit::logOut << "state READ,     OK" << endl;
                    break;

                case r_Object::transient:
                    RMInit::logOut << "state TRANSIENT,     OK" << endl;
                    break;

                default:
                    RMInit::logOut << "state UNKNOWN" << endl;
                    break;
                }
            }
            else
                RMInit::logOut << "state NOT LOADED" << endl;
        }
    }

    // inspect removed objects
    if( removed_objects->elem )
    {
        r_Iterator<T> iter = create_removed_iterator();
        for ( iter.reset( 1 ); iter.not_done(); iter++ )
        {
            // *1
            //
            // The following is a very ugly cast, but necessary if collection elements are not restricted
            // to r_Ref objects. Anyway, if the elements are not of type r_Ref, it is not possible to make
            // them persistent. A workaround would be to call a global function instead of having a template
            // specification for our case.
            // The oid could also be got by (*iter)->get_oid() from r_Object but in this case, dereferencing
            // r_Ref would load the object from the server.
            r_OId currentOId = ((r_Ref<r_Object>)(*iter)).get_oid();

            RMInit::logOut << "    Collection object " << currentOId << "  " << std::flush;

            RMInit::logOut << "state REMOVED,  removing ... " << std::flush;
            try
            {
//        r_Database::actual_database->communication->removeObjFromColl( object_name, currentOId );
                r_Database::actual_database->removeObjFromColl( object_name, currentOId );
                RMInit::logOut << "OK" << endl;
            }
            catch( r_Error& obj )
            {
                RMInit::logOut << "FAILED" << endl;
                RMInit::logOut << obj.what() << endl;
            }
        }
    }
}



template<class T>
void
r_Collection<T>::add_node( r_Collection<T>::CNode* &root, const T& element )
{
    CNode* ptr = root;

    if ( ptr->elem == NULL )
    {
        ptr->elem = new T;
        *(ptr->elem) = element;
    }
    else
    {
        while ( ptr->next != NULL )
            ptr = ptr->next;

        ptr->next = new CNode;
        ptr = ptr->next;
        ptr->next = NULL;
        ptr->elem = new T;
        *(ptr->elem) = element;
    }
}

template<class T>
int
r_Collection<T>::remove_node( CNode* &root, const T& element )
{
    CNode* ptr     = root;
    CNode* ptrLast = root;
    int    success = 0;

    if( ptr && ptr->elem )
    {
        // Look for the element or end of list
        while ( *(ptr->elem) != element && ptr->next != NULL )
        {
            ptrLast = ptr;
            ptr = ptr->next;
        }

        // If element is found, destroy the element itself.
        // After that, there are four cases
        // case 1: The element was the only element of the list
        //         (if so, don't destroy the node itself, only set the elem field NULL)
        // case 2: The element had no successor
        //         (if so, the node before the element becomes the last node)
        // case 3: The element had no predecessor
        //         (if so, the node after the element becomes the first node)
        // case 4: The element had a successor and a prodecessor
        //         (if so, the node after the element becomes the successor of the
        //          node before the element)
        if ( *(ptr->elem) == element )
        {
            success = 1;

            delete ptr->elem;
            if ( ptr == ptrLast && ptr->next == NULL )  // case 1
                ptr->elem = NULL;
            else if ( ptr->next == NULL ) // case 2
            {
                ptrLast->next = NULL;
                delete ptr;
            }
            else if ( ptr == ptrLast ) // case 3
            {
                root = ptr->next;
                delete ptr;
            }
            else  // case 4
            {
                ptrLast->next = ptr->next;
                delete ptr;
            }
        }
    }

    return success;
}



template<class T>
void
r_Collection<T>::remove_all_nodes( CNode* &root )
{
    if( root )
    {
        CNode* ptr = root;
        CNode* ptrLast = root;

        while ( ptr->next != NULL )
        {
            delete ptr->elem;
            ptrLast = ptr;
            ptr = ptr->next;
            delete ptrLast;
        }
        delete ptr->elem;
        delete ptr;
    }

    root = 0;
}


template<class T>
void
r_Collection<T>::init_node_list( CNode* &root )
{
    root = new CNode;
    root->next = NULL;
    root->elem = NULL;
}



template<class T>
const r_Type*
r_Collection<T>::get_element_type_schema()
{
    const r_Type*        typePtr = r_Object::get_type_schema();
    const r_Type* elementTypePtr = 0;

    if( typePtr )
    {
        if( typePtr->type_id() == r_Type::COLLECTIONTYPE )
        {
            const r_Collection_Type* collectionTypePtr = (const r_Collection_Type*)typePtr;
            elementTypePtr = &(collectionTypePtr->element_type());
        }
    }

    return elementTypePtr;
}









