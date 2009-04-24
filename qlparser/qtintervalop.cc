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

static const char rcsid[] = "@(#)qlparser, QtIntervalOp: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtintervalop.cc,v 1.13 2003/12/27 20:51:28 rasdev Exp $";

#include "raslib/rmdebug.hh"

#include "qlparser/qtintervalop.hh"
#include "qlparser/qtdata.hh"
#include "qlparser/qtintervaldata.hh"
#include "qlparser/qtstringdata.hh"
#include "qlparser/qtatomicdata.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif


const QtNode::QtNodeType QtIntervalOp::nodeType = QT_INTERVALOP;

QtIntervalOp::QtIntervalOp( QtOperation* initInput1, QtOperation* initInput2 )
  :  QtBinaryOperation( initInput1, initInput2 )
{
}



bool
QtIntervalOp::isCommutative() const
{
  return false; // NOT commutative
}



QtData*
QtIntervalOp::evaluate( QtDataList* inputList )
{
  QtData* returnValue = NULL;
  QtData* operand1 = NULL;
  QtData* operand2 = NULL;

  if( getOperands( inputList, operand1, operand2 ) )
  {
    r_Sinterval sinterval;

    switch( operand1->getDataType() )
    {
      case QT_LONG:
      case QT_SHORT:
      case QT_OCTET:  
                 try{
                    sinterval.set_low( (r_Range)(((QtAtomicData*)operand1)->getSignedValue()) );
	         }
                 catch(...)
	         {
                   RMInit::logOut << "Error: QtIntervalOp::evaluate() - interval bound must be of type integer or '*'." << endl;
                   parseInfo.setErrorNo(389);

                   // delete the old operands
                   if( operand1 ) operand1->deleteRef();
                   if( operand2 ) operand2->deleteRef();

                   throw parseInfo;
		 }
                 break;        

      case QT_ULONG:
      case QT_USHORT:
      case QT_CHAR: 
                 try{
                   sinterval.set_low( (r_Range)((QtAtomicData*)operand1)->getUnsignedValue() );
	         }
                 catch(...)
	         {
                   RMInit::logOut << "Error: QtIntervalOp::evaluate() - interval bound must be of type integer or '*'." << endl;
                   parseInfo.setErrorNo(389);

                   // delete the old operands
                   if( operand1 ) operand1->deleteRef();
                   if( operand2 ) operand2->deleteRef();

                   throw parseInfo;
		 }
                 break;        

      case QT_STRING: 
	QtStringData *p;
	p = dynamic_cast<QtStringData *>(operand1);

	if (p && (p->getStringData() == string("*")) ) 
		sinterval.set_low('*');
	else
 	{
              RMInit::logOut << "Error: QtIntervalOp::evaluate() - interval bound must be '*'." << endl;
              parseInfo.setErrorNo(389);
 
              // delete the old operands
              if( operand1 ) operand1->deleteRef();
              if( operand2 ) operand2->deleteRef();
 
              throw parseInfo;
        }       
        break;

      default:
        RMInit::logOut << "Error: QtIntervalOp::evaluate() - interval bound must be of type integer or '*'." << endl;
        parseInfo.setErrorNo(388);

        // delete the old operands
        if( operand1 ) operand1->deleteRef();
        if( operand2 ) operand2->deleteRef();

        throw parseInfo;
    }      


    switch( operand2->getDataType() )
    {
      case QT_LONG:
      case QT_SHORT:
      case QT_OCTET:       
                 try{
                   sinterval.set_high( (r_Range)(((QtAtomicData*)operand2)->getSignedValue()) );
	         }
                 catch(...)
	         {
                   RMInit::logOut << "Error: QtIntervalOp::evaluate() - interval bound must be of type integer or '*'." << endl;
                   parseInfo.setErrorNo(389);

                   // delete the old operands
                   if( operand1 ) operand1->deleteRef();
                   if( operand2 ) operand2->deleteRef();

                   throw parseInfo;
	         }
                 break;        

      case QT_ULONG:
      case QT_USHORT:
      case QT_CHAR:
                 try{
                   sinterval.set_high( (r_Range)((QtAtomicData*)operand2)->getUnsignedValue() );
	         }
                 catch(...)
	         {
                   RMInit::logOut << "Error: QtIntervalOp::evaluate() - interval bound must be of type integer or '*'." << endl;
                   parseInfo.setErrorNo(389);
                   // delete the old operands
                   if( operand1 ) operand1->deleteRef();
                   if( operand2 ) operand2->deleteRef();

                   throw parseInfo;
	         }
                 break;        

      case QT_STRING:
	QtStringData *p;
	p = dynamic_cast<QtStringData *>(operand2);

	if (p && (p->getStringData() == string("*")) ) 
		sinterval.set_high('*');
	else
 	{
              RMInit::logOut << "Error: QtIntervalOp::evaluate() - interval bound must be '*'." << endl;
              parseInfo.setErrorNo(389);
 
              // delete the old operands
              if( operand1 ) operand1->deleteRef();
              if( operand2 ) operand2->deleteRef();
 
              throw parseInfo;
        }       
        break;

      default:
        RMInit::logOut << "Error: QtIntervalOp::evaluate() - interval bound must be of type integer or '*'." << endl;
        parseInfo.setErrorNo(388);

        // delete the old operands
        if( operand1 ) operand1->deleteRef();
        if( operand2 ) operand2->deleteRef();

        throw parseInfo;
    }      

    returnValue = new QtIntervalData( sinterval );

    // delete the old operands
    if( operand1 ) operand1->deleteRef();
    if( operand2 ) operand2->deleteRef();
  }

  return returnValue;
}



void
QtIntervalOp::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtIntervalOp Object " << getNodeType() << endl;

  QtBinaryOperation::printTree( tab, s, mode );
}



void
QtIntervalOp::printAlgebraicExpression( ostream& s )
{
  s << "(";

  if( input1 )
    input1->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ":";

  if( input2 )
    input2->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";
}



const QtTypeElement&
QtIntervalOp::checkType( QtTypeTuple* typeTuple )
{
  RMDBCLASS( "QtIintervalOp", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

  dataStreamType.setDataType( QT_TYPE_UNKNOWN );  

  // check operand branches
  if( input1 && input2 )
  {

  const QtTypeElement& input1Type = input1->checkType( typeTuple );
  const QtTypeElement& input2Type = input2->checkType( typeTuple );

  bool opTypesValid = true;

  // check operand1
  opTypesValid &= input1Type.getDataType() == QT_STRING ||
                  input1Type.getDataType() == QT_LONG   ||
                  input1Type.getDataType() == QT_SHORT  ||
                  input1Type.getDataType() == QT_OCTET  ||
                  input1Type.getDataType() == QT_ULONG  ||
                  input1Type.getDataType() == QT_USHORT ||
                  input1Type.getDataType() == QT_CHAR; 

  // check operand2
  opTypesValid &= input2Type.getDataType() == QT_STRING ||
                  input2Type.getDataType() == QT_LONG   ||
                  input2Type.getDataType() == QT_SHORT  ||
                  input2Type.getDataType() == QT_OCTET  ||
                  input2Type.getDataType() == QT_ULONG  ||
                  input2Type.getDataType() == QT_USHORT ||
                  input2Type.getDataType() == QT_CHAR; 

  if( !opTypesValid )
  {
    RMInit::logOut << "Error: QtIntervalOp::evaluate() - interval bound must be of type integer or '*'." << endl;
    parseInfo.setErrorNo(389);
    throw parseInfo; 
  }

  dataStreamType.setDataType( QT_INTERVAL );
  }
  else
    RMInit::logOut << "Error: QtIntervalOp::checkType() - input branch invalid." << endl;

  return dataStreamType;
}
