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

static const char rcsid[] = "@(#)qlparser, QtBinaryOperation: $Id: qtbinaryoperation.cc,v 1.25 2002/04/12 09:51:53 coman Exp $";

#include "qlparser/qtbinaryoperation.hh"
#include "qlparser/qtconst.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif

// constructors

QtBinaryOperation::QtBinaryOperation() : 
	QtOperation(),
	input1(NULL),
	input2(NULL)
{
}


QtBinaryOperation::QtBinaryOperation( QtNode* node ) :
	QtOperation( node ),
	input1(NULL),
	input2(NULL)
{
}


QtBinaryOperation::QtBinaryOperation( QtOperation* initInput1, QtOperation* initInput2 ) :
	QtOperation(),
	input1( initInput1 ),
	input2( initInput2 )
{
  if ( input1 ) 
    input1->setParent( this );

  if ( input2 )
    input2->setParent( this );
}

// destructor

QtBinaryOperation::~QtBinaryOperation()
{
  if ( input1 )
  {
    delete input1;
    input1=NULL;
  }
  if ( input2 )
  {
    delete input2;
    input2=NULL;
  }
}


// pre-evaluate the operation if the nodes contain constant data

void
QtBinaryOperation::simplify()
{
  RMDBCLASS( "QtBinaryOperation", "simplify()", "qlparser", __FILE__, __LINE__ )

  // In order to work bottom up, first inspect the descendants
  QtNode::simplify();

  // Test, if both operands are available.
  if( input1 && input2 )
  {
    // Test, if both operands are of const type.
    if( input1->getNodeType() ==  QT_CONST && input2->getNodeType() == QT_CONST )
    {
      // evaluate the self node with no input list
      QtData* newConst = this->evaluate( NULL );

      if( newConst )
      {
        // create a new constant node and fill it with newConst
        QtConst* newNode = new QtConst( newConst );

        // set its data stream type
        newNode->checkType( NULL );

        // link it to the parent 
        getParent()->setInput( this, newNode );

        // delete the self node and its descendants
        delete this; 
      }
    }
  }
  
}


// compare this to another node

bool
QtBinaryOperation::equalMeaning( QtNode* node )
{
  RMDBCLASS( "QtBinaryOperation", "equalMeaning( QtNode* )", "qlparser", __FILE__, __LINE__ )

  bool result = false;

  // are the nodes of the same type?
  if ( getNodeType() == node->getNodeType() )
  {
    QtBinaryOperation* binNode = (QtBinaryOperation *) node; // by force

    if ( input1 && input2 ) {
      if ( isCommutative() ) 
        // operation is commutative
        result = ( input1->equalMeaning( binNode->getInput1() ) &&
                   input2->equalMeaning( binNode->getInput2() )     ) ||
                 ( input1->equalMeaning( binNode->getInput2() ) &&
                   input2->equalMeaning( binNode->getInput1() )     );
      else 
        // not commutative    
        result = input1->equalMeaning( binNode->getInput1() ) &&
                 input2->equalMeaning( binNode->getInput2() );
    };           
  };

  return result;
}


// get childs

QtNode::QtNodeList*
QtBinaryOperation::getChilds( QtChildType flag )
{
  RMDBCLASS( "QtBinaryOperation", "getChilds( QtChildType )", "qlparser", __FILE__, __LINE__ )

  QtNodeList* resultList = new QtNodeList();

  if ( flag == QT_LEAF_NODES || flag == QT_ALL_NODES ) {
    QtNodeList* subList=NULL;
    
    if ( input1 ) {
      subList = input1->getChilds( flag );
      resultList->splice(resultList->begin(), *subList);
      delete subList;
      subList=NULL;
    };    
    
    if ( input2 ) {
      subList = input2->getChilds( flag );
      resultList->splice(resultList->begin(), *subList);
      delete subList;
      subList=NULL;
    };      
  };

  if ( flag == QT_DIRECT_CHILDS || flag == QT_ALL_NODES ) {
    if ( input1 ) 
      resultList->push_back( input1 );
    if ( input2 ) 
      resultList->push_back( input2 );
  };

  return resultList;
}


// get the two operands

bool
QtBinaryOperation::getOperands( QtDataList* inputList, QtData* &operand1, QtData* &operand2 )
{
  RMDBCLASS( "QtBinaryOperation", "getOperands( QtDataList*, QtData*, QtData* )", "qlparser", __FILE__, __LINE__ )

  bool success = false;

  // get the operands
  try {
    if ( input1 ) operand1 = input1->evaluate( inputList );
    if ( input2 ) operand2 = input2->evaluate( inputList );
  }
  catch(...)
  {
    //clean up code
    if( operand1 ) {
      operand1->deleteRef();
      operand1 = NULL;
    }

    if( operand2 ){
      operand2->deleteRef();
      operand2 = NULL;
    }
  
    throw;
  }
  
  // test if the operands are valid
  success = operand1 && operand2;

  if( !success )
  {
  
    if( operand1 ) 
    {
      operand1->deleteRef();
      operand1 = NULL;
    }

    if( operand2 ) 
    {
      operand2->deleteRef();
      operand2 = NULL;
    }

    RMDBGONCE( 1, RMDebug::module_qlparser, "QtBinaryOperation", "Error: QtBinaryOperation::getOperands() - at least one operand is not provided." )
  
  }

  return success;
}


// get the first or the second operand

bool
QtBinaryOperation::getOperand( QtDataList* inputList, QtData* &operand, int number )
{
  RMDBCLASS( "QtBinaryOperation", "getOperand( QtDataList*, QtData*, int )", "qlparser", __FILE__, __LINE__ )

  bool success = false;

  // get the operand
  if ( number == 1 ) {
     if ( input1 ) {
       operand = input1->evaluate( inputList );
     } else { 
       if ( input2 )
         operand = input2->evaluate( inputList );
     }	 
  } else {
    if ( input2 )
      operand = input2->evaluate( inputList );
  }

  // test if the operands are valid
  if ( operand )
    success = true;
  else {
    RMDBGONCE( 1, RMDebug::module_qlparser, "QtBinaryOperation", "Error: QtBinaryOperation::getOperand() - operand is not provided." )
  }

  return success;
}


// get spelling

string
QtBinaryOperation::getSpelling()
{
  char tempStr[20];
  sprintf(tempStr, "%ud", (unsigned long)getNodeType());
  string result  = string(tempStr);
  result.append( "(" );
  
  if ( input1 && input2 ) {  
    string result1 = input1->getSpelling();
    string result2 = input2->getSpelling();

    if( result1.compare( result2 ) < 0 || !isCommutative() )
    {
      result.append( result1 );
      result.append( "," );
      result.append( result2 );
    }
    else
    {
      result.append( result2 );
      result.append( "," );
      result.append( result1 );
    };
  };
  
  result.append( ")" );
  RMDBGONCE(2, RMDebug::module_qlparser, "QtBinaryOperation", "Result:" << result.c_str())
  return result;
}


// get area type

QtNode::QtAreaType
QtBinaryOperation::getAreaType()
{
  QtNode::QtAreaType result = QT_AREA_MDD;

  if ( input1 && input2 )
    if ( input1->getAreaType() == QtNode::QT_AREA_SCALAR &&
         input2->getAreaType() == QtNode::QT_AREA_SCALAR )
    result = QT_AREA_SCALAR;

  return result;
}


// idempotent

/*
void
QtBinaryOperation::checkIdempotency()
{
}
*/

// optimize load

void
QtBinaryOperation::optimizeLoad( QtTrimList* trimList )
{
  RMDBCLASS( "QtBinaryOperation", "optimizeLoad( QtTrimList* )", "qlparser", __FILE__, __LINE__ )

  QtNode::QtTrimList *list1=NULL; 
  QtNode::QtTrimList *list2=NULL;

  if( input1 && input2 )
  {
    list1 = trimList;

    // for list2 make a copy of list1
    list2 = new QtNode::QtTrimList();

    for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
    {
      QtTrimElement* elem = new QtTrimElement;
      *elem = **iter;
      list2->push_back( elem );
    }

    if ( input1 ) 
      input1->optimizeLoad( list1 );
    
    if ( input2 ) 
      input2->optimizeLoad( list2 );
      
  }
  else 
  {
    // release( trimList->begin(), trimList->end() );
    for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
    { 
      delete *iter;
      *iter=NULL;
    }
    delete trimList;
    trimList=NULL;
  }
}


void
QtBinaryOperation::printTree( int tab, ostream& s, QtChildType mode )
{
  if( mode != QT_DIRECT_CHILDS )
  {
    if( input1 )
    {
      s << SPACE_STR(tab).c_str() << "input1: " << endl;
      input1->printTree( tab+2, s, mode );
    }
    else
      s << SPACE_STR(tab).c_str() << "no input1" << endl;

    if( input2 )
    { 
      s << SPACE_STR(tab).c_str() << "input2: " << endl;
      input2->printTree( tab+2, s, mode );
    }
    else
      s << SPACE_STR(tab).c_str() << "no input2" << endl;
  }
}



bool
QtBinaryOperation::isCommutative() const
{
  return true; // by default, a binary operation is commutative
}








