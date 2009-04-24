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

static const char rcsid[] = "@(#)qlparser, QtOperation: $Id: qtoperation.cc,v 1.17 2002/06/05 18:18:17 coman Exp $";


#include "qlparser/qtoperation.hh"

#include <iostream>

const QtNode::QtNodeType QtOperation::nodeType = QtNode::QT_OPERATION;

QtOperation::QtOperation()
  :  QtNode()
{
}


QtOperation::QtOperation( QtNode* node )
  :  QtNode( node )
{
}


QtOperation*
QtOperation::getUniqueOrder( const QtNode::QtNodeType )
{
  // default method;

  return this;
}


QtData*
QtOperation::evaluate( QtDataList* /*inputList*/ )
{
  RMInit::logOut << "Error: Method evaluate(...) is undefined for a node in the query tree." << std::endl;
  return NULL;
}


void
QtOperation::optimizeLoad( QtTrimList* trimList )
{
  if( trimList )
  {
    std::vector<QtNode::QtTrimElement*>::iterator iter;
    for( iter=trimList->begin(); iter!=trimList->end(); iter++ )
    {
      delete *iter;
      *iter=NULL;
    }
    delete trimList;
    trimList=NULL;
  }

  RMInit::logOut << "Error: Method optimizeLoad(...) is undefined for a QtOperation node in the query tree." << std::endl;
}



const QtTypeElement&
QtOperation::checkType( QtTypeTuple* typeTuple )
{
  RMDBCLASS( "QtOperation", "checkType( QtTypeElement, QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

  dataStreamType.setDataType( QT_TYPE_UNKNOWN );

  return dataStreamType;
}


void
QtOperation::printTree( int tab, std::ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtOperation Object: type " << std::flush;
  dataStreamType.printStatus( s );
  s << std::endl;

  if( mode != QtNode::QT_DIRECT_CHILDS )
  {
      s << SPACE_STR(tab).c_str() << "no operation" << std::endl;
  }

}



void
QtOperation::printAlgebraicExpression( std::ostream& s )
{
  s << "op<";

    s << "no ops"; 

  s << ">";
}
