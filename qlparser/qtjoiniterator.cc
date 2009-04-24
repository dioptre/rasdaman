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


#include "raslib/rmdebug.hh"

#include "qlparser/qtjoiniterator.hh"
#include "qlparser/qtmdd.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif

const QtNode::QtNodeType QtJoinIterator::nodeType = QtNode::QT_JOIN_ITERATOR;


QtJoinIterator::QtJoinIterator()
  : QtIterator(),
    actualTupel(NULL),
    outputStreamIsEmpty(false)
{
}


QtJoinIterator::QtJoinIterator( QtNode* node )
  : QtIterator( node ),
    actualTupel(NULL),
    outputStreamIsEmpty(false)
{
}


QtJoinIterator::~QtJoinIterator()
{
  vector<QtData*>::iterator i; //default

  if( actualTupel )
  {
    // first delete still existing data carriers
    for( QtDataList::iterator iter=actualTupel->begin(); iter!=actualTupel->end(); iter++ )
      if( *iter ) (*iter)->deleteRef();

    delete actualTupel;
    actualTupel=NULL;
  }
}


void
QtJoinIterator::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtJoinIterator Object: type " << flush;
  dataStreamType.printStatus( s );
  s << endl;

  QtIterator::printTree( tab, s, mode );
}



void
QtJoinIterator::printAlgebraicExpression( ostream& s )
{
  s << "join";

  QtIterator::printAlgebraicExpression( s );
}


void
QtJoinIterator::open()
{
  RMDBCLASS( "QtJoinIterator", "open()", "qlparser", __FILE__, __LINE__ )

  QtIterator::open();

  outputStreamIsEmpty = false;  // initialization

  if( inputs )
  {
    // The idea of actualTupel initialization:
    // 
    //    tupel[0]  tupel[1]  tupel[2]  ...  |
    //    -----------------------------------------------------
    //       0         0         0           |  initial phase
    //       0        b1        c1           |  open 
    //      a1        b1        c1           |  next invocation
    //      a2        b1        c1           |        " 
    //      a1        b2        c1           |        "
    //       :         :         :           |        "

    // allocate an empty tupel, right now each input stream provides one data element
    actualTupel = new QtDataList( inputs->size() );

    // set the first element of the tupel to NULL
    (*actualTupel)[0] = NULL;

    // fill the tupel, except of the first element, with the first elements of the input streams
	//the first element is filled in the ::next() method
    for( int tupelPos=1; tupelPos<actualTupel->size(); tupelPos++ )
    {
      QtDataList* resultList = (*inputs)[tupelPos]->next();

      if( resultList )
      {
        // take the first data element of the input stream result
        (*actualTupel)[tupelPos] = (*resultList)[0];

        // delete the result vector (only the first data carriers is taken, the others are never deleted)
        delete resultList;
	resultList = NULL;
      }
      else
      {
	// In that case, one of the input streams is empty. Therefore, the output stream of
	// the self object is empty either.
	(*actualTupel)[tupelPos] = NULL;
	outputStreamIsEmpty = true;
      }
    }

    // Reset the first stream again, because the first tupel element is catched when next() is
    // called for the first time.
    // (*inputs)[0]->reset();
  }
}


QtNode::QtDataList*
QtJoinIterator::next()
{
  RMDBCLASS( "QtJoinIterator", "next()", "qlparser", __FILE__, __LINE__ )


  QtDataList* returnValue = NULL;

  if( inputs && actualTupel && !outputStreamIsEmpty )
  {
    bool        nextTupelAvailable = true;
    bool        nextTupelValid = false;
    int         tupelPos;
    QtDataList* resultList = NULL;
    QtONCStreamList::iterator iter;

    while( !nextTupelValid && nextTupelAvailable && !outputStreamIsEmpty )
    {
      // switch to the next tupel which means

      nextTupelAvailable = false;
      tupelPos           = 0;
      iter               = inputs->begin();

      while( !nextTupelAvailable && iter!=inputs->end() )
      {
        resultList = (*iter)->next();

        // Test, if the first input stream is empty, because this is not tested in open()
        if( resultList == NULL && tupelPos==0 && (*actualTupel)[0] == 0 )
          outputStreamIsEmpty = true;          

        if( resultList == NULL )
        {
          (*iter)->reset();              // reset the stream ...
	//this causes the first element of the list to be deleted - not the others
        resultList = (*iter)->next();  // ... and read the first element again
	// this was commented out because it will cause problems when the stream is closed
	// if it is commented out it will break join queries
        }
        else
          nextTupelAvailable = true;

        //
        // exchange the actual element in the tupel
        //
  
        //  delete the data carrier
        if( (*actualTupel)[tupelPos] )
        {
          (*actualTupel)[tupelPos]->deleteRef();
          (*actualTupel)[tupelPos] = NULL;
        }

        if( resultList )
        {
          // take the first data element of the input stream result - copy the data carrier pointer
          (*actualTupel)[tupelPos] = (*resultList)[0];

          // delete the result vector (only the first data carrier is taken, the others are never deleted)
          delete resultList;
          resultList = NULL;
        }

        iter++;
        tupelPos++;
      }

      if( nextTupelAvailable )
        nextTupelValid = true;
   }

   if( nextTupelAvailable )
   {
     // Copy the actual tupel in order to pass it as the next stream element
     // which means increase references to data elements.
     returnValue = new QtDataList( actualTupel->size() );

     for( tupelPos=0; tupelPos < actualTupel->size(); tupelPos++ )
       if( (*actualTupel)[tupelPos] )
       {
   	 (*returnValue)[tupelPos] = (*actualTupel)[tupelPos];
         (*actualTupel)[tupelPos]->incRef();
       }
       else
       {
	 // should not come here, because now the next tupel isn't valid

         // delete return value again
         for( tupelPos=0; tupelPos < returnValue->size(); tupelPos++ )
           if( (*returnValue)[tupelPos] )
             (*returnValue)[tupelPos]->deleteRef();

         delete returnValue;
         returnValue = NULL;

	 RMInit::logOut << "Internal Error in QtJoinIterator::next()" << endl;
       }
   }
  }

  return returnValue;
}



void
QtJoinIterator::close()
{
  RMDBCLASS( "QtJoinIterator", "close()", "qlparser", __FILE__, __LINE__ )

  if( actualTupel )
  {
    // first delete still existing data carriers
    for( QtDataList::iterator iter=actualTupel->begin(); iter!=actualTupel->end(); iter++ )
      if( *iter ) (*iter)->deleteRef();

    delete actualTupel;
    actualTupel = NULL;
  }

  QtIterator::close();
}


void
QtJoinIterator::reset()
{
  RMDBCLASS( "QtJoinIterator", "reset()", "qlparser", __FILE__, __LINE__ )

  // reset the input streams
  QtIterator::reset();

  if( inputs )
  {
    // first delete still existing data carriers
    for( QtDataList::iterator iter=actualTupel->begin(); iter!=actualTupel->end(); iter++ )
      if( *iter )
      {
        (*iter)->deleteRef();
	(*iter) = NULL;
      }

    // fill the tupel with the first elements of the input streams except of the first element
    for( int tupelPos=1; tupelPos<actualTupel->size(); tupelPos++ )
    {
      QtDataList* resultList = (*inputs)[tupelPos]->next();

      if( resultList )
      {
        // take the first data element of the input stream result
        (*actualTupel)[tupelPos] = (*resultList)[0];

        // delete the result vector (only the first data carriers is taken, the others are never deleted)
        delete resultList;
	resultList = NULL;
      }
      else
	(*actualTupel)[tupelPos] = NULL;

    }

    (*actualTupel)[0] = NULL;  // fist tupel element is catched when next() is called for the first time
  }
}



const QtTypeTuple&
QtJoinIterator::checkType()
{
  RMDBCLASS( "QtJoinIterator", "checkType()", "qlparser", __FILE__, __LINE__ )

  getInputTypeTuple( dataStreamType );

  return dataStreamType;
}
