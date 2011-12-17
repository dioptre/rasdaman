# 
# MAKEFILE FOR:  
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
# Top Level makefile. This points to the various modules that have to be build
# and/or deployed
#
#
# COMMENTS: 
#
##################################################################
AUTOMAKE_OPTIONS = foreign
RASMGR_PORT=@RASMGR_PORT@
RMANBASE=@abs_top_srcdir@

CHECKLIMITS=@CHECKLIMITS@

ACLOCAL_AMFLAGS = -I m4

nobase_include_HEADERS=raslib/*.h raslib/*.hh raslib/*.icc raslib/*.cc \
		rasodmg/*.hh rasodmg/*.cc rasodmg/*.icc conversion/*.h conversion/*.hh \
		clientcomm/*.h clientcomm/*.hh commline/*.hh commline/*.cc debug/*.hh

SUBDIRS = relblobif relindexif relmddif relcatalogif reladminif relstorageif \
          indexmgr catalogmgr tilemgr storagemgr commline network \
          raslib clientcomm servercomm rasodmg qlparser conversion \
          mddmgr httpserver mymalloc server rasmgr rascontrol rasdl \
          applications bin insertutils manuals_and_examples include \
          systemtest java

dist_log_DATA=empty
            
# excluded for the moment, contains nothing that is delivered:
# exportutils 

