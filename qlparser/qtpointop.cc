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

static const char rcsid[] = "@(#)qlparser, QtPointOp: $Id: qtpointop.cc,v 1.7 2002/06/05 18:18:17 coman Exp $";

#include "raslib/rmdebug.hh"

#include "qlparser/qtpointop.hh"
#include "qlparser/qtdata.hh"
#include "qlparser/qtpointdata.hh"
#include "qlparser/qtatomicdata.hh"

#include "catalogmgr/ops.hh"
#include "relcatalogif/type.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif


const QtNode::QtNodeType QtPointOp::nodeType = QT_POINTOP;

QtPointOp::QtPointOp( QtOperationList* opList )
  :  QtNaryOperation( opList )
{
}



QtData*
QtPointOp::evaluate( QtDataList* inputList )
{
  RMDBCLASS( "QtPointOp", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

  QtData*     returnValue = NULL;
  QtDataList* operandList = NULL;

  if( getOperands( inputList, operandList ) )
  {
    vector<QtData*>::iterator dataIter;
    bool              goOn=true;

    if( operandList )
    {
      // first check operand types
      for( dataIter=operandList->begin(); dataIter!=operandList->end() && goOn; dataIter++ )
        if (!( (*dataIter)->getDataType() == QT_SHORT || (*dataIter)->getDataType() == QT_USHORT ||
                (*dataIter)->getDataType() == QT_LONG  || (*dataIter)->getDataType() == QT_ULONG  ||
                (*dataIter)->getDataType() == QT_OCTET || (*dataIter)->getDataType() == QT_CHAR ))
        {
          goOn=false;
          break;
        }

      if( !goOn )
      {
        RMInit::logOut << "Error: QtPointOp::evaluate() - operands of point expression must be of type integer." << std::endl;

        parseInfo.setErrorNo(410);

        // delete the old operands
        if( operandList )
        {
          for( dataIter=operandList->begin(); dataIter!=operandList->end(); dataIter++ )
            if( (*dataIter) ) (*dataIter)->deleteRef();

          delete operandList;
          operandList=NULL;
        }

        throw parseInfo;
      }

      //
      // create a QtPointData object and fill it
      //
      r_Point pt( operandList->size() );

      for( dataIter=operandList->begin(); dataIter!=operandList->end(); dataIter++ )
        if( (*dataIter)->getDataType() == QT_SHORT || 
            (*dataIter)->getDataType() == QT_LONG  || 
            (*dataIter)->getDataType() == QT_OCTET )
          pt << ((QtAtomicData*)(*dataIter))->getSignedValue();
        else
          pt << ((QtAtomicData*)(*dataIter))->getUnsignedValue();
    
      returnValue = new QtPointData( pt ); 

      // delete the old operands
      if( operandList )
      {
        for( dataIter=operandList->begin(); dataIter!=operandList->end(); dataIter++ )
          if( (*dataIter) ) (*dataIter)->deleteRef();

        delete operandList;
        operandList=NULL;
      }
    }
  }

  return returnValue;
}



void
QtPointOp::printTree( int tab, std::ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtPointOp Object " << getNodeType() << std::endl;

  QtNaryOperation::printTree( tab, s, mode );
}



void
QtPointOp::printAlgebraicExpression( std::ostream& s )
{
  s << "[";
 
  QtNaryOperation::printAlgebraicExpression( s );
 
  s << "]";
}



const QtTypeElement&
QtPointOp::checkType( QtTypeTuple* typeTuple )
{
  RMDBCLASS( "QtPointOp", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

  dataStreamType.setDataType( QT_TYPE_UNKNOWN );  

  QtOperationList::iterator iter;
  bool              opTypesValid = true;
    
  for( iter=operationList->begin(); iter!=operationList->end() && opTypesValid; iter++ )
  {
    const QtTypeElement& type = (*iter)->checkType( typeTuple );

    // valid types: integers
    if (!(  type.getDataType() == QT_SHORT    ||
            type.getDataType() == QT_LONG     ||
            type.getDataType() == QT_OCTET    ||
            type.getDataType() == QT_USHORT   ||
            type.getDataType() == QT_ULONG    ||
            type.getDataType() == QT_CHAR))
      {
        opTypesValid = false;
        break;
      }
  }

  if( !opTypesValid )
  {
    RMInit::logOut << "Error: QtPointOp::checkType() - operand of point expression must be of type integer." << std::endl;
    parseInfo.setErrorNo(410);
    throw parseInfo; 
  }

  dataStreamType.setDataType( QT_POINT );

  return dataStreamType;
}
















