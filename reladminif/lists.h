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
#ifndef _LISTS_H_
#define _LISTS_H_

#include <vector>
#include <set>
#include <map>
#include "relmddif/mddid.hh"

class DBObject;
class OId;
class InlineMinterval;
class r_Minterval;
class KeyObject;
class IndexDS;
class HierIndexDS;
class Tile;

#include "raslib/mddtypes.hh"

//used to hold oids for indexes, blobs, and dbmintervals.  no double entries
typedef std::set< OId, std::less< double > > OIdSet;

//used to hold oids for indexes, blobs, and dbmintervals.  no double entries
typedef std::set< const OId, std::less< double > > OIdConstSet;

typedef std::vector<HierIndexDS*> HierIndexDSPVector;

//used to hold DBObject*.  e.g. in objectbroker to temporarily store them before deletion
typedef std::vector<DBObject*> DBObjectPVector;

//used to hold DBObject*.  e.g. in objectbroker to temporarily store them before deletion
typedef std::vector<const DBObject*> DBObjectPConstVector;

//used to hold oids for indexes, blobs, and dbmintervals
typedef std::vector<OId> OIdVector;

//used to hold oids for indexes, blobs, and dbmintervals
typedef std::vector<const OId> OIdConstVector;

//holds type information on specific blobs which are stored in above oidlists
typedef std::vector<r_Data_Format> CompTypeVector;

typedef std::vector<const InlineMinterval*> IntervalPConstVector;

typedef std::vector<InlineMinterval*> IntervalPVector;

typedef std::vector<const InlineMinterval> IntervalConstVector;

typedef std::vector<InlineMinterval> IntervalVector;

typedef std::vector<const KeyObject*> KeyObjectPConstVector;

typedef std::vector<KeyObject*> KeyObjectPVector;

typedef std::vector<const KeyObject> KeyObjectConstVector;

typedef std::vector<KeyObject> KeyObjectVector;

typedef std::vector<r_Minterval> DomainVector;

typedef std::vector<r_Minterval*> DomainPVector;

typedef std::vector<const r_Minterval*> DomainPConstVector;

typedef std::vector<IndexDS*> IndexPVector;

typedef std::vector<Tile*> TilePVector;

typedef std::map< double, DBObject*, std::less<double> > DBObjectPMap;
typedef std::pair< double, DBObject* > DBObjectPPair;
typedef std::pair< const double, DBObject* > ConstDBObjectPPair;

typedef std::map< double, const DBObject*, std::less<double> > DBObjectPConstMap;
typedef std::pair< double, const DBObject* > DBObjectPConstPair;
typedef std::pair< const double, const DBObject* > ConstDBObjectPConstPair;

typedef std::map< double, OId, std::less<double> > OIdMap;
typedef std::pair< const double, OId > OIdPair;
typedef std::pair< const double, const OId > OIdConstPair;

typedef std::map< double, r_Minterval, std::less<double> > DomainMap;
typedef std::pair< const double, r_Minterval > DomainPair;
typedef std::pair< const double, const r_Minterval > DomainConstPair;
#endif
