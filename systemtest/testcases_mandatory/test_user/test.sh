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
# test_user - test rasdaman server authentication
#
# SYNOPSIS:
#	test_user.sh
#
# DESCRIPTION
#	Performs rasql test queries to check whether authentication is observed.
#
# RESPONDING TO INCIDENT
#	-none-
#
# PROCEDURE
#	Perform rasql calls performing different operations types requiring 
#	different authentication. check whether operations are recejcted/accepted
#	properly.
#
# PRECONDITIONS
# - rasql utility available
# - rasdaman up and running, with database having user/password as defined below
#
# RETURN CODES
        RC_OK=0         # everything went fine
        RC_ERROR=1      # something went wrong
#
# CHANGE HISTORY
#       2006-jan-02     P.Baumann       created
#
# RESTRICTIONS
#	-/-
#


# --- CONSTANTS -----------------------------------------------------

RCTEXT_OK="OK"
RCTEXT_ERROR="NOT_OK"

ERROR=ERROR
INDENT="+++"

# name of script
PROG=`basename $0`
PROGBASE=`basename $0 .sh`

# log file output
DIR_NAME=$(dirname $0)
LOG_DIR=$DIR_NAME  
LOG=$LOG_DIR/log

# reference log file
REFLOG=$LOG.reference
# save old log here if it exists
SAVELOG=$LOG.save

# --- TEST SETTINGS -------------------------------------------------

# r/o login
USER_RO=rasguest
PASSWD_RO=rasguest

# r/w login
USER_RW=rasadmin
PASSWD_RW=rasadmin

# nonex login
USER_NONEX=nonex
PASSWD_NONEX=nonex

# test collection
TESTCOLL=AuthentTestCollection
TESTCOLL_TYPE=GreySet

# --- ACTION --------------------------------------------------------

# save old log if present
if [ -f $LOG ]
then
        echo found old log file, shifting it to $SAVELOG
        mv $LOG $SAVELOG
fi

echo $PROG: testing rasdaman authentication at `date` | tee $LOG

# good cases
echo $INDENT good cases | tee -a $LOG
echo $INDENT $INDENT write
( rasql --quiet -q "create collection $TESTCOLL $TESTCOLL_TYPE" --user $USER_RW --passwd $PASSWD_RW \
        || (export RC=$?; echo Fatal $ERROR, exit code $RC) ) | tee -a $LOG
( rasql --quiet -q "insert into $TESTCOLL values marray x in [1:10,1:10] values (char) x[0]" --user $USER_RW --passwd $PASSWD_RW \
        || (export RC=$?; echo Fatal $ERROR, exit code $RC) ) | tee -a $LOG
( rasql --quiet -q "update $TESTCOLL as m set m[1:1,1:1] assign marray x in [1:1,1:1] values 42c" --user $USER_RW --passwd $PASSWD_RW \
        || (export RC=$?; echo Fatal $ERROR, exit code $RC) ) | tee -a $LOG
echo $INDENT $INDENT read
( rasql --quiet -q "select a[1,1] from $TESTCOLL as a" --user $USER_RW --passwd $PASSWD_RW \
        || (export RC=$?; echo Fatal $ERROR, exit code $RC) ) | tee -a $LOG
( rasql --quiet -q "select a[1,1] from $TESTCOLL as a" --user $USER_RO --passwd $PASSWD_RO \
        || (export RC=$?; echo Fatal $ERROR, exit code $RC) ) | tee -a $LOG
echo $INDENT $INDENT "write (2)"
# not yet supported by server:
# ( rasql --quiet -q "delete from $TESTCOLL" --user $USER_RW --passwd $PASSWD_RW \
( rasql --quiet -q "delete from $TESTCOLL where true" --user $USER_RW --passwd $PASSWD_RW \
        || (export RC=$?; echo Fatal $ERROR, exit code $RC) ) | tee -a $LOG
( rasql --quiet -q "drop collection $TESTCOLL" --user $USER_RW --passwd $PASSWD_RW \
        || (export RC=$?; echo Fatal $ERROR, exit code $RC) ) | tee -a $LOG
echo $INDENT $INDENT set up test env for subsequent cases
( rasql --quiet -q "create collection $TESTCOLL $TESTCOLL_TYPE" --user $USER_RW --passwd $PASSWD_RW \
        || (export RC=$?; echo Fatal $ERROR, exit code $RC) ) | tee -a $LOG
( rasql --quiet -q "insert into $TESTCOLL values marray x in [1:10,1:10] values (char) x[0]" --user $USER_RW --passwd $PASSWD_RW \
        || (export RC=$?; echo Fatal $ERROR, exit code $RC) ) | tee -a $LOG
echo $INDENT good cases done. | tee -a $LOG

# bad cases
echo $INDENT bad cases | tee -a $LOG
echo $INDENT $INDENT nonex login
( rasql --quiet -q "select a[1,1] from $TESTCOLL as a" --user $USER_NONEX --passwd $PASSWD_NONEX \
        || (export RC=$?; echo Recognized bad case, exit code $RC) ) | tee -a $LOG
echo $INDENT $INDENT write op with r/o login
( rasql --quiet -q "update $TESTCOLL as m set m[1:1,1:1] assign marray x in [1:1,1:1] values 42c" --user $USER_RO --passwd $PASSWD_RO \
        || (export RC=$?; echo Recognized bad case, exit code $RC) ) | tee -a $LOG
( rasql --quiet -q "insert into $TESTCOLL values marray x in [1:10,1:10] values (char) x[0]" --user $USER_RO --passwd $PASSWD_RO \
        || (export RC=$?; echo Recognized bad case, exit code $RC) ) | tee -a $LOG
( rasql --quiet -q "delete from $TESTCOLL where true" --user $USER_RO --passwd $PASSWD_RO \
        || (export RC=$?; echo Recognized bad case, exit code $RC) ) | tee -a $LOG
echo $INDENT $INDENT wrong passwd
( rasql --quiet -q "select a[1,1] from $TESTCOLL as a" --user $USER_RO --passwd $PASSWD_NONEX \
        || (export RC=$?; echo Recognized bad case, exit code $RC) ) | tee -a $LOG
( rasql --quiet -q "update $TESTCOLL as m set m[1:1,1:1] assign marray x in [1:1,1:1] values 42c" --user $USER_RW --passwd $PASSWD_RO \
        || (export RC=$?; echo Recognized bad case, exit code $RC) ) | tee -a $LOG
echo $INDENT bad cases done. | tee -a $LOG

echo $INDENT cleanup: 
( rasql --quiet -q "drop collection $TESTCOLL" --user $USER_RW --passwd $PASSWD_RW \
        || (export RC=$?; echo Fatal $ERROR, exit code $RC) ) | tee -a $LOG

echo $PROG: done at `date`.
exit $RC

