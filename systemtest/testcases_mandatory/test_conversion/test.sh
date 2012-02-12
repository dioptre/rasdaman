#!/bin/bash
#!/bin/ksh
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
#	test.sh
# Description
#	Command-line utility for testing rasdaman.
#	1)creating collection
# 	2)insert images into collection
# 	3)extract images 
# 	4)compare
# 	5)cleanup 
#
# PRECONDITIONS
# 	1)Postgres Server must be running
# 	2)Rasdaman Server must be running
# 	3)database RASBASE must exists
# 	4)rasql utility must be fully running
#	5)images needed for testing shall be put in directory of images 	
# Usage: ./test.sh 
#        
# CHANGE HISTORY
#       2009-Sep-16     J.Yu       created
#

# further tests will be done on dem, inv_dem, tor and inv_tor, after their implementations.

# Variables
PROGNAME=`basename $0`
DIR_NAME=$(dirname $0)
LOG_DIR=$DIR_NAME  
LOG=$LOG_DIR/log
OLDLOG=$LOG.save
USERNAME=rasadmin	
PASSWORD=rasadmin
DATABASE=RASBASE
IMAGEDIR=$DIR_NAME/testdata
ORACLE_DIR=$DIR_NAME/oracle
RASQL="rasql --quiet"
RASDL="rasdl"
GDALINFO="gdalinfo -noct -checksum"

  CODE_OK=0
  CODE_FAIL=255

# NUM_TOTAL: the number of manipulations
# NUM_FAIL: the number of fail manipulations
# NUM_SUC: the number of success manipulations
  NUM_TOTAL=0
  NUM_FAIL=0
  NUM_SUC=0 
  
#--------------- check if old logfile exists ----------------------------------
if [ -f $LOG ]
then
	echo Old logfile found, copying it to $OLDLOG
	mv $LOG $OLDLOG
fi

echo "Test by:"$PROGNAME" at "`date`|tee $LOG

#---------------------------Precondition------------------------------------------
# check the Postgres
ps -e | grep --quiet -w postgres
if [ $? -ne 0 ]
then
   echo no postgres available|tee -a $LOG
   exit $CODE_FAIL
fi

# check the Rasdaman
ps -e | grep --quiet rasmgr
if [ $? -ne 0 ]
then
   echo no rasmgr available|tee -a $LOG 
   exit $CODE_FAIL
fi

# check tiffinfo
which gdalinfo > /dev/null
if [ $? -ne 0 ]
then
   echo gdal tools missing, please add gdalinfo to the PATH|tee -a $LOG 
   exit $CODE_FAIL
fi

# check usr
#
# check data collection
$RASQL -q "select r from RAS_COLLECTIONNAMES as r"
if [ $? -ne 0 ]
then
   echo no data collection available|tee -a $LOG 
   exit $CODE_FAIL
fi

# check data type
$RASDL --print|grep --quiet GreySet
if [ $? -ne 0 ]
then
   echo no GreSet type available|tee -a $LOG 
   exit $CODE_FAIL
fi

$RASDL --print|grep --quiet RGBSet
if [ $? -ne 0 ]
then
   echo no RGBSet type available, try create_db.sh|tee -a $LOG 
   exit $CODE_FAIL
fi

# check data set

#--------------------------initiation--------------------------------------------
# drop collection if they already exists

if   $RASQL -q "select r from RAS_COLLECTIONNAMES as r" --out string|grep test_tmp 
then
	echo dropping collection ... | tee -a $LOG
	$RASQL -q "drop collection test_tmp" --user $USERNAME --passwd $PASSWORD | tee -a $LOG
fi

################## jpeg() and inv_jpeg() #######################
echo -----jpeg and inv_jpeg conversion------ | tee -a $LOG
echo creating collection ... | tee -a $LOG
$RASQL -q "create collection test_tmp  GreySet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection  test_tmp| tee -a $LOG

echo inserting collection ... | tee -a $LOG
$RASQL -q 'insert into test_tmp  values inv_jpeg($1)' -f $IMAGEDIR/mr_1.jpeg --user $USERNAME --passwd $PASSWORD || echo Error inserting jpeg image | tee -a $LOG

echo extracting collection ... | tee -a $LOG
$RASQL -q "select jpeg(a) from test_tmp as a" --out file --outfile mr_1.jpeg  --user $USERNAME --passwd $PASSWORD || 
		echo Error extracting jpeg image | tee -a $LOG

echo  comparing images | tee -a $LOG
$GDALINFO mr_1.jpeg* | grep 'Checksum' > mr_1.jpeg.result
diff $ORACLE_DIR/mr_1.jpeg.checksum mr_1.jpeg.result

if [ $? != "0" ]
then
	echo input and output does not match | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
else
	echo input and output match | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
fi

$RASQL -q "drop collection test_tmp" --user $USERNAME --passwd $PASSWORD | tee -a $LOG
rm mr_1.jpeg*
################## tiff() and inv_tiff() #######################
echo ------tiff and inv_tiff conversion------ | tee -a $LOG
echo creating collection ... | tee -a $LOG
$RASQL -q "create collection test_tmp  GreySet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection test_tmp | tee -a $LOG

echo inserting collection ... | tee -a $LOG
$RASQL -q 'insert into test_tmp  values inv_tiff($1)' -f $IMAGEDIR/mr_1.tif --user $USERNAME --passwd $PASSWORD || echo Error inserting tiff image | tee -a $LOG

echo extracting collection ... | tee -a $LOG
$RASQL -q "select tiff(a) from test_tmp as a" --out file --outfile mr_1.tif --user $USERNAME --passwd $PASSWORD || 
		echo Error extracting tiff image | tee -a $LOG

echo  comparing images | tee -a $LOG
$GDALINFO mr_1.tif* | grep 'Checksum' > mr_1.tif.result
diff $ORACLE_DIR/mr_1.tif.checksum mr_1.tif.result

if [ $? != "0" ]
then
	echo input and output does not match | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
else
	echo input and output match | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
fi

echo dropping collections ... | tee -a $LOG
$RASQL -q "drop collection test_tmp" --user $USERNAME --passwd $PASSWORD | tee -a $LOG
rm mr_1.tif*


echo ------tiff and inv_tiff multiband conversion------ | tee -a $LOG

echo reading types ... | tee -a $LOG
$RASDL -p | grep --quiet 'TestSet'
if [ $? -ne 0 ]; then
  $RASDL -r $IMAGEDIR/types.dl -i > /dev/null
fi

echo creating collection ... | tee -a $LOG
$RASQL -q "create collection test_tmp TestSet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection test_tmp | tee -a $LOG

echo inserting collection ... | tee -a $LOG
$RASQL -q 'insert into test_tmp values inv_tiff($1, "bandtype=octet")' -f $IMAGEDIR/multiband.tif --user $USERNAME --passwd $PASSWORD || echo Error inserting tiff image | tee -a $LOG

echo extracting collection ... | tee -a $LOG
$RASQL -q "select tiff(a) from test_tmp as a" --out file --outfile multiband.tif --user $USERNAME --passwd $PASSWORD || echo Error extracting tiff image | tee -a $LOG

echo  comparing images | tee -a $LOG
$GDALINFO multiband.tif* | grep 'Checksum' > multiband.tif.result
diff $ORACLE_DIR/multiband.tif.checksum multiband.tif.result

if [ $? != "0" ]
then
	echo input and output does not match | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
else
	echo input and output match | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
fi

echo dropping collections ... | tee -a $LOG
$RASQL -q "drop collection test_tmp" --user $USERNAME --passwd $PASSWORD | tee -a $LOG

rm multiband.tif*
################## png() and inv_png() #######################

echo ------png and inv_png conversion------ | tee -a $LOG
echo creating collection ... | tee -a $LOG
$RASQL -q "create collection test_tmp  GreySet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection test_tmp | tee -a $LOG

echo inserting collection ... | tee -a $LOG
$RASQL -q 'insert into test_tmp  values inv_png($1)' -f $IMAGEDIR/mr_1.png --user $USERNAME --passwd $PASSWORD || echo Error inserting png image | tee -a $LOG

echo extracting collection ... | tee -a $LOG
$RASQL -q "select png(a) from test_tmp as a" --out file --outfile mr_1.png --user $USERNAME --passwd $PASSWORD || 
		echo Error extracting png image | tee -a $LOG

echo  comparing images | tee -a $LOG
cmp $IMAGEDIR/mr_1.png mr_1.png.unknown 

if [ $? != "0" ]
then
	echo input and output does not match | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
else
	echo input and output match | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
fi

echo dropping collections ... | tee -a $LOG
$RASQL -q "drop collection test_tmp" --user $USERNAME --passwd $PASSWORD | tee -a $LOG
rm mr_1.png.unknown
################## bmp() and inv_bmp() #######################

echo ------bmp and inv_bmp conversion------ | tee -a $LOG
echo creating collection ... | tee -a $LOG
$RASQL -q "create collection test_tmp  GreySet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection test_tmp | tee -a $LOG

echo inserting collection ... | tee -a $LOG
$RASQL -q 'insert into test_tmp  values inv_bmp($1)' -f $IMAGEDIR/mr_1.bmp --user $USERNAME --passwd $PASSWORD || echo Error inserting bmp image | tee -a $LOG

echo extracting collection ... | tee -a $LOG
$RASQL -q "select bmp(a) from test_tmp as a" --out file --outfile mr_1.bmp --user $USERNAME --passwd $PASSWORD || 
		echo Error extracting bmp image | tee -a $LOG

echo  comparing images | tee -a $LOG
$GDALINFO mr_1.bmp* | grep 'Checksum' > mr_1.bmp.result
diff $ORACLE_DIR/mr_1.bmp.checksum mr_1.bmp.result

if [ $? != "0" ]
then
	echo input and output does not match | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
else
	echo input and output match | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
fi

echo dropping collections ... | tee -a $LOG
$RASQL -q "drop collection test_tmp" --user $USERNAME --passwd $PASSWORD | tee -a $LOG
rm mr_1.bmp*

################## vff() and inv_vff() #######################


echo ------vff and inv_vff conversion------ | tee -a $LOG
echo creating collection ... | tee -a $LOG
$RASQL -q "create collection test_tmp  GreySet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection test_tmp | tee -a $LOG

echo inserting collection ... | tee -a $LOG
$RASQL -q 'insert into test_tmp  values inv_vff($1)' -f $IMAGEDIR/mr_1.vff --user $USERNAME --passwd $PASSWORD || echo Error inserting vff image | tee -a $LOG

echo extracting collection ... | tee -a $LOG
$RASQL -q 'select vff(a) from test_tmp as a' --out file --outfile mr_1.vff  --user $USERNAME --passwd $PASSWORD || 
		echo Error extracting vff image | tee -a $LOG

echo  comparing images | tee -a $LOG
cmp $IMAGEDIR/mr_1.vff  mr_1.vff.unknown 

if [ $? != "0" ]
then
	echo input and output does not match | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
else
	echo input and output match | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
fi

echo dropping collections ... | tee -a $LOG
$RASQL -q "drop collection test_tmp" --user $USERNAME --passwd $PASSWORD | tee -a $LOG
rm mr_1.vff.unknown
################## hdf() and inv_hdf() #######################


echo ------hdf and inv_hdf conversion------ | tee -a $LOG
echo creating collection ... | tee -a $LOG
$RASQL -q "create collection test_tmp  GreySet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection test_tmp | tee -a $LOG

echo inserting collection ... | tee -a $LOG
$RASQL -q 'insert into test_tmp  values inv_hdf($1)' -f $IMAGEDIR/mr_1.hdf --user $USERNAME --passwd $PASSWORD || echo Error inserting hdf4 image | tee -a $LOG

echo extracting collection ... | tee -a $LOG
$RASQL -q "select hdf(a) from test_tmp as a" --out file --outfile mr_1.hdf --user $USERNAME --passwd $PASSWORD || 
		echo Error extracting hdf4 image | tee -a $LOG

echo  comparing images | tee -a $LOG
$GDALINFO mr_1.hdf* | grep 'Checksum' > mr_1.hdf.result
diff $ORACLE_DIR/mr_1.hdf.checksum mr_1.hdf.result

if [ $? != "0" ]
then
	echo input and output does not match | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
else
	echo input and output match | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
fi

echo dropping collections ... | tee -a $LOG
$RASQL -q "drop collection test_tmp" --user $USERNAME --passwd $PASSWORD | tee -a $LOG
rm mr_1.hdf*
################## csv() #######################


echo ------csv conversion------ | tee -a $LOG
echo creating collection ... | tee -a $LOG
$RASQL -q "create collection test_tmp  GreySet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection test_tmp | tee -a $LOG

echo inserting collection ... | tee -a $LOG
$RASQL -q 'insert into test_tmp  values inv_png($1)' -f $IMAGEDIR/mr_1.png --user $USERNAME --passwd $PASSWORD || echo Error inserting png image | tee -a $LOG

echo extracting collection ... | tee -a $LOG
$RASQL -q "select csv(a) from test_tmp as a" --out file --outfile mr_1.csv --user $USERNAME --passwd $PASSWORD || echo Error extracting csv image | tee -a $LOG

echo  comparing images | tee -a $LOG
cmp $IMAGEDIR/mr_1.csv mr_1.csv*

if [ $? != "0" ]
then
	echo input and output does not match | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
else
	echo input and output match | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
fi

echo dropping collections ... | tee -a $LOG
$RASQL -q "drop collection test_tmp" --user $USERNAME --passwd $PASSWORD | tee -a $LOG
rm mr_1.csv*


echo ------csv composite type conversion------ | tee -a $LOG
echo creating collection ... | tee -a $LOG
$RASQL -q "create collection test_tmp  RGBSet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection test_tmp | tee -a $LOG

echo inserting collection ... | tee -a $LOG
$RASQL -q 'insert into test_tmp  values inv_png($1)' -f $IMAGEDIR/rgb.png --user $USERNAME --passwd $PASSWORD || echo Error inserting png image | tee -a $LOG

echo extracting collection ... | tee -a $LOG
$RASQL -q "select csv(a[115:130,110:112]) from test_tmp as a" --out file --outfile rgb.csv --user $USERNAME --passwd $PASSWORD || echo Error extracting csv image | tee -a $LOG

echo  comparing images | tee -a $LOG
cmp $IMAGEDIR/rgb.csv rgb.csv*

if [ $? != "0" ]
then
	echo input and output does not match | tee -a $LOG
	NUM_FAIL=$(($NUM_FAIL + 1))
else
	echo input and output match | tee -a $LOG
	NUM_SUC=$(($NUM_SUC + 1))
fi

echo dropping collections ... | tee -a $LOG
$RASQL -q "drop collection test_tmp" --user $USERNAME --passwd $PASSWORD | tee -a $LOG
rm rgb.csv*
################## Dem() and inv_dem() #######################


# echo ------dem and inv_dem conversion------ | tee -a $LOG
# echo creating collection ... | tee -a $LOG
# $RASQL -q "create collection test_tmp  GreySet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection test_tmp | tee -a $LOG

# echo inserting collection ... | tee -a $LOG
# $RASQL -q 'insert into test_tmp  values inv_dem($1)' -f $IMAGEDIR/mr_1.dem --user $USERNAME --passwd $PASSWORD || echo Error inserting dem image | tee -a $LOG

# echo extracting collection ... | tee -a $LOG
# $RASQL -q "select dem(a) from test_tmp as a" --out file --outfile mr_1.dem --user $USERNAME --passwd $PASSWORD || 
#		echo Error extracting dem image | tee -a $LOG

# echo  comparing images | tee -a $LOG
# cmp $IMAGEDIR/mr_1.dem mr_1.dem.unknown 

# if [ $? != "0" ]
# then
#	echo input and output does not match | tee -a $LOG
# 	NUM_FAIL=$(($NUM_FAIL + 1))
# else
# 	echo input and output match | tee -a $LOG
#	NUM_SUC=$(($NUM_SUC + 1))
# fi

# echo dropping collections ... | tee -a $LOG
# $RASQL -q "drop collection test_tmp" --user $USERNAME --passwd $PASSWORD | tee -a $LOG
# rm mr_1.dem.unknown
################## tor() and inv_tor() #######################


# echo ------tor and inv_tor conversion------ | tee -a $LOG
# echo creating collection ... | tee -a $LOG
# $RASQL -q "create collection test_tmp  GreySet" --user $USERNAME --passwd $PASSWORD || echo Error creating collection test_tmp | tee -a $LOG

# echo inserting collection ... | tee -a $LOG
# $RASQL -q 'insert into test_tmp  values inv_tor($1)' -f $IMAGEDIR/mr_1.tor --user $USERNAME --passwd $PASSWORD || echo Error inserting tor image | tee -a $LOG

# echo extracting collection ... | tee -a $LOG
# $RASQL -q 'select tor(a) from test_tmp as a' --out file --outfile mr_1.tor  --user $USERNAME --passwd $PASSWORD || 
#		echo Error extracting tor image | tee -a $LOG

# echo  comparing images | tee -a $LOG
# cmp $IMAGEDIR/mr_1.tor  mr_1.tor.unknown 

# if [ $? != "0" ]
# then
#	echo input and output does not match | tee -a $LOG
#	NUM_FAIL=$(($NUM_FAIL + 1))
# else
#	echo input and output match | tee -a $LOG
#	NUM_SUC=$(($NUM_SUC + 1))
# fi

# echo dropping collections ... | tee -a $LOG
# $RASQL -q "drop collection test_tmp" --user $USERNAME --passwd $PASSWORD | tee -a $LOG
# rm mr_1.tor.unknown

################# summary #######################
  NUM_TOTAL=$(($NUM_SUC + $NUM_FAIL))
# Print the summary
  echo "test done at "`date`|tee -a $LOG
  echo "Total conversions: "$NUM_TOTAL|tee -a $LOG
  echo "Successful conversion number: "$NUM_SUC|tee -a $LOG
  echo "Failed conversion number: "$NUM_FAIL|tee -a $LOG
  echo "Detail test log is in "$LOG 
  
  if [ $NUM_TOTAL = $NUM_SUC ]
  then
  	exit $CODE_OK
  else
	exit $CODE_FAIL
  fi
