#!/bin/sh 
# config
# the path where java should be invoked
cd ../src
# where the test cases are located
TESTPATH=../testing/testcases
export CLASSPATH=.:wcps.jar:../lib/jaxb-api.jar:../lib/jaxb-impl.jar:../lib/postgresql-8.3-604.jdbc4.jar:../lib/jsr173_api.jar:../lib/rasj.jar:../lib/commons-math-1.1.jar
JAVA=/usr/jdk1.6.0_05/bin/java 

function fail {
    echo "Error: $1"
    echo "Test $2 failed"
    echo "TESTS FAILED"
    exit 1
}

function runTest {
    test=$1
    cp $TESTPATH/$test request.wcps || faile "Cannot copy test file" $test
    rm wcpsRequest.xml
    $JAVA syntaxParser.WCPSRequestProcessor || fail "Parser Error" $test
    diff -w wcpsRequest.xml $TESTPATH/$test.result || fail "Different XML results" $test
    $JAVA wcps.server.cli.WCPService wcpsRequest.xml 1>request.rasql || fail "WCPS to RasQL translation failed" $test
    if [ -f $TESTPATH/$test.rasql ] ; then
        diff -w request.rasql $TESTPATH/$test.rasql || fail "Different RasQL result" $test
    else
        WARNINGS+="Test $test is missing the rasql result file. Skipping.\n"
    fi
    echo $test passed
    RESULT+="$test\n"
}

RESULT=
WARNINGS=
if [ $# -eq 1 ] ; then
    runTest $1
    
else
    for test in `ls $TESTPATH | grep test\$` ; do
        echo $test 
        runTest $test
    done
fi

echo -e "Test passed:\n$RESULT"
echo "Tests PASSED"
echo -e "Warnings:\n$WARNINGS"         
