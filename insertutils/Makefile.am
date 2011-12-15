# -*-Makefile-*-
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
# MAKEFILE FOR:  
#   utility programs
# 
# COMMENTS: 
# - Various insertion-tools for special client data
# 
##################################################################

AM_CXXFLAGS = $(COMMCXXFLAGS)
AM_LDFLAGS = $(COMMLDFLAGS)    
bin_PROGRAMS = insertppm
bin_SCRIPTS = insertdemo.sh
insertppm_SOURCES=insertppm.cc ../mymalloc/mymalloc_cln.cc
insertppm_LDADD=../rasodmg/librasodmg.a ../clientcomm/libclientcomm.a \
                ../conversion/libconversion.a \
                ../raslib/libraslib.a \
                ../network/libnetwork.a
CLEANFILES = insertdemo.sh
EXTRA_DIST = insertdemo.sh.in

insertdemo.sh: insertdemo.sh.in Makefile
	rm -f insertdemo.sh
	sed -e 's|@bindir[@]|$(bindir)/|g' \
        -e 's|@pkgdatadir[@]|$(pkgdatadir)/|g'\
        <insertdemo.sh.in >insertdemo.sh
	chmod +x insertdemo.sh
	chmod a-w insertdemo.sh
