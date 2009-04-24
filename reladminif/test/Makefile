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
# reladminif tests.
# 
# COMMENTS: 
# - List environment dependencies, known bugs, specialities etc.
# 
##################################################################
######################### Definitions ############################

# all test programs
ALLTESTS  = test_databaseif dbreftest admintest perstest eoidtest dbnamedobjtest testindex testconnect test_databaseif

# Needed rasdaman libraries for linking. To be completed.
NEEDEDLIBS = $(CACHETAMGR) $(QLPARSER) $(INDEXMGR) $(RELADMINIF) $(RELMDDIF) $(RELBLOBIF) $(RELINDEXIF) $(CACHETAMGR) $(RELCATALOGIF) $(RASLIB) $(RELADMINIF) $(RELMDDIF)

# insert names of source files which need STL<TOOLKIT>
# also insert names of source files which include .hh files which need stl 
NEEDSTL := dbreftest.C eoidtest.C admintest.C perstest.C dbnamedobjtest.C testindex.c

# insert any files that are not object, bind or generated .c/.C files
# e.g.: libs
MISCCLEAN := client.bm client.dbg client.log ir.out core

# insert object files here.  makes only sense if oracle and db2 files are the same
OBJS := dbreftest.o admintest.o perstest.o eoidtest.o dbnamedobjtest.o testindex.o testconnect.o test_databaseif.o

# add to compile flag set
CXXFLAGS += $(I_SYM)$(RMANBASE)/raslib 

########################### Targets ##############################

# make all tests
.PHONY: test
test: $(ALLTESTS)

indextest.o: indextest.cc
	$(CXX) -c $(BASEDBCXXFLAGS) $(STLCXXFLAGS) $(CXXFLAGS) $<

######################## Dependencies ############################

admintest: admintest.o $(NEEDEDLIBS)
admintest.o: admintest.C

dbreftest: dbreftest.o
dbreftest.o: dbreftest.C

eoidtest: eoidtest.o
eoidtest.o: eoidtest.C

dbnamedobjtest: dbnamedobjtest.o
dbnamedobjtest.o: dbnamedobjtest.C

perstest: perstest.o
perstest.o: perstest.C

dbcheck.o: dbcheck.c
dbcheck.c: dbcheck.pc
dbcheck: dbcheck.o
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -L/home/proj/rasdaman/3rdParty/linux/lib -L$(RMANHOME)/lib -Xlinker -Bstatic $(LIBAKINSIDE) -lmddmgr -ltilemgr -lindexmgr -lcatalogmgr -lstoragemgr -lreladminif -lrelmddif -lrelstorageif -lrelindexif -lrelcatalogif -lrelblobif -lcompression -lconversion -lraslib $(RMANHOME)/mymalloc/mymalloc_svc.o -lpng -ltiff -ljpeg -lmfhdf -ldf -lz $(REL_STATIC_LIBS)

check: check.o
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ -L$(RMANHOME)/lib -Xlinker -Bstatic $(LIBAKINSIDE) -lmddmgr -ltilemgr -lindexmgr -lcatalogmgr -lstoragemgr -lreladminif -lrelmddif -lrelstorageif -lrelindexif -lrelcatalogif -lrelblobif -lcompression -lconversion -lraslib $(RMANHOME)/mymalloc/mymalloc_svc.o -lpng -ltiff -ljpeg -lmfhdf -ldf -lz $(REL_STATIC_LIBS)

test_databaseif: test_databaseif.o ../databaseif.o
	$(PURIFY) $(CXX) $(LDFLAGS) -o $@ $^ $(RMANBASE)/mymalloc/mymalloc_svc.o ../sqlerror.o ../oidif.o ../adminif.o -L$(RMANBASE)/lib -lraslib -lrasodmg $(BASEDBLDFLAGS)

