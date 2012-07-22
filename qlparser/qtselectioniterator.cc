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

static const char rcsid[] = "@(#)qlparser, QtSelectionIterator: $Id: qtselectioniterator.cc,v 1.24 2001/08/07 12:00:12 barbat Exp $";

#include "raslib/rmdebug.hh"

#include "qlparser/qtselectioniterator.hh"
#include "qlparser/qtatomicdata.hh"

#include "relcatalogif/type.hh"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

const QtNode::QtNodeType QtSelectionIterator::nodeType = QtNode::QT_SELECTION_ITERATOR;


QtSelectionIterator::QtSelectionIterator()
    : QtIterator(),
      conditionTree(NULL)
{
}


QtSelectionIterator::QtSelectionIterator( QtNode* node )
    : QtIterator( node ),
      conditionTree(NULL)
{
}


QtSelectionIterator::~QtSelectionIterator()
{
    if( conditionTree )
    {
        delete conditionTree;
        conditionTree=NULL;
    }
}


QtNode::QtNodeList*
QtSelectionIterator::getChilds( QtChildType flag )
{
    QtNodeList* resultList=NULL;
    QtNodeList* subList=NULL;

    RMDBGENTER( 1, RMDebug::module_qlparser, "QtSelectionIterator", " QtSelectionIterator::getChilds() ")

    resultList = QtIterator::getChilds( flag );

    RMDBGIF( 4, RMDebug::module_qlparser, "QtSelectionIterator", \
             RMInit::dbgOut << "1. childs from stream subtree " << endl; \
             list<QtNode*>::iterator debugIter; \
             for( debugIter=resultList->begin(); debugIter!=resultList->end(); debugIter++ ) \
             (*debugIter)->printTree( 2, RMInit::dbgOut, QtNode::QT_DIRECT_CHILDS ); \
           )

        if( conditionTree )
        {
            if( flag == QT_LEAF_NODES || flag == QT_ALL_NODES )
            {
                subList = conditionTree->getChilds( flag );

                RMDBGIF( 4, RMDebug::module_qlparser, "QtSelectionIterator", \
                         RMInit::dbgOut << "2. childs from operation subtree (without direct childs) " << endl; \
                         list<QtNode*>::iterator debugIter; \
                         for( debugIter=subList->begin(); debugIter!=subList->end(); debugIter++ ) \
                         (*debugIter)->printTree( 2, RMInit::dbgOut, QtNode::QT_DIRECT_CHILDS ); \
                       )

                    // remove all elements in subList and insert them at the beginning in resultList
                    resultList->splice( resultList->begin(), *subList );

                RMDBGIF( 4, RMDebug::module_qlparser, "QtSelectionIterator", \
                         RMInit::dbgOut << "3. merge of the lists " << endl;  \
                         list<QtNode*>::iterator debugIter; \
                         for( debugIter=resultList->begin(); debugIter!=resultList->end(); debugIter++ ) \
                         (*debugIter)->printTree( 2, RMInit::dbgOut, QtNode::QT_DIRECT_CHILDS ); \
                       )


                    RMDBGIF( 4, RMDebug::module_qlparser, "QtSelectionIterator", \
                             RMInit::dbgOut << "4. old list (must be empty)" << endl; \
                             list<QtNode*>::iterator debugIter; \
                             for( debugIter=subList->begin(); debugIter!=subList->end(); debugIter++ ) \
                             (*debugIter)->printTree( 2, RMInit::dbgOut, QtNode::QT_DIRECT_CHILDS ); \
                           )

                        // delete temporary subList
                        delete subList;
                subList=NULL;
            };

            // add nodes of next level
            if( flag == QT_DIRECT_CHILDS || flag == QT_ALL_NODES )
                resultList->push_back( conditionTree );

            RMDBGIF( 4, RMDebug::module_qlparser, "QtSelectionIterator",
                     RMInit::dbgOut << "4. current child list including direct childs " << endl; \
                     list<QtNode*>::iterator debugIter; \
                     for( debugIter=resultList->begin(); debugIter!=resultList->end(); debugIter++ ) \
                     (*debugIter)->printTree( 2, RMInit::dbgOut, QtNode::QT_DIRECT_CHILDS ); \
                   )

            };

    return resultList;
}


void
QtSelectionIterator::printTree( int tab, ostream& s, QtChildType mode )
{
    s << SPACE_STR(tab).c_str() << "QtSelectionIterator Object: type " << flush;
    dataStreamType.printStatus( s );
    s << endl;

    if( mode != QtNode::QT_DIRECT_CHILDS )
    {
        if( conditionTree )
        {
            s << SPACE_STR(tab).c_str() << "condition : " << endl;
            conditionTree->printTree( tab + 2, s );
        }
        else
            s << SPACE_STR(tab).c_str() << "no condition" << endl;
    }

    QtIterator::printTree( tab, s, mode );
}



void
QtSelectionIterator::printAlgebraicExpression( ostream& s )
{
    s << "sel<";

    conditionTree->printAlgebraicExpression( s );

    s << ">";

    QtIterator::printAlgebraicExpression( s );
}



QtNode::QtDataList*
QtSelectionIterator::next()
{
    RMDBGENTER( 1, RMDebug::module_qlparser, "QtSelectionIterator", "QtSelectionIterator::next()" )

    QtDataList* returnValue = NULL;

    if( inputs )
    {
        bool        nextTupelValid = false;
        QtDataList* actualTupel=NULL;

        while( !nextTupelValid )
        {
            actualTupel = (*inputs)[0]->next();

            if( actualTupel )
            {
                if( conditionTree )
                {
                    // evaluate the condition tree
                    QtData* resultData = conditionTree->evaluate( actualTupel );

                    if( resultData )
                    {
                        if( resultData->getDataType() == QT_BOOL )
                            nextTupelValid = (bool)((QtAtomicData*)resultData)->getUnsignedValue();
                        else
                        {
                            RMInit::logOut << "Error: QtSelectionIterator::next() - result of the WHERE part must be of type Bool." << endl;
                            parseInfo.setErrorNo(359);
                            throw parseInfo;
                        }

                        resultData->deleteRef();

                        if( !nextTupelValid )
                        {
                            // delete transient objects
                            vector<QtData*>::iterator iter;

                            for( iter=actualTupel->begin(); iter!=actualTupel->end(); iter++)
                                if( *iter ) (*iter)->deleteRef();

                            // delete vector itself
                            delete actualTupel;
                            actualTupel = NULL;
                        };
                    }
                }
                else
                    nextTupelValid = true;
            }
            else
                break;
        }

        returnValue = actualTupel;
    }

    RMDBGEXIT( 1, RMDebug::module_qlparser, "QtSelectionIterator", "QtSelectionIterator::next()" )

    return returnValue;
}

/*
inline void
QtSelectionIterator::preOptimize()
{
  if( conditionTree )
    conditionTree->optimizeLoad( new QtNode::QtTrimList );

  // pass it to the input streams
  QtIterator::preOptimize();
};
*/

const QtTypeTuple&
QtSelectionIterator::checkType()
{
    RMDBCLASS( "QtSelectionIterator", "checkType()", "qlparser", __FILE__, __LINE__ )

    // concatenate types of inputs
    getInputTypeTuple( dataStreamType );

    // type check for condition tree
    if( conditionTree )
    {
        const QtTypeElement& type = conditionTree->checkType( (QtTypeTuple*)&dataStreamType );

        if( type.getDataType() != QT_BOOL )
        {
            RMInit::logOut << "Error: QtSelectionIterator::next() - result of the WHERE part must be of type Bool." << endl;
            parseInfo.setErrorNo(359);
            throw parseInfo;
        }
    }

    // pass type tuple
    return dataStreamType;
}
