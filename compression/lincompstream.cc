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
 * INCLUDE: lincompstream.cc
 *
 * MODULE:  compression
 * CLASS:   r_LinCompStream, r_LinDecompStream
 *
 * COMMENTS:
 *
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"
#include "raslib/parseparams.hh"
#include "raslib/memblockvec.hh"
#include "compression/lincompstream.hh"
// for creation of linear (de)compression streams from identifiers
#include "compression/nocompstream.hh"
#include "compression/rlestream.hh"
#include "compression/zlibstream.hh"
#include "compression/arithstream.hh"
#include "compression/filestream.hh"
#include "compression/compresstime.hh"



const char r_Linear_Stream::bankNameSep = ':';


r_Linear_Stream::r_Linear_Stream( void )
{
  params = NULL;
  myTimer = NULL;
}

r_Linear_Stream::~r_Linear_Stream( void )
{
  if (params != NULL)
    delete params;
  CBENCH_STATEMENT(if (myTimer != NULL) delete myTimer);
}


r_Linear_Stream::r_Lin_Stream_Format r_Linear_Stream::get_id( const char *name )
{
  if (strcasecmp(name, "none") == 0)
    return r_Lin_Stream_None;
  else if (strcasecmp(name, "rle") == 0)
    return r_Lin_Stream_RLE;
  else if (strcasecmp(name, "zlib") == 0)
    return r_Lin_Stream_ZLib;
  else if (strcasecmp(name, "arith") == 0)
    return r_Lin_Stream_Arith;
  else if (strcasecmp(name, "file") == 0)
    return r_Lin_Stream_File;
  return r_Lin_Stream_NUMBER;
}

const char *r_Linear_Stream::get_format_string( r_Lin_Stream_Format fmt )
{
  switch (fmt)
  {
    case r_Lin_Stream_None:
      return "None";
    case r_Lin_Stream_RLE:
      return "RLE";
    case r_Lin_Stream_ZLib:
      return "ZLib";
    case r_Lin_Stream_Arith:
      return "Arithmetic";
    case r_Lin_Stream_File:
      return "File";
    default:
      break;
  }
  return "???";
}

r_Linear_Stream::r_Lin_Stream_Format *r_Linear_Stream::get_bank_ids( const char *names )
{
  r_Lin_Stream_Format *fmts;
  unsigned int number, i;
  const char *d;
  char buffer[32];
  char *b;

  d = names; number = 1;
  while (*d != '\0')
  {
    if (*d == bankNameSep) number++;
    d++;
  }
  fmts = new r_Lin_Stream_Format[number+1];
  d = names; i = 0;
  while (*d != '\0')
  {
    while (isspace((unsigned int)(*d))) d++;
    b = buffer;
    while ((!isspace((unsigned int)(*d))) && (*d != bankNameSep) && (*d != '\0')) *b++ = *d++;
    *b = '\0';
    if ((fmts[i++] = get_id(buffer)) == r_Lin_Stream_NUMBER)
    {
      RMInit::logOut << "r_Linear_Stream::get_bank_ids(): unknown format \""
		     << buffer << '\"' << endl;
      delete [] fmts;
      return NULL;
    }
    while (isspace((unsigned int)(*d))) d++;
    if (*d == bankNameSep) d++;
  }
  fmts[i] = r_Lin_Stream_NUMBER;
  if (i != number)
  {
    RMInit::logOut << "r_Linear_Stream::get_bank_ids(): warning, inconsistent stream number "
		   << i << ", should be " << number;
  }
  return fmts;
}

char *r_Linear_Stream::get_bank_string( const r_Lin_Stream_Format *ids )
{
  unsigned int i, len;
  char *result, *b;
  
  len = 0;
  for (i=0; ids[i] != r_Lin_Stream_NUMBER; i++)
  {
    len += strlen(get_format_string(ids[i])) + 1;
  }
  result = new char[len+1];
  b = result;
  for (i=0; ids[i] != r_Lin_Stream_NUMBER; i++)
  {
    const char *str = get_format_string(ids[i]);
    strcpy(b, str);
    b += strlen(str);
    *b++ = bankNameSep;
  }
  // remove trailing name separator
  if (b != result) b--;
  *b = '\0';

  return result;
}

int r_Linear_Stream::set_num_symbols( unsigned int syms )
{
  // by default this is ignored by most streams (except for arithmetic coding)
  return 0;
}


// this method should basically only be called when COMPBENCHMARK is defined
void r_Linear_Stream::instantiate_timer( const char *func, int level )
{
#ifdef COMPBENCHMARK
  if (myTimer != NULL)
    delete myTimer;

  myTimer = new RMTimer(get_name(), func, level);

  myTimer->start();
#endif
}


std::ostream &operator<<( std::ostream &s, r_Linear_Stream::r_Lin_Stream_Format fmt )
{
  s << r_Linear_Stream::get_format_string(fmt);
  return s;
}


/*
 *  Linear compression storage
 */

r_Lin_Comp_Store::r_Lin_Comp_Store( r_ULong bsize )
{
  mblocks = NULL;
  streamer = NULL;
  cacheSize = bsize;
  cacheOff = 0;
  cache = NULL;
}

r_Lin_Comp_Store::r_Lin_Comp_Store( r_Lin_Comp_Stream *str, r_ULong csize )
{
  mblocks = NULL;
  streamer = str;
  cacheSize = csize;
  cacheOff = 0;
  cache = NULL;
}

r_Lin_Comp_Store::~r_Lin_Comp_Store( void )
{
  // although cache is used in both modes, it's merely a pointer to a block within
  // mblocks in static mode and mustn't be freed directly in that case.
  if (mblocks != NULL)
  {
    delete mblocks;
  }
  else if (cache != NULL)
  {
    delete [] ((char*)cache);
  }
}

int r_Lin_Comp_Store::start( r_Bytes typeSize )
{
  if (streamer != NULL)
  {
    RMDBGONCE( 3, RMDebug::module_compression, "r_Lin_Comp_Store", "start() stream " << streamer->get_name() );
    streamer->begin(typeSize);
    cache = new char[cacheSize];
  }
  else
  {
    RMDBGONCE( 3, RMDebug::module_compression, "r_Lin_Comp_Store", "start() static, bsize " << cacheSize );
    mblocks = new r_Memory_Block_Vector(cacheSize);
    cache = mblocks->add();
  }

  if (cache == NULL)
    return -1;

  return 0;
}

void *r_Lin_Comp_Store::frag_ptr( void )
{
  return (void*)(((char*)cache) + cacheOff);
}

r_ULong r_Lin_Comp_Store::frag_size( void ) const
{
  return cacheSize - cacheOff;
}

int r_Lin_Comp_Store::frag_stored( r_ULong size )
{
  if (size != 0)
  {
    cacheOff += size;
    if (cacheOff >= cacheSize)
    {
      if (streamer == NULL)
      {
	cache = mblocks->add();
      }
      else
      {
	if (streamer->put(cache, cacheOff) < 0)
	  return -1;
      }
      cacheOff = 0;
    }
  }
  return 0;
}

int r_Lin_Comp_Store::put( const void *data, r_ULong size )
{
  const char *dptr = (const char*)data;
  r_ULong left = size;

  // note: cache in static mode!
  while (left != 0)
  {
    r_ULong rest;

    rest = cacheSize - cacheOff;
#ifdef RMANDEBUG
    if (rest == 0)
    {
      RMInit::logOut << "r_Lin_Comp_Store::put(): fatal error!" << endl;
      throw r_Error (COMPRESSIONFAILED);      
    }
#endif
    if (rest > left)
      rest = left;

    if (rest != 0)
    {
      memcpy(((char*)cache) + cacheOff, dptr, rest);

      if (frag_stored(rest) < 0)
	return -1;

      left -= rest;
      dptr += rest;
    }
  }
  return 0;
}

int r_Lin_Comp_Store::put( unsigned char val )
{
  ((unsigned char*)cache)[cacheOff] = val;
  return frag_stored(1);
}

int r_Lin_Comp_Store::flush( void )
{
  if (streamer != NULL)
  {
    if (cacheOff != 0)
    {
      if (streamer->put(cache, cacheOff) < 0)
	return -1;

      cacheOff = 0;
    }
  }
  return 0;
}

r_ULong r_Lin_Comp_Store::stop( void )
{
  RMDBGONCE( 3, RMDebug::module_compression, "r_Lin_Comp_Store", "stop()" );
  if (streamer == NULL)
  {
    return mblocks->get_size(cacheOff);
  }
  else
  {
    if (flush() < 0)
      return 0;

    return streamer->end();
  }
}

void r_Lin_Comp_Store::copy_data( void *dest )
{
  if (streamer == NULL)
  {
    mblocks->copy_data(dest, cacheOff);
  }
  else
  {
    streamer->copy_data(dest);
  }
}

void r_Lin_Comp_Store::print_status( std::ostream &str ) const
{
  if (streamer == NULL)
  {
    str << "[static; " << mblocks->get_number() << " blocks, " << mblocks->get_size(cacheOff) << " bytes]";
  }
  else
  {
    str << "[stream: " << streamer->get_name() << ']';
  }
}


std::ostream &operator<<( std::ostream &str, const r_Lin_Comp_Store &store )
{
  store.print_status(str);
  return str;
}




/*
 *  Linear compression encoders...
 */
r_Lin_Comp_Stream::r_Lin_Comp_Stream( const r_Lin_Comp_Stream &src ) : r_Linear_Stream()
{
  target = NULL;	// anything else would have bad side-effects...
  streamer = src.streamer;
  blockSize = src.blockSize;
  autoDeleteStreams = 0;
}

r_Lin_Comp_Stream::r_Lin_Comp_Stream( void ) : r_Linear_Stream()
{
  target = NULL;
  streamer = NULL;
  blockSize = 4096;
  autoDeleteStreams = 0;
}

r_Lin_Comp_Stream::~r_Lin_Comp_Stream( void )
{
  if (target != NULL)
    delete target;

  if (autoDeleteStreams != 0)
    free_streams();
}

void r_Lin_Comp_Stream::set_block_size( r_ULong bsize )
{
  blockSize = bsize;
}

void r_Lin_Comp_Stream::set_stream( r_Lin_Comp_Stream *str )
{
  streamer = str;
}

r_Lin_Comp_Stream *r_Lin_Comp_Stream::get_stream( void )
{
  return streamer;
}

const r_Lin_Comp_Stream *r_Lin_Comp_Stream::get_stream( void ) const
{
  return streamer;
}

void r_Lin_Comp_Stream::print_bank_name( std::ostream &str ) const
{
  const r_Lin_Comp_Stream *s = streamer;

  str << get_name();
  while (s != NULL)
  {
    str << " : " << s->get_name();
    s = s->get_stream();
  }
}

void r_Lin_Comp_Stream::free_streams( void )
{
  r_Lin_Comp_Stream *s = streamer;

  while (s != NULL)
  {
    r_Lin_Comp_Stream *next = s->get_stream();
    delete s;
    s = next;
  }
}

const char *r_Lin_Comp_Stream::get_stream_file( void ) const
{
  const r_Lin_Comp_Stream *s = this;

  while (s != NULL)
  {
    if (s->get_format() == r_Lin_Stream_File)
      return ((r_File_Comp_Stream*)s)->get_file_name();
    s = s->streamer;
  }
  return NULL;
}

int r_Lin_Comp_Stream::set_stream_file( const char *name )
{
  r_Lin_Comp_Stream *s = this;

  while (s != NULL)
  {
    if (s->get_format() == r_Lin_Stream_File)
    {
      ((r_File_Comp_Stream*)s)->set_file_name(name);
      return 1;
    }
    s = s->streamer;
  }
  return 0;
}

void r_Lin_Comp_Stream::set_storage_handler( const r_Storage_Man &newStore )
{
  mystore = newStore;
}

void r_Lin_Comp_Stream::set_params( const char *str )
{
  if (params != NULL)
    params->process(str);
}

void r_Lin_Comp_Stream::init_target( void )
{
  if (target != NULL)
    delete target;

  if (streamer == NULL)
    target = new r_Lin_Comp_Store(blockSize);
  else
    target = new r_Lin_Comp_Store(streamer);
}

void r_Lin_Comp_Stream::exit_target( void )
{
  if (target != NULL)
  {
    delete target;
    target = NULL;
  }
}


// static member function for creation
r_Lin_Comp_Stream* r_Lin_Comp_Stream::create( r_Lin_Stream_Format fmt, const char *pstr ) throw(r_Error)
{
  switch (fmt)
  {
    case r_Lin_Stream_None:
      return new r_No_Comp_Stream();
    case r_Lin_Stream_RLE:
      return new r_RLE_Comp_Stream();
    case r_Lin_Stream_ZLib:
      return new r_ZLib_Comp_Stream( pstr );
    case r_Lin_Stream_Arith:
      return new r_Arith_Comp_Stream();
    case r_Lin_Stream_File:
      return new r_File_Comp_Stream( pstr);
    default:
      RMInit::logOut << "Unknown linear compression format " << fmt << endl;
      r_Error err(r_Error::r_Error_General);
      throw(err);
  }
  return NULL;
}

// allocate a stream bank
r_Lin_Comp_Stream* r_Lin_Comp_Stream::create( r_Lin_Stream_Format *ids, const char *pstr ) throw(r_Error)
{
  r_Lin_Comp_Stream *result = NULL;
  unsigned int i;

  for (i=0; ids[i] != r_Lin_Stream_NUMBER; i++) ;
  try
  {
    for (; i!=0; i--)
    {
      r_Lin_Comp_Stream *str = create(ids[i-1], pstr);
      str->set_stream(result);
      result = str;
    }
  }
  catch( r_Error &err )
  {
    if (result != NULL)
    {
      result->free_streams();
      delete result;
    }
    throw(err);
  }
  // mark the root stream as a stream bank owning all child streams
  if (result != NULL)
    result->autoDeleteStreams = 1;

  return result;
}


std::ostream &operator<<( std::ostream &str, const r_Lin_Comp_Stream &comp )
{
  comp.print_bank_name(str);
  return str;
}



/*
 *  Linear decompression storage
 */

r_Lin_Decomp_Store::r_Lin_Decomp_Store( void )
{
  RMDBGONCE( 3, RMDebug::module_compression, "r_Lin_Decomp_Store", "r_Lin_Decomp_Store()" );
  streamer = NULL;
  cache = NULL;
}

r_Lin_Decomp_Store::r_Lin_Decomp_Store( r_Lin_Decomp_Stream *str, r_ULong csize )
{
  RMDBGONCE( 3, RMDebug::module_compression, "r_Lin_Decomp_Store", "r_Lin_Decomp_Store(" << str->get_name() << ')' );
  streamer = str;
  cacheSize = csize;
  cache = new char[cacheSize];
}

r_Lin_Decomp_Store::~r_Lin_Decomp_Store( void )
{
  if (cache != NULL)
    delete [] (char*)cache;
}

int r_Lin_Decomp_Store::start( r_Bytes typeSize, const void *data, r_ULong size )
{
  if (streamer == NULL)
  {
    RMDBGONCE( 4, RMDebug::module_compression, "r_Lin_Decomp_Store", "start() static" );
    dataBase = data;
    dataSize = size;
    dataOffset = 0;
  }
  else
  {
    RMDBGONCE( 4, RMDebug::module_compression, "r_Lin_Decomp_Store", "start() stream " << streamer->get_name() );
    cacheLevel = 0;
    cacheOff = 0;
    return streamer->begin(typeSize, data, size);
  }
  return 0;
}

int r_Lin_Decomp_Store::ensure_data( void )
{
  // in static mode, we don't have to do anything.
  if ((streamer != NULL) && (cacheOff >= cacheLevel))
  {
    // returns 0 for OK or the _negative_ number of unread bytes
    int status = streamer->get(cache, cacheSize);
    if (status < 0)
    {
      cacheLevel = (r_ULong)((r_Long)cacheSize + (r_Long)status);
      if (cacheLevel == 0)
	return -1;
    }
    else
    {
      // total refill of cache possible
      cacheLevel = cacheSize;
    }
    cacheOff = 0;
    //cout << "CACHE LEVEL " << cacheLevel << endl;
    return 1;
  }
  return 0;
}

int r_Lin_Decomp_Store::get( void *data, r_ULong size )
{
  r_ULong rest;

  if (streamer == NULL)
  {
    rest = dataSize - dataOffset;
    if (rest > size)
      rest = size;
    memcpy(data, ((const char*)dataBase) + dataOffset, rest);
    dataOffset += rest;
    if (rest < size)
      return -((int)(size - rest));
  }
  else
  {
    r_ULong left = size;
    char *dptr = (char*)data;

    while (left != 0)
    {
      if (ensure_data() < 0)
	return -((int)left);

      rest = cacheLevel - cacheOff;
      if (rest > left)
	rest = left;

      memcpy(dptr, ((char*)cache) + cacheOff, rest);
      dptr += rest;
      cacheOff += rest;
      left -= rest;
    }
  }
  return 0;
}

int r_Lin_Decomp_Store::get( unsigned char &val )
{
  if (streamer == NULL)
  {
    if (dataOffset >= dataSize)
      return -1;

    val = ((unsigned char*)dataBase)[dataOffset++];
  }
  else
  {
    if (ensure_data() < 0)
      return -1;

    val = ((unsigned char*)cache)[cacheOff++];
  }
  return 0;
}

int r_Lin_Decomp_Store::stop( void )
{
  RMDBGONCE( 4, RMDebug::module_compression, "r_Lin_Decomp_Store", "stop()" );
  if (streamer != NULL)
  {
    return streamer->end();
  }
  return 0;
}

const void *r_Lin_Decomp_Store::frag_ptr( void ) const
{
  if (streamer == NULL)
    return (const void*)(((char*)dataBase)+dataOffset);

  return (const void*)(((char*)cache)+cacheOff);
}

r_ULong r_Lin_Decomp_Store::frag_size( void ) const
{
  if (streamer == NULL)
    return dataSize - dataOffset;

  return cacheLevel - cacheOff;
}

int r_Lin_Decomp_Store::frag_read( r_ULong size )
{
  if (streamer == NULL)
    dataOffset += size;
  else
    cacheOff += size;

  return 0;
}

void r_Lin_Decomp_Store::print_status( std::ostream &str ) const
{
  if (streamer == NULL)
  {
    str << "[static: current " << dataOffset << ", total " << dataSize << ']';
  }
  else
  {
    str << "[stream: " << streamer->get_name() << ']';
  }
}


std::ostream &operator<<( std::ostream &str, const r_Lin_Decomp_Store &store )
{
  store.print_status(str);
  return str;
}



/*
 *  linear compression decoders
 */
r_Lin_Decomp_Stream::r_Lin_Decomp_Stream( void )
{
  params = NULL;
  source = NULL;
  streamer = NULL;
  autoDeleteStreams = 0;
}

r_Lin_Decomp_Stream::~r_Lin_Decomp_Stream( void )
{
  if (source != NULL)
    delete source;

  if (autoDeleteStreams != 0)
    free_streams();
}

void r_Lin_Decomp_Stream::set_params( const char *str )
{
  if (params != NULL)
    params->process(str);
}

void r_Lin_Decomp_Stream::set_stream( r_Lin_Decomp_Stream *str )
{
  streamer = str;
}

r_Lin_Decomp_Stream *r_Lin_Decomp_Stream::get_stream( void )
{
  return streamer;
}

const r_Lin_Decomp_Stream *r_Lin_Decomp_Stream::get_stream( void ) const
{
  return streamer;
}

void r_Lin_Decomp_Stream::print_bank_name( std::ostream &str ) const
{
  const r_Lin_Decomp_Stream *s = streamer;

  str << get_name();
  while (s != NULL)
  {
    str << " : " << s->get_name();
    s = s->get_stream();
  }
}

void r_Lin_Decomp_Stream::free_streams( void )
{
  r_Lin_Decomp_Stream *s = streamer;

  while (s != NULL)
  {
    r_Lin_Decomp_Stream *next = s->get_stream();
    delete s;
    s = next;
  }
}

const char *r_Lin_Decomp_Stream::get_stream_file( void ) const
{
  const r_Lin_Decomp_Stream *s = this;

  while (s != NULL)
  {
    if (s->get_format() == r_Lin_Stream_File)
      return ((r_File_Decomp_Stream*)s)->get_file_name();
    s = s->streamer;
  }
  return NULL;
}

int r_Lin_Decomp_Stream::set_stream_file( const char *name )
{
  r_Lin_Decomp_Stream *s = this;

  while (s != NULL)
  {
    if (s->get_format() == r_Lin_Stream_File)
    {
      ((r_File_Decomp_Stream*)s)->set_file_name(name);
      return 1;
    }
    s = s->streamer;
  }
  return 0;
}

void r_Lin_Decomp_Stream::init_source( void )
{
  if (source != NULL)
    delete source;

  if (streamer == NULL)
    source = new r_Lin_Decomp_Store();
  else
    source = new r_Lin_Decomp_Store(streamer);
}

void r_Lin_Decomp_Stream::exit_source( void )
{
  delete source;
  source = NULL;
}

// static member function for creation
r_Lin_Decomp_Stream* r_Lin_Decomp_Stream::create( r_Lin_Stream_Format fmt, const char *pstr ) throw(r_Error)
{
  switch (fmt)
  {
    case r_Lin_Stream_None:
      return new r_No_Decomp_Stream();
    case r_Lin_Stream_RLE:
      return new r_RLE_Decomp_Stream();
    case r_Lin_Stream_ZLib:
      return new r_ZLib_Decomp_Stream();
    case r_Lin_Stream_Arith:
      return new r_Arith_Decomp_Stream();
    case r_Lin_Stream_File:
      return new r_File_Decomp_Stream( pstr );
    default:
      RMInit::logOut << "Unknown linear compression format " << fmt << endl;
      r_Error err(r_Error::r_Error_General);
      throw(err);
  }
  return NULL;
}

// allocate a stream bank
r_Lin_Decomp_Stream* r_Lin_Decomp_Stream::create( r_Lin_Stream_Format *ids, const char *pstr ) throw(r_Error)
{
  r_Lin_Decomp_Stream *result = NULL;
  unsigned int i;

  for (i=0; ids[i] != r_Lin_Stream_NUMBER; i++) ;
  try
  {
    for (; i!=0; i--)
    {
      r_Lin_Decomp_Stream *str = create(ids[i-1], pstr);
      str->set_stream(result);
      result = str;
    }
  }
  catch( r_Error &err )
  {
    if (result != NULL)
    {
      result->free_streams();
      delete result;
    }
    throw(err);
  }
  // mark the root stream as a stream bank owning all the child streams
  if (result != NULL)
    result->autoDeleteStreams = 1;

  return result;
}


std::ostream &operator<<( std::ostream &str, const r_Lin_Decomp_Stream &decomp )
{
  decomp.print_bank_name(str);
  return str;
}



/*
 *  The container object for matching compression / decompression streams
 */

r_Lin_Codec_Stream::r_Lin_Codec_Stream( void ) : comp(NULL), decomp(NULL)
{
}


r_Lin_Codec_Stream::r_Lin_Codec_Stream( r_Linear_Stream::r_Lin_Stream_Format fmt, const char *pstr )
{
  comp = r_Lin_Comp_Stream::create(fmt, pstr);
  decomp = r_Lin_Decomp_Stream::create(fmt, pstr);
}


r_Lin_Codec_Stream::r_Lin_Codec_Stream( r_Linear_Stream::r_Lin_Stream_Format *ids, const char *pstr )
{
  comp = r_Lin_Comp_Stream::create(ids, pstr);
  decomp = r_Lin_Decomp_Stream::create(ids, pstr);
}


r_Lin_Codec_Stream::r_Lin_Codec_Stream( const r_Lin_Codec_Stream &lc )
{
  comp = lc.comp->clone();
  decomp = lc.decomp->clone();
}


r_Lin_Codec_Stream::~r_Lin_Codec_Stream( void )
{
  if (comp != NULL)
    delete comp;
  if (decomp != NULL)
    delete decomp;
}
