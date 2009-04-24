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
SRCCXX= directql.cc directql_error.cc directql_signal.cc
OBJS      = ${SRCCXX:%.cc=%.o}
ALLTESTS = ${SRCCXX:%.cc=%}

# some additional flags for compiling and linking

CXXFLAGS += -I$(RMANBASE)/servercomm -I$(RMANBASE)/commline
LDFLAGS  := -I$(RMANBASE)/servercomm $(LDFLAGS)

# add communication flags
CXXFLAGS += $(COMMCXXFLAGS)
LDFLAGS += $(COMMLDFLAGS) 

# add compile and link options for STL
CXXFLAGS += $(STLCXXFLAGS)
LDFLAGS  += $(STLLDFLAGS) -L$(RMANBASE)/lib

########################### Targets ##############################

# test target for servercomm
.PHONY : directql

directql: $(OBJS) \
		 $(QLPARSER) \
	         $(SERVERCOMM) \
	         $(CLIENTCOMM) \
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
	$(CXX) $(LDFLAGS)  -o $@ $^ -lrasodmg $(CLIENTCOMM) -lnetwork $(QLPARSER) $(INDEXMGR) $(CACHETAMGR) $(RELINDEX) $(RELSTORAGEIF) $(RELADMINIF) $(STORAGEMGR) $(CONVERSION) $(INDEXIF)  $(RELCATALOGIF) -lcommline -lpq -lecpg -lssl -lcrypto -lcrypt -lpgtypes -lm -ljpeg -ltiff -lpng -lmfhdf -ldf -lz -ldl -rdynamic -Xlinker --export-dynamic


# general rules
include $(RMANBASE)/Makefile.rel

# automatically created dependencies
include Makefile.dep
