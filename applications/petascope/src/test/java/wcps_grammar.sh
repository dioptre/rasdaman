#! /bin/bash
# Run the grammar CLI tool 

if [ $# != 1 ]; then
	echo "WCPS Abstract Syntax Grammar Test Tool."
	echo "Parses an abstract syntax query and transforms it into XML for WCPS. "
	echo "Third parameter (String) is the query. "
	echo ""
	echo "Usage: ./grammar queryString"
	echo "For example: ./grammar \"for c in (nir) return encode(c,\"png\") "
	exit 1
fi

CLASSPATH=.:../lib/wcps-all.jar:../lib/jaxb-api.jar:../lib/jaxb-impl.jar:../lib/postgresql-8.3-604.jdbc4.jar:../lib/jsr173_api.jar:../lib/rasj.jar:../lib/commons-math-1.1.jar:../lib/antlrworks-1.2.3.jar

java -cp $CLASSPATH wcps.server.cli.grammar "$1"
