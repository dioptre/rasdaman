#ifndef _QTCONST_
#define _QTCONST_

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

#include "qlparser/qtdata.hh"
#include "qlparser/qtoperation.hh"
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

 The class serves as a carrier for a {\tt QtData} object
 and, additionally, provides functionality necessary in 
 the query tree.

*/

class QtConst : public QtOperation
{
  public:
    /// constructor getting a pointer to the data object 
    QtConst( QtData* newDataObj );
    /**
      Lifetime of {\tt newDataObj} is managed by this class from now on.
    */
    
    /// virtual destructor
    virtual ~QtConst();

    /// test if the two nodes have an equal meaning in the query tree
    virtual bool equalMeaning( QtNode* node );

    /// creates a unique name for a common subexpression
    virtual std::string getSpelling();

    /// test if the edge to the parent node is of type mdd or atomic
    virtual QtAreaType getAreaType();

    /// optimizing load access
    virtual void optimizeLoad( QtTrimList* trimList );

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    //@Man: Operations of the ONC protocol:
    //@{
    ///
    QtDataList* next();
    //@}

    //@Man: Read/Write methods:
    //@{
    ///
      /// return the data object
      inline QtData* getDataObj();
    ///
    //@}

    /// type checking of the subtree
    virtual const QtTypeElement& checkType( QtTypeTuple* typeTuple = NULL );

  private:
    /// pointer to the data object
    QtData* dataObj;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtconst.icc"

#endif

