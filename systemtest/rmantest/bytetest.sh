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
#	bytetest.sh
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
# Usage: ./bytetest.sh [INPUT_DIR] [ORACLE_DIR][OUT_DIR]
#        e.g. ./bytetest.sh inputFolder/mandatory Oracle/mandatory Repository/mandatory
#        e.g. ./bytetest.sh inputFolder/bug_fixed Oracle/bug_fixed Repository/bug_fixed
#        e.g. ./bytetest.sh inputFolder/bug_unfixed Oracle/bug_unfixed Repository/bug_unfixed
#        e.g. ./bytetest.sh inputFolder/other Oracle/other Repository/other
#         images needed for testing shall be put in directory of images
#	  Expected results for comparation go into Oracle, there is a tricky here on how to get expected result, run the script then copy the # #	  output in Repository(judge the correctness before copy) to Oracle	  	
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
# INPUT_DIR: the test input folder, including queries for extracting some aspects of tested data
# INPUT_DIR_T: the test input folder, including queries for updating some aspects of tested data
# IMAGE_DIR: the images needed 
# ORACLE_DIR: the expected result
  RMANBASE=$RMANHOME
  RASQL="rasql"
  DIR_NAME=$(dirname $0)
  INPUT_DIR=$DIR_NAME/inputFolder/mandatory
  INPUT_DIR_T=$DIR_NAME/inputFolder_t
  IMAGE_DIR=$DIR_NAME/images
  ORACLE_DIR=$DIR_NAME/Oracle/mandatory
  REP_DIR=$DIR_NAME/Repository/mandatory
  QUERY=""
# OUT_DIR: the query output
# REPOET: test report
# NUM_TOTAL: number of the test query
# NUM_FAIL: number of fail test
# NUM_SUC: number of success test
# Q_ID: query identifier
  NUM_TOTAL=0
  NUM_FAIL=0
  NUM_SUC=0 
  Q_ID=""
# Check for the inputFolder, Oracle and Repository
  if   [   $#   =   3   ]   
  then   
	if [ -d $1 ]
	then	
		INPUT_DIR=$1
	fi

	if [ -d $2 ]
	then	
		ORACLE_DIR=$2
	fi	
	
	if [ -d $3 ]
	then	
		REP_DIR=$3
	fi
  fi 
  OUT_DIR=$REP_DIR/outputFolder`date "+%Y%m%d"`
  REPORT=$OUT_DIR/test_report

# Check for the repository directory, if it does not exsist, then create one.
  if [ ! -d $REP_DIR ]
  then
	mkdir $REP_DIR	
  fi

# Check for the query output directory, if it does not exsist, then create one.
  if [ ! -d $OUT_DIR ]
  then
	mkdir $OUT_DIR	
  else
	if [ -d $OUT_DIR.save ]
	then
		rm -rf $OUT_DIR.save
	fi
	mv $OUT_DIR $OUT_DIR.save
       mkdir $OUT_DIR
  fi
  
  echo   "Test by:"$PROGNAME $INPUT_DIR $ORACLE_DIR $OUT_DIR" at "`date`|tee $REPORT


  if [ ! -f $INPUT_DIR/*.rasql ]
  then
	echo "there is no rasql for test in the "$INPUT_DIR
  else	
       # initialation
  	rm tmp.unknown
	# Query by query for extracting some aspects of tested data
  	for i in $INPUT_DIR/*.rasql
  	do
	# Send query in input folder, and save response to output folder.
        	Q_ID=`basename $i`
		echo -e "\n test query in " $Q_ID|tee -a $REPORT
		QUERY=`cat $i`
  		$RASQL -q  "$QUERY" --out file --outfile tmp|tee -a $REPORT

		# if the result is a scalar, there will be no tmp file by rasql, here we output the Result element scalar into tmp.unknown
		if [ ! -f tmp.unknown ]
		then
			$RASQL -q  "$QUERY" --out file --outfile tmp|grep Result > tmp.unknown
		fi
 		mv tmp.unknown $OUT_DIR/$Q_ID

		# Compare the result byte by byte with the expected result in orale folder
        	cmp $ORACLE_DIR/$Q_ID $OUT_DIR/$Q_ID
    		if [ $? != 0 ]
    		then 
			echo -e "\n Result error for the query."|tee -a $REPORT   				
        		NUM_FAIL=$(($NUM_FAIL + 1))
     		else
			echo -e "\n Result correct for the query."|tee -a $REPORT
			NUM_SUC=$(($NUM_SUC + 1)) 
    		fi
  	done
  fi
  
  
  NUM_TOTAL=$(($NUM_SUC + $NUM_FAIL))
# Print the summary
  echo "test done at "`date`|tee -a $REPORT
  echo "Total query number: "$NUM_TOTAL|tee -a $REPORT
  echo "Successful query number: "$NUM_SUC|tee -a $REPORT
  echo "Failed query number: "$NUM_FAIL|tee -a $REPORT
  echo "Detail test report is in "$REPORT 
