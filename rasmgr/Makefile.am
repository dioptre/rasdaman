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
# * MODULE: rasmgr
# * 
# * COMMENTS:
##########################################################################

AM_CXXFLAGS=@BASEDBCXXFLAGS@
AM_LDFLAGS=@BASEDBLDFLAGS@ 

bin_PROGRAMS=rasmgr
rasmgr_SOURCES=ras_crypto.cc rasmgr_config.cc rasmgr_host.cc rasmgr_main.cc rasmgr_rascontrol.cc rasmgr_srv.cc \
         	   rasmgr_comm.cc rasmgr_dbm.cc rasmgr_master_nb.cc rasmgr_rascontrol_help.cc \
               rasmgr_users.cc rasmgr_comm_nb.cc rasmgr_error.cc rasmgr_localsrv.cc rasmgr_random.cc  \
               hostcmp.cc \
               ras_crypto.hh rasmgr_config.hh rasmgr_host.hh rasmgr_rascontrol.hh rasmgr_srv.hh \
         	   rasmgr_comm.hh rasmgr_dbm.hh \
               rasmgr_users.hh rasmgr_comm_nb.hh rasmgr_error.hh rasmgr_localsrv.hh \
               rasmgr.hh rasmgr_protocol.hh rasmgr_master.hh \
               ../mymalloc/mymalloc_cln.cc ../mymalloc/mymalloc.h

rasmgr_LDADD= ../raslib/libraslib.a ../network/libnetwork.a ../commline/libcommline.a

SUBDIRS=../mymalloc ../network ../commline ../raslib

$(RECURSIVE_CLEAN_TARGETS):
	@$(MAKE) $(AM_MAKEFLAGS) `echo $@ | sed s/-recursive/-am/`




#ifdef STATIC_LIBS
#  EXTRASTATICLIBS= -Xlinker -Bstatic -nodefaultlibs -lstdc++
# EXTRADINAMICLIBS= -Xlinker -Bdynamic -lm -lgcc -lc -lgcc
#endif
#SRCCXX = ras_crypto.cc rasmgr_config.cc rasmgr_host.cc rasmgr_main.cc rasmgr_rascontrol.cc rasmgr_srv.cc \
#	rasmgr_comm.cc rasmgr_dbm.cc rasmgr_master_nb.cc rasmgr_rascontrol_help.cc \
#	rasmgr_users.cc rasmgr_comm_nb.cc rasmgr_error.cc rasmgr_localsrv.cc rasmgr_random.cc rasmgr_slave.cc \
#	hostcmp.cc


#rasmgr: rasmgr_main.o rasmgr_config.o rasmgr_comm.o rasmgr_comm_nb.o rasmgr_host.o rasmgr_dbm.o rasmgr_srv.o rasmgr_random.o \
#        rasmgr_master_nb.o rasmgr_rascontrol.always rasmgr_users.o ras_crypto.o rasmgr_slave.o rasmgr_localsrv.o rasmgr_error.o hostcmp.o
#	$(PURIFY) $(CXX) $(CXXFLAGS) -o rasmgr rasmgr_main.o rasmgr_config.o rasmgr_comm.o rasmgr_comm_nb.o rasmgr_host.o rasmgr_dbm.o rasmgr_master_nb.o rasmgr_rascontrol.o rasmgr_users.o ras_crypto.o rasmgr_slave.o rasmgr_localsrv.o rasmgr_error.o rasmgr_random.o rasmgr_srv.o hostcmp.o \
#	$(LDFLAGS) $(EXTRASTATICLIBS) -lcrypto $(EXTRADINAMICLIBS) $(NETWORK) $(COMMLINE)

