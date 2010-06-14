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
#   package RasJ
# 
# COMMENTS: 
# 
##################################################################
######################### Definitions ############################

# standard include with general options
include $(RMANBASE)/Makefile.inc

# directory where HTML documentation is created
DOCDIR := $(DOCBASE)/java/org

########################### Targets ##############################

SRCS = DArray.java DatabaseOpenException.java ObjectNotPersistentException.java \
	DBag.java Implementation.java QueryException.java DCollection.java \
	LockNotGrantedException.java QueryInvalidException.java \
	DList.java NotImplementedException.java QueryParameterCountInvalidException.java \
	DMap.java ODMGException.java QueryParameterTypeInvalidException.java \
	DSet.java ODMGRuntimeException.java Transaction.java Database.java \
	OQLQuery.java TransactionAbortedException.java DatabaseClosedException.java \
	ObjectDeletedException.java TransactionInProgressException.java \
	DatabaseIsReadOnlyException.java DatabaseNotFoundException.java ObjectNameNotFoundException.java \
	TransactionNotInProgressException.java
OBJS = ${SRCS:%.java=%.class}
MISCCLEAN = *.class

########################### Targets ##############################

# compile everything
.PHONY : all
all: $(OBJS)


# delete all files
empty:
	-rm -f $(SRCS) $(MISCCLEAN)
        
# general rules
include $(RMANBASE)/Makefile.rel
