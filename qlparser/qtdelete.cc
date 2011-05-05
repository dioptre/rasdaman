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
 * COMMENTS:
 *
 ************************************************************/

static const char rcsid[] = "@(#)qlparser, QtDelete: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtdelete.cc,v 1.14 2003/12/27 20:51:28 rasdev Exp $";

#include "qlparser/qtdelete.hh"
#include "qlparser/qtdata.hh"
#include "qlparser/qtmdd.hh"
#include "qlparser/qtmddaccess.hh"
#include "mddmgr/mddobj.hh"
#include "mddmgr/mddcoll.hh"

#include <iostream>



const QtNode::QtNodeType QtDelete::nodeType = QtNode::QT_DELETE;



QtDelete::QtDelete()
  : QtExecute(), input(NULL)
{
}



QtDelete::~QtDelete()
{
  if( input )
  {
    delete input;
    input=NULL;
  }
}



int
QtDelete::evaluate()
{
  RMDBCLASS( "QtDelete", "evaluate()", "qlparser", __FILE__, __LINE__ )

  QtNode::QtDataList* nextTupel=NULL;

  // open input stream
  try
  {
    input->open();
  }
  catch( ... )
  {
    input->close();
    throw;
  }

  try
  {
    while( (nextTupel = input->next()) )
    {
      QtData* target = (*nextTupel)[0];

      // check delete target
      if( target->getDataType() != QT_MDD )
      {
        RMInit::logOut << "Error: QtDelete::evaluate() - delete target must be an expression resulting in an r_Marray<>" << std::endl;

        // delete tupel vector received by next()
        vector<QtData*>::iterator dataIter;
        for( dataIter=nextTupel->begin(); dataIter!=nextTupel->end(); dataIter++ )
          if( *dataIter ) (*dataIter)->deleteRef();
        delete nextTupel;
        nextTupel=NULL;

        parseInfo.setErrorNo(951);
        throw parseInfo;
      }
        
      QtMDD*  targetMDD = (QtMDD*) target;
      MDDObj* targetObj = targetMDD->getMDDObject();

      // get leaf of ONC input
      QtNodeList*  leafList = input->getChilds( QtNode::QT_LEAF_NODES );
      QtMDDAccess* inputLeaf = NULL;
      // take the last QtMDDAccess object from the list assuming that it is the right one
      for( std::list<QtNode*>::iterator iter=leafList->begin(); iter!=leafList->end(); iter++ )
        if( (*iter)->getNodeType() == QT_MDD_ACCESS )
          inputLeaf = (QtMDDAccess*)*iter;      
      delete leafList;
      leafList=NULL;

      // delete the MDD object
      if( inputLeaf )
      {
        inputLeaf->getMDDColl()->remove( targetObj );
        RMInit::logOut << "deleted..." << std::flush;
      }

      if(targetMDD && targetObj && !targetObj->isPersistent());
         targetMDD->setLifetime( QtData::QT_TRANSIENT );

      // delete tupel vector received by next()
      vector<QtData*>::iterator dataIter;
      for( dataIter=nextTupel->begin(); dataIter!=nextTupel->end(); dataIter++ )
        if( *dataIter ) (*dataIter)->deleteRef();
      delete nextTupel;
      nextTupel=NULL;
    } // while

  }
  catch( ... )
  {
    input->close();
    throw;
  }

  input->close();

  return 0;
}



QtNode::QtNodeList*
QtDelete::getChilds( QtChildType flag )
{
  QtNodeList* resultList=NULL;

  if( input )
  {
    // allocate resultList
    if( flag == QT_DIRECT_CHILDS );
      resultList = new QtNodeList();

    if( flag == QT_LEAF_NODES || flag == QT_ALL_NODES )
      resultList = input->getChilds( flag );

    // add the nodes of the current level
    if( flag == QT_DIRECT_CHILDS || flag == QT_ALL_NODES )
      resultList->push_back( input );
  }
  else
    resultList = new QtNodeList();

  return resultList;
}



void
QtDelete::printTree( int tab, std::ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtDelete Object" << std::endl;

  if( mode != QtNode::QT_DIRECT_CHILDS )
  {
    if( input )
    {
      s << SPACE_STR(tab).c_str() << "input: " << std::endl;
      input->printTree( tab+2, s, mode );
    }
    else
      s << SPACE_STR(tab).c_str() << "no input" << std::endl;

    s << std::endl;
  }
}



void
QtDelete::printAlgebraicExpression( std::ostream& s )
{
  s << "delete";

  if( input )
  {
    s << "( ";
    input->printAlgebraicExpression( s );
    s << " )";
  }
  else
    s << "(<no input>)";
}



void
QtDelete::setStreamInput( QtONCStream* newInput )
{
  input = newInput;
  input->setParent( this );
};

QtONCStream*
QtDelete::getStreamInput()
{
	return input;
}


/*
void
QtDelete::preOptimize()
{
  if( input )
    input->preOptimize();
}
*/


void
QtDelete::checkType()
{
  RMDBCLASS( "QtDelete", "checkType()", "qlparser", __FILE__, __LINE__ )

  // check operand branches
  if( input )
  {

  // get input type
  QtTypeTuple inputType  = input->checkType(); 

  if( inputType.tuple[0].getDataType() != QT_MDD )
  {
    RMInit::logOut << "Error: QtDelete::checkType() - delete target must be an expression resulting in an r_Marray<>" << std::endl;
    parseInfo.setErrorNo(951);
    throw parseInfo;
  }
  }
  else
    RMInit::logOut << "Error: QtDelete::checkType() - operand branch invalid." << std::endl;

}








