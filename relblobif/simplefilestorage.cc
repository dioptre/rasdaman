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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "raslib/rminit.hh"
#include "../reladminif/oidif.hh"
#include "simplefilestorage.hh"
#include "debug-srv.hh"

using namespace std;

SimpleFileStorage::~SimpleFileStorage()
{
}

SimpleFileStorage::SimpleFileStorage(const string& path) throw (r_Error) : root_path(path) 
{
	// Check if the path exist is readable/writable etc.
	struct stat status;
	if (stat(path.c_str(), &status)<0 || !S_ISDIR(status.st_mode)) {
		throw 1; // Root directory not found
	}
	TALK("SimpleFileStorage initialized on root path" << path);
}

void SimpleFileStorage::insert(const char* data, r_Bytes size, int BlobId) throw (r_Error)
{
	ENTER("SimpleFileStorage::insert with BlobID="<<BlobId);
	vector<string> path;
	getPath(BlobId, &path);
	string file_path = path[0]; // Root path
	// Iterate trough the levels and create all directories needed.
	for (int i = 1; i < path.size() - 1; ++i) 
	{
		file_path += '/' + path[i];
		struct stat status;
		if (!stat(file_path.c_str(), &status))
			if (!mkdir(file_path.c_str(), 0770))
				throw 2; // Cannot create directory
	}
	file_path += '/' + path[path.size() - 1];
	int fd = open(file_path.c_str(), O_CREAT | O_WRONLY, 0770);
	if (fd < 0)
		throw 3; // Cannot create file
	int offset = 0;
	// Send the data to the disk
	while (offset < size) {
		int count = write(fd, data + offset, size - offset);
		if (count == -1)
			throw 4;  // Error while writing data to the disk
		offset += count;
	}
	if (close(fd)<0)
		throw 4;  // Error while writing data to the disk
	LEAVE("SimpleFileStorage::insert");
}

void SimpleFileStorage::update(const char* data, r_Bytes size, int BlobId) throw (r_Error)
{
	ENTER("SimpleFileStorage::update");
	vector<string> path;
	getPath(BlobId, &path);
	string file_path = path[0]; // Root path
	// Iterate trough the levels and create all directories needed.
	for (int i = 1; i < path.size() - 1; ++i) 
	{
		file_path += '/' + path[i];
		struct stat status;
		if (!stat(file_path.c_str(), &status))
			if (!mkdir(file_path.c_str(), 0770))
				throw 2; // Cannot create directory
	}
	file_path += '/' + path[path.size() - 1];

	int fd = open(file_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
	if (fd < 0)
		throw 6; // Cannot open file
	int offset = 0;
	// Send the data to the disk
	while (offset < size) {
		int count = write(fd, data + offset, size - offset);
		if (count == -1)
			throw 4;  // Error while writing data to the disk
		offset += count;
	}
	if (close(fd)<0)
		throw 4;  // Error while writing data to the disk
	LEAVE("SimpleFileStorage::update");
}

void SimpleFileStorage::retrieve(int BlobId, char** data, r_Bytes* size) throw (r_Error)
{
	ENTER("SimpleFileStorage::read");
	string path;
	getPath(BlobId, &path);
	struct stat status;
	if (stat(path.c_str(), &status) < 0)
		throw 5; // File does not exist, the blob does not exist.
	
	*size = status.st_size;
	*data = (char*)malloc(status.st_size);

	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		throw 6; // Cannot open file
	int offset = 0;
	// Send the data to the disk
	while (offset < *size) {
		int count = read(fd, *data + offset, *size - offset);
		if (count == -1)
			throw 7;  // Error while reading data from the disk
		offset += count;
	}
	if (close(fd) < 0)
		throw 7;  // Error while reading data from the disk
	LEAVE("SimpleFileStorage::read");
}

void SimpleFileStorage::remove(int BlobId) throw (r_Error) 
{
	ENTER("SimpleFileStorage::remove");
	string path;
	getPath(BlobId, &path);
	if (unlink(path.c_str()) < 0) 
		throw 8; // Error while removing the file
	LEAVE("SimpleFileStorage::remove");
}

void SimpleFileStorage::getPath(int BlobId, vector<string> *path) {
	ENTER("SimpleFileStorage::getPath");
	path->clear();
	path->push_back(root_path);
	stringstream aux;
	aux << BlobId;
	path->push_back(aux.str());
	LEAVE("SimpleFileStorage::getPath");
}

void SimpleFileStorage::getPath(int BlobId, string *path) {
	vector<string> segments;
	getPath(BlobId, &segments);
	*path = segments[0];
	for (int i = 1; i < segments.size(); ++i)
		*path += '/' + segments[i];
}
