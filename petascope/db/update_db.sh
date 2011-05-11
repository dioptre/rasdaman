#!/bin/bash

# To setup petascope database for the first time: ./update_db.sh 0
# To upgrade the database starting from a specific update number N, use ./update_db.sh N
#
# Dimitar Misev

counter=$1

if [ $counter = 'dropall' ]; then
	psql -d petascopedb < droptables.sql
else
	while [ -f update$counter.sql ]; do
		psql -d petascopedb < update$counter.sql
		let counter=$counter+1
	done
fi
