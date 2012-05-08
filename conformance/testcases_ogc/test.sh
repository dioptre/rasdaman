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
# Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012 Peter Baumann /
# rasdaman GmbH.
#
# For more information please see <http://www.rasdaman.org>
# or contact Peter Baumann via <baumann@rasdaman.com>.
#
# SYNOPSIS
#	test.sh
# Description
#	Command-line utility for testing rasdaman.
#	1) download ogc teamengine 2.0 
#	2) download ogc ctl 
#	3) run test
#	4) Give out the testing result. 
#
# PRECONDITIONS
# 	Rasdaman petascope, unzip installed
#
# Usage: ./test.sh 
#	  	
# Parameters: 
# 
#
# CHANGE HISTORY
#       2012-Mar-09     J.Yu       created
#
# Parameter definistion and initiation
  PROG=`basename $0`
  WGET="wget"
  UNZIP="unzip"
  CHMOD="chmod"
# RMANBASE: change RMANBASE depending on your system, where rasdaman is installed
# RASQL="rasql --quiet" no ornament msg
# TEAM_URL: the teamengine URL
# TEAM_ZIP: the downloaded teamengine zip, user may need to update this to the latest version
# TEAM_DIR: the teamengine folder
# CTL_URL: the test script URL, user may need to change this when the latest release is available
# CTL_ZIP: the downloaded test suite zip, user may need to change this to the latest version
# CTL_DIR: the test script folder
# LOG_DIR: the log folder
  DIR_NAME=$(dirname $0)
  TEAM_URL=http://sourceforge.net/projects/teamengine/files/latest/download
  TEAM_ZIP=$DIR_NAME/teamengine2.zip 
  TEAM_DIR=$DIR_NAME/teamengine2.0 
  TEAM_BIN_TEST=$DIR_NAME/teamengine2.0
  CTL_WCS2_URL=http://localhost/ogc/ctl/wcs2.zip
  CTL_WCS2_ZIP=$DIR_NAME/wcs2.zip
  CTL_WCS2_DIR=$DIR_NAME/wcs2
  LOG_DIR=$DIR_NAME  
  LOG=$LOG_DIR/log

  CODE_OK=0
  CODE_FAIL=255
  
  echo   "Test by: " $PROG " at "`date`|tee $LOG
#---------------------------Precondition------------------------------------------
# check the wget
whereis wget
if [ $? -ne 0 ]
then
   echo $PROG "Error: can not find " $WGET|tee -a $LOG
   exit $CODE_FAIL
fi

# check the unzip
whereis unzip
if [ $? -ne 0 ]
then
   echo $PROG "Error: can not find " $UNZIP|tee -a $LOG 
   exit $CODE_FAIL
fi

#--------------------------initiation--------------------------------------------
# download teamengine and test suites
$WGET $TEAM_URL -O $TEAM_ZIP| tee -a $LOG
$UNZIP -o $TEAM_ZIP -d $TEAM_DIR| tee -a $LOG

# find the teamengine test script
TEAM_BIN_TEST=`find $TEAM_DIR -name test.sh`
# change TEAM to excute mode
$CHMOD -R +x $TEAM_DIR/*| tee -a $LOG

# download wcs2 test suites
$WGET $CTL_WCS2_URL -O $CTL_WCS2_ZIP| tee -a $LOG
$UNZIP -o $CTL_WCS2_ZIP -d $CTL_WCS2_DIR| tee -a $LOG

#==========================test==================================================
# test by wcs2 ctl   
 $TEAM_BIN_TEST -source=$CTL_WCS2_DIR/wcs2/ctl| tee -a $LOG

#==========================clean==================================================
# test clean
  rm $TEAM_ZIP $CTL_WCS2_ZIP
  rm -rf $TEAM_DIR $CTL_WCS2_DIR
