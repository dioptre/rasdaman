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
#ifndef _DBTILE_HH_
#define _DBTILE_HH_

// -*-C++-*- (for Emacs)

/*************************************************************
 *
 *
 * PURPOSE:
 *
 *
 * COMMENTS:
 *
 ************************************************************/

class OId;
class r_Error;
#include "reladminif/dbobject.hh"
#include "raslib/mddtypes.hh"
#include "tileid.hh"
#include "relindexif/indexid.hh"

//@ManMemo: Module: {\bf reldbif}.

/*@Doc:
This class is used in tilemgr as an interface to the persistent tiles.  There are subclasses of dbtile which store themselves differently.  This class can also be used to get rid of tilemgr/TransTile.  When no persistent tile is needed then a dbtile and not a blobtile/inlinetile can be used.
*/


class DBTile	: public DBObject
	{
	public:
		friend std::ostream& operator << (std::ostream& stream, DBTile& b);
		/*@Doc:
		prints info about the dbtile (flags, id, dbdata, size, modified)
		*/
	//@Man: read methods
	//@{
		/// get contents as array of chars for read access.
		char* getCells();
		/*@Doc:
		  Returns a pointer to the contents of the DBTile. This pointer
		  can be used as a char array with getSize() elements.
		  Retriving the contents with this method will mark this object dirty.
		*/

		/// get contents as array of chars for read access.
		const char* getCells() const;
		/*@Doc:
		  Returns a pointer to the contents of the DBTile. This pointer
		  can be used as a char array with getSize() elements.
		*/

		/// get one char in the contents of the DBTile.
		char getCell(r_Bytes index) const;
		/*@Doc:
		  Returns a copy of the char. This functions function should usually
		  not be used because of performance considerations.
		*/

		r_Data_Format getDataFormat() const;
		/*@Doc:
		  Returns the Data Format of this tile (r_Tiff, r_Array, ...)
		*/

		r_Data_Format getCurrentFormat() const;
		/*@Doc:
		  Returns the Data Format of the tiles contents (r_Tiff, r_Array, ...)
		*/

		void setDataFormat(const r_Data_Format& dataformat);
		/*@Doc:
		  Sets the Data Format of this tile (r_Tiff, r_Array, ...)
		  This method will mark this object dirty.
		*/

		void setCurrentFormat(const r_Data_Format& dataformat) const;
		/*@Doc:
		  Sets the Data Format of the contents of this tile (r_Array, or the compressed stuff)
		  This method will _not_ mark this object dirty.
		*/

		/// get size of contents of DBTile in chars.
		r_Bytes getSize() const;
		/*@Doc:
		  Returns the size of this Tile in bytes.
		*/
	//@}

	//@Man: write methods
	//@{
		/// change one char in the contents of the DBTile.
		void setCell(r_Bytes index, char newCell);
		/*@Doc:
		  Usually modifications should be done directly on the char array
		  returned by getCells() because of much better performance.
		  This method will mark this object dirty.
		*/

		void setCells(char* newCells);
		/*@Doc:
		  Completely replaces the character array of this tile.
		  The old arry is deleted.
		  Modification is set to dirty.
		*/		

	//@}

	//@Man:	constructors
	//@{
		DBTile(r_Data_Format dataformat = r_Array);
		/*@Doc:
		constructs a new empty DBTile.  Length is 0.
		*/
		
		DBTile(const OId& BlobId) throw (r_Error);
		/*@Doc:
		Passes to DBObject.
		*/
		
		DBTile(r_Bytes newSize, char c = 0, r_Data_Format dataformat = r_Array);
		/*@Doc:
		constructs a new DBTile of size newSize filled with c.
		*/

		DBTile(r_Bytes newSize, r_Bytes patSize, const char* pat, r_Data_Format dataformat = r_Array);
		/*@Doc:
		 Constructs a new DB Tile of size newSize filled with the repeated
		 char array pat of size patSize. If after filling some chars are
		 left, they are filled with 0
		*/
		/*@ManMemo: constructs a new DB Tile with the char array newCells 
			    with newSize elements as contents. */
			    
		DBTile(r_Bytes newSize, const char* newCells, r_Data_Format dataformat = r_Array);
		/*@Doc:
		constructs a new DBTile of size newSize filled with the contents of newCells.
		the newCells are copied.
		*/
	//@}

		virtual ~DBTile();
		/*@Doc:
		validates the object.  deletes it cells.
		*/

		void resize(r_Bytes newSize);
		/*@Doc:
		resize DBTile.  previous contents are lost.
		*/

		virtual r_Bytes getMemorySize() const;
		/*@Doc:
		returns the space taken up by this object in memory:
		size * sizeof(char) + sizeof(char*) +
			DBObject::getMemorySize() + sizeof(r_Data_Format) + sizeof(r_Bytes)
		*/

		virtual void printStatus(unsigned int level = 0, std::ostream& stream = std::cout) const;
		/*@Doc:
		prints the status of DBObject, the dataformat, the size and the contents as (int)
		*/

		void setNoModificationData(char* data) const;
		/*@Doc:
		Completely replaces the array.
		The object will not be marked dirty.
		This is used for transparent compression/decompression.
		*/

		void setNoModificationSize(r_Bytes newSize) const;
		/*@Doc:
		Sets the size of this tile.
		The object will not be marked dirty.
		This is used for transparent compression/decompression.
		*/

	protected:
		mutable r_Bytes size;
		/*@Doc:
		total size of the contents of DBTile in number of chars.
		*/

		mutable char* cells;
		/*@Doc:
		the data is allocated by malloc
		*/

		r_Data_Format dataFormat;
		/*@Doc:
		data format to construct a PersTile
		*/

		mutable r_Data_Format currentFormat;
		/*@Doc:
		the current format of the contents.  This is neccessary to know when getting mixed up compressed contents.
		*/
	};

#endif
