#!/bin/bash

# check_opensockets.sh: periodically check the number of waiting sockets in the local system
# parameters:
#	$1	sleep time between checks

export SLEEPTIME=$1

if test "$1" == ""
then
	echo usage: $0 sleepseconds
	exit
fi

echo "$0: start checking for open (waiting) sockets, in intervals of $SLEEPTIME seconds."

while echo \--- check time: `date` \----------------
do
	netstat -na | grep WAIT
	echo summary: `netstat -na | grep WAIT | wc -l` TIME_WAIT state sockets.
	sleep $SLEEPTIME
done

echo $0: done.

