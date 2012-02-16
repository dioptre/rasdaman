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
#
# rasdl_test_negatives.sh: Test negative array ranges. 
# 
# COMMENTS: 
# - requires psql to be installed
# - deletes and recreates databases, needs base DBMS up and running
# - FIXME:  under work
# - bash -x serves to log command lines
##################################################################

# program to be tested
RASDL=../rasdl

# databases to be used (will be destroyed!)
DB1=TESTDB1
TYPES=testDelTypes.dl

if hash psql 2>&-; then

#Check if the test database is empty
    COUNT=`psql -c "\dt" $DB1 | wc -l`

    #Delete the previous database, note that it's recommended to use DB1 exclusively for testing
    if [ "$COUNT" -gt "1" ]; then
	$RASDL --database $DB1 --deldatabase
	echo "Warning: A previous instance of the database $DB1 was found. The delete mdd,set,struct types test might not be reliable. Please consider running the test again with an empty instance of $DB1"
    fi

    #Create the testing database and insert the test types
    $RASDL --database $DB1 --createdatabase
    $RASDL --database $DB1 -r $TYPES -i

    #Try deleting a type that has dependent types, deletion should fail
    $RASDL --database $DB1 --delbasetype testStruct
    COUNT=`$RASDL --database $DB1 -p | grep testStruct | wc -l`

    if [ "$COUNT" -lt "1" ]; then
	echo "Error: A base type with dependent types was deleted. This should not be permitted"
	$RASDL --database $DB1 --deldatabase > /dev/null
	exit -1
    fi 

    #Try deleting all the types in the correct order
    $RASDL --database $DB1 --delsettype testSet
    let COUNT=`$RASDL --database $DB1 -p | grep testSet | wc -l`*1
    $RASDL --database $DB1 --delmddtype testMarray
    let COUNT=COUNT+`$RASDL --database $DB1 -p | grep testMarray | wc -l`*2    
    $RASDL --database $DB1 --delbasetype testStruct
    let COUNT=$COUNT+`$RASDL --database $DB1 -p | grep -E testStruct | wc -l`*4

    case $COUNT in
	1 )
	    echo "Error: rasdl failed to delete a set type from the database.";ERROR=-1;;
	2 ) 
	    echo "Error: rasdl failed to delete a array type from the database.";ERROR=-1;;
	3 )
	    echo "Error: rasdl failed to delete array and set types from the database.";ERROR=-1;;
	4 )
	    echo "Error: rasdl failed to delete a struct type from the database.";ERROR=-1;;
	5 )
	    echo "Error: rasdl failed to delete struct and set types from the database.";ERROR=-1;;
	6 )
	    echo "Error: rasdl failed to delete struct and array types from the database";ERROR=-1;;
	7 )
	    echo "Error: rasdl failed to delete struct, array and set  types from the database";ERROR=-1;;
	0 )
	    echo "Success: All struct, array and set types were successfully deleted from database.";ERROR=0;;
	* )
	    echo "Error: The type deletion test ended unexpectedly. Please check for failures in other tests, particularly in the database deletion test that might explain this result.";ERROR=-1;;
    esac

    $RASDL --database $DB1 --deldatabase > /dev/null

    if [ "$ERROR" == 0 ]; then
	exit 0
    else
	exit -1
    fi 

else
    
    echo "Warning: psql is required to test delete database operation. Test will not be executed."
fi

echo $0: done.