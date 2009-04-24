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
#include <qlparser/symtab.hh>

#include <raslib/itertype.hh>
#include <raslib/attribute.hh>
#include <raslib/dlist.hh>

#include <indexmgr/keyobject.hh>

#include <reladminif/dbref.hh>
#include <reladminif/dbobjectiterator.hh>
#include <reladminif/dbobjectiditerator.hh>
#include <reladminif/dbobject.hh>

#include <relblobif/dbtile.hh>
#include <relblobif/blobtile.hh>
#include <relblobif/inlinetile.hh>

#include <relcatalogif/structtype.hh>
#include <relcatalogif/settype.hh>
#include <relcatalogif/mddtype.hh>
#include <relcatalogif/dbminterval.hh>

#include <relindexif/hierindex.hh>
#include <relindexif/dbrcindexds.hh>
#include <relindexif/dbtcindex.hh>

#include <relmddif/dbmddobj.hh>
#include <relmddif/dbmddset.hh>

#include <relstorageif/dbstoragelayout.hh>

template class DBObjectIterator<SetType>;
template class DBObjectIterator<StructType>;
template class DBObjectIterator<MDDType>;
template class DBObjectIterator<DBMDDObj>;
template class DBObjectIdIterator<DBMDDObj>;
template bool operator< (const DBRef<DBMDDObj>&, const DBRef<DBMDDObj>&);

template class DBRef<DBTile>;
template class DBRef<InlineTile>;
template class DBRef<BLOBTile>;
template class DBRef<DBObject>;
template class DBRef<DBMDDObj>;
template class DBRef<DBMDDSet>;
template class DBRef<DBHierIndex>;
template class DBRef<DBTCIndex>;
template class DBRef<DBRCIndexDS>;
template class DBRef<DBMinterval>;
template class DBRef<StructType>;
template class DBRef<MDDType>;
template class DBRef<SetType>;
template class DBRef<DBStorageLayout>;

template class r_IterType<r_Attribute>;

template class SymbolTable<int>;

template std::ostream& operator<< (const std::vector<KeyObject>&, std::ostream&);
template std::ostream& operator<< (std::ostream &, const std::vector<KeyObject>&);
template std::ostream& operator<< (std::ostream &, const std::vector<double>&);

