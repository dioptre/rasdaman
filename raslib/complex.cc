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
*/

#include "raslib/complex.hh"
#include "raslib/rminit.hh"
#include "raslib/error.hh"
#include "raslib/complextype.hh"

r_Complex::r_Complex(const char* newBuffer, const r_Complex_Type* newType)
	:	r_Primitive(newBuffer, newType)
	{
	}

r_Complex::r_Complex(const r_Complex& obj)
	:	r_Primitive(obj)
	{
	}

r_Complex::~r_Complex()
	{
	}

r_Scalar*
r_Complex::clone() const
	{
	return new r_Complex(*this);
	}

const r_Complex&
r_Complex::operator=(const r_Complex& obj)
	{ 
	r_Primitive::operator=(obj); 
	return *this;
	}

r_Double
r_Complex::get_re() const throw (r_Error)
	{
	if (!get_buffer() || !valueType || !valueType->isComplexType())
		{
		RMInit::logOut << "r_Complex::get_re() value type is not a complex, not initialised or not buffered" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid);
		throw err;
		}
	return ((r_Complex_Type *)valueType)->get_re(get_buffer());
	}

r_Double
r_Complex::get_im() const throw (r_Error)
	{
	if(!get_buffer() || !valueType || !valueType->isComplexType())
		{
		RMInit::logOut << "r_Complex::get_im() value type is not a complex, not initialised or not buffered" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid);
		throw err;
		}
	return ((r_Complex_Type *)valueType)->get_im(get_buffer());
	}

void
r_Complex::set_re(r_Double re) throw (r_Error)
	{
	if (!valueType || !valueType->isComplexType())
		{
		RMInit::logOut << "r_Complex::set_re(" << re << ") value type is not a complex or not initialised" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid);
		throw err;
		}
	((r_Complex_Type *)valueType)->set_re((char*)get_buffer(), re);
	}

void
r_Complex::set_im(r_Double im) throw (r_Error)
	{
	if(!valueType || !valueType->isComplexType())
		{
		RMInit::logOut << "r_Complex::get_im() value type is not a complex or not initialised" << endl;
		r_Error err(r_Error::r_Error_TypeInvalid);
		throw err;
		}
	((r_Complex_Type *)valueType)->set_im((char*)get_buffer(), im);
	}


bool
r_Complex::isComplex() const
	{
	return true;
	}

