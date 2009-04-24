# -*-Makefile-*- (for Emacs)
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
# 
# MAKEFILE FOR:  
#   mddif
#
# COMMENTS: 
#   uses O2
# 
##################################################################

AM_CXXFLAGS=@BASEDBCXXFLAGS@
AM_LDFLAGS=@BASEDBLDFLAGS@

.SUFFIXES= .@EMBEDDEDSQLEXT@ .@EMBEDDEDSQLOUT@
.@EMBEDDEDSQLEXT@.@EMBEDDEDSQLOUT@:
	$(EMBEDDEDSQLPRECOMPILER) $@ $<

noinst_LIBRARIES=librelmddif.a
librelmddif_a_SOURCES=dbmddsetcommon.cc dbmddobj.hh dbmddset.hh dbiterid.hh mddid.hh
EXTRA_librelmddif_a_SOURCES= dbmddobj.pgc dbmddset.pgc 
librelmddif_a_LIBADD=dbmddobj.$(OBJEXT) dbmddset.$(OBJEXT)
librelmddif_a_DEPENDENCIES=dbmddobj.$(OBJEXT) dbmddset.$(OBJEXT)

BUILT_SOURCES=dbmddobj.@EMBEDDEDSQLOUT@ dbmddset.@EMBEDDEDSQLOUT@

CLEANFILES=dbmddobj.@EMBEDDEDSQLOUT@ dbmddset.@EMBEDDEDSQLOUT@ \
           client.bm client.dbg client.log ir.out core

