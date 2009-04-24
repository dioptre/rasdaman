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
 * INCLUDE: jpeg.hh
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_JPEG
 *
 * COMMENTS:
 *
 * Provides interface to convert data to JPEG and back.
 *
*/

#ifndef _R_CONV_JPEG_HH_
#define _R_CONV_JPEG_HH_

#include "conversion/convertor.hh"


//@ManMemo: Module {\bf conversion}

/*@Doc:
  JPEG convertor class.

  Supported parameters are

  \begin{tabular}{lcl}
  quality && int && quality ratio between 0 and 100
  \end{tabular}

 */

class r_Conv_JPEG : public r_Convert_Memory
{
  public:
  /// constructor using an r_Type object
  r_Conv_JPEG( const char *src, const r_Minterval &interv, const r_Type *tp ) throw(r_Error);
  /// constructor using convert_type_e shortcut
  r_Conv_JPEG( const char *src, const r_Minterval &interv, int tp ) throw(r_Error);
  /// destructor
  ~r_Conv_JPEG( void );

  /// convert to JPEG
  virtual r_convDesc &convertTo( const char *options=NULL ) throw(r_Error);
  /// convert from JPEG
  virtual r_convDesc &convertFrom( const char *options=NULL ) throw(r_Error);
  /// cloning
  virtual r_Convertor *clone( void ) const;
  /// identification
  virtual const char *get_name( void ) const;
  virtual r_Data_Format get_data_format( void ) const;


  private:
  /// init JPEG class
  void initJPEG( void );

  /// parameters
  int quality;
};

#endif
