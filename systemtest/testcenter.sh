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
#
#
##################################################################

TESTER=$USER
CLIENT=$HOSTNAME
SERVER=$HOSTNAME
BASENAME=RASBASE
USERNAME=rasguest
USERPASSWD=rasguest
TRANSFER=Array
TRANSFERPARAMS=""
STORAGE=Array
STORAGEPARAMS=""
TILING=""
TILINGPARAMS=""
LOGFILE=testcenter.log
TESTINTERACTIVE="off"
DIFFINTERACTIVE="on"
UPDATEINTERACTIVE="on"
RESETDB="on"
SCRIPTS="off"
QUERIES="on"
TESTDIR="testdata"
returnValue="EXECUTION OK"
DEBUG="off"

if (test -n "$1"); then
 if (test -d $1 -o -f $1); then
   TESTDIR=$1
 else
   SERVER=$1
   if (test -n "$2"); then
     TESTDIR=$2
   fi
 fi
fi

showOptions()
{
    echo "===================================================================="
    echo "                TEST CENTER OPTIONS:"
    echo "===================================================================="
    echo "  Date...........................: " `date`
    echo "  (T)ester.......................: " $TESTER
    echo "  (C)lient name..................: " $CLIENT
    echo "  (S)erver name..................: " $SERVER
    echo "  (B)ase name....................: " $BASENAME
    echo "  (U)ser name....................: " $USERNAME
    echo "  User (p)assword................: " $USERPASSWD
    echo "  T(r)ansfer format..............: " $TRANSFER
    echo "  Tra(n)sfer format params.......: " $TRANSFERPARAMS
    echo "  Stora(g)e format...............: " $STORAGE
    echo "  Stor(a)ge format params........: " $STORAGEPARAMS
    echo "  Tiling sc(h)eme................: " $TILING
    echo "  Tiling sche(m)e params.........: " $TILINGPARAMS
    echo "  (L)og file.....................: " $LOGFILE
    echo "  (Q)uery dir....................: " $TESTDIR
    echo ""
    echo "  Tests (i)nteractive............: " $TESTINTERACTIVE
    echo "  Show dif(f) interactive........: " $DIFFINTERACTIVE
    echo "  Update testbe(d) interactive...: " $UPDATEINTERACTIVE
    echo "  R(e)set test database..........: " $RESETDB
    echo "  Debug c(o)mmands...............: " $DEBUG
    echo ""
    echo "  Phase (1) Test queries.........: " $QUERIES
    echo "  Phase (2) Test scripts.........: " $SCRIPTS
    echo ""
}

setParameters()
{
  option="*"

  while [[ $option != "" ]]; do

    clear
    showOptions
    
    echo "  \nEnter option to change setting or <return> to continue! \c"
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
    elif [[ $option = u ]]; then
      echo "  Enter new user name: \c"
      read USERNAME
      echo 
    elif [[ $option = p ]]; then
      echo "  Enter new user password: \c"
      read USERPASSWD
      echo 
    elif [[ $option = r ]]; then
      echo "  Enter new transfer format: \c"
      read TRANSFER
      echo 
    elif [[ $option = n ]]; then
      echo "  Enter new transfer format params: \c"
      read TRANSFERPARAMS
      echo 
    elif [[ $option = g ]]; then
      echo "  Enter new storage format: \c"
      read STORAGE
      echo 
    elif [[ $option = a ]]; then
      echo "  Enter new storage format params: \c"
      read STORAGEPARAMS
      echo 
    elif [[ $option = h ]]; then
      echo "  Enter new tiling scheme: \c"
      read TILING
      echo 
    elif [[ $option = m ]]; then
      echo "  Enter new tiling scheme params: \c"
      read TILINGPARAMS
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
      if [[ $TESTINTERACTIVE = "off" ]]; then
        TESTINTERACTIVE="on"
      else
        TESTINTERACTIVE="off"
      fi
    elif [[ $option = f ]]; then
      if [[ $DIFFINTERACTIVE = "off" ]]; then
        DIFFINTERACTIVE="on"
      else
        DIFFINTERACTIVE="off"
      fi
    elif [[ $option = d ]]; then
      if [[ $UPDATEINTERACTIVE = "off" ]]; then
        UPDATEINTERACTIVE="on"
      else
        UPDATEINTERACTIVE="off"
      fi
    elif [[ $option = o ]]; then
      if [[ $DEBUG = "off" ]]; then
        DEBUG="on"
      else
        DEBUG="off"
      fi
    elif [[ $option = e ]]; then
      if [[ $RESETDB = "off" ]]; then
        RESETDB="on"
      else
        RESETDB="off"
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


  echo $testCnt ". Test query: " $testFile "\c"

  if [[ $DEBUG = "on" ]]; then
     echo "shell command:"
     echo $query -file $testFile -testbed
  else

	  YesNo="y"
	  if [[ $TESTINTERACTIVE = "on" ]]; then
	    echo "  Test it (y/n) ? \c"
	    read YesNo
	  fi
	
	  if [[ $YesNo = "y" ]]; then
	    $query -file $testFile -testbed > testbedOutput
	    checkTestImage $testFile;
	  fi
  fi
}



testScript() 
{
  testFile=$1

  echo $testCnt ". Test script: " $testFile "\c"

  if [[ $DEBUG = "on" ]]; then
     echo "shell command:"
     echo $testFile $SERVER $BASENAME > testbedOutput
  else
	  YesNo="y"
	  if [[ $TESTINTERACTIVE = "on" ]]; then
	    echo "  Test it (y/n) ? \c"
	    read YesNo
	  fi

	  if [[ $YesNo = "y" ]]; then
	    $testFile $SERVER $BASENAME > testbedOutput
	    checkTestImage $testFile;
	  fi
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
    echo FAILED
    echo "  ERROR: verification file $verificationFile does not exist"

    YesNo="n"
    if [[ $DIFFINTERACTIVE = "on" ]]; then
	    echo "  Do you want to see the output (y/n) ? \c"
	    read YesNo__
    fi

    if [[ $YesNo = "y" ]]; then
      echo "\n\nI. TEST FILE"
      cat $testFile 
      echo "\n\nII. OUTPUT"
      cat testbedOutputInfo
      echo "\n\n"
    fi

    YesNo="n"
    if [[ $UPDATEINTERACTIVE = "on" ]]; then
	    echo "  Do you want to create a verification file (y/n) ? \c"
	    read YesNo
    fi
    if [[ $YesNo = "y" ]]; then
      cp testbedOutputInfo $verificationFile
      echo "  CREATED\n"
      returnValue=$returnValue" VERIFICATION FILE NOT FOUND, CREATED AS NEW"
      cvs add -m "testcenter verificationf file for query $testFile" $verificationFile
      cvs commit -m "testcenter verification file created as new by TESTER=$TESTER" $verificationFile
    else
      cp testbedOutputInfo $testFile".tmp"
      echo "  NOT CREATED\n"
      returnValue=$returnValue" VERIFICATION FILE NOT FOUND, NOT CREATED"
    fi

  else


    diff -c testbedOutputInfo $verificationFile 2>&1 1> /tmp/diff.out

    if [[ $? != 0 ]]; then
      echo "FAILED"
      returnValue="EXECUTION FAILED"

      YesNo="n"
      if [[ $DIFFINTERACTIVE = "on" ]]; then
	      echo "  Do you want to see the diff output (y/n) ? \c"
	      read YesNo
      fi


      if [[ $YesNo = "y" ]]; then
	      echo "\n\nI. TEST FILE"
	      cat $testFile 
	      echo "\n\n DIFF OUTPUT:"
	      cat /tmp/diff.out
	      echo "\n\n"
      fi

      YesNo="n"
      if [[ $UPDATEINTERACTIVE = "on" ]]; then
      	echo "  Do you want to update the verification file (y/n) ? \c"
	read YesNo
      fi
      if [[ $YesNo = "y" ]]; then
	if(cvs status $verificationFile); then
	   cvs edit $verificationFile
           cp testbedOutputInfo $verificationFile
           echo "  UPDATED\n"
           cvs commit -m "testcenter verification file updated by TESTER=$TESTER" $verificationFile
           returnValue=$returnValue" VERIFICATION FILE UPDATED"
	else
           cp testbedOutputInfo $verificationFile
           echo "  CREATED\n"
	   cvs add -m "testcenter verificationf file for query $testFile" $verificationFile
      	   cvs commit -m "testcenter verification file created as new by TESTER=$TESTER" $verificationFile
           returnValue=$returnValue" VERIFICATION FILE NOT IN REPOSITORY, CHECKED AS NEW"
	fi
      else
	cp testbedOutputInfo $testFile".tmp"
      fi

    else
      echo "OK"
    fi

  fi

  rm testbedOutput testbedOutputInfo /tmp/diff.out
}

checkBinaries()
{
 # test programs
 insert="$RMANBASE/bin/test_insert3"
 query="$RMANBASE/bin/test_query"
 insertppm="$RMANBASE/insertutils/insertppm"
 images="$RMANBASE/systemtest/images"

 if [[ ! -a $insertppm ]]; then
  echo "\nTest program $insertppm not found"
  echo "Please build the program and try again."
  exit 1 
 fi

 if [[ ! -a $insert ]]; then
  echo "\nTest program $insert not found"
  echo "Please build the program and try again."
  exit 1 
 fi

 if [[ ! -a $query ]]; then
  echo "\nTest program $query not found"
  echo "Please build the program and try again."
  exit 1 
 fi
}

setBinariesParams()
{
# server and database required
if(test -z "$SERVER"); then
  echo "Server name is not set!"
  exit 1
fi
if(test -z "$BASENAME"); then
  echo "Base name is not set!"
  exit 1
fi

if(test -z "$USERNAME"); then
  echo "User name is not set!"
  exit 1
fi

if(test -z "$USERPASSWD"); then
  echo "User password is not set!"
  exit 1
fi

#test_insert3 params
insert="$insert -server $SERVER -base $BASENAME -user $USERNAME -passwd $USERPASSWD"
if(test -n "$TRANSFER"); then
  insert="$insert -transferformat $TRANSFER"
fi
if(test -n "$TRANSFERPARAMS"); then
  insert="$insert -transferformatparams $TRANSFERPARAMS"
fi
if(test -n "$STORAGE"); then
  insert="$insert -storageformat $STORAGE"
fi
if(test -n "$STORAGEPARAMS"); then
  insert="$insert -storageformatparams $STORAGEPARAMS"
fi
if(test -n "$TILING"); then
  insert="$insert -tiling $TILING"
fi
if(test -n "$TILINGPARAMS"); then
  insert="$insert -tilingparams $TILINGPARAMS"
fi

#insertppm params
insertppm="$insertppm -server $SERVER -base $BASENAME -user $USERNAME -passwd $USERPASSWD"
if(test -n "$TRANSFER"); then
insertppm="$insertppm -transferformat $TRANSFER"
fi
if(test -n "$TRANSFERPARAMS"); then
insertppm="$insertppm -transferformatparams $TRANSFERPARAMS"
fi
if(test -n "$STORAGE"); then
insertppm="$insertppm -storageformat $STORAGE"
fi
if(test -n "$STORAGEPARAMS"); then
insertppm="$insertppm -storageformatparams $STORAGEPARAMS"
fi

#test_query params 
query="$query -server $SERVER -base $BASENAME -user $USERNAME -passwd $USERPASSWD"
if(test -n "$TRANSFER"); then
  query="$query -transferformat $TRANSFER"
fi
if(test -n "$TRANSFERPARAMS"); then
  query="$query -transferformatparams $TRANSFERPARAMS"
fi
if(test -n "$STORAGE"); then
  query="$query -storageformat $STORAGE"
fi
if(test -n "$STORAGEPARAMS"); then
  query="$query -storageformatparams $STORAGEPARAMS"
fi
if(test -n "$TILING"); then
  query="$query -tiling $TILING"
fi
if(test -n "$TILINGPARAMS"); then
  query="$query -tilingparams $TILINGPARAMS"
fi
}

resetLog()
{
YesNo="n"
if [[ $UPDATEINTERACTIVE = "on" ]]; then
 if [[ -a $LOGFILE ]]; then
  echo "Delete log file $LOGFILE (y/n) ? \c"
  read YesNo
  echo "\n"
 fi
fi
if [[ $YesNo = y ]]; then
    rm -f $LOGFILE
fi
}

resetDB()
{
if [[ $RESETDB = "on" ]]; then
   echo "Reset demo data in db (y/n) ? \c"
   read YesNo
   echo "\n"

   if [[ $YesNo = y ]]; then

	# delete collections
	echo "Delete test collections of $BASENAME...\c" ;
	echo "=============================================================================" >> $LOGFILE
	echo "Delete test collections of $BASENAME" >> $LOGFILE ;
	echo "=============================================================================\n" >> $LOGFILE

	if [[ $DEBUG = "on" ]]; then
		echo "shell command:"
		echo "10\n" $insert -setname ImgCharA     
		echo "10\n" $insert -setname ImgCharB     
		echo "10\n" $insert -setname ImgCharC     
		echo "10\n" $insert -setname ImgCharD     
		echo "10\n" $insert -setname ImgCharMask  
		echo "10\n" $insert -setname OvlCharA     
		echo "10\n" $insert -setname OvlCharB     	
		echo "10\n" $insert -setname OvlCharC     

		echo "10\n" $insert -setname ImgRGBA      
		echo "10\n" $insert -setname ImgULongC    
		echo "10\n" $insert -setname OlapCharA    
		echo "10\n" $insert -setname StringCharA  
		echo "10\n" $insert -setname CubeCharA    
		echo "10\n" $insert -setname OvlRGBA      
		echo "10\n" $insert -setname OvlRGBB      
		echo "10\n" $insert -setname OvlRGBC
	else
		echo "10\n" | $insert -setname ImgCharA     >> $LOGFILE ;
		echo "10\n" | $insert -setname ImgCharB     >> $LOGFILE ;
		echo "10\n" | $insert -setname ImgCharC     >> $LOGFILE ;
		echo "10\n" | $insert -setname ImgCharD     >> $LOGFILE ;
		echo "10\n" | $insert -setname ImgCharMask  >> $LOGFILE ;
		echo "10\n" | $insert -setname OvlCharA     >> $LOGFILE ;
		echo "10\n" | $insert -setname OvlCharB     >> $LOGFILE ;	
		echo "10\n" | $insert -setname OvlCharC     >> $LOGFILE ;

		echo "10\n" | $insert -setname ImgRGBA      >> $LOGFILE ;
		echo "10\n" | $insert -setname ImgULongC    >> $LOGFILE ;
		echo "10\n" | $insert -setname OlapCharA    >> $LOGFILE ;
		echo "10\n" | $insert -setname StringCharA  >> $LOGFILE ;
		echo "10\n" | $insert -setname CubeCharA    >> $LOGFILE ;
		echo "10\n" | $insert -setname OvlRGBA      >> $LOGFILE ;
		echo "10\n" | $insert -setname OvlRGBB      >> $LOGFILE ;
		echo "10\n" | $insert -setname OvlRGBC      >> $LOGFILE ;	
        fi


	echo "OK" ;

	echo "\n\nCreate demo data in db (y/n) ? \c"
	read YesNo
	echo "\n"

	if [[ $YesNo = y ]]; then
		  #create collections
		  echo "Insert test collections into $BASENAME...\c" ;
		  echo "=============================================================================" >> $LOGFILE
		  echo "Insert test collections into $BASENAME" >> $LOGFILE ;
		  echo "=============================================================================\n" >> $LOGFILE

		if [[ $DEBUG = "on" ]]; then	 
		  echo "shell command:"
		  echo "5\n   0\n 2\n 0\n 10\n 0\n 10\n" $insert -setname ImgCharA  
		  echo "5\n   1\n 2\n 0\n 10\n 0\n 10\n" $insert -setname ImgCharA  
	
		  echo "5\n  10\n 2\n 0\n 10\n 0\n 10\n" $insert -setname ImgCharB  
		  echo "5\n  20\n 2\n 0\n 10\n 0\n 10\n" $insert -setname ImgCharB  
	
		  echo "5\n 100\n 2\n 0\n 10\n 0\n 10\n" $insert -setname ImgCharC  
		  echo "5\n 200\n 2\n 0\n 10\n 0\n 10\n" $insert -setname ImgCharC  

		  echo "13\n      2\n 0\n 10\n 0\n 10\n" $insert -setname ImgCharD  

		  echo "5\n   2\n 2\n-2\n  2\n-1\n  1\n" $insert -setname ImgCharMask

		  echo "1\n       2\n 0\n 9\n  0\n  9\n" $insert -setname ImgULongC

		  echo "6\n       2\n 0\n 10\n 0\n 10\n" $insert -setname ImgRGBA   
		  echo "6\n       2\n 0\n 10\n 0\n 10\n" $insert -setname ImgRGBA   

		  echo "13\n      2\n 0\n 13\n 0\n  1\n" $insert -setname OlapCharA 

		  echo "13\n      1\n 0\n 99\n         " $insert -setname StringCharA -settype GreySet1 -mddtype GreyString 
		  echo "5\n   0\n 3\n 0\n 10\n 0\n 10\n 0\n 10\n" $insert -setname CubeCharA -settype GreySet3 -mddtype GreyCube 
			
		  echo $insertppm -collection OvlRGBA $images/ovl1.ppm 
		  echo $insertppm -collection OvlRGBB $images/ovl2.ppm 
		  echo $insertppm -collection OvlRGBC $images/ovl3.ppm 
		  echo $insertppm -grey -collection OvlCharA $images/ovl1.ppm  
		  echo $insertppm -grey -collection OvlCharB $images/ovl2.ppm 
		  echo $insertppm -grey -collection OvlCharC $images/ovl3.ppm 
		else
		  echo "5\n   0\n 2\n 0\n 10\n 0\n 10\n" | $insert -setname ImgCharA  >> $LOGFILE ;
		  echo "5\n   1\n 2\n 0\n 10\n 0\n 10\n" | $insert -setname ImgCharA  >> $LOGFILE ;
	
		  echo "5\n  10\n 2\n 0\n 10\n 0\n 10\n" | $insert -setname ImgCharB  >> $LOGFILE ;
		  echo "5\n  20\n 2\n 0\n 10\n 0\n 10\n" | $insert -setname ImgCharB  >> $LOGFILE ;
	
		  echo "5\n 100\n 2\n 0\n 10\n 0\n 10\n" | $insert -setname ImgCharC  >> $LOGFILE ;
		  echo "5\n 200\n 2\n 0\n 10\n 0\n 10\n" | $insert -setname ImgCharC  >> $LOGFILE ;

		  echo "13\n      2\n 0\n 10\n 0\n 10\n" | $insert -setname ImgCharD  >> $LOGFILE ;

		  echo "5\n   2\n 2\n-2\n  2\n-1\n  1\n" | $insert -setname ImgCharMask >> $LOGFILE;

		  echo "1\n       2\n 0\n 9\n  0\n  9\n" | $insert -setname ImgULongC >> $LOGFILE;

		  echo "6\n       2\n 0\n 10\n 0\n 10\n" | $insert -setname ImgRGBA   >> $LOGFILE ;
		  echo "6\n       2\n 0\n 10\n 0\n 10\n" | $insert -setname ImgRGBA   >> $LOGFILE ;

		  echo "13\n      2\n 0\n 13\n 0\n  1\n" | $insert -setname OlapCharA >> $LOGFILE ;

		  echo "13\n      1\n 0\n 99\n         " | $insert -setname StringCharA -settype GreySet1 -mddtype GreyString >> $LOGFILE ;
		  echo "5\n   0\n 3\n 0\n 10\n 0\n 10\n 0\n 10\n" | $insert -setname CubeCharA -settype GreySet3 -mddtype GreyCube >> $LOGFILE ;
			
		  $insertppm -collection OvlRGBA $images/ovl1.ppm >> $LOGFILE
		  $insertppm -collection OvlRGBB $images/ovl2.ppm >> $LOGFILE
		  $insertppm -collection OvlRGBC $images/ovl3.ppm >> $LOGFILE
		  $insertppm -grey -collection OvlCharA $images/ovl1.ppm >> $LOGFILE 
		  $insertppm -grey -collection OvlCharB $images/ovl2.ppm >> $LOGFILE
		  $insertppm -grey -collection OvlCharC $images/ovl3.ppm >> $LOGFILE
		fi

		echo "OK" ;
  	fi
   fi
fi
}

#
# main
#

checkBinaries;

setParameters;

setBinariesParams;

resetLog;

showOptions >> $LOGFILE

resetDB;

#
# test query files
#

if [[ $QUERIES = "on" ]]; then

  echo "\nPhase 1: Testing Query Files\n"
  echo "\n=============================================================================" >> $LOGFILE
  echo "Phase 1: Testing Query Files..." >> $LOGFILE ;
  echo "=============================================================================\n" >> $LOGFILE

  testCnt=1
  queryFiles=`find $TESTDIR -name '*.ql' | sort`

  for queryFile in $queryFiles; do

    testQuery $queryFile

    echo "Query " $testCnt $queryFile":" $returnValue >> $LOGFILE

    testCnt=$(($testCnt+1))
  done
fi


#
# test scripts
#

if [[ $SCRIPTS = "on" ]]; then
  echo "\nPhase 2: Testing Scripts\n"
  echo "\n=============================================================================" >> $LOGFILE
  echo "Phase 2: Testing Scripts" >> $LOGFILE
  echo "=============================================================================\n" >> $LOGFILE

  testCnt=1
  scriptFiles=`find scripts -name '*.sh' | sort`

  for scriptFile in $scriptFiles; do

    testScript $scriptFile

    echo "Script " $testCnt $scriptFile":" $returnValue >> $LOGFILE

    testCnt=$(($testCnt+1))
  done
fi

echo "\nTesting finnished at " `date`
echo "\n===================================================================" >> $LOGFILE
echo "Test finnished at " `date` >> $LOGFILE
echo "===================================================================\n" >> $LOGFILE


YesNo="n"
if [[ $UPDATEINTERACTIVE = "on" ]]; then
  echo "\nDo you want to check in the log file (y/n) ? \c"
  read YesNo
  echo "\n\n"
fi
if [[ $YesNo = y ]]; then
  cvs edit session.log
  cp $LOGFILE session.log
  cvs commit -m "updated by testcenter runned by TESTER=$TESTER" session.log
fi
exit 0
