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

bin_SCRIPTS = start_rasdaman.sh stop_rasdaman.sh create_db.sh rasmgr.conf
EXTRA_DIST = stop_rasdaman.sh.in start_rasdaman.sh.in create_db.sh.in rasmgr.conf.in
CLEANFILES = start_rasdaman.sh stop_rasdaman.sh create_db.sh rasmgr.conf
dist_pkgdata_DATA=errtxts_en errtxts_de errtxts_fr errtxts

script_edit = $(SED) \
     -e 's|@logdir[@]|$(logdir)/|g' \
     -e 's|@bindir[@]|$(bindir)/|g' \
     -e 's|@pkgdatadir[@]|$(pkgdatadir)/|g'

start_rasdaman.sh stop_rasdaman.sh create_db.sh: Makefile
	rm -f $@
	$(script_edit) '$@.in' >$@
	chmod +x $@
	chmod a-w $@

start_rasdaman.sh: start_rasdaman.sh.in
stop_rasdaman.sh: stop_rasdaman.sh.in
create_db.sh: create_db.sh.in
rasmgr.conf: rasmgr.conf.in Makefile
	rm -f $@
	$(SED) -e "s|@hostname[@]|`hostname`|g" '$@.in' >$@

# don't overwrite rasmgr.conf if it's already there -- DM 2012-may-16
install-data-hook:
	if test -f $(DESTDIR)$(sysconfdir)/rasmgr.conf; then \
		echo "$@ will not overwrite existing $(DESTDIR)$(sysconfdir)/rasmgr.conf"; \
	else \
		$(MKDIR_P) $(DESTDIR)$(sysconfdir); \
		echo " $(INSTALL_DATA) rasmgr.conf $(DESTDIR)$(sysconfdir)/rasmgr.conf"; \
		$(INSTALL_DATA) rasmgr.conf $(DESTDIR)$(sysconfdir)/rasmgr.conf; \
	fi;
