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
#   module clientcomm
# 
# COMMENTS: 
# - build of akgnet should be revisited wrt. 3rdParty!
#
##################################################################

AM_CXXFLAGS = @CLIENTCXXFLAGS@
AM_LDFLAGS = @CLIENTLDFLAGS@

noinst_LIBRARIES=libclientcomm.a
libclientcomm_a_SOURCES = rpcif_clnt.cc rpcif_xdr.c rpcif.h \
                          clientcomm.cc clientcomm.hh rpcclientcomm.cc \
                          rpcclientcomm.hh clientcomm.icc \
                          ../mymalloc/mymalloc_cln.cc ../mymalloc/mymalloc.h \
                          ../rnprotocol/rnprotocol.cc \
                          ../rnprotocol/rnprotocol.hh \
                          ../rnprotocol/rnpembedded.cc \
                          ../rnprotocol/rnpembedded.hh \
                          ../rnprotocol/rnpcommunication.cc \
                          ../rnprotocol/rnpcommunication.hh \
                          ../rnprotocol/rnprasserver.cc \
                          ../rnprotocol/rnpclientcomm.cc \
                          ../rnprotocol/rnpclientcomm2.cc \
                          ../rnprotocol/rnpclientcomm.hh
libclientcomm_a_CPPFLAGS=-I../rnprotocol

EXTRA_libclientcomm_a_SOURCES = rpcif.x                          
BUILT_SOURCES= rpcif_clnt.cc rpcif_xdr.c rpcif.h rpcif_svc.cc
CLEANFILES=rpcif_clnt.cc rpcif_xdr.c rpcif.h rpcif.h.tmp rpcif_clnt.c.tmp \
           rpcif_svc.cc.tmp rpcif_svc.cc

rpcif.h: rpcif.x	
	$(RPCGEN) -h -o rpcif.h.tmp rpcif.x
	$(AWK) -f rpcif.h.awk rpcif.h.tmp > rpcif.h
	rm rpcif.h.tmp

rpcif_clnt.cc: rpcif.x
	$(RPCGEN) -l -o rpcif_clnt.cc.tmp rpcif.x
	$(AWK) -f rpcif_clnt.c.awk rpcif_clnt.cc.tmp > rpcif_clnt.cc
	rm rpcif_clnt.cc.tmp    

rpcif_xdr.c: rpcif.x
	-rm rpcif_xdr.c
	$(RPCGEN) -c -o rpcif_xdr.c rpcif.x

rpcif_svc.cc: rpcif.x
	$(RPCGEN) -m -o rpcif_svc.cc.tmp rpcif.x
	$(AWK) -f rpcif_svc.c.awk rpcif_svc.cc.tmp > rpcif_svc.cc
	rm rpcif_svc.cc.tmp
