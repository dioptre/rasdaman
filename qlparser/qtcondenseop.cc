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

static const char rcsid[] = "@(#)qlparser, QtCondenseOp: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtcondenseop.cc,v 1.18 2003/12/27 20:51:28 rasdev Exp $";

#include "raslib/rmdebug.hh"

#include "qlparser/qtcondenseop.hh"
#include "qlparser/qtdata.hh"
#include "qlparser/qtmdd.hh"
#include "qlparser/qtpointdata.hh"
#include "qlparser/qtmintervaldata.hh"
#include "qlparser/qtatomicdata.hh"
#include "qlparser/qtcomplexdata.hh"

#include "mddmgr/mddobj.hh"

#include "catalogmgr/typefactory.hh"

#include "catalogmgr/algebraops.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif


const QtNode::QtNodeType QtCondenseOp::nodeType = QT_CONDENSEOP;

QtCondenseOp::QtCondenseOp( Ops::OpType  newOperation,
                            const string &initIteratorName,
                            QtOperation* mintervalExp,
                            QtOperation* cellExp,
                            QtOperation* condExp             )
    :  operation( newOperation ),
       iteratorName( initIteratorName ),
       QtBinaryOperation( mintervalExp, cellExp ),
       condOp( condExp )
{
}



QtCondenseOp::~QtCondenseOp()
{
    if( condOp )
    {
        delete condOp;
        condOp=NULL;
    }
}



QtNode::QtNodeList*
QtCondenseOp::getChilds( QtChildType flag )
{
    RMDBCLASS( "QtCondenseOp", "getChilds( QtChildType )", "qlparser", __FILE__, __LINE__ )
    QtNodeList* resultList;
    resultList = QtBinaryOperation::getChilds( flag );
    if( condOp )
    {
        if( flag == QT_LEAF_NODES || flag == QT_ALL_NODES )
        {
            QtNodeList* subList=NULL;
            subList = condOp->getChilds( flag );
            // remove all elements in subList and insert them at the beginning in resultList
            resultList->splice( resultList->begin(), *subList );
            // delete temporary subList
            delete subList;
            subList=NULL;
        }

        // add the nodes of the current level
        if( flag == QT_DIRECT_CHILDS || flag == QT_ALL_NODES )
            resultList->push_back( condOp );
    }

    return resultList;
}



bool
QtCondenseOp::equalMeaning( QtNode* node )
{
    RMDBCLASS( "QtCondenseOp", "equalMeaning( QtNode* )", "qlparser", __FILE__, __LINE__ )

    bool result = false;

    if( nodeType == node->getNodeType() )
    {
        QtCondenseOp* condNode;
        condNode = (QtCondenseOp*) node; // by force

        // check domain and cell expression
        result  = QtBinaryOperation::equalMeaning( condNode );

        // check condition expression
        result &= ( !condOp && !condNode->getCondOp() ) ||
                  condOp->equalMeaning( condNode->getCondOp() );
    };

    return ( result );
}



string
QtCondenseOp::getSpelling()
{
    char tempStr[20];
    sprintf(tempStr, "%ud", (unsigned long)getNodeType());
    string result  = string(tempStr);
    result.append( "(" );
    result.append( QtBinaryOperation::getSpelling() );
    result.append( "," );

    if( condOp )
        result.append( condOp->getSpelling() );
    else
        result.append( "<nn>" );

    result.append( ")" );

    return result;
}


void
QtCondenseOp::setInput( QtOperation* inputOld, QtOperation* inputNew )
{
    QtBinaryOperation::setInput( inputOld, inputNew );

    if( condOp == inputOld )
    {
        condOp = inputNew;

        if( inputNew )
            inputNew->setParent( this );
    }
};



void
QtCondenseOp::optimizeLoad( QtTrimList* trimList )
{
    RMDBCLASS( "QtCondenseOp", "optimizeLoad( QtTrimList* )", "qlparser", __FILE__, __LINE__ )

    // delete the trimList and optimize subtrees

    // release( trimList->begin(), trimList->end() );
    for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
    {
        delete *iter;
        *iter=NULL;
    }
    delete trimList;
    trimList=NULL;

    QtBinaryOperation::optimizeLoad( new QtNode::QtTrimList() );

    if( condOp )
        condOp->optimizeLoad( new QtNode::QtTrimList() );
}


void
QtCondenseOp::simplify()
{
    RMDBCLASS( "QtCondenseOp", "simplify()", "qlparser", __FILE__, __LINE__ )

    RMDBGMIDDLE(1, RMDebug::module_qlparser, "QtCondenseOp", "simplify() warning: QtCondenseOp itself is not simplified yet")

    // Default method for all classes that have no implementation.
    // Method is used bottom up.

    QtNodeList* resultList=NULL;
    QtNodeList::iterator iter;

    resultList = getChilds( QT_DIRECT_CHILDS );
    for( iter=resultList->begin(); iter!=resultList->end(); iter++ )
        (*iter)->simplify();

    delete resultList;
    resultList=NULL;
}


bool
QtCondenseOp::isCommutative() const
{
    return false; // NOT commutative
}



QtData*
QtCondenseOp::evaluate( QtDataList* inputList )
{
    RMDBCLASS( "QtCondenseOp", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

    QtData* returnValue = NULL;
    QtData* operand1 = NULL;

    if( getOperand( inputList, operand1, 1 ) )
    {

#ifdef QT_RUNTIME_TYPE_CHECK
        if( operand1->getDataType() != QT_MINTERVAL )
            RMInit::logOut << "Internal error in QtMarrayOp::evaluate() - "
                           << "runtime type checking failed (Minterval)." << endl;

        // delete old operand
        if( operand1 ) operand1->deleteRef();

        return 0;
    }
#endif

    r_Minterval domain = ((QtMintervalData*)operand1)->getMintervalData();

    RMDBGMIDDLE(1, RMDebug::module_qlparser, "QtCondenseOp", "Marray domain " << domain)

    // determine aggregation type
    BaseType* cellType = (BaseType*) input2->getDataStreamType().getType();

    // get operation object
    BinaryOp* cellBinOp = Ops::getBinaryOp( operation, cellType, cellType, cellType );

    // create execution object QLCondenseOp
    QLCondenseOp* qlCondenseOp = new QLCondenseOp( input2, condOp, inputList, iteratorName,
            cellType, 0, cellBinOp );

    // result buffer
    char* result=NULL;

    try
    {
        // execute query engine marray operation
        result = Tile::execGenCondenseOp( qlCondenseOp, domain );
    }
    catch(...)
    {
        // free ressources
        delete qlCondenseOp;
        qlCondenseOp=NULL;
        delete cellBinOp;
        cellBinOp=NULL;
        if( operand1 ) operand1->deleteRef();

        throw;
    }

    // allocate cell buffer
    char* resultBuffer = new char[ cellType->getSize() ];

    // copy cell content
    memcpy( (void*)resultBuffer, (void*)result, cellType->getSize() );

    delete qlCondenseOp;
    qlCondenseOp=NULL;
    delete cellBinOp;
    cellBinOp=NULL;

    // create data object for the cell
    QtScalarData* scalarDataObj = NULL;
    if( cellType->getType() == STRUCT )
        scalarDataObj = new QtComplexData();
    else
        scalarDataObj = new QtAtomicData();

    scalarDataObj->setValueType  ( cellType );
    scalarDataObj->setValueBuffer( resultBuffer );

    // set return data object
    returnValue = scalarDataObj;

    // delete old operands
    if( operand1 ) operand1->deleteRef();
}

return returnValue;
}



void
QtCondenseOp::printTree( int tab, ostream& s, QtChildType mode )
{
    s << SPACE_STR(tab).c_str() << "QtCondenseOp Object " << getNodeType() << endl;

    s << SPACE_STR(tab).c_str() << "Iterator Name: " << iteratorName.c_str() << endl;

    QtBinaryOperation::printTree( tab, s, mode );
}



void
QtCondenseOp::printAlgebraicExpression( ostream& s )
{
    s << "(";

    s << iteratorName.c_str() << ",";

    if( input1 )
        input1->printAlgebraicExpression( s );
    else
        s << "<nn>";

    s << ",";

    if( input2 )
        input2->printAlgebraicExpression( s );
    else
        s << "<nn>";

    s << ")";
}



const QtTypeElement&
QtCondenseOp::checkType( QtTypeTuple* typeTuple )
{
    RMDBCLASS( "QtCondenseOp", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

    dataStreamType.setDataType( QT_TYPE_UNKNOWN );

    // check operand branches
    if( input1 && input2 )
    {

        // check domain expression
        const QtTypeElement& domainExp = input1->checkType( typeTuple );

        if( domainExp.getDataType() != QT_MINTERVAL )
        {
            RMInit::logOut << "Error: QtMarrayOp::checkType() - Can not evaluate domain expression to an minterval" << endl;
            parseInfo.setErrorNo(401);
            throw parseInfo;
        }

        // add domain iterator to the list of bounded variables
        bool newList = false;
        if( !typeTuple )
        {
            typeTuple = new QtTypeTuple();
            newList = true;
        }
        typeTuple->tuple.push_back( QtTypeElement( QT_POINT, iteratorName.c_str() ) );

        //
        // check value expression
        //

        // get value expression type
        const QtTypeElement& valueExp = input2->checkType( typeTuple );

        // check type
        if( valueExp.getDataType() != QT_BOOL   && valueExp.getDataType() != QT_COMPLEX &&
                valueExp.getDataType() != QT_CHAR   && valueExp.getDataType() != QT_OCTET   &&
                valueExp.getDataType() != QT_USHORT && valueExp.getDataType() != QT_SHORT   &&
                valueExp.getDataType() != QT_ULONG  && valueExp.getDataType() != QT_LONG    &&
                valueExp.getDataType() != QT_FLOAT  && valueExp.getDataType() != QT_DOUBLE     )
        {
            RMInit::logOut << "Error: QtMarrayOp::checkType() - Value expression must be either of type atomic or complex" << endl;
            parseInfo.setErrorNo(412);
            throw parseInfo;
        }

        dataStreamType = valueExp;

        //
        // check condition expression
        //

        if( condOp )
        {
            // get value expression type
            const QtTypeElement& condExp = condOp->checkType( typeTuple );

            // check type
            if( condExp.getDataType() != QT_BOOL )
            {
                RMInit::logOut << "Error: QtMarrayOp::checkType() - Condition expression must be of type boolean" << endl;
                parseInfo.setErrorNo(413);
                throw parseInfo;
            }
        }

        // remove iterator again
        typeTuple->tuple.pop_back();
        if( newList )
        {
            delete typeTuple;
            typeTuple = NULL;
        }
    }
    else
        RMInit::logOut << "Error: QtCondenseOp::checkType() - operand branch invalid." << endl;

    return dataStreamType;
}


