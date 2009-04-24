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
 * SOURCE: convertor.cc
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Convertor_Factory
 *
 * COMMENTS:
 * 		Create convertors out of data formats
*/

#include "raslib/mddtypes.hh"
#include "raslib/error.hh"


class r_Convertor;
class r_Minterval;
class r_Type;


//@ManMemo: Module {\bf conversion}

/*@Doc:
  r_Convertor_Factory is a class encapsulating static functions to
  create convertors out of data formats. The alternative of making
  this functionality static members of r_Converstor would have meant
  that all code using just one convertor would have to link the libs
  of all convertors, rendering it unusable.
*/

class r_Convertor_Factory
{
  public:
  /// check for support
  static bool is_supported( r_Data_Format fmt );
  /// creating from r_Type
  static r_Convertor *create( r_Data_Format fmt, const char *src, const r_Minterval &interv,
			      const r_Type *tp ) throw(r_Error);
  /// creating from internal type
  static r_Convertor *create( r_Data_Format fmt, const char *src, const r_Minterval &interv,
			      int type ) throw(r_Error);
};
