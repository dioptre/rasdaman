#!/bin/bash
# 
# SOURCE: test_rasogc.sh
# 
# PURPOSE:
# run rasogc test by doing some requests.
# adapt variables below to local situation!
#
# PRECONDITIONS:
# - Brandenburg ortho image in db
# - rasogc (servlet, rasdaman, DBVMS) up and running
# - URL to WMS must be correct
# - existing layers must be addressed, with the proper styles available, and within the overall bbox of the resp. layer
#
# CHANGE HISTORY (append further entries):
# when         who     what
# ----------------------------------------------------------
# 2003-may-24  PB        created
# 2003-jun-04  PB        zoom box is computed as centered box of basic case
# 2005-jul-14  PB        embedded into test env, made automatic
# 2012-apr-24 pcampalani adapt tests for Petascope.
#
# COMMENTS:
#
# Copyright (C) 2001 Dr. Peter Baumann

WGET=/usr/bin/wget

# dir for test output
TMPDIR=`basename $0 .sh`.test

# against this dir contents we compare in the regression test
OLDDIR=`basename $0 .sh`.old

URL="http://localhost:8080"
SERVICE="petascope/wms"

# coordinates for basic good case (must be integer for the expr below to work):
# VAT db:
LAYER="australia_wms"
SRS='EPSG:4326'
STYLES='standard'

XMIN='110'
XMAX='120'
YMIN='-40'
YMAX='-30'

WIDTH=900
HEIGHT=900

# coordinates for good zoom case (center of previous box, half size):
XMIN_ZOOM=`expr \( 3 \* $XMIN + $XMAX \) / 4 `
YMIN_ZOOM=`expr \( 3 \* $YMIN + $YMAX \) / 4 `
XMAX_ZOOM=`expr \( $XMIN + 3 \* $XMAX \) / 4 `
YMAX_ZOOM=`expr \( $YMIN + 3 \* $YMAX \) / 4 `

# --- action -----------------------------------------------------------

mkdir $TMPDIR || exit 1
mkdir $OLDDIR || exit 1

echo "+++ +++ \"good\" cases"

echo "+++ get capabilities"
$WGET -O $TMPDIR/1 "$URL/$SERVICE?REQUEST=GetCapabilities&SERVICE=WMS>$URL/rasogc/servlet/rasogc?REQUEST=GetCapabilities&SERVICE=WMS"

echo "+++ get map:"

echo "regular request:"
$WGET -O $TMPDIR/2 "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"

echo "zoom in by some factor:"
$WGET -O $TMPDIR/3 "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN_ZOOM,$YMIN_ZOOM,$XMAX_ZOOM,$YMAX_ZOOM&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "background color 0xFf0000:"
$WGET -O $TMPDIR/4 "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN_ZOOM,$YMIN_ZOOM,$XMAX_ZOOM,$YMAX_ZOOM&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&BGCOLOR=0xFf0000"
echo "background color 0x00Ff00:"
$WGET -O $TMPDIR/5 "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN_ZOOM,$YMIN_ZOOM,$XMAX_ZOOM,$YMAX_ZOOM&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&BGCOLOR=0x00Ff00"
echo "background color 0x0000fF:"
$WGET -O $TMPDIR/6 "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN_ZOOM,$YMIN_ZOOM,$XMAX_ZOOM,$YMAX_ZOOM&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&BGCOLOR=0x0000fF"

echo "+++ reload capabilities:"
$WGET -O $TMPDIR/7 "$URL/$SERVICE?REQUEST=ReloadCapabilities"

echo "+++ upper/lower case mixing:"

$WGET -O $TMPDIR/8 "$URL/$SERVICE?RequesT=GetCapabilities&sErViCe=WMS"
$WGET -O $TMPDIR/9 "$URL/$SERVICE?version=1.1.0&sErViCe=WMS&Request=GetMap&laYers=$LAYER&sRs=$SRS&StYlEs=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&wIDTh=$WIDTH&hEIGHT=$HEIGHT&fOrMaT=image/jpeg&eXCeptions=application/vnd.ogc.se_inimage&cuSTOMDEM=$CUSTOMDEM"

echo "exception format types - using a syntactic standard error situation, BBOX wrong:"

echo "XML: XML code is returned"
$WGET -O $TMPDIR/a "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=_wrong_&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_xml"
echo "inimage: image containing text is returned"
$WGET -O $TMPDIR/b "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=_wrong_&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage"
echo "blank: white, empty image is returned"
$WGET -O $TMPDIR/c "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=_wrong_&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_blank"

echo "+++ +++ test queries: \"bad\" cases"

echo "+++ missing elements:"

echo "SERVICE missing: silently assumes WMS, thats ok"
$WGET -O $TMPDIR/d "$URL/$SERVICE?VERSION=1.1.0&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "REQUEST missing: response undefined"
$WGET -O $TMPDIR/e "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "VERSION missing: error msg"
$WGET -O $TMPDIR/f "$URL/$SERVICE?SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "LAYERS missing: error msg"
$WGET -O $TMPDIR/g "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "SRS missing: error msg"
$WGET -O $TMPDIR/h "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "STYLES missing: error msg"
$WGET -O $TMPDIR/i "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "BBOX missing: error msg"
$WGET -O $TMPDIR/j "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "WIDTH missing: undefined reaction"
$WGET -O $TMPDIR/k "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "HEIGHT missing: undefined reaction"
$WGET -O $TMPDIR/l "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "FORMAT missing: undefined reaction ...??"
$WGET -O $TMPDIR/m "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "EXCEPTIONS missing: "good" request returns nicely"
$WGET -O $TMPDIR/n "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&CUSTOMDEM=$CUSTOMDEM"
echo "CUSTOMDEM missing: ...ok"
$WGET -O $TMPDIR/o "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage"
echo "BGCOLOR missing: white background"
$WGET -O $TMPDIR/p "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN_ZOOM,$YMIN_ZOOM,$XMAX_ZOOM,$YMAX_ZOOM&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage"

echo "+++ malformed requests:"

echo "SERVICE wrong: silently assumes WMS ...tbd"
$WGET -O $TMPDIR/q "$URL/$SERVICE?VERSION=1.1.0&SERVICE=_wrong_&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "REQUEST wrong: reaction undefined"
$WGET -O $TMPDIR/r "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=_wrong_&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "VERSION wrong: error msg"
$WGET -O $TMPDIR/s "$URL/$SERVICE?VERSION=_wrong_&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "LAYERS wrong: ok, error message"
$WGET -O $TMPDIR/t "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=_wrong_&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "SRS wrong: ok, error message"
$WGET -O $TMPDIR/u "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=_wrong_&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "STYLES wrong: ok, error message"
$WGET -O $TMPDIR/v "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=_wrong_&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "BBOX wrong: error msg"
$WGET -O $TMPDIR/w "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=_wrong_&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "WIDTH wrong: response undefined"
$WGET -O $TMPDIR/x "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=_wrong_&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "HEIGHT wrong: response undefined"
$WGET -O $TMPDIR/y "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=_wrong_&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "FORMAT wrong: error msg"
$WGET -O $TMPDIR/z "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=_wrong_&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=$CUSTOMDEM"
echo "EXCEPTIONS wrong: ok, XML error msg"
$WGET -O $TMPDIR/A "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=_wrong_&CUSTOMDEM=$CUSTOMDEM"
echo "CUSTOMDEM wrong: ok, proper = non-DEM image"
$WGET -O $TMPDIR/B "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN,$YMIN,$XMAX,$YMAX&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&CUSTOMDEM=_wrong_"
echo "BGCOLOR wrong - string, no hex:"
$WGET -O $TMPDIR/C "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN_ZOOM,$YMIN_ZOOM,$XMAX_ZOOM,$YMAX_ZOOM&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&BGCOLOR=0x_wrong_"
echo "BGCOLOR wrong - no 0x:"
$WGET -O $TMPDIR/D "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN_ZOOM,$YMIN_ZOOM,$XMAX_ZOOM,$YMAX_ZOOM&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&BGCOLOR=336699"
echo "BGCOLOR wrong - 0x, then string:"
$WGET -O $TMPDIR/E "$URL/$SERVICE?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&LAYERS=$LAYER&SRS=$SRS&STYLES=$STYLES&BBOX=$XMIN_ZOOM,$YMIN_ZOOM,$XMAX_ZOOM,$YMAX_ZOOM&WIDTH=$WIDTH&HEIGHT=$HEIGHT&FORMAT=image/jpeg&EXCEPTIONS=application/vnd.ogc.se_inimage&BGCOLOR=0x_wrong_"

# FIXME OLDDIR does not contain data. No comparison takes place: the test just lets you read the responses in TMPDIR.
for i in `ls $OLDDIR`
do
	cmp $OLDDIR/$i $TMPDIR/$i || echo "Error comparing file $i"
done

# rmdir $TMPDIR

echo "$0: done."

