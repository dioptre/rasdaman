#!/bin/bash
####################### Makefile header ##########################
#
# test_insertppm: test insertppm, rasgeo ppm import utility 
#
# TEST CANDIDATE:
# insertppm
#
# TEST MODE:
# automatic
#
# PREREQUISITES:
# - rasdaman server (RNP, RPC) running, any base DBMS
# - ~rasdaman/images accessible with standard imagery
#
# CHANGE HISTORY (append further entries):
# when:       who:      what:
# ----------------------------------------------------------------
# 2005-dec-02 PB        created
#
# COMMENTS:
# - exit -1 if "ERROR" found in log file or in diff to canonical log file
# - test program must be in cwd
# - old log is saved to <logfile>.save
# - todo:
# 	test storage and transfer parameters
#
# Copyright (C) 2005 Dr. Peter Baumann
#
##################################################################

# --- declarations -----------------------------------------------

# shorthands and orga stuff
IPPM=../insertppm

PROG=`basename $0`
INDENT="+++ "

# ERROR keyword, should be standardised across all rasdaman tests
ERROR=ERROR

# log file output
LOG=`basename $0 .sh`.log

# reference log file
REFLOG=$LOG.reference

# save old log here if it exists
SAVELOG=$LOG.save

# test defaults:
COLL_RGB=rgb
COLL_GREY=mr
COLL_3D=mr3d
FILEPATH=~rasdaman/images
FILE_RGB=$FILEPATH/anthur.ppm
FILE_GREY=$FILEPATH/mr_1.pgm
FILE_LIST=$FILEPATH/mr_?.pgm
TYPE_GREY=grey
TYPE_RGB=color
TILED_3D=[1:2,3:4,5:6]

SERVER=`hostname`
PORT=7001
DATABASE=RASBASE
USER=rasadmin
PASSWD=rasadmin

TRANSFERFORMAT=TIFF
TRANSFERFORMATPARAMS=TIFF_PARAMS
STORAGEFORMAT=RLE
STORAGEFORMATPARAMS=RLE_PARAMS

# --- action -----------------------------------------------

# save old log if present
if [ -f $LOG ]
then
	echo found old log file, shifting it to $SAVELOG
	mv $LOG $SAVELOG
fi
echo "$0: testing insertppm start" | tee $LOG

echo $INDENT $INDENT starting softly
echo $INDENT no params
( $IPPM  || (export RC=$?; echo exit code $RC) ) | tee -a $LOG

echo $INDENT call help
( $IPPM -h  || (export RC=$?; echo exit code $RC) ) | tee -a $LOG

echo $INDENT call help
( $IPPM --help  || (export RC=$?; echo exit code $RC) ) | tee -a $LOG

echo $INDENT all correct
echo $INDENT $INDENT all mandatory syntactically correct
# ( $IPPM -c $COLL_RGB $FILE_RGB || (export RC=$?; echo Fatal $ERROR, exit code $RC) ) | tee -a $LOG

echo $INDENT $INDENT all syntactically correct, long version,
# ( $IPPM --collection $COLL_3D --type $TYPE_GREY --rescale --tiled $TILED_3D --verbose \
# 	--server $SERVER --port $PORT --database $DATABASE --user $USER --passwd $PASSWD \
# 	`ls $FILE_LIST` || (export RC=$?; echo Fatal $ERROR, exit code $RC) ) | tee -a $LOG

echo collection param missing:
( $IPPM $FILE_RGB \
	|| (export RC=$?; echo exit code $RC) ) | tee -a $LOG
echo file param missing:
( $IPPM --collection $COLL_RGB \
	|| (export RC=$?; echo exit code $RC) ) | tee -a $LOG

echo $INDENT $INDENT starting softly done
echo $INDENT $INDENT illegal cmd line options start

echo $INDENT $INDENT mdd domain parameter syntax mismatch

echo tbd.

echo $INDENT $INDENT illegal cmd line options done
echo
echo $INDENT $INDENT image type bool
echo $INDENT $INDENT $INDENT nontiled
echo tbd.
echo $INDENT $INDENT $INDENT tiled
echo tbd.
echo $INDENT $INDENT image type grey
echo $INDENT $INDENT $INDENT nontiled
echo tbd.
echo $INDENT $INDENT $INDENT tiled
echo tbd.
echo $INDENT $INDENT image type rgb
echo $INDENT $INDENT $INDENT nontiled
echo tbd.
echo $INDENT $INDENT $INDENT tiled
echo tbd.

echo $INDENT $INDENT transfer and storage parameters
echo tbd.

# --- now compare result with canonical log:

if [ `diff $LOG $REFLOG | wc -l` -ne "0" ]
then
	echo $0: $ERROR: log file differes from reference log file, please examine!
	exit -1
else
	echo $0: no difference found between log and reference log
fi

echo "$0: testing insertppm done" >>$LOG
echo "$0: done."


