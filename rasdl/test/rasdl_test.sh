#!/bin/bash -x
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
# rasdl_test.sh: test rasdl
# 
# COMMENTS: 
# - deletes and recreates databases, needs base DBMS up and running
# - FIXME:  under work
# - bash -x serves to log command lines
# - has local copy of basictypes.dl; keep consistent!
##################################################################

# program to be tested
RASDL=../rasdl

# databases to be used (will be destroyed!)
DB1=TESTDB1
DB2=TESTDB2

echo "$0: start rasdl test."

# help
$RASDL -h
$RASDL --help

# unknown parameter
$RASDL --unknown
$RASDL -y

# inconsistent parameters and options
$RASDL -i
$RASDL --hh headerfile
$RASDL 

# --- from here on we need database connection
# clean starting point, result is of no interest
$RASDL --database $DB1 --deldatabase
$RASDL --database $DB2 --deldatabase

# --- nonex. db

#delete nonexisting database
$RASDL --database $DB2 --deldatabase

# fill into nonexisting database
$RASDL --database $DB2 --read basictypes.dl --insert

# --- good cases

# create db, regular
$RASDL --database $DB1 --createdatabase

# create existing database
$RASDL --database $DB1 --createdatabase

# fill into existing database
$RASDL --database $DB1 --read basictypes.dl --insert

# print types
$RASDL --database $DB1 --print

# generate hh file
rm basictypes.hh
$RASDL --database $DB1 --hh basictypes.hh
# for the diff lateron:
cat basictypes.hh

# delete base type
$RASDL --database $DB1 --delbasetype char
$RASDL --database $DB1 --print

# delete MDD type
$RASDL --database $DB1 --delmddtype GreyImage
$RASDL --database $DB1 --print

# delete set type
$RASDL --database $DB1 --delsettype GreySet
$RASDL --database $DB1 --print

# delete database (cleanup)
$RASDL --database $DB1 --deldatabase

echo "$0: rasdl test done."
