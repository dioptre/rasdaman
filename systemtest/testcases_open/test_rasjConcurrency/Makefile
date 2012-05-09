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
#   Rasj Concurrency Tests
# 
# COMMENTS: 
#	Junit tests for simultaneous requests to rasdaman.
##################################################################

SRCDIR=./rasjTests
BINDIR=./bin
SRCS = $(SRCDIR)/RasjQuery.java $(SRCDIR)/SimultaneousConnectionsTest.java $(SRCDIR)/SimultaneousConnectionsTestUtil.java
JUNIT=find $(RMANBASE)/applications/petascope/lib -name 'junit-*.jar'
OBJS = ${SRCS:%.java=%.class}
CP=$(BINDIR):$(RMANBASE)/java/bin:`$(JUNIT)`
JAVACOPTS = -cp $(CP) -d $(BINDIR)
JAVAOPTS = -cp $(CP)
JAVAC=$(JAVA_HOME)/bin/javac
JAVA=$(JAVA_HOME)/bin/java

.PHONY : all
all:
	mkdir $(BINDIR)
	$(JAVAC) $(JAVACOPTS) $(SRCS)

clean:
	rm -rf $(BINDIR)

systemtest:
	$(JAVA) $(JAVAOPTS) org.junit.runner.JUnitCore SimultaneousConnectionsTest