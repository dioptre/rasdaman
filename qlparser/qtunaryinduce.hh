#ifndef _QTUNARYINDUCE_HH_
#define _QTUNARYINDUCE_HH_

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

#include "qlparser/qtunaryoperation.hh"
#include "qlparser/qtmdd.hh"
#include "qlparser/qtatomicdata.hh"

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

This class has just QtNot as subclass which represents the not operations
{\tt MDD<Bool> -> MDD<Bool>} and {\tt MDD<Int>  -> MDD<Int>}.

*/

class QtUnaryInduce : public QtUnaryOperation
{
  public:
    /// constructor getting the operand
    QtUnaryInduce( QtOperation* input );

  protected:
    /// computes the unary operation
    QtData* computeOp( QtData* operand, Ops::OpType operation );

    /// method for testing and evaluating the input branch
    bool getOperand( QtDataList* inputList, QtData* &operand );
    /**
      The method checks if the input branch is valid. Then it passes the evaluate message to its
      operand with the {\tt inputList} as argument. The returned result is provided through the argument
      {\tt operand} called by reference. The method returns {\tt true} it the operand is
      valid, otherwise {\tt false}.
    */

    /// computes an unary induce operation with one MDD object
    QtData* computeUnaryMDDOp( QtMDD* operand, const BaseType* resultBaseType, Ops::OpType operation, unsigned int operandOffset = 0 );
    /**
      The method carries out the unary induce operation specified by {\tt operation} on the operand. For
      the result, a new transient MDD object is created and returned. In the end, the MDD object of the operand
      is freed.
    */

    /// computes an unary induce operation with one MDD object
    QtData* computeUnaryOp( QtScalarData* operand, const BaseType* resultBaseType, Ops::OpType operation, unsigned int operandOffset = 0 );
    /**
      The method carries out the unary operation specified by {\tt operation} on the operand.
    */

  private:
	/// attribute for identification of nodes
	static const QtNodeType nodeType;
};



//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtNot : public QtUnaryInduce
{
  public:
    /// constructor getting the operand
    QtNot( QtOperation* input );

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );

    /**
      The method invokes the evaluate method of its superclass {\tt QtUnaryInduce} with the not
      operation as an additional argument to compute the result of the node.
    */

    /// prints the tree
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// type checking of the subtree
    virtual const QtTypeElement& checkType( QtTypeTuple* typeTuple = NULL );

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};



//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtDot : public QtUnaryInduce
{
  public:
    /// constructor getting operand and element name
    QtDot( const std::string& elementName );

    /// constructor getting operand and element number
    QtDot( unsigned int elementNo );

    /// creates a unique name for a common subexpression
    virtual std::string getSpelling();

    /// tests if the two nodes have an equal meaning in the query tree
    virtual bool equalMeaning( QtNode* node );

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );
    /**
      The method invokes the evaluate method of its superclass {\tt QtUnaryInduce} with the not
      operation as an additional argument to compute the result of the node.
    */

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// type checking of the subtree
    virtual const QtTypeElement& checkType( QtTypeTuple* typeTuple = NULL );

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;

    /// attribute storing selection element name
    std::string elementName;

    /// attribute storing selection element number
    int elementNo;
    /**
      The number is either positive, which means that the number 
      is valid, or -1 saying that the element name is valid for
      selection.
    */
};


//--------------------------------------------
//  QtCast
//--------------------------------------------
//@ManMemo: Module: {\bf qlparser}

/*@Doc:
QtCast provides type conversions
*/

class QtCast : public QtUnaryInduce {
public:
    enum cast_types {
    	t_bool, t_octet, t_char, t_short, t_ushort,
        t_long, t_ulong, t_float, t_double
    };
                        
    QtCast(QtOperation*, cast_types);
    /// method for evaluating the node
    QtData* evaluate(QtDataList*);
    /// prints the tree 
    virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES);
    /// prints the algebraic expression
    virtual void printAlgebraicExpression(std::ostream& = std::cout);
    /// retrives node type
    inline virtual const QtNodeType getNodeType() const;
    /// semantics check
    virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );
                                                   
private:
	Ops::OpType getOp(cast_types);

    static const QtNodeType nodeType;                                                            
    cast_types castType;  
};
                                                                

//--------------------------------------------
//  QtRealPartOp
//--------------------------------------------

class QtRealPartOp : public QtUnaryInduce {
public:
	QtRealPartOp(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES);
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL);
                            
private:
	static const QtNodeType nodeType;
};
                                                                   
//--------------------------------------------
//  QtImaginarPartOp
//--------------------------------------------

class QtImaginarPartOp : public QtUnaryInduce {
public:
	QtImaginarPartOp(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES);
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL);
                            
private:
	static const QtNodeType nodeType;
};
                                                                   
#include "autogen_qtui.hh"	
#include "qlparser/qtunaryinduce.icc"
#include "autogen_qtui.icc"


#endif

