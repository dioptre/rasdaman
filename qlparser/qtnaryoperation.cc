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

static const char rcsid[] = "@(#)qlparser, QtNaryOperation: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtnaryoperation.cc,v 1.13 2003/12/27 20:51:28 rasdev Exp $";

#include "qlparser/qtnaryoperation.hh"
#include "qlparser/qtconst.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif

const QtNode::QtNodeType QtNaryOperation::nodeType = QtNode::QT_NARY_OPERATION;

QtNaryOperation::QtNaryOperation()
    :  QtOperation(),
       operationList(NULL)
{
}


QtNaryOperation::QtNaryOperation( QtNode* node )
    :  QtOperation( node ),
       operationList(NULL)
{
}


QtNaryOperation::QtNaryOperation( QtOperationList* opList )
    :  QtOperation(),
       operationList( opList )
{
    if( operationList )
    {
        QtOperationList::iterator iter;

        for( iter=operationList->begin(); iter!=operationList->end(); iter++)
            (*iter)->setParent( this );
    }
}


QtNaryOperation::~QtNaryOperation()
{
    if( operationList )
    {
        QtOperationList::iterator iter;

        for( iter=operationList->begin(); iter!=operationList->end(); iter++)
        {
            delete *iter;
            *iter=NULL;
        }

        delete operationList;
        operationList=NULL;
    }
}


void
QtNaryOperation::simplify()
{
    RMDBCLASS( "QtNaryOperation", "simplify()", "qlparser", __FILE__, __LINE__ )

    // In order to work bottom up, first inspect the descendants
    QtNode::simplify();

    // Test, if all operands are available.
    bool success = (operationList != NULL);
    QtOperationList::iterator iter;

    if( success )
        for( iter=operationList->begin(); iter!=operationList->end(); iter++)
            if ((*iter) == NULL)
            {
                success = false;
                break;
            }

    if( success )
    {
        // Test, if all operands are of const type.
        for( iter=operationList->begin(); iter!=operationList->end(); iter++)
            if ((*iter)->getNodeType() != QT_CONST)
            {
                success = false;
                break;
            }

        if( success )
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



bool
QtNaryOperation::equalMeaning( QtNode* node )
{
    RMDBCLASS( "QtNaryOperation", "equalMeaning( QtNode* )", "qlparser", __FILE__, __LINE__ )

    bool result;
    result=false;

    if( getNodeType() == node->getNodeType() )
    {
        QtNaryOperation* naryNode;
        naryNode = (QtNaryOperation* ) node; // by force

        // get 2nd operation list
        QtOperationList* operationList2 = naryNode->getInputs();

        // create iterators
        QtOperationList::iterator iter, iter2;

        result = true;
        for( iter =operationList->begin(), iter2 =operationList2->begin();
                iter!=operationList->end() && iter2!=operationList2->end();
                iter++, iter2++ )
            if (!( (*iter)->equalMeaning( *iter2 ) ))
            {
                result=false;
                break;
            }

        // input lists must have the same length
        result &= ( iter==operationList->end() && iter2==operationList2->end() );
    };

    return result;
}



QtNode::QtNodeList*
QtNaryOperation::getChilds( QtChildType flag )
{
    RMDBCLASS( "QtNaryOperation", "getChilds( QtChildType )", "qlparser", __FILE__, __LINE__ )

    QtNodeList* resultList=NULL;
    QtNodeList* subList=NULL;

    QtOperationList::iterator iter;

    resultList = new QtNodeList();

    for( iter=operationList->begin(); iter!=operationList->end(); iter++ )
    {
        if( flag == QT_LEAF_NODES || flag == QT_ALL_NODES )
        {
            subList = (*iter)->getChilds( flag );
            resultList->splice(resultList->begin(), *subList);
            delete subList;
            subList=NULL;
        };

        if( flag == QT_DIRECT_CHILDS || flag == QT_ALL_NODES )
            resultList->push_back( *iter );
    };

    return resultList;
}


bool
QtNaryOperation::getOperands( QtDataList* inputList, QtDataList* &operandList )
{
    RMDBCLASS( "QtNaryOperation", "getOperands( QtDataList*, QtDataList* )", "qlparser", __FILE__, __LINE__ )

    bool success = (operationList != 0);

    // Test, if all operands are available.
    QtOperationList::iterator iter;

    if( success )
        for( iter=operationList->begin(); iter!=operationList->end(); iter++)
            if ((*iter) == NULL)
            {
                success=false;
                break;
            }

    if( success )
    {
        // get the operands
        operandList = new QtDataList( operationList->size() );

        int pos=0;

        for( iter=operationList->begin(); iter!=operationList->end(); iter++)
        {
            if( *iter )
                (*operandList)[pos] = (*iter)->evaluate( inputList );

            pos++;
        }

        // Test, if all operands are valid.
        for( pos=0; pos<operandList->size(); pos++ )
            if ((*operandList)[pos] == NULL)
            {
                success=false;
                break;
            }

        if( !success )
        {
            // if not all operands are valid, delete the old ones
            QtDataList::iterator dataIter;

            for( dataIter=operandList->begin(); dataIter!=operandList->end(); dataIter++ )
                if( (*dataIter) ) (*dataIter)->deleteRef();

            delete operandList;
            operandList = NULL;

            RMDBGONCE(2, RMDebug::module_qlparser, "QtNaryOperation", "Information: QtNaryOperation::getOperands() - at least one operand is not provided." )
        }
    }
    else
        RMInit::logOut << endl << "Error: QtNaryOperation::getOperands() - at least one operand branch is invalid." << endl;

    return success;
}



string
QtNaryOperation::getSpelling()
{
    QtOperationList::iterator iter;

    char tempStr[20];
    sprintf(tempStr, "%ud", (unsigned long)getNodeType());
    string result  = string(tempStr);

    result.append( "(" );

    for( iter=operationList->begin(); iter!=operationList->end(); iter++ )
    {
        if( iter!=operationList->begin() ) result.append( "," );

        result.append( (*iter)->getSpelling() );
    }

    result.append( ")" );

    RMDBGONCE(2, RMDebug::module_qlparser, "QtNaryOperation", "Result:" << result.c_str() )

    return result;
}



void
QtNaryOperation::setInput( QtOperation* inputOld, QtOperation* inputNew )
{
    QtOperationList::iterator iter;

    for( iter=operationList->begin(); iter!=operationList->end(); iter++)
    {
        if( *iter == inputOld )
        {
            (*iter) =  inputNew;

            if( inputNew )
                inputNew->setParent( this );
        };
    };
};



QtNode::QtAreaType
QtNaryOperation::getAreaType()
{
    return QT_AREA_SCALAR;
}

/*
void
QtNaryOperation::checkIdempotency()
{
  // default method: do nothing
}
*/

void
QtNaryOperation::optimizeLoad( QtTrimList* trimList )
{
    RMDBCLASS( "QtNaryOperation", "optimizeLoad( QtTrimList* )", "qlparser", __FILE__, __LINE__ )

    // delete trimList
    // release( trimList->begin(), trimList->end() );
    for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
    {
        delete *iter;
        *iter=NULL;
    }
    delete trimList;
    trimList=NULL;

    if( operationList )
    {
        QtOperationList::iterator iter;

        for( iter=operationList->begin(); iter!=operationList->end(); iter++)
            if( *iter )
                (*iter)->optimizeLoad( new QtNode::QtTrimList );
    }
}


void
QtNaryOperation::printTree( int tab, ostream& s, QtChildType mode )
{
    if( mode != QT_DIRECT_CHILDS )
    {
        if( operationList )
        {
            QtOperationList::iterator iter;
            int no;

            for( no=1, iter=operationList->begin(); iter!=operationList->end(); iter++, no++ )
                if( *iter )
                {
                    s << SPACE_STR(tab).c_str() << "input" << no << ": " << endl;
                    (*iter)->printTree( tab+2, s, mode );
                }
        }
    }
}



void
QtNaryOperation::printAlgebraicExpression( ostream& s )
{
    s << "(";

    if( operationList )
    {
        QtOperationList::iterator iter;

        for( iter=operationList->begin(); iter!=operationList->end(); iter++ )
        {
            if( iter!=operationList->begin() ) s << ",";

            if( *iter )
                (*iter)->printAlgebraicExpression( s );
            else
                s << "<nn>";
        }
    }
    else
        s << "<nn>";

    s << ")";
}



