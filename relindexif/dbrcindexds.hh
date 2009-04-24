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
 * RC index RDBMS adaptor include file.
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _DBRCINDEXDS_HH_
#define _DBRCINDEXDS_HH_

#include "reladminif/dbobject.hh"
#include "indexmgr/indexds.hh"
#include "relcatalogif/inlineminterval.hh"

//@ManMemo: Module: {\bf relindexif}
/*@Doc:
Can't have DBRCIndex and DBHierIndex under a common father class because the compiler has problems resolving the DBRef<> functions correctly.

Persistent class for storing data on regular computed indexes.

Beware of the cache when droping the IndexDS classes!

See indexmgr/indexds.hh for documentation.

Data to store:
RAS_RCINDEXDYN
	OId	NUMBER(15,0),
	Count	NUMBER(3,0),
	DynData	VARCHAR(3990)
NB: under Oracle 9i, Dyndata is defined as:
	DynData	BLOB NOT NULL
This should be done for the other systems too,
as VARCHAR usually is subject to charset translation
which we don't want on our binary data.

DynData holds: r_Dimension, OId::OIdType, OId::OIdCounter, InlineMinterval, InlineMinterval
As:
	r_Dimension	dimension	4
	OId::OIdType	myBaseOIdType	2
	OId::OIdCounter	myBaseCounter	4
	unsigned int	mySize		4
	InlineMinterval	myDomain	10 * dim

*/
class DBRCIndexDS	:	public IndexDS
	{
	public:
		DBRCIndexDS(const r_Minterval& definedDomain, unsigned int numberTiles, OId::OIdType theEntryType = OId::BLOBOID);
		/*@Doc:
			Create a new index which handles the domain definedDomain, with tiles of domain
			tileConfig.  As soon as you create this index it will check if the tileConfig fits
			the definedDomain (the tileConfig must completely cover the definedDomain) and then
			allocate as many oids as are neccessary to fill the definedDomain.
		*/

		virtual r_Minterval getCoveredDomain() const;
		/// return defined domain
		
		virtual r_Minterval getAssignedDomain() const;	
		/// return defined domain

		virtual r_Minterval getObjectDomain(unsigned int pos) const;
		/// throw r_Error_FeatureNotSupported
		
		virtual r_Dimension getDimension() const;
		
		virtual void setAssignedDomain(const r_Minterval& domain);
		/// throw r_Error_FeatureNotSupported
		
		virtual unsigned int getSize() const;
		/// this will return the maximum number of tiles that can be stored in the definedDomain.
		
		virtual r_Bytes getTotalStorageSize() const;
		
		virtual bool isValid() const;
		/// returns true

		virtual bool isUnderFull() const;
		/// returns false

		virtual bool isOverFull() const;
		/// returns false

		virtual bool isSameAs(const IndexDS* pix) const; 
		
		virtual bool removeObject(unsigned int pos);
		/// throw r_Error_FeatureNotSupported
		
		virtual bool removeObject(const KeyObject& theKey);
		/// throw r_Error_FeatureNotSupported

		virtual void insertObject(const KeyObject& theKey, unsigned int pos);
		/// throw r_Error_FeatureNotSupported

		virtual void setObject(const KeyObject& theKey, unsigned int pos);
		/// throw r_Error_FeatureNotSupported

		virtual void setObjectDomain(const r_Minterval& dom, unsigned int pos);
		/// throw r_Error_FeatureNotSupported

		virtual const KeyObject& getObject(unsigned int pos) const;
		/// throw r_Error_FeatureNotSupported

		virtual void getObjects(KeyObjectVector& objs) const;
		/// throw r_Error_FeatureNotSupported

		virtual unsigned int getOptimalSize() const;
		/// returns the maximum number of entries that can be stored in this index

		virtual void freeDS();
		
		virtual OId::OIdPrimitive getIdentifier() const;
		
		static r_Bytes BytesPerTupel; 
		/*@Doc:
			tuning parameter.  used to calculate the optimal size of
			an index.  this is also the number of bytes written to the
			database.
		*/

		virtual void printStatus(unsigned int level = 0, std::ostream& stream = std::cout) const;

		virtual ~DBRCIndexDS();

		virtual void destroy();

		virtual IndexDS* getNewInstance() const;
		/// throw r_Error_FeatureNotSupported

		virtual OId::OIdType getBaseOIdType() const;
		
		virtual OId::OIdCounter getBaseCounter() const;

	protected:
		friend class ObjectBroker;
		/*@Doc:
			ObjectBroker needs to access OId constructor
		*/

		DBRCIndexDS(const OId& id);
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

		OId::OIdCounter myBaseCounter;
		/*@Doc:
			The first oid that will be used to store entries.
		*/

		OId::OIdType myBaseOIdType;
		/*@Doc:
			The type of objects to store in this index (most of the time this will be OId::BLOBOID).
		*/

		OId::OIdCounter mySize;

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
