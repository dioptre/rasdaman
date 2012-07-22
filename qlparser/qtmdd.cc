#include "mymalloc/mymalloc.h"
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

static const char rcsid[] = "@(#)qlparser, QtMDD: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtmdd.cc,v 1.44 2003/12/27 20:51:28 rasdev Exp $";

#include "qlparser/qtmdd.hh"
#include "qlparser/qtscalardata.hh"
#include "qlparser/qtmintervaldata.hh"
#include "qlparser/qtoperation.hh"

#include "mddmgr/mddobj.hh"
#include "tilemgr/tile.hh"
#include "mddmgr/mddcoll.hh"
#include "catalogmgr/typefactory.hh"

#include "raslib/rmdebug.hh"

#include "relcatalogif/mdddimensiontype.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif
#include <iostream>

extern MDDColl* mddConstants;

QtMDD::QtMDD( MDDObj* ptr )
    : QtData(),
      mddObject( ptr )
{
    if( ptr && ptr->isPersistent() )
        setLifetime( QtData::QT_PERSISTENT );
    else
        setLifetime( QtData::QT_TRANSIENT );

    if( ptr )
        loadDomain = ptr->getDefinitionDomain();
}


QtMDD::QtMDD( MDDObj* ptr, string name )
    : QtData( name ),
      mddObject( ptr )
{
    if( ptr && ptr->isPersistent() )
        setLifetime( QtData::QT_PERSISTENT );
    else
        setLifetime( QtData::QT_TRANSIENT );

    if( ptr )
        loadDomain = ptr->getCurrentDomain();
}


QtMDD::QtMDD( QtOperation* mintervalOp, list<QtScalarData*>* literalList )
    : QtData(), mddObject(0)
{
    list< QtScalarData* >::iterator         elemIter;
    QtScalarData*                           scalarElem=NULL;

    //
    // evaluate domain
    //

    if( mintervalOp )
    {

        QtData* operand = mintervalOp->evaluate(NULL);

        if( operand->getDataType() != QT_MINTERVAL )
        {
            RMInit::logOut << "Error: QtMDD( QtOperation*, list<QtScalarData*>* ) - Can not evaluate domain expression to an minterval." << endl;
            ParseInfo errorInfo = getParseInfo();
            errorInfo.setErrorNo(401);
            throw errorInfo;
        }

        r_Minterval domain = ((QtMintervalData*)operand)->getMintervalData();

        // delete old operand
        if( operand ) operand->deleteRef();

        //
        // determine base type
        //

        if( literalList->size()!=0 )
        {
            scalarElem = *(literalList->begin());
            const BaseType* baseType = scalarElem->getValueType();

            //
            // allocate memory and fill it with cell values of the list
            //
            unsigned long cellCount = 0;
            unsigned long cellSize  = baseType->getSize();
            char* cellBuffer   = (char*)mymalloc( domain.cell_count()*cellSize );
            char* bufferOffset = cellBuffer;

            for( elemIter = literalList->begin(); elemIter != literalList->end(); elemIter++ )
            {
                scalarElem = *elemIter;
                cellCount++;

                // do not write beyond array boundary
                if( cellCount <= domain.cell_count() )
                {
                    if( scalarElem->getValueType() != baseType )
                    {
                        RMDBGONCE(2, RMDebug::module_qlparser, "QtMDD", "Error: QtMDD() - All cell values of an MDD must be of the same type." )
                        free( cellBuffer );
                        cellBuffer=NULL;
                        ParseInfo errorInfo = getParseInfo();
                        errorInfo.setErrorNo(301);
                        throw errorInfo;
                    }
                    memcpy( (void*)bufferOffset, (void*)(scalarElem->getValueBuffer()), (unsigned int)cellSize );
                    bufferOffset += cellSize;
                }
            }

            // delete literal list - done by caller
            //  delete literalList;

            if( cellCount != domain.cell_count() )
            {
                RMDBGONCE(2, RMDebug::module_qlparser, "QtMDD", "Error: QtMDD() - Number of cells specified does not match the number of cells of the given spatial domain." )
                free( cellBuffer );
                cellBuffer=NULL;
                ParseInfo errorInfo = getParseInfo();
                errorInfo.setErrorNo(302);
                throw errorInfo;
            }

            //
            // create transient tile
            //
            Tile* tile = new Tile( domain, baseType, cellBuffer, 0 );

            //
            // create transiend mddObject and attach created tile
            //
            MDDDimensionType* mddDimensionType = new MDDDimensionType( "tmp", baseType, domain.dimension() );
            TypeFactory::addTempType( mddDimensionType );
            mddObject = new MDDObj( mddDimensionType, domain );
            mddObject->insertTile( tile );
            loadDomain = domain;
        }
        else
            RMInit::logOut << "Internal Error: QtMDD( domain, literalList ) - list of literal lists is empty" << endl;
    }
    else
    {
        RMInit::logOut << "Error: QtMDD( QtOperation*, list<QtScalarData*>* ) - Domain of MDD constructor has to be defined." << endl;
        ParseInfo errorInfo = getParseInfo();
        errorInfo.setErrorNo(400);
        throw errorInfo;
    }

}



QtMDD::QtMDD( int constantNo )
    : QtData(),
      mddObject( NULL )
{
    RMDBGONCE(2, RMDebug::module_qlparser, "QtMDD", "QtMDD() - constant no " << constantNo )

    if( mddConstants )
    {

        MDDCollIter* mddIter = mddConstants->createIterator();
        //for( mddIter->reset(); mddIter->notDone(); mddIter->advance() )
        mddIter->reset();

        // take the MDD object
        mddObject = mddIter->getElement();

        // remove it from the constant list
        mddConstants->remove( mddObject );

        delete mddIter;
        mddIter = NULL;

        if( mddObject )
            loadDomain = mddObject->getCurrentDomain();
    }
    else
    {
        RMInit::logOut << "Error: QtMDD() - Unsatisfied MDD constant parameter." << endl;
        ParseInfo errorInfo = getParseInfo();
        errorInfo.setErrorNo(373);
        throw errorInfo;
    }

}



QtMDD::QtMDD( const QtMDD& obj )
    : QtData( obj ),
      mddObject( obj.mddObject )
{
}


QtMDD::~QtMDD()
{
    //this causes problems when passing more than one trans mddobj
    if( mddObject && getLifetime() == QtData::QT_TRANSIENT )
    {
        RMDBGONCE( 2, RMDebug::module_qlparser, "QtMDD", "~QtMDD() - transient MDD object " << mddObject << " deleted" )
        delete mddObject;
        mddObject = NULL;
    }
}


BaseType*
QtMDD::getCellType() const
{
    return (BaseType*)mddObject->getCellType();
}



unsigned long
QtMDD::getCellSize() const
{
    return mddObject->getCellType()->getSize();
}



QtDataType
QtMDD::getDataType() const
{
    return QT_MDD;
}



bool
QtMDD::equal( const QtData* /*obj*/ ) const
{
    int returnValue = false;  // not equal by initialization

    // Later on, MDD constants can be compared.

    return returnValue;
}



string
QtMDD::getSpelling() const
{
    string result;

    // no spelling right now

    return result;
}



char* QtMDD::getTypeStructure() const
{
    if( mddObject )
        return mddObject->getMDDBaseType()->getTypeStructure();
    else
        return NULL;
}



void
QtMDD::printStatus( ostream& stream ) const
{
    if( mddObject )
        stream << "MDD object: load domain: " << loadDomain << endl;
    else
        stream << "<no object>" << endl;

    QtData::printStatus( stream );

    RMDBGIF(3, RMDebug::module_qlparser, "QtMDD", mddObject->printStatus(0, stream); )

    RMDBGIF(30, RMDebug::module_qlparser, "QtMDD", \
            vector<Tile*>* vec = mddObject->getTiles(); \
            for( int i = 0; i<vec->size(); i++ ) \
            ((*vec)[i])->printStatus(); \
            delete vec; vec=NULL; )
    }
