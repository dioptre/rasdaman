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
#   test programs of module relblobif
# 
# COMMENTS: 
#   List environment dependencies, known bugs, specialities etc.
# 
##################################################################
######################### Definitions ############################

# standard include with general options
include $(RMANBASE)/Makefile.inc

# all test programs
ALLTESTS = dbmddcollidt dbmddcolloidentryt mdditest dbiteritest dbobjtest intervaltest

# source files
CPPSOURCES =

# object files
OBJS = dbmddcollidt.o dbmddcolloidentryt.o mdditest.o dbobjtest.o dbiteritest.o intervaltest.o

NEEDSTL = dbmddcollidt.C dbmddcolloidentryt.C mdditest.C dbiteritest.C intervaltest.C dbobjtest.C

MISCCLEAN = client.bm client.dbg client.log ir.out core

NEEDEDLIBS = $(RELBLOBIF) $(RELADMINIF) $(RASLIB) $(RELCATALOGIF) $(RELMDDIF) $(RASLIB) $(CACHETAMGR) $(RELINDEXIF) $(RELBLOBIF) $(RELADMINIF)

########################### Targets ##############################

# make all tests
.PHONY: test
test: $(ALLTESTS)

########################### Dependencies #########################

dbmddcollidt : dbmddcollidt.o $(NEEDEDLIBS)
dbmddcollidt.o : dbmddcollidt.C

dbmddcolloidentryt : dbmddcolloidentryt.o $(NEEDEDLIBS)
dbmddcolloidentryt.o : dbmddcolloidentryt.C

mdditest: mdditest.o $(NEEDEDLIBS)
mdditest.o: mdditest.C

intervaltest: intervaltest.o $(NEEDEDLIBS)
intervaltest.o: intervaltest.C

dbobjtest: dbobjtest.o $(NEEDEDLIBS)
dbobjtest.o: dbobjtest.C

dbitertest: dbitertest.o $(NEEDEDLIBS)
dbitertest.o: dbitertest.C

