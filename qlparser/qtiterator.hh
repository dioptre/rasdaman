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
#ifndef _QTITERATOR_
#define _QTITERATOR_

#include "qlparser/qtoncstream.hh"

#include <vector>

/*************************************************************
 *
 *
 * COMMENTS:
 *
 ************************************************************/

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

It is super class for every class capable of iterating over collections of MDD.
It has a list of inputs of type QtONCStream, so every instance of type
QtONCStream or of a subtype of it can serve as an input stream for a QtIterator
object. The input streams can be combined differently, as cross product of the
inputs in class QtSelectionIterator or as simple tupels in the order they occure
in the input streams in class QtOperationIterator.

*/

class QtIterator : public QtONCStream
{
  public:
    /// list of QtONCStream objects
    typedef std::vector<QtONCStream*> QtONCStreamList;

    /// default constructor
    QtIterator();

    /// constructor getting a pointer to the parent
    QtIterator( QtNode* node );

    /// destructor
    virtual ~QtIterator();

    /// return childs of the node
    virtual QtNodeList* getChilds( QtChildType flag );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    //@Man: Operations of the ONC protocol
    //@{
    ///
    virtual void open();
    ///
    virtual void close();
    ///
    virtual void reset();
    //@}

    //@Man: Read/Write methods:
    //@{
    ///
    inline void setStreamInputs( QtONCStreamList* inputs );

	/// Returns inputs
	QtONCStreamList* getStreamInputs();
    /// exchange the stream input {\tt oldInput} with {\tt newInput}
    void setStreamInput( QtONCStream* oldInput, QtONCStream* newInput );
    ///
    //inline virtual void setParents();
    //@}

    /// pre optimization
    //virtual void preOptimize();
    /**
       Pre-Optimization step is passed to the input streams.
    */

    /// get input type tuple
    void getInputTypeTuple( QtTypeTuple& typeTuple );

  protected:
    /// list of input streams
    QtONCStreamList* inputs;

  private:
	/// atribute for identification of nodes
	static const QtNodeType nodeType;	
};

#include "qlparser/qtiterator.icc"

#endif

