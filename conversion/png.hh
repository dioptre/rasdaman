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
 * INCLUDE: png.hh
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_PNG
 *
 * COMMENTS:
 *
 * Provides interface to convert data to other formats.
 *
*/

#ifndef _R_CONV_PNG_HH_
#define _R_CONV_PNG_HH_

#include "conversion/convertor.hh"



//@ManMemo: Module {\bf conversion}

/*@Doc:
  PNG convertor class.

  This class doesn't have parameters.
*/

class r_Conv_PNG : public r_Convert_Memory
{
  public:
  /// constructor using an r_Type object
  r_Conv_PNG( const char *src, const r_Minterval &interv, const r_Type *tp ) throw(r_Error);
  /// constructor using convert_type_e shortcut
  r_Conv_PNG( const char *src, const r_Minterval &interv, int tp ) throw(r_Error);
  /// destructor
  ~r_Conv_PNG( void );

  /// convert to PNG
  virtual r_convDesc &convertTo( const char *options=NULL ) throw(r_Error);
  /// convert from PNG
  virtual r_convDesc &convertFrom( const char *options=NULL ) throw(r_Error);
  /// cloning
  virtual r_Convertor *clone( void ) const;
  /// identification
  virtual const char *get_name( void ) const;
  virtual r_Data_Format get_data_format( void ) const;

  private:
  /// names
  static const char *name_InfoKey;
  static const char *name_InfoText;
  static const char *method_convertTo;
  static const char *method_convertFrom;
};

#endif
