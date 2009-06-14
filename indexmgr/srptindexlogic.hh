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

#ifndef _SRPTINDEXLOGIC_HH_
#define _SRPTINDEXLOGIC_HH_

#include "indexmgr/hierindexds.hh"
#include "reladminif/lists.h"
class r_Point;
class StorageLayout;


/**
 *	@file srpindexlogic.hh
 *
 *	@ingroup indexmgr
 */

/*@Doc:
This class contains the logic for access, insertion and removal of objects
into an index data structure.  The logic is based on the R-Plus Tree.  Objects
inserted in this index don`t have to be regular.  The leaf object`s domain
must not overlap.

The leaf objects are sorted into nodes which are split when needed.  Nodes
are split when the number of objects in them is equal to the maximum size
specified by the underlying index data structure.  This check should be done
using the IndexDS::isOverFull() method.

This class uses functionality supplied by SDirIndexLogic.

This class was converted to pure static methods because it is fast to use stack.
*/

class SRPTIndexLogic
	{
	public:
		static bool insertObject2(IndexDS* ixDS, const KeyObject& newObject, const StorageLayout& sl);
		/*@Doc:
			Inserts a new object in the index.
			Must have a name different from the other insertObject because of the stupid compiler.
		*/

		static bool removeObject(IndexDS* ixDS, const KeyObject& tileToRemove, const StorageLayout& sl);
		/*@Doc:
			Removes the object from the indexx.
		*/

		static void intersect2(const IndexDS* ixDS, const r_Minterval& searchInter, KeyObjectVector& objs, const StorageLayout& sl);
		/*@Doc:
			Search the index for a search region.
			Determines all the tiles in the index which intersect a given
			search interval (given by {\tt searchInter}).
			Must have a name different from other intersect because of compiler.
		*/

		static void containPointQuery2(const IndexDS* ixDS, const r_Point& searchPoint, KeyObject& result, const StorageLayout& sl);
		/*@Doc:
			Passes a pointer to the searched item.
			Must have different name from other containPointQuery because of compiler.
		*/

		static void getObjects(const IndexDS* ixDS, KeyObjectVector& objs, const StorageLayout& sl);
		/*@Doc:
			Returns all the tiles belonging to the object.
		*/

		static int insertObject(	const KeyObject& newObject,
						HierIndexDS* ix,
						IndexPVector& leafNodes2Split,
						const StorageLayout& sl);
		/*@Doc:
			Inserts a new object in the index.
			Recursive function which does the real job.
			The return value is 1 if the current node was split, 0 otherwise. This is
			used internally by the algorithm (?really - better not touch it?).
			In {\tt leafNodes2Split}, the set of overflowed leaf nodes is returned.
			It should be passed to {\tt splitNodes()}.
		*/


		static void extendFaces(HierIndexDS*		ix,
					const r_Minterval&	newKeyObjectDom,
					const r_Minterval&	oldCurrDom,
					const bool*		facesToExtendLo,
					const bool*		facesToExtendHi);
		/*@Doc:
			This method extends the domains of all index nodes which
			intersect with the object that will be inserted.
			This is neccessary because subsequently all nodes which
			intersect with the object to be inserted are retrieved by
			DirIndexLogic.
		*/

		static void splitNodes(	HierIndexDS* ixDS,
					IndexPVector& leafNodes2Split,
					const StorageLayout& sl);
		/*@Doc:
			Splits all nodes after insert
			Full nodes are split all at once, to avoid repetition of splits.
			Uses splitLeaf and splitNonLeaf to carry out the task.
		*/

		static void splitLeaf(	HierIndexDS*	n1,
					HierIndexDS*	n2,
					KeyObjectVector& keyvec,
					r_Dimension	axis,
					r_Range		value,
					r_Minterval&	domain,
					const StorageLayout& sl);
		/*@Doc:
			Splits a leaf node
			{\tt n1} has the entries which intersect (leafNodeDomain and  x(axis) <= value),
			{\tt n2 } the ones intersecting (leafNodeDomain and x(axis) > value).
			The keyvec holds the tiles which will be inserted into the 2 new leafs.
			The domain is the complete assigned domain of both leafs.  It will be divided into 2 parts
			based on axis and value.  Then the tiles are assigned to the leaf that covers them - or both.
		*/

		static void splitNonLeaf(	HierIndexDS*		n1,
						HierIndexDS*		n2,
						KeyObjectVector&	keyvec,
						IndexPVector&		leafNodes2Split,
						r_Dimension		axis,
						r_Range			value,
						const r_Minterval&	domain,
						const StorageLayout& sl);
		/*@Doc:
			Splits a nonleaf node
			Does semantically the same as splitLeaf.  Syntactically it is quite different because it has to check
			for over full nodes and treat them.
		*/

		static void redistributeEntries(IndexDS* node,
						KeyObjectVector& listMinKO,
						const StorageLayout& sl);
		/*@Doc:
			stores the Keyobjects in the node.  could do some more
			fancy stuff in the future (like checking for under full and then redistribute those nodes).
		*/

		static void calculatePartition(	r_Dimension& axis,
						r_Range& value,
						const HierIndexDS* node);
		/*@Doc:
			Calculates the optimal partition for this node Partition is returned in {\tt axis}, {\tt value}.
			This is the most problematic stuff because it depends on the order of insertion.
			The index nodes are not optimally filled.  But you know: we are the fastest anyway ; )
		*/

		static void calculateDistribution(	r_Dimension axis,
							r_Range value,
							float& dist1,
							float& dist2,
							const HierIndexDS* node);
		/*@Doc:
			Caluculates the distribution of entries for a given partition
			Used by calculate partition.
			Calculates resulting distribution of children of a node {\ttnode} if
			it is split at axis = value. Distributions are returned in {\ttdist1}
			(percentage of nodes intersecting  x(axis) <= value) and {\ttdist2}
			(percentage of nodes intersecting  x(axis) > value).
		*/

		static void intersect(	const r_Minterval& 	searchInter,
					const r_Minterval& 	parentDomain,
					KeyObjectVector&	intersectedObjs,
					const HierIndexDS*	ix,
					r_Area&			area);
		/*@Doc:
			This method helps you get the data out of the index again : )
			searchInter will tell you for what to look.
			parentDomain is used to determin if you should include a matching object or not.
			you might not want to include a matching object because of duplicate entries in the index.
			this choice is made by intersectNoDuplicates.
			intersectedObjs holds the found entries.
			the area is used to determine if we got everything.
		*/

		static bool intersectNoDuplicates(	const r_Minterval& searchInter,
							const r_Minterval& entryDomain,
							const r_Minterval& parentDomain);
		/*@Doc:
			Decides if the entry at hand should be included from this index or if it is in another
			one and will be included from there.
		*/


		static int binaryRegionSearch(	const HierIndexDS*	ixNode,
						const r_Minterval&	mint,
						r_Area&			area,
						KeyObjectVector&	intersectedObjects,
						int			first,
						int			last,
						const r_Minterval&	parentEntryDom);
		/*@Doc:
			This will use a binary search algorithm to quickly find the nodes we want.
		*/

		static int regionSearch(const HierIndexDS*	ixNode,
					const r_Minterval&	mint,
					r_Area&			area,
					KeyObjectVector&	intersectedObjects,
					const r_Minterval&	parentDomain);
		/*@Doc:
			This is a not binary search algorithm for doing the same as binaryRegionSearch.
		*/

		static void containPointQuery(const r_Point& searchPoint, const HierIndexDS* ix, KeyObject& result, const StorageLayout& sl);
		/*@Doc:
		*/

		static HierIndexDS* convert(const KeyObject& toConvert);
		/*@Doc:
			Helper method for converting a keyobject to a hierindex object.
			the parameter must be deleted by the caller.
			the returned object must be deleted by the caller.
		*/

		static KeyObject convert(HierIndexDS* toConvert);
		/*@Doc:
			Helper method for converting a hierindex to a keyobject.
			the parameter must be deleted by the caller.
			the returned object must be deleted by the caller.
		*/

	};

#endif
