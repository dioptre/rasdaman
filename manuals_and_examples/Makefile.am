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
####################################################################

nobase_dist_pkgdata_DATA=examples/images/* examples/c++/* \
                         examples/java/* examples/queries/* \
                         examples/rasdl/basictypes.dl

dest_Makefile=$(DESTDIR)$(pkgdatadir)/examples/c++/Makefile

install-data-hook:
	-# Set the rasdaman installation root to allow Make finding
	-# include/ and lib/ for the C++ examples.
	sed 's|~~~rmanhome~~~|$(prefix)|' <$(dest_Makefile) >$(dest_Makefile).new
	mv $(dest_Makefile).new $(dest_Makefile)
	-# install documentation
	-#rm -rf $(DESTDIR)$(pkgdatadir)/doc
	(cd ..; doxygen manuals_and_examples/doxygen-all.cfg)
	(cd ..; doxygen manuals_and_examples/doxygen-api.cfg)
	(cd ..; doxygen manuals_and_examples/doxygen-basedbms.cfg)
	cp -r manuals/ $(DESTDIR)$(pkgdatadir)/doc
