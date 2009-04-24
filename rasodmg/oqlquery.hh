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
 * INCLUDE:  oqlquery.hh
 *
 * MODULE:   rasodmg
 * CLASS:    r_OQL_Query
 * FUNCTION: r_oql_execute() 
 *
 * COMMENTS:
 *		None
*/

#ifndef _D_OQL_QUERY_
#define _D_OQL_QUERY_

#include "raslib/error.hh"

#include "raslib/odmgtypes.hh"
#include "rasodmg/marray.hh"
#include "raslib/point.hh"
#include "raslib/sinterval.hh"
#include "raslib/minterval.hh"

template <class T> class r_Set;
template <class T> class r_Ref;
class r_Ref_Any;

//@ManMemo: Module: {\bf rasodmg}

/*@Doc:

 The global function \Ref{r_oql_execute} is used to invoke RasML
 queries. The query statement is represented through an object
 of class \Ref{r_OQL_Query} which is the first argument of the function.
 The constructor gets a parameterized query string where #$i#
 indicates the i-th parameter. The overloaded stream input
 operators allows to insert the parameter values to the query, at
 the same time preserving their respective types. If any of the 
 #$i# are not followed by a right operant construction argument at
 the point \Ref{r_oql_execute} is called, a \Ref{r_Error} exception object 
 of kind {\tt r_Error_QueryParameterCountInvalid} is thrown.
 Once a query has been executed via \Ref{r_oql_execute}, the arguments
 associated with the #$i# parameters are cleared and new arguments
 must be supplied.
 
 The copy constructor and assignment operator copy all the underlying
 data structures associated with the query, based upon the parameters
 that have been passed to the query at the point the operation is
 performed.
 
 The stream operators raise a \Ref{r_Error} exception of type 
 {\tt r_Error_QueryParameterCountInvalid} if the number of arguments is 
 exceeded.
 
*/

class r_OQL_Query
{
  public:
    /// default constructor
    r_OQL_Query();
    
    /// constructor getting the query string
    r_OQL_Query( const char* s );
    
    /// copy constructor
    r_OQL_Query( const r_OQL_Query& q );
    
    /// destructor
    ~r_OQL_Query();
    
    /// assignment operator
    const r_OQL_Query& operator=( const r_OQL_Query& q );
    
    //@Man: Stream input operators for every parameter type:
    //@{
    ///
    r_OQL_Query& operator<<( const char* s )   throw( r_Error );
    ///
    r_OQL_Query& operator<<( r_Char c )        throw( r_Error );
    ///
    r_OQL_Query& operator<<( r_Short s )       throw( r_Error );
    ///
    r_OQL_Query& operator<<( r_UShort us )     throw( r_Error );
    ///
    r_OQL_Query& operator<<( r_Long l )        throw( r_Error );
    ///
    r_OQL_Query& operator<<( r_ULong ul )      throw( r_Error );
    ///
    r_OQL_Query& operator<<( r_Point pt )      throw( r_Error );
    ///
    r_OQL_Query& operator<<( r_Sinterval in )  throw( r_Error );
    ///
    r_OQL_Query& operator<<( r_Minterval in )  throw( r_Error );
    ///
    r_OQL_Query& operator<<( r_GMarray& in )   throw( r_Error );
    ///
    //@}

    /// returns true if the current query is an update one
    int is_update_query() const;

    /// returns true if the current query is an update one
    int is_retrieval_query() const;
    
    //@Man: Methods for internal use:
    //@{
    /// resets the expandation of the query string
    void               reset_query();
    /// gets the expanded query string
    inline const char* get_query() const;
    /// get mdd constants
    inline const r_Set< r_GMarray* >* get_constants() const;
    /// gets the parameterized query string
    inline const char* get_parameterized_query() const;
    ///
    //@}
    
  private:
    /// method replaces the next argument with the delivered valueString
    void replaceNextArgument( const char* valueString ) throw( r_Error );

    /// storage for the expanded query string
    char* queryString;
    
    /// storage for the parameterized query string
    char* parameterizedQueryString;

    /// list for MDD constants
    r_Set< r_GMarray* >* mddConstants;
};




//@ManMemo: Module: {\bf rasodmg}

/*@Doc:
  The free standing function \Ref{r_oql_execute} is called to execute a retrieval query. 
  The first parameter, {\tt query}, is a reference to a \Ref{r_OQL_Query} object specifying
  the query to execute. The second parameter, {\tt result}, is used for returning the
  result of the query. The query result is of type {\tt r_Set< r_Ref_Any >}. 
  
  If the function is not called within the scope of an opened database, a \Ref{r_Error}
  exception of kind {\tt r_Error_DatabaseClosed} is raised. If it is called outside any
  transaction, the exception is of kind {\tt r_Error_TransactionNotOpen}.
  
  A complete list of all possible error kinds is given by the following table.
  
  \begin{tabular}{lll}
  r_Error_ClientUnknown              && Client is not known by the server (earlier communication problems).\\
  r_Error_DatabaseClosed             && No database is not opened.\\
  r_Error_TransactionNotOpen         && Call is not within an active transaction.\\
  r_Error_QueryParameterCountInvalid && At least one of the query parameters is not supplied with a value.\\
  r_Error_TransferFailed             && Other communication problem. \\
  r_Error_QueryExecutionFailed       && The execution of the query failed (further information is available
  in an error object of type {\tt r_Equery_execution}).\\
  r_Error_TypeInvalid                && Result base type doesn't match the template type. \\
  \end{tabular}      
*/

void r_oql_execute( r_OQL_Query& query, r_Set< r_Ref_Any > &result )
throw( r_Error );


//@ManMemo: Module: {\bf rasodmg}

/*@Doc:
  The funcetion is used to execute retrieval queries with the result set being
  of type {\tt r_Set< r_Ref< r_GMarray > >}. The function is supported for 
  compatibility reasons only. We suggest to use the general function 
  \Ref{r_oql_execute} able to maintain query results of any type.
*/
void r_oql_execute( r_OQL_Query& query, r_Set< r_Ref< r_GMarray > > &result )
throw( r_Error );


//@ManMemo: Module: {\bf rasodmg}

/*@Doc:
  The free standing function \Ref{r_oql_execute} is called to execute an update query. 
  The first parameter, {\tt query}, is a reference to a \Ref{r_OQL_Query} object specifying
  the query to execute.
  
  If the function is not called within the scope of an opened database, a \Ref{r_Error}
  exception of kind {\tt r_Error_DatabaseClosed} is raised. If it is called outside any
  transaction, the exception is of kind {\tt r_Error_TransactionNotOpen}.
  
  A complete list of all possible error kinds is given by the following table.
  
  \begin{tabular}{lll}
  r_Error_ClientUnknown              && Client is not known by the server (earlier communication problems).\\
  r_Error_DatabaseClosed             && No database is not opened.\\
  r_Error_TransactionNotOpen         && Call is not within an active transaction.\\
  r_Error_QueryParameterCountInvalid && At least one of the query parameters is not supplied with a value.\\
  r_Error_TransferFailed             && Other communication problem. \\
  r_Error_QueryExecutionFailed       && The execution of the query failed (further information is available
  in an error object of type {\tt r_Equery_execution}).\\
  \end{tabular}      
*/

void r_oql_execute( r_OQL_Query& query )
throw( r_Error );

#include "rasodmg/oqlquery.icc"

#endif























