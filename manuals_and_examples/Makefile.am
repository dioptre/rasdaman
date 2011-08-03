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

EXTRA_DIST = *.cfg manuals

nobase_dist_pkgdata_DATA=examples/images/* \
			 examples/c++/query.cc examples/c++/insertppm.cc examples/c++/basictypes.hh examples/c++/Makefile* \
                         examples/java/* examples/queries/* \
                         examples/rasdl/basictypes.dl
 
install-data-hook:
	-# install documentation
	(cd ..; doxygen manuals_and_examples/doxygen-all.cfg)
	(cd ..; doxygen manuals_and_examples/doxygen-api.cfg)
	(cd ..; doxygen manuals_and_examples/doxygen-basedbms.cfg)
	cp -r manuals/ $(DESTDIR)$(pkgdatadir)/doc
