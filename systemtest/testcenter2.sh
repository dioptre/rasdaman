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
######################### testcenter.sh header ###################
#
# testcenter.sh FOR:
#   Release of RASDAMAN
#
# COMMENTS:
# args needed: serverhost databasehost logdirectory testjava queries scripts checkin conversion compression 
# last six are "on"/"off"-switches.
#
##################################################################

TESTER=$USER
BASENAME=RASBASE
TIMESTAMP=`date +%d%b%y%H%M%S`
LOGFILE="$3/$HOSTNAME.$TIMESTAMP.log"
SEEOUTPUT="off"
CREATEVFILE="off"
UPDATEVFILE="off"
INTERACTIVE="off"
DEMODATA="on"
CREATEDATA="on"
TESTJAVA=$4
QUERIES=$5
SCRIPTS=$6
CHECKINLOG=$7
CONV=$8
COMP=$9
TESTDIR=testdata

echo "$0 $1 $2 $3 $4 $5 $6 $7 $8 $9" 

if [[ $1 = "" || $2 = "" ]]; then
  SERVER=$HOSTNAME
  CLIENT=$HOSTNAME
else
  SERVER=$1
  CLIENT=$2
fi

if [[ $7 = "" ]]; then
  CHECKINLOG="off"
else
  CHECKINLOG=$7
fi

if [[ $6 = "" ]]; then
  SCRIPTS="off"
else
  SCRIPTS=$6
fi

if [[ $5 = "" ]]; then
  QUERIES="off"
else
  QUERIES=$5
fi

if [[ $4 = "" ]]; then
  TESTJAVA="off"
else
  TESTJAVA=$4
fi


setParameters()
{
  option="*"

  while [[ $option != "" ]]; do

    echo "\n"
    echo "  (T)ester...............: " $TESTER
    echo "  (C)lient name..........: " $CLIENT
    echo "  (S)erver name..........: " $SERVER
    echo "  (B)ase name............: " $BASENAME
    echo "  (L)og file.............: " $LOGFILE
    echo "  (Q)uery dir............: " $TESTDIR
    echo "  (I)nteractive..........: " $INTERACTIVE
    echo ""
    echo "  Phase (1) Test Queries.: " $QUERIES
    echo "  Phase (2) Test Scripts.: " $SCRIPTS

    echo "  \nEnter [tcsblqi12] to change setting or <return> to continue! \c"
    read option
    echo "\n" 

    if   [[ $option = t ]]; then
      echo "  Enter new name for tester: \c"
      read TESTER
      echo 
    elif [[ $option = c ]]; then
      echo "  Enter new client name: \c"
      read CLIENT
      echo 
    elif [[ $option = s ]]; then
      echo "  Enter new server name: \c"
      read SERVER
      echo 
    elif [[ $option = b ]]; then
      echo "  Enter new base name: \c"
      read BASENAME
      echo 
    elif [[ $option = l ]]; then
      echo "  Enter new name for the log file: \c"
      read LOGFILE
      echo 
    elif [[ $option = q ]]; then
      echo "  Enter new name for the query data directory: \c"
      read TESTDIR
      echo 
    elif [[ $option = i ]]; then
      if [[ $INTERACTIVE = "off" ]]; then
        INTERACTIVE="on"
      else
        INTERACTIVE="off"
      fi
    elif [[ $option = 1 ]]; then
      if [[ $QUERIES = "off" ]]; then
        QUERIES="on"
      else
        QUERIES="off"
      fi
    elif [[ $option = 2 ]]; then
      if [[ $SCRIPTS = "off" ]]; then
        SCRIPTS="on"
      else
        SCRIPTS="off"
      fi
    fi
  done
}



testQuery() 
{
  testFile=$1

  echo $testCnt ". Test query: " $testFile "\c" >> $LOGFILE 2>&1

  YesNo="y"
  if [[ $INTERACTIVE = "on" ]]; then
    echo $testCnt ". Test query: " $testFile "\c"
    echo "  Test it (y/n) ? \c"
    read YesNo
  fi

  if [[ $YesNo = "y" ]]; then
    $query $testFile -testbed > testbedOutput
    checkTestImage $testFile;
  fi
}


testScript() 
{
  testFile=$1

  echo $testCnt ". Test script: " $testFile "\c" >> $LOGFILE 2>&1

  YesNo="y"
  if [[ $INTERACTIVE = "on" ]]; then
    echo $testCnt ". Test script: " $testFile "\c"
    echo "  Test it (y/n) ? \c"
    read YesNo
  fi

  if [[ $YesNo = "y" ]]; then
    $testFile $SERVER $BASENAME > testbedOutput
    checkTestImage $testFile;
  fi
}


checkTestImage()
{
  returnValue="EXECUTION OK"

  # create info file
  gawk -f testOutputGen.awk testbedOutput > testbedOutputInfo

  # check for verification file
  verificationFile=$testFile".out"

  if [[ ! -a $verificationFile ]]; then
    echo FAILED >> $LOGFILE 2>&1
    echo "  ERROR: verification file $verificationFile does not exist" >> $LOGFILE 2>&1
    if [[ $SEEOUTPUT = "on" ]]; then
      echo "I. TEST FILE"    >> $LOGFILE 2>&1
      cat $testFile          >> $LOGFILE 2>&1
      echo "\n\nII. OUTPUT"  >> $LOGFILE 2>&1
      cat testbedOutputInfo  >> $LOGFILE 2>&1
      echo "\n\n"
    fi
    if [[ $CREATEVFILE = "on" ]]; then
      cp testbedOutputInfo $verificationFile
      echo "  CREATED\n"     >> $LOGFILE 2>&1
      returnValue="VERIFICATION FILE CREATED"
    else
      echo "  NOT CREATED\n" >> $LOGFILE 2>&1
      returnValue="VERIFICATION FILE NOT FOUND"
    fi
  else
    diff testbedOutputInfo $verificationFile >> $LOGFILE 2>&1
    if [[ $? = 1 ]]; then
      echo "FAILED"            >> $LOGFILE 2>&1
      echo "\nProgram output:" >> $LOGFILE 2>&1
      cat testbedOutput        >> $LOGFILE 2>&1
      echo "\n\n"              >> $LOGFILE 2>&1
      returnValue="EXECUTION FAILED"
      if [[ $UPDATEVFILE = "on" ]]; then
        co -l $verificationFile
        cp testbedOutputInfo $verificationFile
        echo "  UPDATED\n"     >> $LOGFILE 2>&1
        ci -u -m"testcenter update" $verificationFile
        returnValue="VERIFICATION FILE UPDATED"
      fi
    else
      echo "OK" >> $LOGFILE 2>&1
    fi
  fi
  rm testbedOutput testbedOutputInfo
}



#
# main
#

# test programs
insert="$RMANBASE/bin/test_insert3"
query="$RMANBASE/bin/test_query"
insertppm="$RMANBASE/insertutils/insertppm"
images="$RMANBASE/systemtest/images"
rasdltest="$RMANBASE/systemtest/rasdl/rasdl.sh"

echo "Test log file." > $LOGFILE 2>&1

if [[ ! -a $insert ]]; then
  echo "\nTest program $insertppm not found" >> $LOGFILE 2>&1
  echo "Please build the program and try again." >> $LOGFILE 2>&1
  return 
fi

if [[ ! -a $insert ]]; then
  echo "\nTest program $insert not found" >> $LOGFILE 2>&1
  echo "Please build the program and try again." >> $LOGFILE 2>&1
  return 
fi

if [[ ! -a $query ]]; then
  echo "\nTest program $query not found" >> $LOGFILE 2>&1
  echo "Please build the program and try again." >> $LOGFILE 2>&1
  return 
fi

if [[ ! -a $rasdltest ]]; then
  echo "\nTest program $rasdltest not found"  >> $LOGFILE 2>&1
  echo "Please build the program and try again."  >> $LOGFILE 2>&1
  return
fi 

#
# test rasdl
#
echo "\nPhase -1: Testing RasDL\n" >> $LOGFILE 2>&1
$rasdl >> $LOGFILE 2>&1
if [[ $? != 0 ]]; then
  echo "\nTest program $rasdltest failed"  >> $LOGFILE 2>&1  
  return
fi

insert="$insert $SERVER $BASENAME"
insertppm="$insertppm -server $SERVER -base $BASENAME"
query="$query $SERVER $BASENAME"
YesNo=""


if [[ $DEMODATA = "on" ]]; then
        echo "\n\nReset demo data in db: \n" >> $LOGFILE 2>&1

	# delete collections
	echo "Delete test collections of $BASENAME..." >> $LOGFILE 2>&1;
	echo "10\n" | $insert ImgCharA     >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert ImgCharB     >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert ImgCharC     >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert ImgCharD     >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert ImgCharMask  >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert OvlCharA     >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert OvlCharB     >> $LOGFILE 2>&1 ;	
	echo "10\n" | $insert OvlCharC     >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert ImgRGBA      >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert ImgULongC    >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert OlapCharA    >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert StringCharA  >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert CubeCharA    >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert OvlRGBA      >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert OvlRGBB      >> $LOGFILE 2>&1 ;
	echo "10\n" | $insert OvlRGBC      >> $LOGFILE 2>&1 ;

	if [[ $CREATEDATA = "on" ]]; then
        	  echo "\n\nCreate demo data in db: \n" >> $LOGFILE 2>&1
		  
		  #create collections
		  echo "Insert test collections into $BASENAME..." >> $LOGFILE 2>&1;
		  echo "5\n   0\n 2\n 0\n 10\n 0\n 10\n" | $insert ImgCharA  >> $LOGFILE 2>&1 ;
		  echo "5\n   1\n 2\n 0\n 10\n 0\n 10\n" | $insert ImgCharA  >> $LOGFILE 2>&1 ;
		  echo "5\n  10\n 2\n 0\n 10\n 0\n 10\n" | $insert ImgCharB  >> $LOGFILE 2>&1 ;
		  echo "5\n  20\n 2\n 0\n 10\n 0\n 10\n" | $insert ImgCharB  >> $LOGFILE 2>&1 ;
		  echo "5\n 100\n 2\n 0\n 10\n 0\n 10\n" | $insert ImgCharC  >> $LOGFILE 2>&1 ;
		  echo "5\n 200\n 2\n 0\n 10\n 0\n 10\n" | $insert ImgCharC  >> $LOGFILE 2>&1 ;
		  echo "13\n      2\n 0\n 10\n 0\n 10\n" | $insert ImgCharD  >> $LOGFILE 2>&1 ;
		  echo "5\n   2\n 2\n-2\n  2\n-1\n  1\n" | $insert ImgCharMask >> $LOGFILE 2>&1;
		  echo "1\n       2\n 0\n 9\n  0\n  9\n" | $insert ImgULongC >> $LOGFILE 2>&1;
		  echo "6\n       2\n 0\n 10\n 0\n 10\n" | $insert ImgRGBA   >> $LOGFILE 2>&1 ;
		  echo "6\n       2\n 0\n 10\n 0\n 10\n" | $insert ImgRGBA   >> $LOGFILE 2>&1 ;
		  echo "13\n      2\n 0\n 13\n 0\n  1\n" | $insert OlapCharA >> $LOGFILE 2>&1 ;
		  echo "13\n      1\n 0\n 99\n         " | $insert StringCharA -settype GreySet1 -mddtype GreyString >> $LOGFILE 2>&1 ;
		  echo "5\n   0\n 3\n 0\n 10\n 0\n 10\n 0\n 10\n" | $insert CubeCharA -settype GreySet3 -mddtype GreyCube >> $LOGFILE 2>&1 ;

		  $insertppm -collection OvlRGBA $images/ovl1.ppm >> $LOGFILE 2>&1
		  $insertppm -collection OvlRGBB $images/ovl2.ppm >> $LOGFILE 2>&1
		  $insertppm -collection OvlRGBC $images/ovl3.ppm >> $LOGFILE 2>&1
		  $insertppm -grey -collection OvlCharA $images/ovl1.ppm >> $LOGFILE 2>&1 
		  $insertppm -grey -collection OvlCharB $images/ovl2.ppm >> $LOGFILE 2>&1
		  $insertppm -grey -collection OvlCharC $images/ovl3.ppm >> $LOGFILE 2>&1
  	fi
fi

#
# print some summary information
#
echo "--------------------------------------------------------------TEST SESSION--" >> $LOGFILE 2>&1
echo "- Date........ : " `date`                                                     >> $LOGFILE 2>&1
echo "- Tester.......: " $TESTER                                                    >> $LOGFILE 2>&1
echo "- Directory....: " $PWD/$TESTDIR                                              >> $LOGFILE 2>&1
echo "- Client name..: " $CLIENT                                                    >> $LOGFILE 2>&1
echo "- Server name..: " $SERVER                                                    >> $LOGFILE 2>&1
echo "- Base name....: " $BASENAME                                                  >> $LOGFILE 2>&1
echo "- Time stamp...: " $TIMESTAMP                                                 >> $LOGFILE 2>&1
echo "----------------------------------------------------------------------START-" >> $LOGFILE 2>&1


#
# test java
#
if [[ $TESTJAVA = "on" ]]; then
  javaquery="$RMANBASE/java/examples/TestQuery.class"
  if [[ ! -a $javaquery ]]; then
    echo "\nTest program $javaquery not found"
    echo "Please build the program and try again."
    return
  fi 
  echo "\nPhase 0: Testing Java Query Files\n" >> $LOGFILE 2>&1
  java -classpath $CLASSPATH:/usr/local/dist/dir/java/:$RMANBASE/java/:$RMANBASE/java/classes:$RMANHOME/java/:$RMANHOME/java/classes examples.TestQuery -server $SERVER -database $BASENAME -testdirectory /home/staff/rastest/testdata -logfile testjava.log
  cat testjava.log >> $LOGFILE
fi

#
# test query files
#
if [[ $QUERIES = "on" ]]; then
  echo "\nPhase 1: Testing Query Files\n" >> $LOGFILE 2>&1
  testCnt=1
  queryFiles=`find $TESTDIR -name '*.ql' | sort`
  for queryFile in $queryFiles; do
    testQuery $queryFile
    echo "Query " $testCnt $queryFile":" $returnValue
    testCnt=$(($testCnt+1))
  done
fi

#
# test scripts
#
if [[ $SCRIPTS = "on" ]]; then
  echo "\nPhase 2: Testing Scripts\n" >> $LOGFILE 2>&1
  testCnt=1
  scriptFiles=`find scripts -name '*.sh' | sort`
  for scriptFile in $scriptFiles; do
    testScript $scriptFile
    echo "Script " $testCnt $scriptFile":" $returnValue
    testCnt=$(($testCnt+1))
  done
fi

#
# test conversion
#
if [[ $CONV = "on" ]]; then
  echo "\nPhase 3: Testing Conversion\n" >> $LOGFILE 2>&1
  cdir=$PWD
  cd $RMANHOME/rasodmg/test
  (echo "v"; echo "e") | ./test_comp_conv.sh
  cd cdir
fi

#
# test compresion
#
if [[ $COMP = "on" ]]; then
  echo "\nPhase 4: Testing Compression\n" >> $LOGFILE 2>&1
  cdir=$PWD
  cd $RMANHOME/rasodmg/test
  (echo "p"; echo "e") | ./test_comp_conv.sh
  cd cdir
fi

echo "\n\n" >> $LOGFILE 2>&1

#
# check in log file
#

if [[ $CHECKINLOG = "on" ]]; then
  co -l session.log
  cp $LOGFILE session.log
  ci -u session.log
fi

echo "Testing finnished."
