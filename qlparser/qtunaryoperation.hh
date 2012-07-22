#ifndef _QTUNARYOPERATION_
#define _QTUNARYOPERATION_

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

The class serves as superclass for all operation classes taking one
argument.

*/

class QtUnaryOperation : public QtOperation
{
public:
    /// default constructor
    QtUnaryOperation();

    /// constructor getting the node to its operand
    QtUnaryOperation( QtOperation* input );

    /// virtual destructor
    virtual ~QtUnaryOperation();

    /// creates a unique name for a common subexpression
    virtual std::string getSpelling();

    /// simplifies the tree
    virtual void simplify();

    /// return childs of the node
    virtual QtNodeList* getChilds( QtChildType flag );

    /// test if the edge to the parent node is of type mdd or atomic
    virtual QtAreaType getAreaType();

    /// tests if the two nodes have an equal meaning in the query tree
    virtual bool equalMeaning( QtNode* node );

    /// method for query rewrite
    inline virtual void setInput( QtOperation* inputOld, QtOperation* inputNew );

    /// optimizing load access
    virtual void optimizeLoad( QtTrimList* trimList );

    /**
      The method passes the given {\tt trimList} list to its input tree.
    */

    /// debugging method
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    //@Man: Read/Write methods for the operand
    //@{
    ///

    ///
    inline virtual void setInput( QtOperation* input );
    ///
    inline QtOperation* getInput();

    //@}

protected:
    /// operation operand
    QtOperation* input;
};

#include "qlparser/qtunaryoperation.icc"

#endif

