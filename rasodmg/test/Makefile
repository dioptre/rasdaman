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
#   test programs of module rasodmg
# 
# COMMENTS: 
# - the rasql situated here is obsolete, see applications/rasql
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

DEFCLIENTLIBS = $(RASODMG) $(CLIENTCOMM) $(COMPRESSION) $(RASLIB) $(CONVERSION)

# use client specific flags
CXXFLAGS := $(CLIENTCXXFLAGS)
LDFLAGS  := $(CLIENTLDFLAGS) 

# add communication flags
CXXFLAGS += $(COMMCXXFLAGS)
LDFLAGS += $(COMMLDFLAGS)

# COMPDATE must be made more general at the final destination
COMPDATE = `date +\"%d.%m.%Y %H:%M:%S\"`
CXXFLAGS +=  -DCOMPDATE="\"$(COMPDATE)\""

## a symbol for not writing too much when linking
QL = $(SUPPORT_BASE)/lib/lib
SUPPORTLIBS= $(QL)tiff.a $(QL)jpeg.a $(QL)png.a $(QL)crypto.a $(QL)z.a \
	$(QL)mfhdf.a $(QL)df.a  $(QL)ppm.a $(QL)pgm.a $(QL)pbm.a $(LIBAKINSIDE) \
	$(LIBAKNET)

# all test programs
ALLTESTS =  test_oqlquery test_collection test_set test_iterator \
	   test_marray deletecollection \
	   test_ref test_insert test_lookup test_query test_insert3 \
	   test_breakdown test_benchmark test_storage test_alignedtiling \
           test_dirtiling test_interesttiling test_stattiling test_bmark_dir \
	   gen_pattern test_bmark_int test_bmark_stat test_bmark_dir1 \
	   test_bmark_int1 test_gmarray test_transaction test_fastscale \
	   test_polygon exporttif defdiff defconv system_update system_compare \
	   system_insert system_query

SRCCXX= defconv.cc system_compare.cc test_bmark_dir1.cc test_dirtiling.cc test_lookup.cc \
	test_stattiling.cc defdiff.cc system_insert.cc test_bmark_int.cc test_fastscale.cc \
	test_marray.cc test_storage.cc deletecollection.cc system_query.cc test_bmark_int1.cc \
	test_gmarray.cc test_oqlquery.cc test_transaction.cc gen_pattern.cc system_update.cc \
	test_bmark_pet.cc test_insert.cc test_polygon.cc rasql.cc test_alignedtiling.cc \
	test_bmark_stat.cc test_insert3.cc test_query.cc readln.cc test_benchmark.cc \
	test_breakdown.cc test_interesttiling.cc test_ref.cc system_basic.cc test_bmark_dir.cc \
	test_collection.cc  test_iterator.cc test_set.cc

OBJS  = ${SRCCXX:%.cc=%.o}

ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
LDFLAGS+= -lsocket
endif

MISCCLEAN = $(OBJS) $(ALLTESTS)

########################### Targets ##############################
# all tests
tests: $(ALLTESTS)

# test target for class r_Collection
# test target for class r_OQL_Query
.PHONY : oqlquery
oqlquery: test_module test_oqlquery

# test target for class r_Collection
.PHONY : collection
collection: test_module test_collection

# test target for class r_Set
.PHONY : set
set: test_module test_set

# test target for class r_Iterator
.PHONY : iterator
iterator: test_module test_iterator

# test target for class r_Marray
.PHONY : marray
marray: test_module test_marray

# test target for class r_GMarray
.PHONY : gmarray
gmarray: test_module test_gmarray

# test target for class r_Ref, r_Transaction
.PHONY : ref
ref: test_module test_ref

# test target for ODMG conformant insertion of r_Marrays
.PHONY : insert
insert: test_module test_insert

# test target for ODMG conformant lookup of r_Marrays
.PHONY : lookup
lookup: test_module test_lookup

# test target for RasQL queries
.PHONY : query
query: test_module test_query

# test target for test_insert3
.PHONY : insert3
insert3: test_module test_insert3

# test target for deletecollection
.PHONY : deletecoll
insert3: test_module deletecollection

# test target for simulation of communication breakdown
.PHONY : breakdown
breakdown: test_module test_breakdown

# test target for RasQL benchmark
.PHONY : benchmark
benchmark: test_module test_benchmark

# test target for storage layout
storage: test_module test_storage test_alignedtiling test_dirtiling

# test target for transactions
transaction: test_module test_transaction

.PHONY : test_module
test_module:
	cd $(RMANBASE)/rasodmg; $(MAKE)
	cd $(RMANBASE)/raslib; $(MAKE)

test_oqlquery: test_oqlquery.o  $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_collection: test_collection.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_set: test_set.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_iterator: test_iterator.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_marray: test_marray.o  $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_gmarray: test_gmarray.o  $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_ref: test_ref.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_insert: test_insert.o $(DEFCLIENTLIBS) 
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_lookup: test_lookup.o $(DEFCLIENTLIBS) 
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) 

test_fastscale: test_fastscale.o $(DEFCLIENTLIBS) 
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

ifeq ($(OSTYPE),$(OSTYPE_LINUX))
exporttif_static: exporttif.o $(DEFCLIENTLIBS) 
	$(PURIFY) $(CXX) $(LDFLAGS) -o test_polygon -Xlinker -Bstatic $^ $(SUPPORTLIBS) -lstdc++ 
endif

ifeq ($(OSTYPE),$(OSTYPE_LINUX))
exporttif: exporttif.o $(DEFCLIENTLIBS) 
	$(PURIFY) $(CXX) -nodefaultlibs $(LDFLAGS) -o $@ -Xlinker -Bstatic $^ \
	$(SUPPORTLIBS)  -lstdc++ -Xlinker -Bdynamic -lm -lgcc -lc -lgcc
endif


ifeq ($(OSTYPE),$(OSTYPE_LINUX))
test_polygon_static: test_polygon.o $(DEFCLIENTLIBS) 
	$(PURIFY) $(CXX) $(LDFLAGS) -o test_polygon -Xlinker -Bstatic $^ \
	$(SUPPORTLIBS) -lstdc++ 
endif

ifeq ($(OSTYPE),$(OSTYPE_LINUX))
test_polygon: test_polygon.o $(DEFCLIENTLIBS) 
	$(PURIFY) $(CXX) -nodefaultlibs $(LDFLAGS) -o $@ -Xlinker -Bstatic $^ \
	$(SUPPORTLIBS) -lstdc++ -Xlinker -Bdynamic -lm -lgcc -lc -lgcc
endif

ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
test_polygon: test_polygon.o $(DEFCLIENTLIBS) 
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ $(SUPPORTLIBS)
endif

test_query: test_query.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

ifeq ($(OSTYPE),$(OSTYPE_LINUX))
test_query_static: test_query.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) -nodefaultlibs $(LDFLAGS) -o test_query $^ -Xlinker -Bstatic \
	$(SUPPORTLIBS) -lstdc++ -Xlinker -Bdynamic -lgcc -lc -lgcc 
endif

system_compare: system_basic.o system_compare.o $(DEFCLIENTLIBS)
ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lstdc++ -lm $(SUPPORTLIBS)
else
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -Xlinker -Bstatic $(SUPPORTLIBS)
endif

system_update: system_update.o system_basic.o $(DEFCLIENTLIBS)
ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) -lNCSCNet -lNCSEcw -lNCSUtil -mt
endif
ifeq ($(OSTYPE),$(OSTYPE_LINUX))
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -Xlinker -Bstatic $(SUPPORTLIBS) 
endif
ifeq ($(OSTYPE),$(OSTYPE_OSF1))
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ $(SUPPORTLIBS)
endif

system_insert: system_insert.o system_basic.o $(DEFCLIENTLIBS)
ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lstdc++ -lm $(SUPPORTLIBS)
else
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ $(SUPPORTLIBS)
endif

system_query: system_query.o system_basic.o $(DEFCLIENTLIBS)
ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ $(SUPPORTLIBS) -lstdc++ -lm
else
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -Xlinker -Bstatic $(SUPPORTLIBS)
endif

defdiff: defdiff.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -Xlinker -Bstatic -lstdc++ -lm $(SUPPORTLIBS)

defconv: defconv.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -Xlinker -Bstatic -lstdc++ -lm $(SUPPORTLIBS)

test_insert3: test_insert3.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

ifeq ($(OSTYPE),$(OSTYPE_LINUX))
test_insert3_static: test_insert3.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) -nodefaultlibs $(LDFLAGS) -o test_insert3 $^  -Xlinker -Bstatic \
	$(SUPPORTLIBS) -lstdc++ -Xlinker -Bdynamic -lgcc -lc -lgcc
endif 

deletecollection: deletecollection.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_breakdown: test_breakdown.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_benchmark: test_benchmark.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_storage: test_storage.o $(DEFCLIENTLIBS) $(TOOLS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_alignedtiling: test_alignedtiling.o $(DEFCLIENTLIBS) 
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) $(TOOLS)

test_dirtiling: test_dirtiling.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) $(TOOLS)

test_interesttiling: test_interesttiling.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) $(TOOLS)

test_stattiling: test_stattiling.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) $(TOOLS)

test_bmark_dir: test_bmark_dir.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) $(TOOLS)

test_bmark_dir1: test_bmark_dir1.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) $(TOOLS)

test_bmark_int: test_bmark_int.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) $(TOOLS)

test_bmark_int1: test_bmark_int1.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) $(TOOLS)

test_bmark_pet: test_bmark_pet.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) $(TOOLS)

test_bmark_stat: test_bmark_stat.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) $(TOOLS)

gen_pattern: gen_pattern.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) $(TOOLS)

test_transaction: test_transaction.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS)

test_db2blob: test_db2blob.o $(DEFCLIENTLIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ -mt $^ -lm $(DB2LDFLAGS) $(SUPPORTLIBS)

test_db2blob.cc: test_db2blob.sqC
	db2 connect to sample
	db2 prep $< bindfile OUTPUT $@
	db2 bind test_db2blob.bnd
	db2 connect reset

.PHONY : raslib
raslib:
	cd $(RMANBASE)/raslib; $(MAKE)

.PHONY : rasodmg
rasodmg:
	cd $(RMANBASE)/rasodmg; $(MAKE)

.PHONY : clientcomm
clientcomm:
	cd $(RMANBASE)/clientcomm; $(MAKE)


rasql:  readln.o rasql.o 
	$(CXX) -o rasql $^ -L$(SUPPORT_BASE)/lib -lreadline -lcurses
	ln -s -f $(RMANHOME)/rasodmg/test/rasql $(RMANHOME)/bin/rasql


updatemap: updatemap.o updatemap_util.o updatemap_error.o $(DEFCLIENTLIBS)
ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -lm $(SUPPORTLIBS) -lNCSCNet -lNCSEcw -lNCSUtil -mt
endif
ifeq ($(OSTYPE),$(OSTYPE_LINUX))
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -Xlinker -Bstatic $(SUPPORTLIBS) 
endif
ifeq ($(OSTYPE),$(OSTYPE_OSF1))
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ $(SUPPORTLIBS)
endif

PHONY: clean
clean:
	-rm $(MISCCLEAN)

######################## Dependencies ############################

# automatically created dependencies
include Makefile.dep

