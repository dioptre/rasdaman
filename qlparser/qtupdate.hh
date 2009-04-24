#ifndef __QTUPDATE_HH__
#define __QTUPDATE_HH___

#include "qlparser/qtexecute.hh"
#include "qlparser/qtoncstream.hh"
#include "qlparser/qtoperation.hh"

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

class QtUpdate : public QtExecute
{
  public:
    /// constructor getting target, domain, and source expressions of the update
    QtUpdate( QtOperation* initUpdateTarget, QtOperation* initUpdateDomain, QtOperation* initUpdateSource );

    /// virtual destructor
    virtual ~QtUpdate();

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
 	/// returns updateTarget
	QtOperation* getUpdateTarget();
	/// returns updateDomain
	QtOperation* getUpdateDomain();
	///returns updateSource
	QtOperation* getUpdateSource();
	///returns input
	QtONCStream* getInput();
   	//@}

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// method for query rewrite
    inline virtual void setInput( QtOperation* child, QtOperation* input);

    /// pre optimization
    //virtual void preOptimize();
    /**
       Pre-Optimization step is passed to the input streams.
    */

    /// type checking
    virtual void checkType();

  private:
    /// one input stream
    QtONCStream* input;

    /// target expression
    QtOperation* updateTarget;

    /// target domain expression
    QtOperation* updateDomain;

    /// target expression
    QtOperation* updateSource;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtupdate.icc"

#endif



