#ifndef _QTCONDENSE_
#define _QTCONDENSE_

#include "qlparser/qtunaryoperation.hh"
#include "raslib/minterval.hh"
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
/**************************************************************
 *
 *
 * COMMENTS:
 *
 ************************************************************/

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class represents a condense operation in the query tree.

*/

class QtCondense : public QtUnaryOperation
{
  public:
    /// constructor getting operation
    QtCondense( Ops::OpType newOpType);

    /// constructor getting operation and operand
    QtCondense( Ops::OpType newOpType, QtOperation* input );

    /// method for computing full condense operation (without early termination option)
    QtData* computeFullCondense( QtDataList* inputList, r_Minterval& areaOp);// = r_Minterval() );

    /// test if the edge to the parent node is of type mdd or atomic
    virtual QtAreaType getAreaType();

    /// optimizing load access
    virtual void optimizeLoad( QtTrimList* trimList );
    /**
      The method deletes the given {\tt trimList} and passes the {\tt optimizeLoad}
      message with empty triming list to its operand tree.

      Optimization process enters a new MDD area.
    */

    /// type checking of the subtree
    virtual const QtTypeElement& checkType( QtTypeTuple* typeTuple = NULL );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method returning class name
    inline virtual const char* getClassName() const;

    /// method returning algebraic identifier
    inline virtual const char* getAlgebraicName() const;

  protected:
    /// operation type
    Ops::OpType opType;

  private:
	/// attribute for identification of nodes
	static const QtNodeType nodeType;
};




//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class represents the specialized condenser {\tt some_cells()} in the query tree.

*/

class QtSome : public QtCondense
{
  public:
    /// default constructor
    QtSome();

    /// constructor getting the operand
    QtSome( QtOperation* input );

    /// optimizes the tree
//    virtual void rewriteOps();

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// method returning class name
    inline virtual const char* getClassName() const;

    /// method returning algebraic identifier
    inline virtual const char* getAlgebraicName() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};



//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class represents the specialized condenser {\tt all_cells()} in the query tree.

*/

class QtAll : public QtCondense
{
  public:
    /// default constructor
    QtAll();

    /// constructor getting the operand
    QtAll( QtOperation* input );

    /// optimizes the tree
//    virtual void rewriteOps();

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// method returning class name
    inline virtual const char* getClassName() const;

    /// method returning algebraic identifier
    inline virtual const char* getAlgebraicName() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};



//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class represents the specialized condenser {\tt count_cells()} in the query tree.

*/

class QtCountCells : public QtCondense
{
  public:
    /// default constructor
    QtCountCells();

    /// constructor getting the operand
    QtCountCells( QtOperation* input );

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// method returning class name
    inline virtual const char* getClassName() const;

    /// method returning algebraic identifier
    inline virtual const char* getAlgebraicName() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class represents the specialized condenser {\tt add_cells()} in the query tree.

*/

class QtAddCells : public QtCondense
{
  public:
    /// default constructor
    QtAddCells();

    /// constructor getting the operand
    QtAddCells( QtOperation* input );

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// method returning class name
    inline virtual const char* getClassName() const;

    /// method returning algebraic identifier
    inline virtual const char* getAlgebraicName() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class represents the specialized condenser {\tt avg_cells()} in the query tree.

*/

class QtAvgCells : public QtCondense
{
  public:
    /// default constructor
    QtAvgCells();

    /// constructor getting the operand
    QtAvgCells( QtOperation* input );

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// method returning class name
    inline virtual const char* getClassName() const;

    /// method returning algebraic identifier
    inline virtual const char* getAlgebraicName() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class represents the specialized condenser {\tt min_cells()} in the query tree.

*/

class QtMinCells : public QtCondense
{
  public:
    /// default constructor
    QtMinCells();

    /// constructor getting the operand
    QtMinCells( QtOperation* input );

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// method returning class name
    inline virtual const char* getClassName() const;

    /// method returning algebraic identifier
    inline virtual const char* getAlgebraicName() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class represents the specialized condenser {\tt max_cells()} in the query tree.

*/

class QtMaxCells : public QtCondense
{
  public:
    /// default constructor
    QtMaxCells();

    /// constructor getting the operand
    QtMaxCells( QtOperation* input );

    /// method for evaluating the node
    QtData* evaluate( QtDataList* inputList );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// method returning class name
    inline virtual const char* getClassName() const;

    /// method returning algebraic identifier
    inline virtual const char* getAlgebraicName() const;

  private:
    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};


#include "qlparser/qtcondense.icc"

#endif

