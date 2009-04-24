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

# standard include with general options
include $(RMANBASE)/Makefile.inc

# all test programs
SRCCXX   = test_rasdl.cc
OBJS     = ${SRCCXX:%.cc=%.o}
ALLTESTS = ${SRCCXX:%.cc=%}
MISCCLEAN = core

# add compile and link options for STL
CXXFLAGS += $(STLCXXFLAGS)
LDFLAGS  += $(STLLDFLAGS)

########################### Targets ##############################

# general system test
.PHONY: systemtest
systemtest:
	rasdl_test.sh > rasdl_test.log 2>&1
	diff rasdl_test.log rasdl_test.log.orig || (echo deviation found in rasdl test; exit)

# test target for qlparser
.PHONY : rasdl
rasdl: test_module test_rasdl

.PHONY : test_module
test_module:
	cd $(RMANBASE)/rasdl; $(MAKE)

test_rasdl: test_rasdl.o $(RASDL)
	$(CXX) $(LDFLAGS) -o $@ $^

######################## Dependencies ############################

test_rasdl.o: test_rasdl.cc

# general rules
include $(RMANBASE)/Makefile.rel

# automatically created dependencies
include Makefile.dep
