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
#   rasdaman server binaries, raslic
# 
# COMMENTS: 
# - For static link you have to do: setenv STATIC_LIBS=TRUE
#
##################################################################

AM_CXXFLAGS=@BASEDBCXXFLAGS@
AM_LDFLAGS=@BASEDBLDFLAGS@

bin_PROGRAMS=rasserver
rasserver_SOURCES=rasserver_main.cc rasserver_config.cc rasserver_entry.cc \
                  createinitmdd.cc rasserver_config.hh rasserver_entry.hh \
                  createinitmdd.hh template_inst.hh\
                  ../debug/debug-clt.hh ../debug/debug.hh ../debug/debug-srv.hh \
                  ../include/bool.h ../include/globals.hh ../include/rasdaman.hh \
                  ../include/stdexcept.h
rasserver_LDADD=../servercomm/libservercomm.a ../qlparser/libqlparser.a ../conversion/libconversion.a \
				../rasodmg/librasodmg.a \
                ../raslib/libraslib.a ../storagemgr/libstoragemgr.a ../reladminif/libreladminif.a \
                ../tilemgr/libtilemgr.a ../mddmgr/libmddmgr.a \
                ../catalogmgr/libcatalogmgr.a ../relmddif/librelmddif.a ../relstorageif/librelstorageif.a \
                ../relcatalogif/librelcatalogif.a ../indexmgr/libindexmgr.a ../relblobif/librelblobif.a \
                ../relindexif/librelindexif.a ../httpserver/libhttpserver.a \
                ../network/libnetwork.a ../commline/libcommline.a \
                ../relcatalogif/librelcatalogif.a ../raslib/libraslib.a 

SUBDIRS = ../relblobif ../relindexif ../relmddif ../relcatalogif ../reladminif ../relstorageif \
          ../indexmgr ../catalogmgr ../tilemgr ../storagemgr ../commline ../network \
          ../raslib ../servercomm ../rasodmg ../qlparser ../conversion \
          ../mddmgr ../httpserver ../mymalloc 

$(RECURSIVE_CLEAN_TARGETS):
	@$(MAKE) $(AM_MAKEFLAGS) `echo $@ | sed s/-recursive/-am/`
