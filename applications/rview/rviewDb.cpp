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
 *  Purpose:
 *
 *  Database abstraction layer. All database accesses are performed though
 *  the functions provided here. This includes creating / deleting / looking
 *  up collections, inserting objects into collections and executing RasQL
 *  queries.
 *
 *  COMMENTS:
 *  		None
 */



// Standard wxWindows preamble.
#ifdef __GNUG__
#pragma implementation
#endif


// changed in wxWindows 2.4.2:
//#include "wx_prec.h"
#include <wx/wxprec.h>


#ifdef __BORLANDC__
#pragma hdrstop
#endif


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include "wb_timer.h"



#include <string.h>


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "raslib/scalar.hh"
#include "raslib/rmdebug.hh"
#include "rasodmg/fastscale.hh"


#include "rviewTypes.hh"

#include "labelManager.hh"

#include "rviewDb.hh"
#include "rviewMDD.hh"
#include "rviewPrefs.hh"



rviewDatabase::rviewDatabase(void)
{
  dbOpen = FALSE;
  lastTransferFormat = r_Array;
  lastStorageFormat = r_Array;
}


rviewDatabase::~rviewDatabase(void)
{
  if (dbOpen)
  {
    close();
  }
}


bool rviewDatabase::isOpen(void)
{
  return dbOpen;
}



int rviewDatabase::open(const char *srvname, int srvport, const char *dbname,
			const char *usrname, const char *usrpassword)
{
  int status;

  if (!dbOpen)
  {
    server 	 = srvname; 
    port	 = srvport;
    database 	 = dbname;
    username 	 = usrname; 
    userpassword = usrpassword;
    
  }
  else
  {
    rviewErrorbox::reportError(lman->lookup("errorDatabaseOpen"));
    return 0;
  }

  rviewProgress *prog = new rviewProgress(lman->lookup("progOpenDb"));

  try
  {
    ::wxStartTimer();

    dbase.set_servername(server,port);
    dbase.set_useridentification(username, userpassword);
    dbase.open(database);
#ifdef RMANDEBUG
    cout << "Open Database Time: " << ::wxGetElapsedTime(TRUE) << "ms" << endl;
#endif
    dbOpen = TRUE;
    status = 1;
  }
  catch ( r_Error &errObj )
  {
/*
    char *errLab;
    cerr << errObj.what() << endl;
    switch ( errObj.get_kind() )
    {
      case r_Error::r_Error_HostInvalid: errLab = "\\errorHostInvalid"; break;
      case r_Error::r_Error_ServerInvalid: errLab = "\\errorServerInvalid"; break;
      case r_Error::r_Error_ClientUnknown: errLab = "\\errorClientUnknown"; break;
      case r_Error::r_Error_DatabaseUnknown: errLab = "\\errorDatabaseUnknown"; break;
      case r_Error::r_Error_DatabaseOpen: errLab = "\\errorDatabaseOpen"; break;
      case r_Error::r_Error_RpcInterfaceIncompatible: errLab = "\\errorRpcInterface"; break;
      default: errLab = "\\errorUnknown"; break;
    } 
*/

    rviewErrorbox::reportError((const char*)errObj.what());
    status = 0;
  }

  prog->Close(TRUE);

  return status;
}



void rviewDatabase::close(void)
{
//  if (ensureDatabase() == 0) return;
  dbase.close();
  dbOpen = FALSE;
}



int rviewDatabase::collectionToDesc(r_Set<r_Ref<r_GMarray> > &mddColl, collection_desc *desc)
{
  int i, collMembers;
  rviewBaseType bt = rbt_none;
  r_Object *mo = NULL;

  desc->number = 0; desc->mddObjs = NULL; desc->strObjs = NULL; desc->collType = NULL;

  collMembers = mddColl.cardinality();

  if (collMembers <= 0)
  {
    return 1;
  }

  if ((desc->mddObjs = new mdd_frame[collMembers]) == NULL)
  {
    cerr << lman->lookup("errorMemory") << endl;
    return 0;
  }

  r_Iterator < r_Ref <r_GMarray> >	iterator = mddColl.create_iterator();

  for (i=0; i<collMembers; i++, iterator++)
  {
    RMDBGONCE(3, RMDebug::module_applications, "rviewDb", "collectionToDesc() MDD number " << i << ":\tDomain = " << (*iterator)->spatial_domain() );

    // Copy each MDD object
    desc->mddObjs[i].mdd = new r_GMarray((const r_GMarray &)(*(*iterator)));
    // oid isn't copied by copy constructor...
    desc->mddObjs[i].mdd->initialize_oid((*iterator)->get_oid());
    if (desc->mddObjs[i].mdd.is_null())
    {
      cerr << lman->lookup("errorMemory") << endl;
    }
    desc->mddObjs[i].flags = 0;
    // Set type information?
    if (mo == NULL)
    {
      // we must use the original mdd data to get the type information!
      mo = (r_Object*)(&(**iterator));
      bt = rviewGetBasetype(mo);
    }
  }

  desc->number = collMembers;

  desc->collType = new char[strlen(rviewBaseTypes[bt]) + 1];
  strcpy(desc->collType, rviewBaseTypes[bt]);

  return 1;
}



int rviewDatabase::lookupCollection(collection_desc *desc)
{
  r_Ref < r_Set < r_Ref <r_GMarray > > >	mddCollPtr;
  r_Transaction					transaction;
  int status;
  char buffer[STRINGSIZE];

  status = 0;
  rviewProgress *prog = new rviewProgress(lman->lookup("progLookup"));

  try
  {
    ::wxStartTimer();

    transaction.begin( r_Transaction::read_only );

    //set storage & transfer params
    if (!ensureDatabase()) 
    {
     transaction.abort();
     return 0;
    }

    mddCollPtr = dbase.lookup_object(desc->collName);

    status = collectionToDesc(*mddCollPtr, desc);

    transaction.commit();

    sprintf(buffer, "%s: %d ms", lman->lookup("textTime"), ::wxGetElapsedTime(TRUE));
    if ((desc->collInfo = new char[strlen(buffer) + 1]) != NULL)
    {
      strcpy(desc->collInfo, buffer);
    }
#ifdef RMANDEBUG
    cout << "Lookup collection " << buffer << endl;
#endif
  }

  catch ( r_Error &errObj )
  {
    const char *msg;

    cerr << errObj.what() << endl;

    if (errObj.get_errorno() != 0)
    {
      msg = errObj.what();
    }
    else
    {
      switch (errObj.get_kind())
      {
	case r_Error::r_Error_ClientUnknown: msg = lman->lookup("errorClientUnknown"); break;
	case r_Error::r_Error_DatabaseClosed: msg = lman->lookup("errorDatabaseClosed"); break;
	case r_Error::r_Error_QueryParameterCountInvalid: msg = lman->lookup("errorQueryParamNum"); break;
	case r_Error::r_Error_TransferFailed: msg = lman->lookup("errorTransferFailed"); break;
	default: msg = lman->lookup("errorUnknown"); break;
      }
    }
    rviewErrorbox::reportError(msg);
  }

  prog->Close(TRUE);

  return status;
}


r_Ref<r_GMarray> rviewDatabase::getScaledObject(r_Fast_Base_Scale *scaler, const r_Minterval &trimDomain, double scale)
{
  r_Transaction ta;
  r_Ref<r_GMarray> result;

  try
  {
    ta.begin(r_Transaction::read_only);

    //FIXME
/*
    //set storage & transfer params
    if (!ensureDatabase())
    {
     ta.abort();
     return 0;
    }
*/
    // apply trimming before scaling!
    result = scaler->get_scaled_object(trimDomain, scale, 1);
    ta.commit();
  }
  catch(r_Error &err)
  {
    ta.abort();
    rviewErrorbox::reportError(err.what());
  }
  // if the transaction failed, result.is_null() will deliver TRUE.
  return result;
}


r_Fast_Base_Scale *rviewDatabase::lookupScaledObject(collection_desc *desc, double scale)
{
  r_Transaction ta;
  r_Fast_Base_Scale *result;
  rviewBaseType baseType;
  char buffer[STRINGSIZE];



  desc->strObjs = NULL;

  rviewProgress *prog = new rviewProgress(lman->lookup("progLookup"));

  try
  {
    ta.begin(r_Transaction::read_only);


    //set storage & transfer params
    if (!ensureDatabase())
    {
     ta.abort();
     return 0;
    }

    r_Ref<r_GMarray> minArray = r_Fast_Base_Scale::get_minimal_array(desc->collName);
    ta.commit();
    baseType = rviewGetBasetype(minArray.ptr());
    minArray.destroy();
  }
  catch(r_Error &err)
  {
    ta.abort();
    rviewErrorbox::reportError(err.what());
    prog->Close(TRUE);
    return NULL;
  }

  try
  {
    ta.begin(r_Transaction::read_only);


    //set storage & transfer params
    if (!ensureDatabase())
    {
     ta.abort();
     return 0;
    }

    switch (baseType)
    {
      case rbt_bool:
	result = new r_Fast_Scale<r_Boolean>(desc->collName);
	break;
      case rbt_char:
	result = new r_Fast_Scale<r_Char>(desc->collName);
	break;
      case rbt_uchar:
	result = new r_Fast_Scale<r_Octet>(desc->collName);
	break;
      case rbt_short:
	result = new r_Fast_Scale<r_Short>(desc->collName);
	break;
      case rbt_ushort:
	result = new r_Fast_Scale<r_UShort>(desc->collName);
	break;
      case rbt_long:
	result = new r_Fast_Scale<r_Long>(desc->collName);
	break;
      case rbt_ulong:
	result = new r_Fast_Scale<r_ULong>(desc->collName);
	break;
      case rbt_float:
	result = new r_Fast_Scale<r_Float>(desc->collName);
	break;
      case rbt_double:
	result = new r_Fast_Scale<r_Double>(desc->collName);
	break;
      case rbt_rgb:
	result = new r_Fast_Scale<RGBPixel>(desc->collName);
	break;
      default:
	rviewErrorbox::reportError(lman->lookup("errorScaledObjType"));
	prog->Close(TRUE);
	return NULL;
    }
    ta.commit();
  }
  catch(r_Error &err)
  {
    ta.abort();
    rviewErrorbox::reportError(err.what());
    prog->Close(TRUE);
    return NULL;
  }

  r_Ref<r_GMarray> mddObj;

  ::wxStartTimer();

  mddObj = getScaledObject(result, result->get_full_domain(), scale);
  if (mddObj.is_null())
  {
    delete result;
    prog->Close(TRUE);
    return NULL;
  }

  sprintf(buffer, "%s: %d ms", lman->lookup("textTime"), ::wxGetElapsedTime(TRUE));
  if ((desc->collInfo = new char[strlen(buffer) + 1]) != NULL)
  {
    strcpy(desc->collInfo, buffer);
  }
  desc->collType = new char[strlen(rviewBaseTypes[baseType]) + 1];
  strcpy(desc->collType, rviewBaseTypes[baseType]);
  desc->mddObjs = new mdd_frame[1];
  desc->number = 1;
  desc->mddObjs[0].mdd = mddObj; desc->mddObjs[0].flags = 0;

  prog->Close(TRUE);

  return result;
}


int rviewDatabase::createCollection(const char *collName, rviewBaseType bt)
{
  r_Ref <r_Set <r_Ref <r_GMarray> > > mddCollPtr;
  r_Transaction ta;

  try
  {
    mddCollPtr = dbase.lookup_object(collName);
  }
  catch (...)
  {
    RMDBGONCE(3, RMDebug::module_applications, "rviewDb", "createCollection( " << collName << " )");

    ta.begin();

    //set storage & transfer params
    if (!ensureDatabase())
    {
     ta.abort();
     return 0;
    }

    try
    {
      switch (bt)
      {
	case rbt_bool:
	  mddCollPtr = new( &dbase, rviewSetNames[bt][2] ) r_Set< r_Ref< r_Marray< r_Boolean > > >;
	  break;
	case rbt_char:
	  mddCollPtr = new( &dbase, rviewSetNames[bt][2] ) r_Set< r_Ref< r_Marray< r_Char > > >;
	  break;
	case rbt_long:
	  mddCollPtr = new( &dbase, rviewSetNames[bt][2] ) r_Set< r_Ref< r_Marray< RGBPixel > > >;
	  break;
	default: ta.abort(); return 0;
      }

      dbase.set_object_name(*mddCollPtr, collName);

      ta.commit();
    }
    catch (r_Error &obj)
    {
      cerr << lman->lookup("errorCollCreate") << ": " << obj.what() << endl;
    }
  }
  return 0;
}



int rviewDatabase::deleteCollection(const char *collName)
{
  r_Ref <r_Set <r_Ref <r_GMarray> > > mddCollPtr;
  r_Transaction ta;

  try
  {
    ta.begin();

    //set storage & transfer params
    if (!ensureDatabase())
    {
     ta.abort();
     return 0;
    }

    mddCollPtr = dbase.lookup_object(collName);
    mddCollPtr.delete_object();
    ta.commit();
    return 1;
  }
  catch (...)
  {
    cerr << lman->lookup("errorCollDelete") << endl;
  }
  return 0;
}



int rviewDatabase::insertObject(const char *collName, r_Ref<r_GMarray> mddObj, r_Minterval *domain)
{
  r_Ref <r_Set <r_Ref <r_GMarray> > > mddCollPtr;
  r_Ref <r_GMarray> mddPtr;
  r_Transaction ta;
  rviewBaseType bt;
  r_Object *mo;
  int dim, status;

  RMDBGENTER(3, RMDebug::module_applications, "rviewDb", "insertObject( " << collName << ", ... )");

  dim = (mddObj->spatial_domain()).dimension();
  if ((dim < 1) || (dim > MAXIMUM_DIMENSIONS))
  {
    rviewErrorbox::reportError(lman->lookup("errorObjectDims"));
    return 0;
  }

  // *mddObj ``removes'' r_Ref. Can't leave out the &* combination!
  mo = (r_Object*)(&(*mddObj));
  bt = rviewGetBasetype(mo);

  ta.begin();

  //set storage & transfer params
  if (!ensureDatabase())
  {
    ta.abort();
    return 0;
  }

  rviewProgress *prog = new rviewProgress(lman->lookup("progInsert"));

  ::wxStartTimer();

  try
  {
    mddCollPtr = dbase.lookup_object(collName);
    RMDBGMIDDLE(3, RMDebug::module_applications, "rviewDb", "insertObject() collection cardinality: " << mddCollPtr->cardinality() );
  }
  catch (r_Error &obj)
  {
    char *setName;

	setName = (char*)obj.what();
    setName = rviewSetNames[bt][dim-1];

    switch (bt)
    {
      case rbt_bool:
	mddCollPtr = new (&dbase, setName) r_Set <r_Ref <r_Marray <r_Boolean> > >;
	break;
      case rbt_char:
	mddCollPtr = new (&dbase, setName) r_Set <r_Ref <r_Marray <r_Char> > >;
	break;
      case rbt_uchar:
	mddCollPtr = new (&dbase, setName) r_Set <r_Ref <r_Marray <r_Octet> > >;
	break;
      case rbt_short:
	mddCollPtr = new (&dbase, setName) r_Set <r_Ref <r_Marray <r_Short> > >;
	break;
      case rbt_ushort:
	mddCollPtr = new (&dbase, setName) r_Set <r_Ref <r_Marray <r_UShort> > >;
	break;
      case rbt_long:
	mddCollPtr = new (&dbase, setName) r_Set <r_Ref <r_Marray <r_Long> > >;
	break;
      case rbt_ulong:
	mddCollPtr = new (&dbase, setName) r_Set <r_Ref <r_Marray <r_ULong> > >;
	break;
      case rbt_float:
	mddCollPtr = new (&dbase, setName) r_Set <r_Ref <r_Marray <r_Float> > >;
	break;
      case rbt_double:
	mddCollPtr = new (&dbase, setName) r_Set <r_Ref <r_Marray <r_Double> > >;
	break;
      case rbt_rgb:
	mddCollPtr = new (&dbase, setName) r_Set <r_Ref <r_Marray <RGBPixel> > >;
	break;
      default:
	cerr << "Unknown base type " << bt << endl;
	ta.abort(); prog->Close(TRUE); return 0;
    }
    dbase.set_object_name(*mddCollPtr, collName);
  }

  status = 0;
  try
  {
    if (mdd_createSubcube(mddObj, mddPtr, domain, &dbase) == 0)
    {
      cerr << "Failed to create persistent MDD object" << endl;
      ta.abort(); prog->Close(TRUE); return 0;
    }
    RMDBGMIDDLE(3, RMDebug::module_applications, "rviewDb", "insertObject()  Object type name " << rviewTypeNames[bt][dim-1] );
    mddPtr->set_type_by_name(rviewTypeNames[bt][dim-1]);
    mddCollPtr->insert_element(mddPtr);

    ta.commit();

#ifdef RMANDEBUG
    cout << "Insert Object Time: " << ::wxGetElapsedTime(TRUE) << "ms" << endl;
#endif

    status = 1;
  }
  catch (r_Error &obj)
  {
    cerr << lman->lookup("errorInsertObj") << ": " << obj.what() << endl;
  }

  prog->Close(TRUE);

  RMDBGEXIT(3, RMDebug::module_applications, "rviewDb", "insertObject()");

  return status;
}



int rviewDatabase::executeQuery(collection_desc *desc, const char *qry, r_Ref<r_GMarray> *updateMdd, bool showProgress)
{
  int status;
  char buffer[STRINGSIZE];

  r_Transaction ta;
  r_Set<r_Ref_Any> mddColl;
  int collMembers;

  rviewProgress *prog = NULL;

  if (showProgress)
    prog = new rviewProgress(lman->lookup("progQuery"));

  status = 0;

  desc->number = 0; desc->mddObjs = NULL; desc->strObjs = NULL; desc->collType = NULL;

  try
  {
    int isUpdateQuery;
    char *mddArg;
    r_OQL_Query query(qry);

    // Do this check before starting the transaction
    if (isUpdateQuery = query.is_update_query())
    {
      if ((mddArg = (char*)strstr(qry, "$")) != NULL)
      {
	if (updateMdd == NULL)
	{
	  rviewErrorbox::reportError(lman->lookup("errorUpdtObject"));
	  if (prog != NULL)
	    prog->Close(TRUE);
	  return 0;
	}
      }
    }

    ::wxStartTimer();

    if (isUpdateQuery)
    {
      ta.begin();

    //set storage & transfer params
    if (!ensureDatabase())
    {
     ta.abort();
     return 0;
    }

    }
    else
    {
      ta.begin(r_Transaction::read_only);


    //set storage & transfer params
    if (!ensureDatabase())
    {
     ta.abort();
     return 0;
    }

    }

#ifdef RMANDEBUG
    //cout << "Expanded query:" << endl << query.get_query() << endl;
#endif

    if (isUpdateQuery)
    {
#ifdef RMANDEBUG
      //cout << "Is update query " << endl;
#endif
      if (mddArg != NULL)
      {
	RMDBGONCE(3, RMDebug::module_applications, "rviewDb", "executeQuery() Update MDD domain " << (*updateMdd)->spatial_domain() );
        query << *(*updateMdd);
      }
      r_oql_execute(query);
    }
    else
    {
#ifdef RMANDEBUG
     //cout << "Is normal query" << endl;
#endif
      r_oql_execute(query, mddColl);
    }
    
    collMembers = mddColl.cardinality();

    if (collMembers != 0)
    {
      int collType;

      collType = mddColl.get_element_type_schema()->type_id();
      if (collType == r_Type::MARRAYTYPE)
      {
	r_Set<r_Ref<r_GMarray> > mddArrayColl;
	r_Iterator<r_Ref_Any> iterator = mddColl.create_iterator();
	int i;

	RMDBGONCE(3, RMDebug::module_applications, "rviewDb", "executeQuery() array-collection, build new set..." );

	for (i=0; i<collMembers; i++, iterator++)
	{
	  mddArrayColl.insert_element((r_Ref<r_GMarray>)(*iterator));
	}
	status = collectionToDesc(mddArrayColl, desc);
      }
      else
      {
	r_Iterator<r_Ref_Any > iterator = mddColl.create_iterator();
	ostrstream memstr(buffer, STRINGSIZE);
	int i;

	RMDBGONCE(3, RMDebug::module_applications, "rviewDb", "executeQuery() non-marray collection, build table..." );

	desc->mddObjs = NULL;
	desc->strObjs = new char*[collMembers];
	desc->number = collMembers;

	for (i=0; i<collMembers; i++, iterator++)
	{
	  memstr.width(3);
	  memstr << i << ": ";
	  switch (collType)
	  {
	    case r_Type::POINTTYPE:
	      ((r_Ref<r_Point>)*iterator)->print_status(memstr); break;
	    case r_Type::SINTERVALTYPE:
	      ((r_Ref<r_Sinterval>)*iterator)->print_status(memstr); break;
	    case r_Type::MINTERVALTYPE:
	      ((r_Ref<r_Minterval>)*iterator)->print_status(memstr); break;
	    case r_Type::OIDTYPE:
	      ((r_Ref<r_OId>)*iterator)->print_status(memstr); break;
	    default:
	     ((r_Ref<r_Scalar>)*iterator)->print_status(memstr); break;
	  }
	  memstr << '\0';
	  desc->strObjs[i] = new char[strlen(buffer)+1];
	  strcpy(desc->strObjs[i], buffer);
	  //cout << "item " << i << ": " << desc->strObjs[i] << endl;
	  memstr.seekp(0);
	}
	(mddColl.get_type_schema())->print_status(memstr);
	memstr << '\0';
	desc->collType = new char[strlen(buffer)+1];
	strcpy(desc->collType, buffer);
	status = 1;
      }
    }
    ta.commit();

    sprintf(buffer, "%s: %d ms", lman->lookup("textTime"), ::wxGetElapsedTime(TRUE));
    if ((desc->collInfo = new char[strlen(buffer) + 1]) != NULL)
    {
      strcpy(desc->collInfo, buffer);
    }

#ifdef RMANDEBUG
    cout << "Execute Query " << buffer << endl;
#endif
    // no query error
    line = -1; col = -1;
  }

  catch ( r_Equery_execution_failed &errObj )
  {
    if (errObj.get_errorno() == 0)
    {
      cerr << lman->lookup("errorQueryUnknown") << endl;
    }
    else
    {
      char msg[1024];
      const char *what;

      what = errObj.what();
      if (what != NULL)	// might actually happen...
      {
        // Memorize the position the error occurred at
        line = errObj.get_lineno(); col = errObj.get_columnno();
        sprintf(msg, "%s: %s", lman->lookup("errorQueryFailed"), errObj.what());
        rviewErrorbox::reportError((const char*)msg);
      }
    }
  }

  catch (r_Error &errObj)
  {
    cerr << errObj.what() << endl;
  }

  if (prog != NULL)
    prog->Close(TRUE);

  return status;
}


int rviewDatabase::getMinterval(r_Minterval &dom, const char *collname, const double *loid)
{

  r_Transaction ta;
  r_Set<r_Ref_Any> collPtr;
  char buffer[STRINGSIZE];
  int length, number;

  length = sprintf(buffer, "SELECT SDOM(x) FROM %s AS x", collname);
  if (loid != NULL)
    sprintf(buffer + length, " WHERE OID(x) = %f", *loid);
  
  try
  {
    ta.begin(r_Transaction::read_only);


    //set storage & transfer params
    if (!ensureDatabase())
    {
     ta.abort();
     return 0;
    }

    r_OQL_Query query(buffer);
    r_oql_execute(query, collPtr);
    number = collPtr.cardinality();
    if (number > 1)
      cout << "rviewDatabase::getMinterval(): more than one object returned!" << endl;

    r_Iterator<r_Ref_Any> iterator = collPtr.create_iterator();
    dom = *((r_Ref<r_Minterval>)(*iterator));

    ta.commit();
  }
  catch (r_Error &err)
  {
    ta.abort();
    cerr << err.what() << endl;
    return 0;
  }

  return 1;
}



// Returns the position of the error in the last query
int rviewDatabase::getErrorInfo(int &l, int &c) const
{
  if (line < 0) return 0;
  l = line; c = col;
  return 1;
}



const r_Database *rviewDatabase::getDatabase(void) const
{
  return &dbase;
}



int rviewDatabase::ensureDatabase(void)
{
  if (dbOpen)
  {
    r_Data_Format currentFormat;

    currentFormat = prefs->getTransferFormat();
    if ((lastTransferFormat != currentFormat) || (lastTransferParams != prefs->transferParm))
    {
      try
      {
	dbase.set_transfer_format(currentFormat, prefs->transferParm);
	lastTransferFormat = currentFormat;
	lastTransferParams = prefs->transferParm;
      }
      catch (r_Error &err)
      {
	cerr << err.what() << endl;
      }
    }
    currentFormat = prefs->getStorageFormat();
    if ((lastStorageFormat != currentFormat) || (lastStorageParams != prefs->storageParm))
    {
      try
      {
	dbase.set_storage_format(currentFormat, prefs->storageParm);
	lastStorageFormat = currentFormat;
	lastStorageParams = prefs->storageParm;
      }
      catch (r_Error &err)
      {
	cerr << err.what() << endl;
      }
    }
    return 1;
  }
  rviewErrorbox::reportError(lman->lookup("errorDatabaseClosed"));
  return 0;
}
