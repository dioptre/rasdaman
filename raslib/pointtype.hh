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
 * INCLUDE: pointtype.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Point_Type
 *
 * COMMENTS:
 *
*/

#ifndef _D_POINT_TYPE_
#define _D_POINT_TYPE_

#include "raslib/type.hh"

//@ManMemo: Module: {\bf raslib}

/*@Doc:
  This class represents the point type in the ODMG conformant
  representation of the RasDaMan type system. 
*/

class r_Point_Type : public r_Type
{
public:
   /// default constructor
   r_Point_Type();

  /// copy constructor
  r_Point_Type( const r_Point_Type& oldObj );

  /// clone operation
  virtual r_Type* clone() const;

  /// retrieve id of the type.
  virtual r_Type::r_Type_Id type_id() const;

  /// converts array of cells from NT byte order to Unix byte order.
  virtual void convertToLittleEndian(char* cells, unsigned int noCells) const;

  /// converts array of cells from Unix byte order to NT byte order.
  virtual void convertToBigEndian(char* cells, unsigned int noCells) const;

  /// writes state of object to specified stream
  virtual void print_status( std::ostream& s = std::cout ) const;  

  virtual bool isPointType() const;

   /// destructor
   ~r_Point_Type();
};

//@Doc: write the status of point type to a stream
extern std::ostream &operator<<( std::ostream &str, const r_Point_Type &type );

#endif

  
   
