#ifndef _STORAGELAYOUT_HH_
#define _STORAGELAYOUT_HH_

#include <vector>

#include "raslib/minterval.hh"
#include "relstorageif/dbstoragelayout.hh"
#include "rasodmg/interesttiling.hh"
#include "rasodmg/stattiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "storagemgr/stgmddconfig.hh"


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
/****************************************************************************
 *
 *
 * INCLUDE: storagelayout.hh
 *
 * MODULE:  storagemgr
 * CLASS:   StorageLayout
 *
 * CHANGE HISTORY (append further entries):
 * when         who        what
 * -----------------------------------------------------------------------
 * 13-Nov-00    hoefner    creation of preliminary version
 * 07-Jan-09    Shams      add tiling attributes
 * COMMENTS:
 *
 ****************************************************************************/

//@ManMemo: Module: {\bf indexmgr}
/*@Doc:
  The {\bf StorageLayout} class is used to set the storage layout for persistent 
  MDD objects. 
*/      
 
class StorageLayout
	{
	public:

		//@Man: Default values
		//@{
		/**
			These values are initialized by rassserver.
		*/
		static const r_Bytes	DBSPageSize; 
		/*@Doc:
		Database system page size.  not used at the moment.
		*/

		static r_Bytes		DefaultMinimalTileSize;
		/*@Doc:
		For inlinetiles.  this is the minimum size for a blobtile to be stored as a single blob.
		if you use inlinetile index and a tile is smaller than this value the inlinetile will be stored inside the indexstructure.
		this value should be smaller than a database system page size.
		*/

		static r_Bytes		DefaultPCTMax;
		/*@Doc:
		for inlinetiles.  the maximum size of inlined inlinetiles.  if they grow larger than this value they are outlined and stored as a single blob.
		*/

		static r_Bytes		DefaultTileSize;
		/*@Doc:
		for serverside retiling based on tile size.  this is very bad for production systems but will allow for easier testing.
		when an incomming tile from the client is larger than this number and tile size tiling is enabled the server will split the tile into smaller tiles so that each resulting tile has a smaller size than the incomming tile.
		*/

		static unsigned int		DefaultIndexSize; 
		/*@Doc:
		this is usually computed by the index structures.  for testing purposes and bugfixing it will allow the user to override the computed number of childs per hierarchical index node.
		*/

		static r_Index_Type		DefaultIndexType; 
		/*@Doc:
		the default index to be used.
		*/

		static r_Tiling_Scheme		DefaultTilingScheme;
		/*@Doc:
		the default tiling to be used.
		*/
		///
		static r_Minterval		DefaultTileConfiguration;
		/*@Doc:
		the default tiling configuration to be used.
		*/
		///
		static r_Data_Format		DefaultDataFormat;
		/*@Doc:
		the default data format for tiles.
		*/
		//@}
	 
		//@Man: Creation 
		//@{ 

		StorageLayout();
		/*@Doc:
		Construct object with the default values.
		*/
		
		StorageLayout(r_Index_Type ixtype);
		/*@Doc:
		this is needed for transdir
		*/
		
		StorageLayout(const DBStorageLayoutId& myStorage);				
		/*@Doc:
		Construct object with specific modes.
		*/

		// StorageLayout(const char* storageLayoutName);
		/*@Doc:
		Construct object from an existing named one.
		*/
		//@}
		
		//@Man: Get operations 
		//@{	
		
		r_Index_Type getIndexType() const;
		/*@Doc:
		return the index type to be used.
		this may be the default value if it was not defined previously.
		*/
	 
		r_Tiling_Scheme getTilingScheme() const;
		/*@Doc:
		return the tiling scheme to be used.
		this may be the default value if it was not defined previously.
		*/
		
		r_Bytes getTileSize() const;
		/*@Doc:
		return the tile size to be used for size based retiling (really bad!).
		this may be the default value if it was not defined previously.
		*/
		
		r_Bytes getMinimalTileSize() const;	
		/*@Doc:
		Get minimum optimal tile size.
		*/

		r_Minterval getTileConfiguration() const;
		/*@Doc:
		return the tile domain which defines origin and extent to be used.
		this may be the default value if it was not defined previously.
		*/
		
		//@}	
		
		//@Man: Set operations 
		//@{	

		void setIndexType(r_Index_Type it);
		/*@Doc:
		override previous values or default values for this option.
		*/
		
		void setTilingScheme(r_Tiling_Scheme ts);
		/*@Doc:
		override previous values or default values for this option.
		*/
	 
		void setTileSize(r_Bytes ts);
		/*@Doc:
		override previous values or default values for this option.
		*/
	 
		void setTileConfiguration(const r_Minterval& tc);
		/*@Doc:
		override previous values or default values for this option.
		*/

		void setDataFormat(r_Data_Format df);
		/*@Doc:
		override previous values or default values for this option.
		*/
	 
		//@}	
		
		//@Man: Operations 
		//@{	

		std::vector< r_Minterval > getLayout(const r_Minterval& tileDomain);
		/*@Doc:
		Partition a multidimensional array according to the storage layout.
		*/

		r_Data_Format getDataFormat(const r_Point& where) const;
		/*@Doc:
		this is supplied to offer later implementations to specify the dataformat depending on the region of space.
		*/
	 
		//@}	
		
		
		//@Man: Destruction
		//@{ 
		~StorageLayout();
		//@}

		DBStorageLayoutId getDBStorageLayout() const;
		/*@Doc:
		return the object which actually stores the option values.
		*/
		
		// Functions added by Andrei Aiordachioaie, to match function definitions. (17-08-2009)
		void setBBoxes(const vector<r_Minterval>& input);
		void setSubTiling();
		void resetSubTiling();
		void setInterestThreshold(double i);
		void setBorderThreshold(unsigned int b);
		void setCellSize(int size);
		void setDirDecomp(vector<r_Dir_Decompose>*);


	protected: 
		std::vector< r_Minterval > calcRegLayout(const r_Minterval& layout) const;
		/*@Doc:
		calculate the domains which intersect the layout parameter.  as point of origin the domain specified in myLayout is used.
		*/
	
		//@Man: Actual Parameters: 
		//@{ 
		
		StgMddConfig* extraFeatures;
		/*@Doc: actual features */
		
		DBStorageLayoutId myLayout;
		//@Man: Persistent Representation of a StorageLayout object.
		//@{
		///All parameters are stored there.
		//@}
                std::vector< r_Minterval >
                calcInterestLayout(const r_Minterval& tileDomain);

		//@Man: Actual Parameters:
		//@{
                std::vector< r_Minterval >
                calcAlignedLayout(const r_Minterval&);

                std::vector< r_Minterval >
                calcDirectionalLayout(const r_Minterval&);

                std::vector< r_Minterval >
                calcStatisticLayout(const r_Minterval&);

		//@}
		
	};
#endif
