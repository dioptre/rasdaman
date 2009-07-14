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

# Major version
MAJOR=1
# Minor version
MINOR=0
# Bugfix version
BUGFIX=0
# Final Version String
VERSION=${MAJOR}.${MINOR}.${BUGFIX}

CLASSPATH=.:lib/junit-4.5.jar:lib/jsr173_1.0_api.jar:lib/rasj.jar:lib/servlet-2_5-api.jar:lib/commons-fileupload-1.2.jar:lib/commons-math-1.1.jar:lib/antlrworks-1.2.3.jar:lib/commons-io-1.3.2.jar
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
.PHONY: default
default: clean grammar core servlet war jar

## Regenerate the wcps.xml.processcoverage package.
.PHONY: gen
gen:
	@echo ""
	@echo "*** Generating JAXB bindings for WCPS schema..."
	${XJC} -d src -p wcps.xml.processcoverage xml/ogc/wcps/1.0.0/wcpsProcessCoverages.xsd
	${JAVAC} src/wcps/xml/processcoverage/*.java

## Generate translation core.
.PHONY: core
core:
	@echo ""
	@echo "*** Compiling core WCPS ..."
	rm -rf ${WCPS_CORE_LIB}
	${JAVAC} src/wcps/server/core/*.java
	mkdir -p wcps/xml/processcoverage wcps/server/core
	cp src/wcps/server/core/*.class wcps/server/core/
	jar cf ${WCPS_CORE_LIB} wcps
	rm -rf wcps

## Compile WCPS tests
.PHONY: tests
tests:
	@echo ""
	@echo "*** Compiling tests ..."
	${JAVAC} src/wcps/server/test/*.java

## Servlet interface.
.PHONY: servlet
servlet: 
	@echo ""
	@echo "*** Compiling servlet files ..."
	${JAVAC} src/wcps/server/servlet/*.java

## Packages a servlet in a .WAR container.
.PHONY: war
war:
	@echo ""
	@echo "*** Building WAR archive ..."
	@rm -rf ${NAME}.war
	mkdir -p WEB-INF/lib
	cp lib/*.jar WEB-INF/lib/
	rm WEB-INF/lib/servlet-2_5-api.jar
	mkdir -p WEB-INF/classes/wcps/server/servlet
	mkdir -p WEB-INF/classes/wcs/server/servlet
	mkdir -p WEB-INF/classes/grammar
	cp web.xml WEB-INF/
	cp src/wcps/server/servlet/*.class WEB-INF/classes/wcps/server/servlet/
	cp src/grammar/*.class WEB-INF/classes/grammar/
	jar cf ${NAME}.war WEB-INF/ dbparams.properties xml/ogc/wcps/1.0.0/*.xsd misc/wcps-servlet.html

## Translation core command-line interface. Useful for development and testing.
.PHONY: cli
cli:
	@echo ""
	@echo "*** Compiling command-line tools ..."
	${JAVAC} src/wcps/server/cli/*.java

.PHONY: jar
jar: core cli tests
	@echo ""
	@echo "*** Building jar file ..."
	@rm -f ${WCPS_ALL_LIB}
	jar cf ${WCPS_ALL_LIB} -C src/ ./wcps/ -C src/ ./grammar/

## Clean up.
.PHONY: clean
clean:
	@echo ""
	@echo "*** Cleaning up..."
	@rm -rf src/wcps/xml src/wcps/server/core/*.class src/wcps/server/servlet/*.class src/wcps/server/cli/*.class lib/wcps.jar ${NAME}.war WEB-INF src/wcs/server/core/*.class src/wcs/server/cli/*.class src/wcs/server/servlet/*.class src/wcps/server/test/*.class src/grammar/*.class ${NAME_WCS}.war ${WCPS_CORE_LIB} ${WCPS_ALL_LIB}
	@rm -rf doc/*
	@rm -f petascope-*.tar.bz2
	@rm -f src/grammar/wcps.tokens src/grammar/wcps__.g

## Grammar generation using ANTLR
.PHONY: grammar
grammar:
	@echo ""
	@echo "*** Generating WCPS grammar files (with ANTLR)..."
	${ANTLR} src/grammar/wcps.g

.PHONY: doc
doc:
	@echo ""
	@echo "*** Generating documentation ..."
	${JAVADOC} -sourcepath src/ \
		   -d doc \
		   -windowtitle "Petascope Class reference"\
		   -doctitle "Petascope" \
		   -header '<b>Petascope</b>'\
		   -bottom '<a href="http://petascope.org">http://petascope.org</a>'\
		   -subpackages wcps

dist: clean doc petascope-${VERSION}.tar.bz2
	
.PHONY: petascope-${VERSION}.tar.bz2
petascope-${VERSION}.tar.bz2:
	@echo ""
	@echo "*** Building distribution archive ..."
	@rm $@ -f
	tar cvjf ../$@ *
	mv ../$@ $@
	@echo ""
	@echo "*** Done building distribution archive !"

