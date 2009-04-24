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

static const char rcsid[] = "@(#)qlparser, QtCommand: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtcommand.cc,v 1.21 2005/09/03 20:17:55 rasdev Exp $";

#include "qlparser/qtcommand.hh"
#include "mddmgr/mddcoll.hh"
#include "catalogmgr/typefactory.hh"
#include "reladminif/databaseif.hh"
#include "relcatalogif/settype.hh"
#include "servercomm/servercomm.hh"

#include <iostream>


extern ServerComm::ClientTblElt* currentClientTblElt;

const QtNode::QtNodeType QtCommand::nodeType = QtNode::QT_COMMAND;



QtCommand::QtCommand( QtCommandType initCommand, const std::string& initCollection, const std::string& initType )
  : QtExecute(), 
    command( initCommand ),
    collectionName( initCollection ),
    typeName( initType )
{
}



QtCommand::QtCommand( QtCommandType initCommand, const std::string& initCollection )
  : QtExecute(), 
    command( initCommand ),
    collectionName( initCollection )
{
}



int
QtCommand::evaluate()
{
    RMDBGENTER(2, RMDebug::module_qlparser, "QtCommand", "evaluate()")

  switch( command )
  {
    case QT_DROP_COLLECTION:
      if( currentClientTblElt )
        if (!MDDColl::dropMDDCollection(collectionName.c_str()))
        {
          RMInit::logOut << "Error during query evaluation: collection name not found: " << collectionName.c_str() << std::endl;
          parseInfo.setErrorNo(957);
          throw parseInfo;
        }       
        break;
    case QT_CREATE_COLLECTION:
      if( currentClientTblElt )
      {
        // get collection type
        CollectionType* collType = (CollectionType*)TypeFactory::mapSetType( (char*)typeName.c_str() );

        if( collType )
        {
          // allocate a new oid within the current db
          OId oid;
#ifdef BASEDB_O2
          if( !OId::allocateMDDCollOId( &oid ) )
	  {
#else
          OId::allocateOId(oid, OId::MDDCOLLOID);
#endif
            try
            {
              MDDColl* coll = MDDColl::createMDDCollection(collectionName.c_str(), oid, collType);
              delete coll;
              coll=NULL;
            }
            catch( r_Error& obj )
            {
              RMInit::logOut << "Error during query evaluation: collection name exists already: " << collectionName.c_str() << std::endl;
              if (obj.get_kind() != r_Error::r_Error_NameNotUnique)
              	RMInit::logOut << "Exception: " << obj.what() << std::endl;;
              parseInfo.setErrorNo(955);
              throw parseInfo;
            }    
#ifdef BASEDB_O2
          }
          else
          {
            RMInit::logOut << "Error: QtCommand::evaluate() - oid allocation failed" << std::endl;
            parseInfo.setErrorNo(958);
            throw parseInfo;
          }    
#endif
        }
        else
        {
          RMInit::logOut << "Error during query evaluation: collection type not found: " << (char*)typeName.c_str() << std::endl;
          parseInfo.setErrorNo(956);
          throw parseInfo;
        }
        break;
      }
  }
  
  RMDBGEXIT(2, RMDebug::module_qlparser, "QtCommand", "evaluate()")

  return 0;
}



void
QtCommand::printTree( int tab, std::ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtCommand Object" << std::endl;

  switch( command )
  {
    case QT_DROP_COLLECTION:   s << SPACE_STR(tab).c_str() << "  drop collection("   << collectionName.c_str() << ")"; break;
    case QT_CREATE_COLLECTION: s << SPACE_STR(tab).c_str() << "  create collection(" << collectionName.c_str() << ", " << typeName.c_str() <<")"; break;
    default: s << "<command unknown>";
  }
  
  s << std::endl;    
}



void
QtCommand::printAlgebraicExpression( std::ostream& s )
{
  s << "command<";

  switch( command )
  {
    case QT_DROP_COLLECTION:   s << "drop collection("   << collectionName.c_str() << ")"; break;
    case QT_CREATE_COLLECTION: s << "create collection(" << collectionName.c_str() << ", " << typeName.c_str() <<")"; break;
    default: s << "unknown";
  }
    
  s << ">";
}



void
QtCommand::checkType()
{
  // nothing to do here
}

