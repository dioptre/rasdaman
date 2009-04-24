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
// -*-C++-*- (for Emacs)

/*************************************************************
 *
 *
 * PURPOSE:
 *   The Blobtile class is used to store the tiles in the database.
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _BLOBTILE_HH_
#define _BLOBTILE_HH_

class OId;
class r_Error;

#include "dbtile.hh"
#include "raslib/mddtypes.hh"

//@ManMemo: Module: {\bf relblobif}.

/*@Doc:

BLOBTile is the persistent class for storing the contents of MDD tiles
in the database. Each instance represents the contents of a tile of a MDD Object
from the database. BLOBTiles are just arrays of unsigned characters.
In main memory they are encapsulated in the class \Ref{PersTile}.

At the moment a BLOBTile is loaded into main memory, when it is
accessed the first time. This usually happens, when the RasDaMan DBMS
accesses the contents of a \Ref{PersTile}.

{\bf Interdependencies}

BLOBTile is an interface class with the base DBMS. It is, therefore,
highly dependent on the base DBMS used.
*/

class BLOBTile	: public DBTile
	{
	public:
	//@Man:	constructors
	//@{
		BLOBTile(r_Data_Format dataformat = r_Array);
			/*@Doc:
			constructs a new empty BLOBTile and gets an id for it.
			*/
		
		BLOBTile(const OId& BlobId) throw (r_Error);
			/*@Doc:
			constructs a BlobTile out of the database
			*/
		
		BLOBTile(r_Bytes newSize, char c = 0, r_Data_Format dataformat = r_Array);
			/*@Doc:
			constructs a new BLOBTile of size newSize filled with c.
			*/
		
		BLOBTile(r_Bytes newSize, r_Bytes patSize, const char* pat, r_Data_Format dataformat = r_Array);
			/*@Doc:
			 Constructs a new BLOB Tile of size newSize filled with the repeated
			 char array pat of size patSize. If after filling some chars are
			 left, they are filled with 0
			*/
			/*@ManMemo: constructs a new BLOB Tile with the char array newCells 
				    with newSize elements as contents. */
			    
		BLOBTile(r_Bytes newSize, const char* newCells, r_Data_Format dataformat = r_Array);
			/*@Doc:
			constructs a new BLOBTile of size newSize filled with the contents of newCells.
			*/

		BLOBTile(r_Bytes newSize, const char* newCells, r_Data_Format dataformat, const OId& myOId);
			/*@Doc:
			constructs a new BLOBTile of size newSize filled with the contents of newCells.
			the oid will be assigned to this blob.  used by regular computed index.
			*/

		BLOBTile(const OId& BlobId, r_Bytes newSize, r_Data_Format newFmt);
			/*@Doc:
			constructs a new BLOBTile of size newSize filled with zeros.
			the tile will think it is not modified and also not in the db but persistent.
			this is used by the rc index.
			*/

	//@}

		virtual ~BLOBTile();
			/*@Doc:
			validates the object.  deletes it cells.
			*/

		static void kill(const OId& target, unsigned int range = 0);
			/*@Doc:
			delete a blobtile without loading it first into memory.
			is used by the indexes.
			delete the blobtile and range consecutive tiles.
			*/

		static r_Bytes BLOBBufferLength;
			/*@Doc:
			info on the length of the BLOBBuffer
			*/

	protected:

		virtual void updateInDb() throw (r_Error);
			/*@Doc:
			update the contents of a Tile in the db
			*/

		virtual void insertInDb() throw (r_Error);
			/*@Doc:
			inserts the Blob into the db.
			*/

		virtual void readFromDb() throw (r_Error);
			/*@Doc:
			read blob from db into blobtile
			*/

		virtual void deleteFromDb() throw (r_Error);
			/*@Doc:
			deletes a blob from TILES, sets size to 0 and flags to -1
			*/

	private:
		static char* BLOBBuffer;
			/*@Doc:
			for writing into the DB.  currently not needed by oracle.
			*/

	};

#endif
