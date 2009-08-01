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
#   module rasdl
# 
# COMMENTS: 
# For static linking you have to do first: setenv STATIC_LIBS=true
# 
##################################################################

AM_LFLAGS=-I
YACC = bison 
AM_YFLAGS=-d -y
AM_CXXFLAGS=@BASEDBCXXFLAGS@
AM_LDFLAGS=@BASEDBLDFLAGS@

bin_PROGRAMS=rasdl
rasdl_SOURCES=lex.ll odl.yy parse.cc parse.hh symbtbl.cc symbtbl.hh \
              alloca.cc rasdl.cc rasdl_error.cc rasdl_error.hh \
              template_inst.hh yparse.hh \
              ../mymalloc/mymalloc.h ../mymalloc/mymalloc_svc.cc
rasdl_LDADD = ../reladminif/libreladminif.a ../relmddif/librelmddif.a \
              ../relstorageif/librelstorageif.a ../relindexif/librelindexif.a \
              ../relcatalogif/librelcatalogif.a ../relblobif/librelblobif.a \
              ../indexmgr/libindexmgr.a ../catalogmgr/libcatalogmgr.a \
              ../storagemgr/libstoragemgr.a ../tilemgr/libtilemgr.a \
              ../compression/libcompression.a ../commline/libcommline.a ../raslib/libraslib.a \
              ../conversion/libconversion.a ../rasodmg/librasodmg.a

BUILT_SOURCES=lex.cc odl.cc odl.h              
CLEANFILES=lex.cc odl.cc odl.h

SUBDIRS=../reladminif ../relmddif ../relstorageif ../relindexif ../relcatalogif \
        ../relblobif ../indexmgr ../catalogmgr ../storagemgr ../tilemgr \
        ../compression ../raslib ../conversion ../commline ../rasodmg

$(RECURSIVE_CLEAN_TARGETS):
	@$(MAKE) $(AM_MAKEFLAGS) `echo $@ | sed s/-recursive/-am/`

#.PHONY : doc
#doc:
#	-rm $(DOCDIR)/*
#	gawk -f rasdl.awk odl.y > odl.grammar
#	head -n 16 rasdlgrammar.html >  $(DOCDIR)/rasdlgrammar.html
#	cat  odl.grammar             >> $(DOCDIR)/rasdlgrammar.html
#	tail -n  8 rasdlgrammar.html >> $(DOCDIR)/rasdlgrammar.html
#	$(DOCXX) -d $(DOCDIR) *.hh
#	chmod 664 $(DOCDIR)/* $(DOCDIR)/.??*

