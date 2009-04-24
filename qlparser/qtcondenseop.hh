#ifndef _QTCONDENSEOP_
#define _QTCONDENSEOP_

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

#include "qlparser/qtbinaryoperation.hh"

#include "catalogmgr/ops.hh"
#include <stdio.h>

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
/*************************************************************
 *
 *
 * COMMENTS:
 *
 ************************************************************/

//@ManMemo: Module: {\bf qlparser}

/*@Doc:
 
  The class represents the root of a cond expression.

*/

class QtCondenseOp : public QtBinaryOperation
{
  public:
    /// constructor getting iterator, minterval exp, cell exp, and cell condition exp (optional)
    QtCondenseOp( Ops::OpType   newOperation,
                  const std::string& initIteratorName, 
                  QtOperation*  mintervalExp, 
                  QtOperation*  cellExp,
                  QtOperation*  condExp = NULL     );

    /// virtual destructor
    ~QtCondenseOp();

    /// return childs of the node
    virtual QtNodeList* getChilds( QtChildType flag );

    /// test if the two nodes have an equal meaning in a subtree
    virtual bool equalMeaning( QtNode* node );

    /// creates a unique name for a common subexpression
    virtual std::string getSpelling();

    /// method for query rewrite
    virtual void setInput( QtOperation* inputOld, QtOperation* inputNew );

    /// optimizing load access
    void optimizeLoad( QtTrimList* trimList );

    /// simplifies the tree
    virtual void simplify();

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

    //@Man: Read/Write methods:
    //@{
    ///

      /// 
      inline QtOperation* getCondOp();

    ///
    //@}

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// type checking of the subtree
    virtual const QtTypeElement& checkType( QtTypeTuple* typeTuple = NULL );

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;

    /// attribute storing the iterator name
    std::string iteratorName;

    /// attribute storing optional cell condition expression
    QtOperation* condOp;

    /// attribute storing condensing operation
    Ops::OpType operation;
};


#include "qlparser/qtcondenseop.icc"

#endif

