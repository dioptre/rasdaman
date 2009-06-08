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

/* *
* 	COMENTS
*
* 		No comments
*/

//@ManMemo: Module: {\bf raslib}

//for rb_tree, select1st
#include <function.h>
#include <tree.h>
#include <vector>
#include <utility>
#include <memory>

#if(__GNUC__==2 &&__GNUC_MINOR__==95)
using std::rb_tree;
using std::select1st;
#else
using __gnu_cxx::rb_tree;
using __gnu_cxx::select1st;
#endif

using std::vector;
using std::pair;

// commented by Constantin Jucovschi (gcc 3.4+ no longer supports __default_alloc_template)
//using std::__default_alloc_template;			
using std::fill_n;

#include "qlparser/symtab.hh"

#include "raslib/attribute.hh"
#include "raslib/itertype.hh"
#include "raslib/dlist.hh"

#include "tile.hh"

#include "indexmgr/keyobject.hh"

#include "rasodmg/ref.hh"

#include "reladminif/dbref.hh"
#include "reladminif/dbobjectiditerator.hh"

#include "relblobif/blobtile.hh"
#include "relblobif/dbtile.hh"
#include "relblobif/inlinetile.hh"

#include "relcatalogif/typeiterator.hh"
#include "relcatalogif/settype.hh"
#include "relcatalogif/structtype.hh"
#include "relcatalogif/mddtype.hh"
#include "relcatalogif/inlineminterval.hh"
#include "relcatalogif/dbminterval.hh"

#include "relindexif/dbtcindex.hh"
#include "relindexif/hierindex.hh"
#include "relindexif/dbrcindexds.hh"

#include "relmddif/dbmddobj.hh"
#include "relmddif/dbmddset.hh"

#include "relstorageif/dbudfds.hh"
#include "relstorageif/dbudfpackageds.hh"
#include "relstorageif/dbstoragelayout.hh"

template class SymbolTable<int>; 

//template class r_IterType<r_Attribute>;

template class r_Ref<r_Scalar>;
template class r_Ref<r_OId>;

template class DBRef<DBHierIndex>;
template class DBRef<DBRCIndexDS>;
template class DBRef<DBTCIndex>;
template class DBRef<BLOBTile>;
template class DBRef<DBTile>;
template class DBRef<InlineTile>;
template class DBRef<DBMDDSet>;
template class DBRef<DBMinterval>;
template class DBRef<DBStorageLayout>;
template class DBRef<DBUDFDS>;
template class DBRef<DBUDFPackageDS>;
//template class DBRef<DBMDDObj>;
// template bool operator< (const DBRef<DBMDDObj>&, const DBRef<DBMDDObj>&);

//template TypeIterator<StructType>;
//template TypeIterator<SetType>;
template class TypeIterator<MDDType>;
template class DBRef<DBMDDObj>;
template class DBRef<DBObject>;

template class DBObjectIdIterator<DBMDDObj>;
template class DBObjectIterator<DBMDDObj>;
template class DBObjectIterator<DBMDDSet>;
template class DBObjectIterator<StructType>;
template class DBObjectIterator<SetType>;
template class DBRef<StructType>;
template class DBRef<SetType>;
template class DBRef<MDDType>;

template std::ostream& operator<< (const vector<KeyObject>&, std::ostream&);
template std::ostream& operator<< (std::ostream &, const vector<KeyObject>&);
//template  std::ostream& operator << (std::ostream& os, const std::vector<double>& list);
//template  std::ostream& operator << (std::ostream& os, const std::vector<r_Minterval>& list);

template class rb_tree<OId, pair<OId const, DBMDDObj *>, select1st<pair<OId const, DBMDDObj *> >, less<OId> >;
template class rb_tree<OId, pair<OId const, DBMinterval *>, select1st<pair<OId const, DBMinterval *> >, less<OId> >;
template class rb_tree<OId, pair<OId const, DBRef<DBMDDObj> >, select1st<pair<OId const, DBRef<DBMDDObj> > >, less<OId> >;
template class rb_tree<OId, pair<OId const, DBMDDSet *>, select1st<pair<OId const, DBMDDSet *> >, less<OId> >;
template class rb_tree<OId, pair<OId const, MDDType *>, select1st<pair<OId const, MDDType *> >, less<OId> >;
template class rb_tree<OId, pair<OId const, SetType *>, select1st<pair<OId const, SetType *> >, less<OId> >;
template class rb_tree<OId, pair<OId const, StructType *>, select1st<pair<OId const, StructType *> >, less<OId> >;
template class rb_tree<long, pair<long const, BLOBTile *>, select1st<pair<long const, BLOBTile *> >, less<long> >;
template class rb_tree<long, pair<long const, InlineTile *>, select1st<pair<long const, InlineTile *> >, less<long> >;
template class vector<BaseType const * >;
template class vector<OId >;
template class vector<Tile * >;
template class vector<Type * >;
template class vector<char * >;
template class vector<char >;
template class vector<r_Data_Format >;
template class vector<unsigned int >;

template class Tile ** fill_n<Tile **, unsigned int, Tile *>(Tile **, unsigned int, Tile * const &);

