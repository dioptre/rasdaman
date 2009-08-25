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
#   module qlparser
# 
# 
# COMMENTS: 
#   
# 
##################################################################

lib_LIBRARIES=libqlparser.a

# -I gnererates an interactive scanner which doesn't try to look ahead past a newline
# -i generates a scanner which doesn't care about upper and lower case; doesn't work
AM_LFLAGS=-i -I

YACC = bison 
# -d generates token definitions in .h file
AM_YFLAGS=-d -y
AM_CXXFLAGS=@BASEDBCXXFLAGS@
AM_CFLAGS=$(CXXFLAGS)
libqlparser_a_SOURCES=oql.yy lex.ll alloca.c\
                      symtab.cc symtab.hh \
                      qtoncstream.cc qtoncstream.hh qtoncstream.icc \
                      qtmddaccess.cc qtmddaccess.hh qtmddaccess.icc \
                      qtiterator.cc qtiterator.hh qtiterator.icc \
                      qtoperationiterator.cc qtoperationiterator.hh qtoperationiterator.icc \
                      qtselectioniterator.cc qtselectioniterator.hh qtselectioniterator.icc \
                      qtjoiniterator.cc qtjoiniterator.hh qtjoiniterator.icc \
                      qtoperation.cc qtoperation.hh qtoperation.icc \
                      qtbinaryoperation.cc qtbinaryoperation.hh qtbinaryoperation.icc \
                      qtbinaryinduce.cc qtbinaryinduce.hh qtbinaryinduce.icc \
                      qtbinaryinduce2.cc qtbinaryinduce2.hh qtbinaryinduce2.icc \
                      qtunaryoperation.cc qtunaryoperation.hh qtunaryoperation.icc \
                      qtunaryinduce.cc qtunaryinduce.hh qtunaryinduce.icc \
                      qtvariable.cc qtvariable.hh qtvariable.icc \
                      qtconst.cc qtconst.hh qtconst.icc \
                      qtdata.cc qtdata.hh qtdata.icc \
                      qtscalardata.cc qtscalardata.hh qtscalardata.icc \
                      qtatomicdata.cc qtatomicdata.hh qtatomicdata.icc \
                      qtcomplexdata.cc qtcomplexdata.hh qtcomplexdata.icc \
                      qtmdd.cc qtmdd.hh qtmdd.icc \
                      qtstringdata.cc qtstringdata.hh qtstringdata.icc \
                      qtcondense.cc qtcondense.hh qtcondense.icc \
                      parseinfo.cc parseinfo.hh parseinfo.icc \
                      qtdomainoperation.cc qtdomainoperation.hh qtdomainoperation.icc \
                      qtconversion.cc qtconversion.hh qtconversion.icc\
                      qtupdate.cc qtupdate.hh qtupdate.icc \
                      qtinsert.cc qtinsert.hh qtinsert.icc \
                      qtdelete.cc qtdelete.hh \
                      qtcommand.cc qtcommand.hh qtcommand.icc \
                      qtoid.cc qtoid.hh qtoid.icc\
                      qtintervalop.cc qtintervalop.hh qtintervalop.icc \
                      qtmintervalop.cc qtmintervalop.hh qtmintervalop.icc \
                      qtintervaldata.cc qtintervaldata.hh qtintervaldata.icc \
                      qtpointop.cc qtpointop.hh qtpointop.icc \
                      qtmintervaldata.cc qtmintervaldata.hh qtmintervaldata.icc \
                      qtpointdata.cc qtpointdata.hh qtpointdata.icc \
                      qtnaryoperation.cc qtnaryoperation.hh qtnaryoperation.icc \
                      qtunaryfunc.cc qtunaryfunc.hh qtunaryfunc.icc \
                      qtbinaryfunc.cc qtbinaryfunc.hh qtbinaryfunc.icc \
                      qtmarrayop.cc qtmarrayop.hh qtmarrayop.icc \
                      qtmarrayop2.cc qtmarrayop2.hh qtmarrayop2.icc \
                      qtcondenseop.cc qtcondenseop.hh qtcondenseop.icc \
                      qtnode.cc qtnode.hh qtnode.icc \
                      querytree.cc querytree.hh querytree.icc \
                      qtexecute.hh qtdelete.icc \
					  qtmddconfig.hh qtmddconfig.cc \
					  qtmddcfgop.hh qtmddcfgop.cc

EXTRA_libqlparser_a_SOURCES = autogen_qtui.hh autogen_qtui.icc autogen_qtui.cc

BUILT_SOURCES=lex.cc oql.cc oql.h
CLEANFILES=lex.cc oql.cc oql.h
