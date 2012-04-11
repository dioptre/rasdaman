#!/bin/bash
#
# test_getcap.sh - test rasogc GetCapabilities response
#
# SYNTAX
#       test_getcap.sh
#
# DESCRIPTION
#       Verify that the GetCapabilities response is standards conformant.
#
# RESPONDING TO INCIDENT
#       2005-05-25_IGN
#
# PROCEDURE
#	issues a GetCapabilities request and verifies <name> tag to be "OGC:WMS".
#
# PRECONDITIONS
# - WMS configured and running (rasogc, rasdaman, base DBMS)
# - URL/path address as defined below
#
# RETURN CODES
        RC_OK=0         # everything went fine
        RC_ERROR=1      # something went wrong
#
# CHANGE HISTORY
#       2005-may-25     P.Baumann       created
#       2005-jul-14     P.Baumann       brushed up
#
# COPYRIGHT (c) 2003 Dr. Peter Baumann


RCTEXT_OK="OK"
RCTEXT_ERROR="NOT_OK"

# --- CONSTANTS -----------------------------------------------------

# service URI
URI="http://localhost:8080"

# service path
SERVICE="petascope/wms"

# request
REQUEST="SERVICE=WMS&REQUEST=GetCapabilities"

# complete request:
COMPLETE_REQUEST=$URI/$SERVICE?$REQUEST

# name of script
PROG=`basename $0`

# temp file for getcap response:
TMPFILE=`basename $PROG .sh`.tmp

# --- ACTION --------------------------------------------------------

echo $PROG: Verify that GetCapabilities response is standards compliant

# send request to WMS, get response file
wget -nv -O $TMPFILE $COMPLETE_REQUEST

# search for proper string in response, we must have one line (at least)
if [ `grep -i name $TMPFILE | grep WMS:WMS | wc -l` -eq 0 ]
then
	RC=$RC_ERROR
	RCTEXT=$RCTEXT_ERROR
	# log response for analysis purposes
	#cat $TMPFILE
else
	RC=$RC_OK
	RCTEXT=$RCTEXT_OK
fi

# clean up
rm -f $TMPFILE

echo $PROG: done, result is $RCTEXT.
exit $RC

