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

static const char rcsid[] = "@(#)qlparser, QtOId: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtoid.cc,v 1.13 2003/12/27 20:51:28 rasdev Exp $";

#include "qlparser/qtoid.hh"
#include "qlparser/qtvariable.hh"
#include "qlparser/qtatomicdata.hh"
#include "qlparser/qtmdd.hh"

#include "mddmgr/mddobj.hh"

#include "raslib/oid.hh"

const QtNode::QtNodeType QtOId::nodeType = QtNode::QT_OID;


QtOId::QtOId( QtVariable* newInput )
  : QtUnaryOperation( newInput )
{
  RMDBCLASS( "QtOId", "QtOId( QtVariable* )", "qlparser", __FILE__, __LINE__ )
}



QtData*
QtOId::evaluate( QtDataList* inputList )
{
  RMDBCLASS( "QtOId", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

  QtData* returnValue = NULL;
  QtData* operand = NULL;

  operand = input->evaluate( inputList );

  if( operand )
  {
#ifdef QT_RUNTIME_TYPE_CHECK
    if( operand->getDataType() == QT_MDD )
    {
      RMInit::logOut << "Internal error in QtOId::evaluate() - "
                     << "runtime type checking failed (MDD)." << std::endl; 

      // delete old operand
      if( operand ) operand->deleteRef();

      return 0;
    }    
#endif

    QtMDD*  qtMDD  = (QtMDD*) operand;
    MDDObj* mddObj = qtMDD->getMDDObject();

    if( mddObj->isPersistent() )
    {
      MDDObj* persMDD = (MDDObj*) mddObj;

      // get local oid and pass it as double
      OId localOId;
      if( !persMDD->getOId( &localOId ) )
      {
        RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtOid", "  oid = " << (double)localOId  )

        returnValue = new QtAtomicData( (double)localOId, (unsigned short)8 );
      }
      else
      {
        RMInit::logOut << "Error: QtOId::evaluate() - could not get oid." << std::endl;

        // delete old operand
        if( operand ) operand->deleteRef();

        parseInfo.setErrorNo(384);
        throw parseInfo;
      }
    }
    else
    {
      RMInit::logOut << "Error: QtOId::evaluate() - operand is not a persistent MDD." << std::endl;

      // delete old operand
      if( operand ) operand->deleteRef();
      parseInfo.setErrorNo(383);
      throw parseInfo;
    }

    // delete old operand
    if( operand ) operand->deleteRef();
  }
  else
    RMInit::logOut << "Error: QtOId::evaluate() - operand is not provided." << std::endl;

  return returnValue;
}



void
QtOId::printTree( int tab, std::ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtOId Object: " << std::endl;

  QtUnaryOperation::printTree( tab, s, mode );
}



void
QtOId::printAlgebraicExpression( std::ostream& s )
{
  s << "oid(" << std::flush;

  if( input )
    input->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";  
}



const QtTypeElement&
QtOId::checkType( QtTypeTuple* typeTuple )
{
  RMDBCLASS( "QtOId", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

  dataStreamType.setDataType( QT_TYPE_UNKNOWN );  

  // check operand branches
  if( input )
  {

  // get input type
  const QtTypeElement& inputType = input->checkType( typeTuple ); 

  if( inputType.getDataType() != QT_MDD )
  {
    RMInit::logOut << "Error: QtOId::checkType() - operand is not of type MDD." << std::endl;
    parseInfo.setErrorNo(383);
    throw parseInfo;
  }

  dataStreamType.setDataType( QT_DOUBLE );
  }
  else
    RMInit::logOut << "Error: QtOId::checkType() - operand branch invalid." << std::endl;

  return dataStreamType;
}
