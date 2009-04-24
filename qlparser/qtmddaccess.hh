#ifndef _QTMDDACCESS_
#define _QTMDDACCESS_

#include "qlparser/qtoncstream.hh"

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

// forward declarations
class MDDColl;
class MDDCollIter;

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

This class encapsulates a MDDCollection. It gives access to the elements
through the ONC protocol. The list returned by next() consists of just one
element.

*/

class QtMDDAccess : public QtONCStream
{
  public:
    /// constructor getting the collection name if no iterator name is specified
    QtMDDAccess( const std::string& collectionName );

    /// constructor getting collection and iterator name
    QtMDDAccess( const std::string& collectionName, const std::string& iteratorName );

    /// destructor
    virtual ~QtMDDAccess();

    //@Man: Operations of the ONC protocol
    //@{
    ///
    void        open();
    ///
    QtDataList* next();
    ///
    void        close();
    ///
    void        reset();
    //@}

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// method for retrieving the current MDD collection
    inline MDDColl* getMDDColl();

    /// type checking
    virtual const QtTypeTuple& checkType();

  private:
    /// name of the MDD collection
    std::string collectionName;

    /// name of the iterator
    std::string iteratorName;

    /// pointer to the MDD collection
    MDDColl* mddColl;

    /// pointer to the curent iterator
    MDDCollIter* mddIter;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtmddaccess.icc"

#endif

