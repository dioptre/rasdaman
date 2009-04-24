#!/usr/bin/ksh

echo "runbm started."

# defaults if no parameters are given
RUNS='-n 10'
basename='NorbertBase'

# parse command line
while [[ 1 -eq 1 ]]; do
  if [[ $1 = '-n' ]]; then
    shift; RUNS="-n $1"; shift;
  elif [[ $1 = '-e' ]]; then
    shift; RUNS="-e $1"; shift;
  elif [[ $1 = '-base' ]]; then
    shift; basename=$1; shift;
  else
    break;
  fi
done

QLFILES=${@}
# optionally add different servers here
RASSERVERS='rasserver'

BMTIME=$(date +%d%b%y_%H-%M)

mkdir bm_$BMTIME

# shutdown server in case it is already running
$RMANBASE/server/rascontrol $HOST down

for servername in $RASSERVERS; do
  for qlname in $QLFILES; do
    echo "$servername $qlname:"
    # take time when this test started
    QLTIME=$(date +%d%b%y_%H-%M)

    filename=$(pwd)/bm_$BMTIME/bm${servername#rasserver}_${qlname%\.ql}

    # start server

    # should be like the following line, but rasserver does not like long
    # filenames :-(
    # $RMANBASE/server/$servername -bl 4 -l $filename\_server.log -b $filename\_server.bm -d $filename\_server.dbg &

    # workaround
    $RMANBASE/server/$servername -bl 4 -notimeout &
    # store PID
    RMANPID=$!

    # wait till it has finished
    sleep 30

    # write information file
    touch $filename\.inf
    echo "Filename of rasserver used: $servername" > $filename\.inf
    echo "Host: $HOST" >> $filename\.inf
    echo "Time: $QLTIME" >> $filename\.inf
    echo "Output of what on rasserver:" >> $filename\.inf
    what $RMANBASE/server/$servername >> $filename\.inf
    echo "Output of what on test_benchmark:" >> $filename\.inf
    what $RMANBASE/rasodmg/test/test_benchmark >> $filename\.inf

    # run benchmark
    $RMANBASE/rasodmg/test/test_benchmark $RUNS $HOST $basename $qlname

    # save client information
    mv client.bm $filename\_client.bm
    mv client.log $filename\_client.log
    mv client.dbg $filename\_client.dbg

    # shutdown server
    $RMANBASE/server/rascontrol $HOST down
    # just to be sure
    kill $RMANPID 1>/dev/null 2>&1

    # workaround
    mv server.log $filename\_server.log
    mv server.bm $filename\_server.bm
    mv server.dbg $filename\_server.dbg

    # store queryfile
    cp $qlname $filename\.ql

    # create slk file for Excel
    $RMANBASE/utilities/bm2sylk.pl $filename\_client.bm $filename\_server.bm > $filename.slk

  done
done

echo "runbm finished."

exit
