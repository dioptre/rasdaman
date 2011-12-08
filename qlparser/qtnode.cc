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

static const char rcsid[] = "@(#)qlparser, QtNode: $Id: qtnode.cc,v 1.27 2002/06/05 18:18:17 coman Exp $";

#include "qlparser/qtnode.hh"
#include "catalogmgr/typefactory.hh"
#include "relcatalogif/type.hh"
#include "relcatalogif/basetype.hh"

#include <iostream>
#include <algorithm>


const QtNode::QtNodeType QtNode::nodeType = QT_UNDEFINED_NODE;

	const int QtNode::QtNodes = 80;

	const QtNode::QtNodeType QtNode::QtRoot = QT_UNDEFINED_NODE;

	const QtNode::QtNodeType QtNode::QtInheritance[][2] = {
	{QT_UNDEFINED_NODE, QT_EXECUTE},
	{QT_EXECUTE, QT_COMMAND},
	{QT_EXECUTE, QT_DELETE},
	{QT_EXECUTE, QT_INSERT},
	{QT_EXECUTE, QT_PYRAMID},
	{QT_EXECUTE, QT_UPDATE},
	{QT_UNDEFINED_NODE, QT_ONC_STREAM},
	{QT_ONC_STREAM, QT_ITERATOR},
	{QT_ITERATOR, QT_JOIN_ITERATOR},
	{QT_ITERATOR, QT_OPERATION_ITERATOR},
	{QT_ITERATOR, QT_SELECTION_ITERATOR},
	{QT_ONC_STREAM, QT_MDD_ACCESS},
	{QT_UNDEFINED_NODE, QT_OPERATION},
	{QT_OPERATION, QT_BINARY_OPERATION},
	{QT_BINARY_OPERATION, QT_BINARY_INDUCE},
	{QT_BINARY_INDUCE, QT_AND},
	{QT_BINARY_INDUCE, QT_BIT},
	{QT_BINARY_INDUCE, QT_DIV},
	{QT_BINARY_INDUCE, QT_EQUAL},
	{QT_BINARY_INDUCE, QT_IS},
	{QT_BINARY_INDUCE, QT_LESS},
	{QT_BINARY_INDUCE, QT_LESS_EQUAL},
	{QT_BINARY_INDUCE, QT_MINUS},
	{QT_BINARY_INDUCE, QT_MULT},
	{QT_BINARY_INDUCE, QT_NOT_EQUAL},
	{QT_BINARY_INDUCE, QT_OR},
	{QT_BINARY_INDUCE, QT_OVERLAY},
	{QT_BINARY_INDUCE, QT_PLUS},
	{QT_BINARY_INDUCE, QT_XOR},
	{QT_BINARY_OPERATION, QT_CONDENSEOP},
	{QT_BINARY_OPERATION, QT_EXTEND},
	{QT_BINARY_OPERATION, QT_INTERVALOP},
	{QT_BINARY_OPERATION, QT_MARRAYOP},
	{QT_BINARY_OPERATION, QT_SCALE},
	{QT_BINARY_OPERATION, QT_SHIFT},
	{QT_OPERATION, QT_CONST},
	{QT_OPERATION, QT_GROUP_ITERATOR},
	{QT_OPERATION, QT_IDENT},
	{QT_OPERATION, QT_MDD_STREAM},
	{QT_OPERATION, QT_NARY_OPERATION},
	{QT_NARY_OPERATION, QT_MINTERVALOP},
	{QT_NARY_OPERATION, QT_POINTOP},
	{QT_OPERATION, QT_UNARY_OPERATION},
	{QT_UNARY_OPERATION, QT_CONDENSE},
	{QT_CONDENSE, QT_ADDCELLS},
	{QT_CONDENSE, QT_ALL},
	{QT_CONDENSE, QT_AVGCELLS},
	{QT_CONDENSE, QT_COUNTCELLS},
	{QT_CONDENSE, QT_MAXCELLS},
	{QT_CONDENSE, QT_MINCELLS},
	{QT_CONDENSE, QT_SOME},
	{QT_UNARY_OPERATION, QT_CONVERSION},
	{QT_UNARY_OPERATION, QT_CSE_ROOT},
	{QT_UNARY_OPERATION, QT_DOMAIN_OPERATION},
	{QT_UNARY_OPERATION, QT_HI},
	{QT_UNARY_OPERATION, QT_LO},
	{QT_UNARY_OPERATION, QT_OID},
	{QT_UNARY_OPERATION, QT_SDOM},
	{QT_UNARY_OPERATION, QT_UNARY_INDUCE},
	{QT_UNARY_INDUCE, QT_CAST},
	{QT_UNARY_INDUCE, QT_DOT},
	{QT_UNARY_INDUCE, QT_IMAGINARPART},
	{QT_UNARY_INDUCE, QT_NOT},
	{QT_UNARY_INDUCE, QT_REALPART},
	{QT_UNARY_INDUCE, QT_ABS},
	{QT_UNARY_INDUCE, QT_SQRT},
	{QT_UNARY_INDUCE, QT_EXP},
	{QT_UNARY_INDUCE, QT_LOG},
	{QT_UNARY_INDUCE, QT_LN},
	{QT_UNARY_INDUCE, QT_SIN},
	{QT_UNARY_INDUCE, QT_COS},
	{QT_UNARY_INDUCE, QT_TAN},
	{QT_UNARY_INDUCE, QT_SINH},
	{QT_UNARY_INDUCE, QT_COSH},
	{QT_UNARY_INDUCE, QT_TANH},
	{QT_UNARY_INDUCE, QT_ARCSIN},
	{QT_UNARY_INDUCE, QT_ARCCOS},
	{QT_UNARY_INDUCE, QT_ARCTAN},
	{QT_OPERATION, QT_MDD_VAR}
};



int QtNode::minim[QtNodes];
int QtNode::maxim[QtNodes];
int QtNode::child_range[QtNodes+1];

bool QtNode::MinMaxDone = false;
	
QtNode::QtNode()
  : parent(NULL)
{
  if (!MinMaxDone) {
	  MinMaxDone = true;
	  SetMinMax();
  }
}


QtNode::QtNode( QtNode* node )
  : parent( node )
{
  if (!MinMaxDone) {
	  MinMaxDone = true;
	  SetMinMax();
  }
}


QtNode::~QtNode()
{
}

bool
QtNode::subtype( enum QtNodeType a, enum QtNodeType b )
{
  return (minim[a]<=minim[b] && maxim[b]<=maxim[a]);
}


QtNode::QtNodeList*
QtNode::getChilds( QtChildType flag )
{
  RMDBCLASS( "QtNode", "getChilds( QtChildType )", "qlparser", __FILE__, __LINE__ )

  // Default definition is used for all leaf nodes.

  // Algorithm:
  //
  // In mode QT_ALL_NODES each node calls getChild() of every son,
  // merges these lists and inserts direct childs but not the node
  // itself.
  // In mode QT_DIRECT_CHILDS each node inserts just direct childs.
  // In mode QT_LEAF_NODES the method call is passed to the sons
  // and just the leaf nodes insert themselves. 

  QtNodeList* resultList=NULL;

  resultList = new QtNodeList();

  if( flag == QT_LEAF_NODES )
    resultList->push_back( this );

  return resultList;
}


QtNode::QtNodeList*
QtNode::getChild( const QtNodeType node, QtChildType flag )
{
  RMDBCLASS( "QtNode", "getChild( QtChildType )", "qlparser", __FILE__, __LINE__ )

  QtNodeList* resultList=NULL;
  QtNodeList::iterator iter;

  resultList = getChilds( flag );

  for( iter=resultList->begin(); iter!=resultList->end(); iter++ )
    if( node != (*iter)->getNodeType() )
      resultList->erase( iter-- );

  return resultList;
}


bool
QtNode::equalMeaning( QtNode* /*node*/ )
{
  return false;
}


std::string
QtNode::getSpelling()
{
  return "";
}

QtNode::QtAreaType
QtNode::getAreaType()
{
  return QT_AREA_SCALAR;
}



void
QtNode::simplify()
{
  RMDBCLASS( "QtNode", "simplify()", "qlparser", __FILE__, __LINE__ )

  // Default method for all classes that have no implementation.
  // Method is used bottom up.

  QtNodeList* resultList=NULL;
  QtNodeList::iterator iter;

  try
  {
    resultList = getChilds( QT_DIRECT_CHILDS );
    for( iter=resultList->begin(); iter!=resultList->end(); iter++ )
      (*iter)->simplify();
  }
  catch(...)
  {
    if( resultList ) 
    {
      delete resultList;
      resultList=NULL;
    }
    throw;
  }

  delete resultList;
  resultList=NULL;
}


QtNode::QtNodeType 
QtNode::getQtNodeTypeParent (QtNode::QtNodeType node) {
	int i = 0;
	for (i=0; i<QtNodes; i++)
		if ( QtInheritance[i][1] == node ) return QtInheritance[i][0];
	throw r_Error(QTNODETYPEPARENTDOESNOTEXIST);
}


bool 
operator<(const QtNode::QtNodePair a, const QtNode::QtNodePair b){
		return (a.base < b.base);
}

void 
QtNode::num_node (const QtNodePair *arr, const enum QtNodeType x) {
		int i;
		static int ID = 0 ;
		enum QtNodeType child;
		minim[x] = ID++;
		for (i = child_range[x]; i < child_range[x+1]; i++) 
			num_node(arr, arr[i].deriv);
		maxim[x] = ID++;
}

void
QtNode::set_child_range(const QtNodePair *arr) {
	int i;
	child_range[QtNodes] = QtNodes-1;
	for (i=QtNodes-3; i>=0; i--) 
		if (arr[i].base != arr[i+1].base) child_range[arr[i+1].base] = i+1;
	child_range[arr[0].base] = 0;
	for (i=QtNodes-1; i>0; i--)
		if (child_range[i] == 0) child_range[i] = child_range[i+1];
}

void 
QtNode::SetMinMax()
{
	int i;
	QtNodePair arr[QtNodes-1];
	for (i=0; i<(QtNodes-1); i++) {
			arr[i].base = QtInheritance[i][0];
			arr[i].deriv = QtInheritance[i][1];
		}
	std::sort(arr,arr+QtNodes-1);
	//creating child_range
	set_child_range(arr);
	//numbering the nodes
	num_node(arr, QtRoot);	
}


QtTypeElement::QtTypeElement()
  : dataType( QT_TYPE_UNKNOWN ),
    type(NULL),
    name(NULL)
{
}



QtTypeElement::QtTypeElement( const QtDataType initDataType,
                              const char*      initName )
  : dataType( QT_TYPE_UNKNOWN ),
    type( NULL ),
    name( NULL )
{
  setDataType( initDataType );

  if( initName ) name = strdup( initName );
}



QtTypeElement::QtTypeElement( const Type*            initType,
                              const char*      initName )
  : dataType( QT_TYPE_UNKNOWN ),
    type( NULL ),
    name( NULL )
{
  setType( initType );

  if( initName ) name = strdup( initName );
}



QtTypeElement::QtTypeElement( const QtTypeElement& typeElement )
  : dataType( typeElement.dataType ),
    type( typeElement.type ),
    name(NULL)
{
  if( typeElement.name ) name = strdup( typeElement.name );
}



QtTypeElement::~QtTypeElement()
{
  // Note: Types are free by the type factory.

  if( name )
  {
    free( name );
    name=NULL;
  }
}



const QtTypeElement& 
QtTypeElement::operator=( const QtTypeElement& obj )
{
  if( this != &obj )
  {
    if( name )
    {
      free( name );
      name = NULL;
    }

    dataType = obj.dataType;
    type     = obj.type;

    if( obj.name ) name = strdup( obj.name );

  }

  return *this;
}



void
QtTypeElement::printStatus( std::ostream& s ) const
{
  if( type )
  {
    char* typeStructure = type->getTypeStructure();
    s << typeStructure << std::flush;
    free( typeStructure );
    typeStructure=NULL;
  }
  else
  {
    switch( dataType )
    {
      case QT_STRING:
        s << "string" << std::flush;
        break;

      case QT_INTERVAL:
        s << "interval" << std::flush;
        break;

      case QT_MINTERVAL:
        s << "minterval" << std::flush;
        break;

      case QT_POINT:
        s << "point" << std::flush;
        break;

      default:
        // including case QT_TYPE_UNKNOWN
        s << "<unknown type>" << std::flush;
        break;
    }
  }

  if( name )
    s << ":" << name << std::flush;
}



void 
QtTypeElement::setDataType( const QtDataType newDataType )
{
  dataType = newDataType;

  // reset type information
  type = NULL;

  switch( dataType )
  {
    case QT_TYPE_UNKNOWN:
      break;
    case QT_BOOL:
      type = TypeFactory::mapType("Bool");
      break;
    case QT_CHAR:
      type = TypeFactory::mapType("Char");
      break;
    case QT_OCTET:
      type = TypeFactory::mapType("Octet");
      break;
    case QT_USHORT:
      type = TypeFactory::mapType("UShort");
      break;
    case QT_SHORT:
      type = TypeFactory::mapType("Short");
      break;
    case QT_ULONG:
      type = TypeFactory::mapType("ULong");
      break;
    case QT_LONG:
      type = TypeFactory::mapType("Long");
      break;
    case QT_FLOAT:
      type = TypeFactory::mapType("Float");
      break;
    case QT_DOUBLE:
      type = TypeFactory::mapType("Double");
      break;
    case QT_COMPLEXTYPE1:
      type = TypeFactory::mapType("Complex1");
      break;
    case QT_COMPLEXTYPE2:
      type = TypeFactory::mapType("Complex2");
      break;

    case QT_MDD:
    case QT_COMPLEX:
      RMInit::logOut << "QtTypeElement::setDataType() - MDD and complex types need to be specified further." << std::endl;
      break;

    case QT_STRING:
    case QT_INTERVAL:
    case QT_MINTERVAL:
    case QT_POINT:
    default:
      // transient types
      break;
  }
}



void 
QtTypeElement::setType(const Type* newType )
{
  type     = NULL;
  dataType = QT_TYPE_UNKNOWN;
  
  if( newType )
    switch( newType->getType() )
    {
      case BOOLTYPE: dataType = QT_BOOL;    break;
      case CHAR:     dataType = QT_CHAR;    break;
      case OCTET:    dataType = QT_OCTET;   break;
      case USHORT:   dataType = QT_USHORT;  break;
      case SHORT:    dataType = QT_SHORT;   break;
      case ULONG:    dataType = QT_ULONG;   break;
      case LONG:     dataType = QT_LONG;    break;
      case FLOAT:    dataType = QT_FLOAT;   break;
      case DOUBLE:   dataType = QT_DOUBLE;  break;
      case COMPLEXTYPE1:  dataType = QT_COMPLEXTYPE1;   break;
      case COMPLEXTYPE2: dataType = QT_COMPLEXTYPE2;  break;
      case MDDTYPE:  dataType = QT_MDD;     break;
      case STRUCT:   dataType = QT_COMPLEX; break;
      default:   dataType = QT_TYPE_UNKNOWN; break;
    }

  // if type is supported
  if( dataType != QT_TYPE_UNKNOWN )
    type = newType;
}



QtTypeTuple::QtTypeTuple( unsigned int length )
  : tuple( length )
{
}



void 
QtTypeTuple::concat( const QtTypeTuple& typeTuple )
{       
  // reserve space for concatenated type
  tuple.reserve( tuple.size() + typeTuple.tuple.size() );

  // concatenate tuples
  for( std::vector<QtTypeElement>::const_iterator iter = typeTuple.tuple.begin();
       iter != typeTuple.tuple.end(); iter++ )
    tuple.push_back( *iter );
}



void 
QtTypeTuple::concat( const QtTypeElement& typeElement )
{       
  tuple.push_back( typeElement );
}



void
QtTypeTuple::printStatus( std::ostream& s ) const
{
  s << "<" << std::flush;

  for( std::vector<QtTypeElement>::const_iterator iter = tuple.begin();
       iter != tuple.end();
       iter++ )
  {
    if( iter != tuple.begin() )
      s << ", " << std::flush;

    (*iter).printStatus( s );
  }

  s << ">" << std::flush;
}




