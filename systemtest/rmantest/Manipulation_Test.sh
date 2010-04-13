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
#	Manipulation_Test.sh
# Description
#	Command-line utility for testing rasdaman.
#	1)creating collection tmp
#	2)insert MDD into collection tmp
#	3)update the MDD in tmp
#	4)delete MDD from tmp
#	5)drop tmp
#
# PRECONDITIONS
# 	1)Postgres Server must be running
# 	2)Rasdaman Server must be running
# 	3)database RASBASE must exists
# 	4)rasql utility must be fully running	
# Usage: ./Manipulation_Test.sh 
#        
# CHANGE HISTORY
#       2009-Sep-16     J.Yu       created
#


# Variables
PROGNAME=`basename $0`
DIR_NAME=$(dirname $0)
REPORT=$DIR_NAME/Repository/$PROGNAME.report`date "+%Y%m%d"`
OLDREPORT=$REPORT.old
USERNAME=rasdaman	
PASSWORD=rasdaman
DATABASE=RASBASE
RASQL="rasql --quiet"

# NUM_TOTAL: the number of manipulations
# NUM_FAIL: the number of fail manipulations
# NUM_SUC: the number of success manipulations
  NUM_TOTAL=0
  NUM_FAIL=0
  NUM_SUC=0 



# --------------- check if old logfile exists ----------------------------------
if [ -f $REPORT ]; then
	echo Old logfile found, copying it to $OLDREPORT
	mv $REPORT $OLDREPORT
fi


# drop collection if they already exists

echo  "Test by:"$PROGNAME" at "`date`|tee $REPORT

$RASQL -q "select r from RAS_COLLECTIONNAMES as r"|grep tmp
if  [ $? = "0" ]
then
	echo dropping collection ... | tee -a $REPORT
	$RASQL -q "drop collection tmp" --user $USERNAME --passwd $PASSWORD | tee -a $REPORT
fi
################## starting manipulation test #######################
echo starting manipulation test...  | tee -a $REPORT
echo creating collection tmp... | tee -a $REPORT
$RASQL -q "create collection tmp  GreySet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection  tmp| tee -a $REPORT
$RASQL -q "select r from RAS_COLLECTIONNAMES as r"|grep tmp
if  [ $? != "0" ]
then
	echo create collection tmp  GreySet successfully ... | tee -a $REPORT
	NUM_SUC=$(($NUM_SUC + 1))
else
	echo create collection tmp  GreySet unsuccessfully ... | tee -a $REPORT
	NUM_FAIL=$(($NUM_FAIL + 1))
fi

echo inserting MDD into collection ... | tee -a $REPORT
$RASQL -q 'insert into tmp  values marray x in [0:255, 0:210] values 1c' --user $USERNAME --passwd $PASSWORD || echo Error inserting | tee -a $REPORT
$RASQL -q "select all_cells(tmp=1) from tmp" --out string|grep "Result element 1: t" 

if  [ $? != "0" ]
then
	echo insert MDD into collection tmp  successfully ... | tee -a $REPORT
	NUM_SUC=$(($NUM_SUC + 1))
else
	echo insert MDD into collection tmp  unsuccessfully ... | tee -a $REPORT
	NUM_FAIL=$(($NUM_FAIL + 1))
fi


echo updating MDD from collection ... | tee -a $REPORT
$RASQL -q 'update tmp as a set a assign a[0:179,0:54] + 1c' --user $USERNAME --passwd $PASSWORD || echo Error updating| tee -a $REPORT
$RASQL -q "select all_cells(tmp[0:179,0:54]=2) from tmp" --out string|grep "Result element 1: t" 

if  [ $? != "0" ]
then
	echo update MDD from collection tmp  successfully ... | tee -a $REPORT
	NUM_SUC=$(($NUM_SUC + 1))
else
	echo update MDD from collection tmp  unsuccessfully ... | tee -a $REPORT
	NUM_FAIL=$(($NUM_FAIL + 1))
fi


echo deleting MDD from collection ... | tee -a $REPORT
$RASQL -q 'delete from tmp as a where all_cells(a>0)' --user $USERNAME --passwd $PASSWORD || echo Error deleting| tee -a $REPORT
$RASQL -q "select all_cells(tmp[0:179,0:54]=2) from tmp" --out string|grep "Result element 1: t" 

if  [ $? != "0" ]
then
	echo delete MDD from collection tmp  successfully ... | tee -a $REPORT
	NUM_SUC=$(($NUM_SUC + 1))
else
	echo delete MDD from collection tmp  unsuccessfully ... | tee -a $REPORT
	NUM_FAIL=$(($NUM_FAIL + 1))
fi

echo dropping collection tmp... | tee -a $REPORT
$RASQL -q "drop collection tmp" --user $USERNAME --passwd $PASSWORD || echo Error droping|| tee -a $REPORT
$RASQL -q "select r from RAS_COLLECTIONNAMES as r"|grep tmp

if  [ $? != "0" ]
then
	echo drop collection tmp successfully ... | tee -a $REPORT
	NUM_SUC=$(($NUM_SUC + 1))
else
	echo drop collection tmp unsuccessfully ... | tee -a $REPORT
	NUM_FAIL=$(($NUM_FAIL + 1))
fi

NUM_TOTAL=$(($NUM_SUC + $NUM_FAIL))
# Print the summary
  echo "test done at "`date`|tee -a $REPORT
  echo "Total manipulations: "$NUM_TOTAL|tee -a $REPORT
  echo "Successful manipulation number: "$NUM_SUC|tee -a $REPORT
  echo "Failed manipulation number: "$NUM_FAIL|tee -a $REPORT
  echo "Detail test report is in " $REPORT 




