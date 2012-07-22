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

static const char rcsid[] = "@(#)qlparser, QtUnaryOperation: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtunaryoperation.cc,v 1.23 2003/12/27 20:51:28 rasdev Exp $";

#include "qlparser/qtunaryoperation.hh"
#include "qlparser/qtconst.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif

QtUnaryOperation::QtUnaryOperation()
    :  QtOperation(),
       input(NULL)
{
}


QtUnaryOperation::QtUnaryOperation( QtOperation* inputInit )
    :  QtOperation(),
       input( inputInit )
{
    if( input )
        input->setParent( this );
}


QtUnaryOperation::~QtUnaryOperation()
{
    if( input )
    {
        delete input;
        input=NULL;
    }
}



string
QtUnaryOperation::getSpelling()
{
    char tempStr[20];
    sprintf(tempStr, "%ud", (unsigned long)getNodeType());
    string result  = string(tempStr);
    result.append( "(" );
    result.append( input->getSpelling() );
    result.append( ")" );

    return result;
}



void
QtUnaryOperation::simplify()
{
    RMDBCLASS( "QtUnaryOperation", "simplify()", "qlparser", __FILE__, __LINE__ )

    // In order to work bottom up, first inspect the descendants
    QtNode::simplify();

    // Test, if operand is available.
    if( input )
    {
        // Test, if operand is of const type.
        if( input->getNodeType() ==  QT_CONST )
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
QtUnaryOperation::equalMeaning( QtNode* node )
{
    RMDBCLASS( "QtUnaryOperation", "equalMeaning( QtNode* )", "qlparser", __FILE__, __LINE__ )

    bool result = false;

    if( getNodeType() == node->getNodeType() )
    {
        QtUnaryOperation* unaryNode = (QtUnaryOperation*) node; // by force

        result = input->equalMeaning( unaryNode->getInput() );
    };

    return ( result );
}



QtNode::QtNodeList*
QtUnaryOperation::getChilds( QtChildType flag )
{
    RMDBCLASS( "QtUnaryOperation", "getChilds( QtChildType )", "qlparser", __FILE__, __LINE__ )

    QtNodeList* resultList=NULL;

    if( flag == QT_DIRECT_CHILDS )
        resultList = new QtNodeList();

    if( flag == QT_LEAF_NODES || flag == QT_ALL_NODES )
        resultList = input->getChilds( flag );

    if( flag == QT_DIRECT_CHILDS || flag == QT_ALL_NODES )
        resultList->push_back( input );

    return resultList;
}


QtNode::QtAreaType
QtUnaryOperation::getAreaType()
{
    return( input->getAreaType() );
}


void
QtUnaryOperation::optimizeLoad( QtTrimList* trimList )
{
    RMDBCLASS( "QtUnaryOperation", "optimizeLoad( QtTrimList* )", "qlparser", __FILE__, __LINE__ )

    // by default, pass load domain to the input
    if( input )
        input->optimizeLoad( trimList );
    else
    {
        delete trimList;
        trimList=NULL;
    }
}


void
QtUnaryOperation::printTree( int tab, ostream& s, QtChildType mode )
{
    if( mode != QtNode::QT_DIRECT_CHILDS )
        if( input )
        {
            s << SPACE_STR(tab).c_str() << "input: " << endl;
            input->printTree( tab+2, s, mode );
        }
        else
            s << SPACE_STR(tab).c_str()  << "no input" << endl;
}






