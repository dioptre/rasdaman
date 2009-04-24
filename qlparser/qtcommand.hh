#ifndef __QTCOMMAND_HH__
#define __QTCOMMAND_HH___

#include "qlparser/qtexecute.hh"

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif
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

/**

*/

class QtCommand : public QtExecute
{
  public:
    enum QtCommandType
    {
      QT_DROP_COLLECTION,
      QT_CREATE_COLLECTION
    };

    /// constructor getting command, collection and type name (create collection)
    QtCommand( QtCommandType initCommand, const std::string& initCollection, const std::string& initType );

    /// constructor getting command and collection name (drop collection)
    QtCommand( QtCommandType initCommand, const std::string& initCollection );

    /// method for evaluating the node
    virtual int evaluate();

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// type checking
    virtual void checkType();

  private:
    /// command type
    QtCommandType command;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;

    /// collection name for drop/create collection
    std::string collectionName;

    /// type name for create collection
    std::string typeName;
};

#include "qlparser/qtcommand.icc"

#endif



