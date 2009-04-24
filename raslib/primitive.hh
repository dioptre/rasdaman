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
 * INCLUDE: primitive.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Primitive
 *
 * COMMENTS:
 * 		The class represents a primitive type value.
 *
*/

#ifndef _D_PRIMITIVE_
#define _D_PRIMITIVE_

#include <iostream>

class r_Error;

#include "raslib/scalar.hh"
#include "raslib/odmgtypes.hh"

class r_Primitive_Type;
 
//@ManMemo: Module: {\bf raslib}

/*@Doc:

 Class \Ref{r_Primitive} represents a primitive type value.
 
*/

class r_Primitive : public r_Scalar
{
  public:

  explicit
  
    /// constructs a scalar type value
    r_Primitive( const char* newBuffer, const r_Primitive_Type* newType );
 
    /// copy constructor
    r_Primitive( const r_Primitive& obj );

    /// destructor
    ~r_Primitive();

    /// clone operator
    virtual r_Scalar* clone() const;

    /// operator for assigning a primitive
    virtual const r_Primitive& operator= ( const r_Primitive& );

    /// gets the pointer to the buffer
    const char* get_buffer() const;

    /// debug output
    virtual void print_status(std::ostream& s) const;

    virtual bool isPrimitive() const;

    //@Man: Type-safe value access methods. In case of type mismatch, an exception is raised.
    //@{
    ///

      /// 
      r_Boolean get_boolean() const throw( r_Error );
      /// 
      r_Char    get_char()    const throw( r_Error );
      ///  
      r_Octet   get_octet()   const throw( r_Error );
      /// 
      r_Short   get_short()   const throw( r_Error );
      /// 
      r_UShort  get_ushort()  const throw( r_Error );
      /// 
      r_Long    get_long()    const throw( r_Error );
      /// 
      r_ULong   get_ulong()   const throw( r_Error );
      /// 
      r_Float   get_float()   const throw( r_Error );
      /// 
      r_Double  get_double()  const throw( r_Error );
      
       /// 
      void set_boolean(r_Boolean) throw( r_Error );
      /// 
      void set_char(r_Char)       throw( r_Error );
      ///  
      void set_octet(r_Octet)     throw( r_Error );
      /// 
      void set_short(r_Short)     throw( r_Error );
      /// 
      void set_ushort(r_UShort)   throw( r_Error );
      /// 
      void set_long(r_Long)       throw( r_Error );
      /// 
      void set_ulong(r_ULong)     throw( r_Error );
      /// 
      void set_float(r_Float)     throw( r_Error );
      /// 
      void set_double(r_Double)   throw( r_Error );
     

    ///
    //@}

  private:
    /// buffer
    char* valueBuffer;
};



//@ManMemo: Module: {\bf raslib}
/**
  Output stream operator for objects of type {\tt const} \Ref{r_Primitive}.
*/  
extern std::ostream& operator<<( std::ostream& s, const r_Primitive& obj );

#endif

