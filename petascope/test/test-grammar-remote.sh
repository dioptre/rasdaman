#! /bin/bash

CLASSPATH=.:lib/wcps-all.jar:lib/jaxb-api.jar:lib/jaxb-impl.jar:lib/postgresql-8.3-604.jdbc4.jar:lib/jsr173_api.jar:lib/rasj.jar:lib/commons-math-1.1.jar:lib/antlrworks-1.2.3.jar:lib/commons-io-1.3.2.jar

cd .. && make jar && java -cp $CLASSPATH wcps.server.test.GrammarTestOnline
