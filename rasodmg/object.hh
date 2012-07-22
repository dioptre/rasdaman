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
 * INCLUDE:  object.hh
 *
 * MODULE:   rasodmg
 * CLASS:    r_Object
 *
 * COMMENTS:
 *      None
*/

#ifndef _D_OBJECT_
#define _D_OBJECT_

#include "raslib/error.hh"
#include "raslib/oid.hh"
#include "raslib/rminit.hh"

#include <stdlib.h>

// forward declarations
class r_Database;
class r_Type;

//@ManMemo: Module: {\bf rasodmg}

/*@Doc:

  Through inheritance of this class, the type definer specifies that
  the subclass is capable of having persistent as well as transient
  instances.

  {\bf Attention: }Right now, just the classes \Ref{r_Set} and \Ref{r_Marray}
  inherit from \Ref{r_Object} which, therefore, are the only persistent
  capable classes. In fact, just objects of type {\tt r_Set<r_Ref<r_Marray<...>>>}
  can be made persitent right now.

*/

/**
  * \ingroup Rasodmgs
  */
class r_Object
{
public:
    /// default constructor
    r_Object();

    /// constructor getting objType
    r_Object( unsigned short objType ) throw(r_Error);
    /**
      {\tt objType} specifies the type of the object (1=Marray, 2=Collection).
      This is needed for oid allocation and propably dropped in future.
    */

    /// copy constructor
    r_Object( const r_Object&, unsigned short objType=0 ) throw(r_Error);
    /**
      {\tt objType} specifies the type of the object (1=Marray, 2=Collection).
      This is needed for oid allocation and propably dropped in future.
    */

    /// virtual destructor
    virtual ~r_Object();

    /// mark the object as modified
    inline void mark_modified();

    /// new operator for transient objects
    void* operator new( size_t size );

    /// new operator for persistent objects
    void* operator new( size_t size, r_Database *database, const char* type_name = 0 );

    /// new operator for transient objects carrying type information
    void* operator new( size_t size, const char* type_name );

    /// delete operator
    void operator delete( void* obj_ptr );

    /// set object type by name
    inline void set_type_by_name( const char* name ) throw(r_Error);
    /**
      With this method a type name has to be given by the user for each
      object which he wants to make persistent. The type name is the name
      introduced in the RasDL file. If an object without a valid type
      name is made persistent, an error occurs while committing the
      transaction.

      NOTE: This method is updated. Use {\tt void* operator new( size_t size,
      r_Database *database, const char* type_name )} instead.
    */

    /// set object type by name
    inline void set_type_structure( const char* name ) throw(r_Error);

    /// get object type name
    inline const char* get_type_name() const;

    /// get object type structure
    inline const char* get_type_structure() const;

    /// get oid
    inline const r_OId& get_oid() const;

    /// get type schema
    const r_Type* get_type_schema();

    void set_type_schema(const r_Type* type) throw (r_Error);

    //@Man: Methods and types for internal use only:
    //@{
    ///

    /// object life status
    enum ObjectStatus { no_status, deleted, created, modified, read, transient };
    /**
      created   - The object was created in the current transaction and has to be stored in the database.
      deleted   - The object was deleted. It is still in main memory and in the database. It is going to
                  be removed at the end of the transaction.
      modified  - Object was modified and has to be updated in the database.
      read      - The object was read from the database without modifying it afterwards.
      transient - The object belongs to a declarative query result. In most cases, it has no persistent
                  counterpart in the db. It is NOT considered in the update phase.
    */

    /// object types
    enum ObjectType { no_object, persistent_object, transient_object };

    ///
    /// inserts an object into the database
    virtual void insert_obj_into_db()=0;
    /// inserts an object into a specific collection in the database
    virtual void insert_obj_into_db( const char* )=0;
    /// updates an object in database
    virtual void update_obj_in_db();
    /// load an object from the database
    virtual void load_obj_from_db();
    /// deletes an object from the database
    void delete_obj_from_db();

    ///
    /// initialize oid of the object
    void initialize_oid( const r_OId& initOId );

    ///
    /// it is called when an object comes into transient memory
    virtual void r_activate()
    {
        ;
    };
    /// it is called when an object leaves transient memory
    virtual void r_deactivate();

    ///
    /// test object status returns 1 if it matches
    int test_status( ObjectStatus status );
    /// gets the status of the object
    inline ObjectStatus get_status() const;

    ///
    /// set object name. object name should contain only [a-zA-Z0-9_]
    inline void set_object_name( const char* name ) throw(r_Error);
    /// get object name
    inline const char* get_object_name() const;

    ///
    /// new operator for activating an object (status = read)
    void* operator new( size_t size, r_Database *database, ObjectStatus status, const r_OId& oid );

    ///
    //@}

protected:
    /// test object type returns 1 if it matches
    int test_type( ObjectType type );

    /// stores object name if it has one
    char* object_name;

    /// stores object type name if it has one
    char* type_name;

    /// store type structure as string if it has one
    char* type_structure;

    /// pointer to type schema (built on request)
    r_Type* type_schema;

    /// internal object type (1 marray, 2 collection)
    unsigned short internal_obj_type;

private:
    /// right now, the object life status is either deleted, created, or read
    ObjectStatus object_status;

    /// persistent_object or transient_object
    ObjectType object_type;

    /// object identifier
    r_OId oid;

    /// holds the next object type between new operation and constructor
    static ObjectType next_object_type;

    /// holds the next object status between new operation and constructor
    static ObjectStatus next_object_status;

    /// holds the next object type name between new operation and constructor
    static char* next_object_type_name;

    /// holds the next object oid between new operation and constructor
    static r_OId next_object_oid;

    /// holds the last object type between destructor and delete operator
    static ObjectType last_object_type;
};

#include "rasodmg/object.icc"

#endif




