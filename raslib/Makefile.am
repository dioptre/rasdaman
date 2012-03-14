# -*-Makefile-*- (for Emacs)
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
# MAKEFILE FOR:  
#   module raslib
# 
# COMMENTS: 
# 
##################################################################

AM_CXXFLAGS=@CLIENTCXXFLAGS@
AM_LDFLAGS=@CLIENTLDFLAGS@

lib_LIBRARIES=libraslib.a
libraslib_a_SOURCES= sinterval.hh dlist.hh point.hh minterval.hh error.hh \
                     rmdebug.hh rminit.hh metaobject.hh type.hh marraytype.hh \
                     basetype.hh primitivetype.hh sintervaltype.hh \
                     collectiontype.hh property.hh attribute.hh mintervaltype.hh \
                     pointtype.hh oidtype.hh structuretype.hh itertype.hh \
                     oid.hh scalar.hh primitive.hh structure.hh miter.hh mddtypes.hh \
                     miterd.hh mitera.hh memblockvec.hh parseparams.hh storageman.hh \
                     endian.hh flatbasetype.hh complex.hh complextype.hh shhopt.h \
                     sinterval.cc dlist.cc point.cc minterval.cc error.cc \
                     rmdebug.cc rminit.cc metaobject.cc type.cc marraytype.cc \
                     basetype.cc primitivetype.cc sintervaltype.cc \
                     collectiontype.cc property.cc attribute.cc mintervaltype.cc \
                     pointtype.cc oidtype.cc structuretype.cc itertype.cc \
                     oid.cc scalar.cc primitive.cc structure.cc miter.cc mddtypes.cc \
                     miterd.cc mitera.cc memblockvec.cc parseparams.cc storageman.cc \
                     endian.cc flatbasetype.cc complex.cc complextype.cc shhopt.c \
                     complex.icc complextype.icc error.icc minterval.icc miterd.icc miterf.icc \
                     miter.icc oid.icc point.icc rmdebug.icc sinterval.icc \
                     rmdebug.hh rm.hh odmgtypes.hh miterf.hh template_inst.hh

SUBDIRS = test