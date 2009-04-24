#!/bin/bash

if [ $USER != rastest ]; then
  echo "You are $USER."
  echo "Only user rastest is allowed to run $0."
  exit 1
fi

EXC="exc.sh"

CREDB="rasdl RASBASE -c"
INSDB="rasdl RASBASE -r $RMANHOME/include/basictypes.odl -i"
PRNDB="rasdl RASBASE -p"
DELDB="rasdl RASBASE -deldatabase"

TOUT="test_rasdl.out"

CMD=$CREDB
echo -n "1. Creating RASBASE: "
COUT="create1.out"
$EXC "$CMD" $TOUT
diff $COUT $TOUT
if [ $? = 0 ]; then
  echo "OK"
else 
  echo "FAILED"
fi

echo -n "2. Creating RASBASE again: "
COUT="create2.out"
$EXC "$CMD" $TOUT
diff $COUT $TOUT
if [ $? = 0 ]; then
  echo "OK"
else 
  echo "FAILED"
fi

CMD=$INSDB
echo -n "3. Inserting basic types into RASBASE: "
COUT="insert1.out"
$EXC "$CMD" $TOUT
diff $COUT $TOUT
if [ $? = 0 ]; then
  echo "OK"
else 
  echo "FAILED"
fi

echo -n "4. Inserting basic types into RASBASE again: "
COUT="insert2.out"
$EXC "$CMD" $TOUT
diff $COUT $TOUT
if [ $? = 0 ]; then
  echo "OK"
else 
  echo "FAILED"
fi

CMD=$PRNDB
echo -n "5. Printing basic types from RASBASE: "
COUT="types1.out"
$EXC "$CMD" $TOUT
diff $COUT $TOUT
if [ $? = 0 ]; then
  echo "OK"
else 
  echo "FAILED"
fi

CMD=$DELDB
echo -n "6. Deleting RASBASE: "
COUT="delete1.out"
$EXC "$CMD" $TOUT
diff $COUT $TOUT
if [ $? = 0 ]; then
  echo "OK"
else 
  echo "FAILED"
fi

echo -n "7. Deleting RASBASE again: "
COUT="delete2.out"
$EXC "$CMD" $TOUT
diff $COUT $TOUT
if [ $? = 0 ]; then
  echo "OK"
else 
  echo "FAILED"
fi
