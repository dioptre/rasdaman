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

#Ensure psql is installed
if hash psql 2>&-; then

    #Check if the test database is empty
    COUNT=`psql -c "\dt" $DB1 | wc -l`

    #Try deleting database if it's not empty, failure will also fail the test
    if [ "$COUNT" -gt "1" ]; then
	$RASDL --database $DB1 --deldatabase
	echo "Warning: A previous instance of the database $DB1 was found. The delete database test might not be reliable. Please consider running the test again with an empty instance of $DB1"
    fi 
    
    #Create and delete the testing database
    $RASDL --database $DB1 --createdatabase
    $RASDL --database $DB1 --deldatabase

    #Check if test database is empty
    COUNT=`psql -c "\dt" $DB1 | wc -l`

    #Output the results
    if [ "$COUNT" -gt "1" ]; then
	echo "Error: Delete database test failed. Unable to delete database"	
    else
	echo "Success: Delete database test completed"
    fi

else
    
    echo "Warning: psql is required to test delete database operation. Test will not be executed."
fi

echo $0: done.