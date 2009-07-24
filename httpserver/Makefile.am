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
#   Rasdaman http server
# 
##################################################################

lib_LIBRARIES=libhttpserver.a
AM_CFLAGS=$(CXXFLAGS)
libhttpserver_a_SOURCES= types.h http.h server.h defs.h http-defs.h protos.h \
                    logging.cc signals.cc config.cc support.cc childs.cc init.cc \
                    http-support.cc http-methods.cc http-fields.cc http-date.cc \
                    http-error.cc http-readmsg.cc http-writemsg.cc \
                    http-doit.cc http.cc httpserver.h main.cc 
