#ifndef _QTOPERATIONITERATOR_
#define _QTOPERATIONITERATOR_

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

#include "qlparser/qtiterator.hh"
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
/*************************************************************
 *
 *
 * COMMENTS:
 *
 ************************************************************/


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

A QtOperationIterator takes the next element of its input streams an creates
a tupel out of them. The operation specified through an operation tree of type
QtOperation is evaluated for this tupel and the result is passed to its output
stream. The class stands for the SELECT part of a query. The operations in the
SELECT clause are expressed through an operation tree which is attached to this
class.

*/

class QtOperationIterator : public QtIterator
{
  public:
    /// default constructor
    QtOperationIterator();

    /// constructor getting a pointer to the parent
    QtOperationIterator( QtNode* node );

    /// virtual destructor
    virtual ~QtOperationIterator();

    /// return childs of the node
    virtual QtNodeList* getChilds( QtChildType flag );

    /// method for query rewrite
    inline virtual void setInput( QtOperation* child, QtOperation* input);

    /// optimizing load access
    //virtual void preOptimize();
    /**
      Trigger load optimization in the operation tree and pass the optimization step to 
      the input streams.
    */

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    //@Man: Read/Write methods:
    //@{
    ///
    //inline virtual void setParents();
    ///
    inline void setOperationTree( QtOperation* operation );
    ///
    inline QtNode::QtOperationList* getOperationTreeList();
    //@}

    //@Man: Operations of the ONC protocol:
    //@{
    ///
    QtDataList* next();
    //@}

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// type checking
    virtual const QtTypeTuple& checkType();

  private:
    /// operation trees
    QtOperationList* operationTreeList;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtoperationiterator.icc"

#endif

