#!/bin/bash -x

# test_hostcmp: test hostCmp() function of rasmgr

# return codes
export RC_OK=0
export RC_ERROR=1

INDENT="+++ "
TESTPROG=./test_hostcmp

HOST_SHORT=abc
HOST_LONG=${HOST_SHORT}.def.ghi
HOST_OTHER=xyz

echo $0: testing rasmgr/rascontrol.

echo $INDENT good cases, equal:
$TESTPROG $HOST_SHORT $HOST_SHORT
$TESTPROG $HOST_LONG $HOST_LONG
$TESTPROG "" ""

echo $INDENT general cases, equal:
$TESTPROG $HOST_SHORT $HOST_LONG
$TESTPROG $HOST_LONG $HOST_SHORT

echo $INDENT general cases, NOT equal:
$TESTPROG $HOST_SHORT $HOST_OTHER
$TESTPROG $HOST_LONG  $HOST_OTHER

echo $INDENT special cases, NOT equal:
$TESTPROG $HOST_SHORT ""
$TESTPROG "" $HOST_SHORT

echo $0: done.

