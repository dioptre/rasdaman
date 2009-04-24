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
# *
# * MODULE: rascontrol
# * CLASS:  
# *
# * COMMENTS:
# *  
######################## Defines ###############################

CRYPTO=../rasmgr/ras_crypto.o

SRCCXX=rascontrol.cc rasmgr_utils_comm.cc rasmgrtest.cc raspasswd.cc \
	rascontrol_class.cc rasmgr_tester.cc rasmgr_utils_conf.cc rasmgrtester_conf.cc

bin_PROGRAMS=rascontrol raspasswd 

rascontrol_SOURCES=rascontrol.cc rascontrol_class.cc rascontrol.hh \
                   ../rasmgr/ras_crypto.cc ../rasmgr/ras_crypto.hh \
                   rasmgr_utils_conf.cc rasmgr_utils_conf.hh \
                   rasmgr_utils_comm.cc rasmgr_utils_comm.hh 
rascontrol_LDADD = ../network/libnetwork.a ../commline/libcommline.a

raspasswd_SOURCES=raspasswd.cc raspasswd.hh \
                  ../rasmgr/ras_crypto.cc ../rasmgr/ras_crypto.hh \
                  rasmgr_utils_comm.cc rasmgr_utils_comm.hh \
                  rasmgr_utils_conf.cc rasmgr_utils_conf.hh
raspasswd_LDADD = ../network/libnetwork.a ../commline/libcommline.a                  

SUBDIRS=../network ../commline

$(RECURSIVE_CLEAN_TARGETS):
	@$(MAKE) $(AM_MAKEFLAGS) `echo $@ | sed s/-recursive/-am/`

