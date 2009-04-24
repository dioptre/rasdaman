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

static const char rcsid[] = "@(#)qlparser, QtMDDAccess: $Id: qtmddaccess.cc,v 1.30 2002/04/23 16:22:41 purify Exp $";

#include "raslib/rmdebug.hh"

#include "qlparser/qtmddaccess.hh"
#include "qlparser/qtmdd.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif

#include "mddmgr/mddcoll.hh"
#include "mddmgr/mddcolliter.hh"

#include "servercomm/servercomm.hh"

extern ServerComm::ClientTblElt* currentClientTblElt;

const QtNode::QtNodeType QtMDDAccess::nodeType = QT_MDD_ACCESS;

QtMDDAccess::QtMDDAccess( const string& collectionNameNew )
  :  QtONCStream(),
     collectionName( collectionNameNew ),
     mddColl(NULL),
     mddIter(NULL)
{
}


QtMDDAccess::QtMDDAccess( const string& collectionNameNew, const string& initName )
  :  QtONCStream(),
     collectionName( collectionNameNew ),
     iteratorName( initName ),
     mddColl(NULL),
     mddIter(NULL)
{
}


QtMDDAccess::~QtMDDAccess()
{
 //just to be on the safe side
 close();
}


void
QtMDDAccess::open()
{
  RMDBCLASS( "QtMDDAccess", "open()", "qlparser", __FILE__, __LINE__ )

  // delete an existing iterator
  if( mddIter ) 
  {
    delete mddIter;
    mddIter=NULL;
  }

  // create the iterator
  mddIter = mddColl->createIterator();

  //for( mddIter->reset(); mddIter->notDone(); mddIter->advance() )
  //  mddIter->getElement()->printStatus();

  mddIter->reset();
}


QtNode::QtDataList*
QtMDDAccess::next()
{
  RMDBCLASS( "QtMDDAccess", "next()", "qlparser", __FILE__, __LINE__ )

  QtDataList* returnValue = NULL;
  MDDObj* ptr = NULL;

  if( mddColl && mddIter && mddIter->notDone() )
  {
    //
    // create a list with a pointer to the next element of the mdd collection
    //

    // encapsulate the next MDDObj in an QtMDD object
    ptr =  mddIter->getElement();
    QtMDD*  elem = new QtMDD( ptr, iteratorName );
    

    
    // create the list
    QtNode::QtDataList* dataList = new QtNode::QtDataList(1); // create container to contain one element

    // insert the element into the list
    (*dataList)[0] = elem;

    // if mddColl is not persistent delete thist from 
    // collection to avoid multiple destruction
    if(!mddColl->isPersistent()) {
       mddColl->remove(ptr);
       mddIter->reset();
    }
    else {
      // increment the iterator
      mddIter->advance();
    }

    returnValue = dataList;
  }

  return returnValue;
}


void
QtMDDAccess::close()
{
  RMDBCLASS( "QtMDDAccess", "close()", "qlparser", __FILE__, __LINE__ )

  // delete the mdd iterator
  if( mddIter ) 
  {
    delete mddIter; 
    mddIter=NULL;
  }

  // This is now done in ServerComm::ClientTblElt::releaseTransferStructures().
  //
  // delete the mdd objects and the mdd collection
  // if( mddColl )
  //  {
  //    mddColl->releaseAll();
  //    delete mddColl;
  //  }
}


void
QtMDDAccess::reset()
{
  if( mddIter ) mddIter->reset();
}


void
QtMDDAccess::printTree( int tab, ostream& s, QtChildType /*mode*/ )
{
  s << SPACE_STR(tab).c_str() << "QtMDDAccess Object: type " << flush;
  dataStreamType.printStatus( s );
  s << endl;

  s << SPACE_STR(tab).c_str() << collectionName.c_str()
    << " <- " << iteratorName.c_str() << endl;
}



void
QtMDDAccess::printAlgebraicExpression( ostream& s )
{
  s << collectionName.c_str() << " as " << iteratorName.c_str() << flush;
}



const QtTypeTuple&
QtMDDAccess::checkType()
{
  RMDBCLASS( "QtMDDAccess", "checkType()", "qlparser", __FILE__, __LINE__ )

  dataStreamType = QtTypeTuple(0);

  //
  // create the collection and add it to the list in the client table entry
  //

  try
  {
    mddColl = MDDColl::getMDDCollection( collectionName.c_str() );

	if( currentClientTblElt )
		{
		if (mddColl->isPersistent())
			{
			if( !(currentClientTblElt->persMDDCollections) )
				currentClientTblElt->persMDDCollections = new vector<MDDColl*>();

			currentClientTblElt->persMDDCollections->push_back( (MDDColl*)mddColl );
			}
		else	{
			currentClientTblElt->transferColl = mddColl;
			}
		}
	else	{
		RMInit::logOut << "Internal Error in QtMDDAccess::open(): No client context available" << endl;
		}
  }
  catch(...)
  {
    RMInit::logOut << "Error: QtMDDAccess::open() collection: " << collectionName.c_str() << " is unknown" << endl;
    parseInfo.setErrorNo(355);
    throw parseInfo;
  }

  CollectionType* collType = (CollectionType*) mddColl->getCollectionType();

  if( !collType )
    RMInit::logOut << "Internal error in QtMDDAccess::checkType() - no collection type available" << endl;

  dataStreamType = QtTypeTuple( 1 );
 
  dataStreamType.tuple[0].setType( collType->getMDDType() );
  dataStreamType.tuple[0].setName( iteratorName.c_str() );  

  return dataStreamType;
}
