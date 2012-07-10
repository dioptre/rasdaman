#
# This file is part of rasdaman community.
#
# Rasdaman community is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Rasdaman community is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
#
# Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
# rasdaman GmbH.
#
# For more information please see <http://www.rasdaman.org>
# or contact Peter Baumann via <baumann@rasdaman.com>.      
#
###################################################################

bin_SCRIPTS = update_petascopedb.sh petascope_insertdemo.sh
EXTRA_DIST = update_petascopedb.sh.in petascope_insertdemo.sh.in
CLEANFILES = update_petascopedb.sh petascope_insertdemo.sh
nobase_dist_pkgdata_DATA=petascope/*

script_edit = $(SED) \
     -e 's|@sysconfdir[@]|$(sysconfdir)|g' \
     -e 's|@pkgdatadir[@]|$(pkgdatadir)|g' \
     -e 's|@bindir[@]|$(bindir)|g'

update_petascopedb.sh petascope_insertdemo.sh:
	rm -f $@
	$(script_edit) '$@.in' >$@
	chmod +x $@
	chmod a-w $@
