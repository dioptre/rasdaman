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
 * INCLUDE:  transaction.hh
 *
 * MODULE:   rasodmg
 * CLASS:    r_Transaction
 *
 * COMMENTS:
 *			None
*/

//@ManMemo: Module: {\bf rasodmg}

#ifndef _D_TRANSACTION_
#define _D_TRANSACTION_

#include "raslib/error.hh"
#include "raslib/oid.hh"

class r_Object;

#include "rasodmg/set.hh"
#include "rasodmg/ref.hh"

/*@Doc:

  Transactions can be started, committed, aborted, and checkpointed. 
  It is important to note that all access, creation, modification, 
  and deletion of persistent objects must be done within a transaction.
  Right now, only one transaction can be active at a time.

*/

class r_Transaction
{
  public:
    /// possible states of the transaction
    enum r_TAStatus { active, inactive, comiting, aborting };
  
    /// possible transaction modes
    enum r_TAMode { read_write, read_only };

    /// default constructor
    r_Transaction();
    
    /// destructor, an active transaction is aborted
    ~r_Transaction();
    
    /// start the transaction
    void begin( r_TAMode mode = read_write ) throw( r_Error );
    /**
      By default, a transaction is started in write mode. If the read_only
      mode is specified, no write operations are allowed within the transaction
      anymore.
      If any write operation occurs in read_only mode, the exception r_Error with
      kind {\tt r_Error_TransactionReadOnly} will be raised and the transaction will
      be aborted.
      In order to achieve maximal performance, read-only transactions should be used
      whenever posssible, i.e., when no update operations occur within this transaction.
    */

    /// commit transaction and make changes persistent
    void commit() throw( r_Error );
    /**
      The transaction is committed and changes are made persistent 
      in the database.
      While committing, the following errors can occur:

      \begin{tabular}{lll}
      r_Error_TransferFailed && Server communication problem.\\
      r_Error_ObjectUnknown && Name of object is unknown.\\
      r_Error_DatabaseClassUndefined && Type name of object not known by the database.\\
      r_Error_CollectionElementTypeMismatch && Collection and MDD type mismatch.\\
      \end{tabular}
    */
    
    /// abort transaction and forget changes within transaction
    void abort();
    
    /// returns the current state
    inline r_TAStatus get_status() const;
    
    /// returns current mode
    inline r_TAMode get_mode() const;
    
    //@Man: Methods and types for internal use only:
    //@{
    ///
    
      /// store a pointer to the actual transaction
      static r_Transaction* actual_transaction;

      /// load an object (internal use only)
      r_Ref_Any load_object( const r_OId& oid );
   
      /// possible non-r_Object values maintained by the transaction
      enum GenRefType { MINTERVAL, SINTERVAL, POINT, OID, SCALAR };

      /// adds a non-r_Object to the list of persistent objects
      void add_object_list( GenRefType type, void* ref );

    ///
    //@} 

  private:
    /// adds an object of type \Ref{r_Object} to the list of persistent objects
    void add_object_list( const r_Ref<r_Object>& );
  
    /// current transaction state
    r_TAStatus ta_state;

    /// current transaction mode (just valid if transaction is active)
    r_TAMode ta_mode;    

    /// list of \Ref{r_Object} references which have been created within the transaction
    r_Set< r_Ref<r_Object> > object_list;
    
    // element type of non \Ref{r_Object} list maintained by the transaction
    typedef struct{ GenRefType type; void* ref; } GenRefElement;

    /// list of non \Ref{r_Object} references which have been created within the transaction
    r_Set< GenRefElement* > non_object_list;

    friend class r_Object;
};

#define DEF_TRANSACTION

#include "rasodmg/iterator.hh"
// For HP cfront compiler each template instantiation used in a library 
// must be defined in an included header.
typedef r_Iterator<r_Object*> r_Iterator_r_Object_dummy;
#include "rasodmg/transaction.icc"

#ifdef EARLY_TEMPLATE
#ifdef __EXECUTABLE__
#ifdef __VISUALC__
#include "rasodmg/ref.cpp"
#else
#include "rasodmg/ref.cc"
#endif
#endif 
#endif


#endif
