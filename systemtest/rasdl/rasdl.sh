#!/bin/bash
cdir=$PWD
cd $RMANHOME/systemtest/rasdl
./test_rasdl.sh > log.out 2>&1
diff log.out compare.out
retval=$? 
cd $cdir
echo $retval
exit $retval
