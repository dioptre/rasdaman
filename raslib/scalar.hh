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
 * INCLUDE: scalar.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Scalar
 *
 * COMMENTS:
 * 
 * The class represents a scalar type value.
 *
*/

#ifndef _D_SCALAR_
#define _D_SCALAR_

#include <iosfwd>

#include "raslib/error.hh"

class r_Base_Type;
 
//@ManMemo: Module: {\bf raslib}

/*@Doc:

 Class \Ref{r_Scalar} represents a scalar type value which 
 is either \Ref{r_Primitive} or \Ref{r_Structure}.
 
*/

class r_Scalar
{
  public:
    /// constructs a scalar value
    r_Scalar( const r_Base_Type* newType );

    /// copy constructor
    r_Scalar( const r_Scalar& obj );

    /// destructor
    virtual ~r_Scalar();

    /// clone operator
    virtual r_Scalar* clone() const=0;

    /// operator for assigning a scalar
    virtual const r_Scalar& operator= ( const r_Scalar& );

    /// debug output
    virtual void print_status(std::ostream& s) const = 0;

    /// get type
    virtual const r_Base_Type* get_type() const; 

    /// 
    virtual bool isStructure() const;

    ///
    virtual bool isComplex() const;

    ///
    virtual bool isPrimitive() const;

  protected:
    /// type
    r_Base_Type* valueType;
};



//@ManMemo: Module: {\bf raslib}
/**
  Output stream operator for objects of type {\tt const} \Ref{r_Scalar}.
*/  
extern std::ostream& operator<<(std::ostream& s, const r_Scalar& obj );

#endif

