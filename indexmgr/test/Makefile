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
#   test programs of module indexmgr
# 
# COMMENTS: 
#   List environment dependencies, known bugs, specialities etc.
# 
##################################################################
#
# This is just an example Makefile for a test program.
# The dependency of the test program on the lib of the 
# corresponding module is in the Makefile of the module.
#

######################### Definitions ############################

# standard include with general options
include $(RMANBASE)/Makefile.inc

# all test programs
SRCCXX= test_clusterix.cc test_expix.cc test_ix.cc \
	test_abc.cc test_dirix.cc test_hierix.cc test_ix1.cc
OBJS      = ${SRCCXX:%.cc=%.o}
ALLTESTS  = ${SRCCXX:%.cc=%}
MISCCLEAN = core

# some additional flags for compiling and linking
CXXFLAGS := $(STLCXXFLAGS) $(CXXFLAGS)

# includes in module directory
LDFLAGS += -I$(RMANBASE)/indexif
LDFLAGS := $(STLLDFLAGS) $(LDFLAGS)  -L$(SUPPORT_BASE)/lib -lz


########################### Targets ##############################

# make all tests
.PHONY: test
test: $(ALLTESTS)

# test target for class Index (always make module!)
.PHONY: test_index_target
test_index_target: build_module test_dirix test_clusterix test_expix

######################## Dependencies ############################

# make module
.PHONY: build_module
build_module:
	cd $(RMANBASE)/indexmgr; $(MAKE)

# can not be used as a target (module library is not remade!)

test_abc: test_abc.o 
	$(PURIFY) $(CXX)  -o $@ $^ -lm

test_dirix: test_dirix.o $(INDEXMGR) $(INDEXIF) $(RASLIB) $(CACHETAMGR) $(CATALOGIF) $(BLOBIF) $(ADMINIF) $(MDDIF)
	$(PURIFY) $(CXX)  $(BASEDBLDFLAGS) $(LDFLAGS) -o $@ $^ -lm $(QLPARSER)

test_ix1: test_ix1.o $(INDEXMGR) $(INDEXIF) $(RASLIB) $(CACHETAMGR) $(CATALOGIF) \
         $(BLOBIF) $(ADMINIF) $(MDDIF) $(TOOLS)  $(QLPARSER)
	$(PURIFY) $(CXX)  $(BASEDBLDFLAGS) $(LDFLAGS) -o $@ $^ -lm $(QLPARSER)

test_ix: test_ix.o  $(RASLIB)
	$(PURIFY) $(CXX)  $(BASEDBLDFLAGS) $(LDFLAGS) -o $@ $^ -lm 

test_hierix: test_hierix.o $(INDEXMGR) $(INDEXIF) $(RASLIB) $(CACHETAMGR) $(CATALOGIF) $(BLOBIF) $(ADMINIF) $(MDDIF)
	$(PURIFY) $(CXX)  $(BASEDBLDFLAGS) $(LDFLAGS) -o $@ $^ -lm $(QLPARSER)

test_expix: test_expix.o $(INDEXMGR) $(INDEXIF) $(RASLIB) $(CACHETAMGR) $(CATALOGIF) $(BLOBIF) $(ADMINIF) $(MDDIF) $(TOOLS)  $(QLPARSER)
	$(PURIFY) $(CXX)  $(BASEDBLDFLAGS) $(LDFLAGS) -o $@ $^ -lm $(INDEXMGR)

test_clusterix: test_clusterix.o $(INDEXMGR) $(INDEXIF) $(RASLIB) $(CACHETAMGR) $(CATALOGIF) $(BLOBIF) $(ADMINIF) $(MDDIF)
	$(PURIFY) $(CXX)  $(BASEDBLDFLAGS) $(LDFLAGS) -o $@ $^ -lm $(INDEXIF) $(INDEXMGR)

# general rules
include $(RMANBASE)/Makefile.rel

# automatically created dependencies
include Makefile.dep
