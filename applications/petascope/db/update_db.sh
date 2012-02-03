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
# SYNOPSIS
# ./update_db.sh
# Description
#  Command-line utility for updating the petascope database. The updating is
#  done by importing all appropriate updateX.sql files to postgres. The script
#  determines automatically which updates need to be imported. There are three
#  cases:
#   1. no petascope database is present (no ps_coverage table), in which case 
#      the updates start from 0.
#   2. a ps_updates table is present, in this case the updates start from the
#      number saved in the update column.
#	  3. dropall is specified, which drops all petascope tables
#
# PRECONDITIONS
#  1)Postgres Server must be running
#  2)Rasdaman Server should be running if petascope is being installed for the
#    first time

CODE_OK=0
CODE_FAIL=255
SETTINGS=settings.properties

# check the Postgres
ps -e | grep --quiet -w postgres
if [ $? -ne 0 ]; then
    ps -e | grep --quiet -w postmaster
    if [ $? -ne 0 ]; then
       echo "no postgres available"
       exit $CODE_FAIL
    fi
fi

# check the settings
if [ ! -e settings.properties ]; then
	echo "Warn: no settings.properties available"
	echo "Copying default settings src/main/resources/$SETTINGS to db/$SETTINGS"
	cp "../src/main/resources/$SETTINGS" .
fi

# postgres connection details
USER=`grep metadata_user "$SETTINGS" | awk -F "=" '{print $2}'`
DB=`grep metadata_url "$SETTINGS" | awk -F "/" '{print $4}'`
PORT=`grep metadata_url "$SETTINGS" | awk -F ":|/" '{print $6}'`
PASS=`grep metadata_pass "$SETTINGS" | awk -F "=" '{print $2}'`
HOST=`hostname`
USER="${USER#"${USER%%[![:space:]]*}"}"
PASS="${PASS#"${PASS%%[![:space:]]*}"}"

echo $HOST:*:*:$USER:$PASS > ~/.pgpass
chmod 600 ~/.pgpass


PSQL="psql -U $USER -d $DB -p $PORT -h $HOST"
echo Postgres connection details
echo Database: $DB
echo Port: $PORT
echo cmd: $PSQL
echo Testing connection...
$PSQL -c "create table ps_tmp (id integer)" > /dev/null
if [ $? -ne 0 ]; then
	echo Error
	exit $CODE_FAIL
else
	$PSQL -c "drop table ps_tmp" > /dev/null
	echo OK
fi


if [ -n "$1" ]; then
	if [ "$1" = "dropall" ]; then
		$PSQL -f droptables.sql > /dev/null
        if [ "$?" != "0" ]; then 
            echo Cannot drop tables from $DB
        else
            echo Dropped all petascope tables from $DB
        fi
	fi
else
	# updating petascope database
	$PSQL -c 'select * from ps_dbupdates' > /dev/null 2>&1
	if [ $? -ne 0 ]; then # create from scratch
		echo "Creating petascope database from scratch"
		counter=0
		
		# check first if rasdaman is running
		ps -e | grep --quiet -w rasmgr
		if [ $? -ne 0 ]; then
			 echo "rasdaman server not running"
			 exit $CODE_FAIL
		fi
		
		# rasdaman connection details
		RAS_USER=`grep rasdaman_admin_user "$SETTINGS" | awk -F "=" '{print $2}'`
		RAS_PASS=`grep rasdaman_admin_pass "$SETTINGS" | awk -F "=" '{print $2}'`
		RAS_DB=`grep rasdaman_database "$SETTINGS" | awk -F "=" '{print $2}'`
		RAS_SERVER=`grep rasdaman_url "$SETTINGS" | awk -F ":|/" '{print $4}'`
		RAS_PORT=`grep rasdaman_url "$SETTINGS" | awk -F ":|/" '{print $5}'`
		echo Rasdaman connection details:
		echo User: $RAS_USER
		echo Pass: $RAS_PASS
		echo Database: $RAS_DB
		echo Server: $RAS_SERVER
		echo Port: $RAS_PORT
		echo
		RASQL="rasql --user $RAS_USER --passwd $RAS_PASS -d $RAS_DB -s $RAS_SERVER -p $RAS_PORT"
		
		# insert sample data
		$RASQL -q 'create collection NIR RGBSet' > /dev/null 2>&1
		if [ $? -ne 0 ]; then
			echo Sample coverage data NIR already inserted
		else
        echo Inserting coverage NIR into rasdaman
            $RASQL -q 'insert into NIR values inv_tiff($1)' --file nir.tiff > /dev/null 2>&1
		fi

		$RASQL -q 'create collection mean_summer_airtemp GreySet' > /dev/null 2>&1
		if [ $? -ne 0 ]; then
			echo Sample coverage data mean_summer_airtemp already inserted
		else
            echo Inserting coverage mean_summer_airtemp into rasdaman
            $RASQL -q 'insert into mean_summer_airtemp values inv_tiff($1)' --file mean_summer_airtemp.tif > /dev/null 2>&1
		fi
	else # update from the last update number
		counter=`$PSQL -c 'select update from ps_dbupdates where id=1' | head -n 3 | tail -n 1 | awk '{print $1}'`
		echo "Updating petascope database from update $counter"
	fi
	echo
	
	# inserting the database updates
	while [ -e update$counter.sql ]; do
		$PSQL < update$counter.sql > /dev/null 2>&1
		let counter=$counter+1
	done
	$PSQL -c "update ps_dbupdates set update=$counter where id=1" > /dev/null 2>&1
	echo Updated database to update $counter
fi

exit $CODE_OK
