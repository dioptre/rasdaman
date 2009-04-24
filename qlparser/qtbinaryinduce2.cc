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
 * PURPOSE:
 *
 *
 * COMMENTS:
 *
 ************************************************************/

static const char rcsid[] = "@(#)qlparser, QtBinaryInduce: $Id: qtbinaryinduce2.cc,v 1.25 2002/09/11 14:48:51 hoefner Exp $";

#include "raslib/rmdebug.hh"

#include "qlparser/qtbinaryinduce2.hh"
#include "qlparser/qtatomicdata.hh"
#include "qlparser/qtconst.hh"

#include "mddmgr/mddobj.hh"

#include "catalogmgr/typefactory.hh"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

const QtNode::QtNodeType QtIs::nodeType = QT_IS;

QtIs::QtIs( QtOperation* initInput1, QtOperation* initInput2 )
  :  QtBinaryInduce( initInput1, initInput2, Ops::OP_EQUAL ) 
{
}

/*
void
QtIs::checkIdempotency()
{
  if( input1->getSpelling().compare( input2->getSpelling() ) == 0 )
  {
    RMInit::logOut << "> rule (idempotency): A IS A -> TRUE" << endl;

    getParent()->setInput( this, new QtConst( new QtAtomicData( 1 ) ) );

    // delete the node itself and its descendants
    delete this;
  };
}
*/

void
QtIs::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtIs Object " << getNodeType() << endl;

  QtBinaryInduce::printTree( tab, s, mode );
}


void
QtIs::printAlgebraicExpression( ostream& s )
{
  s << "(";

  if( input1 )
    input1->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << " is ";

  if( input2 )
    input2->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";
}


const QtNode::QtNodeType QtAnd::nodeType = QT_AND;

QtAnd::QtAnd( QtOperation* initInput1, QtOperation* initInput2 )
  :  QtBinaryInduce( initInput1, initInput2, Ops::OP_AND )
{
}

/*
void
QtAnd::checkIdempotency()
{
  if( input2->getNodeType()                           == QtNode::QT_CONST &&
      ((QtConst*)input2)->getDataObj()->getDataType() == QT_BOOL     )
  {
    QtAtomicData* boolData = (QtAtomicData*)((QtConst*)input2)->getDataObj();

    if( boolData->getUnsignedValue() )
    {
      RMInit::logOut << "> rule (idempotency): A AND TRUE -> A" << endl;

      getParent()->setInput( this, input1 );

      // delete the node itself and its descendants but not input1
      setInput1(NULL);
      delete this;
    }
    else
    {
      if( input1->getAreaType() == QtNode::QT_AREA_SCALAR )
      {
        RMInit::logOut << "> rule (idempotency): A (scalar) AND FALSE -> FALSE" << endl;

        getParent()->setInput( this, input2 );

        // delete the node itself and its descendants but not input2
        setInput2(NULL);
        delete this;
      };
    }
  }
  else
    if( input1->getNodeType()                           == QtNode::QT_CONST && 
        ((QtConst*)input1)->getDataObj()->getDataType() == QT_BOOL     )
    {
      QtAtomicData* boolData = (QtAtomicData*)((QtConst*)input1)->getDataObj();

      if( boolData->getUnsignedValue() )
      {
        RMInit::logOut << "> rule (idempotency): TRUE AND A -> A" << endl;

        getParent()->setInput( this, input2 );

        // delete the node itself and its descendants but not input2
        setInput2(NULL);
        delete this;
      }
      else
      {
        if( input2->getAreaType() == QtNode::QT_AREA_SCALAR )
        {
          RMInit::logOut << "> rule (idempotency): FALSE AND A (scalar) -> FALSE" << endl;

          getParent()->setInput( this, input1 );

          // delete the node itself and its descendants but not input1
          setInput1(NULL);
          delete this;
        };
      };
    }
    else
      if( input1->getSpelling().compare( input2->getSpelling() ) == 0 )
      {
        RMInit::logOut << "> rule (idempotency): A AND A -> A" << endl;

        getParent()->setInput( this, input1 );

        // delete the node itself and its descendants but not input1
        setInput1(NULL);
        delete this;
      }
}
*/



/*
void
QtAnd::rewriteOps()
{
  if( input1 && input2 )
  {
    if( input1->getNodeType() == QtNode::QT_SOME &&
        input2->getNodeType() == QtNode::QT_ALL )
    {
      RMInit::logOut << "> rule (condenser order): SOME_CELLS AND ALL_CELLS -> ALL_CELLS AND SOME_CELLS" << endl;

      // order condenser expressions

      QtOperation* node1 = getInput1();
      QtOperation* node2 = getInput2();

      setInput1( node2 );
      setInput2( node1 );
    };
  }
  else
    RMInit::logOut << "Error: QtAnd::rewriteOps() - the operand branch is invalid." << endl;
}
*/


QtData*
QtAnd::evaluate( QtDataList* inputList )
{
  /*
  // RUNTIME OPTIMIZATION: FALSE AND A     -> MDD(FALSE)
  //                       A     AND FALSE -> MDD(FALSE)
  //                       TRUE  AND A     -> A
  //                       A     AND TRUE  -> A
  */  

  QtData* returnValue = NULL;

  if( input1->getDataStreamType().getDataType() == QT_BOOL && 
      input2->getDataStreamType().getDataType() == QT_BOOL    )
  {
    // RUNTIME OPTIMIZATION: FALSE AND A -> FALSE
    //                       TRUE  AND A -> A
  
    QtData* operand1=NULL;

    if( getOperand( inputList, operand1, 1 ) )
    {
      bool op1 = ((QtAtomicData*)operand1)->getUnsignedValue();

      if( op1 )
      {
        // first operand is obsolete
        if( operand1 ) operand1->deleteRef();

        QtData* operand2=NULL;

        if( getOperand( inputList, operand2, 2 ) )
	{
          returnValue = operand2;
        }
      }
      else
      {
        returnValue = operand1;
        RMDBGONCE(1, RMDebug::module_qlparser, "QtAnd",  "   -> FALSE AND A evaluates FALSE")
      }
    }
  }
  else
    returnValue = QtBinaryInduce::evaluate( inputList );

  return returnValue;
}


void
QtAnd::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtAnd Object " << getNodeType() << endl;

  QtBinaryInduce::printTree( tab, s, mode );
}


void
QtAnd::printAlgebraicExpression( ostream& s )
{
  s << "(";

  if( input1 )
    input1->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << " and ";

  if( input2 )
    input2->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";
}



const QtNode::QtNodeType QtOr::nodeType = QT_OR;

QtOr::QtOr( QtOperation* initInput1, QtOperation* initInput2 )
  :  QtBinaryInduce( initInput1, initInput2, Ops::OP_OR )
{
}

/*
void
QtOr::checkIdempotency()
{
  if( input2->getNodeType()                           == QtNode::QT_CONST &&
      ((QtConst*)input2)->getDataObj()->getDataType() == QT_BOOL     )
  {
    QtAtomicData* boolData = (QtAtomicData*)((QtConst*)input2)->getDataObj();

    if( boolData->getUnsignedValue() )
    {
      if( input1->getAreaType() == QtNode::QT_AREA_SCALAR )
      {
        RMInit::logOut << "> rule (idempotency): A (scalar) OR TRUE -> TRUE" << endl;

        getParent()->setInput( this, input2 );

        // delete the node itself and its descendants but not input2
        setInput2(NULL);
        delete this;
      };
    }
    else
    {
      RMInit::logOut << "> rule (idempotency): A OR FALSE -> A" << endl;

      getParent()->setInput( this, input1 );

      // delete the node itself and its descendants but not input1
      setInput1(NULL);
      delete this;
    }
  }
  else
    if( input1->getNodeType()                           == QtNode::QT_CONST &&
        ((QtConst*)input1)->getDataObj()->getDataType() == QT_BOOL     )
    {
      QtAtomicData* boolData = (QtAtomicData*)((QtConst*)input1)->getDataObj();

      if( boolData->getUnsignedValue() )
      {
        if( input2->getAreaType() == QtNode::QT_AREA_SCALAR )
        {
          RMInit::logOut << "> rule (idempotency): TRUE OR A (scalar) -> TRUE" << endl;

          getParent()->setInput( this, input1 );

          // delete the node itself and its descendants but not input1
          setInput1(NULL);
          delete this;
        };
      }
      else
      {
        RMInit::logOut << "> rule (idempotency): FALSE OR A -> A" << endl;

        getParent()->setInput( this, input2 );

        // delete the node itself and its descendants but not input2
        setInput2(NULL);
        delete this;
      };
    }
    else
      if( input1->getSpelling().compare( input2->getSpelling() ) == 0 )
      {
        RMInit::logOut << "> rule (idempotency): A OR A -> A" << endl;

        getParent()->setInput( this, input1 );

        // delete the node itself and its descendants but not input1
        setInput1(NULL);
        delete this;
      };
}
*/


/*
void
QtOr::rewriteOps()
{
  if( input1 && input2 )
  {
    if( input1->getNodeType() == QtNode::QT_ALL &&
        input2->getNodeType() == QtNode::QT_SOME )
    {
      RMInit::logOut << "> rule (condenser order): ALL_CELLS OR SOME_CELLS -> SOME_CELLS OR ALL_CELLS" << endl;

      // order condenser expressions

      QtOperation* node1 = getInput1();
      QtOperation* node2 = getInput2();

      setInput1( node2 );
      setInput2( node1 );
    };
  }
  else
    RMInit::logOut << "Error: QtOr::rewriteOps() - the operand branch is invalid." << endl;
}
*/

QtData*
QtOr::evaluate( QtDataList* inputList )
{
  /*
  // RUNTIME OPTIMIZATION: TRUE  OR  A     -> MDD(TRUE)
  //                       A     OR  TRUE  -> MDD(TRUE)
  //                       FALSE OR  A     -> A
  //                       A     OR  FALSE -> A
  */

  QtData* returnValue = NULL;

  if( input1->getDataStreamType().getDataType() == QT_BOOL && 
      input2->getDataStreamType().getDataType() == QT_BOOL    )
  {
    // RUNTIME OPTIMIZATION: FALSE OR A -> A
    //                       TRUE  OR A -> TRUE
  
    QtData* operand1=NULL;

    if( getOperand( inputList, operand1, 1 ) )
    {
      bool op1 = ((QtAtomicData*)operand1)->getUnsignedValue();

      if( !op1 )
      {
        // first operand is obsolete
        if( operand1 ) operand1->deleteRef();

        QtData* operand2=NULL;

        if( getOperand( inputList, operand2, 2 ) )
	{
          returnValue = operand2;
        }
      }
      else
      {
        returnValue = operand1;
        RMDBGONCE(1, RMDebug::module_qlparser, "QtOr",  "   -> TRUE OR A evaluates TRUE")
      }
    }
  }
  else
    returnValue = QtBinaryInduce::evaluate( inputList );

  return returnValue;
}


void
QtOr::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtOr Object " << getNodeType() << endl;

  QtBinaryInduce::printTree( tab, s, mode );
}


void
QtOr::printAlgebraicExpression( ostream& s )
{
  s << "(";

  if( input1 )
    input1->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << " or ";

  if( input2 )
    input2->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";
}



const QtNode::QtNodeType QtXor::nodeType = QT_XOR;


QtXor::QtXor( QtOperation* initInput1, QtOperation* initInput2 )
  :  QtBinaryInduce( initInput1, initInput2, Ops::OP_XOR )
{
}

/*
void
QtXor::checkIdempotency()
{
  RMDBGENTER(1, RMDebug::module_qlparser, "QtXor",  "enter Xor::checkIdempotency()" )

  if( input1->getSpelling().compare( input2->getSpelling() ) == 0 )
  {
    RMInit::logOut << "> rule (idempotency): A XOR A -> FALSE" << endl;

    getParent()->setInput( this, new QtConst( new QtAtomicData( 0 ) ) );

    // delete the node itself and its descendants
    delete this;
  };

  RMDBGEXIT(1, RMDebug::module_qlparser, "QtXor",  "exit  Xor::checkIdempotency()" )
}
*/

void
QtXor::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtXor Object " << getNodeType() << endl;

  QtBinaryInduce::printTree( tab, s, mode );
}


void
QtXor::printAlgebraicExpression( ostream& s )
{
  s << "(";

  if( input1 )
    input1->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << " xor ";

  if( input2 )
    input2->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";
}



const QtNode::QtNodeType QtEqual::nodeType = QT_EQUAL;

QtEqual::QtEqual( QtOperation* initInput1, QtOperation* initInput2 )
  :  QtBinaryInduce( initInput1, initInput2, Ops::OP_EQUAL )
{
}

/*
void
QtEqual::checkIdempotency()
{
  if( input1->getSpelling().compare( input2->getSpelling() ) == 0 )
  {
    RMInit::logOut << "> rule (idempotency): A == A -> TRUE" << endl;

    getParent()->setInput( this, new QtConst( new QtAtomicData( 1 ) ) );

    // delete the node itself and its descendants
    delete this;
  };
}
*/


void
QtEqual::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtEqual Object " << getNodeType() << endl;

  QtBinaryInduce::printTree( tab, s, mode );
}



void
QtEqual::printAlgebraicExpression( ostream& s )
{
  s << "(";

  if( input1 )
    input1->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << " = ";

  if( input2 )
    input2->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";
}



const QtNode::QtNodeType QtLess::nodeType = QT_LESS;


QtLess::QtLess( QtOperation* initInput1, QtOperation* initInput2 )
  :  QtBinaryInduce( initInput1, initInput2, Ops::OP_LESS )
{
}


bool
QtLess::isCommutative() const
{
  return false; // NOT commutative
}



void
QtLess::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtLess Object " << getNodeType() << endl;

  QtBinaryInduce::printTree( tab, s, mode );
}



void
QtLess::printAlgebraicExpression( ostream& s )
{
  s << "(";

  if( input1 )
    input1->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << " < ";

  if( input2 )
    input2->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";
}



const QtNode::QtNodeType QtLessEqual::nodeType = QT_LESS_EQUAL;


QtLessEqual::QtLessEqual( QtOperation* initInput1, QtOperation* initInput2 )
  :  QtBinaryInduce( initInput1, initInput2, Ops::OP_LESSEQUAL )
{
}


bool
QtLessEqual::isCommutative() const
{
  return false; // NOT commutative
}



void
QtLessEqual::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtLessEqual Object " << getNodeType() << endl;

  QtBinaryInduce::printTree( tab, s, mode );
}


void
QtLessEqual::printAlgebraicExpression( ostream& s )
{
  s << "(";

  if( input1 )
    input1->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << " <= ";

  if( input2 )
    input2->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";
}



const QtNode::QtNodeType QtNotEqual::nodeType = QT_NOT_EQUAL;

QtNotEqual::QtNotEqual( QtOperation* initInput1, QtOperation* initInput2 )
  :  QtBinaryInduce( initInput1, initInput2, Ops::OP_NOTEQUAL )
{
}

/*
void
QtNotEqual::checkIdempotency()
{
  if( input1->getSpelling().compare( input2->getSpelling() ) == 0 )
  {
    RMInit::logOut << "> rule (idempotency): A != A -> FALSE" << endl;

    getParent()->setInput( this, new QtConst( new QtAtomicData( 0 ) ) );

    // delete the node itself and its descendants
    delete this;
  };
}
*/

void
QtNotEqual::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtNotEqual Object " << getNodeType() << endl;

  QtBinaryInduce::printTree( tab, s, mode );
}



void
QtNotEqual::printAlgebraicExpression( ostream& s )
{
  s << "(";

  if( input1 )
    input1->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << " != ";

  if( input2 )
    input2->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";
}





const QtNode::QtNodeType QtOverlay::nodeType = QT_OVERLAY;

QtOverlay::QtOverlay( QtOperation* initInput1, QtOperation* initInput2 )
  :  QtBinaryInduce( initInput1, initInput2, Ops::OP_OVERLAY )
{
}

/*
void QtOverlay::checkIdempotency()
{
}
*/


bool QtOverlay::isCommutative() const
{
  return false; // NOT commutative
}



void
QtOverlay::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtOverlay Object " << getNodeType() << endl;

  QtBinaryInduce::printTree( tab, s, mode );
}



void
QtOverlay::printAlgebraicExpression( ostream& s )
{
  s << "(";

  if( input2 )
    input2->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << " overlay ";

  if( input1 )
    input1->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";
}

/********************************************************
 *	QtBit
 ********************************************************
 */
 
 
const QtNode::QtNodeType QtBit::nodeType = QT_BIT;

QtBit::QtBit(QtOperation* initInput1, QtOperation* initInput2)
	:  QtBinaryInduce(initInput1, initInput2, Ops::OP_BIT) {}

                                                                                                                                                                                                                   
bool QtBit::isCommutative() const {
	return false; 
}

void QtBit::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtBit Object " << getNodeType() << endl;
  	QtBinaryInduce::printTree(tab, s, mode);
}

void QtBit::printAlgebraicExpression(ostream& s) {
  	s << "(";

  	if( input2 )
    	input2->printAlgebraicExpression(s);
  	else
    	s << "<nn>";

  	s << " bit ";

  	if( input1 )
    	input1->printAlgebraicExpression(s);
  	else
    	s << "<nn>";

  	s << ")";
}

const QtTypeElement& QtBit::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtBit", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

  	dataStreamType.setDataType(QT_TYPE_UNKNOWN);  

  	// check operand branches
	if(input1 && input2) 
        {

  	// get input types
  	const QtTypeElement& inputType1 = input1->checkType(typeTuple); 
  	const QtTypeElement& inputType2 = input2->checkType(typeTuple); 
  
  	if(RManDebug >= 4) {
	    RMInit::dbgOut << "Operand 1: " << flush;
    	inputType1.printStatus( RMInit::dbgOut );
	    RMInit::dbgOut << endl;

    	RMInit::dbgOut << "Operand 2: " << flush;
	    inputType2.printStatus( RMInit::dbgOut );
    	RMInit::dbgOut << endl;

	    RMDBGONCE( 4, RMDebug::module_qlparser, "QtBit", "Operation            " << opType )
  	}

  	if(inputType2.getDataType() < QT_BOOL || inputType2.getDataType() > QT_LONG) {
    	  RMInit::logOut << "Error: QtBit::checkType() - second operand must be of integral type." << endl;
          parseInfo.setErrorNo(418);
	  throw parseInfo;
        }

	if(inputType1.getDataType() == QT_MDD) {
    	const BaseType* baseType1 = ((MDDBaseType*)(inputType1.getType()))->getBaseType();
    	BaseType* baseType2 = (BaseType*)(inputType2.getType());

    	const BaseType* resultBaseType = Ops::getResultType(opType, baseType1, baseType2);

    	if(!resultBaseType) {
      	  RMInit::logOut << "Error: QtBit::checkType() - unary induce: operand types are incompatible." << endl;
      	  parseInfo.setErrorNo(364);
      	  throw parseInfo;
    	}

    	MDDBaseType* resultMDDType = new MDDBaseType("tmp", resultBaseType);
    	TypeFactory::addTempType(resultMDDType);

    	dataStreamType.setType(resultMDDType);    
  	}

	else if(inputType1.isBaseType()) {
    	BaseType* baseType1 = (BaseType*)(inputType1.getType());
    	BaseType* baseType2 = (BaseType*)(inputType2.getType());

    	const BaseType* resultBaseType = Ops::getResultType(opType, baseType1, baseType2);

    	if(!resultBaseType) {
      	  RMInit::logOut << "Error: QtBit::computeOp() - operand types are incompatible." << endl;
      	  parseInfo.setErrorNo(365);
      	  throw parseInfo;
    	}

    	dataStreamType.setType(resultBaseType);    
  	}
  	else {
    	  RMInit::logOut << "Error: QtBit::checkType() - operation is not supported on these data types." << endl;
    	  parseInfo.setErrorNo(403);
    	  throw parseInfo;
  	}
        }
        else
    	  RMInit::logOut << "Error: QtBit::checkType() - operand branch invalid." << endl;

  	return dataStreamType;
}
