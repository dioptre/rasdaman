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

static const char rcsid[] = "@(#)qlparser, QtOperationIterator: $Id: qtoperationiterator.cc,v 1.24 2001/08/07 12:36:49 barbat Exp $";


#include "raslib/rmdebug.hh"

#include "qlparser/qtoperationiterator.hh"
#include "qlparser/qtmdd.hh"
#include "qlparser/qtdata.hh"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

const QtNode::QtNodeType QtOperationIterator::nodeType = QT_OPERATION_ITERATOR;


QtOperationIterator::QtOperationIterator()
    : QtIterator()
{
    operationTreeList = new QtOperationList();
}


QtOperationIterator::QtOperationIterator( QtNode* node )
    : QtIterator( node )
{
    operationTreeList = new QtOperationList();
}


QtOperationIterator::~QtOperationIterator()
{
    // release( operationTreeList->begin(), operationTreeList->end() );
    QtOperationList::iterator iter;
    for( iter = operationTreeList->begin(); iter!=operationTreeList->end(); iter++ )
    {
        delete *iter;
        *iter=NULL;
    }
    delete operationTreeList;
    operationTreeList=NULL;
}


QtNode::QtNodeList*
QtOperationIterator::getChilds( QtChildType flag )
{
    RMDBCLASS( "QtOperationIterator", "getChilds( QtChildType )", "qlparser", __FILE__, __LINE__ )

    QtNodeList* resultList=NULL;
    QtNodeList* subList=NULL;

    QtOperationList::iterator iter;

    resultList = QtIterator::getChilds( flag );

    RMDBGIF(3, RMDebug::module_qlparser, "QtOperationIterator", \
            RMInit::dbgOut << "1. childs from stream subtree " << endl; \
            list<QtNode*>::iterator debugIter; \
            for( debugIter=resultList->begin(); debugIter!=resultList->end(); debugIter++ ) \
            (*debugIter)->printTree( 2, RMInit::dbgOut, QtNode::QT_DIRECT_CHILDS ); )

        for( iter=operationTreeList->begin(); iter!=operationTreeList->end(); iter++ )
        {
            if( flag == QT_LEAF_NODES || flag == QT_ALL_NODES )
            {
                subList = (*iter)->getChilds( flag ) ;

                RMDBGIF(3, RMDebug::module_qlparser, "QtOperationIterator", \
                        RMInit::dbgOut << "2. childs from operation subtree (without direct childs) " << endl; \
                        list<QtNode*>::iterator debugIter; \
                        for( debugIter=subList->begin(); debugIter!=subList->end(); debugIter++ ) \
                        (*debugIter)->printTree( 2, RMInit::dbgOut, QtNode::QT_DIRECT_CHILDS ); )

                    // remove all elements in subList and insert them at the beginning of resultList
                    resultList->splice( resultList->begin(), *subList );

                RMDBGIF(3, RMDebug::module_qlparser, "QtOperationIterator", \
                        RMInit::dbgOut << "3. merge of the lists " << endl; \
                        list<QtNode*>::iterator debugIter; \
                        for( debugIter=resultList->begin(); debugIter!=resultList->end(); debugIter++ ) \
                        (*debugIter)->printTree( 2, RMInit::dbgOut, QtNode::QT_DIRECT_CHILDS ); )

                    RMDBGIF(3, RMDebug::module_qlparser, "QtOperationIterator", \
                            RMInit::dbgOut << "4. old list (must be empty)" << endl; \
                            list<QtNode*>::iterator debugIter; \
                            for( debugIter=subList->begin(); debugIter!=subList->end(); debugIter++ ) \
                            (*debugIter)->printTree( 2, RMInit::dbgOut, QtNode::QT_DIRECT_CHILDS ); )

                        // delete temporary subList
                        delete subList;
                subList=NULL;
            };

            // add nodes of next level
            if( flag == QT_DIRECT_CHILDS || flag == QT_ALL_NODES )
                resultList->push_back( *iter );

            RMDBGIF(3, RMDebug::module_qlparser, "QtOperationIterator", \
                    RMInit::dbgOut << "4. current child list including direct childs " << endl; \
                    list<QtNode*>::iterator debugIter; \
                    for( debugIter=resultList->begin(); debugIter!=resultList->end(); debugIter++ ) \
                    (*debugIter)->printTree( 2, RMInit::dbgOut, QtNode::QT_DIRECT_CHILDS ); )

            };

    return resultList;
}


void
QtOperationIterator::printTree( int tab, ostream& s, QtChildType mode )
{
    QtOperationList::iterator iter;

    s << SPACE_STR(tab).c_str() << "QtOperationIterator Object: type " << flush;
    dataStreamType.printStatus( s );
    s << endl;

    if( mode != QtNode::QT_DIRECT_CHILDS )
    {
        if( operationTreeList->empty() )
            s << SPACE_STR(tab).c_str() << "no operation" << endl;
        else
            for( iter=operationTreeList->begin(); iter!=operationTreeList->end(); iter++ )
            {
                s << SPACE_STR(tab).c_str() << "operation: " << endl;
                (*iter)->printTree( tab + 2, s, mode );
            }
    }

    QtIterator::printTree( tab, s, mode );
}



void
QtOperationIterator::printAlgebraicExpression( ostream& s )
{
    s << "op<";

    if( operationTreeList )
    {
        for( int i=0; i<operationTreeList->size(); i++ )
        {
            (*operationTreeList)[i]->printAlgebraicExpression( s );

            if( i < operationTreeList->size()-1 )
                s << ", ";
        }
    }
    else
        s << "no ops";

    s << ">";

    QtIterator::printAlgebraicExpression( s );
}



QtNode::QtDataList*
QtOperationIterator::next()
{
    RMDBCLASS( "QtOperationIterator", "next()", "qlparser", __FILE__, __LINE__ )

    QtDataList* returnValue = NULL;

    if( inputs )
    {
        QtDataList* nextTupel=NULL;
        QtDataList* resultList=NULL;

        // create a composed tupel of type QtDataList of the next elements of the input streams
        // right now, just take the QtDataList vector of the first input stream
        nextTupel = (*inputs)[0]->next();

        if( nextTupel )
        {
            QtOperationList::iterator iter;
            vector<QtData*>::iterator dataIter;

            resultList = new QtDataList( operationTreeList->size() );

            int pos=0;

            for( iter=operationTreeList->begin(); iter!=operationTreeList->end(); iter++)
            {
                // send them through the operand tree

                try
                {
                    if( *iter )
                        (*resultList)[pos] = (*iter)->evaluate( nextTupel );
                }
                catch(...)
                {
                    // Delete the tupel vector received by next(). Just tupel elements which are not
                    // further referenced are deleted.
                    for( dataIter=nextTupel->begin(); dataIter!=nextTupel->end(); dataIter++ )
                        if( (*dataIter) ) (*dataIter)->deleteRef();

                    for (QtDataList::iterator deleteIter = resultList->begin(); deleteIter != resultList->end(); deleteIter++)
                    {
                        delete *deleteIter;
                        *deleteIter = NULL;
                    }
                    delete resultList;
                    resultList = NULL;
                    delete nextTupel;
                    nextTupel = NULL;

                    throw;
                }

                pos++;
            }

            // Delete the tupel vector received by next(). Just tupel elements which are not
            // further referenced are deleted.
            for( dataIter=nextTupel->begin(); dataIter!=nextTupel->end(); dataIter++ )
                if( (*dataIter) ) (*dataIter)->deleteRef();

            // ... and now the vector itself
            delete nextTupel;
            nextTupel=NULL;

            returnValue = resultList;
        }
    }
    return returnValue;
}

/*
void
QtOperationIterator::preOptimize()
{
  QtOperationList::iterator iter;

  for( iter=operationTreeList->begin(); iter!=operationTreeList->end(); iter++ )
   (*iter)->optimizeLoad( new QtNode::QtTrimList );

  // pass it to the input streams
  QtIterator::preOptimize();
};
*/


const QtTypeTuple&
QtOperationIterator::checkType()
{
    RMDBCLASS( "QtOperationIterator", "checkType()", "qlparser", __FILE__, __LINE__ )

    dataStreamType = QtTypeTuple();

    QtTypeTuple inputTypeTuple;

    getInputTypeTuple( inputTypeTuple );

    // type check of operation trees
    QtOperationList::iterator iter;

    for( iter=operationTreeList->begin(); iter!=operationTreeList->end(); iter++ )
        if( *iter )
            dataStreamType.concat( (*iter)->checkType( &inputTypeTuple ) );

    return dataStreamType;
}


