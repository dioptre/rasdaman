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
# test_ql-extend.sh - test extend() function of rasql
#
# SYNOPSIS:
#	test_ql-extend.sh
#
# DESCRIPTION
#	Performs test queries to check whether extend() function works.
echo	not yet done!
exit
#
# RESPONDING TO INCIDENT
#	Incident_2005-07-12_IGN_png-transparency
#
# PROCEDURE
#	Perform rasql calls, check output and, where applicable, the image generated.
#	Output checking inspects target domain and image contents
#	(original area unchanged, new areas set to 0; contents check just by dumping
#	and relying on regression comparison)
#
# PRECONDITIONS
# - rasql utility available
# - rasdaman up and running, with database having user/password as defined below
# - ImageMagick installed to have 'identify' utility
#
# RETURN CODES
        RC_OK=0         # everything went fine
        RC_ERROR=1      # something went wrong
#
# CHANGE HISTORY
#       2005-jul-16     P.Baumann       created
#
# RESTRICTIONS
#	test only with 2D, should be extended
#	test only with RGB, should be extended to cover all pixel types
#


# --- CONSTANTS -----------------------------------------------------

RCTEXT_OK="OK"
RCTEXT_ERROR="NOT_OK"

# name of script
PROG=`basename $0`

# temp file for getcap response:
TMPFILE=/tmp/`basename $PROG .sh`.tmp

# --- TEST SETTINGS -------------------------------------------------

# login (must allow r/w)
USER=rasadmin
PASSWD=rasadmin

# test collection
TESTCOLL=ExtendTestCollection

# test image boxes
BOX_ALLPOS=[10:20,30:40]
BOX_ALLNEG=[-20:-10,-40:-30]
BOX_MIXED=[-10:20,-30:40]

# extend boxes, good cases: top right
EXTEND_GOOD_TOPRIGHT_ALLPOS=[10:20,30:40] ??
EXTEND_GOOD_TOPRIGHT_ALLNEG=[-20:-10,-40:-30] ??
EXTEND_GOOD_TOPRIGHT_MIXED=[-10:20,-30:40] ??

...

# bad test cases: extend area inside image
EXTED_BAD_INSIDE_ALLPOS=[11:19,31:39]
EXTED_BAD_INSIDE_ALLNEG=[-19:-11,-39:-31]
EXTED_BAD_INSIDE_MIXED=[-11:19,-29:39]

# --- ACTION --------------------------------------------------------

echo $PROG: Verify rasql extend function

# delete eventually preexisting test collection
# create test collection
# create white test images
# - all corner points in positive quadrant
# - corner points in all quadrants
# - all corner points in negative quadrant
# extend each test image, good cases
# - extend top right (cross origin for neg locations!)
# - extend bottom left (cross origin for pos locations!)
# - extend in all directions
# extend each test image, bad cases
# - extend area completely inside test image

# delete test collection

# search for proper string in response
if [ ...... ]
then
	RC=$RC_ERROR
	RCTEXT=$RCTEXT_ERROR
else
	RC=$RC_OK
	RCTEXT=$RCTEXT_OK
fi

# clean up
rm -f $TMPFILE

echo $PROG: done, result is $RCTEXT.
exit $RC

