#ifndef _QTVARIABLE_
#define _QTVARIABLE_

#include "qlparser/qtoperation.hh"
#include "raslib/minterval.hh"

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

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

  The class stands for a MDD variable which corresponds to an element of the
  tupel passed by the evaluate() method.

*/


class QtVariable : public QtOperation
{
public:
    /// constructor getting iterator variable name
    QtVariable( const std::string& iteratorName );

    /// constructor getting iterator variable name and load domain
    QtVariable( const std::string& iteratorName, const r_Minterval& loadDomain );

    /// destructor
    virtual ~QtVariable();

    /// test if the two nodes have an equal meaning in the query tree
    virtual bool equalMeaning( QtNode* node );

    /// creates a unique name for a common subexpression
    virtual std::string getSpelling();

    /// test if the edge to the parent node is of type mdd or atomic
    virtual QtAreaType getAreaType();

    /// optimizing load access
    void optimizeLoad( QtTrimList* trimList );

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList ) throw (ParseInfo);

    /// prints the tree
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// methods for iterator name
    inline const std::string getIteratorName() const;
    inline void setIteratorName( std::string& str );

    /// method for loadDomain
    inline const r_Minterval getLoadDomain() const;

    /// method for loadDomain
    inline void setLoadDomain( r_Minterval& loadDomain );

    /// method for oldLoadDomain
    inline void setOldLoadDomain();

    /// method for oldLoadDomain
    inline const r_Minterval getOldLoadDomain();

    /// method for LoadDomain
    inline vector<bool>* getLoadDomainFlag();

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// type checking of the subtree
    virtual const QtTypeElement& checkType( QtTypeTuple* typeTuple = NULL ) throw (ParseInfo);

private:

    /// attribute storing the iterator name
    std::string iteratorName;

    /// minterval storing the minimal domain which is needed in the upper tree
    r_Minterval loadDomain;

    /// unique backup of the loadDomain
    r_Minterval oldLoadDomain;

    /// bitvector for discrimination of trimming and projections
    vector<bool>* domainFlag;

    /// attribute storing the index of the variable's data element in the data vector
    int dataIndex;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtvariable.icc"

#endif

