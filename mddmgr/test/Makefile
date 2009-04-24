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
#   test programs of module mddmgr
# 
# COMMENTS: 
# 
##################################################################
######################### Definitions ############################

# standard include with general options
include $(RMANBASE)/Makefile.inc

# all test programs
SRCCXX    =  test_extendobjs.cc test_mddobj.cc test_persmddcoll.cc test_transmddcoll.cc \
	     test_extbmarkint.cc test_mddjoin.cc test_mddops.cc test_persmddobj.cc   \
	     test_transmddobj.cc
	     
OBJS      = ${SRCCXX:%.cc=%.o}

ALLTESTS  = ${SRCCXX:%.cc=%}
	    
MISCCLEAN = core	   

# Sun Solaris: -lposix4 is needed for test_tile because of clock_gettime
ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
	LDFLAGS += -lposix4
endif

ALLFLAGS =  $(BASEDBLDFLAGS) $(LDFLAGS) $(STLLDFLAGS) -L$(SUPPORT_BASE)/lib -lz

MAINLIBS = $(RASLIB)   $(CACHETAMGR) $(MDDIF)    $(CATALOGIF)  \
	   $(INDEXMGR) $(BLOBIF)     $(ADMINIF)  $(QLPARSER) $(INDEXIF)

########################### Targets ##############################

# make all tests
.PHONY: test
test: ALLTESTS


######################## Dependencies ############################

test_persmddcoll: test_persmddcoll.o $(MAINLIBS)  
	$(PURIFY) $(CXX) $(ALLFLAGS) -o $@ $^ $(STLLDFLAGS)  -lm $(CACHETAMGR) \
	$(INDEXIF)

test_transmddcoll: test_transmddcoll.o  $(RASLIB) $(CACHETAMGR)\
	$(MDDIF) $(CATALOGIF)  $(INDEXIF) $(INDEXMGR) $(BLOBIF) $(ADMINIF)  
	$(PURIFY) $(CXX)  $(BASEDBLDFLAGS) $(STLLDFLAGS) $(LDFLAGS) -o $@ $^

test_mddobj: test_mddobj.o $(RASLIB) $(CACHETAMGR)\
	$(MDDIF) $(CATALOGIF)  $(INDEXIF) $(INDEXMGR) $(BLOBIF) $(ADMINIF)  
	$(PURIFY) $(CXX)  $(BASEDBLDFLAGS) $(STLLDFLAGS) $(LDFLAGS) -o $@ $^ -lm \
		$(CACHETAMGR)

test_transmddobj: test_transmddobj.o  $(MAINLIBS)   
	$(PURIFY) $(CXX)  $(ALLFLAGS) -o $@ $^ $(INDEXIF)

# can not be used as a target (module library is not remade!)
test_mddops: test_mddops.o $(RASLIB) $(CACHETAMGR)\
	$(MDDIF) $(CATALOGIF)  $(INDEXIF) $(INDEXMGR) $(BLOBIF) $(ADMINIF)  
	$(PURIFY) $(CXX) $(STLLDFLAGS) $(LDFLAGS) $(BASEDBLDFLAGS) -o $@ $^

# can not be used as a target (module library is not remade!)
test_mddjoin: test_mddjoin.o $(RASLIB) $(CACHETAMGR)\
	$(MDDIF) $(CATALOGIF)  $(INDEXIF) $(INDEXMGR) $(BLOBIF) $(ADMINIF)  
	$(PURIFY) $(CXX) $(STLLDFLAGS) $(LDFLAGS) $(BASEDBLDFLAGS) -o $@ $^

test_persmddobj: test_persmddobj.o $(RASLIB) $(CACHETAMGR)\
	$(MDDIF) $(CATALOGIF) $(INDEXMGR) $(BLOBIF) $(ADMINIF)  $(QLPARSER) \
        $(INDEXIF)
	$(PURIFY) $(CXX)  $(BASEDBLDFLAGS) $(LDFLAGS) $(STLLDFLAGS)  -o $@ $^ \
	$(STLLDFLAGS)  -lm $(CACHETAMGR) -L$(SUPPORT_BASE)/lib -lz

test_extbmarkint: test_extbmarkint.o $(MAINLIBS)  
	$(PURIFY) $(CXX)  $(BASEDBLDFLAGS) $(LDFLAGS) $(STLLDFLAGS)  -o $@ $^ \
	$(STLLDFLAGS)  -lm $(CACHETAMGR) $(INDEXIF) -L$(SUPPORT_BASE)/lib -lz 

# general rules
include $(RMANBASE)/Makefile.rel

# general dependencies for module libraries
include $(RMANBASE)/Makefile.dep

# automatically created dependencies
include Makefile.dep
