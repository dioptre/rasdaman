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
/
/*************************************************************
 *
 * 
 *
 *
 * COMMENTS:
 *
 ************************************************************/

static const char rcsid[] = "@(#)qlparser, QLMarrayOp, QLCondenseOp: $Header: /home/rasdev/CVS-repository/rasdaman/catalogmgr/algebraops.cc,v 1.5 2003/12/20 23:41:27 rasdev Exp $";

#include "raslib/rmdebug.hh"

#include "algebraops.hh"

#include "qlparser/qtoperation.hh"
#include "qlparser/qtdata.hh"
#include "qlparser/qtpointdata.hh"
#include "qlparser/qtscalardata.hh"
#include "qlparser/qtatomicdata.hh"

#include "relcatalogif/basetype.hh"

QLMarrayOp::QLMarrayOp( QtOperation*     newCellExpression,
                        std::vector<QtData*>* newDataList,
                        std::string           &newIteratorName,
                        BaseType*        newResType,
                        unsigned int     newResOff        ) :
     MarrayOp( newResType, newResOff ),
     cellExpression( newCellExpression ),
     dataList( newDataList ),
     iteratorName( newIteratorName )
{
}



QLMarrayOp::~QLMarrayOp()
{
}



void
QLMarrayOp::operator() ( char *result, const r_Point &p )
{
  // update point data of input list
  if ( dataList ) 
    ((QtPointData *)dataList->back())->setPointData( p );

  if ( cellExpression ) {
    QtData* resultData = cellExpression->evaluate( dataList );

    if( resultData ) {
      if( resultData->isScalarData() ) {
	QtScalarData* scalarResultData = (QtScalarData*)resultData;
	memcpy( (void*)result,
        	(void*)scalarResultData->getValueBuffer(),
        	scalarResultData->getValueType()->getSize() );
      }
      else
	RMInit::logOut << "Internal Error: QLMarrayOp::operator() - cell type invalid." << endl;
      resultData->deleteRef();
    }
  }
}




QLCondenseOp::QLCondenseOp( QtOperation*     newCellExpression,
                            QtOperation*     newCondExpression,
                            std::vector<QtData*>* newDataList,
                            std::string           &newIteratorName,
                            BaseType*        newResType,
	                    unsigned int     newResOff,
                            BinaryOp*        newAccuOp,
                            char*            newInitVal          )

  :  GenCondenseOp( newResType, newResOff, newAccuOp, newInitVal ),
     cellExpression( newCellExpression ),
     condExpression( newCondExpression ),
     dataList( newDataList ),
     iteratorName( newIteratorName )
{
  //
  // add point with its iterator name to the data list  
  //

  // create QtPointData object 
  QtPointData* pointData = new QtPointData( r_Point() );

  // set its iterator name
  pointData->setIteratorName( iteratorName );

  // add it to the list
  dataList->push_back( pointData );
}



QLCondenseOp::~QLCondenseOp()
{
  // remove point data object from inputList again
  dataList->back()->deleteRef();
  dataList->pop_back();
}



void
QLCondenseOp::operator() ( const r_Point& p )
{
  unsigned int currentCellValid = 1;

  // update point data of input list
  if ( dataList )
    ((QtPointData*)dataList->back())->setPointData( p );

  if ( condExpression ) {
    QtData* condData = condExpression->evaluate( dataList );
#ifdef QT_RUNTIME_TYPE_CHECK
    if( condData->getDataType() != QT_BOOL ) {
      RMInit::logOut << "Internal error in QLCondenseOp::operator() - "
                       << "runtime type checking failed (BOOL)." << endl; 
    }
    else
#endif
    currentCellValid = ((QtAtomicData*)condData)->getUnsignedValue();    
    condData->deleteRef();
  }

  if ( currentCellValid ) {
    QtData* resultData = cellExpression->evaluate( dataList );
    if ( resultData ) {
#ifdef QT_RUNTIME_TYPE_CHECK
      if ( !(resultData->isScalarData()) ) {
        RMInit::logOut << "Internal Error: QLCondenseOp::operator() - cell type invalid." << endl;
      }
      else
#endif
      {
        QtScalarData* scalarResultData = (QtScalarData*)resultData;
        (*accuOp)( initVal, initVal, scalarResultData->getValueBuffer() );
      }

      resultData->deleteRef();
    }
  }
}
