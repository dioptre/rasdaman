#!/bin/bash
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
# test_errors-seq.sh - test single-client errors due to bad client behavior
#
# SYNOPSIS:
#	test_errors-seq.sh
#
# DESCRIPTION
#	Performs test queries to check proper handling of error conditions.
#	- Errors on query level are handled by rasql.
#	- Errors on protocol level are triggered via a test program.
#	Ater provoking an error, it is tested whether the server remains
#	available fur same/other clients (whatever is appropriate).
#
# RESPONDING TO INCIDENT
#	none
#
# PROCEDURE
#	perform invocations, checks response and subsequent server availbility.
#
# PRECONDITIONS
# - rasql utility available
# - rasdaman up and running, with database having user/password as defined below
#
# RETURN CODES
        RC_OK=0         # everything went fine
        RC_ERROR=1      # something went wrong
#
RCTEXT_OK="OK"
RCTEXT_ERROR="NOT_OK"

# --- CONSTANTS -----------------------------------------------------

# name of script
PROG=`basename $0`

# name of test program
TESTPROG=`basename $0 .sh`

# log output
LOGFILE=/tmp/`basename $PROG .sh`.log

# old log file for regression comparison:
OLDFILE=`basename $PROG .sh`.old

# how to react on error
function raiseError() { echo "$PROG: fatal error, aborting."; exit $RC_ERROR; }
#function raiseError() { echo "$PROG: error in test; resuming."; RC=$RC_ERROR; }

# --- ACTION --------------------------------------------------------

echo $PROG: testing client/server communication for open/close db/ta
echo $PROG: testing client/server communication for open/close db/ta, `date` >$LOGFILE

# initialize overall return code
RC=$RC_OK

# --- test
(cd $TESTPROG; make )
$TESTPROG/$TESTPROG 2>&1 >>$LOGFILE || raiseError

# --- compare files against old ones
if [ `cmp -s $OLDFILE $LOGFILE` ]
then
	echo "Error: regression discrepancy between files $OLDDIR/$i and $LOGDIR/$i -- $RCTEXT_ERROR"
fi

# --- cleanup and summarise
if [ $RC -eq $RC_OK ]
then
	rm -f $LOGFILE
	RCTEXT=$RCTEXT_OK
else
	RCTEXT=$RCTEXT_ERROR
fi

echo $PROG: done, result is $RCTEXT.
exit $RC

