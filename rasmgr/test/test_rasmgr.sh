#!/bin/bash -x

# return codes
export RC_OK=0
export RC_ERROR=1

export RASMGR=rasmgr
export RASMGR_CONF=rasmgr.conf
export RASMGR_AUTH=rasmgr_auth.dat

# settings demanded by rasmgr
export RMANHOME=.

echo $0: testing rasmgr/rascontrol.

# start rasmgr

if [ -f nohup.out ]
then
	rm nohup.out || (echo "Error: cannot remove old nohup log file, exiting."; exit $RC_ERROR)
fi

export START_AND_KILL_RASMGR="
	( nohup $RASMGR & ; 		\
	  export RASMGR_PID=\$!; 	\
	  sleep 4; 			\
	  kill \$RASMGR_PID; 		\
	  cat nohup.out; 		\
	  rm nohup.out			\
	)				\
	|| echo Error: cannot start/terminate/... rasmgr. "

echo $START_AND_KILL_RASMGR

# -- conf file -------------------

# conf file not present
if [ -f $RASMGR_CONF ]
then
	rm $RASMGR_CONF || (echo "Error: cannot remove old $RASMGR_CONF file, exiting."; exit $RC_ERROR)
fi
eval $START_AND_KILL_RASMGR
exit

# empty conf file 
echo >$RASMGR_CONF
$START_AND_KILL_RASMGR

# conf file not readable
chmod a-r $RASMGR_CONF
$START_AND_KILL_RASMGR
chmod a+rw $RASMGR_CONF

# illegal cmd in conf file
cat >$RASMGR_CONF <<EOF
iiiiiiiiiiillegal cmd
EOF
$START_AND_KILL_RASMGR

# for further tests, we provide a sane conf file
cat >$RASMGR_CONF <<EOF
define dbh melange_host -connect /
define srv S1 -host `hostname` -type r -port 0x29999901 -dbh melange_host
define db RASBASE -dbh melange_host
EOF

# -- auth file -------------------
rm $RASMGR_AUTH || (echo "Error: cannot remove old $RASMGR_AUTH file, exiting."; exit $RC_ERROR)

# auth file not present
if [ -f $RASMGR_AUTH ]
then
	rm $RASMGR_AUTH || (echo "Error: cannot remove old $RASMGR_AUTH file, exiting."; exit $RC_ERROR)
fi
$START_AND_KILL_RASMGR

# empty auth file 
echo >$RASMGR_AUTH
$START_AND_KILL_RASMGR

# auth file not readable
chmod a-r $RASMGR_AUTH
$START_AND_KILL_RASMGR
chmod a+rw $RASMGR_AUTH

# auth file has illegal contents
echo iiiiiiiiillegal >$RAMGR_AUTH
$START_AND_KILL_RASMGR

# auth file has good contents -- let rasmgr generate the default file, then check
rm $RASMGR_AUTH
$START_AND_KILL_RASMGR
$START_AND_KILL_RASMGR

# -- highlander check ------------
# start an additional rasmgr, see what it tells
$START_RASMGR
export SECOND_PID=$!
$START_AND_KILL_RASMGR
kill $SECOND_PID

echo $0: done.

