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
#ifndef _DBHIERINDEX_HH_
#define _DBHIERINDEX_HH_

#include "reladminif/dbobject.hh"
#include "indexmgr/hierindexds.hh"
#include "relcatalogif/inlineminterval.hh"

//@ManMemo: Module: {\bf relindexif}
/*@Doc:
This class stores data of hierarchical indexes in the database.

There should be another interface to include the isLeaf/isRoot/...
functionality.

Beware of the cache when droping the IndexDS classes!

See indexmgr/hierindexds.hh and indexmgr/indexds.hh for documentation.
*/
class DBHierIndex	:	public HierIndexDS
	{
	public:
		DBHierIndex(r_Dimension dim, bool isNode, bool makePersistent);
		/*@Doc:
			constructs a new index with type ixType, dimension dim.
			if isNode is true the index behaves as a node, else as
			a leaf instance is imediately persistent
		*/

		virtual double getOccupancy() const;

		HierIndexDS* getParent() const;
	
		void setParent(const HierIndexDS* newPa);
	
		virtual void setIsNode(bool beNode);
	
		virtual bool isLeaf() const;
	
		virtual bool isRoot() const;
		/*@Doc:
			is a check for a valid myParent OId
		*/

		virtual unsigned int getHeight() const;
	
		virtual unsigned int getHeightOfTree() const;
		/*@Doc:
			Recursive function to get height of the tree.
		*/
		
		virtual unsigned int getHeightToRoot() const;
		/*@Doc:
			Recursive function to get the number of levels to the root.
		*/
		
		virtual unsigned int getHeightToLeaf() const;
		/*@Doc:
			Recursive function to get the number of levels to the
			leafs.
		*/
		
		virtual unsigned int getTotalEntryCount() const;
	
		virtual unsigned int getTotalNodeCount() const;

		virtual unsigned int getTotalLeafCount() const;

		virtual r_Minterval getCoveredDomain() const;
		
		virtual r_Minterval getAssignedDomain() const;	
		
		virtual r_Minterval getObjectDomain(unsigned int pos) const;	
		
		virtual r_Dimension getDimension() const;
		
		virtual void setAssignedDomain(const r_Minterval& domain);
		
		virtual unsigned int getSize() const;
		
		virtual r_Bytes getTotalStorageSize() const;
		
		virtual bool isValid() const;

		virtual bool isUnderFull() const;

		virtual bool isOverFull() const;

		virtual bool isSameAs(const IndexDS* pix) const; 
		
		virtual bool removeObject(unsigned int pos);
		
		virtual bool removeObject(const KeyObject& theKey);

		virtual void insertObject(const KeyObject& theKey, unsigned int pos);

		virtual void setObject(const KeyObject& theKey, unsigned int pos);

		virtual void setObjectDomain(const r_Minterval& dom, unsigned int pos);

		virtual const KeyObject& getObject(unsigned int pos) const;

		virtual void getObjects(KeyObjectVector& objs) const;

		virtual unsigned int getOptimalSize() const;

		static unsigned int getOptimalSize(r_Dimension dim);
			/*@Doc:
				Used to calculate the optimal number of entries for
				that dimension
			*/

		virtual void freeDS();
		
		virtual OId::OIdPrimitive getIdentifier() const;
		
		static r_Bytes BytesPerTupel; 
		/*@Doc:
			tuning parameter.  used to calculate the optimal size of
			an index.  this is also the number of bytes written to the
			database.
		*/

		virtual void printStatus(unsigned int level = 0, std::ostream& stream = std::cout) const;

		virtual ~DBHierIndex();

		virtual void destroy();

		virtual IndexDS* getNewInstance() const;

		virtual BinaryRepresentation getBinaryRepresentation() const throw (r_Error);

		virtual void setBinaryRepresentation(const BinaryRepresentation&) throw (r_Error);

	protected:
		friend class ObjectBroker;
		/*@Doc:
			ObjectBroker needs to access OId constructor
		*/

		DBHierIndex(const OId& id);
		/*@Doc:
		*/
	
		virtual void readFromDb() throw (r_Error);
		/*@Doc:
		*/

		virtual void updateInDb() throw (r_Error);
		/*@Doc:
		*/

		virtual void deleteFromDb() throw (r_Error);
		/*@Doc:
		*/

		virtual void insertInDb() throw (r_Error);
		/*@Doc:
		*/

		void extendCoveredDomain(const r_Minterval& newTilesExtents) throw (r_Edim_mismatch, r_Eno_interval);
			/*@Doc:
				Recalculates the current domain of this index to
				include newTilesExtents.
			*/

		OId parent;
		/*@Doc:
			persistent, identifies the parent
		*/

		bool _isNode;
		/*@Doc:
			persistent, tells the object what it is.
		*/


		unsigned int maxSize;
			/*@Doc:
				Non persistent attribute.  a cache so the maxSize does not have to be calculated all the time.
			*/

		KeyObjectVector myKeyObjects;

		InlineMinterval myDomain;
		/*@Doc:
			Defined domain of this index.
		*/

		short currentDbRows;
		/*@Doc:
			is needed to support update of index in database
			keeps the number of rows currently taken up in the db by
			this instance
		*/
	};
#endif
