#ifndef __QTINSERT_HH__
#define __QTINSERT_HH___

#include "qlparser/qtexecute.hh"
#include "qlparser/qtoperation.hh"
#include "qlparser/qtmddconfig.hh"
#include "qlparser/qtmddcfgop.hh"
#include "storagemgr/sstoragelayout.hh"

#include "rasodmg/stattiling.hh"
#include "rasodmg/interesttiling.hh"

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif
#include <iostream>

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

*/

class QtInsert : public QtExecute
{
  public:
    /// constructor getting name of collection and insert expression
    QtInsert( const std::string& initCollectionName, QtOperation* initSource );

	QtInsert(const std::string& initCollectionName, QtOperation* initSource, QtOperation* storage);

    /// virtual destructor
    virtual ~QtInsert();

    /// method for evaluating the node
    virtual int evaluate();

    /// return childs of the node
    virtual QtNodeList* getChilds( QtChildType flag );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// method for query rewrite
    inline virtual void setInput( QtOperation* child, QtOperation* input);

	/// returns source
	QtOperation* getSource();

	/// tiling functions
	r_Data_Format getDataFormat(QtMDDConfig* config);
	r_Index_Type getIndexType(QtMDDConfig* config);
	r_Tiling_Scheme getTilingScheme(QtMDDConfig* cfg);
	vector<r_Minterval> getIntervals(QtMDDConfig* cfg);
	r_Minterval getTileConfig(QtMDDConfig* cfg);

	void setStorageLayout(StorageLayout* layout);

    /// pre optimization
//    virtual void preOptimize();
    /**
       Pre-Optimization step is passed to the input streams.
    */

    /// type checking
    virtual void checkType();
    
  private:
    /// insert expression
    QtOperation* source;

	// Storage and Tiling type
	QtOperation* stgLayout;

    /// collection name
    std::string collectionName;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;
};

#include "qlparser/qtinsert.icc"

#endif

