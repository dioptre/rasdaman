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
 * SOURCE:   object.cc
 *
 * MODULE:   rasodmg
 * CLASS:    r_Object
 *
 * COMMENTS:
 *		None
*/

#include "mymalloc/mymalloc.h"

static const char rcsid[] = "@(#)rasodmg, r_Object: $Id: object.cc,v 1.37 2002/08/19 14:09:32 schatz Exp $";

#include "raslib/rmdebug.hh"
#include "raslib/type.hh"
#include "raslib/error.hh"

#include "rasodmg/object.hh"

#ifdef __VISUALC__
#ifndef __EXECUTABLE__
#define __EXECUTABLE__
#define OBJECT_NOT_SET
#endif
#endif

#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "clientcomm/clientcomm.hh"

#ifdef OBJECT_NOT_SET
#undef __EXECUTABLE__
#endif

#include <iostream>


// At the beginning, next and last object types/status are not specified.
r_Object::ObjectStatus r_Object::next_object_status    = r_Object::no_status;
r_Object::ObjectType   r_Object::next_object_type      = r_Object::no_object;
char*                  r_Object::next_object_type_name = 0;
r_OId                  r_Object::next_object_oid       = r_OId();
r_Object::ObjectType   r_Object::last_object_type      = r_Object::no_object;



r_Object::r_Object()
  : object_status( next_object_status ),
    object_name(0),
    type_name(0),
    type_structure(0),
    type_schema(0),
    oid()
{
  RMDBCLASS( "r_Object", "r_Object()", "rasodmg", __FILE__, __LINE__ )
   
  if( next_object_type_name ) type_name = strdup( next_object_type_name );

  if( next_object_type == persistent_object )
    RMInit::logOut << "Error: A peristent object is constructed with default constructor." << std::endl;
  else
    object_type = transient_object;
  
  internal_obj_type = 0;

  // reset next object type/status
  r_Object::next_object_type      = no_object;
  r_Object::next_object_status    = no_status;
  r_Object::next_object_type_name = 0;
  r_Object::next_object_oid       = r_OId();
}



r_Object::r_Object( unsigned short objType ) throw(r_Error)
  : object_status( next_object_status ),
    object_name(0),
    type_name(0),
    type_structure(0),
    type_schema(0),
    oid()
{
  RMDBCLASS( "r_Object", "r_Object( unsigned short )", "rasodmg", __FILE__, __LINE__ )
   
  if( next_object_type_name ) type_name = strdup( next_object_type_name );

  if( next_object_type == persistent_object )
  {
    if( r_Transaction::actual_transaction == 0 )
    {
      RMInit::logOut << "Error: Tried to create a persistent object outside a transaction." << std::endl;
      throw r_Error(r_Error::r_Error_TransactionNotOpen);
    }
    
    object_type = persistent_object;

    switch( object_status )
    {
      case created:
        // In case the object is newly created, get a new oid and assign it to the object.
        oid = r_Database::actual_database->get_new_oid( objType );
        break;
      case read:
      case transient:
        // In case the object is read from db, use the oid stored in next_object_oid.
        oid = next_object_oid;
        break;
      default:
	RMDBGONCE(0, RMDebug::module_raslib, "r_Object", "r_Object(objType) bad object_status " << object_status);          
      break;
    }
    
    // Add the object to the list of persistent objects in the current transaction.
    if( oid.is_valid() )
      r_Transaction::actual_transaction->add_object_list( r_Ref<r_Object>( oid, this ) );
    else
      r_Transaction::actual_transaction->add_object_list( r_Ref<r_Object>( this ) );

  }
  else
    object_type = transient_object;
  
  internal_obj_type = objType;

  // reset next object type/status
  r_Object::next_object_type      = no_object;
  r_Object::next_object_status    = no_status;
  r_Object::next_object_type_name = 0;
  r_Object::next_object_oid       = r_OId();
}



/* OBSOLETE
r_Object::r_Object( unsigned short objType, const char* name ) throw(r_Error)
  : object_status( next_object_status ),
    object_name( strdup(name) ),
    type_name(0),
    type_structure(0),
    type_schema(0),
    oid()
{
  RMANDEBUGOUT( "r_Object::r_Object( const char* name)" ) 
     
  if( next_object_type_name ) type_name = strdup( next_object_type_name );

  if( next_object_type == persistent_object )
  {
    if( r_Transaction::actual_transaction == 0 )
    {
      RMInit::logOut << "Error: Tried to create a persistent object outside a transaction." << std::endl;
      throw r_Error(r_Error::r_Error_TransactionNotOpen);
    }
    
    object_type = persistent_object;
    
    // get a new oid and assign it to the object
    //    oid = r_Database::actual_database->get_new_oid( objType );

    // add the object to the list of persistent objects in the actual transaction
    r_Transaction::actual_transaction->add_object_list( r_Ref<r_Object>( this ) );
  }
  else
    object_type = transient_object;
  
  // reset next object type/status
  r_Object::next_object_type      = no_object;
  r_Object::next_object_status    = no_status;
  r_Object::next_object_type_name = 0;
  r_Object::next_object_oid       = r_OId();
}
*/


r_Object::r_Object( const r_Object& obj, unsigned short objType ) throw(r_Error)
  : object_status( next_object_status ),
    object_name(0),
    type_name(0),
    type_structure(0),
    type_schema(0),
    oid()
{
  RMDBCLASS( "r_Object", "r_Object( const r_Object& )", "rasodmg", __FILE__, __LINE__ )
     
  if( next_object_type_name ) type_name = strdup( next_object_type_name );

  if( next_object_type == persistent_object )
  {
    if( r_Transaction::actual_transaction == 0 )
    {
      RMInit::logOut << "Error: Tried to create a persistent object outside a transaction." << std::endl;
      throw r_Error(r_Error::r_Error_TransactionNotOpen);
    }
    
    object_type = persistent_object;
    
    switch( object_status )
    {
      case created:
        // In case the object is newly created, get a new oid and assign it to the object.
	oid = r_Database::actual_database->get_new_oid( objType );
        break;
      case read:
      case transient:
        // In case the object is read from db, use the oid stored in next_object_oid.
        oid = next_object_oid;
        break;
      default:
	RMDBGONCE(0, RMDebug::module_raslib, "r_Object", "r_Object(obj, objType) bad object_status " << object_status);          
      break;        
    }
    
    // Add the object to the list of persistent objects in the actual transaction.
    if( oid.is_valid() )
      r_Transaction::actual_transaction->add_object_list( r_Ref<r_Object>( oid, this ) );
    else
      r_Transaction::actual_transaction->add_object_list( r_Ref<r_Object>( this ) );
  }
  else
    object_type = transient_object;
  
  internal_obj_type = objType;

  // reset next object type/status
  r_Object::next_object_type      = no_object;
  r_Object::next_object_status    = no_status;
  r_Object::next_object_type_name = 0;
  r_Object::next_object_oid       = r_OId();

  if( obj.object_name )
    object_name = strdup( obj.object_name );

  if( obj.type_name && !type_name )
    type_name = strdup( obj.type_name );

  if( obj.type_structure )
    type_structure = strdup( obj.type_structure );
}

void
r_Object::set_type_schema(const r_Type* tyy) throw (r_Error)
	{
	if (type_schema)
		{
		RMInit::logOut << "r_Object::set_type_schema(" << tyy->name() << ") this object has already a type" << std::endl;
		throw r_Error(ILLEGALARGUMENT);
		}
	type_schema = tyy->clone();
	}


/*************************************************************
 * Method name...: ~r_Object()
 *
 * Arguments.....:
 *   none
 * Return value..:
 *   none
 * Description...: Destructor. 
 ************************************************************/

r_Object::~r_Object()
{
  RMDBCLASS( "r_Object", "~r_Object()", "rasodmg", __FILE__, __LINE__ )
       
  // Free memory in the transient case. In the persistent case, r_deactivate()
  // is invoked at the commit/abort point.
  if( test_type( transient_object ) )
    r_deactivate();
   
  object_status = deleted; 
   
  // store the object type for the delete operator
  r_Object::last_object_type = object_type;
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
void
r_Object::r_deactivate()
{
  RMDBCLASS( "r_Object", "r_deactivate()", "rasodmg", __FILE__, __LINE__ )
     
  if( type_schema )
  {
    delete type_schema;
    type_schema = 0;
  }

  if( object_name )
  {
    free( object_name );
    object_name = 0;
  }

  if( type_name )
  {
    free( type_name );
    type_name = 0;
  }

  if( type_structure )
  {
    delete [] type_structure;
    type_structure = 0;
  }

  oid.r_deactivate();
}
    
 

/*************************************************************
 * Method name...: operator new( size_t size )
 *
 * Arguments.....:
 *   none
 * Return value..:
 *   none
 * Description...: New operator set the next_object_type and
 *                 allocates memory for the object.
 ************************************************************/
void*
r_Object::operator new( size_t size )
{
  RMDBCLASS( "r_Object", "operator new( size_t )", "rasodmg", __FILE__, __LINE__ )
     
  r_Object::next_object_type      = transient_object;
  r_Object::next_object_status    = created;
  r_Object::next_object_type_name = 0;
  r_Object::next_object_oid       = r_OId();
  
  void* a = mymalloc( size );  
  return a;
}


    
/*************************************************************
 * Method name...: operator new( size_t size,
 *                               r_Database *database,
 *                               const char* type_name  )
 *
 * Arguments.....:
 *   none
 * Return value..:
 *   none
 * Description...: New operator set the next_object_type and
 *                 allocates memory for the object.
 ************************************************************/
void*
r_Object::operator new( size_t size, r_Database* /*database*/, const char* type_name )
{
  RMDBCLASS( "r_Object", "operator new( size_t, r_Database, const char* )", "rasodmg", __FILE__, __LINE__ )
     
  r_Object::next_object_type      = persistent_object;
  r_Object::next_object_status    = created;
  r_Object::next_object_type_name = (char*)type_name;
  r_Object::next_object_oid       = r_OId();
  
  void* a = mymalloc( size );  
  return a;  
}


    
void*
r_Object::operator new( size_t size, const char* type_name )
{
  RMDBCLASS( "r_Object", "operator new( size_t, const char* )", "rasodmg", __FILE__, __LINE__ )
     
  r_Object::next_object_type      = transient_object;
  r_Object::next_object_status    = created;
  r_Object::next_object_type_name = (char*)type_name;
  r_Object::next_object_oid       = r_OId();
  
  void* a = mymalloc( size );  
  return a;  
}



/*************************************************************
 * Method name...: operator delete( void* obj_ptr )
 *
 * Arguments.....:
 *   none
 * Return value..:
 *   none
 * Description...: Delete operator.
 *                 Transient objects are deleted immediately from 
 *                 main memory. 
 *                 Persistent objects have been marked as deleted in
 *                 the destructor. Further accesses through a r_Ref raise
 *                 an exception. Main memory is freed after the transaction
 *                 commits.
 ************************************************************/
void
r_Object::operator delete( void* obj_ptr )
{
  RMDBCLASS( "r_Object", "operator delete()", "rasodmg", __FILE__, __LINE__ )

  if( r_Object::last_object_type == transient_object )
    free( obj_ptr );
 
  r_Object::last_object_type = no_object;
}


    
/*************************************************************
 * Method name...: test_status( ObjectStatus status )
 *
 * Arguments.....:
 *   none
 * Return value..:
 *   none
 * Description...: Tests if status matches the object status. If so
 *                 in returns 1, otherwise 0.
 ************************************************************/
int
r_Object::test_status( ObjectStatus status )
{
  return ( status == object_status );
}



/*************************************************************
 * Method name...: test_type( ObjectType type )
 *
 * Arguments.....:
 *   none
 * Return value..:
 *   none
 * Description...: Tests if type matches the object type. If so
 *                 in returns 1, otherwise 0.
 ************************************************************/
int
r_Object::test_type( ObjectType type )
{
  return ( type == object_type );
}



/*************************************************************
 * Method name...: operator new( size_t       size,
 *                               r_Database   *database,
 *                               ObjectStatus status  )
 *
 * Arguments.....:
 *   none
 * Return value..:
 *   none
 * Description...: New operator set the next_object_type to 
 *   persistent object and the next_object_status to the 
 *   given status. Memory for the object is allocated.
 ************************************************************/
void*
r_Object::operator new( size_t size, r_Database* /*database*/, ObjectStatus status, const r_OId& oid )
{
  RMDBCLASS( "r_Object", "operator new( size_t, r_Database, ObjectStatus )", "rasodmg", __FILE__, __LINE__ )

  r_Object::next_object_type      = persistent_object;
  r_Object::next_object_status    = status;
  r_Object::next_object_type_name = 0;
  r_Object::next_object_oid       = oid;
  
  void* a = mymalloc( size );  
  return a;  
}



const r_Type*
r_Object::get_type_schema()
{
  if( !type_schema )
  {
    // If type structure not known then try to get it from the server
    if ( (type_structure == NULL) || (strlen(type_structure) == 0) )
    {
      ClientComm::r_Type_Type typeType = (ClientComm::r_Type_Type)0;

      // we need an open database and an active transaction
      if ( r_Database::actual_database == NULL ) return NULL;
      else
      {
        if ( r_Database::actual_database->get_status() == r_Database::not_open )
	  return NULL;
      }
      if ( r_Transaction::actual_transaction == NULL) return NULL;
      else
      {
        if ( r_Transaction::actual_transaction->get_status() != r_Transaction::active )
	  return NULL;
      }

      // set the object type and contact the database if the type name is defined.
      if ( internal_obj_type == 1 ) typeType = ClientComm::r_MDDType_Type;
      else if ( internal_obj_type == 2 ) typeType = ClientComm::r_SetType_Type;
      if ( (type_name == NULL) || (strlen(type_name) == 0) || (typeType == 0) )
	return NULL;
      
      try {
         type_structure = r_Database::actual_database->getComm()->getTypeStructure( type_name, typeType );
      }
      catch(r_Error& errObj) {
         RMInit::logOut << "r_Object::get_type_schema() failed retriving typestructure" << std::endl;
	 RMInit::logOut << "Error " << errObj.get_errorno() << " : " << errObj.what() << std::endl;
         return NULL;
      }
    }

    type_schema = r_Type::get_any_type( type_structure );
  }
  return type_schema;
}



void
r_Object::update_obj_in_db()
{
  RMInit::logOut << " dummy implementation " << std::flush;
}



void
r_Object::load_obj_from_db()
{
  RMInit::logOut << " dummy implementation " << std::flush;
}



void
r_Object::delete_obj_from_db()
{
  RMDBCLASS( "r_Object", "delete_obj_from_db()", "rasodmg", __FILE__, __LINE__ )
     
  if( object_name && strlen( object_name ) )
  {
    RMInit::logOut << object_name << "... " << std::flush;
    
    // delete myself from the database
    r_Database::actual_database->getComm()->deleteCollByName( object_name );
  }
  else
  { 
    RMInit::logOut << "no name - take oid ... " << std::flush;

    if( oid.get_local_oid() )
      // delete myself from the database
      r_Database::actual_database->getComm()->deleteObjByOId( oid );
    else
      RMInit::logOut << " no oid ... FAILED" << std::flush;   
  }
}


void
r_Object::initialize_oid( const r_OId& initOId )
{
  oid = initOId;
}
    
