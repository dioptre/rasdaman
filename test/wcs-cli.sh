#!/bin/bash

# CLASSPATH=.:wcps.jar:../lib/jaxb-api.jar:../lib/jaxb-impl.jar:../lib/postgresql-8.3-604.jdbc4.jar:../lib/jsr173_api.jar:../lib/rasj.jar:../lib/commons-math-1.1.jar

CLASSPATH=.:../lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar:../lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar:../lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar:../lib/postgresql-8.3-604.jdbc4.jar


cd src && java -cp $CLASSPATH wcs.server.cli.CLI
