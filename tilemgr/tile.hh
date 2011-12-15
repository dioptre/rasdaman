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
 *   Tile is the abstract base class for persTile, transTile
 *   and constTile
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _TILE_HH_
#define _TILE_HH_

#include <vector>
#include <set>

#include "raslib/minterval.hh"   // for r_Minterval
#include "raslib/point.hh"       // for r_Point
#include "raslib/mddtypes.hh"    // for r_Data_Format
#include "catalogmgr/ops.hh"                // for Ops::OpType
#include "relcatalogif/basetype.hh"           // for BaseType
#include "relblobif/tileid.hh"
#include "relblobif/dbtile.hh"
#include "reladminif/dbref.hh"

#ifdef RMANBENCHMARK
#include "raslib/rmdebug.hh"		// for RMTimer
#endif
class KeyObject;
class PersMDDObjIx;


//@ManMemo: Module: {\bf cachetamgr}.

/*@Doc:

{\bf Interdependencies}

Tile uses a pointer to \Ref{BaseType} to store the base type of the Tile.
It uses a \Ref{r_Minterval} to store the domain of the Tile.
Pointers to Tiles are used by many classes.

Persistent Tiles are created either by servercomm, when data is received from a client, or by indexif if a BLOBTile is retrieved from the 
index. The query tree can also create tiles in case of INSERT or UPDATE queries, and Tile::splitTile creates new tiles.
*/

/**
  * \defgroup Tilemgrs Tilemgr Classes
  */

/**
  * \ingroup Tilemgrs
  */ 

class Tile
{
	public:

		/// assignment operator (needed, as class uses dynamic memory).
		const Tile& operator=(const Tile& cell);

		Tile(const r_Minterval& newDom, const BaseType* newType, DBTileId newBLOBTile);
		/*Doc
		  Constructs a new Tile with basetype {\tt newType} and spatial
		  domain {\tt newDom}. Its contents are stored in \Ref{BLOBTile}
		  {\tt newBLOBTile}. The contents are potentially compressed.
		*/
		Tile(const Tile& tile);
		/// constructs a TransTile joined out of the Tiles in {\tt tilesVec}.
		Tile(std::vector<Tile*>* tilesVec);
		/*@Doc:
		  Constructs a new Tile out of the vector {\tt tilesVec}
		  containing pointers to tiles. The Tile created has the
		  closure of the domains of all tiles in the vector as it's domain
		  and the same base type. The tiles should not overlap and must have
		  the same basetype and dimension. Non filled areas in the created
		  tile are of undefined value. The contents are copied, the memory
		  of the tiles and the vector has to be freed by the caller. The 
		  resulting Tile is by default uncompressed.
		*/
		/*@ManMemo:
		  constructs a Tile with the domain {\tt resDom}
		  and the contents joined out of the Tiles in {\tt tilesVec}.
		*/
		Tile(std::vector<Tile*>* tilesVec, const r_Minterval& resDom);
		/*@Doc:
		  Constructs a new Tile out of the vector {\tt tilesVec}
		  containing pointers to tiles. The contents which fall in the area
		  {\tt resDom} are copied into the new Tile. The Tile
		  created has the domain {\tt resDom} and the same base type as the
		  Tiles in {\tt tilesVec}. The tiles should not overlap and must
		  have the same basetype and dimension. Non filled areas in the
		  created tile are of undefined value. The memory of the tiles and
		  the vector has to be freed by the caller. Every tile in {\tt
		  tilesVec} has to overlap with {\tt resDom}.
		*/
		/// constructs Tile as projection of {\tt projTile}.
		Tile(const Tile* projTile, const r_Minterval& projDom, const std::set<r_Dimension, std::less<r_Dimension> >* projDim);
		/*@Doc:
		  Constructs a new Tile out of the projection of Tile {\tt
		  projTile} with the dimensions given in {\tt projDim} projected
		  away (zero based dimension counting!). Only the area specified in
		  projDom is used for the new Tile. {\tt projDom} must have the
		  same dimension as the domain of {\tt projTile}. Dimensions
		  projected away must have the coordinate to be projected at
		  as domain, e.g. 28:28.
		*/
		/*@ManMemo: constructs a Tile with base type {\tt newType} and 
			    spatial domain {\tt newDom}. */
		Tile(const r_Minterval& newDom, const BaseType* newType, r_Data_Format newFormat = r_Array);
		/*@Doc
		  The contents are undefined! This constructor should usually not
		  be used.
		*/
		/// constructs a Tile with contents {\tt newCells}.
		Tile(const r_Minterval& newDom, const BaseType* newType, char* newCells, r_Bytes newSize = 0, r_Data_Format newFormat = r_Array);
		/*Doc
		  Constructs a new Tile with basetype {\tt newType} and spatial
		  domain {\tt newDom}. The char array {\tt newCells} contains the 
		  potentially compressed contents of the new Tile. The memory for 
		  the cells is managed by Tile and has to be allocated with
		  malloc(). If newSize is 0, it is assumed to be uncompressed contents, 
		  and the size is calculated from domain and base type.
		*/
		Tile(const r_Minterval& newDom, const BaseType* newType, const char* newCells, bool, r_Bytes newSize = 0, r_Data_Format newFormat = r_Array);
		/*Doc
		  Constructs a new Tile with basetype {\tt newType} and spatial
		  domain {\tt newDom}. The char array {\tt newCells} contains the 
		  potentially compressed contents of the new Tile. 'bool' is used only,
		  for making the difference between this constructor and the upper one.
		  This one doesn't delete the passed data!
		  If newSize is 0, it is assumed to be uncompressed contents, 
		  and the size is calculated from domain and base type.
		*/
	//@Man: read methods
	//@{
		/// returns the spatial domain of the tile.
		const r_Minterval& getDomain() const;
		/// returns the BaseType of the tile.
		const BaseType* getType() const;
		/// returns the dimension of the tile.
		r_Dimension getDimension() const;
		/// returns size of the (uncompressed) contents of the tile in chars.
		r_Bytes getSize() const;
		/// returns size of the contents of the tile as stored in chars.
		r_Bytes getCompressedSize() const;
		/// returns the format of the data maintained by the tile
		r_Data_Format getDataFormat() const;
		/// returns true for persistent instances.
		bool isPersistent() const;
		/// returns true if the contents are currently compressed and must be decompressed in order to be usefull
		bool isCompressed() const;
	//@}

	//@Man: functions to reading and writing the content.
	//@{
		/// access to cell for reading (index is 1D) one cell length is basetype length.
		const char* getCell(r_Area index) const;
		/// access to cell for modifying (index is 1D).
		char* getCell(r_Area index);
		/// set cell (index is 1D).
		void setCell(r_Area index, const char* newCell);
		/// access to a cell using an r_Point.
		char* getCell(const r_Point& aPoint);
		/// access to a cell using an r_Point.
		const char* getCell(const r_Point& aPoint) const;
		/// returns pointer to (uncompressed) contents of Tile.
		const char* getContents() const;
		/// returns pointer to (uncompressed) contents of Tile.
		char* getContents();
		/// sets (uncompressed) contents of Tile.
		void setContents(char* newContents);
		/*@Doc:
		  The memory for the cells is managed by the Tile and has to be
		  allocated with malloc(). Its size has to be correct according to
		  domain and base type of the Tile.
		*/
	//@}

		/// printed output for testing.
		void printStatus(unsigned int level = 0, std::ostream &stream = std::cout) const;
		/*@Doc:
		   Prints the contents of the Tile on stream. Prints every cell in
		   the Tile with the {\tt printCell} function of the \Ref{BaseType}.
		   For dimensionality > 1, 2D-slices are printed with an empty line
		   in between. These 2D slices cover the lowest 2 indices (0 and 1).
		*/

		void setPersistent(bool state = true);

		/// splits tile in vector of tiles of smaller size.
		std::vector<Tile*>* splitTile(r_Minterval resDom, int storageDomain = 0);
		/*@Doc:
		   The Tile is split into subtiles with the same extent as {\tt
		   resDom}. The storage domain (pers. or transient) of the subtiles is 
		   defined by {\tt storageDomain}. If it is null (default value) they 
		   have the same type (\Ref{TransTile} resp. \Ref{Tile}) as self. 
		   If {\tt storageDomain} is 1, they are persistent, if {\tt storageDomain}
		   has a value other than 0 or 1, they are made transient.
		   The algoritm starts with the smallest
		   coordinate in each dimension, so that if the Tile does not divide
		   into Tiles of extent {\tt resDom}, the last Tiles in each
		   dimension may be smaller. The Tiles returned as pointers have to
		   be freed by the caller! 
		*/


	//@Man: methods for carrying out operations
	//@{
		/// carries out condense function (const)
		char* execCondenseOp(CondenseOp* myOp, const r_Minterval& areaOp);
		/*@Doc:
		  The condense function {\tt myOp} is applied to all cells of self in
		  the area {\tt areaOp}. The result is stored in myOp which also
		  gives the start value for the condense operation. The return value
		  is a pointer to a member of myOp, so it gets invalid if myOp is
		  deleted! For further information on condense operations see \Ref{Ops}.
		*/


		/// carries out unary function with self as result.
		void execUnaryOp(UnaryOp* myOp, const r_Minterval& areaRes, const Tile* opTile, const r_Minterval& areaOp);
		/*@Doc:
		  The unary function {\tt myOp} is applied to all cells of the tile
		  {\tt opTile} in the area {\tt areaOp}. The result of the
		  operation is stored in self in the area {\tt areaRes}. The areas
		  must have the same extent, but may differ in an offset.
		*/

		/// carries out binary function with self as result.
		void execBinaryOp(	BinaryOp* myOp, const r_Minterval& areaRes, 
				const Tile* op1Tile, const r_Minterval& areaOp1,
				const Tile* op2Tile, const r_Minterval& areaOp2);
		/*@Doc:
		  The binary function {\tt myOp} is applied to all cells of the tiles
		  {\tt op1Tile} and {\tt op2Tile} in the respective areas. The
		  result of the operation is stored in self in the area {\tt
		  areaRes}. The areas must have the same extent for all tiles. but
		  may differ in an offset vector.
		*/

		/// carries out binary function with self as result.
		virtual void execConstOp(	BinaryOp* myOp, const r_Minterval& areaRes, 
					const Tile* opTile, const r_Minterval& areaOp,
					const char* cell, int constPos = 1);
		/*@Doc:
		  The binary function {\tt op} is applied to all cells of the tile
		  {\tt op1Tile} and the constant {\tt cell} in the area {\tt
		  areaOp}. If {\tt constPos} is 1, then {\tt const op cell} is
		  carried out; if it is 2, then {\tt cell op const} is carried out.
		  The result of the operations is stored in self in the area {\tt
		  areaRes}. {\tt areaOp} and {\tt areaRes} must have the same
		  extent, but may differ in an offset.
		*/


		/// fills tile in area {\tt areaRes} using MarrayOp {\tt myOp}.
		virtual void execMarrayOp(MarrayOp* myOp, const r_Minterval& areaRes, const r_Minterval& areaOp);
		/*@Doc: 
		  {\tt myOp} maps a point to a value. It is important that the base
		  type specified it the same as the tile has.
		*/

		/// executes general condense operation {\tt myOp} in area {\tt areaOp} (const)
		static char* execGenCondenseOp(GenCondenseOp* myOp, const r_Minterval& areaOp);
		/*@Doc: 
		  {\tt myOp} maps a point to a value. The return values has the resType
		  defined in {\tt myOp}. The tile is not accessed (static function), 
		  the function is defined here to be located together with the other
		  operation execution functions.
		*/
		/// executes scaling operation.
		virtual void execScaleOp(	const Tile* opTile, const r_Minterval& areaOp,
					const r_Point& origin, 
					const std::vector<double>& scaleFactors);
		/*@Doc: 
		  The tile {\tt opTile} is scaled down in each dimension by the 
		  corresponding element in vector {\tt scaleFactors}. The result
		  is stored in the tile on which the operation is called. Scaling
		  is done by using a nearest neighbour algorithm based on 
		  {\tt origin} as the point where the scaling process started.
		  {\tt opTile} has to have the same dimensionality as the result
		  tile and scaleFactors has to have a corresponding number of 
		  elements.
		*/
	//@}
		/// return spatial domain of result tile for scaling in areaScaled.
		/// return 0 if the result tile will be empty.
		   /// (the same function, but with implicit origin (0,0,...0) and working fine!)
		int scaleGetDomain(	const r_Minterval& areaOp,
				const std::vector<double>& scaleFactors,
				r_Minterval& areaScaled);

		/*@Doc: 
		  Return result domain in areaScaled if scaling using the factors in
		  scaleFactors would be applied in area {\tt areaOp} with the scaling
		  factors specified in {\tt scaleFactors}. If the tile would become
		  completely empty, false is returned. This can then be used to create
		  a temporary tile for the result on which the {\tt execScaleOp}
		  function can be called.
		*/
		
		/// virtual destructor.
		virtual ~Tile();

		/// copy a subcube from one tile to another
		virtual void copyTile(const r_Minterval& areaRes, const Tile *opTile, const r_Minterval& areaOp);
		/*@Doc:
		  The part of opTile covered by areaOp is copied to areaRes of this tile. Identical in functionality to execUnaryOp(OP_IDENTITY, ...) but much faster.
		  Requires matching base types and matching domains.
		*/

		DBTileId getDBTile();
		/*Doc
		  Returns a pointer to the \Ref{BLOBTile} holding the contents of
		  the Tile. This function is used to persistently store the
		  contents of a Tile.
		*/

#ifdef RMANBENCHMARK
                // RMTimer for taking O2 times. Could be protected. Is controlled
                // in servercomm/servercomm2.cc.
                static RMTimer opTimer;
                static RMTimer relTimer;
#endif

	protected:
	//@Man: utility functions used internally.
	//@{
		/// calculate offset in cells
		r_Bytes calcOffset(const r_Point& point) const;
		// fill cells of size size with pattern newCell of size patSize.
	//@}

		/// spatial domain of the tile.
		r_Minterval domain;
		/// pointer to base type for cells of Tile.
		const BaseType* type;
		/// Smart pointer to the persistent BLOBTile.
		DBTileId blobTile;
	};

#endif
