#ifndef _QTJOINITERATOR_
#define _QTJOINITERATOR_

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

An object of this class takes its inputs and computes a tupel for every element
of the cross product. The result is passed to the output stream.
The class represents the FROM part of a query.

*/

class QtJoinIterator : public QtIterator
{
public:
    /// default constructor
    QtJoinIterator();

    /// constructor getting a pointer to the parent
    QtJoinIterator( QtNode* node );

    /// virtual destructor
    virtual ~QtJoinIterator();

    /// prints the tree
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    //@Man: Operations of the ONC protocol:
    //@{
    ///
    void open();
    ///
    QtDataList* next();
    /**
      According to the producer/consumer protocol of ONC streams, the {/tt next()}
      method has to store the last tupel of the crossproduct and update it with a new
      element everytime it is invoked. Than it makes a copy of the tupel and pass
      it to the caller.
    */
    ///
    void close();
    ///
    void reset();
    ///
    //@}

    //@Man: Read/Write methods:
    //@{
    ///
    //inline virtual void setParents();
    //@}

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// type checking
    virtual const QtTypeTuple& checkType();

private:
    /// inidicates if the output stream is empty or not
    bool outputStreamIsEmpty;
    /** The output stream is empty if at least one of the input streams is empty.
        In that case, the cross product has no elements. This is indicated
    by the variable {\tt outputStreamIsEmpty} which is set in the method
    {\tt open()}.
    */

    /// pointer to a copy of the last passed tupel
    QtDataList* actualTupel;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtjoiniterator.icc"

#endif

