 #
 # This file is part of %PACKAGENAME%.
 #
 # %PACKAGENAME% is free software: you can redistribute it and/or modify
 # it under the terms of the GNU Lesser General Public License as
 # published by the Free Software Foundation, either version 3 of
 # the License, or (at your option) any later version.
 #
 # %PACKAGENAME% is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU Lesser General Public License for more details.
 #
 # You should have received a copy of the GNU Lesser General Public
 # License along with %PACKAGENAME%. If not, see <http://www.gnu.org/licenses/>.
 #
 # For more information please see <http://www.%PACKAGENAME%.org>
 # or contact Peter Baumann via <baumann@rasdaman.com>.
 #
 # Copyright 2009 Jacobs University Bremen, Peter Baumann.
 #


NAME=petascope-wcps
NAME_WCS=petascope-wcs

WCS_LIBS=lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar:lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar:lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar:lib/commons-io-1.3.2.jar
CLASSPATH=.:lib/junit-4.5.jar:lib/jsr173_1.0_api.jar:lib/rasj.jar:lib/servlet-2_5-api.jar:lib/commons-fileupload-1.2.jar:lib/commons-math-1.1.jar:${WCS_LIBS}
JAVA_PATH=
ANTLR=${JAVA_PATH}java -cp lib/antlrworks-1.2.2.jar org.antlr.Tool
JAVAC=${JAVA_PATH}javac -g -Xlint -cp ${CLASSPATH} -sourcepath src/
XJC=${JAVA_PATH}java -jar lib/jaxb-xjc.jar
JAVA=${JAVA_PATH}java -cp ${CLASSPATH} 
JAVADOC=${JAVA_PATH}javadoc
## Typical build process.
.PHONY: most
most: clean core servlet war

## Complete build process.
.PHONY: all
all: clean core wcs cli servlet war wcs-war

## Regenerate the wcps.xml.processcoverage package.
.PHONY: gen
gen:
	${XJC} -d src -p wcps.xml.processcoverage xml/ogc/wcps/1.0.1/wcpsProcessCoverage.xsd
	${JAVAC} src/wcps/xml/processcoverage/*.java

## Generate translation core.
.PHONY: core
core:
	rm -rf lib/wcps.jar
	${JAVAC} src/wcps/server/core/*.java
	mkdir -p wcps/xml/processcoverage wcps/server/core
	cp src/wcps/server/core/*.class wcps/server/core/
	jar cf lib/wcps.jar wcps
	rm -rf wcps

## Compile the WCS code
.PHONY: wcs
wcs: 
	${XJC} -d src -p wcs.server.core xml/ogc/wcps/1.0.1/wcpsProcessCoverage.xsd
	${JAVAC} src/wcs/server/core/*.java
	${JAVAC} src/wcs/server/cli/*.java
	${JAVAC} src/wcs/server/servlet/*.java

# Packages the WCS Servlet in the .WAR container
.PHONY: wcs-war
wcs-war:
	rm -rf ${NAME_WCS}.war
	rm -rf WEB-INF
	mkdir -p WEB-INF/lib
	cp lib/*.jar WEB-INF/lib/
	rm WEB-INF/lib/servlet-2_5-api.jar
	mkdir -p WEB-INF/classes/wcps/server/servlet
	mkdir -p WEB-INF/classes/java_cup
	mkdir -p WEB-INF/classes/syntaxParser
	mkdir -p WEB-INF/classes/wcs/server/servlet
	mkdir -p WEB-INF/classes/wcs/server/core
	cp web-wcs.xml WEB-INF/web.xml
	cp src/wcps/server/servlet/*.class WEB-INF/classes/wcps/server/servlet/
	cp src/wcs/server/servlet/*.class WEB-INF/classes/wcs/server/servlet/
	cp src/wcs/server/core/*.class WEB-INF/classes/wcs/server/core/
	cp src/syntaxParser/*.class WEB-INF/classes/syntaxParser
	cp -R src/java_cup/* WEB-INF/classes/java_cup
	jar cf ${NAME_WCS}.war WEB-INF/ dbparams.properties xml/ogc/wcps/1.0.1/*.xsd xml/ogc/wcps/1.0.0/*.xsd misc/wcs-servlet-default.html


## Servlet interface.
.PHONY: servlet
servlet: syntaxParser
	${JAVAC} src/wcps/server/servlet/*.java
	${JAVAC} src/wcs/server/servlet/*.java

## Packages a servlet in a .WAR container.
.PHONY: war
war:
	rm -rf ${NAME}.war
	mkdir -p WEB-INF/lib
	cp lib/*.jar WEB-INF/lib/
	rm WEB-INF/lib/servlet-2_5-api.jar
	mkdir -p WEB-INF/classes/wcps/server/servlet
	mkdir -p WEB-INF/classes/java_cup
	mkdir -p WEB-INF/classes/syntaxParser
	mkdir -p WEB-INF/classes/wcs/server/servlet
	mkdir -p WEB-INF/classes/wcs/server/core
	cp web.xml WEB-INF/
	cp src/wcps/server/servlet/*.class WEB-INF/classes/wcps/server/servlet/
	cp src/wcs/server/servlet/*.class WEB-INF/classes/wcs/server/servlet/
	cp src/wcs/server/core/*.class WEB-INF/classes/wcs/server/core/
	cp src/syntaxParser/*.class WEB-INF/classes/syntaxParser
	cp -R src/java_cup/* WEB-INF/classes/java_cup
	jar cf ${NAME}.war WEB-INF/ dbparams.properties xml/ogc/wcps/1.0.1/*.xsd xml/ogc/wcps/1.0.0/*.xsd

## Translation core command-line interface. Useful for development and testing.
.PHONY: cli
cli:
	${JAVAC} src/wcps/server/cli/*.java
	${JAVAC} src/wcs/server/cli/*.java

.PHONY: syntaxParser
syntaxParser:
	$(MAKE) -C src/syntaxParser all

## Clean up.
.PHONY: clean
clean:
	-rm -rf src/wcps/xml src/wcps/server/core/*.class src/wcps/server/servlet/*.class src/wcps/server/cli/*.class lib/wcps.jar ${NAME}.war WEB-INF src/wcs/server/core/*.class src/wcs/server/cli/*.class src/wcs/server/servlet/*.class ${NAME_WCS}.war
	-rm -rf doc/*
	$(MAKE) -C src/syntaxParser clean

## Grammar generation using ANTLR
.PHONY: grammar
grammar:
	${ANTLR} src/grammar/wcps.g
	echo "package grammar;" > src/grammar/wcpsParser.java.tmp
	echo "" >> src/grammar/wcpsParser.java.tmp
	cat src/grammar/wcpsParser.java >> src/grammar/wcpsParser.java.tmp
	mv src/grammar/wcpsParser.java.tmp src/grammar/wcpsParser.java

.PHONY: doc
doc:
	${JAVADOC} -sourcepath src/ \
		   -d doc \
		   -windowtitle "Petascope Class reference"\
		   -doctitle "Petascope" \
		   -header '<b>Petascope</b>'\
		   -bottom '<a href="http://petascope.org">http://petascope.org</a>'\
		   -subpackages wcps \
		   -subpackages wcs \



