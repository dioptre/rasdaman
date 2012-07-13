#!/bin/bash
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
#  test.sh
# Description
#  Command-line utility for testing WCPS functionality in petascope.
#
# PRECONDITIONS
#   Postgres, Rasdaman installed and running
#
# Usage: ./test_wcps.sh
#
# CHANGE HISTORY
#       2012-Jun-14     DM         created
#

PROG=`basename $0`

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ] ; do SOURCE="$(readlink "$SOURCE")"; done
SCRIPT_DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

. "$SCRIPT_DIR"/test.cfg

#
# constants
#
TESTDATA_PATH="$SCRIPT_DIR/testdata"
[ -d "$TESTDATA_PATH" ] || error "Testdata directory not found: $TESTDATA_PATH"
QUERIES_PATH="$SCRIPT_DIR/queries"
[ -d "$QUERIES_PATH" ] || error "Queries directory not found: $QUERIES_PATH"
ORACLE_PATH="$SCRIPT_DIR/oracle"
[ -d "$ORACLE_PATH" ] || error "Oracle directory not found: $ORACLE_PATH"
OUTPUT_PATH="$SCRIPT_DIR/output"
mkdir -p "$OUTPUT_PATH"

#
# variables
#
COLLS="rgb mr"
failed=0
total=0

#
# cleanup stuff
#
function cleanup()
{
  loge "--------------------------------------------------------"
  loge
  if [ $total -ne 0 ]; then
    log "  PASSED:" $(($total - $failed))
    log "  FAILED: $failed"
    loge "          ------------"
    log "   TOTAL: $total"
    loge ""
  fi

  log "cleanup..."
  #raserase_colls
  [ $failed -eq 0 ]
  exit
}

# trap keyboard interrupt (control-c)
trap cleanup SIGINT


# ------------------------------------------------------------------------------
# work
# ------------------------------------------------------------------------------

#
# checks
#
check_petascope
check_postgres
check_rasdaman
check_wget
check_gdal
check_netcdf

check_collection rgb
if [ $? -ne 0 ]; then
  raserase_colls

  $RASIMPORT -f "$TESTDATA_PATH"/rgb.png -coll rgb -t RGBImage:RGBSet || error "failed importing testdata"
  $RASIMPORT -f "$TESTDATA_PATH"/mr_1.png -coll mr || error "failed importing testdata"
fi

mkdir -p "$OUTPUT_PATH"
pushd "$QUERIES_PATH" > /dev/null

for f in *.test; do

  [ -f "$f" ] || continue
  
  # test single file
  #[ "$f" == "21-just_exponent.test" ] || continue

  # test header
  loge ""
  loge "--------------------------------------------------------"
  log "running test: $f"
  loge
  cat "$f"
  loge
  
  # expected result
  f_exp="$ORACLE_PATH/$f.out.gz"
  
  # URL encode query
  f_enc=`cat $f | xxd -plain | tr -d '\n' | sed 's/\(..\)/%\1/g'`

  # send to petascope
  f_out="$OUTPUT_PATH/$f.out"
  time $WGET -q --post-data "query=$f_enc" $WCPS_URL -O "$f_out"

  loge
  
  # check file type
  file "$f_out" | grep "ASCII" > /dev/null
  if [ $? -ne 0 ]; then
    # it's an image so we convert to text with gdal/ncdump
    mv "$f_out" "$f_out.tmp"
    gdal_translate -of netCDF "$f_out.tmp" "$f_out" > /dev/null 2>&1
    
    # convert to netcdf cdl, removing lines that will certainly be different
    ncdump "$f_out" | sed '/:GDAL =/d' | sed '/:history =/d' > "$f_out.tmp"
    mv "$f_out.tmp" "$f_out"
  fi

  if [ ! -f "$f_exp" ]; then
    log "can not compare result, expected file not found: $f_exp"
    failed=$(($failed + 1))
    log " ->  QUERY FAILED"
  else
    # compare
    gunzip -c "$f_exp" | cmp "$f_out" > /dev/null
    if [ $? -ne 0 ]; then
      # comparison failed
      failed=$(($failed + 1))
      log " ->  QUERY FAILED"
    else
      # comparison ok
      log " ->  QUERY PASSED"
    fi
  fi
  
  total=$(($total + 1))

done

popd > /dev/null

cleanup
