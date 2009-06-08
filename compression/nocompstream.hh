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
 * INCLUDE: nocompstream.hh
 *
 * MODULE:  compression
 * CLASS:   r_No_Comp_Stream, r_No_Decomp_Stream
 *
 * COMMENTS:
 *
*/

/**
*	@file nocompstream.hh
*
*	@ingroup Compression
*/

#ifndef _NOCOMPSTREAM_H_
#define _NOCOMPSTREAM_H_


#include "compression/lincompstream.hh"



//@ManMemo: Module {\bf compression}

//@Man: compression

/*@Doc:
  Linear compression class implementing simple storage without
  compression.
*/

class r_No_Comp_Stream : public r_Lin_Comp_Stream
{
public:
  /// default constructor
  r_No_Comp_Stream( void );
  /// copy constructor
  r_No_Comp_Stream( const r_No_Comp_Stream& src );
  /// destructor
  ~r_No_Comp_Stream( void );
  /// identification
  r_Lin_Stream_Format get_format( void ) const;
  const char* get_name( void ) const;
  /// cloning
  r_Lin_Comp_Stream* clone( void ) const;
  /// control compression
  int begin( r_Bytes typeSize, r_ULong inputSize=0 );
  int put( const void* data, r_ULong size );
  void* end( r_ULong &size );
  r_ULong end( void );
  void copy_data( void* dest );
};


//@Man: Decompression

/*@Doc:
  Linear decompression class implementing simple storage without
  decompression.
*/
class r_No_Decomp_Stream : public r_Lin_Decomp_Stream
{
public:
  /// default constructor
  r_No_Decomp_Stream( void );
  /// copy constructor
  r_No_Decomp_Stream( const r_No_Decomp_Stream& src );
  /// destructor
  ~r_No_Decomp_Stream( void );
  /// identification
  r_Lin_Stream_Format get_format( void ) const;
  const char* get_name( void ) const;
  /// cloning
  r_Lin_Decomp_Stream* clone( void ) const;
  /// control decompression
  int begin( r_Bytes typeSize, const void* data, r_ULong size,
	     void* dest=NULL );
  int get( void* buffer, r_ULong size );
  int get( r_ULong size );
  int end( void );
};

#endif
