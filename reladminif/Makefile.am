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
#   reladminif
# 
# 
# COMMENTS: 
# - stagerif not used
# 
##################################################################

AM_CXXFLAGS=@BASEDBCXXFLAGS@
AM_LDFLAGS=@BASEDBLDFLAGS@

.SUFFIXES= .@EMBEDDEDSQLEXT@ .@EMBEDDEDSQLOUT@
.@EMBEDDEDSQLEXT@.@EMBEDDEDSQLOUT@:
	$(EMBEDDEDSQLPRECOMPILER) $@ $<


lib_LIBRARIES=libreladminif.a
libreladminif_a_SOURCES=adminifcommon.cc adminif.hh databaseifcommon.cc databaseif.hh \
                        transactionifcommon.cc transactionif.hh sqlerror.hh \
                        oidifcommon.cc oidif.hh dbobject.cc dbobject.hh \
                        dbnamedobject.cc dbnamedobject.hh eoid.cc eoid.hh \
                        dbref.cc dbref.hh objectbrokercommon.cc objectbroker.hh \
                        dbobjectiterator.cc dbobjectiterator.hh \
                        externs.h lists.h binaryrepresentation.hh destroyable.hh \
                        sqlglobals.h dbobjectiditerator.hh
EXTRA_libreladminif_a_SOURCES=adminif.pgc databaseif.pgc transactionif.pgc \
                        sqlerror.pgc oidif.pgc objectbroker.pgc \
                        dbobjectiditerator.cc

libreladminif_a_LIBADD= adminif.$(OBJEXT) databaseif.$(OBJEXT) transactionif.$(OBJEXT) \
                        sqlerror.$(OBJEXT) oidif.$(OBJEXT) objectbroker.$(OBJEXT) 
libreladminif_a_DEPENDENCIES= adminif.$(OBJEXT) databaseif.$(OBJEXT) transactionif.$(OBJEXT) \
                              sqlerror.$(OBJEXT) oidif.$(OBJEXT) objectbroker.$(OBJEXT) 


BUILT_SOURCES= adminif.@EMBEDDEDSQLOUT@ databaseif.@EMBEDDEDSQLOUT@ transactionif.@EMBEDDEDSQLOUT@ \
               sqlerror.@EMBEDDEDSQLOUT@ oidif.@EMBEDDEDSQLOUT@ objectbroker.@EMBEDDEDSQLOUT@


CLEANFILES= adminif.@EMBEDDEDSQLOUT@ databaseif.@EMBEDDEDSQLOUT@ transactionif.@EMBEDDEDSQLOUT@ \
            sqlerror.@EMBEDDEDSQLOUT@ oidif.@EMBEDDEDSQLOUT@ objectbroker.@EMBEDDEDSQLOUT@ \
            client.bm client.dbg client.log ir.out


