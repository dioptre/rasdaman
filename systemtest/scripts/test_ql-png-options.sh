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
# test_ql-png-options.sh - test png(mdd,options) function of rasql
#
# SYNOPSIS:
#	test_ql-png-options.sh
#
# DESCRIPTION
#	Performs test queries to check whether transparency and background
#	option works in function png(_,_).
#Write log to stdout/stderr for ???
#	Does not test general image encoding!
#
# RESPONDING TO INCIDENT
#	Incident_2005-07-12_IGN_png-transparency
#
# PROCEDURE
# - create test collections and objects
# - perform rasql calls, check output and, where applicable, the image generated
#   check that (i) file is generated, (ii) has transparency color set appropriately
# - remove output files and test collections
#
# PRECONDITIONS
# - rasql binary available in the $RMANBASE/applications/rasql development directory
# - rasdaman up and running,
# - database allows write access using user/password as defined below
# - ImageMagick installed to have 'identify' utility
# - can create subdir in cwd
#
# RETURN CODES
        RC_OK=0         # everything went fine
        RC_ERROR=1      # something went wrong
#
# CHANGE HISTORY
#       2005-jul-26     P.Baumann       created
#       2005-aug-25     P.Baumann       use raiseError() to centralize bailout behavior
#
# COMMENTS
# - add test for INV_PNG()
#


# --- CONSTANTS -----------------------------------------------------

# textual representation of error conditions
RCTEXT_OK="OK"
RCTEXT_ERROR="NOT_OK"

# name of script
PROG=`basename $0`

# reference data directory
OLDDIR=./`basename $PROG .sh`.old

# temp dir for image files
TMPDIR=./`basename $PROG .sh`.test

# user/password (must have r/w rights!)
USER=rasadmin
PASSWD=rasadmin

# test collections for monochrome/gray/color images
TESTCOLL_MONO=TestPngMono
TESTCOLL_GRAY=TestPngGray
TESTCOLL_COLOR=TestPngColor

# the rasql utility, suppressing unnecessarily verbose output
RMANBASE=~rasdev/Compile/rasdaman
RASQL="$RMANBASE/applications/rasql/rasql --quiet"

# to get error messages (from $RMANHOME/bin/errtxts):
RMANHOME=$RMANBASE

# how to react on error
function raiseError() { echo "$PROG: fatal error, aborting."; exit $RC_ERROR; }
#function raiseError() { echo "$PROG: error in test; resuming."; RC=$RC_ERROR; }

# --- ACTION --------------------------------------------------------

echo "$PROG: test rasql PNG() function"

# --- preparation

echo "$PROG: create test dir"
mkdir -p $TMPDIR

# initialize overall return code
RC=$RC_OK

# --- cleanup to prepare
# delete test collection & image & temp data
$RASQL -q "drop collection $TESTCOLL_MONO" --user $USER --passwd $PASSWD
$RASQL -q "drop collection $TESTCOLL_GRAY" --user $USER --passwd $PASSWD
$RASQL -q "drop collection $TESTCOLL_COLOR" --user $USER --passwd $PASSWD

# create test collections
# - mono
$RASQL -q "create collection $TESTCOLL_MONO BoolSet"  --user $USER --passwd $PASSWD || raiseError
# - gray
$RASQL -q "create collection $TESTCOLL_GRAY GreySet"  --user $USER --passwd $PASSWD || raiseError
# - color
$RASQL -q "create collection $TESTCOLL_COLOR RGBSet"  --user $USER --passwd $PASSWD || raiseError

# create test objects
# - mono
$RASQL -q "insert into $TESTCOLL_MONO values marray x in [0:3,0:3] values (x[0]+x[1]) = 1" --user $USER --passwd $PASSWD || raiseError
# - gray
$RASQL -q "insert into $TESTCOLL_GRAY values marray x in [0:3,0:3] values (char) (x[0]*x[1])" --user $USER --passwd $PASSWD || raiseError
# - color
$RASQL -q "insert into $TESTCOLL_COLOR values marray x in [0:3,0:3] values ((char)(x[0]*x[1]))*{1c,1c,1c}" --user $USER --passwd $PASSWD || raiseError

# --- tests

echo "$PROG: test good cases pf transp / non-transp, bg colors"
echo "$PROG: --- set bg to transparent"
echo "$PROG: --- --- mono"
echo "$PROG: --- --- --- dec"
$RASQL -q "select png(a,\"tRNS=0)\" ) from $TESTCOLL_MONO as a"  --out file --outfile $TMPDIR/png-mono-trns_1 || raiseError
if [ `identify -verbose $TMPDIR/png-mono-trns_1.png | grep Opacity | grep 0 | wc -l` -eq 0 ]
then
	echo "Error: transparency not set in PNG file $TMPDIR/pngmono-trns_1.png -- $RCTEXT_ERROR"
	raiseError
fi
if [ ! `cmp -s $OLDDIR/png-mono-trns_1.png $TMPDIR/png-mono-trns_1.png` ]
then
	echo "Error: PNG file contents in file $TMPDIR/pngmono-trns_1.png does not match regression source -- $RCTEXT_ERROR"
	raiseError
fi

# ==== complete down here:
$RASQL -q "select png(a,\"tRNS=1)\" ) from $TESTCOLL_MONO as a"  --out file --outfile $TMPDIR/png-mono-trns_2 || raiseError

echo "$PROG: --- --- gray"
echo "$PROG: --- --- --- dec"
$RASQL -q "select png(a,\"tRNS=0)\" )   from $TESTCOLL_GRAY as a"  --out file --outfile $TMPDIR/png-gray-trns_1 || raiseError
$RASQL -q "select png(a,\"tRNS=2)\" )   from $TESTCOLL_GRAY as a"  --out file --outfile $TMPDIR/png-gray-trns_2 || raiseError
$RASQL -q "select png(a,\"tRNS=255)\" ) from $TESTCOLL_GRAY as a"  --out file --outfile $TMPDIR/png-gray-trns_3 || raiseError
echo "$PROG: --- --- --- oct"
$RASQL -q "select png(a,\"tRNS=00)\" )    from $TESTCOLL_GRAY as a"  --out file --outfile $TMPDIR/png-gray-trns_4 || raiseError
$RASQL -q "select png(a,\"tRNS=02)\" )    from $TESTCOLL_GRAY as a"  --out file --outfile $TMPDIR/png-gray-trns_5 || raiseError
$RASQL -q "select png(a,\"tRNS=07777)\" ) from $TESTCOLL_GRAY as a"  --out file --outfile $TMPDIR/png-gray-trns_6 || raiseError
echo "$PROG: --- --- --- hex"
$RASQL -q "select png(a,\"tRNS=0x0)\" )  from $TESTCOLL_GRAY as a"  --out file --outfile $TMPDIR/png-gray-trns_7 || raiseError
$RASQL -q "select png(a,\"tRNS=0x2)\" )  from $TESTCOLL_GRAY as a"  --out file --outfile $TMPDIR/png-gray-trns_8 || raiseError
$RASQL -q "select png(a,\"tRNS=0xff)\" ) from $TESTCOLL_GRAY as a"  --out file --outfile $TMPDIR/png-gray-trns_9 || raiseError

echo "$PROG: --- --- color"
echo "$PROG: --- --- --- dec"
$RASQL -q "select png(a,\"tRNS=(1;2;3)\" ) from $TESTCOLL_COLOR as a" --out file --outfile $TMPDIR/png-color-trns_1 || raiseError
echo "$PROG: --- --- --- hex"
$RASQL -q "select png(a,\"tRNS=(0x77;0xd0;0xf8)\" ) from rgb as a" --out file || raiseError
echo "$PROG: --- --- --- mixed"
$RASQL -q "select png(a,\"tRNS=(1;02;0x3)\" ) from $TESTCOLL_COLOR as a" --out file --outfile $TMPDIR/png-color-trns_10 || raiseError


echo "$PROG: test bad cases: syntax errors, overflow in bg colors"

echo "$PROG: --- tRNS tag wrong"
if [ `$RASQL -q "select png(a,\"XXX=(1;2;3)\" ) from $TESTCOLL_COLOR as a" 2>&1 | grep "Execution error 381" | wc -l` -ne 1 ]
then
	echo "Error: cannot sense proper error message -- $RCTEXT_ERROR"
	raiseError
fi

echo "$PROG: --- no number, gray"
if [ `$RASQL -q "select png(a,\"tRNS=zzz\" ) from $TESTCOLL_GRAY as a" 2>&1 | grep "Execution error 381" | wc -l` -ne 1 ]
then
	echo "Error: cannot sense proper error message -- $RCTEXT_ERROR"
	raiseError
fi

echo "$PROG: --- no number, color"
if [ `$RASQL -q "select png(a,\"tRNS=(q;w;s)\" ) from $TESTCOLL_GRAY as a" 2>&1 | grep "Execution error 381" | wc -l` -ne 1 ]
then
	echo "Error: cannot sense proper error message -- $RCTEXT_ERROR"
	raiseError
fi

echo "$PROG: --- color/gray mismatch"
if [ `$RASQL -q "select png(a,\"tRNS=12\" ) from $TESTCOLL_COLOR as a" 2>&1 | grep "Execution error 381" | wc -l` -ne 1 ]
then
	echo "Error: cannot sense proper error message -- $RCTEXT_ERROR"
	raiseError
fi

if [ `$RASQL -q "select png(a,\"tRNS=(1;2;3)\" ) from $TESTCOLL_GRAY as a" 2>&1 | grep "Execution error 381" | wc -l` -ne 1 ]
then
	echo "Error: cannot sense proper error message -- $RCTEXT_ERROR"
	raiseError
fi

echo "$PROG: --- paren error (not recognized currently)"
if [ `$RASQL -q "select png(a,\"tRNS=12;13;14)\" ) from $TESTCOLL_GRAY as a" 2>&1 | grep "Execution error 381" | wc -l` -ne 1 ]
then
	echo "Error: cannot sense proper error message -- $RCTEXT_ERROR"
	raiseError
fi

echo "$PROG: --- number overflow"
if [ `$RASQL -q "select png(a,\"tRNS=1000000000;2000000000;30000000000000)\" ) from $TESTCOLL_COLOR as a" 2>&1 | grep "Execution error 381" | wc -l` -ne 1 ]
then
	echo "Error: cannot sense proper error message -- $RCTEXT_ERROR"
	raiseError
fi


# compare files against old ones
for i in `(cd $TMPDIR; ls *.png)`
do
	if [ `cmp -s $OLDDIR/$i $TMPDIR/$i` ]
	then
		echo "Error: regression discrepancy between files $OLDDIR/$i and $TMPDIR/$i -- $RCTEXT_ERROR"
	fi
done

# --- cleanup and summarise
# delete test collection & image & temp data
$RASQL -q "drop collection $TESTCOLL_MONO" --user $USER --passwd $PASSWD || raiseError
$RASQL -q "drop collection $TESTCOLL_GRAY" --user $USER --passwd $PASSWD || raiseError
$RASQL -q "drop collection $TESTCOLL_COLOR" --user $USER --passwd $PASSWD || raiseError

if [ $RC -eq $RC_OK ]
then
	rm -rf $TMPDIR
	RCTEXT=$RCTEXT_OK
else
	RCTEXT=$RCTEXT_ERROR
fi

echo $PROG: done, result is $RCTEXT.
exit $RC
