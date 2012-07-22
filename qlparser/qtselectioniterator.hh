#ifndef _QTSELECTIONITERATOR_
#define _QTSELECTIONITERATOR_

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

An condition expression specified through a reference to
an instance of the QtOperation sub-hierarchy, is evaluated for the actual input
The boolean result determines if the tupel is passed to the output stream or not.
So the method {\tt next()} reads tupels from its inputs until one can be
passed to its caller. The class represents the WHERE condition in the query

*/

class QtSelectionIterator : public QtIterator
{
public:
    /// default constructor
    QtSelectionIterator();

    /// constructor getting a pointer to the parent
    QtSelectionIterator( QtNode* node );

    /// virtual destructor
    virtual ~QtSelectionIterator();

    /// return childs of the node
    virtual QtNodeList* getChilds( QtChildType flag );

    /// method for query rewrite
    inline virtual void setInput( QtOperation* child, QtOperation* input);

    /// optimizing load access
//    virtual void preOptimize();
    /**
      Trigger load optimization in the condition tree and pass the optimization step to
      the input streams.
    */

    /// prints the tree
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    //@Man: Operations of the ONC protocol:
    //@{
    ///
    QtDataList* next();
    //@}

    //@Man: Read/Write methods:
    //@{
    ///
    //inline virtual void setParents();
    ///
    inline void setConditionTree( QtOperation* operation );
    ///
    inline QtOperation* getConditionTree();
    //@}

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// type checking
    virtual const QtTypeTuple& checkType();

private:
    /// pointer to a tree representing the where part of the query
    QtOperation* conditionTree;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtselectioniterator.icc"

#endif


