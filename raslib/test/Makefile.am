#
# This file is part of rasdaman community.
# 
# Rasdaman community is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Rasdaman community is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
# 
# Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
# 
# For more information please see <http://www.rasdaman.org>
# or contact Peter Baumann via <baumann@rasdaman.com>.
# 
# MAKEFILE FOR:
# 
# COMMENTS: 
# 
##################################################################

AM_CXXFLAGS=@CLIENTCXXFLAGS@
AM_LDFLAGS=@CLIENTLDFLAGS@

TESTS = test_endian test_error test_metaobject test_minterval test_miterd \
        test_oid test_params test_point test_rmdebug test_sinterval test_timer
check_PROGRAMS = test_endian test_error test_metaobject test_minterval test_miter test_miterd \
		 test_oid test_params test_point test_rmdebug test_sinterval test_timer

test_endian_SOURCES = test_endian.cc
test_error_SOURCES = test_error.cc ../../mymalloc/mymalloc_cln.cc
test_metaobject_SOURCES = test_metaobject.cc
test_minterval_SOURCES = test_minterval.cc ../../mymalloc/mymalloc_cln.cc
test_miter_SOURCES = test_miter.cc
test_miterd_SOURCES = test_miterd.cc ../../mymalloc/mymalloc_cln.cc

# compiler errors in raslib/miterf.icc
# test_miterf_SOURCES = test_miterf.cc

test_oid_SOURCES = test_oid.cc ../../mymalloc/mymalloc_cln.cc
test_params_SOURCES = test_params.cc ../../mymalloc/mymalloc_cln.cc
test_point_SOURCES = test_point.cc ../../mymalloc/mymalloc_cln.cc
test_rmdebug_SOURCES = test_rmdebug.cc ../../mymalloc/mymalloc_cln.cc
test_sinterval_SOURCES = test_sinterval.cc ../../mymalloc/mymalloc_cln.cc
test_timer_SOURCES = test_timer.cc ../../mymalloc/mymalloc_cln.cc

LDADD = ../../rasodmg/librasodmg.a ../../clientcomm/libclientcomm.a \
	../libraslib.a \
	../../conversion/libconversion.a ../../network/libnetwork.a

SUBDIRS = ../../mymalloc
