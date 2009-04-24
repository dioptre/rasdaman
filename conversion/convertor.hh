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
 * INCLUDE: convertor.hh
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Convertor, r_Convert_Memory
 *
  * COMMENTS:
 *
 * Provides interface to convert data to other formats.
 *
*/

#ifndef _R_CONVERTOR_
#define _R_CONVERTOR_

#include <stdlib.h>

#include "raslib/error.hh"
#include "raslib/minterval.hh"
#include "raslib/type.hh"
#include "raslib/mddtypes.hh"
#include "raslib/storageman.hh"
//#include "conversion/memfs.h"



// Declare to avoid including memfs.h (and with that tiffio.h)
struct memFSContext;

class r_Parse_Params;


typedef struct r_convDesc {
  const char *src;		// pointer to source data
  char *dest;			// pointer to destination data
  r_Minterval srcInterv;	// dimensions of source data
  r_Minterval destInterv;       // dimensions of destination data
  int baseType;			// shortcut to src basetype
  const r_Type *srcType;	// basetypes of src data
  r_Type *destType;             // basetypes of dest data
} r_convDesc;



//@ManMemo: Module {\bf conversion}


/*@Doc:
  Conversion classes from and to data exchange formats. Can also be used for
  tile compression of special MDD types (= images)

  \begin{itemize}
  \item
  the member function convertTo() performs the conversion MDD -> DEF
  \item
  the member function convertFrom() performs the conversion DEF -> MDD
  \item
  the r_convDesc reference returned from this call is only valid while
  the convertor object is.
  \item
  after successful execution the returned r_convDesc structure contains
  the following information:
    \begin{itemize}
    \item
    dest: pointer to converted data, allocated by the configured heap
    storage object which will use malloc() by default (see
    set_storage_handler()); must be deallocated by the caller.
    \item
    destInterv: r_Minterval describing the converted object's domain.
    \item
    destType: pointer to an r_Type object describing the converted
    object's type; must be deallocated by the caller.
    \end{itemize}
  \item
  on failure an exception is thrown.
  \end{itemize}

  The member function convertTo() receives a parameter string as argument
  which is NULL for default parameters. The format of the string is such
  that it can be parsed by r_Parse_Params. The params member variable is
  initialized to NULL in this class. Derived classes that wish to add
  parameters must first check whether params is NULL and create a new
  object if so, then add their parameters. This makes it possible to
  accumulate parameters over all class hierarchies.
*/

class r_Convertor
{
  public:
  /// default constructor (should not be used)
  r_Convertor( void );
  /// constructor using an r_Type object
  r_Convertor( const char *src, const r_Minterval &interv, const r_Type *tp,
	       bool fullTypes=false) throw(r_Error);
  /// constructor using convert_type_e shortcut
  r_Convertor( const char *src, const r_Minterval &interv, int type ) throw(r_Error);
  /**
     convert_type_e is an enumerator that acts as a shortcut to base types
     relevant for DEFs. The values and what they correspond to are listed
     below (the types below the line are for HDF):

     \begin{tabular}{ll}
     ctype_void && No type, used for errors\\
     ctype_bool && bool\\
     ctype_char && char\\
     ctype_rgb && struct {char, char, char}\\
     \hline
     ctype_int8 && signed char\\
     ctype_uint8 && unsigned char\\
     ctype_int16 && short\\
     ctype_uint16 && unsigned short\\
     ctype_int32 && int\\
     ctype_uint32 && unsigned int\\
     ctype_int64 && (unsupported)\\
     ctype_uint64 && (unsupported)\\
     ctype_float32 && float\\
     ctype_float64 && double\\
     \end{tabular}
  */

  /// destructor
  virtual ~r_Convertor( void );

  //@Man: Interface
  /// convert array to DEF
  virtual r_convDesc &convertTo( const char *options=NULL ) throw(r_Error) = 0;

  /// convert DEF to array
  virtual r_convDesc &convertFrom( const char *options=NULL ) throw(r_Error) = 0;

  /// cloning
  virtual r_Convertor *clone( void ) const = 0;

  /// identification
  virtual const char *get_name( void ) const = 0;
  virtual r_Data_Format get_data_format( void ) const = 0;

  /// set storage handler, default is malloc/free
  void set_storage_handler( const r_Storage_Man &newStore );
  
  /// get storage handler, default is malloc/free
  const r_Storage_Man& get_storage_handler( ) const;


  /// base type shortcuts
  enum convert_type_e {
    // undefined type 
    ctype_void,
    // Shortcut for the three important base types r_Boolean, r_Char and RGBPixel
    ctype_bool,
    ctype_char,
    ctype_rgb,
    // More generic types for HDF
    ctype_int8,
    ctype_uint8,
    ctype_int16,
    ctype_uint16,
    ctype_int32,
    ctype_uint32,
    ctype_int64,
    ctype_uint64,
    ctype_float32,
    ctype_float64
  };
  
  //@{ helper structure for encoding string-to-int parameters
  typedef struct convert_string_s {
    const char *key;
    int id;
  } convert_string_t;
  //@}

  /// get a r_Type from an internal type
  static r_Type *get_external_type( int ctype ) throw(r_Error);

  /// get a internal type from a r_Type
  static convert_type_e get_internal_type( const r_Type* type, bool fullTypes=false ) throw(r_Error);  

  protected:
  /// initialize internal structures
  void initShare( const char *src, const r_Minterval &interv );
  
  /// true if we should free the src area (in case the input was converted to rgb)
  bool destroySrc;

  /// convert unsupported type to rgb by applying the default color scheme
  template <class baseType>
  void applyColorScheme();

  /// conversion context
  r_convDesc desc;

  /// parameter parser
  r_Parse_Params *params;

  /// storage manager
  r_Storage_Man mystore;
};

///ostream operator for convert_type_e
std::ostream& operator<<(std::ostream& os, r_Convertor::convert_type_e& cte);

/*@Doc:
  Abstract base class for all memory-to-memory conversion classes,
  uses memfs for of data with unknown size at the time of creation.
*/

class r_Convert_Memory : public r_Convertor
{
  public:
  /// constructor using an r_Type object
  r_Convert_Memory( const char *src, const r_Minterval &interv, const r_Type *tp,
		    int fullTypes=0) throw(r_Error);
  /// constructur using convert_type_e shortcut
  r_Convert_Memory( const char *src, const r_Minterval &interv, int type ) throw(r_Error);
  /// destructor
  virtual ~r_Convert_Memory( void );


  protected:
  /// init memfs
  void initMemory( void ) throw(r_Error);

  /// variables
  memFSContext *memFS;
  void *handle;
};

#endif
