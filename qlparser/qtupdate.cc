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

static const char rcsid[] = "@(#)qlparser, QtUpdate: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtupdate.cc,v 1.28 2003/12/27 20:51:28 rasdev Exp $";

#include "raslib/dlist.hh"

#include "qlparser/qtupdate.hh"
#include "qlparser/qtdata.hh"
#include "qlparser/qtmdd.hh"
#include "qlparser/qtmintervaldata.hh"

#include "tilemgr/tile.hh"
#include "tilemgr/tiler.hh"

#include "mddmgr/mddobj.hh"

#include <iostream>
#include <memory>


const QtNode::QtNodeType QtUpdate::nodeType = QtNode::QT_UPDATE;



QtUpdate::QtUpdate( QtOperation* initUpdateTarget, QtOperation* initUpdateDomain, QtOperation* initUpdateSource )
  : QtExecute(), input(NULL),
    updateTarget( initUpdateTarget ),
    updateDomain( initUpdateDomain ),
    updateSource( initUpdateSource )
{
  if( updateTarget ) updateTarget->setParent( this );
  if( updateDomain ) updateDomain->setParent( this );
  if( updateSource ) updateSource->setParent( this );
}



QtUpdate::~QtUpdate()
{
  if( updateTarget )
  {
    delete updateTarget;
    updateTarget=NULL;
  }

  if( updateDomain )
  {
    delete updateDomain;
    updateDomain=NULL;
  }

  if( updateSource )
  {
    delete updateSource;
    updateSource=NULL;
  }

  if( input )
  {
    delete input;
    input=NULL;
  }
}



int
QtUpdate::evaluate()
{
  RMDBCLASS( "QtUpdate", "evaluate()", "qlparser", __FILE__, __LINE__ )

  // Test, if all necessary operands are available.
  if( updateTarget && updateSource && input )
  {
    QtNode::QtDataList* nextTupel;

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
        // get obligatory operands
        QtData* target = updateTarget->evaluate( nextTupel );
        QtData* source = updateSource->evaluate( nextTupel );
        
        // Test, if the operands are valid.
        if( target && source )
        {
          // check update target
          if( target->getDataType() != QT_MDD )
	  {
            RMInit::logOut << "Error: QtUpdate::evaluate() - update target must be an iterator variable." << endl;

            // delete tupel vector received by next()
            for( vector<QtData*>::iterator dataIter=nextTupel->begin();
                 dataIter!=nextTupel->end(); dataIter++ )
              if( *dataIter ) (*dataIter)->deleteRef();
            delete nextTupel;
            nextTupel=NULL;

            // delete the operands
	    if( target ) target->deleteRef();
            if( source ) source->deleteRef();

            parseInfo.setErrorNo(950);
            throw parseInfo;
          }

          // check update source
          if( source->getDataType() != QT_MDD )
	  {
            RMInit::logOut << "Error: QtUpdate::evaluate() - update source must be an expression resulting in an MDD" << endl;

            // delete tupel vector received by next()
            for( vector<QtData*>::iterator dataIter=nextTupel->begin();
                 dataIter!=nextTupel->end(); dataIter++ )
              if( *dataIter ) (*dataIter)->deleteRef();
            delete nextTupel;
            nextTupel=NULL;

            // delete the operands
	    if( target ) target->deleteRef();
            if( source ) source->deleteRef();

            parseInfo.setErrorNo(951);
            throw parseInfo;
          }

	  QtMDD* targetMDD = (QtMDD*) target;
          QtMDD* sourceMDD = (QtMDD*) source;

          MDDObj* targetObj = targetMDD->getMDDObject();
          MDDObj* sourceObj = sourceMDD->getMDDObject();

          // test, if target is a persistent object
          if( !targetObj->isPersistent() )
	  {
            RMInit::logOut << "Error: QtUpdate::evaluate() - result of target expression must be an assignable value (l-value)." << endl;

            // delete tupel vector received by next()
            for( vector<QtData*>::iterator dataIter=nextTupel->begin();
                 dataIter!=nextTupel->end(); dataIter++ )
              if( *dataIter ) (*dataIter)->deleteRef();
            delete nextTupel;
            nextTupel=NULL;

            // delete the operands
	    if( target ) target->deleteRef();
            if( source ) source->deleteRef();

            parseInfo.setErrorNo(954);
            throw parseInfo;
          }

          // get optional domain
          QtData*     domainData = NULL;
          r_Minterval domain;

          if( updateDomain )
	  {
            domainData = updateDomain->evaluate( nextTupel );

            if( domainData )
              domain = ((QtMintervalData*)domainData)->getMintervalData();
          }

          RMDBGIF( 1, RMDebug::module_qlparser, "QtUpdate", \
            if( domainData ) \
              RMInit::dbgOut << endl << "  target MDD, domain " << domain << endl; \
            else  \
              RMInit::dbgOut << endl << "  target MDD" << endl; \

            targetMDD->printStatus( RMInit::dbgOut ); \
            RMInit::dbgOut << endl << "  source MDD" << endl; \
            sourceMDD->printStatus( RMInit::dbgOut ); \
            RMInit::dbgOut << endl; \
          )

          // 1st update strategy:
          //
          // 1. All cell values of the source domain which are already defined are
          //    updated with the corresponding new values.
          // 2. If a source tile does not intersect with any tile of the target 
          //    object, it is inserted.

          // In case of update domain existence, test for compatibility.
          if( domainData )
	  {
            // Dimensionality of the udate domain specification has to be equal to 
            // the target MDD dimensionality.
            if( domain.dimension() != targetMDD->getLoadDomain().dimension() )
	    {
              RMInit::logOut << "Error: QtUpdate::evaluate() - Update domain dimensionality must match target MDD dimensionaltiy." << endl;

              // delete tupel vector received by next()
              for( vector<QtData*>::iterator dataIter=nextTupel->begin();
                   dataIter!=nextTupel->end(); dataIter++ )
                if( *dataIter ) (*dataIter)->deleteRef();
              delete nextTupel;
              nextTupel=NULL;

              // delete the operands
	      if( target     ) target->deleteRef();
	      if( domainData ) domainData->deleteRef();
              if( source     ) source->deleteRef();

              parseInfo.setErrorNo(963);
              throw parseInfo;
  	    }

            // The number of interval dimension of the update domain has to be 
            // equal to the number of dimensions of the source domain.
            int updateIntervals = 0;

            const vector<bool>*  trimFlags = ((QtMintervalData*)domainData)->getTrimFlags();
            for( int i=0; i<trimFlags->size(); i++ )
              if( (*trimFlags)[i] )
                updateIntervals++;

            if( updateIntervals != sourceMDD->getLoadDomain().dimension() )
	    {
              RMInit::logOut << "Error: QtUpdate::evaluate() - Number of update intervals must match source dimensionality." << endl;

              // delete tupel vector received by next()
              for( vector<QtData*>::iterator dataIter=nextTupel->begin();
                   dataIter!=nextTupel->end(); dataIter++ )
                if( *dataIter ) (*dataIter)->deleteRef();
              delete nextTupel;
              nextTupel=NULL;

              // delete the operands
	      if( target     ) target->deleteRef();
	      if( domainData ) domainData->deleteRef();
              if( source     ) source->deleteRef();

              parseInfo.setErrorNo(962);
              throw parseInfo;
	    }

            // Before: Warning: Fixed bounds in update domain specifications are ignored.
            
            // After: Actually we should be stricter here and respect the documentation:
            // throw an exception if the source domain isn't within the target domain -- DM 2012-mar-12
            
            // copied from r_Minterval::covers(..), we have to rule out slices here
            const r_Minterval& sourceDomain = sourceMDD->getLoadDomain();
            int j = 0;
            for (r_Dimension i = 0; i < domain.dimension() ; i++)
            {
              if( (*trimFlags)[i] ) // consider only trims
              {
                if((domain[i].is_low_fixed()  && (!(sourceDomain[j].is_low_fixed())  || domain[i].low()  > sourceDomain[j].low())) ||
                   (domain[i].is_high_fixed() && (!(sourceDomain[j].is_high_fixed()) || domain[i].high() < sourceDomain[j].high())))
                {
                  RMInit::logOut << "Error: QtUpdate::evaluate() - source domain " <<
                      sourceDomain << " isn't within the target domain " << domain << endl;

                  // delete tupel vector received by next()
                  for( vector<QtData*>::iterator dataIter=nextTupel->begin();
                      dataIter!=nextTupel->end(); dataIter++ )
                  if( *dataIter ) (*dataIter)->deleteRef();
                  delete nextTupel;
                  nextTupel=NULL;

                  // delete the operands
                  if( target     ) target->deleteRef();
                  if( domainData ) domainData->deleteRef();
                  if( source     ) source->deleteRef();

                  parseInfo.setErrorNo(967);
                  throw parseInfo;
                }
                ++j;
              }
            }

	  } 

          r_Minterval sourceMDDDomain( sourceMDD->getLoadDomain() );
          RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtUpdate", "source mdd domain " << sourceMDDDomain )

	  // compute source MDD domain taking into account update domain
          if( domainData )
	  {
            const vector<bool>*  trimFlags = ((QtMintervalData*)domainData)->getTrimFlags();
            r_Minterval newSourceMDDDomain( targetMDD->getLoadDomain().dimension() );

            for( int i=0, j=0; i<trimFlags->size(); i++ )
              if( (*trimFlags)[i] )
                newSourceMDDDomain << sourceMDDDomain[j++];  
              else
                newSourceMDDDomain << domain[i];

            sourceMDDDomain = newSourceMDDDomain;
                
            RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtUpdate", "source update domain " << sourceMDDDomain )
	  }             
          // check if you may update that are ( you should not go out of bounds for mdddomaintype )
          if (!targetObj->getMDDBaseType()->compatibleWithDomain(&sourceMDDDomain))
		{
              RMInit::logOut << "Error: QtUpdate::evaluate() - The update domain is outside the allowed domain of the target mdd." << endl;

              // delete tupel vector received by next()
              for( vector<QtData*>::iterator dataIter=nextTupel->begin();
                   dataIter!=nextTupel->end(); dataIter++ )
                if( *dataIter ) (*dataIter)->deleteRef();
              delete nextTupel;
              nextTupel=NULL;

              // delete the operands
	      if( target     ) target->deleteRef();
	      if( domainData ) domainData->deleteRef();
              if( source     ) source->deleteRef();

              parseInfo.setErrorNo(953);
              throw parseInfo;
		
		}
          //
          // get all source tiles
          //
          vector<Tile*>* sourceTiles = sourceObj->getTiles();
RMDBGIF(1, RMDebug::module_qlparser, "QtUpdate", \
          if (sourceTiles) \
            RMDBGMIDDLE(1, RMDebug::module_qlparser, "QtUpdate", "there are " << sourceTiles->size() << " source tiles") \
          else \
            RMDBGMIDDLE(1, RMDebug::module_qlparser, "QtUpdate", "there are no source tiles") )

          //
          // get all target tiles in the relevant area
          //

          vector<Tile*>* targetTiles = NULL;
//this is not a very good idea
//          try{
            targetTiles = targetObj->intersect( sourceMDDDomain );
/*          }
          catch(...)
	  {
	    RMInit::logOut << "BUG of MDDObj::intersect" << endl;
            targetTiles = new vector<Tile*>;
	  }
*/
          if( !targetTiles )
            {
            targetTiles = new vector<Tile*>;
            RMDBGMIDDLE(1, RMDebug::module_qlparser, "QtUpdate", "found no target tiles") 
            }
          else	{
                RMDBGMIDDLE(1, RMDebug::module_qlparser, "QtUpdate", "found " << targetTiles->size() << " target tiles in mdd")
                }

          //
          // iterate over source tiles
          //
	unsigned long targetTileArea = 0;
	unsigned long sourceTileArea = 0;
	unsigned long targetTileDomain = 0;
	unsigned long updatedArea = 0;
	bool computed = false;
	vector<r_Minterval> insertedDomains;
	vector<r_Minterval> sourceDomains;
	vector<r_Minterval> targetDomains;
	vector<r_Minterval>::iterator domIt;
	vector<r_Minterval>::iterator intervalIt;
	vector<Tile*> retval;
	vector<Tile*>::iterator retvalIt;
	vector<Tile*>::iterator sourceIt;
	vector<Tile*>::iterator targetIt;
	sourceIt = sourceTiles->begin();
	//this lives here because we don't want memory leaks because of exceptions
	//of course we seldom use this operation ( as we use copy tile most of the time )
	
	UnaryOp* tempOp = NULL;
	if (sourceIt != sourceTiles->end())
		tempOp = Ops::getUnaryOp(Ops::OP_IDENTITY, targetObj->getCellType(), (*sourceIt)->getType(), 0, 0);
	std::auto_ptr<UnaryOp> myOp(tempOp);
  const vector<bool>* trimFlags = NULL;
	for( ; sourceIt != sourceTiles->end(); sourceIt++ )
		{
		// calculate relevant area of source tile
		r_Minterval sourceTileDomain = (*sourceIt)->getDomain().create_intersection( sourceMDD->getLoadDomain() );

		RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtUpdate", "original source tile domain " << sourceTileDomain )

		// compute update source tile domain taking into account update domain
		r_Minterval updateSourceTileDomain;

		if( domainData )
			{
			updateSourceTileDomain = r_Minterval( targetMDD->getLoadDomain().dimension() );
			trimFlags = ((QtMintervalData*)domainData)->getTrimFlags();

			for( int i=0, j=0; i<trimFlags->size(); i++ )
				if( (*trimFlags)[i] )
					updateSourceTileDomain << sourceTileDomain[j++];	
				else
					updateSourceTileDomain << domain[i];
			}
		else
			updateSourceTileDomain = sourceTileDomain;

		//calculate number of cells in this area
		sourceTileArea = sourceTileArea + sourceTileDomain.cell_count();
		RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtUpdate", "update source tile domain " << updateSourceTileDomain )

		bool intersection = false;

		for( targetIt = targetTiles->begin(); targetIt != targetTiles->end(); targetIt++ )
			{
                        RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtUpdate", "target tile domain " << (*targetIt)->getDomain())
			if (!computed)
				{
				targetTileArea = targetTileArea + sourceMDDDomain.create_intersection((*targetIt)->getDomain()).cell_count();
                                RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtUpdate", "target area sum " << targetTileArea)
				}
			// if tiles are intersecting
			if( updateSourceTileDomain.intersects_with( (*targetIt)->getDomain() ) )
				{
                                RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtUpdate", "source tile domain " << updateSourceTileDomain << " intersects with target domain " << (*targetIt)->getDomain())
				intersection = true;

				// get intersecting updateSourceTileDomain
				r_Minterval intersectUpdateSourceTileDomain = updateSourceTileDomain.create_intersection( (*targetIt)->getDomain() );

				// compute corresponding sourceTileDomain
				r_Minterval intersectSourceTileDomain = intersectUpdateSourceTileDomain;
				if( domainData )
					{
					const vector<bool>* trimFlags = ((QtMintervalData*)domainData)->getTrimFlags();

					for( int i=0, j=0; i<trimFlags->size(); i++ )
						if( !((*trimFlags)[i]) )
							intersectSourceTileDomain.delete_dimension( j );
						else
							j++;
					}

				RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtUpdate", "update domains: target tile " << (*targetIt)->getDomain() << " update target at " << intersectUpdateSourceTileDomain << ", source tile " << (*sourceIt)->getDomain() << " update with data at " << intersectSourceTileDomain ) 
				//(*targetIt)->execUnaryOp( Ops::OP_IDENTITY, intersectUpdateSourceTileDomain, *sourceIt, intersectSourceTileDomain );
				(*targetIt)->copyTile(intersectUpdateSourceTileDomain, *sourceIt, intersectSourceTileDomain);
				updatedArea = updatedArea + intersectUpdateSourceTileDomain.cell_count();
				}
			}
		computed = true;
		// insert the tile
		if (!intersection)
			{
			// Create a new persistent tile, copy the transient data,
			// and insert it into the target mdd object.
			Tile* newPersTile = new Tile(updateSourceTileDomain, targetObj->getCellType(), (*sourceIt)->getDataFormat());
			if (updateSourceTileDomain.dimension() == sourceTileDomain.dimension())
				newPersTile->copyTile( updateSourceTileDomain, *sourceIt, sourceTileDomain );
			else
				newPersTile->execUnaryOp( &(*myOp), updateSourceTileDomain, *sourceIt, sourceTileDomain );
			RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtUpdate", "update domains: target tile " << newPersTile->getDomain() << " update target at " << updateSourceTileDomain << ", source tile " << (*sourceIt)->getDomain() << " update with data at " << sourceTileDomain ) 
			// this will make a crash in updateset3 because of a strange triming doman
			//newPersTile->copyTile(updateSourceTileDomain, *sourceIt, sourceTileDomain);

			targetObj->insertTile( newPersTile );
			updatedArea = updatedArea + updateSourceTileDomain.cell_count();
			insertedDomains.push_back((*sourceIt)->getDomain());
			}
          	}//for is done

	if (sourceTileArea > updatedArea)
		{
		RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtUpdate", "the source area was not updated completely, tiling the rest")
		while (!insertedDomains.empty())
			{
			intervalIt = insertedDomains.begin();
			for (sourceIt = sourceTiles->begin(); sourceIt != sourceTiles->end(); sourceIt++)
				if ((*sourceIt)->getDomain() == *intervalIt)
					{
					sourceTiles->erase(sourceIt);
					break;
					}
			insertedDomains.erase(intervalIt);
			}
    
		r_Minterval targetTileDomain;
		for (targetIt = targetTiles->begin(); targetIt != targetTiles->end(); targetIt++)
			{
			targetTileDomain = (*targetIt)->getDomain();
			targetDomains.push_back(targetTileDomain);
			}
    
		for (sourceIt = sourceTiles->begin(); sourceIt != sourceTiles->end(); sourceIt++) {
			
			/*
			* When we try to update an object of N dimensions with data of less
			* dimensions, we should extend the source data here to N dimensions as well,
			* otherwise an index violation error is thrown.
			* 
			* Example: "update Coll as m set m[5,*:*,*:*] assign marray x in [0:10,0:10] values 1c"
			* Coll is 3D but we update it with a 2D array, by restricting the target update interval
			* to slice 5. So the update array is extended to [5:5,0:10,0:10]
			* 
			* -- DM 2012-may-23
			*/
			
			r_Minterval origSourceDomain = (*sourceIt)->getDomain();
			r_Minterval resSourceDomain;
			if (trimFlags) {
				resSourceDomain = r_Minterval(targetTileDomain.dimension());
				for (int i = 0, j = 0; i < trimFlags->size(); i++)
					if ((*trimFlags)[i])
						resSourceDomain << origSourceDomain[j++];
					else
						resSourceDomain << targetTileDomain[i];
			} else
			  resSourceDomain = origSourceDomain;
			sourceDomains.push_back(resSourceDomain);
//			sourceDomains.push_back((*sourceIt)->getDomain());
		}
    
		r_Tiler t(sourceDomains, targetDomains);
		t.split();
		t.removeDoubleDomains();
		t.removeCoveredDomains();
		t.mergeDomains();
		retval = t.generateTiles(*sourceTiles);
		for (retvalIt = retval.begin(); retvalIt != retval.end(); retvalIt++)
			{
			targetObj->insertTile((Tile*)(*retvalIt));
			
			// Copy data from source tiles to the newly inserted tiles -- DM 2012-may-23
			for (sourceIt = sourceTiles->begin(), domIt = sourceDomains.begin();
					sourceIt != sourceTiles->end(); sourceIt++, domIt++)
				{
				if ((*domIt).intersects_with((*retvalIt)->getDomain()))
					{
					r_Minterval intersectRetvalTileDomain = (*domIt).create_intersection((*retvalIt)->getDomain());
					r_Minterval intersectSourceTileDomain = intersectRetvalTileDomain;
					if( trimFlags )
						{
						for( int i=0, j=0; i<trimFlags->size(); i++ )
							if( !((*trimFlags)[i]) )
								intersectSourceTileDomain.delete_dimension( j );
							else
								j++;
						}
					(*retvalIt)->copyTile(intersectRetvalTileDomain, *sourceIt, intersectSourceTileDomain);
					}
				}
			}
		RMDBGMIDDLE( 2, RMDebug::module_qlparser, "QtUpdate", "insertion of the rest is done")
		}

          // delete tile vectors
          delete sourceTiles;
          sourceTiles=NULL;
          delete targetTiles;
          targetTiles=NULL;

          // delete optional operand
          if( domainData ) domainData->deleteRef();
        }
        else
          RMInit::logOut << "Error: QtUpdate::evaluate() - target or source is not provided." << endl;

        // delete the operands
	if( target )     target->deleteRef();
        if( source )     source->deleteRef();

        // delete tupel vector received by next()
        for( vector<QtData*>::iterator dataIter=nextTupel->begin();
             dataIter!=nextTupel->end(); dataIter++ )
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
  }
  else
    RMInit::logOut << "Error: QtUpdate::evaluate() - at least one operand branch is invalid." << endl;

  return 0;
}



QtNode::QtNodeList*
QtUpdate::getChilds( QtChildType flag )
{
  RMDBCLASS( "QtUpdate", "getChilds( QtChildType flag )", "qlparser", __FILE__, __LINE__ )

  QtNodeList* resultList=NULL;

  // allocate resultList
  resultList = new QtNodeList();

  if( flag == QT_LEAF_NODES || flag == QT_ALL_NODES )
  {
    if( input )
    {
      QtNodeList* subList = input->getChilds( flag );

      // remove all elements in subList and insert them at the beginning in resultList
      resultList->splice( resultList->begin(), *subList );

      // delete temporary subList
      delete subList;
      subList=NULL;
    }

    if( updateTarget )
    {
      QtNodeList* subList = updateTarget->getChilds( flag );

      // remove all elements in subList and insert them at the beginning in resultList
      resultList->splice( resultList->begin(), *subList );

      // delete temporary subList
      delete subList;
      subList=NULL;
    }

    if( updateDomain )
    {
      QtNodeList* subList = updateDomain->getChilds( flag );

      // remove all elements in subList and insert them at the beginning in resultList
      resultList->splice( resultList->begin(), *subList );

      // delete temporary subList
      delete subList;
      subList=NULL;
    }

    if( updateSource )
    {
      QtNodeList* subList = updateSource->getChilds( flag );

      // remove all elements in subList and insert them at the beginning in resultList
      resultList->splice( resultList->begin(), *subList );

      // delete temporary subList
      delete subList;
      subList=NULL;
    }
  };

  // add the nodes of the current level
  if( flag == QT_DIRECT_CHILDS || flag == QT_ALL_NODES )
  {
    resultList->push_back( input );
    resultList->push_back( updateTarget );
    resultList->push_back( updateDomain );
    resultList->push_back( updateSource );
  }

  return resultList;
}



void
QtUpdate::printTree( int tab, ostream& s, QtChildType mode )
{
  s << SPACE_STR(tab).c_str() << "QtUpdate Object" << endl;

  if( mode != QtNode::QT_DIRECT_CHILDS )
  {
    if( updateTarget )
    {
      s << SPACE_STR(tab).c_str() << "target: " << endl;
      updateTarget->printTree( tab + 2, s );

      if( updateDomain )
      {
        s << SPACE_STR(tab+2).c_str() << "domain: " << endl;
        updateDomain->printTree( tab + 4, s );
      }
      else
        s << SPACE_STR(tab+2).c_str() << "no domain" << endl;
    }
    else
      s << SPACE_STR(tab).c_str() << "no target" << endl;

    if( updateSource )
    {
      s << SPACE_STR(tab).c_str() << "source: " << endl;
      updateSource->printTree( tab + 2, s );
    }
    else
      s << SPACE_STR(tab).c_str() << "no source" << endl;

    if( input )
    {
      s << SPACE_STR(tab).c_str() << "input: " << endl;
      input->printTree( tab+2, s, mode );
    }
    else
      s << SPACE_STR(tab).c_str() << "no input" << endl;

    s << endl;
  }
}



void
QtUpdate::printAlgebraicExpression( ostream& s )
{
  s << "update<" << std::flush;

  if( updateTarget )
    updateTarget->printAlgebraicExpression( s );
  else
    s << "<no target>";

  if( updateDomain )
    updateDomain->printAlgebraicExpression( s );

  s << "," << std::flush;

  if( updateSource )
    updateSource->printAlgebraicExpression( s );
  else
    s << "<no source>";

  s << ">" << std::flush;

  if( input )
  {
    s << "( ";
    input->printAlgebraicExpression( s );
    s << " )";
  }
  else
    s << "(<no input>)" << std::flush;
}



void
QtUpdate::setStreamInput( QtONCStream* newInput )
{
  input = newInput;
  input->setParent( this );
};

QtOperation*
QtUpdate::getUpdateTarget()
{
	return updateTarget;
}

QtOperation*
QtUpdate::getUpdateDomain()
{
	return updateDomain;
}

QtOperation*
QtUpdate::getUpdateSource()
{
	return updateSource;
}

QtONCStream*
QtUpdate::getInput()
{
	return input;
}



void
QtUpdate::checkType()
{
  RMDBCLASS( "QtUpdate", "checkType()", "qlparser", __FILE__, __LINE__ )

  // check operand branches
  if( updateTarget && updateSource && input )
  {

  // get input type
  QtTypeTuple inputType  = input->checkType(); 

  // check target
  const QtTypeElement& targetType = updateTarget->checkType( &inputType ); 
  if( targetType.getDataType() != QT_MDD )
  {
    RMInit::logOut << "Error: QtUpdate::checkType() - update target must be an iterator variable." << endl;
    parseInfo.setErrorNo(950);
    throw parseInfo;
  }

  // check domain
  if( updateDomain )
  {
    const QtTypeElement& domainType = updateDomain->checkType( &inputType ); 
    if( domainType.getDataType() != QT_MINTERVAL )
    {
      RMInit::logOut << "Error: QtUpdate::checkType() - update domain must be of type Minterval." << endl;
      parseInfo.setErrorNo(961);
      throw parseInfo;
    }
  }

  // check source
  const QtTypeElement& sourceType = updateSource->checkType( &inputType ); 
  if( sourceType.getDataType() != QT_MDD )
  {
    RMInit::logOut << "Error: QtUpdate::checkType() - update source must be an expression resulting in an MDD." << endl;
    parseInfo.setErrorNo(951);
    throw parseInfo;
  }

  // test for compatible base types
  bool compatible = false;
  const BaseType* type1 = ((MDDBaseType*)(targetType.getType()))->getBaseType();
  const BaseType* type2 = ((MDDBaseType*)(sourceType.getType()))->getBaseType();
  
  // substituted the string comparison as it fails for composite types: 
  // the update base type is usually "struct { char 0, char 1,...}" 
  // while the target type is "struct { char red, char green, ...}"
  // Now we rather use the compatibleWith method which handles such cases -- DM 2012-mar-07
  
  // If the source MDD comes from an inv_* function we consider it's compatible
  // at this point, as we can't determine the type until the data is decoded -- DM 2012-mar-12
  
  QtNodeList* convChildren = updateSource->getChild(QT_CONVERSION, QT_ALL_NODES);
  compatible = (convChildren != NULL && !convChildren->empty()) ||
                type1->compatibleWith(type2); //(strcmp(type1, type2) == 0);
          
  if( !compatible )
  {
    RMInit::logOut << "Error: QtUpdate::checkType() - update base type does not match mdd base type." << endl;
    parseInfo.setErrorNo(952);
    throw parseInfo;
  }
  }
  else 
   RMInit::logOut << "Error: QtUpdate::checkType() - operand branch invalid." << endl;
}





