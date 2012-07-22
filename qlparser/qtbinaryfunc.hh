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
#ifndef _QTBINARYFUNC_
#define _QTBINARYFUNC_

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

#include "qlparser/qtbinaryoperation.hh"
#include "raslib/sinterval.hh"
#include "raslib/minterval.hh"
#include <vector>
/*************************************************************
 *
 *
 * COMMENTS:
 * - why is this "binary"? they all have just one MDD!
 *
 ************************************************************/

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class represents a shift operation on MDD objects.

*/

class QtShift : public QtBinaryOperation
{
public:
    /// constructor getting the two operands
    QtShift( QtOperation* mddOp, QtOperation* pointOp );

    /// returns FALSE saying that the operation IS NOT commutative
    virtual bool isCommutative() const;

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );
    /**
     */

    /// prints the tree
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// optimizing load access
    virtual void optimizeLoad( QtTrimList* trimList );

    /// type checking of the subtree
    virtual const QtTypeElement& checkType( QtTypeTuple* typeTuple = NULL );

private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};



//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class represents an extend operation on MDD objects.

*/

class QtExtend : public QtBinaryOperation
{
public:
    /// constructor getting the two operands
    QtExtend( QtOperation* mddOp, QtOperation* mintervalOp );
    /**
    FIXME: creates one single output tile, but should have a result tiling
     */

    /// returns FALSE saying that the operation IS NOT commutative
    virtual bool isCommutative() const;

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );
    /**
     */

    /// prints the tree
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// optimizing load access
    virtual void optimizeLoad( QtTrimList* trimList );

    /// type checking of the subtree
    virtual const QtTypeElement& checkType( QtTypeTuple* typeTuple = NULL );

private:
    /**
    aux function for QtExtend::evaluate(): build up (recursing the dimension) a list of all spatial domains that sit in the corners between outerDomain and innerDomain; at the recursion bottom the resulting domain is added to the cornerList.
    Preconditions:
    - outerDomain, innerDomain defined values, of dimension maxDim
    - currentDimension <= maxDim
    - currentInterval has coordinates (no "*") set up to dimension currentDimension
    @params outerDomain extended domain
    @params innerDomain inner domain
    @params currentDim current dimension to be inspected (0..maxDim)
    @params maxDim maximum dimension to be inspected (#dimensions-1, suitable for indexing an r_Point or r_Minterval)
    @params currentInterval current interval which is being built by iterating over its dimensions
    @params cornerList list of corner intervals that is to be built; has to be deallocated by caller.
    **/
    void extendGetCornerTiles( r_Minterval outerDomain, r_Minterval innerDomain, r_Dimension currentDim, r_Dimension maxDim, r_Minterval currentInterval, vector<r_Minterval>* cornerList );

    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};



//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class represents a scale operation on MDD objects.

*/

class QtScale : public QtBinaryOperation
{
public:
    /// constructor getting the two operands
    QtScale( QtOperation* mddOp, QtOperation* pointOp );

    /// returns FALSE saying that the operation IS NOT commutative
    virtual bool isCommutative() const;

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );
    /**
     */

    /// prints the tree
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// optimizing load access
    virtual void optimizeLoad( QtTrimList* trimList );

    /// type checking of the subtree
    virtual const QtTypeElement& checkType( QtTypeTuple* typeTuple = NULL );

    /// scale domains - initial version
    virtual int scaleDomain( const r_Minterval& areaOp, const r_Point& origin1, const r_Point& origin2, const vector<double>& scaleFactors, r_Minterval &areaScaled );

    /// scale domains - the used version
    virtual int scaleDomain( const r_Minterval& areaOp, const vector<double>& scaleFactors, r_Minterval &areaScaled );

private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtbinaryfunc.icc"

#endif

