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
 * INCLUDE: attribute.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Attribute
 *
 * COMMENTS:
 *		None
*/

#ifndef _D_ATTRIBUTE_
#define _D_ATTRIBUTE_

#include "raslib/property.hh"
#include "raslib/odmgtypes.hh"
#include "raslib/mddtypes.hh"

class r_Base_Type;
class r_Type_Id;
class r_Error;


//@ManMemo: Module: {\bf raslib}

/*@Doc:
  This class represents attributes of structs in the ODMG 
  conformant representation of the RasDaMan type system.
*/
 

class r_Attribute : public r_Property
	{
	public:
		/// default constructor.
		/// initialise important attributes to NULL
		r_Attribute();

		/// constructor getting name and type of attribute.
		r_Attribute(const char* newTypeName, const r_Base_Type& newType);

		/// copy constructor.
		r_Attribute(const r_Attribute& oldObj);

		/// assignment operator.
		const r_Attribute& operator=(const r_Attribute& oldObj);

		/// destructor.
		virtual ~r_Attribute();

		/// retrieve (local) offset
		r_Bytes offset() const;

		/// set (local) offset
		void set_offset(r_Bytes newOffset);

		/// retrieve global offset
		r_Bytes global_offset() const;

		/// set global offset
		void set_global_offset(r_Bytes newOffset);

		/// writes state of object to specified stream
		virtual void print_status(std::ostream& s = std::cout) const;  

		/// subscript operator to access attributes of a structured attribute
		/// throws error when type is not a struct type
		r_Attribute operator[](unsigned int number) const throw(r_Error);

		//@Man: Type-safe value access methods. In case of type mismatch, an exception is raised.
		//@{
		/// 
		r_Boolean get_boolean(const char* cell) const throw(r_Error);

		/// 
		r_Char    get_char(const char* cell)    const throw(r_Error);

		///  
		r_Octet   get_octet(const char* cell)   const throw(r_Error);

		/// 
		r_Short   get_short(const char* cell)   const throw(r_Error);

		/// 
		r_UShort  get_ushort(const char* cell)  const throw(r_Error);

		/// 
		r_Long    get_long(const char* cell)    const throw(r_Error);

		/// 
		r_ULong   get_ulong(const char* cell)   const throw(r_Error);

		/// 
		r_Float   get_float(const char* cell)   const throw(r_Error);

		/// 
		r_Double  get_double(const char* cell)  const throw(r_Error);

		///
		//@}

	protected:
		/// local offset
		r_Bytes localOffset;

		/// global offset
		r_Bytes globalOffset;
	};

//@Doc: write the status of a attribute to a stream
extern std::ostream &operator<<( std::ostream &str, const r_Attribute &type );

#endif
