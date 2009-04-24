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
 * INCLUDE: nocompstream.cc
 *
 * MODULE:  compression
 * CLASS:   r_No_Comp_Stream, r_No_Decomp_Stream
 *
 * COMMENTS:
 *
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "compression/nocompstream.hh"
#include "compression/compresstime.hh"
#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"




r_No_Comp_Stream::r_No_Comp_Stream( void ) : r_Lin_Comp_Stream()
{
  blockSize = 4096;
}

r_No_Comp_Stream::r_No_Comp_Stream( const r_No_Comp_Stream& src ) : r_Lin_Comp_Stream(src)
{
  blockSize = src.blockSize;
}

r_No_Comp_Stream::~r_No_Comp_Stream( void )
{
}

int r_No_Comp_Stream::begin( r_Bytes typeSize, r_ULong inputSize )
{
  CBENCH_STATEMENT(instantiate_timer("compress", 0));
  init_target();
  int status = target->start(typeSize);
  CBENCH_STATEMENT(myTimer->pause());
  return status;
}

int r_No_Comp_Stream::put( const void* data, r_ULong size )
{
  CBENCH_STATEMENT(myTimer->resume());
  int status = target->put(data, size);
  CBENCH_STATEMENT(myTimer->pause());
  return status;
}

void* r_No_Comp_Stream::end( r_ULong &size )
{
  CBENCH_STATEMENT(myTimer->resume());

  size = target->stop();
  void* result;

  if (size == 0)
  {
    CBENCH_STATEMENT(myTimer->stop());
    return NULL;
  }

  result = mystore.storage_alloc(size);

  target->copy_data(result);

  exit_target();

  CBENCH_STATEMENT(myTimer->stop());

  return result;
}

r_ULong r_No_Comp_Stream::end( void )
{
  CBENCH_STATEMENT(myTimer->resume());
  r_ULong size = target->stop();
  CBENCH_STATEMENT(myTimer->stop());
  return size;
}

void r_No_Comp_Stream::copy_data( void* dest )
{
  target->copy_data(dest);

  exit_target();
}

const char* r_No_Comp_Stream::get_name( void ) const
{
  return "r_No_Comp_Stream";
}

r_Linear_Stream::r_Lin_Stream_Format r_No_Comp_Stream::get_format( void ) const
{
  return r_Lin_Stream_None;
}

r_Lin_Comp_Stream* r_No_Comp_Stream::clone( void ) const
{
  return new r_No_Comp_Stream(*this);
}




r_No_Decomp_Stream::r_No_Decomp_Stream( void ) : r_Lin_Decomp_Stream()
{
}

r_No_Decomp_Stream::r_No_Decomp_Stream( const r_No_Decomp_Stream& src ) : r_Lin_Decomp_Stream()
{
}

r_No_Decomp_Stream::~r_No_Decomp_Stream( void )
{
}

int r_No_Decomp_Stream::begin( r_Bytes typeSize, const void* data, r_ULong size, void* dest )
{
  CBENCH_STATEMENT(instantiate_timer("decompress", 0));
  init_source();
  int status = source->start(typeSize, data, size);
  CBENCH_STATEMENT(myTimer->pause());
  return status;
}

int r_No_Decomp_Stream::get( void* buffer, r_ULong size )
{
  CBENCH_STATEMENT(myTimer->resume());
  int status = source->get(buffer, size);
  CBENCH_STATEMENT(myTimer->pause());
  return status;
}

int r_No_Decomp_Stream::get( r_ULong size )
{
  CBENCH_STATEMENT(myTimer->resume());
  int status = get(destPtr, size);
  destPtr = (void*)(((char*)destPtr) + size);
  CBENCH_STATEMENT(myTimer->pause());
  return status;
}

int r_No_Decomp_Stream::end( void )
{
  CBENCH_STATEMENT(myTimer->resume());
  int status = source->stop();
  exit_source();
  CBENCH_STATEMENT(myTimer->stop());
  return status;
}

const char* r_No_Decomp_Stream::get_name( void ) const
{
  return "r_No_Decomp_Stream";
}

r_Linear_Stream::r_Lin_Stream_Format r_No_Decomp_Stream::get_format( void ) const
{
  return r_Lin_Stream_None;
}

r_Lin_Decomp_Stream* r_No_Decomp_Stream::clone( void ) const
{
  return new r_No_Decomp_Stream(*this);
}
