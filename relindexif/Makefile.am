# -*-Makefile-*- (for Emacs)
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
#   indexif
# 
##################################################################

AM_CXXFLAGS=@BASEDBCXXFLAGS@
AM_LDFLAGS=@BASEDBLDFLAGS@

.SUFFIXES= .@EMBEDDEDSQLEXT@ .@EMBEDDEDSQLOUT@
.@EMBEDDEDSQLEXT@.@EMBEDDEDSQLOUT@:
	$(EMBEDDEDSQLPRECOMPILER) $@ $<

noinst_LIBRARIES=librelindexif.a
librelindexif_a_SOURCES=dbrcindexdscommon.cc dbrcindexds.hh \
                        dbtcindexcommon.cc dbtcindex.hh \
                        hierindexcommon.cc hierindex.hh \
                        indexid.hh
EXTRA_librelindexif_a_SOURCES=dbrcindexds.pgc dbtcindex.pgc hierindex.pgc 

librelindexif_a_LIBADD=dbrcindexds.$(OBJEXT) dbtcindex.$(OBJEXT) hierindex.$(OBJEXT)
librelindexif_a_DEPENDENCIES=dbrcindexds.$(OBJEXT) dbtcindex.$(OBJEXT) hierindex.$(OBJEXT)

BUILT_SOURCES=dbrcindexds.@EMBEDDEDSQLOUT@ dbtcindex.@EMBEDDEDSQLOUT@ hierindex.@EMBEDDEDSQLOUT@

CLEANFILES=dbrcindexds.@EMBEDDEDSQLOUT@ dbtcindex.@EMBEDDEDSQLOUT@ hierindex.@EMBEDDEDSQLOUT@

