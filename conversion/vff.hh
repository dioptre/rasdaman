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
 * FILE: vff.hh
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_VFF
 *
 * COMMENTS:
 *
 * Provides interface to convert data to VFF and back.
 *
*/

#ifndef _R_CONV_VFF_HH_
#define _R_CONV_VFF_HH_

#include "conversion/convertor.hh"



//@ManMemo: Module {\bf conversion}

/*@Doc:
  VFF convertor class.

  Supported parameters are

  \begin{tabular}{rcl}
  dorder && string && data order to read/write in 3D mode; permutations of "xyz"\\
  dimorder && string && dimension order for vectors (size, origin, ...)\\
  vffendian && int && default endianness, 0 for big endian, 1 for little\\
  \end{tabular}

*/

class r_Conv_VFF : public r_Convertor
{
  public:
  /// constructor using an r_Type object
  r_Conv_VFF( const char *src, const r_Minterval &interv, const r_Type *tp ) throw(r_Error);
  /// constructor using a convert_type_e shortcut
  r_Conv_VFF( const char *src, const r_Minterval &interv, int tp ) throw(r_Error);
  /// destructor
  ~r_Conv_VFF( void );

  /// convert to VFF
  virtual r_convDesc &convertTo( const char *options=NULL ) throw(r_Error);
  /// convert from VFF
  virtual r_convDesc &convertFrom( const char *options=NULL ) throw(r_Error);
  /// cloning
  virtual r_Convertor *clone( void ) const;
  /// identification
  virtual const char *get_name( void ) const;
  virtual r_Data_Format get_data_format( void ) const;


  private:
  /// shared init code
  void initVFF( void );
  /// skip whitespace when parsing the header
  static void skip_white( const char *&str );
  /// read a floating point vector from the header and return pointer to rest
  static const char *read_vector( r_Dimension dim, const char *str, double *&vec );
  /// read a string from the header and return pointer to rest
  static const char *read_string( const char *str, char *dest, bool allowSpace=0 );
  /// write an interval to a stream
  static void write_interval( const char *keyname, std::ostream &str, const r_Minterval &iv,
			      const unsigned int *order, r_Range inc=0 );
  /// write an origin to a stream
  static void write_origin( const char *keyname, std::ostream &str, const r_Minterval &iv,
                            const unsigned int *order );
  /// get the VFF-endian id for the host machine's
  static const char *get_endian_id( void );
  /// parse data order string, revert to default if failed
  static int parse_data_order( r_Dimension dim, const char *dstr, unsigned int *order );
  /// get default data order for a dimensionality
  static const char *get_default_order( r_Dimension dim );
  /// get default dimension order for a dimensionality
  static const char *get_default_dim_order( r_Dimension dim );

  /// get dimension order in newly allocated array
  unsigned int *get_dimension_order( r_Dimension dim ) const;
  /// get the default endianness
  const char *get_default_endianness( void ) const;

  /// data order parameter
  char *dorderParam;
  /// dimension order parameter
  char *dimOrderParam;
  /// default endianness parameter
  int dfltEndianness;

  /// identifier (ncaa) in header
  static const char *fileMagic;
  /// keyword names
  static const char *keywords[];
  /// special key values
  static const char *kval_Raster;
  static const char *kval_Slice;
  static const char *kval_LEndian;
  static const char *kval_BEndian;
  /// default data order for 2D/3D
  static const char *dfltDataOrder2;
  static const char *dfltDataOrder3;
  /// default dimension order fo 2D/3D
  static const char *dfltDimOrder2;
  static const char *dfltDimOrder3;
  /// end-of-header marker
  static const char endOfHeader;
  /// name of convertTo() method
  static const char *method_convTo;
  /// name of convertFrom() method
  static const char *method_convFrom;
};

#endif
