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
 * SOURCE:   database.cc
 *
 * MODULE:   rasodmg
 * CLASS:    r_Database
 *
 * COMMENTS:
 *		None
*/

static const char rcsid[] = "@(#)rasodmg, r_Database: $Id: database.cc,v 1.47 2005/09/03 20:39:35 rasdev Exp $";

#ifdef __VISUALC__
#ifndef __EXECUTABLE__
#define __EXECUTABLE__
#define DATABASE_NOT_SET
#endif
#endif

#include "rasodmg/database.hh"
#include "rasodmg/transaction.hh"
#include "clientcomm/clientcomm.hh"

#ifdef DATABASE_NOT_SET
#undef __EXECUTABLE__
#endif

#include <string.h>

// At the beginning, no database is actually opened.
r_Database* r_Database::actual_database = 0;


r_Database::r_Database()
	: db_status( not_open ),
	  rasmgrName(0),
	  userName(0),
	  plainPass(0),
	  communication(0)
{
}

r_Database::r_Database( const char* name ) throw(r_Error)
	: db_status( not_open ),
	  userName(0),
	  plainPass(0),
	  communication(0)
{
	if(!name)
	{
		RMInit::logOut << "Error: null database name." << std::endl;
		throw r_Error(r_Error::r_Error_NameInvalid);
	}
	this->rasmgrName = strdup( name );
}

r_Type*
r_Database::get_type_schema(const char* typeName, type_schema typeType) throw (r_Error)
{
	r_Type* retval = 0;

	if ((typeName == NULL) || (strlen(typeName) == 0))
		throw r_Error(r_Error::r_Error_NameInvalid);
	else if ((typeType != COLLECTION ) && (typeType != MARRAY))
		throw r_Error(r_Error::r_Error_TypeInvalid);
	else if (r_Database::actual_database != NULL)
		throw r_Error(r_Error::r_Error_DatabaseClosed);
	else if (r_Database::actual_database->get_status() != r_Database::not_open)
		throw r_Error(r_Error::r_Error_DatabaseClosed);
	else if (r_Transaction::actual_transaction != NULL)
		throw r_Error(r_Error::r_Error_TransactionNotOpen);
	else if (r_Transaction::actual_transaction->get_status() == r_Transaction::active)
		throw r_Error(r_Error::r_Error_TransactionNotOpen);
	else
	{
		ClientComm::r_Type_Type type;
		if (typeType == COLLECTION)
			type = ClientComm::r_SetType_Type;
		else 
			type = ClientComm::r_MDDType_Type;
		char* temp = r_Database::actual_database->communication->getTypeStructure( typeName, type );
		retval = r_Type::get_any_type(temp);
		delete [] temp;
		temp = 0;
	}
	return retval;
}

r_Database::~r_Database()
{
	if( db_status != not_open )
		close();
	
	if( rasmgrName )
		free( rasmgrName );
	if( userName )
		free( userName );
	if( plainPass )
		free( plainPass );
}

void
r_Database::open( const char* database_name, access_status new_status )
	throw( r_Error )
{
	if( db_status != not_open || actual_database )
	{
		r_Error err = r_Error(r_Error::r_Error_DatabaseOpen);
		throw err;
	}
	
	if(!database_name)
	{
		RMInit::logOut << "r_Database::open(name, new_status) name is null" << std::endl;
		throw r_Error(r_Error::r_Error_NameInvalid);
	}  
	
	// While instantiating the communication object, the first connection to
	// the server is established. Any exception is given through to the caller
	// of open(...).
	try
	{
		communication = ClientComm::createObject( rasmgrName, rasmgrPort ); 
		if( userName && plainPass )
			communication->setUserIdentification( userName, plainPass );    
	}
	catch( ... )
	{
		if(communication)
			delete communication;
		throw;  // re-throw the exception (r_Error_HostInvalid, r_Error_ServerInvalid)
	}
	
	// open database
	unsigned int status=0;
	try
	{
		status = communication->openDB( (char*)database_name );
	}
	catch( ... )
	{
		if(communication)
			delete communication;  
		throw;
	}
	
	if( status )
	{
		// translate error values into exceptions
		r_Error err;
	
		switch( status )
		{     
			case 1:
				err = r_Error( r_Error::r_Error_ClientUnknown );
				break;      

			case 2:
				err = r_Error( r_Error::r_Error_DatabaseUnknown );
				break;      

			case 3:
				err = r_Error( r_Error::r_Error_DatabaseOpen );
				break;      

			case 4:
				err = r_Error( r_Error::r_Error_RpcInterfaceIncompatible );
				break;
			case CONNECTIONCLOSED:
				err = r_Error(CONNECTIONCLOSED);
				break;	
			default:
				 err = r_Error( r_Error::r_Error_General );
		}
		
		if(communication)
			delete communication;
		
		throw err;
	}

	actual_database = this;
	db_status       = new_status;
}

void
r_Database::close()
{
	if( db_status != not_open )
	{
		// if a communication object exists, close and delete it 
		if( communication )
		{ 
			// abort any open TA -- PB 2005-sep-02
			// This is quite a hack (borrowed from fastscale.cc):
			// Actual transaction is a pointer to this in a TA.
			// Since the TA was allocated by the application program
			// it should be save to use it like this.
			if (r_Transaction::actual_transaction != 0)
			{	// make _very_ sure we have sequential evaluation -> nested ifs
				if (r_Transaction::actual_transaction->get_status() == r_Transaction::active)
					r_Transaction::actual_transaction->abort();
			}

			communication->closeDB();
			delete communication; 
			communication = 0;
		}
		
		db_status = not_open;
		actual_database = 0;
	}  
}

void
r_Database::create( const char* name ) throw( r_Error )
{
	// this operation is not supported through this interface; use rasdl
	throw( r_Error(803)); // Access denied, no permission
}

void
r_Database::destroy( const char* name ) throw( r_Error )
{
	// this operation is not supported through this interface; use rasdl
	throw( r_Error(803)); // Access denied, no permission
}


void
r_Database::set_servername( const char* name, int port ) throw (r_Error)
{ 
	//We let the name of the function as it is, but it's about the rasmgr name
 
	if(!name)
	{   
		RMInit::logOut << "r_Database::set_servername(name, port) name is null" << std::endl;
		throw r_Error(r_Error::r_Error_NameInvalid);
	}       
 
	if( rasmgrName)
		free( rasmgrName );
		
	rasmgrName = strdup( name );    
	rasmgrPort = port;
}
void 
r_Database::set_useridentification( const char* name, const char *plain_pass ) throw(r_Error)
{
	if(!name)
	{ 
		RMInit::logOut << "r_Database::set_useridentification(name, plain_pass) name is null" << std::endl;
		throw r_Error(r_Error::r_Error_NameInvalid);
	}    
	if(!plain_pass)
	{   
		RMInit::logOut << "r_Database::set_useridentification(name, plain_pass) plain_pass is null" << std::endl;
		throw r_Error(r_Error::r_Error_NameInvalid);
	}      

	if (userName)
	free(userName);
	if (plainPass)
	free(plainPass);
	userName  = strdup( name );
	plainPass = strdup( plain_pass );
}    

void
r_Database::set_object_name( r_Object &obj, const char* name ) throw(r_Error)
{
	obj.set_object_name( name );
}

r_Ref_Any
r_Database::lookup_object( const char* name ) const throw( r_Error )
{
	r_Ref_Any returnValue;
 
	if( db_status == not_open )
	{
		r_Error err = r_Error( r_Error::r_Error_DatabaseClosed );
		throw err;
	}

	if(!name)
	{  
		RMInit::logOut << "r_Database::lookup_object(name) name is null" << std::endl;
		throw r_Error(r_Error::r_Error_NameInvalid);
	}  

	if( !r_Transaction::actual_transaction
	  || r_Transaction::actual_transaction->get_status() !=  r_Transaction::active )
	{
		r_Error err = r_Error( r_Error::r_Error_TransactionNotOpen );
		throw err;
	}

	try
	{
		// get collection
		returnValue = communication->getCollOIdsByName( name );
	}
	catch( ... )
	{
		throw;  // re-throw the exception
	}
	
	return returnValue;
}



r_Ref_Any
r_Database::lookup_object( const r_OId& oid ) const throw( r_Error )
{
	r_Ref_Any returnValue;
 
	if( db_status == not_open )
	{
		r_Error err = r_Error( r_Error::r_Error_DatabaseClosed );
		throw err;
	}

	if( !r_Transaction::actual_transaction 
	  || r_Transaction::actual_transaction->get_status() !=  r_Transaction::active )
	{
		r_Error err = r_Error( r_Error::r_Error_TransactionNotOpen );
		throw err;
	}

	try
	{
		// determine type of object and get it
		if( communication->getObjectType( oid ) == 1 )
			returnValue = communication->getMDDByOId( oid );
		else
			returnValue = communication->getCollOIdsByOId( oid );
	}
	catch( ... )
	{
		throw;  // re-throw the exception
	}
	
	return returnValue;
}


void
r_Database::set_transfer_format( r_Data_Format format, const char *formatParams ) throw( r_Error )
{
	unsigned short result;

	if ( db_status == not_open )
	{
		r_Error err = r_Error( r_Error::r_Error_DatabaseClosed );
		throw (err);
	}
	//keeps from crashing in rpc on linux
	if (formatParams == 0)
		formatParams = "";
	result = communication->setTransferFormat(format, formatParams);

	switch (result)
	{
		case 1:
			{
				r_Error err = r_Error( r_Error::r_Error_ClientUnknown );
				throw(err);
			}
			break;
		case 2:
			{
				r_Error err = r_Error( r_Error::r_Error_FeatureNotSupported );
				throw(err);
			}
			break;
		default:
			break;
	}
}

void
r_Database::set_storage_format( r_Data_Format format, const char *formatParams ) throw( r_Error )
{
	unsigned short result;

	if ( db_status == not_open )
	{
		r_Error err( r_Error::r_Error_DatabaseClosed );
		throw(err);
	}
	
	//keeps from crashing in rpc on linux
	if (formatParams == 0)
		formatParams = "";  
	
	result = communication->setStorageFormat(format, formatParams);

	switch (result)
	{
		case 1:
			{
				r_Error err( r_Error::r_Error_ClientUnknown );
				throw(err);
			}
			break;
		case 2:
			{
				r_Error err( r_Error::r_Error_FeatureNotSupported );
				throw(err);
			}
			break;
		default:
			break;
	}
}



const r_OId
r_Database::get_new_oid( unsigned short objType ) const throw(r_Error)
{
	return communication->getNewOId( objType );
}


void r_Database::insertColl( const char* collName, const char* typeName, const r_OId& oid ) throw( r_Error )
{
	communication->insertColl(collName,typeName,oid );
}

void r_Database::removeObjFromColl( const char* name, const r_OId& oid ) throw ( r_Error )
{
	communication->removeObjFromColl(name,oid);
}

ClientComm* r_Database::getComm()
{
	return communication;
}












