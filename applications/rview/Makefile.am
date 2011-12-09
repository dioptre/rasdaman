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
#   rview
#
#
# COMMENTS:
#
##################################################################

AM_CXXFLAGS=@CLIENTCXXFLAGS@
AM_LDFLAGS=@CLIENTLDFLAGS@

EXTRA_DIST = rview labels.txt .rviewrc

install:
	cp rview labels.txt $(DESTDIR)$(bindir)
	cp .rviewrc $(DESTDIR)$(bindir)/..

#rasql:  rasql.o rasql_error.o rasql_signal.cc # $(LIBS)
#	$(PURIFY) $(CXX) $(LDFLAGS) -o rasql $^ $(LIBS) $(IMGLIBS) -lm

#LIBS = $(l_SYM)rasodmg $(l_SYM)clientcomm $(l_SYM)raslib \
#	$(l_SYM)conversion $(l_SYM)commline $(l_SYM)network

#IMGLIBS = $(l_SYM)tiff $(l_SYM)jpeg $(l_SYM)png $(l_SYM)crypto $(l_SYM)z \
#	$(l_SYM)mfhdf $(l_SYM)df  $(l_SYM)netpbm 

#LDFLAGS += $(L_SYM)$(SUPPORT_BASE)/lib

# COMPDATE must be made more general at the final destination
# -- now done at central Makefile.inc, after integration into rasdaman
# COMPDATE = `date +\"%d.%m.%Y %H:%M:%S\"`
# CXXFLAGS +=  -DCOMPDATE="\"$(COMPDATE)\""

#SRCCXX= rasql.cc rasql_error.cc rasql_signal.cc





