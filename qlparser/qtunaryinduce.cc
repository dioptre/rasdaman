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

static const char rcsid[] = "@(#)qlparser, QtUnaryInduce: $Id: qtunaryinduce.cc,v 1.47 2002/08/19 11:13:27 coman Exp $";

#include "raslib/rmdebug.hh"

#include "qlparser/qtunaryinduce.hh"
#include "qlparser/qtmdd.hh"
#include "qlparser/qtatomicdata.hh"
#include "qlparser/qtcomplexdata.hh"
#include "qlparser/qtnode.hh"

#include "mddmgr/mddobj.hh"
#include "tilemgr/tile.hh"

#include "catalogmgr/typefactory.hh"
#include "relcatalogif/structtype.hh"

#include <sstream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif

const QtNode::QtNodeType QtUnaryInduce::nodeType = QtNode::QT_UNARY_INDUCE;

QtUnaryInduce::QtUnaryInduce( QtOperation* initInput )
    :  QtUnaryOperation( initInput )
{
}



bool
QtUnaryInduce::getOperand( QtDataList* inputList, QtData* &operand )
{
    bool success = false;

    // get the operands
    operand = input->evaluate( inputList );

    // Test, if the operands are valid.
    if( operand )
        success = true;
    else
    {
        RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtUnaryInduce", "Information: QtUnaryInduce::getOperand() - operand is not provided." )
    }

    return success;
}



QtData*
QtUnaryInduce::computeOp( QtData* operand, Ops::OpType operation )
{
    QtData* returnValue = NULL;

    if( operand->getDataType() == QT_MDD )
    {
        QtMDD* mdd = (QtMDD*) operand;

        const BaseType* resultCellType = (BaseType*)(Ops::getResultType( operation, mdd->getCellType() ));

        returnValue = computeUnaryMDDOp( mdd, resultCellType, operation);
    }
    else if( operand->isScalarData() )
    {
        QtScalarData* scalar = (QtScalarData*) operand;

        const BaseType* resultCellType = (BaseType*)(Ops::getResultType( operation, scalar->getValueType() ));

        returnValue = computeUnaryOp( scalar, resultCellType, operation );
    }

    return returnValue;
}



QtData*
QtUnaryInduce::computeUnaryMDDOp( QtMDD* operand, const BaseType* resultBaseType,
                                  Ops::OpType operation, unsigned int operandOffset )
{
    RMDBCLASS( "QtUnaryInduce", "computeUnaryMDDOp( QtMDD*, BaseType*, Ops::OpType, unsigned int ) ", "qlparser", __FILE__, __LINE__ )

    QtData* returnValue = NULL;

    // get the MDD object
    MDDObj* op = ((QtMDD*)operand)->getMDDObject();

    //  get the area, where the operation has to be applied
    const r_Minterval &areaOp = ((QtMDD*)operand)->getLoadDomain();

    // contains all tiles of the operand
    vector<Tile*>* allTiles=NULL;

    // iterator for tiles

    // create MDDObj for result
    MDDBaseType* mddBaseType = new MDDBaseType( "tmp", resultBaseType );
    TypeFactory::addTempType( mddBaseType );

    MDDObj* mddres = new MDDObj( mddBaseType, areaOp );

    // get all tiles in relevant area
    allTiles = op->intersect(areaOp);
    std::vector<Tile*>::iterator tileIt = allTiles->begin();
    if (tileIt != allTiles->end())
    {
        Tile* resTile = NULL;
        UnaryOp* myOp = NULL;
        if (operation == Ops::OP_IDENTITY)
        {
            myOp = Ops::getUnaryOp(operation, resultBaseType, resultBaseType, 0, operandOffset);
        }
        else
        {
            myOp = Ops::getUnaryOp(operation, resultBaseType, (*tileIt)->getType(), 0, 0);
        }
        if (myOp == NULL)
        {
            RMInit::logOut << "QtUnaryInduce::computeUnaryMDDOp(...) could not get operation for result type " << resultBaseType->getName() << " argument type " << (*tileIt)->getType() << " operation " << operation << endl;
            delete myOp;
            myOp = NULL;
            delete allTiles;
            allTiles = NULL;
            //contents of allTiles are deleted when index is deleted
            delete mddres;
            mddres = NULL;
            delete resTile;
            resTile = NULL;
            delete mddres;
            mddres = NULL;
            // i am not sure about that error number...
            parseInfo.setErrorNo(366);
            throw parseInfo;
        }
        // and iterate over them
        try
        {
            for( ; tileIt !=  allTiles->end(); tileIt++ )
            {
                // domain of the actual tile
                const r_Minterval &tileDom = (*tileIt)->getDomain();

                // domain of the relevant area of the actual tile
                r_Minterval intersectDom( tileDom.create_intersection( areaOp ) );

                // create tile for result
                resTile = new Tile( intersectDom, resultBaseType );

                // carry out operation on the relevant area of the tiles
                resTile->execUnaryOp(myOp, intersectDom, (*tileIt), intersectDom);
                // insert Tile in result mdd
                mddres->insertTile( resTile );
            }
        }
        catch(r_Error& err)
        {
            RMInit::logOut << "QtUnaryInduce::computeUnaryMDDOp caught " << err.get_errorno() << " " << err.what() << endl;
            delete myOp;
            myOp = NULL;
            delete allTiles;
            allTiles = NULL;
            //contents of allTiles are deleted when index is deleted
            delete mddres;
            mddres = NULL;
            delete resTile;
            resTile = NULL;
            delete mddres;
            mddres = NULL;
            parseInfo.setErrorNo(err.get_errorno());
            throw parseInfo;
        }
        catch (int err)
        {
            RMInit::logOut << "QtUnaryInduce::computeUnaryMDDOp caught errno error (" << err << ") in unaryinduce" << endl;
            delete myOp;
            myOp = NULL;
            delete allTiles;
            allTiles = NULL;
            //contents of allTiles are deleted when index is deleted
            delete mddres;
            mddres = NULL;
            delete resTile;
            resTile = NULL;
            delete mddres;
            mddres = NULL;
            parseInfo.setErrorNo(err);
            throw parseInfo;
        }

        delete myOp;
        myOp = NULL;
    }
    // delete tile vector
    delete allTiles;
    allTiles = NULL;

    // create a new QtMDD object as carrier object for the transient MDD object
    returnValue = new QtMDD( (MDDObj*)mddres );

    // The following is now done when deleting the last reference to the operand
    // delete the obsolete MDD object
    //  delete op;

    return returnValue;
}


QtData*
QtUnaryInduce::computeUnaryOp( QtScalarData* operand, const BaseType* resultBaseType,
                               Ops::OpType operation, unsigned int operandOffset )
{
    RMDBCLASS( "QtUnaryInduce", "computeUnaryOp( QtScalarData*, BaseType*, Ops::OpType, unsigned int ) ", "qlparser", __FILE__, __LINE__ )

    QtScalarData* scalarDataObj = NULL;

    // allocate memory for the result
    char* resultBuffer = new char[ resultBaseType->getSize() ];

    RMDBGIF( 4, RMDebug::module_qlparser, "QtUnaryInduce", \
             RMInit::dbgOut << "Operand value "; \
             operand->getValueType()->printCell( RMInit::dbgOut, operand->getValueBuffer() ); \
             RMInit::dbgOut << endl; \
           )

    if(( operation == Ops::OP_IDENTITY )) // || ( operation == Ops::OP_SQRT ))
        // operand type is the same as result type
        Ops::execUnaryConstOp( operation, resultBaseType,
                               resultBaseType,
                               resultBuffer,
                               operand->getValueBuffer(),
                               0, operandOffset );
    else
        try
        {
            Ops::execUnaryConstOp( operation, resultBaseType,
                                   operand->getValueType(),
                                   resultBuffer,
                                   operand->getValueBuffer(),
                                   0, operandOffset );
        }
        catch(int err)
        {
            delete[] resultBuffer;
            resultBuffer = NULL;
            parseInfo.setErrorNo(err);
            throw parseInfo;
        }

    RMDBGIF( 4, RMDebug::module_qlparser, "QtUnaryInduce", \
             RMInit::dbgOut << "Result value "; \
             resultBaseType->printCell( RMInit::dbgOut, resultBuffer ); \
             RMInit::dbgOut << endl; \
           )

    if( resultBaseType->getType() == STRUCT )
        scalarDataObj = new QtComplexData();
    else
        scalarDataObj = new QtAtomicData();

    scalarDataObj->setValueType  ( resultBaseType );
    scalarDataObj->setValueBuffer( resultBuffer );

    return scalarDataObj;
}


const QtNode::QtNodeType QtNot::nodeType = QtNode::QT_NOT;

QtNot::QtNot( QtOperation* initInput )
    :  QtUnaryInduce( initInput )
{
}


QtData*
QtNot::evaluate( QtDataList* inputList )
{
    QtData* returnValue = NULL;
    QtData* operand = NULL;

    if( getOperand( inputList, operand ) )
    {
        returnValue = computeOp( operand, Ops::OP_NOT );

        // delete old operand
        if( operand ) operand->deleteRef();
    }
    return returnValue;
}



void
QtNot::printTree( int tab, ostream& s, QtChildType mode )
{
    s << SPACE_STR(tab).c_str() << "QtNot Object" << endl;

    QtUnaryInduce::printTree( tab+2, s, mode );
}



void
QtNot::printAlgebraicExpression( ostream& s )
{
    s << "not(";

    if( input )
        input->printAlgebraicExpression( s );
    else
        s << "<nn>";

    s << ")";
}



const QtTypeElement&
QtNot::checkType( QtTypeTuple* typeTuple )
{
    RMDBCLASS( "QtNot", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

    dataStreamType.setDataType( QT_TYPE_UNKNOWN );

    // check operand branches
    if( input )
    {

        // get input types
        const QtTypeElement& inputType = input->checkType( typeTuple );

        RMDBGIF( 4, RMDebug::module_qlparser, "QtNot", \
                 RMInit::dbgOut << "Operand: " << flush; \
                 inputType.printStatus( RMInit::dbgOut ); \
                 RMInit::dbgOut << endl; \
               )

        if( inputType.getDataType() == QT_MDD )
        {
            const BaseType* baseType = ((const MDDBaseType*)(inputType.getType()))->getBaseType();

            const BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_NOT, baseType ));

            if( !resultBaseType )
            {
                RMInit::logOut << "Error: QtNot::checkType() - induce operand type is not supported." << endl;
                parseInfo.setErrorNo(366);
                throw parseInfo;
            }

            MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
            TypeFactory::addTempType( resultMDDType );

            dataStreamType.setType( resultMDDType );
        }
        else if( inputType.isBaseType() )
        {
            BaseType* baseType = (BaseType*)(inputType.getType());

            const BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_NOT, baseType ));

            if( !resultBaseType )
            {
                RMInit::logOut << "Error: QtNot::checkType() - operand type is not supported." << endl;
                parseInfo.setErrorNo(367);
                throw parseInfo;
            }

            // MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
            // TypeFactory::addTempType( resultMDDType );

            // dataStreamType.setType( resultMDDType );
            dataStreamType.setType( resultBaseType );
        }
        else
        {
            RMInit::logOut << "Error: QtNot::checkType() - operation is not supported for strings." << endl;
            parseInfo.setErrorNo(385);
            throw parseInfo;
        }
    }
    else
        RMInit::logOut << "Error: QtNot::checkType() - operand branch invalid." << endl;

    return dataStreamType;
}




const QtNode::QtNodeType QtDot::nodeType = QtNode::QT_DOT;



QtDot::QtDot( const string& initElementName )
    :  QtUnaryInduce( NULL ),
       elementName( initElementName ),
       elementNo(-1)
{
}



QtDot::QtDot( unsigned initElementNo )
    :  QtUnaryInduce( NULL ),
       elementNo( initElementNo )
{
}



bool
QtDot::equalMeaning( QtNode* node )
{
    bool result = false;

    if( nodeType == node->getNodeType() )
    {
        QtDot* dotNode = (QtDot* ) node; // by force

        // In future, elementName have to be converted to elementNo
        // and then just the numbers are compared.
        if( (elementNo != -1 && elementNo == dotNode->elementNo)     ||
                (elementNo == -1 && elementName == dotNode->elementName)    )
            result = input->equalMeaning( dotNode->getInput() );
    };

    return result;
}


string
QtDot::getSpelling()
{
    char tempStr[20];
    sprintf(tempStr, "%ud", (unsigned long)getNodeType());
    string result  = string(tempStr);

    if( elementNo == -1 )
        result.append( elementName );
    else
    {
        std::ostringstream bufferStream;
        bufferStream << elementNo << ends;

        result.append( bufferStream.str() );
    }

    result.append( "(" );
    result.append( input->getSpelling() );
    result.append( ")" );

    return result;
}


QtData*
QtDot::evaluate( QtDataList* inputList )
{
    RMDBCLASS( "QtDot", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

    QtData* returnValue = NULL;
    QtData* operand = NULL;

    if( getOperand( inputList, operand ) )
    {
        if( operand->getDataType() == QT_MDD )
        {
            QtMDD* mdd = (QtMDD*) operand;

#ifdef QT_RUNTIME_TYPE_CHECK
            // test, if operand has complex base type
            if( mdd->getCellType()->getType() != STRUCT )
            {
                RMInit::logOut << "Internal error in QtDot::evaluate() - "
                               << "runtime type checking failed." << endl;

                // delete old operand
                if( operand ) operand->deleteRef();

                return 0;
            }
#endif

            StructType*  operandType    = (StructType*)mdd->getCellType();
            unsigned int operandOffset;
            const BaseType*    resultCellType = NULL;

            if( elementNo == -1 )
                resultCellType = operandType->getElemType( (char*)(elementName.c_str()) );
            else
                resultCellType = operandType->getElemType( (unsigned int)elementNo );

            if( !resultCellType )
            {
                RMInit::logOut << "Error: QtDot::evaluate() - struct selector is not valid." << endl;
                parseInfo.setErrorNo(370);
                throw parseInfo;
            }

            if( elementNo == -1 )
                operandOffset = operandType->getOffset( (char*)(elementName.c_str()) );
            else
                operandOffset = operandType->getOffset( (unsigned int)elementNo );

            RMDBGIF( 1, RMDebug::module_qlparser, "QtUnaryInduce", \
                     char* typeStructure = operandType->getTypeStructure();  \
                     RMDBGMIDDLE( 4, RMDebug::module_qlparser, "QtUnaryInduce", "Operand base type   " << operandType->getTypeName() << ", structure " << typeStructure ) \
                     free( typeStructure ); typeStructure=NULL; \
                     RMDBGMIDDLE( 4, RMDebug::module_qlparser, "QtUnaryInduce", "Operand base offset " << operandOffset ) \
                     typeStructure = resultCellType->getTypeStructure();   \
                     RMDBGMIDDLE( 4, RMDebug::module_qlparser, "QtUnaryInduce", "Result base type    " << resultCellType->getTypeName() << ", structure " << typeStructure ) \
                     free( typeStructure ); typeStructure=NULL; \
                   )

            returnValue = computeUnaryMDDOp( mdd, resultCellType, Ops::OP_IDENTITY, operandOffset );
        }
        else if( operand->isScalarData() )
        {
            QtScalarData* scalar = (QtScalarData*) operand;

#ifdef QT_RUNTIME_TYPE_CHECK
            // test, if operand has complex base type
            if( scalar->getValueType()->getType() != STRUCT )
            {
                RMInit::logOut << "Internal error in QtDot::evaluate() - "
                               << "runtime type checking failed." << endl;

                // delete old operand
                if( operand ) operand->deleteRef();

                return 0;
            }
#endif

            StructType*  operandType    = (StructType*)scalar->getValueType();
            unsigned int operandOffset;
            const BaseType*    resultCellType=NULL;

            if( elementNo == -1 )
                resultCellType = operandType->getElemType( (char*)(elementName.c_str()) );
            else
                resultCellType = operandType->getElemType( (unsigned int)elementNo );

            if( !resultCellType )
            {
                RMInit::logOut << "Error: QtDot::evaluate() - struct selector is not valid." << endl;
                parseInfo.setErrorNo(370);
                throw parseInfo;
            }

            if( elementNo == -1 )
                operandOffset = operandType->getOffset( (char*)(elementName.c_str()) );
            else
                operandOffset = operandType->getOffset( (unsigned int)elementNo );

            RMDBGIF( 1, RMDebug::module_qlparser, "QtUnaryInduce", \
                     char* typeStructure = operandType->getTypeStructure();  \
                     RMDBGMIDDLE( 4, RMDebug::module_qlparser, "QtUnaryInduce", "Operand scalar type   " << operandType->getTypeName() << ", structure " << typeStructure ) \
                     free( typeStructure ); typeStructure=NULL; \
                     RMDBGMIDDLE( 4, RMDebug::module_qlparser, "QtUnaryInduce", "Operand scalar offset " << operandOffset ) \
                     typeStructure = resultCellType->getTypeStructure();  \
                     RMDBGMIDDLE( 4, RMDebug::module_qlparser, "QtUnaryInduce", "Result scalar type    " << resultCellType->getTypeName() << ", structure " << typeStructure )  \
                     free( typeStructure ); typeStructure=NULL; \
                   )

            returnValue = computeUnaryOp( scalar, resultCellType, Ops::OP_IDENTITY, operandOffset );
        }
        else
        {
            RMInit::logOut << "Error: QtDot::evaluate() - operation is not supported for strings." << endl;
            parseInfo.setErrorNo(385);
            throw parseInfo;
        }

        // delete old operand
        if( operand ) operand->deleteRef();
    }

    return returnValue;
}



void
QtDot::printTree( int tab, ostream& s, QtChildType mode )
{
    if( elementNo == -1 )
        s << SPACE_STR(tab).c_str() << "QtDot Object: access " << elementName.c_str() << endl;
    else
        s << SPACE_STR(tab).c_str() << "QtDot Object: access no " << elementNo << endl;

    QtUnaryInduce::printTree( tab+2, s, mode );
}

void
QtDot::printAlgebraicExpression( ostream& s )
{
    s << "(";

    if( input )
        input->printAlgebraicExpression( s );
    else
        s << "<nn>";

    s << ").";

    if( elementNo == -1 )
        s << elementName.c_str();
    else
        s << elementNo;

    s << " ";
}

const QtTypeElement&
QtDot::checkType( QtTypeTuple* typeTuple )
{
    RMDBCLASS( "QtDot", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

    dataStreamType.setDataType( QT_TYPE_UNKNOWN );

    // check operand branches
    if( input )
    {

        // get input types
        const QtTypeElement& inputType = input->checkType( typeTuple );

        RMDBGIF( 4, RMDebug::module_qlparser, "QtDot", \
                 RMInit::dbgOut << "Operand: " << flush; \
                 inputType.printStatus( RMInit::dbgOut ); \
                 RMInit::dbgOut << endl; \
               )

        if( inputType.getDataType() == QT_MDD )
        {
            const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();

            // test, if operand has complex base type
            if( baseType->getType() != STRUCT )
            {
                RMInit::logOut << "Error: QtDot::evaluate() - operand of induce dot operation must be complex." << endl;
                parseInfo.setErrorNo(368);
                throw parseInfo;
            }

            StructType*  structType    = (StructType*)baseType;
            const BaseType*    resultBaseType = NULL;

            if( elementNo == -1 )
                resultBaseType = structType->getElemType( (char*)(elementName.c_str()) );
            else
                resultBaseType = structType->getElemType( (unsigned int)elementNo );

            if( !resultBaseType )
            {
                RMInit::logOut << "Error: QtDot::evaluate() - struct selector is not valid." << endl;
                parseInfo.setErrorNo(370);
                throw parseInfo;
            }

            MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
            TypeFactory::addTempType( resultMDDType );

            dataStreamType.setType( resultMDDType );
        }
        else if( inputType.isBaseType() )
        {
            BaseType* baseType = (BaseType*)(inputType.getType());

            // test, if operand has complex base type
            if( baseType->getType() != STRUCT )
            {
                RMInit::logOut << "Error: QtDot::evaluate() - operand of dot operation must be complex." << endl;
                parseInfo.setErrorNo(369);
                throw parseInfo;
            }

            StructType*  structType    = (StructType*)baseType;
            const BaseType*    resultBaseType = NULL;

            if( elementNo == -1 )
                resultBaseType = structType->getElemType( (char*)(elementName.c_str()) );
            else
                resultBaseType = structType->getElemType( (unsigned int)elementNo );

            if( !resultBaseType )
            {
                RMInit::logOut << "Error: QtDot::evaluate() - struct selector is not valid." << endl;
                parseInfo.setErrorNo(370);
                throw parseInfo;
            }

            dataStreamType.setType( resultBaseType );
        }
        else
        {
            RMInit::logOut << "Error: QtDot::checkType() - operation is not supported for strings." << endl;
            parseInfo.setErrorNo(385);
            throw parseInfo;
        }
    }
    else
        RMInit::logOut << "Error: QtDot::checkType() - operand branch invalid." << endl;
    return dataStreamType;
}

//--------------------------------------------
//  QtCast
//--------------------------------------------

const QtNode::QtNodeType QtCast::nodeType = QtNode::QT_CAST;

QtCast::QtCast(QtOperation* initInput, cast_types t):
    QtUnaryInduce(initInput), castType(t) {}

QtData* QtCast::evaluate(QtDataList* inputList)
{
    QtData* returnValue = NULL;
    QtData* operand = NULL;

    if(getOperand(inputList, operand))
        returnValue = computeOp( operand, getOp(castType));

    // delete old operand
    if(operand) operand->deleteRef();
    return returnValue;
}

void QtCast::printTree(int tab, ostream& s, QtChildType mode)
{
    const char *type_name[] =
    {
        "bool", "octet", "char", "short", "ushort",
        "long", "ulong", "float", "double"
    };
    s << SPACE_STR(tab).c_str() << "QtCastObject "
      << getNodeType()
      << "<" << type_name[castType] << ">"
      << endl;
    QtUnaryInduce::printTree( tab + 2, s, mode );
}

void QtCast::printAlgebraicExpression(ostream& s)
{
    const char *type_name[] =
    {
        "bool", "octet", "char", "short", "ushort",
        "long", "ulong", "float", "double"
    };
    s << "cast<" << type_name[castType] << ">(";
    if(input)
        input->printAlgebraicExpression(s);
    else
        s << "<nn>";
    s << ")";
}

const QtTypeElement& QtCast::checkType(QtTypeTuple* typeTuple)
{

    RMDBCLASS( "QtCast", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

    dataStreamType.setDataType( QT_TYPE_UNKNOWN );

    // check operand branches
    if(input)
    {

        // get input types
        const QtTypeElement& inputType = input->checkType( typeTuple );
        RMDBGIF( 4, RMDebug::module_qlparser, "QtCast", \
                 RMInit::dbgOut << "Operand: " << flush; \
                 inputType.printStatus( RMInit::dbgOut ); \
                 RMInit::dbgOut << endl; \
               )

        if(inputType.getDataType() == QT_MDD)
        {
            const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
            BaseType* resultBaseType = (BaseType*)(Ops::getResultType( getOp(castType), baseType ));


            if(!resultBaseType)
            {
                RMInit::logOut << "Error: QtCast::checkType() - induce operand type is not support" << endl;
                parseInfo.setErrorNo(366);
                throw parseInfo;
            }

            MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
            TypeFactory::addTempType( resultMDDType );
            dataStreamType.setType( resultMDDType );
        }

        else if(inputType.isBaseType())
        {
            BaseType* baseType = (BaseType*)(inputType.getType());
            BaseType* resultBaseType = (BaseType*)(Ops::getResultType( getOp(castType), baseType ));

            if(!resultBaseType)
            {
                RMInit::logOut << "Error: QtCast::checkType() - operand type is not supported." << endl;
                parseInfo.setErrorNo(367);
                throw parseInfo;
            }

            dataStreamType.setType( resultBaseType );
        }
        else
        {
            RMInit::logOut << "Error: QtCast::checkType() - operation is not supported for strings." << endl;
            parseInfo.setErrorNo(385);
            throw parseInfo;
        }
    }
    else
        RMInit::logOut << "Error: QtCast::checkType() - operand branch invalid." << endl;

    return dataStreamType;
}


//--------------------------------------------
//  QtRealPartOp
//--------------------------------------------

const QtNode::QtNodeType QtRealPartOp::nodeType = QtNode::QT_REALPART;

QtRealPartOp::QtRealPartOp(QtOperation* initInput): QtUnaryInduce(initInput) {}

QtData* QtRealPartOp::evaluate(QtDataList* inputList)
{
    QtData* returnValue = NULL;
    QtData* operand = NULL;

    if(getOperand(inputList, operand))
        returnValue = computeOp( operand, Ops::OP_REALPART );
    // delete old operand
    if(operand) operand->deleteRef();
    return returnValue;
}

void QtRealPartOp::printTree(int tab, ostream& s, QtChildType mode)
{
    s << SPACE_STR(tab).c_str() << "QtRealPartOpObject " << getNodeType() << endl;
    QtUnaryInduce::printTree( tab + 2, s, mode );
}

void QtRealPartOp::printAlgebraicExpression(ostream& s)
{
    s << "Re(";
    if(input)
        input->printAlgebraicExpression(s);
    else
        s << "<nn>";
    s << ")";
}

const QtTypeElement& QtRealPartOp::checkType(QtTypeTuple* typeTuple)
{
    RMDBCLASS( "QtRealPartOp", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

    dataStreamType.setDataType( QT_TYPE_UNKNOWN );

    // check operand branches
    if(input)
    {

        // get input types
        const QtTypeElement& inputType = input->checkType( typeTuple );
        RMDBGIF( 1, RMDebug::module_qlparser, "QtRealPartOp", \
                 RMInit::dbgOut << "Operand: " << flush; \
                 inputType.printStatus( RMInit::dbgOut ); \
                 RMInit::dbgOut << endl;
               )

        if(inputType.getDataType() == QT_MDD)
        {
            const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
            BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_REALPART, baseType ));
            if(!resultBaseType)
            {
                RMInit::logOut << "Error: QtRealPartOp::checkType() - induce operand type is not support" << endl;
                parseInfo.setErrorNo(366);
                throw parseInfo;
            }
            MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
            TypeFactory::addTempType( resultMDDType );
            dataStreamType.setType( resultMDDType );
        }
        else if(inputType.isBaseType())
        {
            BaseType* baseType = (BaseType*)(inputType.getType());
            BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_REALPART, baseType ));
            if(!resultBaseType)
            {
                RMInit::logOut << "Error: QtRealPartOp::checkType() - operand type is not supported." << endl;
                parseInfo.setErrorNo(367);
                throw parseInfo;
            }
            dataStreamType.setType( resultBaseType );
        }
        else
        {
            RMInit::logOut << "Error: QtRealPartOp::checkType() - operation is not supported for strings." << endl;
            parseInfo.setErrorNo(385);
            throw parseInfo;
        }
    }
    else
        RMInit::logOut << "Error: QtRealPartOp::checkType() - operand branch invalid." << endl;

    return dataStreamType;
}

//--------------------------------------------
//  QtImaginarPartOp
//--------------------------------------------

const QtNode::QtNodeType QtImaginarPartOp::nodeType = QtNode::QT_IMAGINARPART;

QtImaginarPartOp::QtImaginarPartOp(QtOperation* initInput): QtUnaryInduce(initInput) {}

QtData* QtImaginarPartOp::evaluate(QtDataList* inputList)
{
    QtData* returnValue = NULL;
    QtData* operand = NULL;

    if(getOperand(inputList, operand))
        returnValue = computeOp( operand, Ops::OP_IMAGINARPART );
    // delete old operand
    if(operand) operand->deleteRef();
    return returnValue;
}

void QtImaginarPartOp::printTree(int tab, ostream& s, QtChildType mode)
{
    s << SPACE_STR(tab).c_str() << "QtImaginarPartObject " << getNodeType() << endl;
    QtUnaryInduce::printTree( tab + 2, s, mode );
}

void QtImaginarPartOp::printAlgebraicExpression(ostream& s)
{
    s << "Im(";
    if(input)
        input->printAlgebraicExpression(s);
    else
        s << "<nn>";
    s << ")";
}

const QtTypeElement& QtImaginarPartOp::checkType(QtTypeTuple* typeTuple)
{
    RMDBCLASS( "QtImaginarPart", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

    dataStreamType.setDataType( QT_TYPE_UNKNOWN );

    // check operand branches
    if(input)
    {

        // get input types
        const QtTypeElement& inputType = input->checkType( typeTuple );

        RMDBGIF( 4, RMDebug::module_qlparser, "QtImaginarPartOp", \
                 RMInit::dbgOut << "Operand: " << flush; \
                 inputType.printStatus( RMInit::dbgOut ); \
                 RMInit::dbgOut << endl; \
               )

        if(inputType.getDataType() == QT_MDD)
        {
            const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
            BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_IMAGINARPART, baseType ));
            if(!resultBaseType)
            {
                RMInit::logOut << "Error: QtImaginarPart::checkType() - induce operand type is not support" << endl;
                parseInfo.setErrorNo(366);
                throw parseInfo;
            }
            MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
            TypeFactory::addTempType( resultMDDType );
            dataStreamType.setType( resultMDDType );
        }
        else if(inputType.isBaseType())
        {
            BaseType* baseType = (BaseType*)(inputType.getType());
            BaseType* resultBaseType = (BaseType*)(Ops::getResultType(Ops::OP_IMAGINARPART, baseType));
            if(!resultBaseType)
            {
                RMInit::logOut << "Error: QtImaginarPart::checkType() - operand type is not supported." << endl;
                parseInfo.setErrorNo(367);
                throw parseInfo;
            }
            dataStreamType.setType( resultBaseType );
        }
        else
        {
            RMInit::logOut << "Error: QtImaginarPart::checkType() - operation is not supported for strings." << endl;
            parseInfo.setErrorNo(385);
            throw parseInfo;
        }
    }
    else
        RMInit::logOut << "Error: QtImaginarPart::checkType() - operand branch invalid." << endl;
    return dataStreamType;
}

#include "autogen_qtui.cc"
