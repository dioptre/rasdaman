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
if [[ $1 = "" || $2 = "" ]]; then
  echo "usage: testoid.sh <server name> <base name>"
  return
fi

SERVER=$1
BASENAME=$2

# test programs
insert="$RMANBASE/rasodmg/test/test_insert3 -server $SERVER -base $BASENAME "
query="$RMANBASE/rasodmg/test/test_query -server $SERVER -base $BASENAME "
lookup="$RMANBASE/rasodmg/test/test_lookup -server $SERVER -base $BASENAME "


testExitStatus() 
{
  if [[ ( $2 = 0 && $1 = 0 ) || ( $2 != 0 && $1 != 0 ) ]]; then
    echo "OK"
  else
    echo "FAILED"
    echo "\nProgram output:"
    cat log
    echo "\n\n"
  fi
}


#
# main
#

setOId=""
imageOId=""

echo "-- Testbed start block."
echo "Delete test set oidSet ... " ;
echo "10\n" | $insert -setname oidSet       > log ;
testExitStatus $? -1;

echo "Insert test image into oidSet ... " ;
echo "5\n   0\n 2\n 0\n 10\n 0\n 10\n" | $insert -setname oidSet -testbed > log ;
testExitStatus $? 0;
echo "-- Testbed end block."

# get oids
setOId=`grep set_oid log | sed 's/-- Testbed:.*=//g' `
imageOId=`grep image_oid log | sed 's/-- Testbed:.*=//g' `

echo "Set   OId: " $setOId
echo "Image OId: " $imageOId

echo "-- Testbed start block."
echo "Get set by oid ... "
$lookup -oid $setOId   -testbed -nooutput > log;
testExitStatus $? 0;

echo "Get image by oid ... "
$lookup -oid $imageOId -testbed -nooutput > log;
testExitStatus $? 0;

echo "Get set by name ... "
$lookup -setname oidSet  -testbed -nooutput > log;
testExitStatus $? 0;

echo "Remove image from oidSet ... " ;
echo "11\n 1\n" | $insert -setname oidSet       > log ;
testExitStatus $? 0;

echo "Try to access image by oid again ... "
$lookup -oid $imageOId -testbed -nooutput > log;
testExitStatus $? -1;

echo "Delete test set oidSet ... " ;
echo "10\n" | $insert -setname oidSet       > log ;
testExitStatus $? 0;

echo "Try to access set by oid again ... "
$lookup -oid $setOId   -testbed -nooutput > log;
testExitStatus $? -1;

echo "Try to access set by name again ... "
$lookup -setname oidSet   -testbed -nooutput > log;
testExitStatus $? -1;
echo "-- Testbed end block."

echo "\nTesting finnished."






