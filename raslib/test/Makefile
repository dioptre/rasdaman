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
#   test programs of module raslib
# 
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

# uses our own mem management routine:
MYMALLOC_H := ../../mymalloc/mymalloc.hh
MYMALLOC_O := ../../mymalloc/mymalloc_cln.o

# use client specific flags
CXXFLAGS := $(CLIENTCXXFLAGS) $(I_SYM)$(MYMALLOC_H)
LDFLAGS  := $(CLIENTLDFLAGS) $(L_SYM)$(MYMALLOC_O)

# raslib has its own template directory because of the name clashes with O2

ifneq ($(OSTYPE),linux-gnu)
  CXXFLAGS := -ptr$(RMANBASE)/rasodmg/ptrepository $(CXXFLAGS)
  LDFLAGS  := -ptr$(RMANBASE)/rasodmg/ptrepository $(LDFLAGS)
endif

# all test programs
ALLTESTS = test_error

# test_sinterval test_point test_minterval test_rmdebug \
#	test_metaobject test_oid test_timer test_miter test_miterf \
#	test_params

########################### Targets ##############################

# test target for class r_Minterval
.PHONY : minterval
minterval: test_module test_minterval

# test target for class r_Miter
.PHONY : miterd
miterd: test_module test_miterd

# test target for class r_Miter
.PHONY : miter
miter: test_module test_miter

# test target for class r_MiterFloat
.PHONY : miterf
miterf: test_module test_miterf

# test target for class r_Sinterval
.PHONY : sinterval
sinterval: test_module test_sinterval

# test target for class r_Point
.PHONY : point
point: test_module test_point

# test target for class r_Error and its subclasses
.PHONY : error
error: test_module test_error

# test target for class RMDebug
.PHONY : rmdebug
rmdebug: test_module test_rmdebug

# test target for class r_Meta_Object and its subclasses
.PHONY : metaobject
metaobject: test_module test_metaobject

# test target for class r_OId
.PHONY : oid
oid: test_module test_oid

# test target for RMTimer
.PHONY : timer
timer: test_module test_timer

# test target for lincomstreams
.PHONY : params
params: test_module test_params

.PHONY : test_module
test_module:
	cd $(RMANBASE)/raslib; $(MAKE)

test_minterval: test_minterval.o $(RASLIB)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^

test_miterd: test_miterd.o $(RASLIB)
	$(PURIFY) $(CXX) $(LDFLAGS) -lm -o $@ $^

test_miter: test_miter.o $(RASLIB)
	$(PURIFY) $(CXX) $(LDFLAGS) -lm -o $@ $^

test_miterf: test_miterf.o $(RASLIB)
	$(PURIFY) $(CXX) $(LDFLAGS) -lm -o $@ $^

test_sinterval: test_sinterval.o $(RASLIB)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^

test_point: test_point.o $(RASLIB)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^

test_error: test_error.o $(RASLIB)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^

test_rmdebug: test_rmdebug.o $(RASLIB)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^

test_metaobject: test_metaobject.o $(RASLIB)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^

test_oid: test_oid.o $(RASLIB)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^

test_timer: test_timer.o $(RASLIB)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^	

test_params: test_params.o $(RASLIB) $(CLIENTCOMM) 
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^

.PHONY : clean
clean:
	-rm $(ALLTESTS)
	-rm *.o

# deletes all non modified, but checked out files
.PHONY : rcsclean
rcsclean:
	-rcsclean

# perform all tests as part of general systemtest
.PHONY: systemtest
systemtest: $(ALLTESTS)
	cp $(RMANBASE)/bin/errtxts .	# needed by test_error
	for PROG in $^; \
	do \
		$$PROG; \
	done

######################## Dependencies ############################

test_minterval.o: test_minterval.cc $(RMANBASE)/raslib/minterval.hh

test_miter.o: test_miter.cc $(RMANBASE)/raslib/miter.hh

test_miterf.o: test_miterf.cc $(RMANBASE)/raslib/miterf.hh

test_sinterval.o: test_sinterval.cc $(RMANBASE)/raslib/sinterval.hh

test_point.o: test_point.cc $(RMANBASE)/raslib/point.hh

test_error.o: test_error.cc $(RMANBASE)/raslib/error.hh

test_oid.o: test_oid.cc $(RMANBASE)/raslib/oid.hh

test_timer.o: test_timer.cc $(RMANBASE)/raslib/

test_params.o: test_params.cc $(RMANBASE)/raslib/parseparams.hh
