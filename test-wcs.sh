#! /bin/bash

LIBS1=lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar:lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar:lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar:lib/commons-io-1.3.2.jar
LIBS2=.:/home/andrei/git_repo/wcps/src/wcs/server/servlet:lib/junit-4.5.jar:lib/jsr173_1.0_api.jar:lib/rasj.jar:lib/servlet-2_5-api.jar:lib/commons-fileupload-1.2.jar:lib/commons-math-1.1.jar

LIBS_SRC1=../lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar:../lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar:../lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar:../lib/commons-io-1.3.2.jar
LIBS_SRC2=.:/home/andrei/git_repo/wcps/src/wcs/server/servlet:../lib/junit-4.5.jar:../lib/jsr173_1.0_api.jar:../lib/rasj.jar:../lib/servlet-2_5-api.jar:../lib/commons-fileupload-1.2.jar:../lib/commons-math-1.1.jar

CLASSPATH=$LIBS2:$LIBS1
CLASSPATH_SRC=$LIBS_SRC2:$LIBS_SRC1

# Compile the test
javac -g -Xlint -cp $CLASSPATH -sourcepath src/ src/wcs/server/servlet/WCSTest.java

# Run the test
# cd src && java -cp ../lib/junit-4.5.jar wcs.server.servlet.WCSTest
cd src && java -cp $CLASSPATH_SRC org.junit.runner.JUnitCore wcs.server.servlet.WCSTest 
