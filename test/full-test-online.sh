#! /bin/bash
# Command-line utility for testing PetaScope WCPS
# Usage: ./full-test-online.sh [URL] [testFolder] [outputFolder]
# Parameters: 
# * URL: the URL of the PetaScope implementation. Defaults to the new implementation on kahlua
# * testFolder: the folder that contains abstract syntax query test cases (extension ".test")
# * outputFolder: the folder where the script should save intermediate and output files. 
# 
# WARNING: if you use relative paths for the folders, they should be relative to the project folder, not to the test folder !

CLASSPATH=.:lib/wcps-all.jar:lib/jaxb-api.jar:lib/jaxb-impl.jar:lib/postgresql-8.3-604.jdbc4.jar:lib/jsr173_api.jar:lib/rasj.jar:lib/commons-math-1.1.jar:lib/antlrworks-1.2.3.jar:lib/commons-io-1.3.2.jar

cd .. && make jar && java -cp $CLASSPATH wcps.server.test.FullTestsOnline $1 $2 $3
