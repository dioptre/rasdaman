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
#	1) Send rasql query
#	2) Get response
#	3) Compare the response with the expected result
#	4) Give out the testing result.
#
# PRECONDITIONS
# 	Postgres, Rasdaman installed
#
# Usage: ./test.sh
#         images needed for testing shall be put in directory of testdata
# Parameters:
#
#
# CHANGE HISTORY
#       2009-Sep-16     J.Yu       created
#       2010-Apr-13     J.Yu       revise on input folder structure to support different queries input, including folders on mandatory, bug fixed, bug unfixed, and other queries.
#
# Parameters definistion and initiation
  PROGNAME=`basename $0`
  USERNAME=rasadmin
  PASSWORD=rasadmin
  DATABASE=RASBASE
# RMANBASE: change RMANBASE depending on your system, where rasdaman is installed
# RASQL="rasql --quiet" no ornament msg
# QUERY_DIR: the test input folder, including queries for extracting some aspects of tested data
# QUERY_DIR_T: the test input folder, including queries for updating some aspects of tested data
# TESTDATA_DIR: the images needed
# ORACLE_DIR: the expected result
  RMANBASE=$RMANHOME
  RASQL="rasql"
  RASDL="rasdl"
  DIR_NAME=$(dirname $0)
  QUERY_DIR=$DIR_NAME/test_rasql
  TESTDATA_DIR=$DIR_NAME/testdata
  ORACLE_DIR=$DIR_NAME/oracle
  LOG_DIR=$DIR_NAME
  LOG=$LOG_DIR/log
FAILED=$LOG_DIR/failed_cases
  TEST_GREY=test_grey
  TEST_GREY2=test_grey2
  TEST_RGB2=test_rgb2
  QUERY=""
# LOG_DIR: the query output
# REPOET: test report
# NUM_TOTAL: number of the test query
# NUM_FAIL: number of fail test
# NUM_SUC: number of success test
# Q_ID: query identifier
  NUM_TOTAL=0
  NUM_FAIL=0
  NUM_SUC=0
  Q_ID=""
  CODE_OK=0
  CODE_FAIL=255

  echo   "Test by:"$PROGNAME $QUERY_DIR $ORACLE_DIR $LOG_DIR" at "`date`|tee $LOG
#---------------------------Precondition------------------------------------------
# check the Postgres
ps -e | grep --quiet -w postgres
if [ $? -ne 0 ]
then
   echo no postgres available|tee -a $LOG
   exit $CODE_FAIL
fi

# check the Rasdaman
ps -e | grep --quiet -w rasmgr
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
   echo no GreySet type available, try create_db.sh|tee -a $LOG
   exit $CODE_FAIL
fi

$RASDL --print|grep --quiet RGBSet
if [ $? -ne 0 ]
then
   echo no RGBSet type available, try create_db.sh|tee -a $LOG
   exit $CODE_FAIL
fi
#--------------------------initiation--------------------------------------------
# drop test collection if they already exists

if   $RASQL -q "select r from RAS_COLLECTIONNAMES as r" --out string|grep -w $TEST_GREY
then
	echo dropping collection ... | tee -a $LOG
	$RASQL -q 'drop collection '$TEST_GREY'' --user $USERNAME --passwd $PASSWORD | tee -a $LOG
fi

if   $RASQL -q "select r from RAS_COLLECTIONNAMES as r" --out string|grep -w $TEST_GREY2
then
	echo dropping collection ... | tee -a $LOG
	$RASQL -q 'drop collection '$TEST_GREY2'' --user $USERNAME --passwd $PASSWORD | tee -a $LOG
fi

if   $RASQL -q "select r from RAS_COLLECTIONNAMES as r" --out string|grep -w $TEST_RGB2
then
	echo dropping collection ... | tee -a $LOG
	$RASQL -q 'drop collection '$TEST_RGB2'' --user $USERNAME --passwd $PASSWORD | tee -a $LOG
fi

echo test data initiation...  | tee -a $LOG
echo creating test collection $TEST_GREY... | tee -a $LOG
$RASQL -q 'create collection '$TEST_GREY' GreySet' --user $USERNAME --passwd $PASSWORD || echo Error creating collection  $TEST_GREY| tee -a $LOG
if $RASQL -q "select r from RAS_COLLECTIONNAMES as r" --out string|grep -w $TEST_GREY
then
	echo create collection $TEST_GREY  GreySet successfully ... | tee -a $LOG
else
	echo create collection $TEST_GREY  GreySet unsuccessfully ... | tee -a $LOG
fi

echo inserting MDD into collection $TEST_GREY... | tee -a $LOG
if [ ! -f $TESTDATA_DIR/mr_1.png ]
then
	echo "there is no test data in "$TESTDATA_DIR
	exit $CODE_FAIL
else
	if $RASQL -q 'insert into test_grey  values inv_png($1)' --file $TESTDATA_DIR/mr_1.png --user $USERNAME --passwd $PASSWORD
		then
			echo insert test data into collection $TEST_GREY  successfully ... | tee -a $LOG
		else
			echo insert test data into collection $TEST_GREY  unsuccessfully ... | tee -a $LOG
			exit $CODE_FAIL
	fi
fi



echo creating test collection $TEST_GREY2... | tee -a $LOG
$RASQL -q 'create collection '$TEST_GREY2' GreySet' --user $USERNAME --passwd $PASSWORD || echo Error creating collection  $TEST_GREY2| tee -a $LOG
if $RASQL -q "select r from RAS_COLLECTIONNAMES as r" --out string|grep -w $TEST_GREY2
then
	echo create collection $TEST_GREY2  GreySet successfully ... | tee -a $LOG
else
	echo create collection $TEST_GREY2  GreySet unsuccessfully ... | tee -a $LOG
fi

echo inserting MDD into collection $TEST_GREY2... | tee -a $LOG
if [ ! -f $TESTDATA_DIR/mr2_1.png ]
then
	echo "there is no test data in "$TESTDATA_DIR
	exit $CODE_FAIL
else
	if $RASQL -q 'insert into '$TEST_GREY2'  values inv_png($1)'  -f $TESTDATA_DIR/mr2_1.png --user $USERNAME --passwd $PASSWORD
		then
			echo insert test data into collection $TEST_GREY2  successfully ... | tee -a $LOG
		else
			echo insert test data into collection $TEST_GREY2  unsuccessfully ... | tee -a $LOG
			exit $CODE_FAIL
	fi
fi


echo creating test collection $TEST_RGB2... | tee -a $LOG
$RASQL -q 'create collection '$TEST_RGB2' RGBSet' --user $USERNAME --passwd $PASSWORD || echo Error creating collection $TEST_RGB2| tee -a $LOG
if $RASQL -q "select r from RAS_COLLECTIONNAMES as r" --out string|grep -w $TEST_RGB2
then
	echo create collection $TEST_RGB2  RGBSet successfully ... | tee -a $LOG
else
	echo create collection $TEST_RGB2  RGBSet unsuccessfully ... | tee -a $LOG
fi

echo inserting MDD into collection $TEST_RGB2... | tee -a $LOG
if [ ! -f $TESTDATA_DIR/rgb.png ]
then
	echo "there is no test data in "$TESTDATA_DIR
	exit $CODE_FAIL
else
	if $RASQL -q 'insert into '$TEST_RGB2'  values inv_png($1)'  --file $TESTDATA_DIR/rgb.png --user $USERNAME --passwd $PASSWORD
		then
			echo insert test data into collection $TEST_RGB2  successfully ... | tee -a $LOG
		else
			echo insert test data into collection $TEST_RGB2 unsuccessfully ... | tee -a $LOG
			exit $CODE_FAIL
	fi
fi
#==========================test by queries==================================================
  if [ ! -d $QUERY_DIR ]
  then
	echo "there is no rasql for test in the "$QUERY_DIR
  else
       # initialation
  	rm tmp.unknown $FAILED
	# Query by query for extracting some aspects of tested data
  	for i in $QUERY_DIR/*.rasql
  	do
	# Send query in query folder.
        	Q_ID=`basename $i`
		echo -e "\n test query in " $Q_ID|tee -a $LOG
		QUERY=`cat $i`
  		$RASQL -q  "$QUERY" --out file --outfile tmp|tee -a $LOG

		# if the result is a scalar, there will be no tmp file by rasql, here we output the Result element scalar into tmp.unknown
		if [ ! -f tmp.unknown ]
		then
			$RASQL -q  "$QUERY" --out string|grep Result > tmp.unknown
		fi
 		mv tmp.unknown $Q_ID

		# Compare the result byte by byte with the expected result in orale folder
        	cmp $ORACLE_DIR/$Q_ID $Q_ID
    		if [ $? != 0 ]
    		then
			echo -e "\n Result error for the query."|tee -a $LOG
        		NUM_FAIL=$(($NUM_FAIL + 1))
            echo "------------------------------------------------------" | tee -a $FAILED
            echo $Q_ID| tee -a $FAILED
            echo $QUERY | tee -a $FAILED
     		else
			echo -e "\n Result correct for the query."|tee -a $LOG
			NUM_SUC=$(($NUM_SUC + 1))
    		fi
   		rm $Q_ID
  	done
  fi

  NUM_TOTAL=$(($NUM_SUC + $NUM_FAIL))

# drop collection after test

if   $RASQL -q "select r from RAS_COLLECTIONNAMES as r" --out string|grep -w $TEST_GREY
then
	echo dropping collection ... | tee -a $LOG
	$RASQL -q 'drop collection '$TEST_GREY'' --user $USERNAME --passwd $PASSWORD | tee -a $LOG
fi

if   $RASQL -q "select r from RAS_COLLECTIONNAMES as r" --out string|grep -w $TEST_GREY2
then
	echo dropping collection ... | tee -a $LOG
	$RASQL -q 'drop collection '$TEST_GREY2'' --user $USERNAME --passwd $PASSWORD | tee -a $LOG
fi

if   $RASQL -q "select r from RAS_COLLECTIONNAMES as r" --out string|grep -w $TEST_RGB2
then
	echo dropping collection ... | tee -a $LOG
	$RASQL -q 'drop collection '$TEST_RGB2'' --user $USERNAME --passwd $PASSWORD | tee -a $LOG
fi

# Print the summary
  echo "test done at "`date`|tee -a $LOG
  echo "Total query number: "$NUM_TOTAL|tee -a $LOG
  echo "Successful query number: "$NUM_SUC|tee -a $LOG
  echo "Failed query number: "$NUM_FAIL|tee -a $LOG
  echo "Detail test report is in "$LOG

  if [ $NUM_TOTAL = $NUM_SUC ]
  then
  	exit $CODE_OK
  else
	exit $CODE_FAIL
  fi
