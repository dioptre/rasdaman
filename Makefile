 #
 # This file is part of Petascope.
 #
 # Petascope is free software: you can redistribute it and/or modify
 # it under the terms of the GNU Lesser General Public License as
 # published by the Free Software Foundation, either version 3 of
 # the License, or (at your option) any later version.
 #
 # Petascope is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU Lesser General Public License for more details.
 #
 # You should have received a copy of the GNU Lesser General Public
 # License along with Petascope. If not, see <http://www.gnu.org/licenses/>.
 #
 # For more information please see <http://www.Petascope.org>
 # or contact Peter Baumann via <baumann@rasdaman.com>.
 #
 # Copyright 2009 Jacobs University Bremen, Peter Baumann.
 #


NAME=petascope
NAME_WCS=petascope-wcs

VERSION=1.0

WCS_LIBS=lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar:lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar:lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar:lib/commons-io-1.3.2.jar
CLASSPATH=.:lib/junit-4.5.jar:lib/jsr173_1.0_api.jar:lib/rasj.jar:lib/servlet-2_5-api.jar:lib/commons-fileupload-1.2.jar:lib/commons-math-1.1.jar:lib/antlrworks-1.2.3.jar:${WCS_LIBS}
WCPS_CORE_LIB=lib/wcps-core.jar
WCPS_ALL_LIB=lib/wcps-all.jar
JAVA_PATH=
ANTLR=${JAVA_PATH}java -cp lib/antlrworks-1.2.3.jar org.antlr.Tool
# Use this line to see warnings
# JAVAC=${JAVA_PATH}javac -g -Xlint -cp ${CLASSPATH} -sourcepath src/
JAVAC=${JAVA_PATH}javac -g -cp ${CLASSPATH} -sourcepath src/
XJC=${JAVA_PATH}java -jar lib/jaxb-xjc.jar
JAVA=${JAVA_PATH}java -cp ${CLASSPATH} 
JAVADOC=${JAVA_PATH}javadoc
## Typical build process.
.PHONY: most
most: clean wcps-grammar wcps-core wcps-servlet wcps-war wcps-jar

## Complete build process.
.PHONY: all
all: clean wcps-core wcs wcps-cli wcps-servlet wcps-war wcs-war

## Regenerate the wcps.xml.processcoverage package.
.PHONY: gen
gen:
	${XJC} -d src -p wcps.xml.processcoverage xml/ogc/wcps/1.0.0/wcpsProcessCoverages.xsd
	${JAVAC} src/wcps/xml/processcoverage/*.java

## Generate translation core.
.PHONY: wcps-core
wcps-core:
	rm -rf ${WCPS_CORE_LIB}
	${JAVAC} src/wcps/server/core/*.java
	mkdir -p wcps/xml/processcoverage wcps/server/core
	cp src/wcps/server/core/*.class wcps/server/core/
	jar cf ${WCPS_CORE_LIB} wcps
	rm -rf wcps

## Compile WCPS tests
.PHONY: wcps-tests
wcps-tests:
	${JAVAC} src/wcps/server/test/*.java

## Servlet interface.
.PHONY: wcps-servlet
wcps-servlet: 
	${JAVAC} src/wcps/server/servlet/*.java
#	${JAVAC} src/wcs/server/servlet/*.java

## Packages a servlet in a .WAR container.
.PHONY: wcps-war
wcps-war:
	rm -rf ${NAME}.war
	mkdir -p WEB-INF/lib
	cp lib/*.jar WEB-INF/lib/
	rm WEB-INF/lib/servlet-2_5-api.jar
	mkdir -p WEB-INF/classes/wcps/server/servlet
	mkdir -p WEB-INF/classes/wcs/server/servlet
	mkdir -p WEB-INF/classes/grammar
#	mkdir -p WEB-INF/classes/wcs/server/core
	cp web.xml WEB-INF/
	cp src/wcps/server/servlet/*.class WEB-INF/classes/wcps/server/servlet/
	cp src/grammar/*.class WEB-INF/classes/grammar/
#	cp src/wcs/server/servlet/*.class WEB-INF/classes/wcs/server/servlet/
#	cp src/wcs/server/core/*.class WEB-INF/classes/wcs/server/core/
	jar cf ${NAME}.war WEB-INF/ dbparams.properties xml/ogc/wcps/1.0.0/*.xsd

## Translation core command-line interface. Useful for development and testing.
.PHONY: wcps-cli
wcps-cli:
	${JAVAC} src/wcps/server/cli/*.java
#	${JAVAC} src/wcs/server/cli/*.java

.PHONY: wcps-jar
wcps-jar: wcps-core wcps-cli wcps-tests
	-rm -f ${WCPS_ALL_LIB}
	jar cf ${WCPS_ALL_LIB} -C src/ ./wcps/ -C src/ ./grammar/

## Clean up.
.PHONY: clean
clean:
	-rm -rf src/wcps/xml src/wcps/server/core/*.class src/wcps/server/servlet/*.class src/wcps/server/cli/*.class lib/wcps.jar ${NAME}.war WEB-INF src/wcs/server/core/*.class src/wcs/server/cli/*.class src/wcs/server/servlet/*.class src/wcps/server/test/*.class src/grammar/*.class ${NAME_WCS}.war ${WCPS_CORE_LIB} ${WCPS_ALL_LIB}
	-rm -rf doc/*
	-rm -f petascope-*.tar.bz2
	-rm -f src/grammar/wcps.tokens src/grammar/wcps__.g

## Grammar generation using ANTLR
.PHONY: grammar
wcps-grammar:
	${ANTLR} src/grammar/wcps.g

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


dist: petascope-${VERSION}.tar.bz2
	
.PHONY: petascope-${VERSION}.tar.bz2	
petascope-${VERSION}.tar.bz2:
	${MAKE} clean
	-rm $@
	tar cvjf ../$@ *
	mv ../$@ $@




############ WCS Stuff ###############
## Compile the WCS code
.PHONY: wcs
wcs: 
	${XJC} -d src -p wcs.server.core xml/ogc/wcps/1.0.0/wcpsProcessCoverages.xsd
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
	mkdir -p WEB-INF/classes/wcs/server/servlet
	mkdir -p WEB-INF/classes/wcs/server/core
	cp web-wcs.xml WEB-INF/web.xml
	cp src/wcps/server/servlet/*.class WEB-INF/classes/wcps/server/servlet/
	cp src/wcs/server/servlet/*.class WEB-INF/classes/wcs/server/servlet/
	cp src/wcs/server/core/*.class WEB-INF/classes/wcs/server/core/
	jar cf ${NAME_WCS}.war WEB-INF/ dbparams.properties xml/ogc/wcps/1.0.0/*.xsd misc/wcs-servlet-default.html


