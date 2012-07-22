#ifndef _QTOPERATION_
#define _QTOPERATION_

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

#include "qlparser/qtnode.hh"

class QtData;  // forward declaration of a subclass of QtOperation

#include <iostream>

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

QtOperation defines a method {\tt evaluate()} getting a tupel of {\tt QtData} and returning
a {\tt QtData} element. Every subclass has to redefine this method to compute its specific
operation. Therefore, it takes its inputs which are also of type {\tt QtOperation}, and
invokes the {\tt evaluate()} method again. The results are used as operands and the
computed value is returned.

*/

class QtOperation : public QtNode
{
public:
    /// default constructor
    QtOperation();

    /// constructor getting a pointer to the parent
    QtOperation( QtNode* parent );

    ///for associative law
    virtual QtOperation* getUniqueOrder( const QtNode::QtNodeType ID );

    /**
      The method gives back a node that has the same QtNodeType and has the lowest
      Spelling of all nodes in the subtree with the same QtNodeType.
    */

    /// method for evaluating the node
    virtual QtData* evaluate( QtDataList* inputList );

    /**
      The method takes the {\tt inputList} to compute the result of the node which is returned in the end.
      The semantics is that elements of the {\tt inputList} are not allowed to be used as a result because
      the {\tt inputList} is freed by the caller. If this is needed, they have to be copied.
    */

    /// optimizing load access
    virtual void optimizeLoad( QtTrimList* trimList );

    /// type checking of the subtree
    virtual const QtTypeElement& checkType( QtTypeTuple* typeTuple = NULL );
    /**
      The method triggers type checking of the node's subtree. If an error occurs, an exception
      is raised.
    */

    //@Man: Read/Write methods
    //@{
    ///
    ///
    inline void                 setDataStreamType( const QtTypeElement& type );
    ///
    inline const QtTypeElement& getDataStreamType() const;
    ///
    //@}

    void printTree( int tab, std::ostream& s, QtChildType mode=QT_ALL_NODES );

    void printAlgebraicExpression( std::ostream& s );

protected:
    /// result type of the node
    QtTypeElement dataStreamType;

private:
    /// atribute for indetification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtoperation.icc"

#endif


