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
# 
# MAKEFILE FOR:  
#   test programs of module relblobif
# 
# COMMENTS: 
#   List environment dependencies, known bugs, specialities etc.
# 
##################################################################
######################### Definitions ############################

LDFLAGS += $(L_SYM)$(RMANHOME)/lib $(LIBAKINSIDE) $(RMANBASE)/mymalloc/mymalloc_cln.o -lcrypt

# source files
CPPSOURCES = updateblob.c deleteblob.c createtable.c droptable.c insertblob.c readblob.c r.c r2.c # copytile.c

# all test programs
ALLTESTS = ${CPPSOURCES:%.c=%}

# object files
OBJS = ${CPPSOURCES:%.c=%.o}

NEEDSTL = 

# Needed rasdaman libraries for linking. To be completed.
#NEEDEDLIBS = $(QLPARSER) $(RELBLOBIF) $(RELADMINIF) $(RASLIB) $(RELCATALOGIF) $(RELMDDIF) $(RASLIB) $(CACHETAMGR) $(RELINDEXIF) $(CACHETAMGR) $(RASQL) $(QLPARSER) 

MISCCLEAN = core client.bm client.dbg client.log ir.out

########################### Targets ##############################

# make all tests
.PHONY: test
test: $(ALLTESTS)

########################### Dependencies #########################

blobdump: blobdump.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(BASEDBLDFLAGS) $(L_SYM)$(PGSQL_ROOT)/lib $(l_SYM)ecpg
blobdump.o: blobdump.c
blobdump.c: blobdump.pgc

