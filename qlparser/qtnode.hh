#ifndef _QTNODE_
#define _QTNODE_

#ifndef CPPSTDLIB
#include <vector.h>        // STL<ToolKit>
#include <list.h>          // STL<ToolKit>
#include <ospace/string.h> // STL<ToolKit>
#else
#include <vector>
#include <list>
#include <string>
#endif

#include "raslib/rmdebug.hh"
#include "raslib/sinterval.hh"
#include "qlparser/parseinfo.hh"
#include "qlparser/qtdata.hh"

// define used in lots of qlparser files to indent output
#ifdef CPPSTDLIB
#define SPACE_STR(numSpace) std::string(numSpace,' ')
#else
#define SPACE_STR(numSpace) std::string(' ',numSpace)
#endif

class QtOperation; // forward declarations of subclasses of QtNode
class Type;
class QtTypeElement;

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


//@ManMemo: Module: {\bf qlparser}

/**

 The class QtNode is the common super class of all node
 classes of the query tree. It provides a reference to its
 parent node in the tree. The parent of root is null.

*/


class QtNode
{
  public:
    /// list of QtData pointers
    typedef std::vector<QtData*> QtDataList;

    /// struct containing dimension and triming information
    struct QtTrimElement
    {
      r_Dimension dimension;
      r_Sinterval interval;
      bool intervalFlag;
    };

    /// list of QtTrimData structures
    typedef std::vector<QtTrimElement*> QtTrimList;

    /// list of QtNode pointers
    typedef std::list<QtNode*> QtNodeList;

    enum QtNodeType
    {
      QT_UNDEFINED_NODE,
      QT_MDD_ACCESS,
      QT_OPERATION_ITERATOR,
      QT_SELECTION_ITERATOR,
      QT_JOIN_ITERATOR,
      QT_UPDATE,
      QT_INSERT,
      QT_DELETE,
      QT_COMMAND,
      QT_PLUS,
      QT_MINUS,
      QT_MULT,
      QT_DIV,
      QT_OR,
      QT_AND,
      QT_XOR,
      QT_IS,
      QT_EQUAL,
      QT_NOT_EQUAL,
      QT_LESS,
      QT_LESS_EQUAL,
      QT_NOT,
      QT_SQRT,
// added by CStancuMara
	  QT_EXECUTE,
	  QT_ONC_STREAM,
	  QT_ITERATOR,
	  QT_OPERATION,
	  QT_BINARY_OPERATION,
	  QT_BINARY_INDUCE,
	  QT_GROUP_ITERATOR,
	  QT_IDENT,
	  QT_NARY_OPERATION,
	  QT_UNARY_OPERATION,
	  QT_CONDENSE,
	  QT_UNARY_INDUCE,
	  
//**************
      QT_ABS, QT_EXP, QT_LOG, QT_LN, QT_SIN, QT_COS, 
      QT_TAN, QT_SINH, QT_COSH, QT_TANH, QT_ARCSIN, 
      QT_ARCCOS, QT_ARCTAN,
      QT_REALPART,
      QT_IMAGINARPART,
      QT_CAST,
//**************

      QT_CSE_ROOT,
      QT_DOMAIN_OPERATION,
      QT_ALL,
      QT_SOME,
      QT_COUNTCELLS,
      QT_ADDCELLS,
      QT_AVGCELLS,
      QT_MINCELLS,
      QT_MAXCELLS,
      QT_MDD_VAR,
      QT_MDD_STREAM,
      QT_CONST,
      QT_DOT,
      QT_CONVERSION,
      QT_OID,
      QT_INTERVALOP,
      QT_MINTERVALOP,
      QT_POINTOP,
      QT_LO,
      QT_HI,
      QT_SDOM,
      QT_SHIFT,
      QT_EXTEND,
      //QT_MINTERVAL_SELECT,
      QT_MARRAYOP,
      QT_CONDENSEOP,
      QT_SCALE,
      QT_OVERLAY,
      QT_BIT,
      QT_PYRAMID,
      QT_LAST_NODE_TYPE
    };



    enum QtAreaType
    {
      QT_AREA_MDD,
      QT_AREA_SCALAR
    };

    enum QtChildType
    {
      QT_DIRECT_CHILDS,
      QT_LEAF_NODES,
      QT_ALL_NODES
    };

    /// list of QtOperation pointers
    typedef std::vector<QtOperation*> QtOperationList;

    /// default constructor
    QtNode();

    /// constructor getting a pointer to the parent
    QtNode( QtNode* node );

    /// destructor
    virtual ~QtNode();

	/// returns weather class b is a subtype of class a
	bool subtype( enum QtNodeType a, enum QtNodeType b );
	
    /// return childs of the node
    virtual QtNodeList* getChilds( QtChildType flag );
    /**
      The method allows different retrieval of the subtree nodes. Dependent on the content of {\tt flag}
      one of the following semantics is used:
      {\tt DIRECT_CHILDS} - A list of all direct child nodes of the current node is returned.
      {\tt LEAF_NODES   } - A list of all leaf nodes of the subtree with the current node as root is returned.
      {\tt ALL_NODES    } - A list of all nodes of the subtree with the current node as root is returned.
      The nodes in the result list have a special order. Every node comes before its parent node in
      the result list.
    */

    /// return childs of a certain class
    QtNodeList* getChild( const QtNodeType node, QtChildType flag = QT_DIRECT_CHILDS );
    /**
      The method allows to specify the class of childs to be considered according to method {\tt getChilds}.
      By default, just direct childs are considered
    */

    /// test if the two nodes have an equal meaning in a subtree
    virtual bool equalMeaning( QtNode* node );

    /**
      The method checks, if the two nodes have an equal meaning in a subtree.
      {\tt equalMeaning()} depends on the type of the node and the information in the node.
      For unary and binary operators the method {\tt equalMeaning()} is invoked on the input nodes.
    */

    /// creates a unique name for a common subexpression
    virtual std::string getSpelling();

    /**
      The method creates a unique name for common subexpressions by concatenating operators
      and variables in the subtree of the common subexpression.
    */

    /// test if the edge to the parent node is of type mdd or atomic
    virtual QtAreaType getAreaType();
    /**
      The method tests if the edge to the parent node belongs to a multidimensional or
      atomic area.
    */

    /// simplifies the tree
    virtual void simplify();
    /**
      The method evaluates constant expressions.
    */

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES )=0;

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout )=0;

    //@Man: Read/Write methods
    //@{
    ///

      /// 
      inline virtual void         setInput( QtOperation* inputOld, QtOperation* inputNew);
      ///
      inline QtNode*              getParent() const;
      ///
      inline void                 setParent( QtNode* node );
      ///
      inline const ParseInfo&     getParseInfo();
      ///
      inline void                 setParseInfo( const ParseInfo &info );

    ///
    //@}

    /// methods for identification of nodes
    inline virtual const QtNodeType getNodeType() const;
    /**
      The method allows you to differ between the different node types.
    */

    /// method for pre optimizations (basically load optimization)
    //virtual void preOptimize(){};
    /**
      {\em Load Optimization}
      The method is invoked through the whole query tree. The information of
      every triming and projection node is collected and stored in a {\tt QtTrimList}
      structure which is passed down. While collecting the information consistency
      of the data is checked. At the leafs, which are load operations, the structure
      is converted to a minterval object and stored in the leaf node.
      The list is dynamically created in the node which is on top of an operation subtree
      and deleted where the information is stored or dropped.

      Defined empty to do nothing in case it is not defined for a subclass.
    */
 
   /// returns the QtNodeType parent of the argument (do not use for the root)	
	enum QtNodeType getQtNodeTypeParent(enum QtNodeType);

	/// number of QtNodeTypes
	static const int QtNodes;
	
	/// the root of the inheritance tree
	static const QtNodeType QtRoot;
	
	/// the inheritance relations list	
	static const QtNodeType QtInheritance[][2];


  protected:
    /// attribute for parser info
    ParseInfo parseInfo;

  private:
    /// pointer to its parent
    QtNode* parent;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;

	/// (base_class, derived_class) pair	
	struct QtNodePair {
		enum QtNodeType base, deriv;
	};

	
	///operator overload for QtNodePair struct
	friend bool operator<( const QtNodePair a, const QtNodePair b);
	
	/// starting point of elements having node as base class
	static int child_range[];

	/// sets up the child_range[] array
	void set_child_range(const QtNodePair *arr);
	
	/// minim and maxim labels to determine subtypes
	static int minim[], maxim[];
 
	/// keeps track if SetMinMax was already called
	static bool MinMaxDone;

	/// sets minim and maxim values for each QtNodeType
	void SetMinMax();

	/// sets min max values once child_range is set up, for subtree with x as root
	void num_node (const QtNodePair *arr, enum QtNodeType x);
	
};




//@ManMemo: Module: {\bf qlparser}

/**

 The class encapsulates type information. It additionally
 can hold a name in order to be identifyable in a list.

 The type is specified by {\tt dataType} and {\tt type}.
 This is necessary because not all types are supported by
 subclasses of \Ref{Type}, e.g. not all types are persistent ones.

 In case of QT_MDD and QT_COMPLEX, further type information 
 can be optained from {\tt type}.

*/

class QtTypeElement
{
  public:
    ///
    QtTypeElement();

    ///
    QtTypeElement( const QtDataType initDataType, const char* initName = NULL ); 

    ///
    QtTypeElement( const Type* initType, const char* initName = NULL );

    ///
    QtTypeElement( const QtTypeElement& typeElement );

    ///
    ~QtTypeElement();
    
    /// assignment: cleanup + copy
    const QtTypeElement& operator= ( const QtTypeElement& );
    
    //@Man: Read/Write methods
    //@{
    ///
      ///
             void  setDataType( const QtDataType newDataType );
      ///
             void  setType( const Type* newType );
      ///
      inline void  setName( const char* newName );
      /// 
      inline const QtDataType getDataType() const;
      ///
      inline const Type*      getType() const;
      ///
      inline const char*      getName() const;
      ///
      inline       bool        isBaseType() const;
      ///
      inline       bool        isInteger() const;
    ///
    //@}
    ///

    /// print type
    void printStatus( std::ostream& s = std::cout ) const;

  private:
    ///
    QtDataType dataType;

    ///
    const Type*      type;

    ///
    char*      name;      
};



//@ManMemo: Module: {\bf qlparser}

/**

 The class encapsulates a tuple of \Ref{QtTypeElement}
 objects.

*/

class QtTypeTuple
{
  public:
    ///
    QtTypeTuple( unsigned int length = 0 );

    /// concatenate type tuple
    void concat( const QtTypeTuple& typeTuple );

    /// concatenate type element
    void concat( const QtTypeElement& typeElement );

    ///
    std::vector<QtTypeElement> tuple;

    /// print type
    void printStatus( std::ostream& s = std::cout ) const;
};

#include "qlparser/qtnode.icc"

#endif










