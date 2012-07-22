#ifndef __QTDELETE_HH__
#define __QTDELETE_HH___

#include "qlparser/qtexecute.hh"
#include "qlparser/qtoncstream.hh"

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

*/

class QtDelete : public QtExecute
{
public:
    /// default constructor
    QtDelete();

    /// virtual destructor
    virtual ~QtDelete();

    /// method for evaluating the node
    virtual int evaluate();

    /// return childs of the node
    virtual QtNodeList* getChilds( QtChildType flag );

    /// prints the tree
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    //@Man: Read/Write methods:
    //@{
    ///
    ///
    void setStreamInput( QtONCStream* newInput );
    ///
    /// returns input
    QtONCStream* getStreamInput();
    //@}

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// pre optimization
//    virtual void preOptimize();
    /**
       Pre-Optimization step is passed to the input streams.
    */

    /// type checking
    virtual void checkType();

private:
    /// one input stream
    QtONCStream* input;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtdelete.icc"

#endif



