#!/bin/bash

EXC="exc.sh"

CREDB="rasdl RASBASE -c"
INSDB="rasdl RASBASE -r $RMANHOME/include/basictypes.odl -i"
PRNDB="rasdl RASBASE -p"
DELDB="rasdl RASBASE -deldatabase"


CMD=$CREDB
echo "1. Creating RASBASE."
COUT="create1.out"
$EXC "$CMD" $COUT
echo "2. Creating RASBASE again."
COUT="create2.out"
$EXC "$CMD" $COUT


CMD=$INSDB
echo "3. Inserting basic types into RASBASE."
COUT="insert1.out"
$EXC "$CMD" $COUT
echo "4. Inserting basic types into RASBASE again."
COUT="insert2.out"
$EXC "$CMD" $COUT


CMD=$PRNDB
echo "5. Printing basic types from RASBASE."
COUT="types1.out"
$EXC "$CMD" $COUT


CMD=$DELDB
echo "6. Deleting RASBASE."
COUT="delete1.out"
$EXC "$CMD" $COUT
echo "7. Deleting RASBASE again."
COUT="delete2.out"
$EXC "$CMD" $COUT
