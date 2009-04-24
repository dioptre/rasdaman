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

static const char rcsid[] = "@(#)qlparser, QtCondense: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtcondense.cc,v 1.47 2005/09/03 20:17:55 rasdev Exp $";

#include "raslib/rmdebug.hh"
#include "debug.hh"

#include "qlparser/qtcondense.hh"
#include "qlparser/qtmdd.hh"
#include "qlparser/qtatomicdata.hh"
#include "qlparser/qtscalardata.hh"
#include "qlparser/qtcomplexdata.hh"
#include "qlparser/qtbinaryinduce.hh"
#include "qlparser/qtbinaryinduce2.hh"

#include "mddmgr/mddobj.hh"

#include "catalogmgr/typefactory.hh"
#include "catalogmgr/ops.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif

const QtNode::QtNodeType QtCondense::nodeType = QtNode::QT_CONDENSE;

QtCondense::QtCondense( Ops::OpType newOpType )
  : QtUnaryOperation(), opType( newOpType )
{
}



QtCondense::QtCondense( Ops::OpType newOpType, QtOperation* initInput )
  : QtUnaryOperation( initInput ), opType( newOpType )
{
}


QtNode::QtAreaType
QtCondense::getAreaType()
{
  return QT_AREA_SCALAR;
}



void
QtCondense::optimizeLoad( QtTrimList* trimList )
{
  // reset trimList because optimization enters a new MDD area

  // delete list
  // release( trimList->begin(), trimList->end() );
  vector<QtNode::QtTrimElement*>::iterator iter;
  for( iter=trimList->begin(); iter!=trimList->end(); iter++ )
  {
    delete *iter;
    *iter=NULL;
  }

  delete trimList;
  trimList=NULL;

  if( input )
    input->optimizeLoad( new QtNode::QtTrimList );
}



QtData*
QtCondense::computeFullCondense( QtDataList* inputList, r_Minterval& areaOp )
{
  RMDBCLASS( "QtCondense", "computeFullCondense( QtDataList*, r_Minterval& )", "qlparser", __FILE__, __LINE__ )

  QtScalarData* returnValue = NULL;

  // get the operand
  QtData* operand = input->evaluate( inputList );

  if( operand )
  {

#ifdef QT_RUNTIME_TYPE_CHECK
    if( operand->getDataType() != QT_MDD )
    {
      RMInit::logOut << "Internal error in QtCountCells::computeFullCondense() - "
                       << "runtime type checking failed (MDD)." << endl; 

      // delete old operand
      if( operand ) operand->deleteRef();

      return 0;
    }
#endif

    QtMDD* mdd = (QtMDD*)operand;

#ifdef QT_RUNTIME_TYPE_CHECK
    if( opType == Ops::OP_SOME || opType == Ops::OP_ALL || opType == Ops::OP_COUNT )
    {
      if( mdd->getCellType()->getType() != BOOLTYPE )
      {
        RMInit::logOut << "Internal error in QtCondense::computeFullCondense() - "
                         << "runtime type checking failed (BOOL)." << endl; 

        // delete old operand
        if( operand ) operand->deleteRef();

        return 0;
      }
    }
#endif

    // get result type
    const BaseType* resultType = Ops::getResultType( opType, mdd->getCellType() );

    // get the MDD object
    MDDObj* op = ((QtMDD*)operand)->getMDDObject();

    //  get the area, where the operation has to be applied
    areaOp = mdd->getLoadDomain();

TALK( "computeFullCondense-last-good\n" );
    // get all tiles in relevant area
    vector<Tile*>* allTiles = op->intersect(areaOp);

TALK( "computeFullCondense-8\n" );
    // get new operation object
    CondenseOp* condOp = Ops::getCondenseOp( opType, resultType, mdd->getCellType() );

TALK( "computeFullCondense-9\n" );
    // and iterate over them
    for( vector<Tile*>::iterator tileIt = allTiles->begin();
         tileIt!=allTiles->end(); tileIt++ )
    {
      // domain of the actual tile
      r_Minterval tileDom = (*tileIt)->getDomain();

      // domain of the relevant area of the actual tile
      r_Minterval intersectDom = tileDom.create_intersection( areaOp );

      (*tileIt)->execCondenseOp( condOp, intersectDom );
    }

    // delete tile vector
    delete allTiles;
    allTiles=NULL;

TALK( "computeFullCondense-a\n" );
    // create result object
    if( resultType->getType() == STRUCT )
      returnValue = new QtComplexData();
    else
      returnValue = new QtAtomicData();

TALK( "computeFullCondense-b\n" );
    // allocate buffer for the result
    char* resultBuffer = new char[resultType->getSize()];
    memcpy( (void*)resultBuffer, (void*)condOp->getAccuVal(), (size_t)resultType->getSize() );

TALK( "computeFullCondense-c\n" );
    returnValue->setValueType  ( resultType );
    returnValue->setValueBuffer( resultBuffer );

TALK( "computeFullCondense-d\n" );
    // delete operation object
    delete condOp;
    condOp=NULL;

TALK( "computeFullCondense-e\n" );
    // delete old operand
    if( operand ) operand->deleteRef();
  }

RMDBGIF(3, RMDebug::module_qlparser, "QtCondense", \
    RMInit::dbgOut << endl << "opType of QtCondense::computeFullCondense(): " << opType << endl; \
    RMInit::dbgOut <<         "Result.....................................: " << flush; \
    returnValue->printStatus( RMInit::dbgOut ); \
    RMInit::dbgOut << endl; )
  return returnValue;
}



const QtTypeElement&
QtCondense::checkType( QtTypeTuple* typeTuple )
{
  RMDBCLASS( "QtCondense", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

  dataStreamType.setDataType( QT_TYPE_UNKNOWN );  

  // check operand branches
  if( input )
  {

  // get input types
  const QtTypeElement& inputType = input->checkType( typeTuple ); 
  
RMDBGIF(3, RMDebug::module_qlparser, "QtCondense", \
    RMInit::dbgOut << "Class..: " << getClassName() << endl; \
    RMInit::dbgOut << "Operand: " << flush; \
    inputType.printStatus( RMInit::dbgOut ); \
    RMInit::dbgOut << endl; )

  if( inputType.getDataType() != QT_MDD )
  {
    RMInit::logOut << "Error: QtCondense::evaluate() - operand must be multidimensional." << endl;
    parseInfo.setErrorNo(353);
    throw parseInfo;
  }

  const BaseType* baseType = ((const MDDBaseType*)(inputType.getType()))->getBaseType();

  if( opType == Ops::OP_SOME || opType == Ops::OP_ALL )
  {
    if( baseType->getType() != BOOLTYPE )
    {
      RMInit::logOut << "Error: QtCondense::evaluate() - operand of quantifiers must be of type r_Marray<d_Boolean>." << endl;
      parseInfo.setErrorNo(354);
      throw parseInfo;
    }
  }

  if( opType == Ops::OP_COUNT )
  {
    if( baseType->getType() != BOOLTYPE )
    {
      RMInit::logOut << "Error: QtCondense::evaluate() - operand of count_cells must be of type r_Marray<d_Boolean>." << endl;
      parseInfo.setErrorNo(415);
      throw parseInfo;
    }
  }

  const BaseType* resultType = Ops::getResultType( opType, baseType );

  if( getNodeType() == QT_AVGCELLS )
  {
    // consider division by the number of cells

    const BaseType* DoubleType = TypeFactory::mapType("Double");
    const BaseType* finalResultType = Ops::getResultType( Ops::OP_DIV, resultType, DoubleType );

    resultType = finalResultType;
  }

  dataStreamType.setType( resultType );
  }
  else
    RMInit::logOut << "Error: QtCondense::checkType() - operand branch invalid." << endl;

  return dataStreamType;
}


void
QtCondense::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << getClassName() << " object" << endl;

  QtUnaryOperation::printTree( tab, s, mode );
}



void
QtCondense::printAlgebraicExpression( ostream& s )
{
  s << getAlgebraicName() << "(";

  if( input )
    input->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";
}



const QtNode::QtNodeType QtSome::nodeType = QtNode::QT_SOME;


QtSome::QtSome()
  : QtCondense( Ops::OP_SOME )
{
}


QtSome::QtSome( QtOperation* inputNew )
  : QtCondense( Ops::OP_SOME, inputNew )
{
}

/*
void
QtSome::rewriteOps()
{
  if( input )
  {
    if( input->getNodeType() == QtNode::QT_OR )
    {
      // pushdown of condenser expression

      QtOr* orNode = (QtOr*) input;
      QtOperation* node1 = orNode->getInput1();
      QtOperation* node2 = orNode->getInput2();

      if( node1 && node2 &&
          node1->getAreaType() == QtNode::QT_AREA_MDD &&
          node2->getAreaType() == QtNode::QT_AREA_MDD )
      {
        RMInit::logOut << "> rule (pushdown condenser): SOME_CELLS(A OR B) -> SOME_CELLS(A) OR SOME_CELLS(B)" << endl;

        QtSome* newNode = new QtSome( node1 );
        setInput( node2 );
        newNode->setDataStreamType( QtTypeElement(QT_BOOL) );

        this->getParent()->setInput( this, orNode );
        orNode->setInput1( newNode );
        orNode->setInput2( this );
        orNode->setDataStreamType( QtTypeElement(QT_BOOL) );

        newNode->rewriteOps();
      }
    };

    input->rewriteOps();
  }
  else
    RMInit::logOut << "Error: QtSome::rewriteOps() - the operand branch is invalid." << endl;
}
*/

QtData*
QtSome::evaluate( QtDataList* inputList )
{
  RMDBCLASS( "QtSome", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

  QtData* returnValue = NULL;
  r_ULong dummy=0; // needed for conversion to and from CULong

  // get the operand
  QtData* operand = input->evaluate( inputList );

  if( operand )
  {
#ifdef QT_RUNTIME_TYPE_CHECK
    if( operand->getDataType() != QT_MDD )
    {
      RMInit::logOut << "Internal error in QtSome::evaluate() - "
                       << "runtime type checking failed (MDD)." << endl; 

      // delete old operand
      if( operand ) operand->deleteRef();

      return 0;
    }
#endif

    QtMDD* mdd = (QtMDD*)operand;

    // get result type
    BaseType* resultType = (BaseType*)dataStreamType.getType();

#ifdef QT_RUNTIME_TYPE_CHECK
    if( mdd->getCellType()->getType() != BOOLTYPE )
      RMInit::logOut << "Internal error in QtSome::evaluate() - "
                       << "runtime type checking failed (BOOL)." << endl; 

      // delete old operand
      if( operand ) operand->deleteRef();

      return 0;
    }
#endif

    // get the MDD object
    MDDObj* op = mdd->getMDDObject();

    //  get the area, where the operation has to be applied
    r_Minterval areaOp = mdd->getLoadDomain();

    // get all tiles in relevant area
        vector<Tile*>* allTiles = op->intersect(areaOp);

    // allocate buffer for the result
	unsigned int typeSize = resultType->getSize();
    char* resultBuffer = new char[typeSize];

    // initialize result buffer with false
    dummy = 0;
    resultType->makeFromCULong( resultBuffer, &dummy );
    	CondenseOp* condOp = Ops::getCondenseOp(Ops::OP_SOME, resultType, resultBuffer, resultType, 0, 0);

    // and iterate over them
    for( vector<Tile*>::iterator tileIt = allTiles->begin(); tileIt !=  allTiles->end() && !dummy ; tileIt++ )
    {
      // domain of the actual tile
      r_Minterval tileDom = (*tileIt)->getDomain();

      // domain of the relevant area of the actual tile
      r_Minterval intersectDom = tileDom.create_intersection( areaOp );
      (*tileIt)->execCondenseOp( condOp, intersectDom );
	resultType->convertToCULong( condOp->getAccuVal(), &dummy );
    }

	delete condOp;
	condOp = NULL;
    // delete tile vector
    delete allTiles;
    allTiles=NULL;

    // create QtAtomicData object for the result
    returnValue = new QtAtomicData( (bool)(dummy) );

    // delete result buffer
    delete[] resultBuffer;
    resultBuffer = NULL;

    // The following is now then when deleting the last reference to the operand.
    // delete the obsolete MDD object
    // delete op;

    // delete old operand
    if( operand ) operand->deleteRef();
  }

  return returnValue;
}



const QtAll::QtNodeType QtAll::nodeType = QtNode::QT_ALL;


QtAll::QtAll()
  : QtCondense( Ops::OP_ALL )
{
}


QtAll::QtAll( QtOperation* inputNew )
  : QtCondense( Ops::OP_ALL, inputNew )
{
}

/*
void
QtAll::rewriteOps()
{
  if( input )
  {
    if( input->getNodeType() == QtNode::QT_AND )
    {
      // pushdown of condenser expression

      QtAnd* andNode = (QtAnd*) input;
      QtOperation* node1 = andNode->getInput1();
      QtOperation* node2 = andNode->getInput2();

      if( node1 && node2 &&
          node1->getAreaType() == QtNode::QT_AREA_MDD &&
          node2->getAreaType() == QtNode::QT_AREA_MDD )
      {
        RMInit::logOut << "> rule (pushdown condenser): ALL_CELLS(A AND B) -> ALL_CELLS(A) AND ALL_CELLS(B)" << endl;

        QtAll* newNode = new QtAll( node1 );
        setInput( node2 );
        newNode->setDataStreamType( QtTypeElement(QT_BOOL) ); 

        this->getParent()->setInput( this, andNode );
        andNode->setInput1( newNode );
        andNode->setInput2( this );
        andNode->setDataStreamType( QtTypeElement(QT_BOOL) );

        newNode->rewriteOps();
      }
    };

    input->rewriteOps();

  }
  else
    RMInit::logOut << "Error: QtAll::rewriteOps() - the operand branch is invalid." << endl;
}
*/

QtData*
QtAll::evaluate( QtDataList* inputList )
{
  RMDBCLASS( "QtAll", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

  QtData* returnValue = NULL;
  r_ULong dummy=0; // needed for conversion to and from CULong

  // get the operand
  QtData* operand = input->evaluate( inputList );

  if( operand )
  {

#ifdef QT_RUNTIME_TYPE_CHECK
    if( operand->getDataType() != QT_MDD )
    {
      RMInit::logOut << "Internal error in QtAll::evaluate() - "
                       << "runtime type checking failed (MDD)." << endl; 

      // delete old operand
      if( operand ) operand->deleteRef();

      return 0;
    }
#endif

    QtMDD* mdd = (QtMDD*)operand;

    // get result type
    const BaseType* resultType = (BaseType*)dataStreamType.getType();

#ifdef QT_RUNTIME_TYPE_CHECK
    if( mdd->getCellType()->getType() != BOOLTYPE )
      RMInit::logOut << "Internal error in QtAll::evaluate() - "
                       << "runtime type checking failed (BOOL)." << endl; 

      // delete old operand
      if( operand ) operand->deleteRef();

      return 0;
    }
#endif

    // get the MDD object
    MDDObj* op = ((QtMDD*)operand)->getMDDObject();

    //  get the area, where the operation has to be applied
    r_Minterval areaOp = mdd->getLoadDomain();

    // get all tiles in relevant area
    vector<Tile*>* allTiles = op->intersect(areaOp);

    // allocate buffer for the result
	unsigned int tempTypeSize = resultType->getSize();
    char* resultBuffer = new char[tempTypeSize];

    // initialize result buffer with true
    dummy = 1;
    resultType->makeFromCULong( resultBuffer, &dummy );
    	CondenseOp* condOp = Ops::getCondenseOp(Ops::OP_ALL, resultType, resultBuffer, resultType, 0, 0);

    for( std::vector<Tile*>::iterator tileIt = allTiles->begin(); tileIt!=allTiles->end() && dummy; tileIt++ )
    {
      // domain of the actual tile
      r_Minterval tileDom = (*tileIt)->getDomain();

      // domain of the relevant area of the actual tile
      r_Minterval intersectDom = tileDom.create_intersection( areaOp );

      (*tileIt)->execCondenseOp( condOp, intersectDom );
	resultType->convertToCULong( condOp->getAccuVal(), &dummy );
    }
	delete condOp;
	condOp = NULL;
    // delete tile vector
    delete allTiles;
    allTiles=NULL;

    // create QtBoolData object for the result
    returnValue = new QtAtomicData( (bool)(dummy) );

    // delete result buffer done in delete CondOp
    delete[] resultBuffer;
    resultBuffer=NULL;

    // delete old operand
    if( operand ) operand->deleteRef();
  }

  return returnValue;
}



const QtCountCells::QtNodeType QtCountCells::nodeType = QtNode::QT_COUNTCELLS;


QtCountCells::QtCountCells()
  : QtCondense( Ops::OP_COUNT )
{
}


QtCountCells::QtCountCells( QtOperation* inputNew )
  : QtCondense( Ops::OP_COUNT, inputNew )
{
}


QtData*
QtCountCells::evaluate( QtDataList* inputList )
{
  RMDBCLASS( "QtCountCells", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )
r_Minterval dummyint;
  QtData* returnValue = QtCondense::computeFullCondense( inputList, dummyint );

  return returnValue;
}



const QtAddCells::QtNodeType QtAddCells::nodeType = QtNode::QT_ADDCELLS;


QtAddCells::QtAddCells()
  : QtCondense( Ops::OP_SUM )
{
}


QtAddCells::QtAddCells( QtOperation* inputNew )
  : QtCondense( Ops::OP_SUM, inputNew )
{
}


QtData*
QtAddCells::evaluate( QtDataList* inputList )
{
  RMDBCLASS( "QtAddCells", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )
r_Minterval dummyint;

  QtData* returnValue = QtCondense::computeFullCondense( inputList, dummyint );

  return returnValue;
}



const QtAvgCells::QtNodeType QtAvgCells::nodeType = QtNode::QT_AVGCELLS;


QtAvgCells::QtAvgCells()
  : QtCondense( Ops::OP_SUM )
{
}


QtAvgCells::QtAvgCells( QtOperation* inputNew )
  : QtCondense( Ops::OP_SUM, inputNew )
{
}


QtData*
QtAvgCells::evaluate( QtDataList* inputList )
{
  RMDBCLASS( "QtAvgCells", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

  // domain for condensing operation 
  r_Minterval areaOp;

  QtData* dataCond = QtCondense::computeFullCondense( inputList, areaOp );

  //
  // divide by the number of cells  
  //

  QtScalarData* scalarDataResult = NULL;
  QtScalarData* scalarDataCond   = (QtScalarData*)dataCond; 
  BaseType*     resultType       = (BaseType*)dataStreamType.getType();


  // allocate memory for the result
  char* resultBuffer = new char[ resultType->getSize() ];

  // allocate ulong constant with number of cells
  r_ULong constValue  = areaOp.cell_count(); 
  const BaseType*     constType   = TypeFactory::mapType("ULong");
  char*         constBuffer = new char[ constType->getSize() ];

  constType->makeFromCULong( constBuffer, &constValue ); 
  
RMDBGIF(3, RMDebug::module_qlparser, "QtCondense", \
    RMInit::dbgOut <<         "Number of cells....: " << flush; \
    constType->printCell( RMInit::dbgOut, constBuffer ); \
    RMInit::dbgOut << endl; )

  Ops::execBinaryConstOp( Ops::OP_DIV, resultType, 
                          scalarDataCond->getValueType(),   constType,
                          resultBuffer,
                          scalarDataCond->getValueBuffer(), constBuffer );

  delete[] constBuffer;
  constBuffer=NULL;
  delete dataCond;
  dataCond=NULL;

  if( resultType->getType() == STRUCT )
    scalarDataResult = new QtComplexData();
  else
    scalarDataResult = new QtAtomicData();

  scalarDataResult->setValueType  ( resultType );
  scalarDataResult->setValueBuffer( resultBuffer );

RMDBGIF(3, RMDebug::module_qlparser, "QtCondense", \
    RMInit::dbgOut << endl << "Result.............: " << flush; \
    scalarDataResult->printStatus( RMInit::dbgOut ); \
    RMInit::dbgOut << endl; )

  return scalarDataResult;
}


const QtMinCells::QtNodeType QtMinCells::nodeType = QtNode::QT_MINCELLS;


QtMinCells::QtMinCells()
  : QtCondense( Ops::OP_MIN )
{
}


QtMinCells::QtMinCells( QtOperation* inputNew )
  : QtCondense( Ops::OP_MIN, inputNew )
{
}


QtData*
QtMinCells::evaluate( QtDataList* inputList )
{
  RMDBCLASS( "QtMinCells", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )
r_Minterval dummyint;

  QtData* returnValue = QtCondense::computeFullCondense( inputList, dummyint );

  return returnValue;
}



const QtMaxCells::QtNodeType QtMaxCells::nodeType = QtNode::QT_MAXCELLS;


QtMaxCells::QtMaxCells()
  : QtCondense( Ops::OP_MAX )
{
}


QtMaxCells::QtMaxCells( QtOperation* inputNew )
  : QtCondense( Ops::OP_MAX, inputNew )
{
}


QtData*
QtMaxCells::evaluate( QtDataList* inputList )
{
  RMDBCLASS( "QtMaxCells", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )
r_Minterval dummyint;

  QtData* returnValue = QtCondense::computeFullCondense( inputList, dummyint );

  return returnValue;
}
