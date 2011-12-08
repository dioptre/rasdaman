#ifndef _QTBINARY2INDUCE_
#define _QTBINARY2INDUCE_

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

#include "qlparser/qtbinaryinduce.hh"

#include "catalogmgr/ops.hh"

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

class QtIs : public QtBinaryInduce
{
  public:
    /// constructor getting the two operands
    QtIs( QtOperation* input1, QtOperation* input2 );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtAnd : public QtBinaryInduce
{
  public:
    /// constructor getting the two operands
    QtAnd( QtOperation* input1, QtOperation* input2 );

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtOr : public QtBinaryInduce
{
  public:
    /// constructor getting the two operands
    QtOr( QtOperation* input1, QtOperation* input2 );

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtXor : public QtBinaryInduce
{
  public:
    /// constructor getting the two operands
    QtXor( QtOperation* input1, QtOperation* input2 );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtEqual : public QtBinaryInduce
{
  public:
    /// constructor getting the two operands
    QtEqual( QtOperation* input1, QtOperation* input2 );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtLess : public QtBinaryInduce
{
  public:
    /// constructor getting the two operands
    QtLess( QtOperation* input1, QtOperation* input2 );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// returns false saying that the operation IS NOT commutative
    virtual bool isCommutative() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtLessEqual : public QtBinaryInduce
{
  public:
    /// constructor getting the two operands
    QtLessEqual( QtOperation* input1, QtOperation* input2 );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// returns false saying that the operation IS NOT commutative
    virtual bool isCommutative() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtNotEqual : public QtBinaryInduce
{
  public:
    /// constructor getting the two operands
    QtNotEqual( QtOperation* input1, QtOperation* input2 );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

 private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtOverlay : public QtBinaryInduce
{
  public:
    /// constructor getting the two operands
    QtOverlay( QtOperation* input1, QtOperation* input2 );

    /// check commutativity
    bool isCommutative() const;

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


class QtBit : public QtBinaryInduce {
public:
	/// constructor getting the two operands
    QtBit( QtOperation* input1, QtOperation* input2 );
        
    /// check commutativity
    bool isCommutative() const;

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// checkType
    const QtTypeElement& checkType( QtTypeTuple* typeTuple );

private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


#include "qlparser/qtbinaryinduce2.icc"

#endif

