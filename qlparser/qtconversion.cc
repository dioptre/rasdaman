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

static const char rcsid[] = "@(#)qlparser, QtConversion: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtconversion.cc,v 1.36 2003/12/27 20:51:28 rasdev Exp $";

#include "raslib/rmdebug.hh"
#include "raslib/basetype.hh"
#include "raslib/structuretype.hh"
#include "conversion/convertor.hh"
#include "conversion/convfactory.hh"

#include "qlparser/qtconversion.hh"
#include "qlparser/qtmdd.hh"

#include "mddmgr/mddobj.hh"
#include "tilemgr/tile.hh"

#include "catalogmgr/typefactory.hh"
#include "relcatalogif/typeiterator.hh"
#include "relcatalogif/structtype.hh"
#include "relcatalogif/chartype.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif

const QtNode::QtNodeType QtConversion::nodeType = QtNode::QT_CONVERSION;


QtConversion::QtConversion( QtOperation* newInput, QtConversionType
newConversionType, const char* s )
  : conversionType( newConversionType ), QtUnaryOperation( newInput ), paramStr(s)
{
  RMDBGONCE(2, RMDebug::module_qlparser, "QtConversion", "QtConversion()" )
}

void
QtConversion::setConversionTypeByName( string formatName )
{
  if(string("bmp") == formatName) 
  	conversionType = QtConversion::QT_TOBMP; 
  else if(string("hdf") == formatName ) 
  	conversionType = QtConversion::QT_TOHDF; 
  else if(string("png") == formatName) 
  	conversionType = QtConversion::QT_TOPNG; 
  else if (string("jpeg") == formatName) 
  	conversionType = QtConversion::QT_TOJPEG; 
  else if (string("tiff") == formatName) 
  	conversionType = QtConversion::QT_TOTIFF; 
  else if (string("vff") == formatName) 
  	conversionType = QtConversion::QT_TOVFF; 
  else if (string("csv") == formatName) 
  	conversionType = QtConversion::QT_TOCSV; 
  else if (string("tor") == formatName) 
  	conversionType = QtConversion::QT_TOTOR; 
  else if (string("dem") == formatName) 
  	conversionType = QtConversion::QT_TODEM;
  else if(string("inv_bmp") == formatName ) 
  	conversionType = QtConversion::QT_FROMBMP;   	
  else if(string("inv_hdf") == formatName ) 
  	conversionType = QtConversion::QT_FROMHDF; 
  else if(string("inv_csv") == formatName ) 
  	conversionType = QtConversion::QT_FROMCSV; 
  else if(string("inv_png") == formatName) 
  	conversionType = QtConversion::QT_FROMPNG; 
  else if (string("inv_jpeg") == formatName) 
  	conversionType = QtConversion::QT_FROMJPEG; 
  else if (string("inv_tiff") == formatName) 
  	conversionType = QtConversion::QT_FROMTIFF; 
  else if (string("inv_vff") == formatName) 
  	conversionType = QtConversion::QT_FROMVFF; 
  else if (string("inv_tor") == formatName) 
  	conversionType = QtConversion::QT_FROMTOR; 
  else if (string("inv_dem") == formatName) 
  	conversionType = QtConversion::QT_FROMDEM;  	
  else
    conversionType = QtConversion::QT_UNKNOWN;
}

bool
QtConversion::lookupConversionTypeByName( string formatName )
{
  return (( string("bmp")      == formatName ) || ( string("hdf")      == formatName ) ||
          ( string("png")      == formatName ) || ( string("jpeg")     == formatName ) || 
          ( string("dem")      == formatName ) || ( string("tor")      == formatName ) ||
          ( string("csv")      == formatName ) || ( string("inv_csv")  == formatName ) ||
          ( string("tiff")     == formatName ) || ( string("vff")      == formatName ) ||
          ( string("inv_bmp")  == formatName ) || ( string("inv_hdf")  == formatName ) ||
          ( string("inv_png")  == formatName ) || ( string("inv_jpeg") == formatName ) || 
          ( string("inv_dem")  == formatName ) || ( string("inv_tor")  == formatName ) ||
          ( string("inv_tiff") == formatName ) || ( string("inv_vff")  == formatName ));
	                              
}

bool
QtConversion::equalMeaning( QtNode* node )
{

  bool result = false;

  if( nodeType == node->getNodeType() )
  {
    QtConversion* convNode;
    convNode = (QtConversion*) node; // by force

    result = input->equalMeaning( convNode->getInput() );

    result &= conversionType == convNode->conversionType;
  };

  return ( result );
}


QtData*
QtConversion::evaluate( QtDataList* inputList )
{
  RMDBCLASS( "QtConversion", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

  QtData* returnValue = NULL;
  QtData* operand = NULL;

  if( conversionType == QT_UNKNOWN )
  {
    RMInit::logOut << "Error: QtConversion::evaluate() - unknown conversion format." << std::endl;
    parseInfo.setErrorNo(382);
    throw parseInfo;
  }

  operand = input->evaluate( inputList );

  if( operand )
  {
#ifdef QT_RUNTIME_TYPE_CHECK
    if( operand->getDataType() != QT_MDD )
    {
      RMInit::logOut << "Internal error in QtConversion::evaluate() - "
                     << "runtime type checking failed (MDD)." << std::endl; 

      // delete old operand
      if( operand ) operand->deleteRef();
      return 0;
    }
#endif

    QtMDD*  qtMDD         = (QtMDD*) operand;
    MDDObj* currentMDDObj = qtMDD->getMDDObject();
    Tile*   sourceTile    = NULL;
	    vector< Tile* >* tiles = NULL;
   if (qtMDD->getLoadDomain().is_origin_fixed() && qtMDD->getLoadDomain().is_high_fixed())
	{
	    // get relevant tiles
	tiles = currentMDDObj->intersect( qtMDD->getLoadDomain() );
	}
	else	{
		      RMDBGONCE(2, RMDebug::module_qlparser, "QtConversion", "evalutate() - no tile available to convert." )
		      return operand;
		}

    // check the number of tiles
    if( !tiles->size() )
    {
      RMDBGONCE(2, RMDebug::module_qlparser, "QtConversion", "evalutate() - no tile available to convert." )
      return operand;
    }

    // create one single tile with the load domain
    sourceTile = new Tile( tiles, qtMDD->getLoadDomain() );

    // delete the tile vector
    delete tiles;
    tiles = NULL;

    // get type structure of the operand base type
    char*  typeStructure = qtMDD->getCellType()->getTypeStructure();

    // convert structure to r_Type
    r_Type* baseSchema = r_Type::get_any_type( typeStructure );

    free( typeStructure );
    typeStructure = NULL;
    RMDBGONCE(2, RMDebug::module_qlparser, "QtConversion", "evalutate() - no tile available to convert." )
    RMDBGIF(2, RMDebug::module_qlparser, "QtConversion", \
  	    RMInit::dbgOut << "Cell base type for conversion: " << std::flush; \
	    baseSchema->print_status( RMInit::dbgOut ); \
	    RMInit::dbgOut << std::endl; )

    // 
    // real conversion
    //

    r_convDesc convResult;
    r_Minterval tileDomain = sourceTile->getDomain();

    //convertor type
    r_Data_Format convType = r_Array;
    //result type from convertor
    r_Data_Format convFormat = r_Array;    

    switch (conversionType)
    {
      case QT_TOTIFF:
	convType = r_TIFF; 
	convFormat = r_TIFF; 	
	break;      
      case QT_FROMTIFF:
	convType = r_TIFF; 
	convFormat = r_Array; 	
	break;
      case QT_TOBMP:
	convType = r_BMP; 
	convFormat = r_BMP; 	
	break;      
      case QT_FROMBMP:
	convType = r_BMP; 
	convFormat = r_Array; 	
	break;      
      case QT_TOHDF:
	convType = r_HDF; 
	convFormat = r_HDF; 	
	break;      
      case QT_TOCSV:
	convType = r_CSV; 
	convFormat = r_CSV; 	
	break;      
      case QT_FROMHDF:
	convType = r_HDF; 
	convFormat = r_Array; 	
	break;      
      case QT_FROMCSV:
	convType = r_CSV; 
	convFormat = r_Array; 	
	break;      
      case QT_TOJPEG:
	convType = r_JPEG; 
	convFormat = r_JPEG; 	
	break;      
      case QT_FROMJPEG:
	convType = r_JPEG; 
	convFormat = r_Array; 	
	break;      
      case QT_TOPNG:
	convType = r_PNG; 
	convFormat = r_PNG; 	
	break;      
      case QT_FROMPNG:
	convType = r_PNG; 
	convFormat = r_Array; 	
	break;      
      case QT_TOVFF:
	convType = r_VFF; 
	convFormat = r_VFF; 	
	break;      
      case QT_FROMVFF:
	convType = r_VFF; 
	convFormat = r_Array; 	
	break;      
      case QT_TOTOR:
	convType = r_TOR; 
	convFormat = r_TOR; 	
	break;      
      case QT_FROMTOR:
	convType = r_TOR; 
	convFormat = r_Array; 	
	break;      
      case QT_TODEM:
	convType = r_DEM; 
	convFormat = r_DEM; 	
	break;      
      case QT_FROMDEM:
	convType = r_DEM; 
	convFormat = r_Array; 	
	break;      
      default:
	RMInit::logOut << "Error: QtConversion::evaluate(): unsupported format " << conversionType << std::endl;
	throw r_Error(CONVERSIONFORMATNOTSUPPORTED);
	break;
    }
    convFormat = r_Array;

      r_Convertor *convertor = NULL;

      try {
        convertor = r_Convertor_Factory::create(convType, sourceTile->getContents(), tileDomain, baseSchema);
        if(conversionType < QT_FROMTIFF) {
	        RMDBGONCE(2, RMDebug::module_qlparser, "QtConversion", "evalutate() - convertor " << convType << " converting to " << convFormat << "...");
		convResult = convertor->convertTo(paramStr);
	}
	else {
	        RMDBGONCE(2, RMDebug::module_qlparser, "QtConversion", "evalutate() - convertor " << convType << " converting from " << convFormat << "...");
		convResult = convertor->convertFrom(paramStr);
	}	
      }
      catch (r_Error &err) {
	RMInit::logOut << "Error: QtConversion::evaluate(): conversion failed" << std::endl;
	delete sourceTile;
	sourceTile = NULL;
    	delete baseSchema;
	baseSchema = NULL;
        // delete old operand
        if( operand ) operand->deleteRef();
        if (convertor != NULL) {
  	   delete convertor;
           convertor=NULL;
        }

	parseInfo.setErrorNo(381);
	throw parseInfo;
      }
      if (convertor != NULL) {
	delete convertor;
        convertor=NULL;
      }

    RMDBGMIDDLE(2, RMDebug::module_qlparser, "QtConversion", "evalutate() - ok")


    //
    // done
    //

    // delete sourceTile 
    delete sourceTile;
    sourceTile = NULL;
    
    // create a transient tile for the compressed data
    const BaseType* myType = NULL;
    const r_Type* currStruct = NULL;
    if(convResult.destType->isPrimitiveType()) {
      myType=TypeFactory::mapType(convResult.destType->name());
      if(myType == NULL) { 
        RMInit::logOut << "Error: QtConversion::evaluate() no primitive type compatible found" << std::endl;
        delete convResult.destType;
        convResult.destType=NULL;
        delete convResult.dest;
        convResult.dest=NULL;
        throw r_Error(BASETYPENOTSUPPORTED);
      }

      if(strcasecmp( dataStreamType.getType()->getTypeName(), myType->getTypeName())) {
        //FIXME here we have to change the dataStreamType.getType(), is not char for 
        //conversion from_DEF all the time, we don't know the result type until we parse the data
        MDDBaseType* mddBaseType = new MDDBaseType( "tmp", TypeFactory::mapType(myType->getTypeName()) );
        TypeFactory::addTempType( mddBaseType );

        dataStreamType.setType( mddBaseType );
        RMInit::logOut << " QtConversion::evaluate() for conversion " << conversionType << " real result is " << std::flush;
        dataStreamType.printStatus(RMInit::logOut);
        RMInit::logOut << std::endl;
      }
    }
    else { //we assume that we deal with structure types
      TypeIterator<StructType> structIter = TypeFactory::createStructIter();
      while(structIter.not_done()) {
        // get type structure of current structtype
	typeStructure = structIter.get_element()->getTypeStructure();
	// convert structure to r_Type
	currStruct = r_Type::get_any_type( typeStructure );
	
	if(currStruct == NULL) {
	  RMInit::logOut << "Error: QtConversion::evaluate() no structure type compatible found" << std::endl;
          delete convResult.destType;
          convResult.destType=NULL;
          delete convResult.dest;
          convResult.dest=NULL;
          throw r_Error(STRUCTTYPE_ELEMENT_UNKNOWN);
	}	
	
	free(typeStructure);
	typeStructure = NULL;
	
        if(((r_Structure_Type*)currStruct)->compatibleWith((r_Structure_Type*)convResult.destType)) {
          //we found a type
          delete currStruct;
          currStruct = NULL;
          myType=structIter.get_element().ptr();
          break;
        }
        
        delete currStruct;
        currStruct = NULL;
        structIter.advance();
      }
      if(myType == NULL) { 
        RMInit::logOut << "Error: QtConversion::evaluate() no structure type compatible found" << std::endl;
        delete convResult.destType;
        convResult.destType=NULL;
        delete convResult.dest;
        convResult.dest=NULL;
        throw r_Error(STRUCTTYPE_ELEMENT_UNKNOWN);
      }
      
      if(strcasecmp(dataStreamType.getType()->getTypeName(), myType->getTypeName())) {
         //FIXME here we have to change the dataStreamType.getType(), is not char for 
         //conversion from_DEF all the time, we don't know the result type until we parse the data
         MDDBaseType* mddBaseType = new MDDBaseType( "tmp", TypeFactory::mapType(myType->getTypeName()) );
         TypeFactory::addTempType( mddBaseType );

         dataStreamType.setType( mddBaseType );
         RMInit::logOut << " QtConversion::evaluate() for conversion " << conversionType << " real result is " << std::flush;
         dataStreamType.printStatus(RMInit::logOut);
         RMInit::logOut << std::endl;
      }
    }
    
    Tile* resultTile = new Tile( convResult.destInterv, 
				myType,
    				convResult.dest, 
    				0, 
    				convFormat );     
    				
    // delete destination type
    if( convResult.destType ) {
  	      delete convResult.destType;
	      convResult.destType=NULL;
    }

    // create a transient MDD object for the query result
    MDDBaseType* mddBaseType = (MDDBaseType*)dataStreamType.getType();
    MDDObj* resultMDD = new MDDObj( mddBaseType, convResult.destInterv );
    resultMDD->insertTile( resultTile );

    // create a new QtMDD object as carrier object for the transient MDD object
    returnValue = new QtMDD( (MDDObj*)resultMDD );

    // delete base type schema 
    delete baseSchema;
    baseSchema = NULL;

    // delete old operand
    if( operand ) operand->deleteRef();
  }
  else
    RMInit::logOut << "Error: QtConversion::evaluate() - operand is not provided." << std::endl;

  return returnValue;
}


void
QtConversion::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtConversion Object: ";

  switch( conversionType )
  {
    case QT_TOTIFF:   s << "to TIFF"; break;
    case QT_TOBMP:    s << "to BMP"; break;
    case QT_TOHDF:    s << "to HDF"; break;
    case QT_TOCSV:    s << "to CSV"; break;
    case QT_TOJPEG:   s << "to JPEG"; break;
    case QT_TOPNG:    s << "to PNG"; break;
    case QT_TOVFF:    s << "to VFF"; break;
    case QT_TOTOR:    s << "to TOR"; break;
    case QT_TODEM:    s << "to DEM"; break;    
    case QT_FROMTIFF: s << "from TIFF"; break;
    case QT_FROMBMP:  s << "from BMP"; break;
    case QT_FROMHDF:  s << "from HDF"; break;
    case QT_FROMCSV:  s << "from CSV"; break;
    case QT_FROMJPEG: s << "from JPEG"; break;
    case QT_FROMPNG:  s << "from PNG"; break;
    case QT_FROMVFF:  s << "from VFF"; break;
    case QT_FROMTOR:  s << "from TOR"; break;
    case QT_FROMDEM:  s << "from DEM"; break;        
    default:          s << "unknown conversion"; break;
  }

  s << std::endl;

  QtUnaryOperation::printTree( tab, s, mode );
}



void
QtConversion::printAlgebraicExpression( ostream& s )
{
  s << conversionType << "(";
 
  if( input )
    input->printAlgebraicExpression( s );
  else
    s << "<nn>";

  s << ")";  
}



const QtTypeElement&
QtConversion::checkType( QtTypeTuple* typeTuple )
{
  RMDBCLASS( "QtConversion", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

  dataStreamType.setDataType( QT_TYPE_UNKNOWN );  

  // check operand branches
  if( input )
  {

  // get input type
  const QtTypeElement& inputType = input->checkType( typeTuple ); 

  if( inputType.getDataType() != QT_MDD )
   {
    RMInit::logOut << "Error: QtConversion::checkType() - operand is not of type MDD." << std::endl;
    parseInfo.setErrorNo(380);
    throw parseInfo;
   }

  //FIXME we set for every kind of conversion the result type char
  //for conversion from_DEF we don't know the result type until we parse the data
  MDDBaseType* mddBaseType = new MDDBaseType( "Char", TypeFactory::mapType("Char") );
  TypeFactory::addTempType( mddBaseType );

  dataStreamType.setType( mddBaseType );
  
  if(conversionType>QT_TODEM) {
    RMInit::logOut << std::endl << "QtConversion::checkType() for conversion " << conversionType << " assume the result " << std::flush;
    dataStreamType.printStatus(RMInit::logOut);
    RMInit::logOut << std::endl;
   }
  }
  else
    RMInit::logOut << "Error: QtConversion::checkType() - operand branch invalid." << std::endl;

  return dataStreamType;
}

std::ostream& 
operator<< (std::ostream& os, QtConversion::QtConversionType type) {
  switch( type )
  {
    case QtConversion::QT_TOTIFF:   os << "tiff"; break;
    case QtConversion::QT_TOBMP:    os << "bmp"; break;
    case QtConversion::QT_TOHDF:    os << "hdf"; break;
    case QtConversion::QT_TOCSV:    os << "csv"; break;
    case QtConversion::QT_TOJPEG:   os << "jpeg"; break;
    case QtConversion::QT_TOPNG:    os << "png"; break;
    case QtConversion::QT_TOVFF:    os << "vff"; break;
    case QtConversion::QT_TOTOR:    os << "tor"; break;
    case QtConversion::QT_TODEM:    os << "dem"; break;
    case QtConversion::QT_FROMTIFF: os << "inv_tiff"; break;
    case QtConversion::QT_FROMBMP:  os << "inv_bmp"; break;
    case QtConversion::QT_FROMHDF:  os << "inv_hdf"; break;
    case QtConversion::QT_FROMCSV:  os << "inv_csv"; break;
    case QtConversion::QT_FROMJPEG: os << "inv_jpeg"; break;
    case QtConversion::QT_FROMPNG:  os << "inv_png"; break;
    case QtConversion::QT_FROMVFF:  os << "inv_vff"; break;
    case QtConversion::QT_FROMTOR:  os << "inv_tor"; break;
    case QtConversion::QT_FROMDEM:  os << "inv_dem"; break;    
    default:      		    os << "unknown Conversion"; break;
  }

  return os;
}
  
