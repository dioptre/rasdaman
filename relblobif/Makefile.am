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
#   relblobif
# 
# COMMENTS: 
#   uses Oracle precompiler
# 
##################################################################

AM_CXXFLAGS=@BASEDBCXXFLAGS@
AM_LDFLAGS=@BASEDBLDFLAGS@

.SUFFIXES= .@EMBEDDEDSQLEXT@ .@EMBEDDEDSQLOUT@
.@EMBEDDEDSQLEXT@.@EMBEDDEDSQLOUT@:
	$(EMBEDDEDSQLPRECOMPILER) $@ $<


lib_LIBRARIES=librelblobif.a
librelblobif_a_SOURCES= blobtile.hh \
                        dbtile.cc dbtile.hh inlinetile.cc inlinetile.hh \
                        blobtilecommon.cc tileid.hh
EXTRA_librelblobif_a_SOURCES=blobtile.pgc 
librelblobif_a_LIBADD=blobtile.$(OBJEXT)
librelblobif_a_DEPENDENCIES=blobtile.$(OBJEXT)

BUILT_SOURCES=blobtile.@EMBEDDEDSQLOUT@

CLEANFILES=blobtile.@EMBEDDEDSQLOUT@ client.bm client.dbg client.log ir.out \
           blobtile.$(OBJEXT)
  
