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
 * INCLUDE: complex.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Complex
 *
 * COMMENTS:
 * 		The class represents a complex type value. 
 *
*/

#ifndef _D_COMPLEX_
#define _D_COMPLEX_

#include <iostream>
class r_Error;
class r_Complex_Type;

#include "raslib/odmgtypes.hh"
#include "raslib/mddtypes.hh"
#include "raslib/primitive.hh"

 
//@ManMemo: Module: {\bf raslib}

/*@Doc:

 Class \Ref{r_Complex} represents a complex type value.
 
*/


class r_Complex: public r_Primitive
	{
	public:
	
	explicit
		/// constructs a scalar type value
		r_Complex(const char* newBuffer, const r_Complex_Type* newType);
		
		/// copy constructor
		r_Complex(const r_Complex& obj);

		/// destructor
		~r_Complex();

		virtual bool isComplex() const;

		/// clone operator
		virtual r_Scalar* clone() const;

		/// operator for assigning a primitive
		virtual const r_Complex& operator =(const r_Complex&);
		
		r_Double get_re() const throw(r_Error);
		r_Double get_im() const throw(r_Error);
		
		void set_re(r_Double) throw(r_Error);
		void set_im(r_Double) throw(r_Error);

	};


#endif

