# Makefile for petascope

# ------------------------------------------------------------------------------
#                                 VARIABLES
# ------------------------------------------------------------------------------

VERSION_NUMBER := 1.0

# Location of trees.
SRC 		  	:= src/main
SRC_JAVA  	:= $(SRC)/java
SRC_WEB 	  := $(SRC)/webapp
TEST  			:= src/test
BUILD 			:= build
DIST	 			:= $(BUILD)/dist
WEB 				:= $(BUILD)/web
CLASSES  		:= $(WEB)/WEB-INF/classes
NAME				:= petascope
WAR_NAME				:= $(NAME).war
WAR					:= $(DIST)/$(WAR_NAME)
APIDOCS			:= $(BUILD)/apidocs

# Unix tools
AWK         := awk
FIND        := find
MKDIR       := mkdir -p
CP	    		:= cp -r
CD	    		:= cd
RM          := rm -rf
SHELL       := bash

# Java tools
JAVA        := java
JAVAC       := javac
JFLAGS      := -sourcepath $(SRC_JAVA)        \
               -d $(CLASSES)                 \
               -cp lib/activation.jar:lib/antlrworks-1.2.3.jar:lib/antlrworks-1.3.1.jar:lib/commons-fileupload-1.2.jar:lib/commons-io-1.3.2.jar:lib/commons-math-1.1.jar:lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar:lib/jaxb-api.jar:lib/jaxb-impl.jar:lib/jaxb-xjc.jar:lib/joda-time-1.6.jar:lib/jsr173_1.0_api.jar:lib/log4j-1.2.15.jar:lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar:lib/postgresql-8.3-604.jdbc4.jar:lib/rasj.jar:lib/servlet-2_5-api.jar:lib/slf4j-api-1.5.8.jar:lib/slf4j-log4j12-1.5.8.jar:lib/wcst-schema.jar:lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar:lib/xom-1.2.6.jar:lib/uuid-3.2.jar:lib/cos.jar:lib/junit-4.5.jar				 \
               -source 1.5

JVMFLAGS    := -ea                              \
               -esa                             \
               -Xfuture
JVM         := $(JAVA) $(JVMFLAGS)

JAR         := jar
JARFLAGS    := cf

JAVADOC     := javadoc
JDFLAGS     := -sourcepath $(SRC_JAVA)        \
               -d $(APIDOCS)			\
               -source 1.5 -classpath lib/activation.jar:lib/antlrworks-1.2.3.jar:lib/antlrworks-1.3.1.jar:lib/commons-fileupload-1.2.jar:lib/commons-io-1.3.2.jar:lib/commons-math-1.1.jar:lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar:lib/jaxb-api.jar:lib/jaxb-impl.jar:lib/jaxb-xjc.jar:lib/joda-time-1.6.jar:lib/jsr173_1.0_api.jar:lib/log4j-1.2.15.jar:lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar:lib/postgresql-8.3-604.jdbc4.jar:lib/rasj.jar:lib/servlet-2_5-api.jar:lib/slf4j-api-1.5.8.jar:lib/slf4j-log4j12-1.5.8.jar:lib/wcst-schema.jar:lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar:lib/xom-1.2.6.jar:lib/uuid-3.2.jar:lib/cos.jar:lib/junit-4.5.jar

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
	@echo "    all      compile sources and generate javadoc"
	@echo "    install  deploy WAR archive in Tomcat (CATALINA_HOME should be properly set)"
	@echo "    javadoc  to generate Javadoc in $(APIDOCS)"
	@echo "    setupdb  setup petascope database"
	@echo "    dropdb   destroy petascope database"
	@echo "    war      create WAR archive in $(DIST)"
	@echo "    test     run automated tests"
	@echo "    clean    remove all build files"
	@echo "    wps      build 52n WPS server with petascope support"
	@echo

# all_javas - Temp file for holding source file list
all_javas := $(CLASSES)/all.javas

# Gather source file list, excluding n52 which are exclusively compiled with maven
.INTERMEDIATE: $(all_javas)
$(all_javas):
	$(MKDIR) $(CLASSES)
	$(FIND) $(SRC_JAVA) $(TEST) -name '*.java' | grep -v 'n52' > $@

# compile - Compile the source
.PHONY: compile
compile: $(all_javas)
	$(MKDIR) $(BUILD)
	$(CP) $(SRC_WEB)/* $(WEB)
	$(CP) templates xml $(SRC)/resources/log4j.properties $(WEB)
	$(MKDIR) $(WEB)/WEB-INF/lib
	$(CP) lib/activation.jar lib/antlrworks-1.2.3.jar lib/antlrworks-1.3.1.jar lib/commons-fileupload-1.2.jar lib/commons-io-1.3.2.jar lib/commons-math-1.1.jar lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar lib/jaxb-api.jar lib/jaxb-impl.jar lib/jaxb-xjc.jar lib/joda-time-1.6.jar lib/jsr173_1.0_api.jar lib/log4j-1.2.15.jar lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar lib/postgresql-8.3-604.jdbc4.jar lib/rasj.jar lib/servlet-2_5-api.jar lib/slf4j-api-1.5.8.jar lib/slf4j-log4j12-1.5.8.jar lib/wcst-schema.jar lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar lib/xom-1.2.6.jar lib/uuid-3.2.jar lib/cos.jar $(WEB)/WEB-INF/lib
	# copy resources (xml, xsd, properties)
	$(CP) $(SRC_JAVA)/* $(CLASSES)
	$(FIND) $(BUILD) -name '*.java' | xargs $(RM)
	# remove .svn directories
	$(FIND) $(BUILD) -name '.svn' | xargs $(RM)
	$(JAVAC) $(JFLAGS) @$<
	# copy default settings, and then custom settings in db/settings.properties if they exist
	$(CP) src/main/resources/settings.properties $(CLASSES)
	if test -e db/settings.properties; then $(CP) db/settings.properties $(CLASSES); fi

# Generate the Java doc from sources
.PHONY: javadoc
javadoc: $(all_javas)
	$(JAVADOC) $(JDFLAGS) @$<

# copy WAR to TOMCAT
.PHONY: install
install: war
	$(CP) $(WAR) $(CATALINA_HOME)/webapps

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
	${JAVA} -cp lib/activation.jar:lib/antlrworks-1.2.3.jar:lib/antlrworks-1.3.1.jar:lib/commons-fileupload-1.2.jar:lib/commons-io-1.3.2.jar:lib/commons-math-1.1.jar:lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar:lib/jaxb-api.jar:lib/jaxb-impl.jar:lib/jaxb-xjc.jar:lib/joda-time-1.6.jar:lib/jsr173_1.0_api.jar:lib/log4j-1.2.15.jar:lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar:lib/postgresql-8.3-604.jdbc4.jar:lib/rasj.jar:lib/servlet-2_5-api.jar:lib/slf4j-api-1.5.8.jar:lib/slf4j-log4j12-1.5.8.jar:lib/wcst-schema.jar:lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar:lib/xom-1.2.6.jar:lib/uuid-3.2.jar:lib/cos.jar:lib/junit-4.5.jar:$(BUILD)/test/classes:$(BUILD)/web/WEB-INF/classes org.junit.runner.JUnitCore petascope.AllTests

# clean up
.PHONY: clean
clean:
	$(RM) $(BUILD) $(CATALINA_HOME)/webapps/$(NAME) $(CATALINA_HOME)/webapps/$(WAR_NAME)

# Perform all tasks for a complete build
.PHONY: all
all: clean war javadoc setupdb

# build jar file
.PHONY: jar
jar: compile
	$(CD) $(CLASSES); $(JAR) $(JARFLAGS) ../../../petascope.jar .

# inject petascope libs into the wps 
.PHONY: wps
wps: jar
	$(MKDIR) $(BUILD)/wps
	# adapt the line below to copy 52n war to build/wps/wps.war
	# $(CP) ../wps/52n-wps-webapp/target/52n-wps-webapp-2.0-RC8-SNAPSHOT.war $(BUILD)/wps/wps.war
	cd $(BUILD)/wps; $(JAR) xf wps.war; $(RM) wps.war
	$(CP) $(BUILD)/petascope.jar lib/*.jar $(BUILD)/wps/WEB-INF/lib
	$(CD) $(BUILD)/wps; $(JAR) $(JARFLAGS) ../wps.war .

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
