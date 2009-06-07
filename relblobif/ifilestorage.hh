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
 * The interface used by the file storage modules. 
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _IFILESTORAGE_H__
#define _IFILESTORAGE_H__

#include "raslib/error.hh"
class OId;

// The class is the interface for the file storage modules.
/**
  * \ingroup Relblobifs
  */
class IFileStorage 
{
	public:
		// Update the content of a blob. The blob should exist already.
		virtual void update(const char* data, r_Bytes size, int BlobId) throw (r_Error) = 0;
		// Store the content of a new blob.
		virtual void insert(const char* data, r_Bytes size, int BlobId) throw (r_Error) = 0;
		// Retrive the content of a previously stored blob
		virtual void retrieve(int BlobId, char** data, r_Bytes *size) throw (r_Error) = 0;
		// Delete a previously stored blob.
		virtual void remove(int BlobId) throw (r_Error) = 0;

		//virtual ~IFileStorage() = 0;
};  // class IFileStorage

#endif  // _IFILESTORAGE_H__
