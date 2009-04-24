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
# or contact Peter Baumann via <baumann@rasdaman.com>. # Top Level makefile. This points to the various modules that have to be build
# and/or deployed
# 
# MAKEFILE FOR:  
#   test programs of module tilemgr
# 
# COMMENTS: 
# 
##################################################################
######################### Definitions ############################

# all test programs
SRCCXX = test_access.cc test_splittile.cc test_tile.cc tilertest.cc
OBJS   = ${SRCCXX:%.cc=%.o}
ALLTESTS  = ${SRCCXX:%.cc=%}


# Sun Solaris: -lposix4 is needed for test_tile because of clock_gettime
ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
	LDFLAGS += -lposix4
endif

ALLFLAGS =  $(BASEDBLDFLAGS) $(LDFLAGS) $(STLLDFLAGS) -L$(SUPPORT_BASE)/lib -lz

MAINLIBS = $(RASLIB) $(TILEMGR) $(MDDIF) $(CATALOGIF) $(CATALOGMGR)  \
	   $(INDEXMGR) $(BLOBIF) $(MDDMGR) $(ADMINIF)  $(QLPARSER) $(INDEXIF)

########################### Targets ##############################

# make all tests
.PHONY: test
test: test_tile test_splittile test_access


######################## Dependencies ############################

test_splittile: test_splittile.o $(RASLIB) $(TILEMGR) $(MDDMGR) $(CATALOGMGR) \
	$(MDDIF) $(CATALOGIF)  $(INDEXIF) $(INDEXMGR) $(BLOBIF) $(ADMINIF)  
	$(PURIFY) $(CXX)  $(BASEDBLDFLAGS) $(STLLDFLAGS) $(LDFLAGS) -o $@ $^ \
	$(STLLDFLAGS)

tilertest: tilertest.o $(RASLIB) $(TILEMGR) $(MDDMGR) $(CATALOGMGR) \
	$(MDDIF) $(CATALOGIF)  $(INDEXIF) $(INDEXMGR) $(BLOBIF) $(ADMINIF)  
	$(PURIFY) $(CXX)  $(BASEDBLDFLAGS) $(STLLDFLAGS) $(LDFLAGS) -o $@ $^ $(BLOBIF) $(ADMINIF) $(CATALOGIF) $(INDEXIF) $(MDDIF) $(CATALOGIF) $(STLLDFLAGS) $(INDEXMGR) $(COMPRESSION) $(RASLIB)


# can not be used as a target (module library is not remade!)
test_tile: test_tile.o $(QLPARSER) $(RASLIB) $(ADMINIF) $(CATALOGMGR) \
           $(TILEMGR) $(MDDMGR) $(MDDIF) $(CATALOGIF) $(INDEXMGR) $(BLOBIF) $(INDEXIF) 
	$(PURIFY) $(CXX) $(LDFLAGS) $(STLLDFLAGS) $(BASEDBLDFLAGS) \
        -L$(SUPPORT_BASE)/lib -o $@ $^ -lmalloc -lospace \
        $(RASLIB) $(QLPARSER) -lz


test_access: test_access.o $(SERVERCOMM) $(QLPARSER) $(CONVERSION) $(RASLIB) \
           $(ADMINIF) $(CACHETAMGR) $(COMPRESSION) $(MDDIF) $(CATALOGIF) \
           $(INDEXMGR) $(BLOBIF) $(INDEXIF) $(HTTPSERVER)
	$(PURIFY) $(CXX)  $(LDFLAGS) $(BASEDBLDFLAGS) -o $@ $^ \
	$(RELCATALOGIF) -lstdc++ -L$(SUPPORT_BASE)/lib $(RASLIB) -ltiff -lz

