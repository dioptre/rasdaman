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
 * - QtScale is expected to have rounding errors with >1 tiles
 * - shift, extend, scale release input tiles only at end; this
 *   shouldbe optimized (release immediately after evaluating)
 * - why is this file called "binary"? all ops have just one MDD!
 * - QtShift(), QtExtend() deliver for >1 tiles under PG an error
 *   "libpq 'select' did not yield 1 result but 0"
 *   which however does not seem to affect the result.
 *
 ************************************************************/

static const char rcsid[] = "@(#)qlparser, QtMintervalSelect, QtShift: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtbinaryfunc.cc,v 1.37 2005/09/03 20:17:55 rasdev Exp $";

#include "mymalloc/mymalloc.h"

#include "mymalloc/mymalloc.h"

#include "qlparser/qtbinaryfunc.hh"
#include "qlparser/qtdata.hh"
#include "qlparser/qtintervaldata.hh"
#include "qlparser/qtmintervaldata.hh"
#include "qlparser/qtatomicdata.hh"
#include "qlparser/qtmdd.hh"
#include "qlparser/qtpointdata.hh"

#include "mddmgr/mddobj.hh"
#include "tilemgr/tile.hh"

#include "raslib/rmdebug.hh"
#include "raslib/dlist.hh"

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif


// --- QtShift --------------------------------------------------

const QtNode::QtNodeType QtShift::nodeType = QT_SHIFT;

QtShift::QtShift( QtOperation* mddOp, QtOperation* pointOp )
	:  QtBinaryOperation( mddOp, pointOp )
{
}



bool
QtShift::isCommutative() const
{
	return false; // NOT commutative
}



QtData*
QtShift::evaluate( QtDataList* inputList )
{
	RMDBCLASS( "QtShift", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

	QtData* returnValue = NULL;
	QtData* operand1 = NULL;
	QtData* operand2 = NULL;

	// evaluate sub-nodes to obtain operand values
	if( getOperands( inputList, operand1, operand2 ) )
	{
		//
		// This implementation simply creates a new transient MDD object with the new
		// domain while copying the data. Optimization of this is left for future work.
		//

		QtMDD*         qtMDDObj          = (QtMDD*)operand1;
		const r_Point& transPoint        = ((QtPointData*)operand2)->getPointData();
		MDDObj*        currentMDDObj     = qtMDDObj->getMDDObject();

		if( transPoint.dimension() != qtMDDObj->getLoadDomain().dimension() )
		{
			// delete the old operands
			if( operand1 ) operand1->deleteRef();
			if( operand2 ) operand2->deleteRef();

			RMInit::logOut << "Error: QtShift::evaluate( QtDataList* ) - dimensionality of MDD and point expression do not match." << endl;
			parseInfo.setErrorNo(407);
			throw parseInfo;          
		}

		// compute new domain
		r_Minterval destinationDomain( qtMDDObj->getLoadDomain().create_translation( transPoint ) );

		// create a transient MDD object for the query result
		MDDObj* resultMDD = new MDDObj( currentMDDObj->getMDDBaseType(), destinationDomain );

		// get all tiles
		vector<Tile* >* tiles = currentMDDObj->intersect( qtMDDObj->getLoadDomain() );

		// iterate over source tiles
		for( vector<Tile*>::iterator tileIter = tiles->begin(); tileIter != tiles->end(); tileIter++ )
		{
			// get relevant area of source tile
			r_Minterval sourceTileDomain = qtMDDObj->getLoadDomain().create_intersection( (*tileIter)->getDomain() );

			// compute translated tile domain
			r_Minterval destinationTileDomain = sourceTileDomain.create_translation( transPoint );

			// create a new transient tile, copy the transient data, and insert it into the mdd object
			// FIXME: how can this work without tile area allocation??? -- PB 2005-jun-19
			Tile* newTransTile = new Tile( destinationTileDomain, currentMDDObj->getCellType() );
			newTransTile->copyTile( destinationTileDomain, *tileIter, sourceTileDomain );
			resultMDD->insertTile( newTransTile );
		}

		// create a new QtMDD object as carrier object for the transient MDD object
		returnValue = new QtMDD( (MDDObj*)resultMDD );

		// delete the tile vector, the tiles itself are deleted when the destructor
		// of the MDD object is called
		delete tiles;
		tiles=NULL;

		// delete the old operands
		if( operand1 ) operand1->deleteRef();
		if( operand2 ) operand2->deleteRef();
	}

	return returnValue;
}



void
QtShift::printTree( int tab, ostream& s, QtChildType mode )
{
	s << SPACE_STR(tab).c_str() << "QtShift Object " << getNodeType() << endl;

	QtBinaryOperation::printTree( tab, s, mode );
}



void
QtShift::printAlgebraicExpression( ostream& s )
{
	s << "shift(";

	if( input1 )
		input1->printAlgebraicExpression( s );
	else
		s << "<nn>";

	s << ",";

	if( input2 )
		input2->printAlgebraicExpression( s );
	else
		s << "<nn>";

	s << ")";
}



void
QtShift::optimizeLoad( QtTrimList* trimList )
{
	RMDBCLASS( "QtShift", "optimizeLoad( QtTrimList* )", "qlparser", __FILE__, __LINE__ )

	QtNode::QtTrimList *list1=NULL, *list2=NULL;

	if( input1 && input2 )
	{
		QtNode::QtTrimList::iterator iter;

		//
		// The result of input2 has to be a constant expression.
		//

		// shift of trimList is just possible, if no open bounds are available
		bool openBounds = false;
		for( iter=trimList->begin(); iter!=trimList->end() && !openBounds; iter++ )
			openBounds = !((*iter)->interval.is_low_fixed()) || !((*iter)->interval.is_high_fixed());

		if( openBounds )
		{
			// release( trimList->begin(), trimList->end() );
			for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
			{
				delete *iter;
				*iter=NULL;
			}
			delete trimList;
			trimList=NULL;

			RMInit::logOut << "Error: QtShift::optimizeLoad() - spatial domain shift of open bounds is not supported" << endl;
			parseInfo.setErrorNo(409);
			throw parseInfo;    
		}

		QtData* operand = input2->evaluate(NULL);

		if( !operand )
		{
			// release( trimList->begin(), trimList->end() );
			for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
			{
				delete *iter;
				*iter=NULL;
			}
			delete trimList;
			trimList=NULL;

			RMInit::logOut <<  "Error: QtShift::optimizeLoad() - second operand of shift function must be a constant expression." << endl;
			parseInfo.setErrorNo(408);
			throw parseInfo;    
		}

		if( operand->getDataType() != QT_POINT )
		{
			// release( trimList->begin(), trimList->end() );
			for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
			{
				delete *iter;
				*iter=NULL;
			}
			delete trimList;
			trimList=NULL;

			operand->deleteRef();

			RMInit::logOut << "Error: QtShift::optimizeLoad() - second operand must be of type Point." << endl;
			parseInfo.setErrorNo(406);
			throw parseInfo;    
		}
		
		// get transPoint
		const r_Point& transPoint = ((QtPointData*)operand)->getPointData();

		// shift trim elements by -transPoint
		for( iter=trimList->begin(); iter!=trimList->end(); iter++ )
		{
			QtTrimElement* elem = *iter;

			if( elem->dimension <= transPoint.dimension() )
				elem->interval.set_interval( elem->interval.low()  - transPoint[elem->dimension], elem->interval.high() - transPoint[elem->dimension] );
		}

		// point is not needed anymore
		operand->deleteRef();

		input1->optimizeLoad( trimList );
	}
	else 
	{
		// release( trimList->begin(), trimList->end() );
		for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
		{
			delete *iter;
			*iter=NULL;
		}
		delete trimList;
		trimList=NULL;
	}
}



const QtTypeElement&
QtShift::checkType( QtTypeTuple* typeTuple )
{
	RMDBCLASS( "QtShift", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

	dataStreamType.setDataType( QT_TYPE_UNKNOWN );  

	// check operand branches
	if( input1 && input2 )
	{

	// get input types
	const QtTypeElement& inputType1 = input1->checkType( typeTuple ); 
	const QtTypeElement& inputType2 = input2->checkType( typeTuple ); 

	if( inputType1.getDataType() != QT_MDD )
	{
		RMInit::logOut << "Error: QtShift::checkType() - first operand must be of type MDD." << endl;
		parseInfo.setErrorNo(405);
		throw parseInfo;    
	}

	if( inputType2.getDataType() != QT_POINT )
	{
		RMInit::logOut << "Error: QtShift::checkType() - second operand must be of type Point." << endl;
		parseInfo.setErrorNo(406);
		throw parseInfo;    
	}

	// pass MDD type
	dataStreamType = inputType1;
	}
	else
		RMInit::logOut << "Error: QtShift::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}


// --- QtExtend --------------------------------------------------

const QtNode::QtNodeType QtExtend::nodeType = QT_EXTEND;

QtExtend::QtExtend( QtOperation* mddOp, QtOperation* mintervalOp )
	:  QtBinaryOperation( mddOp, mintervalOp )
{
}



bool
QtExtend::isCommutative() const
{
	return false; // NOT commutative
}



QtData*
QtExtend::evaluate( QtDataList* inputList )
{
	RMDBCLASS( "QtExtend", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

	QtData* returnValue = NULL;	// operation result
	QtData* operand1 = NULL;	// 1st operand: MDD expression
	QtData* operand2 = NULL;	// 2nd operand: Minterval expression
	vector<Tile*> completeAreaList;	// list of tiles comprising the whole area (possibly with holes); needed for 1-code below

	if( getOperands( inputList, operand1, operand2 ) )
	{
		//
		// This implementation simply creates a single new transient MDD object with the new
		// domain while copying the data.
		// FIXME: create a tiled object
		//

		QtMDD*      qtMDDObj      = (QtMDD*)operand1;					// object to be extended
		r_Minterval targetDomain  = ((QtMintervalData*)operand2)->getMintervalData();	// new domain of extended object
		MDDObj*     currentMDDObj = qtMDDObj->getMDDObject();

		// precondition checks (we call the MDD C and the Minterval M):
		// - dim(C) == dim(M)
		if( targetDomain.dimension() != qtMDDObj->getLoadDomain().dimension() )
		{
			// delete the old operands
			if( operand1 ) operand1->deleteRef();
			if( operand2 ) operand2->deleteRef();

			RMInit::logOut << "Error: QtExtend::evaluate( QtDataList* ) - dimensionality of MDD and point expression do not match." << endl;
			parseInfo.setErrorNo(407);
			throw parseInfo;          
		}

		// - M does not contain open bounds (i.e., "*")
		if( ! targetDomain.is_origin_fixed() || ! targetDomain.is_high_fixed() )
		{
			// delete the old operands
			if( operand1 ) operand1->deleteRef();
			if( operand2 ) operand2->deleteRef();

			RMInit::logOut << "Error: QtExtend::evaluate( QtDataList* ) - target domain must not have open bounds." << endl;
			parseInfo.setErrorNo(420);
			throw parseInfo;    
		}
		// - M.subset( sdom(C) ); can we relieve this?
		if( ! targetDomain.covers( qtMDDObj->getLoadDomain() ) )
		{
			// delete the old operands
			if( operand1 ) operand1->deleteRef();
			if( operand2 ) operand2->deleteRef();

			RMInit::logOut << "Error: QtExtend::evaluate( QtDataList* ) - new interval does not cover MDD to be extended." << endl;
			parseInfo.setErrorNo(421);
			throw parseInfo;          
		}

		// RMInit::logOut << "QtExtend::evaluate( QtDataList* ) - extending MDD with basetype " << currentMDDObj->getMDDBaseType() << " and load domain " << qtMDDObj->getLoadDomain() << " to domain " << targetDomain << endl;

		// create a transient MDD object for the query result
		MDDObj* resultMDD = new MDDObj( currentMDDObj->getMDDBaseType(), targetDomain );

		// --- 1: put all existing tiles into their place ------------------------

		// get all tiles
		vector<Tile* >* tiles = currentMDDObj->intersect( qtMDDObj->getLoadDomain() );

		// iterate over source tiles
		// Note that source and target MDD have the same coordinate basis
		for( vector<Tile*>::iterator tileIter = tiles->begin(); tileIter != tiles->end(); tileIter++ )
		{
			// RMInit::logOut << "QtExtend::evaluate( QtDataList* ) - load domain is " << qtMDDObj->getLoadDomain() << endl;
			// get relevant area of source tile
			r_Minterval sourceTileDomain = qtMDDObj->getLoadDomain().create_intersection( (*tileIter)->getDomain() );

			Tile* newTransTile = new Tile( sourceTileDomain, currentMDDObj->getCellType() );
			// RMInit::logOut << "QtExtend::evaluate( QtDataList* ) - adding source part " << sourceTileDomain << " of tile " << (*tileIter)->getDomain() << endl;
			newTransTile->copyTile( sourceTileDomain, *tileIter, sourceTileDomain );

			resultMDD->insertTile( newTransTile ); // needed for 2-code below
			// completeAreaList.push_back( newTransTile ); // needed for 1-code below
		}

		// --- 2: fill up new space with null values -----------------------------

#if 0	// this 1-code does the same thing as the 2-code, but easier & more efficiently -- PB 2005-jun-24
	// INCOMPLETE!
		// create minimal (1x1) tiles at origin and high end, but only if the source domain isn't there
		if (targetDomain.get_origin() != qtMDDObj->getLoadDomain().get_origin())
		{
			RMInit::logOut << "QtExtend::evaluate( QtDataList* ) - adding aux tile at origin." << endl;
->			Tile* originTile = new Tile( origin..origin+1 , currentMDDObj->getCellType() );
			extendDomainList.push_back( originTile );
		}
		if (targetDomain.get_high() != qtMDDObj->getLoadDomain().get_high())
		{
			RMInit::logOut << "QtExtend::evaluate( QtDataList* ) - adding aux tile at high." << endl;
->			Tile* highTile = new Tile( high-1..high, currentMDDObj->getCellType() );
			extendDomainList.push_back( highTile );
		}

		// merge all tiles into one & free not-used-any-longer stuff
		Tile* completeTile = new Tile( extendDomainList );
		delete[] extendDomainList;
		resultMDD->insertTile( completeTile );
		delete completeTile;

#else	// 2-code; unused -- PB 2005-jun-24
	// the part below does the trick explicitly, leading to a larger number of result tiles.
		// establish list of domains
		vector<r_Minterval> extendDomainList;

		// inspect 2*d lower/upper neighbours
		// RMInit::logOut << "QtExtend::evaluate( QtDataList* ): - inspect 2*d lower/upper neighbours, dimension is " << targetDomain.dimension() << endl;
		for (r_Dimension d=0; d<targetDomain.dimension(); d++)
		{
			// is there any space left of original MDD; ie, has MDD been extended left?
			if (targetDomain.get_origin()[d] < qtMDDObj->getLoadDomain().get_origin()[d])
			{
				// this domain is identical to original MDD except for dim d where it is left of original
				r_Minterval lowerNeighbour = qtMDDObj->getLoadDomain();
				lowerNeighbour[d] = r_Sinterval( targetDomain.get_origin()[d], qtMDDObj->getLoadDomain().get_origin()[d]-1 );
				// RMInit::logOut << "QtExtend::evaluate( QtDataList* ):   adding lower neighbour domain " << lowerNeighbour << endl;
				extendDomainList.push_back( lowerNeighbour );
			}
			// is there any space right of original MDD; ie, has MDD been extended right?
			if (targetDomain.get_high()[d] > qtMDDObj->getLoadDomain().get_high()[d])
			{
				// this domain is identical to original MDD except for dim d where it is right of original
				r_Minterval upperNeighbour = qtMDDObj->getLoadDomain();
				upperNeighbour[d] =r_Sinterval( qtMDDObj->getLoadDomain().get_high()[d]+1, targetDomain.get_high()[d] );
				// RMInit::logOut << "QtExtend::evaluate( QtDataList* ):   adding upper neighbour domain " << upperNeighbour << endl;
				extendDomainList.push_back( upperNeighbour );
			}
		}

		// inspect 2^d corner points

		// RMInit::logOut << "QtExtend::evaluate( QtDataList* ): - inspect 2^d corner neighbours, dimension is " << targetDomain.dimension() << endl;
		r_Minterval cornerBoxDomain = r_Minterval( targetDomain.dimension() );
		QtExtend::extendGetCornerTiles( targetDomain, qtMDDObj->getLoadDomain(), 0, targetDomain.dimension(), cornerBoxDomain, &extendDomainList );

		// merge where possible to minimize tile number
		// ...just an optimization, tbd later

		// create tiles for all domains found
		// RMInit::logOut << "QtExtend::evaluate( QtDataList* ): - creating " << extendDomainList.size() << " tiles..." << endl;
		for( vector<r_Minterval>::iterator domainIter = extendDomainList.begin(); domainIter != extendDomainList.end(); domainIter++ )
		{
			// RMInit::logOut << "QtExtend::evaluate( QtDataList* ): creating tile for domain " << (*domainIter) << endl;
			Tile* newTransTile = new Tile( *domainIter, currentMDDObj->getCellType() );
			resultMDD->insertTile( newTransTile );
		}
#endif 0

		// --- 3: package into MDD object & finalize -----------------------------

		// create a new QtMDD object as carrier object for the transient MDD object
		returnValue = new QtMDD( (MDDObj*)resultMDD );

		// delete the tile vector, the tiles itself are deleted when the destructor
		// of the MDD object is called

		delete tiles;
		tiles=NULL;

		// delete the old operands
		if( operand1 ) operand1->deleteRef();
		if( operand2 ) operand2->deleteRef();

		// temporary: dump result tile
		// RMInit::logOut << "QtExtend::evaluate( QtDataList* ) - result tile = " << newTransTile->printStatus() << endl;
		//	newTransTile->printStatus(99,RMInit::logOut);
	}

	// RMInit::logOut << "QtExtend::evaluate( QtDataList* ) - done." << endl;
	return returnValue;
}

#if 1 // needed for 1-code above -- PB 2005-jun-24
/**
aux function for QtExtend::evaluate(): build up (recursing the dimension) a list of all spatial domains that sit in the corners between outerDomain and innerDomain; at the recursion bottom the resulting domain is added to the cornerList.
**/

void
QtExtend::extendGetCornerTiles( r_Minterval outerDomain, r_Minterval innerDomain, const r_Dimension currentDim, const r_Dimension maxDim, r_Minterval currentInterval, vector<r_Minterval>* cornerList )
{
	// RMInit::logOut << "QtExtend::extendGetCornerTiles( " << outerDomain << ", " << innerDomain << ", " << currentDim << ", " << maxDim << ", " << currentInterval << ", _  ) start" << endl;

	// not yet addressed all dimensions in the current coordinate?
// note: what about 1D? 0D?
	if (currentDim < maxDim)
	{
		// add domain's lower end, continue building up the minterval
		// ...but only if the area is nonempty
		if (outerDomain.get_origin()[currentDim] < innerDomain.get_origin()[currentDim])
		{
			// make local working copy
			r_Minterval extendedInterval( currentInterval );
			// add i-th coordinate to domain, up to (but excluding) innerDomain
			extendedInterval[currentDim] = r_Sinterval( outerDomain.get_origin()[currentDim], innerDomain.get_origin()[currentDim]-1 );
			// inspect next dimension
			// RMInit::logOut << "QtExtend::extendGetCornerTiles(): recursing for lower end box in next dimension " << currentDim+1 << " using extendedInterval " << extendedInterval << endl;
			extendGetCornerTiles( outerDomain, innerDomain, currentDim+1, maxDim, extendedInterval, cornerList );
		}
		// add domain's upper end, continue building up the minterval
		if (innerDomain.get_high()[currentDim] < outerDomain.get_high()[currentDim])
		{
			// make local working copy
			r_Minterval extendedInterval( currentInterval );
			// add i-th coordinate to domain, starting from (but excluding) innerDomain
			extendedInterval[currentDim] = r_Sinterval( innerDomain.get_high()[currentDim]+1, outerDomain.get_high()[currentDim] );
			// inspect next dimension
			// RMInit::logOut << "QtExtend::extendGetCornerTiles(): recursing for upper end box in next dimension " << currentDim+1 << " using extendedInterval " << extendedInterval << endl;
			extendGetCornerTiles( outerDomain, innerDomain, currentDim+1, maxDim, extendedInterval, cornerList );
		}
	}
	else if (currentDim > maxDim)
	{
		// this is an error, see preconditions
		RMInit::logOut << "QtExtend::extendGetCornerTiles(): error: dimension overflow." << endl;
	}
	else	// then we've reached currentDim==maxDim
	{
		// add this minterval to the tile domain list
		cornerList->push_back( currentInterval );
		// RMInit::logOut << "QtExtend::extendGetCornerTiles(): added " << currentInterval << " to tile domain list." << endl;
	}

	// RMInit::logOut << "QtExtend::extendGetCornerTiles() done." << endl;
}
#endif 1

void
QtExtend::printTree( int tab, ostream& s, QtChildType mode )
{
	s << SPACE_STR(tab).c_str() << "QtExtend Object " << getNodeType() << endl;

	QtBinaryOperation::printTree( tab, s, mode );
}



void
QtExtend::printAlgebraicExpression( ostream& s )
{
	s << "extend(";

	if( input1 )
		input1->printAlgebraicExpression( s );
	else
		s << "<nn>";

	s << ",";

	if( input2 )
		input2->printAlgebraicExpression( s );
	else
		s << "<nn>";

	s << ")";
}



void
QtExtend::optimizeLoad( QtTrimList* trimList )
{
	RMDBCLASS( "QtExtend", "optimizeLoad( QtTrimList* )", "qlparser", __FILE__, __LINE__ )

	QtNode::QtTrimList *list1=NULL, *list2=NULL;

	if( input1 && input2 )
	{
#if 0 // not yet sure what to do -- PB 2005-06-18

		QtNode::QtTrimList::iterator iter;

		//
		// The result of input2 has to be a constant expression.
		//

		// shift of trimList is just possible, if no open bounds are available
		bool openBounds = false;
		for( iter=trimList->begin(); iter!=trimList->end() && !openBounds; iter++ )
			openBounds = !((*iter)->interval.is_low_fixed()) || !((*iter)->interval.is_high_fixed());

		if( openBounds )
		{
			// release( trimList->begin(), trimList->end() );
			for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
			{
				delete *iter;
				*iter=NULL;
			}
			delete trimList;
			trimList=NULL;

			RMInit::logOut << "Error: QtExtend::optimizeLoad() - spatial domain shift of open bounds is not supported" << endl;
// XXX need new error code
			parseInfo.setErrorNo(409);
			throw parseInfo;    
		}

		QtData* operand = input2->evaluate(NULL);

		if( !operand )
		{
			// release( trimList->begin(), trimList->end() );
			for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
			{
				delete *iter;
				*iter=NULL;
			}
			delete trimList;
			trimList=NULL;

			RMInit::logOut <<  "Error: QtExtend::optimizeLoad() - second operand of extend function must be a constant expression." << endl;
// XXX correct new error code
			parseInfo.setErrorNo(408);
			throw parseInfo;    
		}

		if( operand->getDataType() != QT_MINTERVAL )
		{
			// release( trimList->begin(), trimList->end() );
			for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
			{
				delete *iter;
				*iter=NULL;
			}
			delete trimList;
			trimList=NULL;

			operand->deleteRef();

			RMInit::logOut << "Error: QtExtend::optimizeLoad() - second operand must be of type Minterval." << endl;
// XXX correct new error code
			parseInfo.setErrorNo(406);
			throw parseInfo;    
		}
		
		// get extend target domain
		const r_Minterval& targetDomain = ((QtPointData*)operand)->getMintervalData();

		// shift trim elements by -transPoint
		// XXX replace with extend() code
		for( iter=trimList->begin(); iter!=trimList->end(); iter++ )
		{
			QtTrimElement* elem = *iter;

			if( elem->dimension <= transPoint.dimension() )
				elem->interval.set_interval( elem->interval.low()  - transPoint[elem->dimension], elem->interval.high() - transPoint[elem->dimension] );
		}

		// point is not needed anymore
		operand->deleteRef();
#endif 0 // not yet sure what to do -- PB 2005-06-18

		input1->optimizeLoad( trimList );
	}
	else 
	{
		// release( trimList->begin(), trimList->end() );
		for( QtNode::QtTrimList::iterator iter=trimList->begin(); iter!=trimList->end(); iter++ )
		{
			delete *iter;
			*iter=NULL;
		}
		delete trimList;
		trimList=NULL;
	}
}



const QtTypeElement&
QtExtend::checkType( QtTypeTuple* typeTuple )
{
	RMDBCLASS( "QtExtend", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

	dataStreamType.setDataType( QT_TYPE_UNKNOWN );  

	// check operand branches
	if( input1 && input2 )
	{

	// get input types
	const QtTypeElement& inputType1 = input1->checkType( typeTuple ); 
	const QtTypeElement& inputType2 = input2->checkType( typeTuple ); 

	if( inputType1.getDataType() != QT_MDD )
	{
		RMInit::logOut << "Error: QtExtend::checkType() - first operand must be of type MDD." << endl;
		parseInfo.setErrorNo(405);
		throw parseInfo;    
	}

	if( inputType2.getDataType() != QT_MINTERVAL )
	{
		RMInit::logOut << "Error: QtExtend::checkType() - second operand must be of type Minterval." << endl;
		parseInfo.setErrorNo(422);
		throw parseInfo;    
	}

	// pass MDD type
	dataStreamType = inputType1;
	}
	else
		RMInit::logOut << "Error: QtExtend::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}



// --- QtScale --------------------------------------------------

const QtNode::QtNodeType QtScale::nodeType = QT_SCALE;

QtScale::QtScale( QtOperation* mddOp, QtOperation* pointOp )
	:  QtBinaryOperation( mddOp, pointOp )
{
}



bool
QtScale::isCommutative() const
{
	return false; // NOT commutative
}

#include <iomanip>
#include <math.h>

// this define was used during testing, we had a problem
inline double FLOOR(double a) {
	 return floor(a);
}

QtData*
QtScale::evaluate( QtDataList* inputList )
{
	RMDBCLASS( "QtScale", "evaluate( QtDataList* )", "qlparser", __FILE__, __LINE__ )

	QtData* returnValue = NULL;
	QtData* operand1 = NULL;
	QtData* operand2 = NULL;

	if(!getOperands( inputList, operand1, operand2 ) )
		return returnValue;
	
	QtMDD*         qtMDDObj          = (QtMDD*)operand1;
	MDDObj*        currentMDDObj     = qtMDDObj->getMDDObject();
	vector<r_Double>  scaleVector(0);

	scaleVector = vector<r_Double>( qtMDDObj->getLoadDomain().dimension() );
	
	r_Minterval sourceDomain = qtMDDObj->getLoadDomain(); 
	r_Minterval targetDomain;
	r_Point     origin1       = sourceDomain.get_origin(); 
	r_Point     origin2       = qtMDDObj->getMDDObject()->getCurrentDomain().get_origin(); 

	r_Minterval wishedTargetDomain;
	r_Point translation;

	//used for scale with wishedIv
	bool isWishedTargetSet = false;
	r_Double sourceRange=0., targetRange=0., f=0., Tl=0., Th=0.;

	switch( operand2->getDataType() )
	{
		case QT_POINT:
			{
				const r_Point& transPoint = ((QtPointData*)operand2)->getPointData();

				if( transPoint.dimension() != qtMDDObj->getLoadDomain().dimension() )
				{
					// delete the old operands
					if( operand1 ) operand1->deleteRef();
					if( operand2 ) operand2->deleteRef();

					RMInit::logOut << "Error: QtScale::evaluate( QtDataList* ) - dimensionalities of MDD and scale expression are not matching." << endl;
					parseInfo.setErrorNo(418);
					throw parseInfo;          
				}

				for( int i=0; i<scaleVector.size(); i++ )
					scaleVector[i] = transPoint[i];      
			}
			break;

		case QT_CHAR:
		case QT_USHORT:
		case QT_ULONG:
			{
				for( int i=0; i<scaleVector.size(); i++ )
				scaleVector[i] = ((QtAtomicData*)operand2)->getUnsignedValue();;            
			}
			break;

		case QT_OCTET:
		case QT_SHORT:
		case QT_LONG:
			{
				for( int i=0; i<scaleVector.size(); i++ )
					scaleVector[i] = ((QtAtomicData*)operand2)->getSignedValue();;            
			}
			break;

		case QT_DOUBLE:
		case QT_FLOAT:
			{
				RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtScale", "Scaling: " << ((QtAtomicData*)operand2)->getDoubleValue() )
				for( int i=0; i<scaleVector.size(); i++ ) 
					scaleVector[i] = ((QtAtomicData*)operand2)->getDoubleValue();            
			}
			break;
		
		case QT_MINTERVAL:
			{
				wishedTargetDomain = ((QtMintervalData*)operand2)->getMintervalData();
				isWishedTargetSet=true;

				if( wishedTargetDomain.dimension() != sourceDomain.dimension())
			 	{
					// delete the old operands
					if( operand1 ) operand1->deleteRef();
					if( operand2 ) operand2->deleteRef();
		
					RMInit::logOut << "Error: QtScale::evaluate( QtDataList* ) - dimensionalities of MDD and scale expression are not matching." << endl;
					parseInfo.setErrorNo(418);
					throw parseInfo;
			  	}
	 
				for( int i=0; i<scaleVector.size(); i++ ) 
			 	{ 
		 			sourceRange = (r_Double)sourceDomain[i].get_extent();
		 			targetRange = (r_Double)wishedTargetDomain[i].get_extent();
		 
		 			if(sourceRange != 0.) 
			  		{ 
			    			scaleVector[i] = targetRange / sourceRange;
						f = scaleVector[i];

			    			Tl = FLOOR(f*sourceDomain[i].low());
			    			//correction by 1e-6 to avoid the strage bug when Th was a 
			    			//integer value and floor return value-1(e.g. query 47.ql)
						Th = FLOOR(f*(sourceDomain[i].high()+1) + 0.000001)-1; 

// FIXME BUG	if(Tl != Th)
//		   Th--;
		   
						RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtScale", "Scale: before f="<<setprecision(12)<<f)
						RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtScale", "Scale: " << endl
							  <<"precalculated: "<<Tl<<':'<<Th<<"<-->"<<wishedTargetDomain[i].low()<<':'<<wishedTargetDomain[i].high()<<endl
							  <<"pro memoria: "<<(r_Range)(f*(sourceDomain[i].high()+1))<<", "<<(f*(sourceDomain[i].high()+1))
							  <<", "<<floor(f*(sourceDomain[i].high()+1))
							  <<", "<<ceil(f*(sourceDomain[i].high()+1)))
			              
						if( (Th-Tl+1) != targetRange ) 
						{ 
							RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtScale", "Scale: correction necessary: "<<Tl<<':'<<Th<<"<-->"<<wishedTargetDomain[i].low()<<':'<<wishedTargetDomain[i].high())
			  
							f = f + (targetRange - (Th-Tl+1))/sourceRange;
		     
							//    cout<<"f="<<setprecision(12)<<f<<" scale[i]="<<setprecision(12)<<scaleVector[i]<<endl;

			  				Tl = FLOOR(f*sourceDomain[i].low());
							//correction by 1e-6 to avoid the strage bug when Th was a 
							//integer value and floor return value-1(e.g. query 47.ql)
							Th = FLOOR(f*(sourceDomain[i].high()+1) + 0.000001)-1;
// FIXME BUG        if(Tl != Th)
//		       Th--;

							RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtScale", "Scale: ->: "<<Tl<<':'<<Th<<"<-->"<<wishedTargetDomain[i].low()<<':'<<wishedTargetDomain[i].high())
		     	 
							scaleVector[i]=f;
							RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtScale", "Scale: after f="<<setprecision(12)<<f)
						}      
					}
					else
					{ 
						scaleVector[i] =0; //exception? it can't heapen, this error is filtered long before reaching this point
					}    
				}
			 }
			 break;
		 default:
			RMDBGONCE(0, RMDebug::module_qlparser, "r_QtScale", "evaluate() bad type operand2" <<  operand2->getDataType());          	
			break;       
	}

// ----------------------------------------------------------

#ifdef RMANDEBUG
	RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtScale", "Scale vector          : " )
	for( int i=0; i<scaleVector.size(); i++ )
	{
		RMDBGMIDDLE( 1, RMDebug::module_qlparser, "QtScale", scaleVector[i] << "," )
	}
#endif

	// scale domain
	if( !scaleDomain( sourceDomain, scaleVector, targetDomain ) )
	{
		// delete the old operands
		if( operand1 ) operand1->deleteRef();
		if( operand2 ) operand2->deleteRef();

		RMInit::logOut << "Error: QtScale::evaluate( QtDataList* ) - empty result after scaling." << endl;
		parseInfo.setErrorNo(419);
		throw parseInfo;          
	}
	RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtBinaryFunc", "Dummy target domain: " << targetDomain << endl )

	if(isWishedTargetSet) 
	{  
		translation = wishedTargetDomain.get_origin() - targetDomain.get_origin();
		targetDomain.translate(translation);
	}
	
	// create a transient MDD object for the query result
	MDDObj* resultMDD = new MDDObj( currentMDDObj->getMDDBaseType(), targetDomain );

	//**********************
	origin1 = r_Point(scaleVector.size()); // all zero!!
	//**********************

	// get all tiles
	vector<Tile* >* tiles = currentMDDObj->intersect( qtMDDObj->getLoadDomain() );

	//tile domain before & after
	r_Minterval sourceTileDomain, destinationTileDomain;

	//
	// Algorithm A: Scale each Tile
	//

	// iterate over source tiles
	for( vector<Tile*>::iterator tileIter = tiles->begin(); tileIter != tiles->end(); tileIter++ )
	{
		// get relevant area of source tile
		sourceTileDomain = qtMDDObj->getLoadDomain().create_intersection( (*tileIter)->getDomain() );

		// compute scaled  tile domain and check if it exists
		if( (*tileIter)->scaleGetDomain( sourceTileDomain, scaleVector, destinationTileDomain) )
		{ 
			RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtBinaryFunc", "Destination tile domain: " << destinationTileDomain << endl )
			// create a new transient tile
			Tile* newTransTile = new Tile( destinationTileDomain, currentMDDObj->getCellType() );
			newTransTile->execScaleOp( *tileIter, sourceTileDomain, origin1,  scaleVector );
		  
			if(isWishedTargetSet)
				((r_Minterval&)newTransTile->getDomain()).translate(translation);

			resultMDD->insertTile( newTransTile );
		}
	}

	// create a new QtMDD object as carrier object for the transient MDD object
	returnValue = new QtMDD( (MDDObj*)resultMDD );
	
	// delete the tile vector, the tiles itself are deleted when the destructor
	// of the MDD object is called
	delete tiles;
	tiles=NULL;

	// delete the old operands
	if( operand1 ) operand1->deleteRef();
	if( operand2 ) operand2->deleteRef();
	
	return returnValue;
}


void
QtScale::printTree( int tab, ostream& s, QtChildType mode )
{
	s << SPACE_STR(tab).c_str() << "QtScale Object " << getNodeType() << endl;

	QtBinaryOperation::printTree( tab, s, mode );
}



void
QtScale::printAlgebraicExpression( ostream& s )
{
	s << "scale(";

	if( input1 )
		input1->printAlgebraicExpression( s );
	else
		s << "<nn>";

	s << ",";

	if( input2 )
		input2->printAlgebraicExpression( s );
	else
		s << "<nn>";

	s << ")";
}



void
QtScale::optimizeLoad( QtTrimList* trimList )
{
	RMDBCLASS( "QtScale", "optimizeLoad( QtTrimList* )", "qlparser", __FILE__, __LINE__ )

	// by default, pass load domain to input1
	if( input1 )
		input1->optimizeLoad( trimList );
	else
	{
		delete trimList;
		trimList=NULL;
	}
}



const QtTypeElement&
QtScale::checkType( QtTypeTuple* typeTuple )
{
	RMDBCLASS( "QtScale", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )

	dataStreamType.setDataType( QT_TYPE_UNKNOWN );  

	// check operand branches
	if( input1 && input2 )
	{

		// get input types
		const QtTypeElement& inputType1 = input1->checkType( typeTuple ); 
		const QtTypeElement& inputType2 = input2->checkType( typeTuple ); 

		if( inputType1.getDataType() != QT_MDD )
		{
			RMInit::logOut << "Error: QtScale::checkType() - first operand must be of type MDD." << endl;
			parseInfo.setErrorNo(416);
			throw parseInfo;    
		}

		if(  inputType2.getDataType() != QT_POINT  && inputType2.getDataType() != QT_MINTERVAL  && 
		     inputType2.getDataType() != QT_FLOAT  && inputType2.getDataType() != QT_DOUBLE &&
		    !inputType2.isInteger()                                                         )
		{
			RMInit::logOut << "Error: QtScale::checkType() - second operand must be either of type Point, Integer or Float." << endl;
			parseInfo.setErrorNo(417);
			throw parseInfo;    
		}

		// pass MDD type
		dataStreamType = inputType1;
	}
	else
		RMInit::logOut << "Error: QtScale::checkType() - operand branch invalid." << endl;
	
	return dataStreamType;
}


int QtScale::scaleDomain( const r_Minterval& areaOp, 
			  const vector<double>& scaleFactors, 
			  r_Minterval &areaScaled )
{
	RMDBGENTER( 2, RMDebug::module_qlparser, "QtScale", 
		    "scaleDomain( D: " << areaOp << ", F: " << scaleFactors << ", D: " << areaScaled << " )" )
	
	try
	{
		areaScaled = areaOp.create_scale(scaleFactors);
	}
	catch(r_Error)
	{
		//error scaling
		RMInit::logOut << "Error: QtScale::scaleDomain() - exception while determining scale target interval for " << areaOp << " and " << scaleFactors << endl;
		return 0;
	}
	
	RMDBGEXIT( 2, RMDebug::module_qlparser, "QtScale", "scaleDomain(...) D: " << areaOp << " mapped to D: " << areaScaled )
	
	return 1;
}

// origin1 von getLoadDomain
// origin2 von getCurrentDomain

int QtScale::scaleDomain( const r_Minterval& areaOp, const r_Point& origin1, const r_Point& origin2, 
			  const vector<double>& scaleFactors, r_Minterval &areaScaled )
{ 
	RMDBGENTER( 2, RMDebug::module_qlparser, 
		    "QtScale", "scaleDomain( D: " << areaOp << ", O1: " << origin1 << ", O2: " << origin2 
		    << ", F: " << scaleFactors << " D: " << areaScaled << " )" )  
		         
	r_Minterval tempIv=areaOp;           

	//reverse_translated with origin1
	tempIv.reverse_translate(origin1);

	//scale it normaly
	if(!scaleDomain(tempIv, scaleFactors, areaScaled))
		return 0;
	 
	//translate areaScaled to origin2
	areaScaled.translate(origin2);
	
	RMDBGEXIT( 2, RMDebug::module_qlparser, "QtScale", "scaledDomain(...) D: " << areaOp << " translated  to D: " << areaScaled )

	return 1;
}

