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

static const char rcsid[] = "@(#)qlparser, QtMarrayOp: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtmarrayop.cc,v 1.19 2003/12/27 20:51:28 rasdev Exp $";

#include "raslib/rmdebug.hh"

#include "qlparser/qtmarrayop.hh"
#include "qlparser/qtdata.hh"
#include "qlparser/qtmdd.hh"
#include "qlparser/qtpointdata.hh"
#include "qlparser/qtmintervaldata.hh"
#include "qlparser/qtintervaldata.hh"

#include "mddmgr/mddobj.hh"
#include "tilemgr/tile.hh"

#include "catalogmgr/typefactory.hh"

#include "catalogmgr/algebraops.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif


const QtNode::QtNodeType QtMarrayOp::nodeType = QT_MARRAYOP;

QtMarrayOp::QtMarrayOp( const string &initIteratorName, QtOperation* mintervalExp, QtOperation* cellExp )
  :  iteratorName( initIteratorName ), QtBinaryOperation( mintervalExp, cellExp )
{
}



void
QtMarrayOp::optimizeLoad( QtTrimList* trimList )
{
  RMDBCLASS( "QtMarrayOp", "optimizeLoad( QtTrimList* )", "qlparser", __FILE__, __LINE__ )

  // delete the trimList and optimize subtrees

  // release( trimList->begin(), trimList->end() );
  for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
  {
    delete *iter;
    *iter=NULL;
  }
  delete trimList;
  trimList=NULL;

  QtBinaryOperation::optimizeLoad( new QtNode::QtTrimList() );
}



bool
QtMarrayOp::isCommutative() const
{
  return false; // NOT commutative
}



QtData*
QtMarrayOp::evaluate( QtDataList* inputList )
{
  RMDBCLASS( "QtMarrayOp", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

  QtData* returnValue = NULL;
  QtData* operand1 = NULL;

  if( getOperand( inputList, operand1, 1 ) )
  {

    //idea: if operand1 is a QT_INTERVAL convert it to QT_MINTERVAL
    if (operand1->getDataType() == QT_INTERVAL)
    {
      //do conversion to QT_MINTERVAL

      //create one-dimensional minterval from operand1 (operand1 is a sinterval)
      r_Minterval tmpMinterval(r_Dimension(1));
      tmpMinterval << ((QtIntervalData*)operand1)->getIntervalData();

      //save operand1-pointer to oldOp
      QtData* oldOp=operand1;
      //overwrite operand1 with new minterval
      operand1=new QtMintervalData(tmpMinterval);
      //delete old operand1
      if( oldOp ) oldOp->deleteRef();
    }

#ifdef QT_RUNTIME_TYPE_CHECK
    if( operand1->getDataType() != QT_MINTERVAL )
      RMInit::logOut << "Internal error in QtMarrayOp::evaluate() - "
                       << "runtime type checking failed (Minterval)." << endl; 

      // delete old operand
      if( operand1 ) operand1->deleteRef();

      return 0;
    }
#endif

    r_Minterval domain = ((QtMintervalData*)operand1)->getMintervalData();

    RMDBGONCE( 4, RMDebug::module_qlparser, "QtMarrayOp", "Marray domain " << domain )

    //
    // add point data with its iterator name to the input list  
    //

    // create a QtPointData object with corner point
    QtPointData* point = new QtPointData( domain.get_origin() );
    // set its iterator name
    point->setIteratorName( iteratorName );
    // if the list of binding variables is empty, create a new one and delete it afterwards
    bool newInputList = false;
    if( !inputList )
    {
      inputList = new QtDataList();
      newInputList = true;
    }
    // add it to the list
    inputList->push_back( point );

    // determine types
    BaseType*    cellType    = (BaseType*)   input2->getDataStreamType().getType();
    MDDBaseType* mddBaseType = (MDDBaseType*)dataStreamType.getType();
    
    // create tile for result
    Tile* resTile = new Tile( domain, cellType );

    // create execution object QLArrayOp
    QLMarrayOp* qlMarrayOp = new QLMarrayOp( input2, inputList, iteratorName, cellType );

    try
    {
      // execute query engine marray operation 
      resTile->execMarrayOp( qlMarrayOp, domain, domain );
    }
    catch(...)
    {
      // free ressources
      delete qlMarrayOp;
      qlMarrayOp=NULL;
      delete resTile;
      resTile=NULL;

      // remove point data object from inputList again
      inputList->back()->deleteRef();
      inputList->pop_back();
      if( newInputList )
      {
        delete inputList;
        inputList=NULL;
      }

      if( operand1 ) operand1->deleteRef();

      throw;
    }

    // delete execution object again
    delete qlMarrayOp;
    qlMarrayOp=NULL;

    // remove point data object from inputList again
    inputList->back()->deleteRef();
    inputList->pop_back();
    if( newInputList )
    {
      delete inputList;
      inputList=NULL;
    }
    // create MDDObj for result
    MDDObj* mddres = new MDDObj( mddBaseType, domain );

    // insert Tile in result mdd
    mddres->insertTile( resTile );

    // create a new QtMDD object as carrier object for the transient MDD object
    returnValue = new QtMDD( (MDDObj*)mddres );

    // delete old operands
    if( operand1 ) operand1->deleteRef();
  }

  return returnValue;
}



void
QtMarrayOp::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtMarrayOp Object " << getNodeType() << endl;

  s << SPACE_STR(tab).c_str() << "Iterator Name: " << iteratorName.c_str() << endl;

  QtBinaryOperation::printTree( tab, s, mode );
}



void
QtMarrayOp::printAlgebraicExpression( ostream& s )
{
  s << "(";

  s << iteratorName.c_str() << ",";

  if( input1 )
    input1->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ",";

  if( input2 )
    input2->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";
}



const QtTypeElement&
QtMarrayOp::checkType( QtTypeTuple* typeTuple )
{
  RMDBCLASS( "QtMarrayOp", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

  dataStreamType.setDataType( QT_TYPE_UNKNOWN );  

  // check operand branches
  if( input1 && input2 )
  {

  // check domain expression
  const QtTypeElement& domainExp = input1->checkType( typeTuple );

  if( (domainExp.getDataType() != QT_MINTERVAL) && (domainExp.getDataType() != QT_INTERVAL) )
  {
    RMInit::logOut << "Error: QtMarrayOp::checkType() - Can not evaluate domain expression to an minterval." << endl;
    parseInfo.setErrorNo(401);
    throw parseInfo;
  }

  //
  // check value expression
  //
  
  // add domain iterator to the list of bounded variables
  bool newList = false;
  if( !typeTuple )
  {
    typeTuple = new QtTypeTuple();
    newList = true;
  } 
  typeTuple->tuple.push_back( QtTypeElement( QT_POINT, iteratorName.c_str() ) );

  // get type
  const QtTypeElement& valueExp = input2->checkType( typeTuple );

  // remove iterator again
  typeTuple->tuple.pop_back();
  if( newList )
  {
    delete typeTuple;
    typeTuple = NULL;
  }

  // check type
  if( valueExp.getDataType() != QT_BOOL   && valueExp.getDataType() != QT_COMPLEX &&
      valueExp.getDataType() != QT_CHAR   && valueExp.getDataType() != QT_OCTET   &&
      valueExp.getDataType() != QT_USHORT && valueExp.getDataType() != QT_SHORT   &&
      valueExp.getDataType() != QT_ULONG  && valueExp.getDataType() != QT_LONG    &&
      valueExp.getDataType() != QT_FLOAT  && valueExp.getDataType() != QT_DOUBLE  &&  
      valueExp.getDataType() != QT_COMPLEXTYPE1 && valueExp.getDataType() != QT_COMPLEXTYPE2 )
  {
    RMInit::logOut << "Error: QtMarrayOp::checkType() - Value expression must be either of type atomic or complex." << endl;
    parseInfo.setErrorNo(412);
    throw parseInfo;
  }

  // create MDD type
  BaseType*    cellType    = (BaseType*)valueExp.getType();
  MDDBaseType* mddBaseType = new MDDBaseType( "tmp", cellType );
  TypeFactory::addTempType( mddBaseType );

  dataStreamType.setType( mddBaseType );
  }
  else
    RMInit::logOut << "Error: QtMarrayOp::checkType() - operand branch invalid." << endl;

  return dataStreamType;
}
