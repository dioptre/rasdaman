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
#	Command-line utility for testing rasdaman.
#	1)creating collection 
#	2)insert MDD into TEST_COLLECTION
#	3)update the MDD 
#	4)delete MDD 
#	5)drop TEST_COLLECTION
#
# PRECONDITIONS
# 	1)Postgres Server must be running
# 	2)Rasdaman Server must be running
# 	3)database RASBASE must exists
# 	4)rasql utility must be fully running	
# Usage: ./test.sh 
#        
# CHANGE HISTORY
#       2009-Sep-16     J.Yu       created
#       2010-July-04    J.Yu       add precondition 


# Variables
PROGNAME=`basename $0`
DIR_NAME=$(dirname $0)
LOG_DIR=$DIR_NAME  
LOG=$LOG_DIR/log
OLDLOG=$LOG.old
USERNAME=rasguest	
PASSWORD=rasguest
DATABASE=RASBASE
RASQL="rasql --quiet"
RASDL="rasdl"
TEST_COLLECTION="test_tmp"

  CODE_OK=0
  CODE_FAIL=255

# NUM_TOTAL: the number of manipulations
# NUM_FAIL: the number of fail manipulations
# NUM_SUC: the number of success manipulations
  NUM_TOTAL=0
  NUM_FAIL=0
  NUM_SUC=0 
  
#--------------- check if old logfile exists ----------------------------------
if [ -f $LOG ]
then
	echo Old logfile found, copying it to $OLDLOG
	mv $LOG $OLDLOG
fi

echo "Test by:"$PROGNAME" at "`date`|tee $LOG

#---------------------------Precondition------------------------------------------
# check the Postgres
ps -e | grep --quiet postmaster
if [ $? -ne 0 ]
then
   echo no postmaster available|tee -a $LOG
   exit $CODE_FAIL 
fi

# check the Rasdaman
ps -e | grep --quiet rasmgr
if [ $? -ne 0 ]
then
   echo no rasmgr available|tee -a $LOG 
   exit $CODE_FAIL
fi

# check usr
#
# check data collection
$RASQL -q "select r from RAS_COLLECTIONNAMES as r"
if [ $? -ne 0 ]
then
   echo no data collection available|tee -a $LOG 
   exit $CODE_FAIL
fi

# check data type
$RASDL --print|grep --quiet GreySet
if [ $? -ne 0 ]
then
   echo no GreSet type available|tee -a $LOG 
   exit $CODE_FAIL
fi

# check data set

#--------------------------initiation--------------------------------------------
# drop collection if they already exists

if   $RASQL -q "select r from RAS_COLLECTIONNAMES as r" --out string|grep -w $TEST_COLLECTION 
then
	echo dropping collection ... | tee -a $LOG
	$RASQL -q "drop collection $TEST_COLLECTION" --user $USERNAME --passwd $PASSWORD | tee -a $LOG
fi
################## starting manipulation test #######################
echo starting manipulation test...  | tee -a $LOG
echo creating collection $TEST_COLLECTION... | tee -a $LOG
$RASQL -q "create collection $TEST_COLLECTION  GreySet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection  $TEST_COLLECTION| tee -a $LOG
if $RASQL -q "select r from RAS_COLLECTIONNAMES as r" --out string|grep -w $TEST_COLLECTION
then
	echo create collection $TEST_COLLECTION  GreySet successfully ... | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
else
	echo create collection $TEST_COLLECTION  GreySet unsuccessfully ... | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
fi

echo inserting MDD into collection ... | tee -a $LOG
if $RASQL -q "insert into $TEST_COLLECTION  values marray x in [0:255, 0:210] values 1c" --user $USERNAME --passwd $PASSWORD 
then
	echo insert MDD into collection $TEST_COLLECTION  successfully ... | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
else
	echo insert MDD into collection $TEST_COLLECTION  unsuccessfully ... | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
fi

echo updating MDD from collection ... | tee -a $LOG
if $RASQL -q "update $TEST_COLLECTION as a set a assign a[0:179,0:54] + 1c" --user $USERNAME --passwd $PASSWORD  
then
	echo update MDD from collection $TEST_COLLECTION  successfully ... | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
else
	echo update MDD from collection $TEST_COLLECTION  unsuccessfully ... | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
fi


echo deleting MDD from collection ... | tee -a $LOG
if $RASQL -q "delete from $TEST_COLLECTION as a where all_cells(a>0)" --user $USERNAME --passwd $PASSWORD  
then
	echo delete MDD from collection $TEST_COLLECTION  successfully ... | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
else
	echo delete MDD from collection $TEST_COLLECTION  unsuccessfully ... | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
fi

echo dropping collection $TEST_COLLECTION... | tee -a $LOG
if $RASQL -q "drop collection $TEST_COLLECTION" --user $USERNAME --passwd $PASSWORD
then
	echo drop collection $TEST_COLLECTION successfully ... | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
else
	echo drop collection $TEST_COLLECTION unsuccessfully ... | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
fi

NUM_TOTAL=$(($NUM_SUC + $NUM_FAIL))
# Print the summary
  echo "test done at "`date`|tee -a $LOG
  echo "Total manipulations: "$NUM_TOTAL|tee -a $LOG
  echo "Successful manipulation number: "$NUM_SUC|tee -a $LOG
  echo "Failed manipulation number: "$NUM_FAIL|tee -a $LOG
  echo "Detail test log is in " $LOG 


  if [ $NUM_TOTAL = $NUM_SUC ]
  then
  	exit $CODE_OK 0
  else
	exit $CODE_FAIL 255
  fi
