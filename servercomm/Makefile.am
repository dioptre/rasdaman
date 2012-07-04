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
#   module servercomm
# 
# COMMENTS: 
# 
##################################################################

AM_CXXFLAGS=@BASEDBCXXFLAGS@
AM_LDFLAGS=@BASEDBLDFLAGS@

lib_LIBRARIES=libservercomm.a
libservercomm_a_SOURCES=../clientcomm/rpcif_xdr.c ../clientcomm/rpcif_svc.cc servercomm.cc \
                        servercomm2.cc manager.cc callbackmgr.cc httpserver.cc \
                        ../mymalloc/mymalloc_svc.cc ../mymalloc/mymalloc.h \
                        ../clientcomm/rpcif.h callbackmgr.hh httpserver.hh httpserver.icc \
                        servercomm.hh servercomm.icc \
                        ../rnprotocol/rnprotocol.cc \
                        ../rnprotocol/rnprotocol.hh \
                        ../rnprotocol/rnpembedded.cc \
                        ../rnprotocol/rnpembedded.hh \
                        ../rnprotocol/rnpcommunication.cc \
                        ../rnprotocol/rnpcommunication.hh \
                        ../rnprotocol/rnpserver.cc \
                        ../rnprotocol/rnpserver.hh \
                        ../rnprotocol/srvrasmgrcomm.cc \
                        ../rnprotocol/srvrasmgrcomm.hh \
                        ../rnprotocol/rnprasserver.cc \
                        ../rnprotocol/rnprasserver.hh \
                        ../rnprotocol/rnpservercomm.cc \
                        ../rnprotocol/rnpservercomm.hh
libservercomm_a_CPPFLAGS=-I../rnprotocol                        

BUILT_SOURCES=../clientcomm/rpcif_xdr.c ../clientcomm/rpcif.h ../clientcomm/rpcif_svc.cc

#.PHONY: ../clientcomm/rpcif_xdr.c ../clientcomm/rpcif.h ../clientcomm/rpcif_svc.cc
../clientcomm/rpcif_xdr.c:
	cd ../clientcomm ; $(MAKE) rpcif_xdr.c

../clientcomm/rpcif.h:
	cd ../clientcomm ; $(MAKE) rpcif.h

../clientcomm/rpcif_svc.cc:
	cd ../clientcomm ; $(MAKE) rpcif_svc.cc
