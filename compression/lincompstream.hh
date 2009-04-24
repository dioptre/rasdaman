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
 * INCLUDE: lincompstream.hh
 *
 * MODULE:  compression
 * CLASS:   r_Lin_Comp_Stream, r_Lin_Decomp_Stream
 *
 * PURPOSE:
 * Abstract base class for linear compression streams
 *
 * COMMENTS:
 *
*/

#ifndef _LINCOMPSTREAM_H_
#define _LINCOMPSTREAM_H_


#include <stdio.h>
#include <iostream>

#include "raslib/error.hh"
#include "raslib/storageman.hh"
#include "raslib/odmgtypes.hh"




class r_Parse_Params;
class RMTimer;


//@ManMemo: Module {\bf compression}

/*@Doc:
  r_Linear_Stream:
  Abstract base class for linear compression and decompression strings.
  Implements enumerator and common interface calls.
  Linear streams are used to compress or uncompress data in a given format.
  The can operate in static mode (the compressed data is stored to / read
  from memory directly) or in streaming mode (the compressed data is stored
  to / read from another linear stream, without having to buffer intermediate
  data explicitly or in its entirety). Streaming mode results in nesting
  linear streams, which can be done to arbitrary depth.
*/

class r_Linear_Stream
{
  public:
  // stream identifiers
  enum r_Lin_Stream_Format {
    r_Lin_Stream_None,
    r_Lin_Stream_RLE,
    r_Lin_Stream_ZLib,
    r_Lin_Stream_Arith,
    r_Lin_Stream_File,
    r_Lin_Stream_NUMBER	// used as error code
  };
  /// default constructor
  r_Linear_Stream( void );
  /// destructor
  virtual ~r_Linear_Stream( void );
  /// identification
  virtual r_Lin_Stream_Format get_format( void ) const = 0;
  /// get the name of the stream, returns r_Lin_Stream_Void if unknown
  virtual const char* get_name(void) const = 0;
  /// set compression parameters from a string
  virtual void set_params( const char *str ) = 0;
  /// set number of symbols in the stream (used by e.g. arithmetic coders)
  virtual int set_num_symbols( unsigned int syms );
  /// get stream identifier from name
  static r_Lin_Stream_Format get_id( const char *name );
  /**
     the name (case independent) may be one of the following

     \begin{tabular}{ll}
     none && r_No_(De)Comp_Stream\\
     rle && r_RLE_(De)Comp_Stream\\
     zlib && r_ZLib_(De)Comp_Stream\\
     arith && r_Arith_(De)Comp_Stream\\
     file && r_File_(De)Comp_Stream\\
     end{tabular}
  */
  /// return the string representation of the given format
  static const char *get_format_string( r_Lin_Stream_Format fmt );
  /// return a linear stream bank (concatenated streams), terminated by r_Lin_Stream_NUMBER
  static r_Lin_Stream_Format *get_bank_ids( const char *names );
  /// return a string representation of the stream bank (caller frees with delete[])
  static char *get_bank_string( const r_Lin_Stream_Format *ids );


  protected:
  /// instantiate the timer for benchmarking with a certain level
  void instantiate_timer( const char *func, int level=0 );
  /// parameters
  r_Parse_Params *params;
  /// timer for benchmarking; always defined to keep object size constant
  RMTimer *myTimer;
  /// the character used to separate stream names in stream banks
  static const char bankNameSep;
};


//@ManMemo: Module {\bf compression}

/// string output
extern std::ostream &operator<<( std::ostream &s, r_Linear_Stream::r_Lin_Stream_Format fmt );



//@ManMemo: Module {\bf compression}

/*@Doc:
  r_Lin_Comp_Store:
  Class abstracting between storing compressed data statically in an object of
  type r_Memory_Block_Vector or another r_Lin_Comp_Stream to unify normal and
  (nested) streaming interface. There is a high-level and a low-level interface.
  The high-level interface allows writing a specified number of bytes to the
  object (which will redirect it to memory or stream automatically) and is the
  preferred way. The low-level interface allows block-oriented, external
  compression libraries like ZLib to operate by giving it a block to store
  data to. In this case, use frag_ptr() to get the address of a block of
  memory (``fragment'') which can be written to, the maximum size of which can
  be obtained with frag_size() and write to it. After your data has been
  written, you MUST call frag_stored() with the number of bytes you wrote to
  frag_ptr(), otherwise subsequent calls of frag_ptr() will all return the
  same data. See zlibstream for an implementation using the low-level
  interface and the other streams on how the high-level interface is used.
*/

class r_Lin_Comp_Stream;
class r_Memory_Block_Vector;

class r_Lin_Comp_Store
{
  public:
  /// constructor for static mode, receiving block size for r_Memory_Block_Vector.
  r_Lin_Comp_Store( r_ULong bsize );
  /// constructor for streaming mode, receiving stream compression object and cache size
  r_Lin_Comp_Store( r_Lin_Comp_Stream *str, r_ULong csize=4096 );
  /// destructor
  ~r_Lin_Comp_Store( void );

  /// start output
  int start( r_Bytes typeSize );
  /// write data; return 0 for OK, -1 for error
  int put( const void *data, r_ULong size );
  /// write single byte
  int put( unsigned char val );
  /// stop outputting data and return size
  r_ULong stop( void );
  /// copy the data
  void copy_data( void *dest );
  /// flush data
  int flush( void );
  /// get pointer to current fragment (low-level interface)
  void *frag_ptr( void );
  /// get the remaining space in the current fragmend (low-level interface)
  r_ULong frag_size( void ) const;
  /// notify that size bytes have been store to current fragmemt (low-level interface)
  int frag_stored( r_ULong size );
  /// print status
  void print_status( std::ostream &str ) const;


  protected:
  /// memory block vector for static mode
  r_Memory_Block_Vector *mblocks;
  /// compression stream for streaming mode
  r_Lin_Comp_Stream *streamer;
  /// cache in streaming mode
  void *cache;
  /// size and current offset in both modes
  r_ULong cacheSize;
  r_ULong cacheOff;
};

extern std::ostream &operator<<( std::ostream &str, const r_Lin_Comp_Store &store );



//@ManMemo: Module {\bf compression}

/*@Doc:
  Abstract base class for linear compression streams. Memory allocation is all
  done transparently in the background. This class provides a common interface
  to compression methods like zlib or RLE. It can operate in static or streaming
  mode. In static mode, compressed data is stored explicitly in memory as a whole,
  whereas in streaming mode, data merely streams through the object and is piped
  into another object of type r_Lin_Comp_Stream; only a small amount of compressed
  data is buffered internally for more efficiency. Use set_stream(str) with str != NULL
  to activate streaming mode, using str as the receiving class.
*/

class r_Lin_Comp_Stream : public r_Linear_Stream
{
  public:
  /// default constructor
  r_Lin_Comp_Stream( void );
  /// copy constructor
  r_Lin_Comp_Stream( const r_Lin_Comp_Stream &src );
  /// destructor
  virtual ~r_Lin_Comp_Stream( void );

  /// change memory handlers (don't change between begin() ... end()!)
  virtual void set_storage_handler( const r_Storage_Man &newStore );

  /// creation
  static r_Lin_Comp_Stream* create( r_Lin_Stream_Format fmt, const char *pstr=NULL ) throw(r_Error);
  /**
     fmt may be one of the following:

     \begin{tabular}{ll}
     r_Lin_Stream_None && r_No_(De)Comp_Stream\\
     r_Lin_Stream_RLE && r_RLE_(De)Comp_Stream\\
     r_Lin_Stream_ZLib && r_ZLib_(De)Comp_Stream\\
     r_Lin_Stream_Arith && r_Arith_(De)Comp_Stream\\
     r_Lin_Stream_File && r_File_(De)Comp_Stream\\
     \end{tabular}

     In addition, theres r_Lin_Stream_NUMBER used as error code.
  */

  /// create a stream bank (format as returned by r_Linear_Stream::get_bank_ids() )
  static r_Lin_Comp_Stream* create( r_Lin_Stream_Format *ids, const char *pstr=NULL ) throw(r_Error);

  /// cloning
  virtual r_Lin_Comp_Stream* clone( void ) const = 0;
  
  /// set block size
  void set_block_size( r_ULong bsize );
  /// select streaming interface
  void set_stream( r_Lin_Comp_Stream *str );
  /*
    Streamed compression doesn't buffer the data but writes it to the specified
    lincompstream instead. This allows easy and efficient concatenation of
    compression streams.
  */
  /// get read/write streamer object
  r_Lin_Comp_Stream *get_stream( void );
  /// get read-only streamer object
  const r_Lin_Comp_Stream *get_stream( void ) const;
  /// print the name of the entire stream bank (recursively)
  void print_bank_name( std::ostream &str ) const;
  /// free all child streams (recursively set by set_stream() )
  void free_streams( void );
  /// get the name of the output file, or NULL if stream bank doesn't contain file stream
  const char *get_stream_file( void ) const;
  /// set the name of the output file if stream bank contains file stream, otherwise return 0
  int set_stream_file( const char *name );


  //@Man: Interface
  //@{
  /**
     typeSize is the size of the base type to encode which is important
     for some derived classes (e.g. RLE). Make sure that all size arguments
     must be multiples of typeSize later on. The value 1 should always
     work, but will usually not be optimal.
     the inputSize parameter of begin() is the size of the input data or
     0 for using defaults). This parameter directly influences the size
     of the input buffer for some streams and should therefore be used
     with care.
  */
  /// begin compression
  virtual int begin( r_Bytes typeSize, r_ULong inputSize=0 ) = 0;
  /// write data to stream
  virtual int put( const void* data, r_ULong size ) = 0;
  /// end compression
  virtual void* end( r_ULong &size ) = 0;
  /**
     standard end call, allocates new (linear) memory, copies the data
     there and frees all internal data
  */
  /// alternative end call, just finalizes the stream and returns the size
  virtual r_ULong end( void ) = 0;
  /// copy output data into linear memory
  virtual void copy_data( void* dest ) = 0;
  //@}
  virtual void set_params( const char *str );


  protected:
  /// init the target object according to the mode
  void init_target( void );
  /// completely finished the target object
  void exit_target( void );
  /// memory management object, defaults to C-style
  r_Storage_Man mystore;
  /// storage abstraction object
  r_Lin_Comp_Store *target;
  /// compression stream for streaming mode
  r_Lin_Comp_Stream *streamer;
  /// block size for static mode
  r_ULong blockSize;
  /// automatically delete all child streams in destructor if bank constructor was used
  int autoDeleteStreams;
};


//@ManMemo: Moduke {\bf compression}

//@Doc: print the stream (bank) name of the compression stream to a C++ stream

extern std::ostream &operator<<( std::ostream &str, const r_Lin_Comp_Stream &comp );



//@ManMemo: Module {\bf compression}

/*@Doc:
  r_Lin_Decomp_Store:
  Class abstracting between loading data from raw memory or from another r_Lin_Decomp_Stream
  to unify normal and (nested) streaming interface. As in r_Lin_Comp_Store, there's a
  high-level and a low-level interface. The high-level interface can be used to request
  a specified number of bytes from the storage source (memory or another stream). The
  low-level interface can be used for external, block-oriented compression libraries
  like ZLib. Use frag_ptr() to get the start address of the input data and frag_size()
  for the maximum number of bytes you can read from there. After you read a block, you
  must call frag_read() with the size of the block as argument, otherwise frag_ptr()
  will always return the same data. In addition, you should always call ensure_data()
  before you call frag_ptr(). For an implementation of the low-level interface see
  zlibstream and the other streams on how the high-level interface should be used.
 */

class r_Lin_Decomp_Stream;

class r_Lin_Decomp_Store
{
  public:
  /// constructor for static mode
  r_Lin_Decomp_Store( void );
  /// constructor for streaming mode, receiving stream decompression object and cache size
  r_Lin_Decomp_Store( r_Lin_Decomp_Stream *str, r_ULong csize=4096 );
  /// destructor
  ~r_Lin_Decomp_Store( void );

  /// start reading data
  int start( r_Bytes typeSize, const void *data, r_ULong size );
  /// read data, returns 0 for OK, negative number of unread bytes otherwise
  int get( void *data, r_ULong size );
  /// read single byte; returns 0 for OK, -1 for error
  int get( unsigned char &val );
  /// stop reading data
  int stop( void );
  /// makes sure there's some data in the cache in streaming mode, returns -1 if EOF
  int ensure_data( void );
  /// get pointer to current fragment (low-level interface)
  const void *frag_ptr( void ) const;
  /// get the valid bytes in the current fragment (low-level interface)
  r_ULong frag_size( void ) const;
  /// notify that size bytes have been read from the current fragment
  int frag_read( r_ULong size );
  /// print status
  void print_status( std::ostream &str ) const;
  

  protected:
  /// start of compressed data in static mode
  const void *dataBase;
  /// size of compressed data in static mode
  r_ULong dataSize;
  /// current offset in compressed data in static mode
  r_ULong dataOffset;
  /// decompression stream in streaming mode
  r_Lin_Decomp_Stream *streamer;
  /// cache in streaming mode
  void *cache;
  /// cache size, current fill level and current offset in streaming mode
  r_ULong cacheSize;
  r_ULong cacheLevel;
  r_ULong cacheOff;
};

extern std::ostream &operator<<( std::ostream &str, const r_Lin_Decomp_Store &store );



//@ManMemo: Module {\bf compression}

/*@Doc:
  r_Lin_Decomp_Stream:
  Abstract base class for linear decompression streams. Linear decompression streams
  can operate in static mode (compressed data is read directly from memory) and
  streaming mode (compressed data is read from another linear decompression stream).
  In streaming mode, only a small amount of compressed data is ever buffered
  internally for more efficiency. Use set_stream(str) with str != NULL to activate
  streaming mode, using str as the stream providing the input data for this stream.
  The data and size passed to the begin() method actually belong to str and are
  passed to it automatically.
 */

class r_Lin_Decomp_Stream : public r_Linear_Stream
{
  public:
  /// default constructor
  r_Lin_Decomp_Stream( void );
  /// destructor
  virtual ~r_Lin_Decomp_Stream( void );

  /// creation
  static r_Lin_Decomp_Stream* create( r_Lin_Stream_Format fmt, const char *pstr=NULL ) throw(r_Error);
  /**
     for the values of fmt see r_Lin_Comp_Stream::create()
  */
  /// create a stream bank
  static r_Lin_Decomp_Stream* create( r_Lin_Stream_Format *ids, const char *pstr=NULL ) throw(r_Error);

  /// cloning
  virtual r_Lin_Decomp_Stream* clone( void ) const = 0;

  /// select streaming interface
  void set_stream( r_Lin_Decomp_Stream *str );
  /*
    Streamed decompression doesn't read the data from linear memory but from another
    object of type r_Lin_Decomp_Stream instead to allow easy and efficient concatenation
    of decompression streams.
  */
  /// get read/write streamer object
  r_Lin_Decomp_Stream *get_stream( void );
  /// get read-only streamer object
  const r_Lin_Decomp_Stream *get_stream( void ) const;
  /// print the name of the entire stream bank (recursively)
  void print_bank_name( std::ostream &str ) const;
  /// free all child streams
  void free_streams( void );
  /// get the name of the output file, or NULL if stream bank doesn't contain file stream
  const char *get_stream_file( void ) const;
  /// set the name of the output file if stream bank contains file stream, otherwise return 0
  int set_stream_file( const char *name );

  //@Man: Interface
  //@{
  /**
     You can use three approaches:
     \begin{enumerate}
     \item
     supply dest to begin() and use the get(size) call which will
     store data to dest and increment dest afterwards, or
     \item
     use get(buffer, size) (in that case dest is irrelevant), or
     \item
     supply dest to begin() to store the pointer internally, retrieve
     the pointer later using getDestPtr() and use it in some other
     way in get(buffer, size) calls.
     \end{enumerate}
  */
  /// get the pointer to destination stored within the object
  inline void* get_dest_ptr(void) {return destPtr;}
  /// start decompression.
  virtual int begin( r_Bytes typeSize, const void* data, r_ULong size,
		     void* dest=NULL ) = 0;
  /// get() returns 0 for OK, otherwise the negative number of non-read bytes
  virtual int get( void* buffer, r_ULong size ) = 0;
  /// get data and store it in the internal destptr (which is incremented)
  virtual int get( r_ULong size ) = 0;
  /// end decompression
  virtual int end( void ) = 0;
  //@}
  /// set compression parameters from string
  virtual void set_params( const char *str );


  protected:
  /// prepare the storage abstraction object
  void init_source( void );
  /// finished with the storage abstraction object
  void exit_source( void );
  /// default destination
  void* destPtr;
  /// storage abstraction object
  r_Lin_Decomp_Store *source;
  /// stream object in streaming mode
  r_Lin_Decomp_Stream *streamer;
  /// automatically delete all child streams in destructor if bank constructor was used
  int autoDeleteStreams;
};


//@ManMemo: Module {\bf compression}

//@Doc: print the stream (bank) name of the decompression stream to a C++ stream

extern std::ostream &operator<<( std::ostream &str, const r_Lin_Decomp_Stream &decomp );




//@ManMemo: Module {\bf compression}

/*@Doc:
  Container class for matching compression and decompression streams
*/

class r_Lin_Codec_Stream
{
  public:
  /// default constructor, should not be used
  r_Lin_Codec_Stream( void );
  /// constructor that creates the codecs
  r_Lin_Codec_Stream( r_Linear_Stream::r_Lin_Stream_Format fmt, const char *pstr=NULL );
  /**
     for the values of fmt see r_Lin_Comp_Stream::create()
  */
  /// constructor for a codec bank
  r_Lin_Codec_Stream( r_Linear_Stream::r_Lin_Stream_Format *ids, const char *pstr=NULL );

  /// copy constructor
  r_Lin_Codec_Stream( const r_Lin_Codec_Stream &lc );
  /// destructor
  ~r_Lin_Codec_Stream( void );

  /// get the stream format
  inline r_Linear_Stream::r_Lin_Stream_Format get_format( void ) {
    return comp->get_format();
  }
  /// get the compression stream
  inline r_Lin_Comp_Stream *get_comp_stream( void ) {
    return comp;
  }
  /// get the decompression stream
  inline r_Lin_Decomp_Stream *get_decomp_stream( void ) {
    return decomp;
  }


  protected:
  r_Lin_Comp_Stream *comp;
  r_Lin_Decomp_Stream *decomp;
};

#endif
