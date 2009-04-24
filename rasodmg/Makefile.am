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
#   module rasodmg
#
# COMMENTS: 
# 
##################################################################

AM_CXXFLAGS=@CLIENTCXXFLAGS@
AM_LDFLAGS=@CLIENTLDFLAGS@

noinst_LIBRARIES=librasodmg.a

librasodmg_a_SOURCES = collection.cc collection.hh collection.icc \
                       set.cc set.hh \
                       iterator.cc iterator.hh iterator.icc \
                       marray.cc marray.hh marray.icc \
                       ref.cc ref.hh ref.icc \
                       transaction.cc transaction.hh transaction.icc \
                       oqlquery.cc oqlquery.hh oqlquery.icc \
                       object.cc object.hh object.icc \
                       database.cc database.hh database.icc \
                       gmarray.cc gmarray.hh gmarray.icc \
                       storagelayout.cc storagelayout.hh storagelayout.icc \
                       tiling.cc tiling.hh \
                       alignedtiling.cc alignedtiling.hh alignedtiling.icc \
                       dirtiling.cc dirtiling.hh \
                       dirdecompose.cc dirdecompose.hh \
                       interesttiling.cc interesttiling.hh \
                       stattiling.cc stattiling.hh \
                       partinsert.cc partinsert.hh \
                       polygon.cc polygon.hh \
                       polycutout.cc polycutout.hh

CLEANFILES = core

