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
#   test programs of module conversion
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
ifeq ($(OSTYPE),linux-gnu)
ALLTESTS = test_convertor test_png test_jpeg test_bmp test_vff
else
ALLTESTS = test_convertor test_hdf test_png test_jpeg test_bmp test_vff
endif

# For the time being: HDF includes / libs
HDFBASE	= $(SUPPORT_BASE)
HDFINC	= -I$(HDFBASE)/include
HDFLIB	= -L$(HDFBASE)/lib -ldf -ljpeg -lz -lmfhdf -lnsl

ifeq ($(OSTYPE),linux-gnu)
  LINKLIBS=$(CONVERSION) $(CLIENTCOMM) $(COMPRESSION) $(RASLIB) $(RASODMG)
  CLIENTLDFLAGS += -lz
else
  ifdef RMANGCC
    LINKLIBS=$(CONVERSION) $(CLIENTCOMM) $(COMPRESSION) $(RASLIB) $(RASODMG)
    CLIENTLDFLAGS += -lsocket -lnsl -lz 
  else
    LINKLIBS=$(CONVERSION) $(RASLIB)
  endif
endif

########################### Targets ##############################

# test target for convertors like r_TIFF
.PHONY : convertor
convertor: test_module $(ALLTESTS)

.PHONY : test_module
test_module:
	cd $(RMANBASE)/conversion; $(MAKE)

.PHONY : tiff
tiff: test_module test_convertor

.PHONY : hdf
hdf: test_module test_hdf

.PHONY : png
png: test_module test_png

.PHONY : jpeg
jpeg: test_module test_jpeg

.PHONY : bmp
bmp: test_module test_bmp

.PHONY : vff
vff: test_module test_vff


test_convertor: test_convertor.o $(LINKLIBS)
	$(PURIFY) $(CXX)  $(CLIENTLDFLAGS) -o $@ $^ -L$(SUPPORT_BASE)/lib -ltiff -ljpeg -lz -lcrypto

test_hdf: test_hdf.o $(LINKLIBS)
	$(PURIFY) $(CXX) $(CLIENTLDFLAGS)  -o $@ $^  -L$(SUPPORT_BASE)/lib -lcrypto -lz $(HDFLIB) 

test_png: test_png.o $(LINKLIBS)
	$(PURIFY) $(CXX) $(CLIENTLDFLAGS)  -o $@ $^ -L$(SUPPORT_BASE)/lib -lpng -lz -lcrypto

test_jpeg: test_jpeg.o $(LINKLIBS)
	$(PURIFY) $(CXX) $(CLIENTLDFLAGS)  -o $@ $^  -L$(SUPPORT_BASE)/lib -ljpeg -lz -lcrypto

test_bmp: test_bmp.o $(LINKLIBS)
	$(PURIFY) $(CXX) $(CLIENTLDFLAGS)  -o $@ $^  -L$(SUPPORT_BASE)/lib -lcrypto -lz

test_vff: test_vff.o $(LINKLIBS)
	$(PURIFY) $(CXX) $(CLIENTLDFLAGS)  -o $@ $^  -L$(SUPPORT_BASE)/lib -lcrypto -lz


.PHONY : clean
clean:
	-rm $(ALLTESTS)
	-rm *.o

# deletes all non modified, but checked out files
.PHONY : rcsclean
rcsclean:
	-rcsclean

######################## Dependencies ############################

test_convertor.o: test_convertor.cc
	$(CXX) test_convertor.cc -c -o $@ $(CLIENTCXXFLAGS)

test_hdf.o: test_hdf.cc
	$(CXX) test_hdf.cc -c -o $@ $(HDFINC) $(CLIENTCXXFLAGS)

test_png.o: test_png.cc
	$(CXX) test_png.cc -c -o $@ $(CLIENTCXXFLAGS)

test_jpeg.o: test_jpeg.cc
	$(CXX) test_jpeg.cc -c -o $@ $(CLIENTCXXFLAGS)

test_bmp.o: test_bmp.cc
	$(CXX) test_bmp.cc -c -o $@ $(CLIENTCXXFLAGS)

test_vff.o: test_vff.cc
	$(CXX) test_vff.cc -c -o $@ $(CLIENTCXXFLAGS)
