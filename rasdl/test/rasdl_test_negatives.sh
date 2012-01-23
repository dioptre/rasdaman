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
# - deletes and recreates databases, needs base DBMS up and running
# - FIXME:  under work
# - bash -x serves to log command lines
##################################################################

# program to be tested
RASDL=rasdl

# databases to be used (will be destroyed!)
DB1=TESTDB1

$RASDL --database $DB1 --deldatabase > /dev/null
$RASDL --database $DB1 --createdatabase > /dev/null

#Insert datatypes with negative indices to the database
$RASDL --database $DB1 --read negative_types.dl --insert > /dev/null

#Print the inserted datatypes
RES=`$RASDL --database $DB1 -p | grep typedef`
ORIG=`cat negative_types_result.orig`

if [ "$RES" != "$ORIG" ]; then
    echo "Error: Deviation found on inserted types with negative ranges"
    $RASDL --database $DB1 --deldatabase > /dev/null
    exit
fi

$RASDL --database $DB1 --deldatabase > /dev/null
