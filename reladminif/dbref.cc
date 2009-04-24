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
*/
/*************************************************************
 *
 *
 * PURPOSE:
 *	 DBRef is a smart pointer for managing objects derived from
 *	 the DbObject class.
 *
 *
 * COMMENTS:
 *
 ************************************************************/


#include "dbref.hh"
#include <iostream>
#include <stdio.h>
#include "raslib/rmdebug.hh"
#include "objectbroker.hh"
#include "indexmgr/indexds.hh"
#include "relindexif/dbrcindexds.hh"
#include "relindexif/dbtcindex.hh"
#include "indexmgr/hierindexds.hh"
#include "debug/debug.hh"

template <class T> bool 
DBRef<T>::pointerCaching = true;

/*for testing
bool
DBRef<BLOBTile>::pointerCaching = false;

bool
DBRef<InlineTile>::pointerCaching = false;

bool
DBRef<DBTile>::pointerCaching = false;
*/

template <class T> void
DBRef<T>::setPointerCaching(bool useIt)
	{
	RMDBGONCE(10, RMDebug::module_adminif, "DBRef", "setPointerCaching(" << useIt << ") " << pointerCaching );
	pointerCaching = useIt;
	}

template <class T> bool
DBRef<T>::getPointerCaching()
	{
	RMDBGONCE(10, RMDebug::module_adminif, "DBRef", "getPointerCaching() " << pointerCaching );  
	return pointerCaching;
	}

template <class T>
DBRef<T>::DBRef(void)
	:	object(0),
		objId(DBOBJID_NONE),
		pointerValid(false)
	{
	RMDBGONCE(11, RMDebug::module_adminif, "DBRef", "DBRef()");
	}


template <class T>
DBRef<T>::DBRef(const OId &id)
	:	object(0),
		pointerValid(false),
		objId(id)
	{
	RMDBGONCE(11, RMDebug::module_adminif, "DBRef", "DBRef(" << id << ")");
	}


template <class T>
DBRef<T>::DBRef(double id)
	:	object(0),
		pointerValid(false),
		objId(id)
	{
	RMDBGONCE(11, RMDebug::module_adminif, "DBRef", "DBRef(double " << id << ")");
	}


template <class T>
DBRef<T>::DBRef(const DBRef<T> &src)
	:	object(0),
		pointerValid(src.pointerValid),
		objId(src.objId)
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "DBRef(const DBRef) src.OId=" << src.objId);
	if (pointerCaching)
		{
		if (src.object)
			{
			object = src.object;
			objId = object->getOId();
			object->incrementReferenceCount();
			}
		}
	else	{
		if (pointerValid && src.object)
			{
			object = src.object;
			}
		}

	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "DBRef(const DBRef) " << objId);
	}


template <class T>
DBRef<T>::DBRef(T *ptr)
	:	object(ptr),
		pointerValid(true),
		objId(DBOBJID_NONE)
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "DBRef(const T* " << ptr << ")");

	if (object != 0)
		{
		objId = object->getOId();
		object->incrementReferenceCount();
		RMDBGMIDDLE(11, RMDebug::module_adminif, "DBRef", "DBRef(T* " << ptr->getOId() << ")");
		}
	else	{
		pointerValid = false;
		RMDBGMIDDLE(11, RMDebug::module_adminif, "DBRef", "DBRef(T* 0) " << objId);
		}
	}


template <class T>
DBRef<T>::~DBRef(void)
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "~DBRef() " << objId);
	if ((object != 0) && pointerCaching)
		object->decrementReferenceCount();
	object = 0;
	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "~DBRef() " << objId);
	}

template <class T>
bool DBRef<T>::operator<(const DBRef<T>& other) const
	{
	RMDBGENTER(3, RMDebug::module_adminif, "DBRef", "DBRef<T>::operator<(" << other.objId << ", " << (r_Ptr)other.object << ") " << objId << ", " << (r_Ptr)object);
	int ret = operator==(other);
	return (ret == -1);
	}

template <class T>
bool operator< (const DBRef<T> &me, const DBRef<T> &him)
	{
	RMDBGENTER(3, RMDebug::module_adminif, "DBRef", "operator<({" << me.getOId() << "}, {" << him.getOId() << "})");
	return me.operator<(him);
	}

template <class T>
int DBRef<T>::operator==(const DBRef<T> &src) const
	{
	RMDBGENTER(3, RMDebug::module_adminif, "DBRef", "operator==(" << src.objId << ", " << (r_Ptr)src.object << ") " << objId << ", " << (r_Ptr)object);
	int retval = 0;
	if (isInitialised())
		{
		if (src.isInitialised())
			{
			if (object)
				{
				if (object->isPersistent())
					{//this persistent
					if (src.object)
						{
						if (src.object->isPersistent())
							{
							if (object->getOId() < src.object->getOId())
								retval = -1;
							else
								if (object->getOId() > src.object->getOId())
									retval = +1;
								//else == -> 0
							}
						else	{//src is transient
							retval = +1;
							}
						}
					else	{//src is persistent
						if (object->getOId() < src.objId)
							{
							retval = -1;
							}
						else	{
							if (object->getOId() > src.objId)
								retval = +1;
							//else == -> 0
							}
						}
					}
				else	{//this transient
					if (src.object)
						{
						if (src.object->isPersistent())
							{
							retval = -1;
							}
						else	{//src is transient
							if (object < src.object)
								retval = -1;
							else
								if (object > src.object)
									retval = +1;
								//else == -> 0
							}
						}
					else	{//src is persistent
						retval = -1;
						}
					}
				}
			else	{//this is persistent
				if (src.object)
					{
					if (src.object->isPersistent())
						{
						if (objId < src.object->getOId())
							{
							retval = -1;
							}
						else	{
							if (objId > src.object->getOId())
								retval = +1;
							//else == -> 0
							}
						
						}
					else	{//src not persistent
						retval = +1;
						}
					}
				else	{//src is persistent
					if (objId < src.objId)
						{
						retval = -1;
						}
					else	{
						if (objId > src.objId)
							retval = +1;
						//else == -> 0
						}
					}
				}
			}
		else	{
			retval = +1;
			}
		}
	else	{
		if (src.isInitialised())
			{
			retval = -1;
			}
		//else is 0
		}
	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator==(" << src.objId << ") " << retval);
	return retval;
	}


template <class T>
DBRef<T> &DBRef<T>::operator=(const DBRef<T> &src)
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator=(" << src.objId << ") " << objId);
	if ((object != 0) && pointerCaching)
		{
		object->decrementReferenceCount();
		}
	object = src.object;
	pointerValid = src.pointerValid;
	objId = src.objId;
	if (pointerCaching)
		{
		if (object)
			{
			objId = object->getOId();
			object->incrementReferenceCount();
			}
		}
	else	{
		if (object && pointerValid)
			objId = object->getOId();
		}

	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator=(" << src.objId << ") " << objId);
	return *this;
	}


template<class T>
DBRef<T> &DBRef<T>::operator=(T *ptr)
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator=( at " << ptr << " ) " << objId);
	if ((object != 0) && pointerCaching)
		object->decrementReferenceCount();

	object = ptr;
	if (object == 0)
		{
		objId = DBOBJID_NONE;
		pointerValid = false;
		}
	else	{
		objId = object->getOId();
		object->incrementReferenceCount();
		pointerValid = true;
		}
	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator=( at " << ptr << " ) " << objId);
	return *this;
	}


template <class T>
T &DBRef<T>::operator *(void) throw ( r_Error )
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator*() " << objId);

	if (is_null())
		{
		TALK( "DBRef::operator*(): object not found " << objId);
		RMDBGMIDDLE(2, RMDebug::module_adminif, "DBRef", "object was not found " << objId);
		r_Error err = r_Error(r_Error::r_Error_RefNull);
		throw err;
		}

	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator*() " << objId);
	return *object;
	}


template <class T>
const T &DBRef<T>::operator *(void) const throw ( r_Error )
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator*() const " << objId);

	if (is_null())
		{
		TALK( "DBRef::operator*(): object not found " << objId);
		RMDBGMIDDLE(2, RMDebug::module_adminif, "DBRef", "object was not found " << objId);
		r_Error err = r_Error(r_Error::r_Error_RefNull);
		throw err;
		}

	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator*() " << objId);
	return *object;
	}


#ifndef __GNUG__

template <class T>
T &DBRef<T>::operator[](int idx) const throw(r_Error)
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator[](" << idx << ") " << objId);

	if (is_null())
		{
		TALK( "DBRef::operator[](): object not found " << objId);
		RMDBGMIDDLE(2, RMDebug::module_adminif, "DBRef", "object was not found " << objId);
		r_Error err = r_Error(r_Error::r_Error_RefNull);
		throw err;
		}

	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator[](" << idx << ") " << objId);
	return *((this + idx).object);
	}

#endif

template <class T>
T *DBRef<T>::operator->(void) throw(r_Error)
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator->() " << objId);

	if (is_null())
		{
		TALK( "DBRef::operator->(): object not found " << objId);
		RMDBGMIDDLE(2, RMDebug::module_adminif, "DBRef", "object was not found " << objId);
		r_Error err = r_Error(r_Error::r_Error_RefNull);
		throw err;
		}

	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator->() " << objId);
	return object;
	}


template <class T>
const T *DBRef<T>::operator->(void) const throw(r_Error)
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator->() const " << objId);

	if (is_null())
		{
		TALK( "DBRef::operator->(): object not found " << objId);
		RMDBGMIDDLE(2, RMDebug::module_adminif, "DBRef", "object was not found " << objId);
		r_Error err = r_Error(r_Error::r_Error_RefNull);
		throw err;
		}

	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator->() const " << objId);
	return object;
	}


template <class T>
T *DBRef<T>::ptr(void) throw(r_Error)
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "ptr() " << objId);

	if (is_null())
		{
		TALK( "DBRef::ptr(): object not found " << objId);
		RMDBGMIDDLE(2, RMDebug::module_adminif, "DBRef", "object was not found " << objId);
		r_Error err = r_Error(r_Error::r_Error_RefNull);
		throw err;
		}

	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "ptr() " << objId);
	return object;
	}


template <class T>
const T *DBRef<T>::ptr(void) const throw(r_Error)
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "ptr() " << objId);

	if (is_null())
		{
		TALK( "DBRef::ptr(): object not found " << objId);
		RMDBGMIDDLE(2, RMDebug::module_adminif, "DBRef", "object was not found " << objId);
		r_Error err = r_Error(r_Error::r_Error_RefNull);
		throw err;
		}

	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "ptr() " << objId);
	return object;
	}


template <class T>
DBRef<T>::operator T*() throw (r_Error)
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator T*() " << objId);

	if (is_null())
		{
		TALK( "DBRef::T*(): object not found " << objId);
		RMDBGMIDDLE(2, RMDebug::module_adminif, "DBRef", "object was not found " << objId);
		r_Error err = r_Error(r_Error::r_Error_RefNull);
		throw err;
		}

	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator T*() " << objId);
	return object;
	}


template <class T>
DBRef<T>::operator const T*() const throw (r_Error)
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator const T*() const" << objId);

	if (is_null())
		{
		TALK( "DBRef::T*(): object not found " << objId);
		RMDBGMIDDLE(2, RMDebug::module_adminif, "DBRef", "object was not found " << objId);
		r_Error err = r_Error(r_Error::r_Error_RefNull);
		throw err;
		}

	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator const T*() " << objId);
	return object;
	}


template <class T>
OId DBRef<T>::getOId(void) const
	{
	if (object && pointerCaching)
		((DBRef<T>)*this).objId = object->getOId();
	return objId;
	}

template <class T>
void DBRef<T>::delete_object(void)
	{
	if (!is_null())
		{
		object->setPersistent(false);
		object->decrementReferenceCount();
		object = 0;
		objId = DBOBJID_NONE;
		}
	else	{
		r_Error err;
		if (objId.getType() == OId::INVALID)
			{
			err = r_Error(r_Error::r_Error_OIdInvalid);
			}
		else	{
			err = r_Error(r_Error::r_Error_ObjectUnknown);
			}
		RMDBGONCE(0, RMDebug::module_adminif, "DBRef", "delete_object() " << objId << " not ok")
		throw err;
		}
	}

template <class T>
bool DBRef<T>::isInitialised() const
	{
	bool retval=false;
	if (object)
		retval=true;
	else	{
		if (objId.getType() != OId::INVALID)
			retval=true;
		}
	return retval;
	}

template <class T>
bool DBRef<T>::is_valid(void) const
	{
	bool retval=false;
	if (!is_null())
		retval=true;
	return retval;
	}


template <class T>
void DBRef<T>::release()
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "release() " << objId);
	if ((object != 0) && pointerCaching)
		{
		object->decrementReferenceCount();
		}
	object = 0;
	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "release() " << objId);
	}


template <class T>
DBRef<T>::operator DBRef<DBObject>() const
	{
 	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBObject>() " << objId);
	if (object && pointerCaching)
		{
 		RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBObject>(" << object << ") " << objId);
		return DBRef<DBObject>(object);
		}
	else	{
 		RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBObject>(" << objId << ") " << objId);
		return DBRef<DBObject>(objId);
		}
	}

template <class T>
DBRef<T>::operator DBRef<InlineTile>() const throw (r_Error)
	{
 	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator DBRef<InlineTile>() " << objId);
	if (object && pointerCaching)
		{
		if (object->getObjectType() == OId::INLINETILEOID)
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<InlineTile>() " << objId); 
			return DBRef<InlineTile>((InlineTile*)object);
			}
		}
	else	{
		if (objId.getType() == OId::INLINETILEOID)
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<InlineTile>() " << objId); 
			return DBRef<InlineTile>(objId);
			}
		}
	TALK( "DBRef::<InlineTile>(): operator mismatch" << objId);
 	RMDBGEXIT(0, RMDebug::module_adminif, "DBRef", "operator DBRef<InlineTile>() mismatch " << objId);
	throw r_Error(r_Error::r_Error_DatabaseClassMismatch);
	}

template <class T>
DBRef<T>::operator DBRef<DBTile>() const throw (r_Error)
	{
 	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBTile>() " << objId << "; object=" << (long) object );
	TALK( "DBRef::DBRef<DBTile>(): object=" << (long) object );
	if (object && pointerCaching)
		{
		if ((object->getObjectType() == OId::BLOBOID) || (object->getObjectType() == OId::INLINETILEOID))
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBTile>() " << objId); 
			return DBRef<DBTile>((DBTile*)object);
			}
		}
	else	{
		if ((objId.getType() == OId::BLOBOID) || (objId.getType() == OId::INLINETILEOID))
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBTile>() " << objId); 
			return DBRef<DBTile>(objId);
			}
	}
	if (object) { TALK( "DBRef::DBRef<DBTile>(): object->getObjectType()=" << object->getObjectType() ); }
	              TALK( "DBRef::DBRef<DBTile>():  objId->getObjectType()=" <<  objId.getType() );
	TALK( "DBRef::DBRef<DBTile>(): operator mismatch" << objId );
	if (object) { RMDBGMIDDLE(0, RMDebug::module_adminif, "DBRef", "object->getObjectType()=" << object->getObjectType() ); }
 	RMDBGMIDDLE(0, RMDebug::module_adminif, "DBRef", "objId->getObjectType()=" <<  objId.getType() );
 	RMDBGEXIT(0, RMDebug::module_adminif, "DBRef", "operator DBRef<DBTile>() mismatch " << objId);
	throw r_Error(r_Error::r_Error_DatabaseClassMismatch);
	}

template <class T>
DBRef<T>::operator DBRef<BLOBTile>() const throw (r_Error)
	{
 	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator DBRef<BLOBTile>() " << objId);
	if (object && pointerCaching)
		{
		if (object->getObjectType() == OId::BLOBOID || (object->getObjectType() == OId::INLINETILEOID))
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<BLOBTile>() " << objId); 
			return DBRef<BLOBTile>((BLOBTile*)object);
			}
		}
	else	{
		if ((objId.getType() == OId::BLOBOID) || (objId.getType() == OId::INLINETILEOID))
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<BLOBTile>() " << objId); 
			return DBRef<BLOBTile>(objId);
			}
		}
	TALK( "DBRef::DBRef<BLOBTile>(): operator mismatch" << objId);
 	RMDBGEXIT(0, RMDebug::module_adminif, "DBRef", "operator DBRef<BLOBTile>() mismatch " << objId);
	throw r_Error(r_Error::r_Error_DatabaseClassMismatch);
	}

template <class T>
DBRef<T>::operator DBRef<DBTCIndex>() const throw (r_Error)
	{
 	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBTCIndex>() " << objId);
	if (object && pointerCaching)
		{
		if (object->getObjectType() == OId::DBTCINDEXOID)
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBTCIndex>() " << objId); 
			return DBRef<DBTCIndex>((DBTCIndex*)object);
			}
		}
	else	{
		if (objId.getType() == OId::DBTCINDEXOID)
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBTCIndex>() " << objId); 
			return DBRef<DBTCIndex>(objId);
			}
		}
	TALK( "DBRef::DBRef<DBTCIndex>(): operator mismatch" << objId);
 	RMDBGEXIT(0, RMDebug::module_adminif, "DBRef", "operator DBRef<DBTCIndex>() mismatch " << objId);
	throw r_Error(r_Error::r_Error_DatabaseClassMismatch);
	}

template <class T>
DBRef<T>::operator DBRef<DBHierIndex>() const throw (r_Error)
	{
 	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBHierIndex>() " << objId);
	if (object && pointerCaching)
		{
		if ((object->getObjectType() == OId::MDDHIERIXOID) || (object->getObjectType() == OId::DBTCINDEXOID))
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBHierIndex>() " << objId); 
			return DBRef<DBHierIndex>((DBHierIndex*)object);
			}
		}
	else	{
		if ((objId.getType() == OId::MDDHIERIXOID) || (objId.getType() == OId::DBTCINDEXOID))
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBHierIndex>() " << objId); 
			return DBRef<DBHierIndex>(objId);
			}
		}
	TALK( "DBRef::DBRef<DBHierIndex>(): operator mismatch" << objId);
 	RMDBGEXIT(0, RMDebug::module_adminif, "DBRef", "operator DBRef<DBHierIndex>() mismatch " << objId);
	throw r_Error(r_Error::r_Error_DatabaseClassMismatch);
	}


template <class T>
DBRef<T>::operator DBRef<DBRCIndexDS>() const throw (r_Error)
	{
 	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBRCIndexDS>() " << objId);
	if (object && pointerCaching)
		{
		if (object->getObjectType() == OId::MDDRCIXOID)
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBRCIndexDS>() " << objId); 
			return DBRef<DBRCIndexDS>((DBRCIndexDS*)object);
			}
		}
	else	{
		if (objId.getType() == OId::MDDRCIXOID)
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator DBRef<DBRCIndexDS>() " << objId); 
			return DBRef<DBRCIndexDS>(objId);
			}
		}
	TALK( "DBRef::DBRef<DBRCIndexDS>(): operator mismatch" << objId);
 	RMDBGEXIT(0, RMDebug::module_adminif, "DBRef", "operator DBRef<DBRCIndexDS>() mismatch " << objId);
	throw r_Error(r_Error::r_Error_DatabaseClassMismatch);
	}


template <class T>
DBRef<T>::operator HierIndexDS*() const throw (r_Error)
	{
 	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator HierIndexDS*() " << objId);
	if (object && pointerCaching)
		{
		if ((object->getObjectType() == OId::MDDHIERIXOID) || (object->getObjectType() == OId::DBTCINDEXOID))
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator HierIndexDS*() " << objId); 
			return (HierIndexDS*)object;
			}
		}
	else	{
		if (objId.getType() == OId::MDDHIERIXOID)
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator IndexDS*() DBHierIndexId" << objId); 
			DBRef<DBHierIndex> t(objId);
			return (HierIndexDS*)t.ptr();
			}
		else	{
 			if (objId.getType() == OId::DBTCINDEXOID) 
				{
 				RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator IndexDS*() DBTCIndexId" << objId); 
				DBRef<DBTCIndex> t(objId);
				return (HierIndexDS*)t.ptr();
				}
			}
		}
	TALK( "DBRef::HierIndexDS*(): operator mismatch" << objId);
 	RMDBGEXIT(0, RMDebug::module_adminif, "DBRef", "operator HierIndexDS*() mismatch " << objId);
	throw r_Error(r_Error::r_Error_DatabaseClassMismatch);
	}


template <class T>
DBRef<T>::operator IndexDS*() const throw (r_Error)
	{
 	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "operator IndexDS*() " << objId);
	if (object && pointerCaching)
		{
		if ((object->getObjectType() == OId::MDDHIERIXOID) || (object->getObjectType() == OId::DBTCINDEXOID) || (object->getObjectType() == OId::MDDRCIXOID))
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator IndexDS*() " << objId); 
			return (IndexDS*)object;
			}
		}
	else	{
		if (objId.getType() == OId::MDDHIERIXOID)
			{
 			RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator IndexDS*() DBHierIndexId" << objId); 
			DBRef<DBHierIndex> t(objId);
			return (IndexDS*)t.ptr();
			}
		else	{
 			if (objId.getType() == OId::DBTCINDEXOID) 
				{
 				RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator IndexDS*() DBTCIndexId" << objId); 
				DBRef<DBTCIndex> t(objId);
				return (IndexDS*)t.ptr();
				}
			else	{
				if (objId.getType() == OId::MDDRCIXOID)
					{
					RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "operator IndexDS*() DBRCIndexId" << objId); 
					DBRef<DBRCIndexDS> t(objId);
					return (IndexDS*)t.ptr();
					}
				}
			}
		}
	TALK( "DBRef::IndexDS*(): operator mismatch" << objId);
 	RMDBGEXIT(0, RMDebug::module_adminif, "DBRef", "operator IndexDS*() mismatch " << objId);
	throw r_Error(r_Error::r_Error_DatabaseClassMismatch);
	}


template <class T>
bool DBRef<T>::is_null(void) const
	{
	RMDBGENTER(11, RMDebug::module_adminif, "DBRef", "is_null() " << objId);
	bool retval = false;
	T* t = 0;
	if (object == 0)
		{
		if (objId.getType() == OId::INVALID)
			{
			throw r_Error(r_Error::r_Error_OIdInvalid);
			}
		else	{
			try	{
				t = (T*)ObjectBroker::getObjectByOId(objId);
				RMDBGMIDDLE(11, RMDebug::module_adminif, "DBRef", "found object");
				t->incrementReferenceCount();
				((DBRef<T>*)this)->object = t;
				RMDBGMIDDLE(11, RMDebug::module_adminif, "DBRef", "object is at: " << object << " found object was at: " << t);
				}
			catch (r_Error& err)
				{
				if (err.get_kind() == r_Error::r_Error_ObjectUnknown)
					retval = true;
				else
					throw;
				}
			}
		}
	else	{
		if (pointerCaching == false)
			{
			if (pointerValid == false)
				{
				try	{
					t = (T*)ObjectBroker::getObjectByOId(objId);
					RMDBGMIDDLE(11, RMDebug::module_adminif, "DBRef", "found object");
					t->incrementReferenceCount();
					((DBRef<T>*)this)->object = t;
					RMDBGMIDDLE(11, RMDebug::module_adminif, "DBRef", "object is at: " << object << " found object was at: " << t);
					}
				catch (r_Error& err)
					{
					if (err.get_kind() == r_Error::r_Error_ObjectUnknown)
						retval = true;
					else
						throw;
					}
				}
			else	{
				//retval = false; is done in initialize
				}
			}
		else	{
			//retval = false; is done in initialize
			}
		}
	RMDBGEXIT(11, RMDebug::module_adminif, "DBRef", "is_null() " << objId << " " << retval);
	return retval;
	}
