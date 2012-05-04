#!/bin/bash
#
# test_transp.sh - test rasogc transparency
#
# SYNTAX
#       test_transp.sh
#
# DESCRIPTION
#       Verify that TRANSPARENCY=true returns a transparent image, set with background color.
#
# RESPONDING TO INCIDENT
#       Incident_2005-07-12_IGN_png-transparency
#
# PROCEDURE
#	issues a request with background and transparency setting, using format that supports it, check result file header.
#
# PRECONDITIONS
# - WMS configured and running (rasogc, rasdaman, base DBMS)
# - URL/path address as defined below
# - Brandenburg Orthophoto via WMS available
# - ImageMagick installed ('identify' tool)
#
# RETURN CODES
        RC_OK=0         # everything went fine
        RC_ERROR=1      # something went wrong
#
# CHANGE HISTORY
#       2005-jul-14     P.Baumann       created
# 	2012-apr-24 	pcampalani 	adapt tests for Petascope, remove BGCOLOR key which is set when 
#					no transparency is requested. Updated `identify' command to
#					recognize image with transparency, see:
# 					[ http://stackoverflow.com/questions/2581469/detect-alpha-channel-with-imagemagick ]					
#
# COPYRIGHT (c) 2005 Dr. Peter Baumann


RCTEXT_OK="OK"
RCTEXT_ERROR="NOT_OK"

# --- CONSTANTS -----------------------------------------------------

# service URI
URI="http://localhost:8080"

# request values
SERVICE="petascope/wms"
LAYER="australia_wms"
SRS='EPSG:4326'
STYLE='standard'
BBOX='110,-40,120,-30'
WIDTH=900
HEIGHT=900

# background color
BACKGROUND_COLOR=FF0000
# request
REQUEST="VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLE&BBOX=$BBOX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/png&EXCEPTIONS=application/vnd.ogc.se_inimage&transparent=true"

# complete request:
COMPLETE_REQUEST=$URI/$SERVICE?$REQUEST

# name of script
PROG=`basename $0`

# temp file for getcap response:
TMPFILE=/tmp/`basename $PROG .sh`.tmp

# --- ACTION --------------------------------------------------------

echo $PROG: Verify transparency

# send request to WMS, get response file
wget -nv -O $TMPFILE $COMPLETE_REQUEST

# search for proper string in response
if [ `identify -format \'%[channels]\' $TMPFILE | grep rgba | wc -l` -eq 0 ]
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
