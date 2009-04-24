#ifndef _QTDOMAINOPERATION_
#define _QTDOMAINOPERATION_

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

#include "qlparser/qtunaryoperation.hh"
#include "qlparser/qtmintervaldata.hh"
#include "raslib/sinterval.hh"
#include "raslib/minterval.hh"

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

The class represents a spatial domain operation, e.g. trimming or projections.

*/

class QtDomainOperation : public QtUnaryOperation
{
  public:

    /// constructor getting an minterval expression
    QtDomainOperation( QtOperation* mintOp );

    /// constructor 
    QtDomainOperation( r_Minterval domainNew, const vector<bool>* newTrimFlags );

    /// destructor
    virtual ~QtDomainOperation();

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

    /// evaluates the node
    QtData* evaluate( QtDataList* inputList );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    //@Man: Read/Write methods:
    //@{
    ///

      ///
      inline virtual void setInput( QtOperation* newInput );
      ///
      inline virtual void setMintervalOp(QtOperation* miop); 
      ///
      inline QtOperation*  getMintervalOp();

    ///
    //@}

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// type checking of the subtree
    virtual const QtTypeElement& checkType( QtTypeTuple* typeTuple = NULL );

  private:
    /// pointer to an minterval expression
    QtOperation* mintervalOp;

    /// the flag determines if the minterval expression has to be calculated at runtime or not
    bool dynamicMintervalExpression;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtdomainoperation.icc"

#endif

