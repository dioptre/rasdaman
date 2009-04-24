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
#		
# COMMENTS: needs class examples.TestQuery
# This facility is not necessary because you can start manualy the server
#
##################################################################

TESTER=$USER
CLIENT=$HOSTNAME
SERVER=$HOSTNAME
BASENAME=RASBASE
TESTDATE=$(date +"%d.%m.%Y_%H:%M:%S")
LOGFILE=testjava.log
LOGRCS=javasession.log
INTERACTIVE="off"
SCRIPTS="off"
QUERIES="on"
PRESERVERESULTS="off"
DEBUGCOMMANDS="off"
PROTOCOL=$RMANPROTOCOL

if [[ $1 = "" ]]; then
  TESTDIR="testdata"
else
  TESTDIR=$1
fi

showOptions()
{
    clear
    echo "===================================================================="
    echo "                TEST JAVA OPTIONS:"
    echo "===================================================================="
    echo "  Date...........................: " `date`
    echo "  (T)ester...............: " $TESTER
    echo "  (C)lient name..........: " $CLIENT
    echo "  (S)erver name..........: " $SERVER
    echo "  (B)ase name............: " $BASENAME
    echo "  (L)og file.............: " $LOGFILE
    echo "  (Q)uery dir............: " $TESTDIR
    echo ""
    echo "  (I)nteractive..........: " $INTERACTIVE
    echo "  (P)reserve results.....: " $PRESERVERESULTS
    echo "  (D)ebug commands.......: " $DEBUGCOMMANDS
    echo ""
    echo "  Phase (1) Test Queries.: " $QUERIES
    echo "  Phase (2) Test Scripts.: " $SCRIPTS
    echo ""
    echo " Protocol................: " $PROTOCOL
}
setParameters()
{
  option="*"

  while [[ $option != "" ]]; do

    showOptions

    echo "  \nEnter [tcsblqipd12] to change setting or <return> to continue! \c"
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
    elif [[ $option = p ]]; then
      if [[ $PRESERVERESULTS = "off" ]]; then
        PRESERVERESULTS="on"
      else
        PRESERVERESULTS="off"
      fi
    elif [[ $option = d ]]; then
      if [[ $DEBUGCOMMANDS = "off" ]]; then
        DEBUGCOMMANDS="on"
      else
        DEBUGCOMMANDS="off"
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

  YesNo="y"
  if [[ $INTERACTIVE = "on" ]]; then
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

  echo $testCnt ". Test script: " $testFile "\c"

  YesNo="y"
  if [[ $INTERACTIVE = "on" ]]; then
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
    echo FAILED
    echo "  ERROR: verification file $verificationFile does not exist"

    echo "  Do you want to see the output (y/n) ? \c"
    read YesNo
    echo "\n"

    if [[ $YesNo = y ]]; then
      echo "I. TEST FILE"
      cat $testFile 
      echo "\n\nII. OUTPUT"
      cat testbedOutputInfo
      echo "\n\n"
    fi

    echo "  Do you want to create a verification file (y/n) ? \c"
    read YesNo
    echo "\n"

    if [[ $YesNo = y ]]; then
      cp testbedOutputInfo $verificationFile
      echo "  CREATED\n"
      returnValue="VERIFICATION FILE CREATED"
    else
      echo "  NOT CREATED\n"
      returnValue="VERIFICATION FILE NOT FOUND"
    fi

  else

    diff testbedOutputInfo $verificationFile

    if [[ $? = 1 ]]; then
      echo "FAILED"
      echo "\nProgram output:"
      cat testbedOutput
      echo "\n\n"
      returnValue="EXECUTION FAILED"

      echo "  Do you want to update the verification file (y/n) ? \c"
      read YesNo
      echo "\n"

      if [[ $YesNo = y ]]; then
        co -l $verificationFile
        cp testbedOutputInfo $verificationFile
        echo "  UPDATED\n"
        ci -u -m"testcenter update" $verificationFile
        returnValue="VERIFICATION FILE UPDATED"
      fi

    else
      echo "OK"
    fi

  fi

  rm testbedOutput testbedOutputInfo
}



#
# main
#

# test programs
query="$RMANBASE/java/examples/TestQuery.class"
images="$RMANBASE/systemtest/images"

if [[ ! -a $query ]]; then
  query="$RMANBASE/java/classes/examples/TestQuery.class"
  if [[ ! -a $query ]]; then
    echo "\nTest program $query not found"
    echo "Please build the program and try again."
    return
  fi 
fi

setParameters;
query="$query -s $SERVER -d $BASENAME"
YesNo=""

if [[ -f $LOGFILE ]]; then
  echo "\nDelete log file $LOGFILE (y/n) ? \c"
  read YesNo
  echo "\n\n"
  if [[ $YesNo = y ]]; then
    rm $LOGFILE
  fi
fi


echo "----------------------------------JAVA TEST SESSION-------------------------" >> $LOGFILE
echo "- Date........ : " $TESTDATE >> $LOGFILE
echo "- Tester.......: " $TESTER >> $LOGFILE
echo "- Directory....: " $PWD/$TESTDIR >> $LOGFILE
echo "- Client name..: " $CLIENT >> $LOGFILE
echo "- Server name..: " $SERVER >> $LOGFILE
echo "- Base name....: " $BASENAME >> $LOGFILE
echo "-------------------------------------START----------------------------------" >> $LOGFILE



#
# test query files
#

if [[ $QUERIES = "on" ]]; then
  echo "\nPhase 1: Testing Query Files\n"
  
  if [[ $PRESERVERESULTS = "on" ]]; then
    if [[ $DEBUGCOMMANDS = "on" ]]; then
     echo java -Xms100m -Xmx200m -DRMANPROTOCOL=$PROTOCOL -classpath $CLASSPATH examples.TestQuery -server $SERVER -database $BASENAME -testdirectory $TESTDIR -logfile $LOGFILE -preserveresult
    else 
     java -Xms100m -Xmx200m -DRMANPROTOCOL=$PROTOCOL -classpath $CLASSPATH examples.TestQuery -server $SERVER -database $BASENAME -testdirectory $TESTDIR -logfile $LOGFILE -preserveresult
    fi
  else
    if [[ $DEBUGCOMMANDS = "on" ]]; then
     echo java -Xms100m -Xmx200m -DRMANPROTOCOL=$PROTOCOL -classpath $CLASSPATH examples.TestQuery -server $SERVER -database $BASENAME -testdirectory $TESTDIR -logfile $LOGFILE
    else 
     java -Xms100m -Xmx200m -DRMANPROTOCOL=$PROTOCOL -classpath $CLASSPATH examples.TestQuery -server $SERVER -database $BASENAME -testdirectory $TESTDIR -logfile $LOGFILE
    fi
  fi
  
fi


#
# test scripts
#

if [[ $SCRIPTS = "on" ]]; then
  echo "\nPhase 2: Testing Scripts\n"

  testCnt=1
  scriptFiles=`find scripts -name '*.sh' | sort`

  for scriptFile in $scriptFiles; do

    testScript $scriptFile

    echo "Script " $testCnt $scriptFile":" $returnValue >> $LOGFILE

    testCnt=$(($testCnt+1))
  done
fi

echo "\n-------------------------------------END------------------------------------" >> $LOGFILE
echo "\n\n" >> $LOGFILE


echo "\nTesting finnished.\n"
