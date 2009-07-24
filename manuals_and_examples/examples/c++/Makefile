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
# Compile and link example C++ programs;
# right now, works only with GNU Make!
# 
# 
##################################################################


######################### Definitions ############################

# choose C++ compiler
ifeq ($(OSTYPE),linux)
OSTYPE=linux-gnu
endif
ifeq ($(OSTYPE),Linux)
OSTYPE=linux-gnu
endif

ifneq ($(OSTYPE),linux-gnu)
endif
ifneq ($(OSTYPE),linux-gnu)
CXX = CC
else
CXX = g++
endif

# RasDaMan central includes
CXXFLAGS = -I$(RMANHOME)/include

ifeq ($(OSTYPE),solaris)

  CXXFLAGS += -DSOLARIS 

  # enable exception handling
  # exceptions are supported by default

  # use ANSI C
  # is used by default

else
ifeq ($(OSTYPE),linux-gnu)

  CXXFLAGS += -DLINUX -DEARLY_TEMPLATE

  # enable exception handling
  # exceptions are supported by default

  # use ANSI C
  # is used by default

else 

  CXXFLAGS += -DHPUX

  # enable exception handling
  CXXFLAGS += +eh

  # use ANSI C
  CXXFLAGS += +a1

  # necessary for templates because of bug in nm
  CXXFLAGS += -ptb

endif
endif

ifeq ($(OSTYPE),solaris)
  LDFLAGS += -lmalloc -lsocket -lnsl 
endif

# add communication flags
CXXFLAGS += -DONCRPC

# pre-installed exchange format libraries needed
FMTLIBS  = /usr/lib/libnetpbm.a /usr/lib/libjpeg.a /usr/local/lib/libpng.a /usr/local/lib/libtiff.a

# libraries needed for linkage (in particular: rasdaman + exchange formats)
LIBS     += -L/usr/lib -L$(RMANHOME)/lib \
	    -lclientcomm -lrasodmg -lcompression -lconversion -lclientcomm -lrasodmg -lraslib  \
	    -lppm -lpgm -lnetpbm -ljpeg -lpng -ltiff -lmfhdf -ldf -lcrypto  -lnetwork \
	    -lm -lz


########################### Targets ##############################

# this global target first checks for required libraries
# disable target 'check' if you don't want this to be run
all: check avg-cell avg-cell-red lookup query insertppm

# check whether exchange format libraries have been installed
.PHONY: check
check:
	@for lib in $(FMTLIBS);			\
	do					\
		if [[ ! -f $$lib ]];		\
		then				\
			echo "error: cannot find required library $$lib - package not installed?";	\
		fi				\
	done

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
	-rm *.o *.log *.dbg *.bm
	-rm query insertppm lookup avg-cell avg-cell-red
ifeq ($(OSTYPE),solaris)
	cd $(RMANHOME)/include/rasodmg/ptrepository; ptclean
else
ifneq ($(OSTYPE),linux-gnu)
	-rm -R $(RMANHOME)/include/rasodmg/ptrepository/*
endif
endif

######################## Dependencies ############################

avg-cell.o: avg-cell.cc

avg-cell-red.o: avg-cell-red.cc

lookup.o: lookup.cc

query.o: query.cc

insertppm.o: insertppm.cc
	$(CXX) $(CXXFLAGS) -c insertppm.cc -I/usr/X11R6/include

# end of Makefile
