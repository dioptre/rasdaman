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

static const char rcsid[] = "@(#)qlparser, QtVariable: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtvariable.cc,v 1.24 2003/12/27 20:40:52 rasdev Exp $";

#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

#include "raslib/rmdebug.hh"

#include "qlparser/qtvariable.hh"
#include "qlparser/qtmdd.hh"

#include "mddmgr/mddobj.hh"


const QtNode::QtNodeType QtVariable::nodeType = QT_MDD_VAR;


QtVariable::QtVariable( const string& initName )
    :  iteratorName( initName ),
       loadDomain(),
       oldLoadDomain(),
       dataIndex(-1)
{
    domainFlag = new vector<bool>();
}


QtVariable::QtVariable( const string& initName, const r_Minterval& initDomain )
    :  iteratorName( initName ),
       loadDomain( initDomain ),
       oldLoadDomain(),
       dataIndex(-1)
{
    domainFlag = new vector<bool>();
}


QtVariable::~QtVariable()
{
    // delete STL vector including its elements
    delete domainFlag;
    domainFlag=NULL;
}


bool
QtVariable::equalMeaning( QtNode* node )
{
    RMDBCLASS( "QtVariable", "equalMeaning( QtNode* )", "qlparser", __FILE__, __LINE__ )

    bool result = false;

    if( nodeType == node->getNodeType() )
    {
        QtVariable* mddVarNode;

        mddVarNode = (QtVariable*) node; // by force

        if( iteratorName.compare ( mddVarNode->getIteratorName()) == 0 )
            if( ( loadDomain.dimension() == 0 ) ||
                    ( (mddVarNode->getLoadDomain()).dimension() == 0 ) )
                result = true;
            else if( loadDomain.dimension() == (mddVarNode->getLoadDomain()).dimension() )
                result = loadDomain.intersects_with( mddVarNode->getLoadDomain() );
    };

    // equalMeaning() depends only on the loadDomain and not on the domainFlag!

    return result;
}


string
QtVariable::getSpelling()
{
    r_Point point;
    r_Dimension d;

    char tempStr[20];
    ostringstream os;
    sprintf(tempStr, "%ud", (unsigned long)getNodeType());
    string result  = string(tempStr);

    result.append( iteratorName );

    if( loadDomain.dimension() > 0)
    {
        os << loadDomain << ends;
        result.append(os.str());
    };

    return result;
}


QtNode::QtAreaType
QtVariable::getAreaType()
{
    return QT_AREA_MDD;
}


void
QtVariable::optimizeLoad( QtTrimList* trimList )
{
    RMDBCLASS( "QtVariable", "optimizeLoad( QtTrimList* )", "qlparser", __FILE__, __LINE__ )

    if( !trimList->empty() )
    {
        // get the highest specified dimension
        r_Dimension maxDimension=0;
        QtTrimList::iterator i;

        for( i=trimList->begin(); i!=trimList->end(); i++ )
            // get the maximum
            maxDimension = maxDimension > (*i)->dimension ? maxDimension : (*i)->dimension;

        // create a new loadDomain object and initialize it with open bounds
        loadDomain = r_Minterval(maxDimension+1);

        delete domainFlag; // delete the old array
        domainFlag = new vector<bool>(maxDimension+1);

        for( int j=0; j<loadDomain.dimension(); j++ )
        {
            loadDomain[j]    = r_Sinterval('*','*');
            (*domainFlag)[j] = true;
        }

        // fill the loadDomain object with the QtTrimList specifications
        for( i=trimList->begin(); i!=trimList->end(); i++ )
        {
            loadDomain[(*i)->dimension]    = (*i)->interval;
            (*domainFlag)[(*i)->dimension] = (*i)->intervalFlag;
        }

        // delete heap based elements
        // release( trimList->begin(), trimList->end() );
        for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
        {
            delete *iter;
            *iter=NULL;
        }

        // changed from RMInit:logOut -- PB 2003-nov-20
        RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtVariable", "optimizeLoad: geometric load optimization: " << iteratorName << loadDomain );
    }

    // delete list
    delete trimList;
    trimList=NULL;
}



QtData*
QtVariable::evaluate( QtDataList* inputList ) throw (ParseInfo)
{
    RMDBCLASS( "QtVariable", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

    RMDBGENTER( 2, RMDebug::module_qlparser, "QtVariable", "QtVariable::evaluate() - " << iteratorName.c_str() << endl )

    vector<QtData*>::iterator i; //default

    QtData* returnValue = NULL;

    if( inputList /* && inputList->size() > dataIndex*/ )
    {
        QtData* dataObject = 0;

        if( dataIndex == -1 )
        {
            // Search for the data object matching the iterator name
            int pos = 0;

            for( QtDataList::iterator iter=inputList->begin(); iter!=inputList->end() && !dataObject; iter++ )
            {
                if( iteratorName == (*iter)->getIteratorName() )
                    dataObject = *iter;
                else
                    pos++;
            }

            if( dataObject ) dataIndex = pos;
        }
        else
            // For performance reasons, take the data element from position determined in the first run.
            dataObject = (*inputList)[dataIndex];

        if( !dataObject )
        {
            RMInit::logOut << "Error: QtVariable::evaluate() - collection iterator " <<
                           iteratorName.c_str() << " is unknwon." << endl;
            parseInfo.setErrorNo(357);
            throw parseInfo;
        }

        if( dataObject->getDataType() == QT_MDD )
        {
            if( loadDomain.dimension() == 0 )
            {
                // If no domain is specified, the load domain equals the current domain.
                // This means that the data object is passed with an incremented reference.
                // This mainly occurs with point accesses.

                dataObject->incRef();
                returnValue = dataObject;
            }
            else
            {
                QtMDD*  qtMDD         = (QtMDD*)dataObject;
                MDDObj* currentMDDObj = qtMDD->getMDDObject();

                RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtVariable", "  definitionDomain: " << currentMDDObj->getDefinitionDomain() )
                RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtVariable", "  currentDomain...: " << currentMDDObj->getCurrentDomain() )

                // load domain for the actual MDDObj
                r_Minterval actLoadDomain;

                // intersect actLoadDomain with defined domain
                try
                {
                    actLoadDomain.intersection_of( loadDomain, currentMDDObj->getCurrentDomain() );
                }
                catch( r_Edim_mismatch& )
                {
                    RMInit::logOut << "Error: QtVariable::evaluate() - specified domain dimensionality does not equal defined dimensionality." << endl;
                    parseInfo.setErrorNo(362);
                    throw parseInfo;
                }
                catch( r_Eno_interval )
                {
                    RMInit::logOut << "Error: QtVariable::evaluate() - Specified domain does not intersect with spatial domain of MDD." << endl;
                    parseInfo.setErrorNo(356);
                    throw parseInfo;
                }
                catch( r_Error& err )
                {
                    RMInit::logOut << "Error: QtVariable::evaluate() - general error." << endl;
                    parseInfo.setErrorNo(350);
                    throw parseInfo;
                }

                RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtVariable", "  loadDomain......: " << actLoadDomain )

                if( qtMDD->getLifetime() == QtData::QT_PERSISTENT )
                {
                    //
                    // Create a new QtMDD object as carrier object for the persistent MDD object
                    // and attach the load domain. Now there are more than one MDD objects pointing
                    // to the same persistent MDD object which should not cause any problem.
                    //
                    // Note: Taking the same MDD object would mean sharing also the load domain which
                    //       is not possible if the iterator variable occurs with different spatial operations.
                    //

                    QtMDD* result = new QtMDD( (MDDObj*)currentMDDObj );
                    result->setLoadDomain( actLoadDomain );

                    returnValue = result;
                }
                else
                {
                    // Take the transient data object and increase its reference.
                    //
                    // Note: For a transient MDD object just one QtMDD carrier object is allowed.

                    qtMDD->incRef();
                    returnValue = qtMDD;
                }

            }

        }
        else
        {
            // Take the atomic data object and increase its reference.
            dataObject->incRef();
            returnValue = dataObject;
        }

    }
    else
    {
        RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtVariable", "Error: QtVariable::evaluate() - the input list is empty." )
    }
    return returnValue;
}


void
QtVariable::printTree( int tab, ostream& s, QtChildType /*mode*/ )
{
    s << SPACE_STR(tab).c_str() << "QtVariable Object: type " << flush;
    dataStreamType.printStatus( s );
    s << " name |" << iteratorName.c_str() << "|" /* << " pos " << dataIndex */ << endl;

    if( loadDomain.dimension() > 0 )
    {
        s << SPACE_STR(tab).c_str() << "load domain: ";
        loadDomain.print_status( s );
        s << " - Trimflag: ";

        for( int i=0; i<domainFlag->size(); i++)
            s << (*domainFlag)[i];
        s << endl;
    }

    if( oldLoadDomain.dimension() > 0 )
    {
        s << SPACE_STR(tab).c_str() << "old  domain: ";
        oldLoadDomain.print_status( s );
        s << endl;
    };
}



void
QtVariable::printAlgebraicExpression( ostream& s )
{
    s << iteratorName.c_str() << flush;
}



const QtTypeElement&
QtVariable::checkType( QtTypeTuple* typeTuple ) throw (ParseInfo)
{
    RMDBCLASS( "QtVariable", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

    dataStreamType.setDataType( QT_TYPE_UNKNOWN );

    if( typeTuple )
    {
        vector<QtTypeElement>::iterator iter;

        // search for the type matching the variable name
        for( iter=typeTuple->tuple.begin();
                iter!=typeTuple->tuple.end() && dataStreamType.getDataType() == QT_TYPE_UNKNOWN;
                iter++ )
        {
            if( (*iter).getName() && iteratorName == string( (*iter).getName() ) )
                dataStreamType = *iter;
        }
    }

    if( (dataStreamType.getDataType() == QT_TYPE_UNKNOWN) )
    {
        RMInit::logOut << "Error: QtVariable::checkType() - variable " << iteratorName.c_str() << " is unknwon." << endl;
        parseInfo.setErrorNo(357);
        throw parseInfo;
    }

    return dataStreamType;
}



