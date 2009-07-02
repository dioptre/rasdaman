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
 * INCLUDE: partinsert.hh
 *
 * MODULE:  rasodmg
 * CLASS:   r_Partial_Insert
 *
 * COMMENTS:
 *		None
*/

#ifndef _R_PARTIAL_INSERT_
#define _R_PARTIAL_INSERT_


#include "raslib/mddtypes.hh"
#include "raslib/oid.hh"
#include "rasodmg/transaction.hh"


class r_Minterval;
class r_Storage_Layout;
class r_GMarray;
class r_Database;


//@ManMemo: {\bf rasodmg}

/*@Doc:
  Class for inserting an MDD into the database stripwise, as in most of
  our insert tools.
*/

/**
  * \ingroup Rasodmgs
  */
class r_Partial_Insert
{
  public:
  /// constructor receiving all necessary parameters. The storage layout is copied
  r_Partial_Insert( r_Database &usedb, const char *collname, const char *mddtype,
		    const char *settype, const r_Storage_Layout &stl );
  /// alternative constructor for regular tiling
  r_Partial_Insert( r_Database &usedb, const char *collname, const char *mddtype,
		    const char *settype, const r_Minterval &dom, unsigned int tsize );
  /// copy constructor
  r_Partial_Insert( const r_Partial_Insert &src );
  /// destructor
  ~r_Partial_Insert( void );

  /// update the marray; no transaction should be activated, this is done internally.
  int update( r_GMarray *mddPtr, 
  	      r_Data_Format transferFormat = r_Array,
  	      const char* transferFormatParams = NULL,  	      
  	      r_Data_Format storageFormat = r_Array,  	      
  	      const char* storageFormatParams = NULL
  	      );
  /*
    The marray may be modified in small aspects such as base type name and storage layout.
    The "transferFormat, transferFormatParams" are used to set the transfer compression used 
	for the communications of client with the server.
    The "storageFormat, storageFormatParams" are used to set the storage format used for 
	MDD created by the client in the RasDaMan database.
  */

  protected:
  /// shared init code
  void init_share( const char *collname, const char *mddtype, const char *settype );

  /// the marray's OId
  r_OId myOId;
  /// the collection name
  char *collName;
  /// the MDD type name
  char *mddType;
  /// the set type name
  char *setType;
  /// the database
  r_Database &mydb;
  /// the storage layout
  r_Storage_Layout *mystl;
  /// the transaction object
  r_Transaction myta;
  /// do we have to do an insert or an update?
  int doUpdate;
  /// format strings for queries
  static const char *format_create;
  static const char *format_update;
};

#endif
