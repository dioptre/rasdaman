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
#   test programs of module qlparser
# 
# COMMENTS: 
# 
##################################################################
#
# This is just an example Makefile for a test program.
# The dependency of the test program on the lib of the 
# corresponding module is in the Makefile of the module.
#

######################### Definitions ############################

# all test programs
SRCCXX   = test_qlparser.cc test_evaluate.cc
OBJS     = ${SRCCXX:%.cc=%.o}
ALLTESTS = ${SRCCXX:%.cc=%}
MISCCLEAN := core

# add compile and link options for STL
CXXFLAGS += $(STLCXXFLAGS)
LDFLAGS  += $(STLLDFLAGS)

########################### Targets ##############################

# test target for qlparser
.PHONY : qlparser
qlparser: test_module test_qlparser

.PHONY : evaluate
evaluate: test_module test_evaluate

.PHONY : test_module
test_module:
	cd $(RMANBASE)/qlparser; $(MAKE)

test_evaluate: test_evaluate.o $(QLPARSER) \
                               $(RASLIB) \
                               $(CACHETAMGR) \
                               $(MDDIF) \
                               $(CATALOGIF) \
                               $(INDEXIF) \
                               $(INDEXMGR) \
                               $(BLOBIF) \
                               $(ADMINIF) \
                               $(CONVERSION)
	$(PURIFY) $(CXX) $(LDFLAGS) $(BASEDBLDFLAGS) -o $@ $^


test_qlparser: $(QLPARSER) test_qlparser.o $(RMANHOME)/qlparser/*.o \
			       $(QLPARSER)  \
			       $(RMANHOME)/lib/librasodmg.a \
                               $(RASLIB) \
                               $(CACHETAMGR) \
                               $(MDDIF) \
                               $(CATALOGIF) \
                               $(INDEXIF) \
                               $(INDEXMGR) \
                               $(BLOBIF) \
                               $(ADMINIF) \
                               $(CONVERSION) \
			       $(RMANHOME)/rasdl/symbtbl.o
	$(PURIFY) $(CXX) $(LDFLAGS) $(BASEDBLDFLAGS) -o $@ $^

