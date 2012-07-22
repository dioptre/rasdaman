#ifndef _QTNARYOPERATION_
#define _QTNARYOPERATION_

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

#include <stdio.h>

#include "qlparser/qtoperation.hh"

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
/**************************************************************
 *
 *
 * COMMENTS:
 *
 ************************************************************/

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class serves as superclass for all operation classes taking n
  (more than two) arguments.

*/

class QtNaryOperation : public QtOperation
{
public:
    /// default constructor
    QtNaryOperation();

    /// constructor getting the node to the parent
    QtNaryOperation( QtNode* node );

    /// constructor getting a list of operands
    QtNaryOperation( QtOperationList* opList );

    /// virtual destructor
    virtual ~QtNaryOperation();

    /// simplifies the tree
    virtual void simplify();

    /// test if the two nodes have an equal meaning in the query tree
    virtual bool equalMeaning( QtNode* node );
    /**
      The meaning of a binary operation is equal, iff all operands have
      the same meaning.
    */

    /// return childs of the node
    virtual QtNodeList* getChilds( QtChildType flag );

    /// creates a unique name for a subexpression
    virtual std::string getSpelling();

    /// method for query rewrite
    virtual void setInput( QtOperation*, QtOperation* inputNew );

    /// tests if the edge to the parent node is of type mdd or atomic
    virtual QtAreaType getAreaType();

    /// optimizing load access
    virtual void optimizeLoad( QtTrimList* trimList );
    /**
      The method deletes the given {\tt trimList} and passes the {\tt optimizeLoad}
      message with empty triming lists to its input trees.
    */

    /// debugging method
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    //@Man: read/write methods for the operands
    //@{
    ///
    ///
    inline void                     setInputs( QtOperationList* opList );
    ///
    inline QtNode::QtOperationList* getInputs();
    ///
    //@}

protected:
    /// method for testing and evaluating the input branches
    bool getOperands( QtDataList* inputList, QtDataList* &operandList );
    /**
      The method checks if the input branches are valid. Then it passes the evaluate message to its two
      operands with the {\tt inputList} as argument. The returned results are provided through the arguments
      {\tt operand1} and {\tt operand2} called by reference. The method returns {\tt true} if the operands are
      valid, otherwise {\tt false}.
    */

    /// operation trees
    QtOperationList* operationList;

private:
    /// atribute for identification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtnaryoperation.icc"

#endif


