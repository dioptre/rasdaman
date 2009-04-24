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
#   test_error-par
#
# COMMENTS:
#
##################################################################

######################### Definitions ############################

# use client specific flags
CXXFLAGS := $(CLIENTCXXFLAGS)
LDFLAGS  := $(CLIENTLDFLAGS)

# add communication flags
CXXFLAGS += $(COMMCXXFLAGS)
LDFLAGS += $(COMMLDFLAGS)

LIBS = $(RASODMG) $(CLIENTCOMM) $(RASLIB) \
	$(CONVERSION) $(LIBAKINSIDE) $(LIBAKNET)

IMGLIBS = $(l_SYM)tiff $(l_SYM)jpeg $(l_SYM)png $(l_SYM)crypto $(l_SYM)z \
	$(l_SYM)mfhdf $(l_SYM)df  $(l_SYM)ppm $(l_SYM)pgm $(l_SYM)pbm

LDFLAGS += $(L_SYM)$(SUPPORT_BASE)/lib

SRCCXX= test_error-par.cc
OBJS  = ${SRCCXX:%.cc=%.o}

ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
	LDFLAGS+= -lsocket
endif

MISCCLEAN = test_error-par

########################### Targets ##############################
# main target
.PHONY: all
all: test_error-par

test_error-par:  test_error-par.o $(LIBS)
	$(PURIFY) $(CXX) $(LDFLAGS) -o test_error-par $^ $(LIBS) $(IMGLIBS) -lm

.PHONY: depend
depend:
	-rm Makefile.dep
	$(MAKEDEPEND) -m -f- -- $(CXXFLAGS) -- $(SRCCXX) $(SRCCC) > Makefile.dep

.PHONY: clean
clean:
	-rm $(OBJS) $(MISCCLEAN)

######################## Dependencies ############################

