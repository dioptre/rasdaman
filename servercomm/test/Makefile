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
#   test programs of module servercomm
######################### Definitions ############################

# standard include with general options
include $(RMANBASE)/Makefile.inc

# all test programs
SRCCXX   = test_servercomm.cc test_dbcontent.cc test_oid.cc
OBJS      = ${SRCCXX:%.cc=%.o}
ALLTESTS = ${SRCCXX:%.cc=%}

# some additional flags for compiling and linking

CXXFLAGS += -I$(RMANBASE)/servercomm
LDFLAGS  := -I$(RMANBASE)/servercomm $(LDFLAGS)

# add communication flags
CXXFLAGS += $(COMMCXXFLAGS)
LDFLAGS += $(COMMLDFLAGS)  

# add compile and link options for STL
CXXFLAGS += $(STLCXXFLAGS)
LDFLAGS  += $(STLLDFLAGS) 

########################### Targets ##############################

# test target for servercomm
.PHONY : servercomm
servercomm: adminif raslib test_module test_servercomm  

# test target for dbcontent
.PHONY : dbcontent
dbcontent: adminif raslib test_module test_dbcontent

.PHONY : adminif
adminif:
	cd $(RMANBASE)/reladminif; $(MAKE)

.PHONY : raslib
raslib:
	cd $(RMANBASE)/raslib; $(MAKE)

.PHONY : qlparser
qlparser:
	cd $(RMANBASE)/qlparser; $(MAKE)

.PHONY : test_module
test_module:
	cd $(RMANBASE)/servercomm; $(MAKE)

test_servercomm: test_servercomm.o \
		 $(QLPARSER) \
                 $(SERVERCOMM) \
		 $(CACHETAMGR) \
		 $(MDDIF) \
		 $(CATALOGIF) \
		 $(INDEXIF) \
		 $(INDEXMGR) \
                 $(BLOBIF) \
		 $(ADMINIF) \
		 $(PREPROCESSOR) \
		 $(STORAGEMGR) \
		 $(TILEMGR) \
		 $(RASLIB) \
		 $(RELINDEX) \
		 $(INDEXIF) 
	$(CXX) $(LDFLAGS)  -o $@ $^ $(QLPARSER) $(INDEXMGR) $(CACHETAMGR) $(RELINDEX) $(RELSTORAGEIF) $(RELADMINIF) $(STORAGEMGR) $(CONVERSION) $(INDEXIF)  $(RELCATALOGIF) /usr/local/pgsql/lib/libpq.a /usr/local/pgsql/lib/libecpg.a -lssl -lcrypt /usr/local/pgsql/lib/libpgtypes.a -lm -ljpeg -ltiff -lpng -lmfhdf -ldf -lz -ldl -lcrypto

test_dbcontent: test_dbcontent.o \
		 $(QLPARSER) \
                 $(SERVERCOMM) \
		 $(RASLIB) \
		 $(CACHETAMGR) \
		 $(MDDIF) \
		 $(CATALOGIF) \
		 $(INDEXIF) \
		 $(INDEXMGR) \
                 $(BLOBIF) \
		 $(ADMINIF) 
	$(CXX) $(LDFLAGS) $(O2LDFLAGS) -o $@ $^ $(QLPARSER) $(INDEXMGR) $(CACHETAMGR) -lm

test_oid: test_oid.o $(RASLIB) $(CACHETAMGR)\
	$(MDDIF) $(CATALOGIF)  $(INDEXIF) $(INDEXMGR) $(BLOBIF) $(ADMINIF)  
	$(PURIFY) $(CXX)  $(O2LDFLAGS) $(LDFLAGS) -o $@ $^ \
	$(RASLIB) -lm -L$(SUPPORT_BASE)/lib -lz


# general rules
include $(RMANBASE)/Makefile.rel

# automatically created dependencies
include Makefile.dep
