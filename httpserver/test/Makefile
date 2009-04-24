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
# or contact Peter Baumann via <baumann@rasdaman.com>. # Top Level makefile. This points to the various modules that have to be build
# and/or deployed
#
# MAKEFILE FOR:  
#   a RasDaMan module
# 
# COMMENTS: 
#   This Makefile was created during the relational port when 
#   splitting up catalogif into O2 dependent und not O2 dependent.
#
# IMPORTANT:
#   As opposed to all other Makefiles catalogmgr does not have 
#   its own library. catalogif is used instead, otherwise the
#   linking of all executables would have had to be changed.
# 
##################################################################

#
# IMPORTANT:
#   This is an example Makefile, it has to be adapted for 
#   every module. This is usually done by editing the 
#   following:
#     * variables OBJS, TESTOBJS and TESTPRG
#     * the dependencies at the end
#     * every occurence of EXLIB
#     * the toplevel Makefile and Makefile.inc
#

######################### Definitions ############################

# standard include with general options
include $(RMANBASE)/Makefile.inc

MISCCLEAN := core

########################### Targets ##############################

# general rules
include $(RMANBASE)/Makefile.rel

# automatically created dependencies
include Makefile.dep
