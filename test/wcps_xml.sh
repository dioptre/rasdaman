#! /bin/bash
# Run the xml CLI tool.
# Use as below: wcps_xml.sh testcases_wcps/1.test.xml
# Input: ProcessCoverages XML document (WCPS 1.0.0)
# Output: RasQL query (on the last line of output)

CLASSPATH=.:lib/wcps-all.jar:lib/jaxb-api.jar:lib/jaxb-impl.jar:lib/postgresql-8.3-604.jdbc4.jar:lib/jsr173_api.jar:lib/rasj.jar:lib/commons-math-1.1.jar:lib/antlrworks-1.2.3.jar

echo "Will run with file '$1'"

cd .. && java -cp $CLASSPATH wcps.server.cli.xml test/$1 
