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
#
# Top Level makefile. This points to the various modules that have to be build
# and/or deployed
# 
# MAKEFILE FOR:  
# Compile and link example C++ programs;
# right now, works only with GNU Make!
# 
# 
##################################################################

######################### Definitions ############################
# RMANHOME will be updated with a proper value after "make install"
RMANHOME=~~~rmanhome~~~
# RasDaMan central includes
CXXFLAGS += -I$(RMANHOME)/include
CXXFLAGS += -DLINUX -DEARLY_TEMPLATE
# add communication flags
CXXFLAGS += -DONCRPC

# libraries needed for linkage (in particular: rasdaman + exchange formats)
LIBS     += -L/usr/lib -L$(RMANHOME)/lib \
	    -lclientcomm -lrasodmg -lcompression -lconversion -lclientcomm -lrasodmg -lraslib  \
	    -lnetwork -lnetpbm -ljpeg -lpng -ltiff -lmfhdf -ldf -lcrypto \
	    -lm -lz

########################### Targets ##############################

all: avg-cell avg-cell-red lookup query insertppm

avg-cell: avg-cell.o 
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

avg-cell-red: avg-cell-red.o 
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

lookup: lookup.o 
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

query: query.o 
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

insertppm:	insertppm.o
	 $(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY : clean
clean:
	-rm -f *.o *.log *.dbg *.bm
	-rm -f query insertppm lookup avg-cell avg-cell-red

######################## Dependencies ############################

avg-cell.o: avg-cell.cc

avg-cell-red.o: avg-cell-red.cc

lookup.o: lookup.cc

query.o: query.cc

insertppm.o: insertppm.cc
	$(CXX) $(CXXFLAGS) -c insertppm.cc -I/usr/X11R6/include

# end of Makefile
