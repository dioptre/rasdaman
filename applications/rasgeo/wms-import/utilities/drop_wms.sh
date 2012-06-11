#!/bin/bash
#
# ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
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
# ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
#
# Unpublish a WMS layer (drop metadata from db) and the associated pyramid collections in rasdaman.
#
# SYNTAX
	ME="$( basename $0 )"
	ARG_KEEPPYR="--keep-pyramids"
	ARG_DELSERVICE="--del-service"
	USAGE="
	usage: $ME <layerName> [$ARG_KEEPPYR] [$ARG_DELSERVICE]
	where
		<layerName> must be an existing WMS layer in Petascope.
		$ARG_KEEPPYR might be optionally set to keep the related pyramid collections in rasdaman.
		$ARG_DELSERVICE to reset the service (delete all the layers and remove the service).
	"
	# In case the usage changes, consequently adjust these values:
	MIN_ARGS=1
	MAX_ARGS=3
# DESCRIPTION
#  	Given a previously published WMS layer from an existing rasdaman collection 
#	(via init_wms.sh utility), this script just reverts the situation, unpublishing
#	the WMS metadata of the specified layer from petascopedb and, if not explicitely
#	requested, drops the associated pyramid collections from RASBASE.
#
# RETURN CODES
        RC_OK=0         # all went fine
        RC_ERROR=1      # something went wrong
#
# PRECONDITIONS
#	- the specified collname has been already, at least, initialized as WMS layer (init_wms.sh)
#	- rasgeo connectfile is set up in $HOME/.rasdaman/ (see rasgeo README file);
#       - of course, rasdaman and postgres are up and running.
#
# TODO: 
#    ...
#
# --- DEFAULTS ------------------------------------------------------

# user / password for logging in to rasdaman
# (defined here so that they do not appear in command line)
USER=rasadmin
PASSWD=rasadmin

WGET="$( which wget )"

# load metadata db connection info (see ./rasgeo/README)
  CONNECT_FILE="$HOME/.rasdaman/rasconnect"
      HOST_KEY=host
    PGPORT_KEY=pgport
 RASDBNAME_KEY=rasdbname
PETADBNAME_KEY=petadbname
  PETAUSER_KEY=petauser
PETAPASSWD_KEY=petapassword
#
   RAS_HOST=$(cat $CONNECT_FILE  | grep $HOST_KEY       | awk 'BEGIN { FS="=" }; { print $2 }')
    PG_PORT=$(cat $CONNECT_FILE  | grep $PGPORT_KEY     | awk 'BEGIN { FS="=" }; { print $2 }')
 RASDB_NAME=$(cat $CONNECT_FILE  | grep $RASDBNAME_KEY  | awk 'BEGIN { FS="=" }; { print $2 }')
PETADB_NAME="$(cat $CONNECT_FILE | grep $PETADBNAME_KEY | awk 'BEGIN { FS="=" }; { print $2 }')"
   PETAUSER=$(cat $CONNECT_FILE  | grep $PETAUSER_KEY   | awk 'BEGIN { FS="=" }; { print $2 }')
 PETAPASSWD=$(cat $CONNECT_FILE  | grep $PETAPASSWD_KEY | awk 'BEGIN { FS="=" }; { print $2 }')

# --- END DEFAULTS --------------------------------------------------

# --- CONSTANTS -----------------------------------------------------
# --- do not change anything here unless you have a real clue

# RASQL 
     RASQL='rasql'
 QUERY_OPT='-q'
  USER_OPT='--user'
PASSWD_OPT='--passwd'
 DROP_COMM='drop collection'

# Tables to be updated in the metadata database
     TABLE_SERVICES=ps_services
 TABLE_SERVICELAYER=ps_servicelayer
       TABLE_LAYERS=ps_layers
       TABLE_STYLES=ps_styles
TABLE_PYRAMIDLEVELS=ps_pyramidlevels
# PS_LAYER
LAYERS_LAYERID=layerid
LAYERS_NAME=name
# PS_PYRAMIDLEVELS
PYRAMIDLEVELS_LAYERID=layerid
PYRAMIDLEVELS_COLLECTIONNAME=collectionname
PYRAMIDLEVELS_SCALEFACTOR=scalefactor

# PSQL return values
PG_SELECT_NULL="(0 rows)"
  PG_SELECT_OK="(1 row)"

# WMS service
PETASCOPE_HOST='localhost'
PETASCOPE_PORT=8080
WMS_PATH='petascope/wms'
PETASCOPEWMS_URL="http://$PETASCOPE_HOST:$PETASCOPE_PORT/$WMS_PATH"	# add argument option?
WMS_VERSION='1.1.0'
WMS_RELOADCAPABILITIES='reloadcapabilities'

# --- END CONSTANTS -------------------------------------------------

# --- PARAMETER EVALUATION ------------------------------------------

echo "$ME: Using databases {$RASDB_NAME,$PETADB_NAME}@$RAS_HOST:$PG_PORT."

# check whether wget tool is available
if [ ! -x "$WGET" ]
then
	echo "$ME: ERROR: $WGET is not executable."
	exit $RC_ERROR
fi

# check number of parameters
if [ $# -lt $MIN_ARGS -o $# -gt $MAX_ARGS ]
then
	echo "$USAGE"
	exit $RC_ERROR
fi

# get parameters and check them
layerName="$1"
while [ $# -gt 0 ]; do 
	case "$1" in
          $ARG_KEEPPYR)    KEEPPYR="$1";;
 	  $ARG_DELSERVICE) DELSERVICE="$1";;
 	  $layerName);;
          *) echo -e "(!) Unknown argument \"$1\".\n $USAGE"; exit $RC_ERROR;;
	esac
        shift
done

# Check existence of WMS layer
query="	SELECT $LAYERS_LAYERID FROM $TABLE_LAYERS WHERE $LAYERS_NAME = '$layerName'; "
echo "$query"
ret=$( echo "$query" | psql -f - --single-transaction -h "$RAS_HOST" -p "$PG_PORT" "$PETADB_NAME" )
#
echo "$ret" | grep "$PG_SELECT_NULL" 1>/dev/null
if [ "$?" -eq 0 ]; then
	echo "$ME: ERROR: "$layerName" must be an existing collection in $PETADB_NAME@$RAS_HOST:$PG_PORT."
	exit $RC_ERROR
else 
	layerId=$( echo "$ret" | sed ':a;N;$!ba;s/\n/ /g' | awk 'BEGIN { FS=" " }; { print $3 };' )
	echo "$ME: WMS layer $layerName has ID #$layerId."

fi

# --- END PARAMETER EVALUATION --------------------------------------
# --- ACTION --------------------------------------------------------

if [ "$KEEPPYR" != "$ARG_KEEPPYR" ]; then
	echo -n "$ME: Dropping pyramid collections associated to $layerName... "
	# Fetch pyramid collection names (don't remove baselayer!)
	query=" SELECT $TABLE_PYRAMIDLEVELS.$PYRAMIDLEVELS_COLLECTIONNAME, $TABLE_PYRAMIDLEVELS.$PYRAMIDLEVELS_SCALEFACTOR
		FROM $TABLE_PYRAMIDLEVELS, $TABLE_LAYERS
		WHERE $TABLE_PYRAMIDLEVELS.$PYRAMIDLEVELS_LAYERID = $TABLE_LAYERS.$LAYERS_LAYERID
		AND $TABLE_LAYERS.$LAYERS_NAME = '$layerName';"
	while read line; do 
		pyramidName=$( echo "$line" | awk 'BEGIN {FS=" " }; { print $1 };' )
		pyramidFactor=$( echo "$line" | awk 'BEGIN {FS=" " }; { print $3 };' )
		if [ "$pyramidFactor" -ne 1 ]; then 
			echo -n "$ME: Dropping pyramid level $pyramidName... "
			"$RASQL" "$QUERY_OPT" "$DROP_COMM $pyramidName" "$USER_OPT" "$USER" "$PASSWD_OPT" "$PASSWD" 1>/dev/null
			echo "Done."
		fi
	done <<< "$( echo "$query" | psql -f - --single-transaction -h "$RAS_HOST" -p "$PG_PORT" "$PETADB_NAME"  | grep "$COLLNAME"_ )"
fi

echo "$ME: Cleaning WMS metadata of $layerName... "
for wmsTable in "$TABLE_PYRAMIDLEVELS" "$TABLE_STYLES" "$TABLE_SERVICELAYER" "$TABLE_LAYERS"; do
	echo -n "$ME: Removing metadata from $wmsTable... "
	query=" DELETE FROM $wmsTable WHERE $LAYERS_LAYERID = $layerId; "
	ret=$( echo "$query" | psql -f - --single-transaction -h "$RAS_HOST" -p "$PG_PORT" "$PETADB_NAME" )
	echo "Done."
done

# If WMS need to be reset, then just remove all metadata from the tables:
if [ "$DELSERVICE" = "$ARG_DELSERVICE" ]; then
	echo -n "$ME: Dropping all tuples from WMS tables... "
	query=" 
		DELETE FROM $TABLE_PYRAMIDLEVELS;
		DELETE FROM $TABLE_STYLES;
		DELETE FROM $TABLE_SERVICELAYER;
		DELETE FROM $TABLE_SERVICES;
		DELETE FROM $TABLE_LAYERS;
	"
	ret=$( echo "$query" | psql -f - --single-transaction -h "$RAS_HOST" -p "$PG_PORT" "$PETADB_NAME" )
	echo "$ME: done."
fi

# reload new capabilities file into rasogc
echo "$ME: reloading capabilities into rasgeo URL=$PETASCOPEWMS_URL..."
$WGET -q "$PETASCOPEWMS_URL?request=$WMS_RELOADCAPABILITIES&service=wms&version=$WMS_VERSION"
rm -f *$WMS_RELOADCAPABILITIES*	# wget bug? Redirects output to wms?.... file.
echo "$ME: Database $PETADB_NAME has been updated and WMS servlet ($PETASCOPEWMS_URL) refreshed."

echo 
echo "$ME: done."
exit $RC_OK
# --- END ACTION --------------------------------------------------------
