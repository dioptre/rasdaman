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
 * SOURCE:   transaction.cc
 *
 * MODULE:   rasodmg
 * CLASS:    r_Transaction
 *
 * COMMENTS:
 *  		None
*/

static const char rcsid[] = "@(#)rasodmg, r_Transaction: $Id: transaction.cc,v 1.38 2005/09/03 20:39:35 rasdev Exp $";


#ifdef __VISUALC__
  #ifndef __EXECUTABLE__
    #define __EXECUTABLE__
    #define TRANSACTION_NOT_SET
  #endif
#endif

#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"

#include "raslib/rmdebug.hh"
#include "raslib/scalar.hh"
#include "clientcomm/clientcomm.hh"

#ifdef __VISUALC__
  #ifdef TRANSACTION_NOT_SET
    #undef __EXECUTABLE__
  #endif

  template class r_Set< r_Ref<r_Object> >;
#endif

#include <iostream>

// Initially there is no transaction active.
r_Transaction* r_Transaction::actual_transaction = 0;



r_Transaction::r_Transaction()
  : ta_state( inactive ), ta_mode( read_write )
{
}
 
   

r_Transaction::~r_Transaction()
{
  if( ta_state == active )
    abort();
}



void
r_Transaction::begin( r_Transaction::r_TAMode mode ) throw( r_Error )
{
  // check if no other transaction is running 
  if( ta_state != inactive || actual_transaction )
  {
    r_Error err = r_Error( r_Error::r_Error_TransactionOpen );
    throw err;
  }

  // check if a database is opened
  if( r_Database::actual_database == 0 )
  {
    r_Error err = r_Error( r_Error::r_Error_DatabaseClosed );
    throw err;
  }

  ta_state = active;

  // if a database is opened, a communication object is existing
  //r_Database::actual_database->communication->openTA( mode == read_only ? 1 : 0 );
  r_Database::actual_database->getComm()->openTA( mode == read_only ? 1 : 0 );
     
  actual_transaction = this;
  ta_mode  = mode;
}



void
r_Transaction::commit() throw( r_Error )
{
  if( ta_state != active )
  {
    throw( new r_Error( r_Error::r_Error_TransactionNotOpen ) );
  }
  else
  {
    RMInit::logOut << std::endl << "Commit Log:" << std::endl;

    //
    // Commit list of r_Object references.
    //

    r_Iterator< r_Ref<r_Object> > iter = object_list.create_iterator();

    for( iter.reset(); iter.not_done(); iter++ )
    {
      if( (*iter)->get_oid().is_valid() )
      {
        RMInit::logOut << "  Object " << (*iter)->get_oid() << "  " << std::flush;

        switch( (*iter)->get_status() )
        {
    	  case r_Object::deleted:
            RMInit::logOut << "state DELETED,  deleting ... " << std::flush;
            (*iter)->r_Object::delete_obj_from_db();
             RMInit::logOut << "OK" << std::endl;
            break;

    	  case r_Object::created:
            RMInit::logOut << "state CREATED,  writing  ... " << std::flush;
            (*iter)->insert_obj_into_db();
	    RMInit::logOut << "OK" << std::endl;
	    break;

	  case r_Object::modified:
            RMInit::logOut << "state MODIFIED, modifying ... " << std::endl;
            (*iter)->update_obj_in_db();
	    break;
	   
 	  case r_Object::read:
	    RMInit::logOut << "state READ,     OK" << std::endl;
            break;
	   
 	  case r_Object::transient:
	    RMInit::logOut << "state TRANSIENT,     OK" << std::endl;
            break;

	  default:
	    RMInit::logOut << "state UNKNOWN" << std::endl;
	    break;
        }
      }
      else
        RMInit::logOut << "  Object with no oid, state TRANSIENT query result" << std::endl;
    }
     
    // Don't do the r_deactivate() in the first loop because if a collection is not
    // before its elements in the list, it tries to save non existing objects.
    
    for( iter.reset(); iter.not_done(); iter++ )
    {       
      if( !(*iter)->test_status( r_Object::deleted ) )
        (*iter)->r_deactivate();
      
      (*iter)->r_Object::r_deactivate();
      
      free( (*iter).get_memory_ptr() );
    }

    object_list.remove_all();
   
    //
    // Commit list of non-r_Object references.
    //

    r_Iterator< GenRefElement* > iter2 = non_object_list.create_iterator();

    for( iter2.reset(); iter2.not_done(); iter2++ )
    {
      RMInit::logOut << "  Value " << std::flush;

      switch( (*iter2)->type )
      {
    	case POINT:
          RMInit::logOut << "transient Point DELETED" << std::endl;
          delete ((r_Point*)(*iter2)->ref);
          break;

    	case SINTERVAL:
          RMInit::logOut << "transient Sinterval DELETED" << std::endl;
          delete ((r_Sinterval*)(*iter2)->ref);
          break;

    	case MINTERVAL:
          RMInit::logOut << "transient Minterval DELETED" << std::endl;
          delete ((r_Minterval*)(*iter2)->ref);
          break;

    	case OID:
          RMInit::logOut << "transient OId DELETED" << std::endl;
          delete ((r_OId*)(*iter2)->ref);
          break;

	default:
          RMInit::logOut << "transient Scalar DELETED" << std::endl;
          delete ((r_Scalar*)(*iter2)->ref);
          break;
      }

      delete *iter2;
    }
     
    non_object_list.remove_all();


    //
    // commit transaction on the server
    //

//    r_Database::actual_database->communication->commitTA();
      r_Database::actual_database->getComm()->commitTA();
         
    ta_state = inactive;
    actual_transaction = 0;
  }
}
    


void
r_Transaction::abort()
{
  if( ta_state != active )
  {
    throw( r_Error( r_Error::r_Error_TransactionNotOpen ) );
  }
  else
  {
    RMInit::logOut << std::endl << "Abort Log:" << std::endl;

    //
    // Abort list of r_Object references.
    //

    r_Iterator< r_Ref<r_Object> > iter = object_list.create_iterator();

    for( iter.reset(); iter.not_done(); iter++ )
    {
      RMInit::logOut << "  Object DELETED" << std::endl;

      if( !(*iter)->test_status( r_Object::deleted ) )
        (*iter)->r_deactivate();
      
      (*iter)->r_Object::r_deactivate();
      
      free( (*iter).get_memory_ptr() );         
    }      
    
    object_list.remove_all();
    
    //
    // Abort list of non-r_Object references.
    //

    r_Iterator< GenRefElement* > iter2 = non_object_list.create_iterator();

    for( iter2.reset(); iter2.not_done(); iter2++ )
    {
      switch( (*iter2)->type )
      {
    	case POINT:
          RMInit::logOut << "  Transient Point DELETED" << std::endl;
          delete ((r_Point*)(*iter2)->ref);
          break;

    	case SINTERVAL:
          RMInit::logOut << "  Transient Sinterval DELETED" << std::endl;
          delete ((r_Sinterval*)(*iter2)->ref);
          break;

    	case MINTERVAL:
          RMInit::logOut << "  Transient Minterval DELETED" << std::endl;
          delete ((r_Minterval*)(*iter2)->ref);
          break;

    	case OID:
          RMInit::logOut << "  Transient OId DELETED" << std::endl;
          delete ((r_OId*)(*iter2)->ref);
          break;

	default:
          RMInit::logOut << "  Transient Scalar DELETED" << std::endl;
          delete ((r_Scalar*)(*iter2)->ref);
          break;
      }

      delete *iter2;
    }
     
    non_object_list.remove_all();

    //
    // Abort transaction on the server.
    //
    if (r_Database::actual_database)
//    	r_Database::actual_database->communication->abortTA();
    	r_Database::actual_database->getComm()->abortTA();
    else
        RMInit::logOut << "  Database was already closed.  Please abort every transaction before closing the database.  Please also check for any try/catches with a close database but without transaction abort." << std::endl;
         
    ta_state = inactive;
    actual_transaction = 0;
  }
}



r_Ref_Any
r_Transaction::load_object( const r_OId& oid )
{
  // check, if object is already loaded
  unsigned int found = 0;
  r_Iterator< r_Ref<r_Object> > iter = object_list.create_iterator();

  iter.reset();
  while( iter.not_done() && !found )
  {
    found = ( (*iter)->get_oid() == oid );
    if( !found ) iter++;
  }

  if( found )
  {
    // return reference of loaded object
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Transaction", "load_object( oid ) - object already loaded")
    return *iter;
  }
  else
  {
    // load object and return reference
    RMDBGONCE(4, RMDebug::module_rasodmg, "r_Transaction", "load_object( oid ) - load object")
    return r_Database::actual_database->lookup_object( oid );
  }
}



void
r_Transaction::add_object_list( const r_Ref<r_Object> &obj )
{
  object_list.insert_element( obj );
}



void
r_Transaction::add_object_list( GenRefType type, void* ref )
{
  GenRefElement* element = new GenRefElement;

  element->type = type;
  element->ref  = ref;

  non_object_list.insert_element( element );
}

