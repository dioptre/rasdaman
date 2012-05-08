# -*-Makefile-*-
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
#   perform conformance test
# 
# COMMENTS: 
# - not yet operational
#
##################################################################
######################### Definitions ############################

# all test programs


########################### Targets ##############################
TESTSEQUENCE = $(shell ls -d ./* \
                                           )
# general system test
.PHONY: ogc-check
ogc-check:
	for testcase in ${TESTSEQUENCE}; do\
		 PATH=${bindir}:${PATH}   $$testcase/*.sh; \
               PROGTEST=$${testcase}/*.sh;\
		 if [ $$? -ne 0 ]; \
		 then\
			echo $${PROGTEST} ... FAIL>>log;\
		 else \
		 	echo $${PROGTEST} ... OK>>log;\
		 fi; \
		 echo "	"see detail in $$testcase/log>>log;\
	done;\
	cat log;\
	rm log;
	
clean:
	for testcase in ${TESTSEQUENCE}; do \
               PATH=${bindir}:${PATH}   rm -rf $${testcase}/log; \
        done;

######################## Dependencies ############################

