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
# - incomplete!
# - not part of systemtest: TestHttp.java
# - test env settings to be taken from next higher Makefile when ready
# - move .log manually to .log.orig if you want to set this as new reference output
##################################################################
######################### Definitions ############################

# standard include with general options
include $(RMANBASE)/Makefile.inc

SRCS = TestRasPoint.java TestRasInterval.java TestMArray.java \
	TestException.java TestDbTa.java TestOdmg.java TestQuery.java \
	MassLoadTest.java

OBJS = ${SRCS:%.java=%.class}

# test programs (java needs them without .class suffix)
TESTS = TestRasPoint TestRasInterval TestDbTa TestException
# TESTS = ${OBJS:%.class=%}

MISCCLEAN = *.class *.log

# FIXME: should make use of general variable
JAVA = java
# Java options needed for testing
JAVAOPTS = -cp .:$(CLASSPATH)

# directory where HTML documentation is created
DOCDIR := $(DOCBASE)/java/rasj/test

########################### Targets ##############################

# compile everything
.PHONY : all
all: $(OBJS)

# run the test for all classes, compare log generated with archived log, abort on first error
.PHONY: systemtest
systemtest: $(TESTS)

# later this should be of that structure:
#	 $(JAVA) $(JAVAOPTS) $@ > $@.log && diff $@.log.orig $@.log

TestRasPoint: TestRasPoint.class
	$(JAVA) $(JAVAOPTS) $@ 

TestRasInterval: TestRasInterval.class
	$(JAVA) $(JAVAOPTS) $@

TestMArray: TestMArray.class
	$(JAVA) $(JAVAOPTS) $@ --server NONE_YET --database RASBASE --collname NONEXISTENT

MassLoadTest: MassLoadTest.class
	$(JAVA) $(JAVAOPTS) $@ --server $(SERVER) --port $(PORT) --database $(DATABASE) --user $(USER) --passwd $(PASSWD)

TestException: TestException.class
	$(JAVA) $(JAVAOPTS) $@ --server $(SERVER) --port $(PORT) --database $(DATABASE) --user $(USER) --passwd $(PASSWD)

TestDbTa: TestDbTa.class
	$(JAVA) $(JAVAOPTS) $@ --server $(SERVER) --port $(PORT) --database $(DATABASE) --user $(USER) --passwd $(PASSWD)

TestOdmg: TestOdmg.class
	$(JAVA) $(JAVAOPTS) $@ --server $(SERVER) --port $(PORT) --database $(DATABASE) --user $(USER) --passwd $(PASSWD)

TestQuery: TestQuery.class
	$(JAVA) $(JAVAOPTS) $@

# not part of systemtest: TestHttp.java

# create javadoc HTML doc:
.PHONY: docu
docu:	$(SRCS)
	-mkdir -p $(DOCDIR_RASJTEST)
	$(JAVADOC) -private -author -d $(DOCDIR_RASJTEST) -sourcepath . *.java

# remove all that can be generated
clean:
	-rm -f $(MISCCLEAN)
	-rm -rf $(DOCDIR)

# delete all files
empty:
	-rm -f $(SRCS) $(MISCCLEAN)
        
############################ Dependencies #######################

# general rules
include $(RMANBASE)/Makefile.rel
