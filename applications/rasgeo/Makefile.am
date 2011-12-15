# -*-Makefile-*-
# This file is part of rasdaman community.
#
# Rasdaman community is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Rasdaman community is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
#
# Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
# rasdaman GmbH.
#
# For more information please see <http://www.rasdaman.org>
# or contact Peter Baumann via <baumann@rasdaman.com>.
#
# MAKEFILE FOR:
#   rmanman
#
#
# COMMENTS:
#
##################################################################

AM_CXXFLAGS=@CLIENTCXXFLAGS@ @BASEDBCXXFLAGS@
AM_LDFLAGS=@CLIENTLDFLAGS@ @BASEDBLDFLAGS@

bin_PROGRAMS=rasimport raserase
rasimport_SOURCES= rimport.cxx rimport.h RasdamanConnector.cxx RasdamanHelper2.cxx nmlog.h
rasimport_LDADD = ../../rasodmg/librasodmg.a ../../clientcomm/libclientcomm.a \
              ../../raslib/libraslib.a \
              ../../conversion/libconversion.a ../../commline/libcommline.a \
              ../../network/libnetwork.a
raserase_SOURCES= rerase.cxx rerase.h RasdamanConnector.cxx RasdamanHelper2.cxx nmlog.h
raserase_LDADD = ../../rasodmg/librasodmg.a ../../clientcomm/libclientcomm.a \
              ../../raslib/libraslib.a \
              ../../conversion/libconversion.a ../../commline/libcommline.a \
              ../../network/libnetwork.a

SUBDIRS= ../../rasodmg ../../clientcomm \
         ../../raslib ../../conversion ../../commline ../../network
         
setupdb:
	psql -d RASBASE -f createEmptyNM_META.sql
	
install:
	cp rasimport raserase $(DESTDIR)$(bindir)	
	
rasconnect:
	cp .rasconnect $(HOME)	
