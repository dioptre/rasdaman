# -*-Makefile-*-
#
# This file is part of rasdaman community.
#
# Rasdaman community is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Rasdaman community is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
#
# Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
# rasdaman GmbH.
#
# For more information please see <http://www.rasdaman.org>
# or contact Peter Baumann via <baumann@rasdaman.com>.
# 
# MAKEFILE FOR:  
#   petascope
# 
# COMMENTS:
# 
##################################################################

# ------------------------------------------------------------------------------
#                                 VARIABLES
# ------------------------------------------------------------------------------

VERSION_NUMBER = 1.0


if GENERATE_DOCS
  GEN_DOCS = javadoc
endif

# Location of trees.
SRC	  = src/main
SRC_MAIN  = $(SRC)/java
SRC_WEB   = $(SRC)/webapp
TEST      = src/test
BUILD     = build
DIST      = $(BUILD)/dist
WEB       = $(BUILD)/web
CLASSES   = $(WEB)/WEB-INF/classes
NAME      = petascope
WAR_NAME  = $(NAME).war
WAR       = $(DIST)/$(WAR_NAME)
APIDOCS   = $(BUILD)/apidocs
SUBDIRS	  = ../../java

# Unix tools
FIND      = find
MKDIR     = mkdir -p
CP        = cp -r
CD        = cd
RM        = rm -rf
SHELL     = bash

# Java tools
JAVA      = java
JAVAC     = javac
JJARS_CP  = lib/activation.jar:lib/antlrworks-1.2.3.jar:lib/antlrworks-1.3.1.jar:lib/commons-fileupload-1.2.jar:lib/commons-io-1.3.2.jar:lib/commons-math-1.1.jar:lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar:lib/jaxb-api.jar:lib/jaxb-impl.jar:lib/jaxb-xjc.jar:lib/joda-time-1.6.jar:lib/jsr173_1.0_api.jar:lib/log4j-1.2.15.jar:lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar:lib/postgresql-8.3-604.jdbc4.jar:../../java/dist/rasj.jar:lib/servlet-2_5-api.jar:lib/slf4j-api-1.5.8.jar:lib/slf4j-log4j12-1.5.8.jar:lib/wcst-schema.jar:lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar:lib/xom-1.2.6.jar:lib/uuid-3.2.jar:lib/cos.jar:lib/junit-4.5.jar:lib/gt-opengis-2.7.4.jar:lib/gt-referencing-2.7.4.jar:lib/jts-1.11.jar:lib/gt-api-2.7.4.jar:lib/commons-pool-1.5.4.jar:lib/gt-epsg-wkt-2.7.4.jar:lib/jai_core-1.1.3.jar:lib/jsr-275-1.0-beta-2.jar:lib/vecmath-1.3.2.jar:lib/hsqldb-1.8.0.7.jar:lib/date4j.jar:lib/gt-metadata-2.7.4.jar
JJARS_PATHS = lib/activation.jar lib/antlrworks-1.2.3.jar lib/antlrworks-1.3.1.jar lib/commons-fileupload-1.2.jar lib/commons-io-1.3.2.jar lib/commons-math-1.1.jar lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar lib/jaxb-api.jar lib/jaxb-impl.jar lib/jaxb-xjc.jar lib/joda-time-1.6.jar lib/jsr173_1.0_api.jar lib/log4j-1.2.15.jar lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar lib/postgresql-8.3-604.jdbc4.jar ../../java/dist/rasj.jar lib/servlet-2_5-api.jar lib/slf4j-api-1.5.8.jar lib/slf4j-log4j12-1.5.8.jar lib/wcst-schema.jar lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar lib/xom-1.2.6.jar lib/uuid-3.2.jar lib/cos.jar lib/junit-4.5.jar lib/gt-opengis-2.7.4.jar lib/gt-referencing-2.7.4.jar lib/jts-1.11.jar lib/gt-api-2.7.4.jar lib/commons-pool-1.5.4.jar lib/gt-epsg-wkt-2.7.4.jar lib/jai_core-1.1.3.jar lib/jsr-275-1.0-beta-2.jar lib/vecmath-1.3.2.jar lib/hsqldb-1.8.0.7.jar lib/date4j.jar lib/gt-metadata-2.7.4.jar
JFLAGS    = -sourcepath $(SRC_MAIN) \
               -d $(CLASSES)        \
               -cp $(JJARS_CP)	    \
               -source 1.5

JVMFLAGS  = -ea                     \
            -esa                    \
            -Xfuture
JVM       = $(JAVA) $(JVMFLAGS)

JAR       = jar
JARFLAGS  = cf

JAVADOC   = javadoc
JDFLAGS   = -sourcepath $(SRC_MAIN)  \
            -d $(APIDOCS)	     \
            -source 1.5 -classpath $(JJARS_CP)
# ------------------------------------------------------------------------------
#                                 TARGETS
# ------------------------------------------------------------------------------


# Create war
.PHONY: war
war: compile
	$(MKDIR) $(DIST)
	$(CD) $(WEB); $(JAR) $(JARFLAGS) ../../$(WAR) .

# Print the targets
.PHONY: help
help:
	@echo
	@echo "  Available rules:"
	@echo "    war      create WAR archive in $(DIST)"
	@echo "    install  copy generated WAR archive to wardir set at configure"
	@echo "    javadoc  to generate Javadoc in $(APIDOCS)"
	@echo "    setupdb  setup petascope database"
	@echo "    dropdb   destroy petascope database"
	@echo "    test     run automated tests"
	@echo "    clean    remove all build files"
	@echo "    wps      build 52n WPS server with petascope support and copy to wardir"
	@echo

# all_javas - Temp file for holding source file list
all_javas = $(CLASSES)/all.javas

# Gather source file list, excluding n52 which are exclusively compiled with maven
.INTERMEDIATE: $(all_javas)
$(all_javas):
	$(MKDIR) $(CLASSES)
	$(FIND) $(SRC_MAIN) $(TEST) -name '*.java' | grep -v 'n52' > $@

# compile - Compile the source
.PHONY: compile
compile: $(all_javas) $(GEN_DOCS)
	$(MKDIR) $(BUILD)
	$(SED) 's|@confdir@|$(DESTDIR)$(sysconfdir)/|g' $(SRC_WEB)/WEB-INF/web.xml.in > $(SRC_WEB)/WEB-INF/web.xml
	$(CP) $(SRC_WEB)/* $(WEB)
	$(CP) templates xml $(WEB)
	$(MKDIR) $(WEB)/WEB-INF/lib
	$(CP) $(JJARS_PATHS) $(WEB)/WEB-INF/lib
	$(CP) $(SRC_MAIN)/* $(CLASSES) # copy resources (xml, xsd, properties)
	$(FIND) $(BUILD) -name '*.java' | xargs $(RM)
	$(FIND) $(BUILD) -name '.svn' | xargs $(RM) # remove .svn directories
	$(JAVAC) $(JFLAGS) @$<

# Generate the Java doc from sources
.PHONY: javadoc
javadoc: $(all_javas)
	$(JAVADOC) $(JDFLAGS) @$<

# copy WAR to TOMCAT
.PHONY: install
install: war
	$(MKDIR) $(DESTDIR)$(wardir)
	$(CP) $(WAR) $(DESTDIR)$(wardir)
if GENERATE_DOCS
	$(MKDIR) $(DESTDIR)$(pkgdatadir)/doc
	$(CP) $(APIDOCS) $(DESTDIR)$(pkgdatadir)/doc/petascope
endif
	if test -f $(DESTDIR)$(sysconfdir)/petascope.properties; then \
		echo "$@ will not overwrite existing $(DESTDIR)$(sysconfdir)/petascope.properties"; \
	else \
		$(MKDIR_P) $(DESTDIR)$(sysconfdir); \
		echo " $(INSTALL_DATA) src/main/resources/petascope.properties $(DESTDIR)$(sysconfdir)/petascope.properties"; \
		$(INSTALL_DATA) src/main/resources/petascope.properties $(DESTDIR)$(sysconfdir)/petascope.properties; \
	fi;
	if test -f $(DESTDIR)$(sysconfdir)/log4j.properties; then \
		echo "$@ will not overwrite existing $(DESTDIR)$(sysconfdir)/log4j.properties"; \
	else \
		$(MKDIR_P) $(DESTDIR)$(sysconfdir); \
		echo " $(INSTALL_DATA) src/main/resources/log4j.properties $(DESTDIR)$(sysconfdir)/log4j.properties"; \
		$(INSTALL_DATA) src/main/resources/log4j.properties $(DESTDIR)$(sysconfdir)/log4j.properties; \
	fi;

# setup database
.PHONY: setupdb
setupdb:
	@${CD} db && ./update_db.sh

# destroy petascope database
.PHONY: dropdb
dropdb:
	@${CD} db && ./update_db.sh dropall

# run any JUnit tests
.PHONY: test
test:
	${JAVA} -cp $(JJARS_CP):$(BUILD)/test/classes:$(BUILD)/web/WEB-INF/classes org.junit.runner.JUnitCore petascope.AllTests

# clean up
.PHONY: clean
clean:
	$(RM) $(BUILD) dist

# build jar file
.PHONY: jar
jar: compile
	$(CD) $(CLASSES); $(JAR) $(JARFLAGS) ../../../petascope.jar .

# inject petascope libs into the 52n wps 
.PHONY: wps
wps: jar
	$(MKDIR) $(BUILD)/wps
	$(CP) $(wpswar) $(BUILD)/wps/wps.war
	cd $(BUILD)/wps; $(JAR) xf wps.war; $(RM) wps.war
	$(CP) $(BUILD)/petascope.jar lib/*.jar $(BUILD)/wps/WEB-INF/lib
	$(CD) $(BUILD)/wps; $(JAR) $(JARFLAGS) ../wps.war .
	$(CP) $(BUILD)/wps.war $(DESTDIR)$(wardir)

.PHONY: all
all: war
	
EXTRA_DIST = \
	build.xml \
	db \
	INSTALL \
	lib \
	LICENCE \
	Makefile \
	nbproject \
	README \
	README.WCS2 \
	src \
	templates \
	xml

DISTFILES = $(EXTRA_DIST)

# Put petascope into archive for RPM generation
.PHONY: distdir
distdir:
	mkdir -p $(distdir)
	cp -R $(DISTFILES) $(distdir)
