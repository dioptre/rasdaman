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
 * SOURCE:   oqlquery.cc
 *
 * MODULE:   rasodmg
 * CLASS:    r_OQL_Query
 * FUNCTION: r_oql_execute()
 *
 * COMMENTS:
 *		None
*/

static const char rcsid[] = "@(#)rasodmg, r_OQL_Query and r_oql_execute(): $Id: oqlquery.cc,v 1.25 2002/08/28 12:21:57 coman Exp $";

#include "rasodmg/oqlquery.hh"

#include <string.h>
#include <ctype.h>     // isdigit()
#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <strstream>
#endif

#ifdef __VISUALC__
#ifndef __EXECUTABLE__
#define __EXECUTABLE__
#define OQLQUERY_NOT_SET
#endif
#endif

#include "rasodmg/database.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/set.hh"
#include "rasodmg/gmarray.hh"
  
#include "raslib/rmdebug.hh"
#include "clientcomm/clientcomm.hh"

#ifdef OQLQUERY_NOT_SET
#undef __EXECUTABLE__
#endif
 
r_OQL_Query::r_OQL_Query()
  : queryString(0),
    parameterizedQueryString(0),
    mddConstants(0)
{
}


r_OQL_Query::r_OQL_Query( const char* s )
  : queryString(0),
    mddConstants(0)
{
  parameterizedQueryString = new char[strlen(s)+1];
  strcpy( parameterizedQueryString, s );
  
  reset_query();
}


r_OQL_Query::r_OQL_Query( const r_OQL_Query& q )
  : queryString(0),
    parameterizedQueryString(0),
    mddConstants(0)
{
  // copy the query string
  if(q.queryString) {
	queryString = new char[strlen(q.queryString)+1];
	strcpy( queryString, q.queryString); 
  }

  // copy the parameterized query string
  if(q.parameterizedQueryString) {
	parameterizedQueryString = new char[strlen(q.parameterizedQueryString)+1];
	strcpy( parameterizedQueryString, q.parameterizedQueryString );
  }

  if( q.mddConstants ) {
    mddConstants = new r_Set< r_GMarray* >( *(q.mddConstants) );
  }
}


r_OQL_Query::~r_OQL_Query()
{
  if( queryString )
    delete[] queryString;
  queryString = 0;
  
  if( parameterizedQueryString )
    delete[] parameterizedQueryString;
  parameterizedQueryString = 0;

  if( mddConstants )
    delete mddConstants;
  mddConstants = 0;
}


const r_OQL_Query& 
r_OQL_Query::operator=( const r_OQL_Query& q )
{
  if( this != &q )
  {
    // clean up and copy the query string

    if( queryString )
    {
      delete[] queryString;
      queryString = 0;
    }

    if( q.queryString )
    {
      queryString = new char[strlen(q.queryString)+1];
      strcpy( queryString, q.queryString ); 
    }

    if( mddConstants )
    {
      delete mddConstants;
      mddConstants = 0;
    }

    if( q.mddConstants )
    {
      mddConstants = new r_Set< r_GMarray* >( *(q.mddConstants) );
    }

    // clean up and copy the parameterized query string  
    if( parameterizedQueryString )
    {
      delete[] parameterizedQueryString;
      parameterizedQueryString = 0;
    }

    if( q.parameterizedQueryString )
    {
      parameterizedQueryString = new char[strlen(q.parameterizedQueryString)+1];
      strcpy( parameterizedQueryString, q.parameterizedQueryString );
    }
  }
  
  return *this;
}
   

r_OQL_Query&
r_OQL_Query::operator<<( const char* s ) throw( r_Error )
{
  try
  {
    replaceNextArgument( s );
  }
  catch( ... )
  {
    throw;
  }

  return *this;
}
   
   
r_OQL_Query&
r_OQL_Query::operator<<( r_Char c ) throw( r_Error )
{
  char valueString[2];
  
  valueString[0] = c;
  valueString[1] = '\0';
   
  try
  {
    replaceNextArgument( valueString );
  }
  catch( ... )
  {
    throw;
  }

  return *this;
}
   
   
r_OQL_Query&
r_OQL_Query::operator<<( r_Short s ) throw( r_Error )
{
  char valueString[256];
  
  std::ostrstream valueStream( valueString, 256 ); 
   
  valueStream << s << std::ends;

  try
  {
    replaceNextArgument( valueString );
  }
  catch( ... )
  {
    throw;
  }

  return *this;
}
   
   
r_OQL_Query&
r_OQL_Query::operator<<( r_UShort us ) throw( r_Error )
{
  char valueString[256];
  
  std::ostrstream valueStream( valueString, 256 ); 
   
  valueStream << us << std::ends;

  try
  {
    replaceNextArgument( valueString );
  }
  catch( ... )
  {
    throw;
  }

  return *this;
}
   
   
r_OQL_Query&
r_OQL_Query::operator<<( r_Long l ) throw( r_Error )
{
  char valueString[256];
  
  std::ostrstream valueStream( valueString, 256 ); 
   
  valueStream << l << std::ends;

  try
  {
    replaceNextArgument( valueString );
  }
  catch( ... )
  {
    throw;
  }

  return *this;
}
   
   
r_OQL_Query&
r_OQL_Query::operator<<( r_ULong ul ) throw( r_Error )
{
  char valueString[256];
  
  std::ostrstream valueStream( valueString, 256 ); 
   
  valueStream << ul << std::ends;

  try
  {
    replaceNextArgument( valueString );
  }
  catch( ... )
  {
    throw;
  }

  return *this;
}
   
   
r_OQL_Query&
r_OQL_Query::operator<<( r_Point pt ) throw( r_Error )
{
  char valueString[256];
  
  std::ostrstream valueStream( valueString, 256 ); 
   
  valueStream << pt << std::ends;

  try
  {
    replaceNextArgument( valueString );
  }
  catch( ... )
  {
    throw;
  }

  return *this;
}
   
   
r_OQL_Query&
r_OQL_Query::operator<<( r_Sinterval in ) throw( r_Error )
{
  char valueString[256];
  
  std::ostrstream valueStream( valueString, 256 ); 
   
  valueStream << in << std::ends;

  try
  {
    replaceNextArgument( valueString );
  }
  catch( ... )
  {
    throw;
  }

  return *this;
}
   
   
r_OQL_Query&
r_OQL_Query::operator<<( r_Minterval in ) throw( r_Error )
{
  char valueString[256];
  
  std::ostrstream valueStream( valueString, 256 ); 
   
  valueStream << in << std::ends;

  try
  {
    replaceNextArgument( valueString );
  }
  catch( ... )
  {
    throw;
  }

  return *this;
}
   
   
r_OQL_Query&
r_OQL_Query::operator<<( r_GMarray& in ) throw( r_Error )
{
  // determine number of next mdd (starting with 0)
  unsigned long mddNo = 0;
  if( mddConstants )
    mddNo = mddConstants->cardinality();

  char valueString[256];  
  std::ostrstream valueStream( valueString, 256 ); 
  valueStream << "#MDD" << mddNo << "#" << std::ends;

  try
  {
    replaceNextArgument( valueString );
  }
  catch( ... )
  {
    throw;
  }

  // save reference to in
  if( !mddConstants )
    mddConstants = new r_Set< r_GMarray* >();

  mddConstants->insert_element( &in ); 

  return *this;
}
   
   


int
r_OQL_Query::is_update_query() const
{
  return !is_retrieval_query();
}



int
r_OQL_Query::is_retrieval_query() const
{
  int returnValue = 0;

  if( parameterizedQueryString )
    returnValue = strstr( parameterizedQueryString, "select" ) ||
                  strstr( parameterizedQueryString, "SELECT" );

  return returnValue;
}

    
 
void
r_OQL_Query::reset_query()
{
  if( queryString )
    delete[] queryString;
  
  queryString = new char[strlen(parameterizedQueryString)+1];
  strcpy( queryString, parameterizedQueryString );

  if( mddConstants )
  {
    delete mddConstants;
    mddConstants = 0;
  }
}


void
r_OQL_Query::replaceNextArgument( const char* valueString )
  throw( r_Error )
{
  char* argumentBegin=NULL;
  char* argumentEnd=NULL;
  char* argumentVal=NULL;
  int   argumentLength=0;
  char* tmpString=NULL;
  int   length=0;
          
  // locate the next argument in the query string

  argumentBegin = argumentEnd = strchr( queryString, '$' );
  
  if( !argumentBegin )
  {
    r_Error err = r_Error( r_Error::r_Error_QueryParameterCountInvalid );
    throw err;
  }

  //  while( *argumentEnd != ' ' && *argumentEnd != '\0' )
  argumentEnd++;
  
  //is digit or invalid argument format
  if(!isdigit(*argumentEnd))
     throw  r_Error( QUERYPARAMETERINVALID );
  
  while( isdigit(*argumentEnd) && *argumentEnd != ' ' && *argumentEnd != '\0' )
    argumentEnd++;

  argumentLength = argumentEnd - argumentBegin;
  argumentVal	 = new char[ argumentLength + 1];
  strncpy(argumentVal, argumentBegin, argumentLength);
  argumentVal[argumentLength] = '\0';

  while(true)
  {
   // allocate a new query string and fill it
   tmpString   = queryString;
   length      = strlen(queryString) - argumentLength + strlen(valueString);
   queryString = new char[ length + 1 ];
  
   *argumentBegin = '\0';
   std::ostrstream queryStream( queryString, length + 1 ); 
   
   queryStream << tmpString << valueString << argumentEnd << std::ends;
  
   //update the reference
   argumentEnd=queryString+strlen(tmpString)+strlen(valueString);
   
   //remove buffer
   delete[] tmpString;

   //search again for this parameter   
   argumentEnd = argumentBegin = strstr( argumentEnd, argumentVal );

   //end string?
   if(argumentBegin == NULL)
     break;

   //skip $
   argumentEnd++;
  
   //is digit or invalid argument format
   if(!isdigit(*argumentEnd))
   {
     delete [] argumentVal;
     throw  r_Error( QUERYPARAMETERINVALID );
   }
   
   //skip digits
   while( isdigit(*argumentEnd) && *argumentEnd != ' ' && *argumentEnd != '\0' )
     argumentEnd++;     
  }
  
  delete[] argumentVal;
}



// HP COMPILER BUG
// The iterator type needs a typedef because otherwise the compiler creates a duplicate
// definition for each instance of the following template function.
typedef r_Iterator<r_GMarray*> r_Iterator_r_GMarray; 

void r_oql_execute( r_OQL_Query& query, r_Set< r_Ref_Any > &result )
  throw( r_Error )
{
  if( r_Database::actual_database == 0 || r_Database::actual_database->get_status() == r_Database::not_open )
  {
    r_Error err = r_Error( r_Error::r_Error_DatabaseClosed );
    throw err;
  }
     
  if( r_Transaction::actual_transaction == 0 || r_Transaction::actual_transaction->get_status() != r_Transaction::active )
  {
    r_Error err = r_Error( r_Error::r_Error_TransactionNotOpen );
    throw err;
  }

  try
  {
    r_Database::actual_database->getComm()->executeQuery( query, result );
  }   
  catch( ... )
  {
    throw;  // re-throw the exception
  }
  
  // reset the arguments of the query object
  query.reset_query();
}



void r_oql_execute( r_OQL_Query& query, r_Set< r_Ref< r_GMarray > > &result )
  throw( r_Error )
{
  if( r_Database::actual_database == 0 || r_Database::actual_database->get_status() == r_Database::not_open )
  {
    r_Error err = r_Error( r_Error::r_Error_DatabaseClosed );
    throw err;
  }
     
  if( r_Transaction::actual_transaction == 0 || r_Transaction::actual_transaction->get_status() != r_Transaction::active )
  {
    r_Error err = r_Error( r_Error::r_Error_TransactionNotOpen );
    throw err;
  }

  try
  {
    r_Set< r_Ref_Any > genericSet;
    

    r_Database::actual_database->getComm()->executeQuery( query, genericSet );

    const r_Type* typeSchema = genericSet.get_element_type_schema();

    if( !typeSchema || typeSchema->type_id() != r_Type::MARRAYTYPE )
    {
	  r_Error err( r_Error::r_Error_TypeInvalid );
	  throw err;
	}
    
    //
    // iterate through the generic set and build a specific one
    //
    result.set_type_by_name( genericSet.get_type_name() );
    result.set_type_structure( genericSet.get_type_structure() );

    r_Iterator< r_Ref_Any > iter;
    for( iter=genericSet.create_iterator(); iter.not_done(); iter++ )
      result.insert_element( r_Ref<r_GMarray>(*iter) );      
  }   
  catch( ... )
  {
    throw;  // re-throw the exception
  }
  
  // reset the arguments of the query object
  query.reset_query();
}



void r_oql_execute( r_OQL_Query& query )
  throw( r_Error )
{
  if( r_Database::actual_database == 0 || r_Database::actual_database->get_status() == r_Database::not_open )
  {
    r_Error err = r_Error( r_Error::r_Error_DatabaseClosed );
    throw err;
  }
     
  if( r_Transaction::actual_transaction == 0 || r_Transaction::actual_transaction->get_status() != r_Transaction::active )
  {
    r_Error err = r_Error( r_Error::r_Error_TransactionNotOpen );
    throw err;
  }

  try
  {
    r_Database::actual_database->getComm()->executeQuery( query );
  }   
  catch( ... )
  {
    throw;  // re-throw the exception
  }
  
  // reset the arguments of the query object
  query.reset_query();
}

