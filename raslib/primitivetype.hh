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
 * INCLUDE: primitivetype.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Primitive_Type
 *
 * COMMENTS:
 *
*/

#ifndef _D_PRIMITIVE_TYPE_
#define _D_PRIMITIVE_TYPE_

class r_Error;

#include "raslib/basetype.hh"
#include "raslib/odmgtypes.hh"

//@ManMemo: Module: {\bf raslib}

/*@Doc:
  This class represents all primitive types in the ODMG conformant
  representation of the RasDaMan type system. Examples are ULONG or
  BOOL.
*/
 
class r_Primitive_Type : public r_Base_Type
{
public:
  /// constructor getting name of type, size of type and type id.
  r_Primitive_Type( const char* newTypeName, const r_Type::r_Type_Id newTypeId );
  /// copy constructor
  r_Primitive_Type( const r_Primitive_Type& oldObj );
  /// assignment operator.
  const r_Primitive_Type& operator=( const r_Primitive_Type& oldObj );
  /// destructor.
  virtual ~r_Primitive_Type();

  /// clone operation
  virtual r_Type* clone() const;

  /// retrieve id of the type.
  virtual r_Type::r_Type_Id type_id() const;

  /// converts array of cells from NT byte order to Unix byte order.
  virtual void convertToLittleEndian(char* cells, r_Area noCells) const;

  /// converts array of cells from Unix byte order to NT byte order.
  virtual void convertToBigEndian(char* cells, r_Area noCells) const;

  /// writes state of object to specified stream
  virtual void print_status( std::ostream& s = std::cout ) const;  
  
  /// check, if type is primitive.
  virtual bool isPrimitiveType() const;

  /// prints value of a primitive type
  virtual void print_value( const char* storage,  std::ostream& s = std::cout  ) const;

    //@Man: Type-safe value access methods. In case of type mismatch, an exception is raised.
    //@{
    ///

      /// 
      r_Double get_value( const char* cell ) const throw( r_Error );
      /// 
      void  set_value( char* cell, r_Double ) throw( r_Error );      
      ///
      void  get_limits( r_Double&,  r_Double& ) throw( r_Error );            
      

      /// 
      r_Boolean get_boolean( const char* cell ) const throw( r_Error );
      /// 
      r_Char    get_char( const char* cell )    const throw( r_Error );
      ///  
      r_Octet   get_octet( const char* cell )   const throw( r_Error );
      /// 
      r_Short   get_short( const char* cell )   const throw( r_Error );
      /// 
      r_UShort  get_ushort( const char* cell )  const throw( r_Error );
      /// 
      r_Long    get_long( const char* cell )    const throw( r_Error );
      /// 
      r_ULong   get_ulong( const char* cell )   const throw( r_Error );
      /// 
      r_Float   get_float( const char* cell )   const throw( r_Error );
      /// 
      r_Double  get_double( const char* cell )  const throw( r_Error );

      /// 
      void  set_boolean( char* cell, r_Boolean ) throw( r_Error );
      /// 
      void  set_char( char* cell, r_Char )       throw( r_Error );
      ///  
      void  set_octet( char* cell, r_Octet )     throw( r_Error );
      /// 
      void  set_short( char* cell, r_Short )     throw( r_Error );
      /// 
      void  set_ushort( char* cell, r_UShort )   throw( r_Error );
      /// 
      void  set_long( char* cell, r_Long )       throw( r_Error );
      /// 
      void  set_ulong( char* cell, r_ULong )     throw( r_Error );
      /// 
      void  set_float( char* cell, r_Float )     throw( r_Error );
      /// 
      void  set_double( char* cell, r_Double )   throw( r_Error );
            

    ///
    //@}

  protected:
  /// default constructor.
  r_Primitive_Type();

    r_Type::r_Type_Id typeId;
};

//@Doc: write the status of a primitive type to a stream
extern std::ostream &operator<<( std::ostream &str, const r_Primitive_Type &type );

#endif

