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
// -*-C++-*- (for Emacs)

/*************************************************************
 *
 *
 * PURPOSE:
 *   The BaseType class is the superclass for all classes
 *   describing the type of a cell
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _BASETYPE_HH_
#define _BASETYPE_HH_

#include <iostream>
#include "catalogmgr/ops.hh"
#include "type.hh"
#include "raslib/odmgtypes.hh"

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:
BaseType is the abstract base class for all types usable as basetypes
for an MDD. At the moment, only atomic types are supported. Later
structured types will also be supported.

Common to each basetype is the ability to get size information,to
print a cell and to provide means to carry out operations on cells of
this type. This functionality is defined as pure virtual functions
here.

{\bf Interdependencies}

Each \Ref{Tile} has a pointer to its BaseType. Pointers to BaseType
are also used in subclasses of \Ref{MDDObject}.
*/

/**
  * \ingroup Relcatalogifs
  */
class BaseType : public Type
{
public:
    virtual unsigned int getSize() const = 0;
    /*@Doc:
    returns the size of one cell of the type in chars.
    */

    virtual r_ULong* convertToCULong(const char* cell, r_ULong* value) const = 0;

    /*@Doc:
    returns value of the cell as a C #unsigned long#.
    */

    virtual char* makeFromCULong(char* cell, const r_ULong* value) const = 0;
    /*@Doc:
    returns C #unsigned long# in cell #cell#.
    */

    virtual r_Long* convertToCLong(const char* cell, r_Long* value) const = 0;
    /*@Doc:
    returns value of the cell as a C #long#.
    */

    virtual char* makeFromCLong(char* cell, const r_Long* value) const = 0;
    /*@Doc:
    returns C #long# in cell #cell#.
    */

    virtual double* convertToCDouble(const char* cell, double* value) const = 0;
    /*@Doc:
    returns value of the cell as a C #double#.
    */

    virtual char* makeFromCDouble(char* cell, const double* value) const = 0;
    /*@Doc:
    returns C #double# in cell #cell#.
    */

    virtual void printCell(ostream& stream, const char* cell) const = 0;
    /*@Doc:
    print contents of a cell to stream.
    */

    //@Man: methods for getting functions (used by \Ref{Tile})
    //@{
    /// get function object for condense operation.
    virtual CondenseOp* getCondenseOp( Ops::OpType op ) const;
    /*@Doc:
      \Ref{CondenseOp} carrying out the operation op on a cell of this
      type. The type of the result depends on the operation carried out,
      but is usually of type self. See \Ref{Ops} for details.
    */

    /// get function object for unary operation.
    virtual UnaryOp* getUnaryOp( Ops::OpType op, const BaseType* optype ) const;
    /*@Doc:
      Returns a pointer to a function object of a subclass of class
      \Ref{UnaryOp} carrying out the operation {\tt op} on a cell of
      type {\tt optype}. The result type has the type self. See
      \Ref{Ops} for details.
    */

    /// get function object for binary operation.
    virtual BinaryOp* getBinaryOp( Ops::OpType op, const BaseType* op1type, const BaseType* op2type) const;
    /*@Doc:
      Returns a pointer to a function object of a subclass of class
      \Ref{BinaryOp} carrying out the operation {\tt op} on two cells of
      type {\tt op1type} respective {\tt op2type}. The result type has
      the type self. See \Ref{Ops} for details.
    */
    //@}

    BaseType();
    /*@Doc:
    default constructor, cannot be used.
    */

    BaseType(const OId& id) throw (r_Error);

    BaseType(const BaseType& old);

    BaseType& operator=(const BaseType& old);

    virtual ~BaseType();
    /*@Doc:
    virtual destructor.
    */

    virtual int compatibleWith(const Type* aType) const;
    /*@Doc:
    returns true if my TypeEnum == aType->getType()
    */

protected:
    BaseType(const char* name);
};

#endif
