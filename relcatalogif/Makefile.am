# -*-Makefile-*- (for Emacs)
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
# MAKEFILE FOR:  
#   relcatalogif
# 
# COMMENTS: 
# 
##################################################################

#
# This Makefile only works with GNU Make!
#

######################### Definitions ############################

AM_CXXFLAGS=@BASEDBCXXFLAGS@
AM_LDFLAGS=@BASEDBLDFLAGS@

.SUFFIXES= .@EMBEDDEDSQLEXT@ .@EMBEDDEDSQLOUT@
.@EMBEDDEDSQLEXT@.@EMBEDDEDSQLOUT@:
	$(EMBEDDEDSQLPRECOMPILER) $@ $<

lib_LIBRARIES=librelcatalogif.a
librelcatalogif_a_SOURCES= type.C type.hh basetype.C basetype.hh atomictype.C ulongtype.C ulongtype.hh ushorttype.C ushorttype.hh \
                       booltype.C booltype.hh compositetype.C compositetype.hh structtypecommon.cc structtype.hh structtype.icc \
                       longtype.C longtype.hh shorttype.C shorttype.hh octettype.C octettype.hh doubletype.C doubletype.hh\
                       floattype.C floattype.hh chartype.C chartype.hh collectiontype.C collectiontype.hh settypecommon.cc settype.hh\
                       mddtypecommon.cc mddtype.hh mddbasetypecommon.cc mddbasetype.hh mdddomaintypecommon.cc mdddomaintype.hh\
                       mdddimensiontypecommon.cc mdddimensiontype.hh dbminterval.hh inlineminterval.cc inlineminterval.hh \
                       typeiterator.hh atomictype.hh uintegraltype.hh uintegraltype.icc integraltype.hh \
                       integraltype.icc realtype.hh realtype.icc alltypes.hh complextype.hh complextype.icc
EXTRA_librelcatalogif_a_SOURCES= settype.pgc mddtype.pgc mddbasetype.pgc mdddomaintype.pgc mdddimensiontype.pgc structtype.pgc dbminterval.pgc
librelcatalogif_a_LIBADD= settype.$(OBJEXT) mddtype.$(OBJEXT) mddbasetype.$(OBJEXT) mdddimensiontype.$(OBJEXT) \
                      structtype.$(OBJEXT) dbminterval.$(OBJEXT) mdddomaintype.$(OBJEXT)
librelcatalogif_a_DEPENDENCIES= settype.$(OBJEXT) mddtype.$(OBJEXT) mddbasetype.$(OBJEXT) mdddimensiontype.$(OBJEXT) \
                            structtype.$(OBJEXT) dbminterval.$(OBJEXT) mdddomaintype.$(OBJEXT)

BUILT_SOURCES= settype.@EMBEDDEDSQLOUT@ mddtype.@EMBEDDEDSQLOUT@ mddbasetype.@EMBEDDEDSQLOUT@ \
               mdddimensiontype.@EMBEDDEDSQLOUT@ structtype.@EMBEDDEDSQLOUT@ dbminterval.@EMBEDDEDSQLOUT@ \
               mdddomaintype.@EMBEDDEDSQLOUT@


CLEANFILES=settype.@EMBEDDEDSQLOUT@ mddtype.@EMBEDDEDSQLOUT@ mddbasetype.@EMBEDDEDSQLOUT@ \
           mdddimensiontype.@EMBEDDEDSQLOUT@ structtype.@EMBEDDEDSQLOUT@ dbminterval.@EMBEDDEDSQLOUT@ \
           mdddomaintype.@EMBEDDEDSQLOUT@ \
           client.bm client.dbg client.log ir.out core



