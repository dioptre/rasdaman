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

#include "ecwmemfs.hh"
#include <fstream>
#include <algorithm>
#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"

const char*
MemoryFileSystem::memorySrc = NULL;

r_Bytes
MemoryFileSystem::memorySrcLength = 0;

const char*
MemoryFileSystem::memorySrcName = "memory.src";

MemoryFileSystem::MemoryFileSystem()
	:	current(NULL),
		source(NULL),
		length(0),
		closed(false),
		owner(true)
	{
	RMDBGONCE(5, RMDebug::module_conversion, "MemoryFileSystem", "MemoryFileSystem()");
	}

MemoryFileSystem::m_Error
MemoryFileSystem::open(const char* memorySource, r_Bytes mSize)
	{
	RMDBGONCE(5, RMDebug::module_conversion, "MemoryFileSystem", "open(source, " << mSize << ")");
	owner = false;
	length = mSize;
	source = (char*)memorySource;
	current = source;
	return No_Error;
	}

MemoryFileSystem::m_Error
MemoryFileSystem::open(const char* fileName)
	{
	owner = true;
	RMDBGENTER(5, RMDebug::module_conversion, "MemoryFileSystem", "open(" << fileName << ")");
	if (fileName == memorySrcName)
		{
		MemoryFileSystem::m_Error err = open(memorySrc, memorySrcLength);
		RMDBGEXIT(5, RMDebug::module_conversion, "MemoryFileSystem", "open(" << fileName << ") " << err);
		return err;
		}
	else	{
		std::ifstream f;
		f.open(fileName);
		if (!f.is_open())
			{
			RMInit::logOut << "MemoryFileSystem::open(" << fileName << ") could not open file" << std::endl;
			RMDBGEXIT(5, RMDebug::module_conversion, "MemoryFileSystem", "open(" << fileName << ") " << Error);
			return Error;
			}
		f.seekg(0, std::ios::end);
		std::ios::pos_type end = f.tellg();
		RMDBGMIDDLE(5, RMDebug::module_conversion, "MemoryFileSystem", "size " << end);
		length = end;
		source = new char[end];
		current = source;
		memset(source, 0, end);
		f.seekg(0, std::ios::beg);
		f.read(source, end);
		f.close();
		RMDBGEXIT(5, RMDebug::module_conversion, "MemoryFileSystem", "open(" << fileName << ") " << No_Error);
		return No_Error;
		}
	}

MemoryFileSystem::m_Error
MemoryFileSystem::close()
	{
	RMDBGENTER(5, RMDebug::module_conversion, "MemoryFileSystem", "close()");
	if (!closed)
		{
		if (owner)
			{
			delete [] source;
			}
		source = NULL;
		current = NULL;
		length = 0;
		RMDBGEXIT(5, RMDebug::module_conversion, "MemoryFileSystem", "close() " << No_Error);
		return No_Error;
		}
	else	{
		RMDBGEXIT(5, RMDebug::module_conversion, "MemoryFileSystem", "close() " << Error);
		return Error;
		}
	}

MemoryFileSystem::~MemoryFileSystem()
	{
	RMDBGONCE(5, RMDebug::module_conversion, "MemoryFileSystem", "~MemoryFileSystem()");
	close();
	}

MemoryFileSystem::m_Error
MemoryFileSystem::read(void* buffer, r_Bytes bSize)
	{
	RMDBGENTER(5, RMDebug::module_conversion, "MemoryFileSystem", "read(buffer, " << bSize << ")");
	bSize = std::min(bSize, length - (current - source));
	RMDBGMIDDLE(5, RMDebug::module_conversion, "MemoryFileSystem", "reading " << bSize);
	memcpy(buffer, current, bSize);
	current += bSize;
	RMDBGEXIT(5, RMDebug::module_conversion, "MemoryFileSystem", "read(buffer, " << bSize << ") " << No_Error);
	return No_Error;
	}

unsigned long long
MemoryFileSystem::tell()
	{
	RMDBGONCE(5, RMDebug::module_conversion, "MemoryFileSystem", "tell() " << (current - source));
	return current - source;
	}

MemoryFileSystem::m_Error
MemoryFileSystem::seek(unsigned long long offset)
	{
	RMDBGENTER(5, RMDebug::module_conversion, "MemoryFileSystem", "seek(" << offset << ")");
	if (offset > length)
		{
		RMDBGEXIT(5, RMDebug::module_conversion, "MemoryFileSystem", "seek(" << offset << ") (length " << length << ") " << Error);
		return Error;
		}
	else	{
		current = offset + source;
		RMDBGEXIT(5, RMDebug::module_conversion, "MemoryFileSystem", "seek(" << offset << ") " << No_Error);
		return No_Error;
		}
	}

