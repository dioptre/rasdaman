#!/bin/bash
#!/bin/ksh
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
# SYNOPSIS
#	test.sh
# Description
#       Opens multiple queries to a rasdaman server through rasj. This thest was created as the result of the resolution of ticket #133
# PRECONDITIONS
# 	Postgres, Rasdaman installed, Java
#
# Usage: ./test.sh 
#         images needed for testing shall be put in directory of testdata	  	
# Parameters: 
# 
#
# CHANGE HISTORY
#       2012-Mar-18     Ernesto Rodriguez       created
#


JAVA=$JAVA_HOME/bin/java
DIR=$(dirname $0)
LOG=$DIR/log
BINDIR=$DIR/bin
echo `cd $DIR;make clean;make`
JUNIT=`find $RMANBASE/applications/petascope/lib -name 'junit-*.jar'`
CP=$BINDIR:$RMANBASE/java/bin:$JUNIT
JAVAOPTS=`echo -cp $CP`

$JAVA $JAVAOPTS org.junit.runner.JUnitCore SimultaneousConnectionsTest > $LOG

