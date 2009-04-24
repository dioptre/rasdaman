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
#   the test program for a module
# 
# COMMENTS: 
#   List environment dependencies, known bugs, specialities etc.
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

# object files for test program
TESTOBJS = test/test.o
# name of test program
TESTPRG = testex

########################### Targets ##############################

$(TESTPRG): $(TESTOBJS) $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

.PHONY : clean
clean:
	-rm $(TESTOBJS)
	-rm $(TESTPRG)

# deletes all non modified, but checked out files
.PHONY : rcsclean
rcsclean:
	-rcsclean

######################## Dependencies ############################

test.o: $(INCDIR)/m1.hh
