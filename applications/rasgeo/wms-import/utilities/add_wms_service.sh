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
# SYNTAX
#	$ $0 ARGS [OPTS]
#
# DESCRIPTION
#       Inserts a tuple in $PETADB::$TABLE_SERVICES table.
#
# RETURN CODES
        RC_OK=0         # all went fine
        RC_ERROR=1      # something went wrong
#
# PRECONDITIONS
#       - a metadata database has been set up (usually petascopedb)
#	- no service with the same name is present in the database
#
# CHANGE HISTORY
#       2012-may-11     pcampalani	created
#
# COPYRIGHT (c) 2003 Dr. Peter Baumann
#####################################################################################################

#################
#   CONSTANTS   #
#################

# Arguments 
VALUE_KEYWORD='VALUE'
#
     NAME_ARG='--name'
    TITLE_ARG='--title'
     HOST_ARG='--host'
     PORT_ARG='--port'
     PATH_ARG='--path'
  FORMATS_ARG='--formats'
    AVAIL_ARG='--availability'
BASELAYER_ARG='--baselayer-name'
   ARGS_COUNT=8		# Update this number in case the mandatory args change
#
CONTACTPERSON_OPT='--contact-person'
   CONTACTORG_OPT='--contact-organization'
  ADDRESSTYPE_OPT='--address-type'
      ADDRESS_OPT='--address'
         CITY_OPT='--city'
  STATEORPROV_OPT='--state-or-province'
     POSTCODE_OPT='--postcode'
      COUNTRY_OPT='--country'
 CONTACTVOICE_OPT='--contact-tel'
   CONTACTFAX_OPT='--contact-fax'
 CONTACTEMAIL_OPT='--contact-email'
    USEQUENCE_OPT='--update-sequence'
     ABSTRACT_OPT='--abstract'
     KEYWORDS_OPT='--keywords'
         FEES_OPT='--fees'
       ACCESS_OPT='--access-constraints'
    VENDORCAP_OPT='--vendor-capabilities'
       OPTS_COUNT=17	# Update this number in case the optional args change
#
USAGE=$( cat << EOF
$0
    $NAME_ARG $VALUE_KEYWORD
    $TITLE_ARG $VALUE_KEYWORD
    $HOST_ARG $VALUE_KEYWORD
    $PORT_ARG $VALUE_KEYWORD
    $PATH_ARG $VALUE_KEYWORD
    $FORMATS_ARG $VALUE_KEYWORD
    $AVAIL_ARG $VALUE_KEYWORD
    $BASELAYER_ARG $VALUE_KEYWORD
    [$CONTACTPERSON_OPT $VALUE_KEYWORD] [$CONTACTORG_OPT $VALUE_KEYWORD]
    [$ADDRESSTYPE_OPT $VALUE_KEYWORD]   [$ADDRESS_OPT $VALUE_KEYWORD]
    [$CITY_OPT $VALUE_KEYWORD]           [$STATEORPROV_OPT $VALUE_KEYWORD]
    [$POSTCODE_OPT $VALUE_KEYWORD]       [$COUNTRY_OPT $VALUE_KEYWORD]
    [$CONTACTVOICE_OPT $VALUE_KEYWORD]    [$CONTACTFAX_OPT $VALUE_KEYWORD]
    [$CONTACTEMAIL_OPT $VALUE_KEYWORD]  [$USEQUENCE_OPT $VALUE_KEYWORD]
    [$ABSTRACT_OPT $VALUE_KEYWORD]       [$KEYWORDS_OPT $VALUE_KEYWORD]
    [$FEES_OPT $VALUE_KEYWORD]           [$ACCESS_OPT $VALUE_KEYWORD]
    [$VENDORCAP_OPT $VALUE_KEYWORD]
EOF
)

# Database connections
          USER=rasadmin
        PASSWD=rasadmin
  CONNECT_FILE="$HOME/.rasdaman/rasconnect"
      HOST_KEY=host
    PGPORT_KEY=pgport
PETADBNAME_KEY=petadbname
  PETAUSER_KEY=petauser
PETAPASSWD_KEY=petapassword
#
PETADB_HOST=$(cat $CONNECT_FILE | grep $HOST_KEY 	| awk 'BEGIN { FS="=" }; { print $2 }')
PETADB_PORT=$(cat $CONNECT_FILE | grep $PGPORT_KEY 	| awk 'BEGIN { FS="=" }; { print $2 }')
PETADB_NAME="$(cat $CONNECT_FILE | grep $PETADBNAME_KEY | awk 'BEGIN { FS="=" }; { print $2 }')"
   PETAUSER=$(cat $CONNECT_FILE | grep $PETAUSER_KEY 	| awk 'BEGIN { FS="=" }; { print $2 }')
 PETAPASSWD=$(cat $CONNECT_FILE | grep $PETAPASSWD_KEY 	| awk 'BEGIN { FS="=" }; { print $2 }')

# Database tables

# Tables to be updated in the metadata databas
TABLE_SERVICES=ps_services
# PS_SERVICES
SERVICES_SERVICEID=serviceid
SERVICES_HOST=hostname
SERVICES_PORT=port
SERVICES_PATH=path
SERVICES_FORMATS=formats
SERVICES_NAME=name
SERVICES_TITLE=title
SERVICES_CONTACTPERSON=contactperson
SERVICES_CONTACTORGANIZATION=contactorganization
SERVICES_ADDRESSTYPE=addresstype
SERVICES_ADDRESS=address
SERVICES_CITY=city
SERVICES_STATEORPROV=stateorprovince
SERVICES_POSTCODE=postcode
SERVICES_COUNTRY=country
SERVICES_CONTACTVOICE=contactvoicetelephone
SERVICES_CONTACTFAX=contactfacsimiletelephone
SERVICES_CONTACTEMAIL=contactelectronicmailaddress
SERVICES_USEQUENCE=updatesequence
SERVICES_AVAIL=availability #TODO turn it to bool, add table contraint that only one service can be ON, then change PETASCOPE WMS!!
SERVICES_ABSTRACT=abstract
SERVICES_KEYWORDS=keywords
SERVICES_FEES=fees
SERVICES_ACCESS=accessconstraints
SERVICES_BASELAYER=baselayername
SERVICES_VENDORCAP=vendorcapabilities

# Database response patterns
SELECT_NULL="(0 rows)"
PG_INSERT_OK='INSERT 0 1'
UNDEFINED_KEYWORD='undefined'

###################
# ARGUMENTS CHECK #
###################
MIN_ARGS=$(( $ARGS_COUNT * 2 ))
MAX_ARGS=$(( $MIN_ARGS + $OPTS_COUNT * 2 ))
if [ $# -lt "$MIN_ARGS" -o $# -gt "$MAX_ARGS" ]; then
	echo -e "USAGE\n$USAGE"
	exit $RC_ERROR
fi

# Store arguments value
echo -en "\nParsing arguments... "
while [ $# -gt 0 ]; do
 
       case "$1" in

          $NAME_ARG) 		NAME="$2";	shift;;
          $TITLE_ARG) 		TITLE="$2";  	shift;;
	  $HOST_ARG) 		HOST="$2";	shift;;
          $PORT_ARG) 		PORT="$2";  	shift;;
          $PATH_ARG) 		WMSPATH="$2"; 	shift;;	# (!) don't overwrite PATH variable.
          $FORMATS_ARG) 	FORMATS="$2";  	shift;;
          $AVAIL_ARG) 		AVAIL="$2";  	shift;;
          $BASELAYER_ARG) 	BASELAYER="$2";	shift;;

          $CONTACTPERSON_OPT)	CONTACTPERSON="$2";  	shift;;
          $CONTACTORG_OPT) 	CONTACTORG="$2";  	shift;;
          $ADDRESSTYPE_OPT) 	ADDRESSTYPE="$2";  	shift;;
          $ADDRESS_OPT) 	ADDRESS="$2";  		shift;;
          $CITY_OPT) 		CITY="$2";  		shift;;
          $STATEORPROV_OPT) 	STATEORPROV="$2";	shift;;
          $POSTCODE_OPT) 	POSTCODE="$2";  	shift;;
          $COUNTRY_OPT) 	COUNTRY="$2";  		shift;;
          $CONTACTVOICE_OPT) 	CONTACTVOICE="$2";  	shift;;
          $CONTACTFAX_OPT) 	CONTACTFAX="$2";  	shift;;

          $USEQUENCE_OPT) 	USEQUENCE="$2";  	shift;;
          $ABSTRACT_OPT) 	ABSTRACT="$2";		shift;;
          $KEYWORDS_OPT) 	KEYWORDS="$2";		shift;;
          $FEES_OPT) 		FEES="$2";		shift;;
          $ACCESS_OPT) 		ACCESS="$2";		shift;;
          $VENDORCAP_OPT) 	VENDORCAP="$2";		shift;;

          *) echo -e "(!) Unknown argument \"$1\".\nusage: $USAGE"; exit $RC_ERROR;; 

        esac
        shift
done

# Check if mandatory arguments were set
[[ "$NAME"      = "" ]] && (echo -e "(!) Missing $NAME_ARG required option.\n usage: $USAGE"; exit $RC_ERROR)
[[ "$TITLE"     = "" ]] && (echo -e "(!) Missing $TITLE_ARG required option.\n usage: $USAGE"; exit $RC_ERROR)
[[ "$HOST"      = "" ]] && (echo -e "(!) Missing $HOST_ARG required option.\n usage: $USAGE"; exit $RC_ERROR)
[[ "$PORT"      = "" ]] && (echo -e "(!) Missing $PORT_ARG required option.\n usage: $USAGE"; exit $RC_ERROR)
[[ "$WMSPATH"   = "" ]] && (echo -e "(!) Missing $PATH_ARG required option.\n usage: $USAGE"; exit $RC_ERROR)
[[ "$FORMATS"   = "" ]] && (echo -e "(!) Missing $FORMATS_ARG required option.\n usage: $USAGE"; exit $RC_ERROR)
[[ "$AVAIL"     = "" ]] && (echo -e "(!) Missing $AVAIL_ARG required option.\n usage: $USAGE"; exit $RC_ERROR) 
[[ "$BASELAYER" = "" ]] && (echo -e "(!) Missing $BASELAYER_ARG required option.\n usage: $USAGE"; exit $RC_ERROR) 

echo "Done."

###################
# INSERT INTO DB  #
###################
echo "*** Using database $PETADB_NAME@$PETADB_HOST:$PETADB_PORT."
# Export variables to open session with psql
export PGUSER="$PETAUSER"
export PGPASSWORD="$PETAPASSWD"

# Check if a service with the same name is already present, and exit in case
# TODO: allow the update of an existing WMS service
# TODO: allow the deletion of an existing WMS service.
# TODO: create script to activate a specified WMS service
echo -n "Checking presence of homonymous WMS services in $PETADB_NAME::$TABLE_SERVICES... "
query="SELECT $SERVICES_SERVICEID FROM $TABLE_SERVICES WHERE $SERVICES_NAME='$NAME';"
ret=$( echo "$query" | psql -f - --single-transaction -h "$PETADB_HOST" -p "$PETADB_PORT" "$PETADB_NAME" )
#
echo "$ret" | grep "$SELECT_NULL" 1>/dev/null
if [ "$?"  -eq 0 ]; then
	echo -en "Done.\nProceed with insertion... "

	# Build up the query with the available fields
	FIELDS="$SERVICES_NAME,$SERVICES_TITLE,$SERVICES_HOST,$SERVICES_PORT,$SERVICES_PATH,$SERVICES_FORMATS,$SERVICES_AVAIL,$SERVICES_BASELAYER"
	VALUES="        '$NAME',       '$TITLE',       '$HOST',        $PORT,     '$WMSPATH',       '$FORMATS',        $AVAIL,        '$BASELAYER'"
	#
	if [ "$CONTACTPERSON" != "" ]; then FIELDS+=",$SERVICES_CONTACTPERSON";	VALUES+=",'$CONTACTPERSON'"; fi
	if [ "$ADDRESSTYPE"   != "" ]; then FIELDS+=",$SERVICES_ADDRESSTYPE"; 	VALUES+=",'$ADDRESSTYPE'"; fi
	if [ "$ADDRESS"       != "" ]; then FIELDS+=",$SERVICES_ADDRESS";	VALUES+=",'$ADDRESS'"; fi
	if [ "$CITY"          != "" ]; then FIELDS+=",$SERVICES_CITY"; 		VALUES+=",'$CITY'"; fi
	if [ "$STATEORPROV"   != "" ]; then FIELDS+=",$SERVICES_STATEORPROV"; 	VALUES+=",'$STATEORPROV'"; fi
	if [ "$POSTCODE"      != "" ]; then FIELDS+=",$SERVICES_POSTCODE";	VALUES+=",'$POSTCODE'"; fi
	if [ "$COUNTRY"       != "" ]; then FIELDS+=",$SERVICES_COUNTRY"; 	VALUES+=",'$COUNTRY'"; fi
	if [ "$CONTACTVOICE"  != "" ]; then FIELDS+=",$SERVICES_CONTACTVOICE"; 	VALUES+=",'$CONTACTVOICE'"; fi
	if [ "$CONTACTFAX"    != "" ]; then FIELDS+=",$SERVICES_CONTACTFAX"; 	VALUES+=",'$CONTACTFAX'"; fi
	if [ "$CONTACTEMAIL"  != "" ]; then FIELDS+=",$SERVICES_CONTACTEMAIL"; 	VALUES+=",'$CONTACTEMAIL'"; fi
	if [ "$USEQUENCE"     != "" ]; then FIELDS+=",$SERVICES_USEQUENCE"; 	VALUES+=",'$USEQUENCE'"; fi
	if [ "$ABSTRACT"      != "" ]; then FIELDS+=",$SERVICES_ABSTRACT"; 	VALUES+=",'$ABSTRACT'"; fi
	if [ "$KEYWORDS"      != "" ]; then FIELDS+=",$SERVICES_KEYWORDS"; 	VALUES+=",'$KEYWORDS'"; fi
	if [ "$FEES"          != "" ]; then FIELDS+=",$SERVICES_FEES"; 		VALUES+=",'$FEES'"; fi
	if [ "$ACCESS"        != "" ]; then FIELDS+=",$SERVICES_ACCESS"; 	VALUES+=",'$ACCESS'"; fi
	if [ "$VENDORCAP"     != "" ]; then FIELDS+=",$SERVICES_VENDORCAP"; 	VALUES+=",'$VENDORCAP'"; fi

	query="INSERT INTO $TABLE_SERVICES ($FIELDS) VALUES ($VALUES) RETURNING $SERVICES_SERVICEID;"
	ret=$( echo "$query" | psql --single-transaction -h "$PETADB_HOST" -p "$PETADB_PORT" "$PETADB_NAME" )

	echo "$ret" | grep "$PG_INSERT_OK" 1>/dev/null
	if [ "$?" -ne 0 ]; then
		echo "Error while inserting tuple in $TABLE_SERVICES table of $PETADB_NAME@$PETADB_HOST:$PETADB_PORT. Postgres response:"
		echo "$ret"
		exit $RC_ERROR
	else
		serviceID=$(  echo $ret | awk 'BEGIN { FS=" " }; { print $3 };' )
	fi 
	echo "Done: serviceId=$serviceID."
else
	serviceID=$(  echo $ret | awk 'BEGIN { FS=" " }; { print $3 };' )
	echo -e "WMS service '$NAME' present in the database (serviceid=$serviceID).\nWill now exit..."
	exit $RC_ERROR
fi

exit $RC_OK
