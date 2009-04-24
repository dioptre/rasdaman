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
#   test programs of module relindexif
# 
# 
# COMMENTS: 
#   List environment dependencies, known bugs, specialities etc.
# 
########################################################################

# standard include with general options
include $(RMANBASE)/Makefile.inc

CPPSOURCES = testindex.c

# Not needed if BASEDB flag correctly set
INDEXIF = $(RELINDEXIF)

# all test programs
ALLTESTS  = testindex

# Needed RasDaMan libraries for linking. To be copmpleted.
#NEEDEDLIBS = $(CACHETAMGR) $(QLPARSER) $(INDEXMGR) $(RELADMINIF) $(RELMDDIF) $(RELBLOBIF) $(RELINDEXIF) $(CACHETAMGR) $(RELCATALOGIF) $(RASLIB) $(RELADMINIF) $(RELMDDIF)
NEEDEDLIBS = $(RASLIB) $(CLIENTCOMM)

MISCCLEAN := client.bm client.dbg client.log ir.out core
OBJS := testindex.o miteratest.o
# some additional flags for compiling and linking
 

############################## Targets #################################

# make all tests
.PHONY: test
test: $(ALLTESTS)

# delete object and test programs
.PHONY: clean
clean:
	-rm $(ALLTESTS)
	-rm *.c
	-rm *.o

# deletes all non modified, but checked out files
.PHONY: rcsclean
rcsclean:
	-rcsclean

########################### Dependencies ###############################


# make all tests
.PHONY: test
test: $(ALLTESTS)

# See reladminif/test/Makefile for an example of how to add 
# here test files.

testindex : testindex.o
testindex.o : testindex.c
testindex.c : testindex.pc

miteratest : miteratest.o
	$(PURIFY) $(CXX) $(LDFLAGS) -o miteratest miteratest.o $(RASLIB) ../../mymalloc/mymalloc_cln.o
miteratest.o : miteratest.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< 

computeindex : computedindex.o
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ wurzelprungt
computeindex.o : computedindex.cc
	$(CXX) -c $(CPPFLAGS) -mt $(CXXFLAGS) $< asdlfkajsd


