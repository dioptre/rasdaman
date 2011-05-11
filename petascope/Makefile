# Makefile for petascope

# ------------------------------------------------------------------------------
#                                 VARIABLES
# ------------------------------------------------------------------------------

VERSION_NUMBER := 1.0

# Location of trees.
SRC  			:= src
TEST  			:= test
BUILD 			:= build
DIST	 		:= $(BUILD)/dist
WEB 			:= $(BUILD)/web
CLASSES  		:= $(WEB)/WEB-INF/classes
WAR			:= $(DIST)/petascope.war
APIDOCS			:= $(BUILD)/apidocs

# Unix tools
AWK         := awk
FIND        := find
MKDIR       := mkdir -p
CP	    := cp -r
CD	    := cd
RM          := rm -rf
SHELL       := bash

# Java tools
JAVA        := java
JAVAC       := javac
JFLAGS      := -sourcepath $(SRC)        \
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
JDFLAGS     := -sourcepath $(SRC)        \
               -d $(APIDOCS)			\
               -source 1.5

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
	@echo "    setupdb  setup PostgreSQL database petascopedb. To be run only once"
	@echo "    war      create WAR archive in $(DIST)"
	@echo "    test     run automated tests"
	@echo "    clean    remove all build files"
	@echo

# all_javas - Temp file for holding source file list
all_javas := $(CLASSES)/all.javas

# Gather source file list
.INTERMEDIATE: $(all_javas)
$(all_javas):
	$(MKDIR) $(CLASSES)
	$(FIND) $(SRC) $(TEST) -name '*.java' > $@

# compile - Compile the source
.PHONY: compile
compile: $(all_javas)
	$(MKDIR) $(BUILD)
	$(CP) web $(BUILD)
	$(CP) templates xml db/settings.properties $(WEB)
	if test -d settings.properties; then $(CP) settings.properties $(WEB); fi
	$(MKDIR) $(WEB)/WEB-INF/lib
	$(CP) lib/activation.jar lib/antlrworks-1.2.3.jar lib/antlrworks-1.3.1.jar lib/commons-fileupload-1.2.jar lib/commons-io-1.3.2.jar lib/commons-math-1.1.jar lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar lib/jaxb-api.jar lib/jaxb-impl.jar lib/jaxb-xjc.jar lib/joda-time-1.6.jar lib/jsr173_1.0_api.jar lib/log4j-1.2.15.jar lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar lib/postgresql-8.3-604.jdbc4.jar lib/rasj.jar lib/servlet-2_5-api.jar lib/slf4j-api-1.5.8.jar lib/slf4j-log4j12-1.5.8.jar lib/wcst-schema.jar lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar lib/xom-1.2.6.jar lib/uuid-3.2.jar lib/cos.jar $(WEB)/WEB-INF/lib
	# copy resources (xml, xsd, properties)
	$(CP) $(SRC)/* $(CLASSES)
	$(FIND) $(BUILD) -name '*.java' | xargs $(RM)
	# remove .svn directories
	$(FIND) $(BUILD) -name '.svn' | xargs $(RM)
	$(JAVAC) $(JFLAGS) @$<
	$(CP) $(SRC)/log4j.properties $(WEB)

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
	${CD} db && ./update_db.sh dropall && ./update_db.sh 0

# run any JUnit tests
.PHONY: test
test:
	${JAVA} -cp lib/activation.jar:lib/antlrworks-1.2.3.jar:lib/antlrworks-1.3.1.jar:lib/commons-fileupload-1.2.jar:lib/commons-io-1.3.2.jar:lib/commons-math-1.1.jar:lib/gml-v_3_1_1-schema-1.0-SNAPSHOT.jar:lib/jaxb-api.jar:lib/jaxb-impl.jar:lib/jaxb-xjc.jar:lib/joda-time-1.6.jar:lib/jsr173_1.0_api.jar:lib/log4j-1.2.15.jar:lib/ows-v_1_0_0-schema-1.0-SNAPSHOT.jar:lib/postgresql-8.3-604.jdbc4.jar:lib/rasj.jar:lib/servlet-2_5-api.jar:lib/slf4j-api-1.5.8.jar:lib/slf4j-log4j12-1.5.8.jar:lib/wcst-schema.jar:lib/wcs-v_1_1_0-schema-1.0-SNAPSHOT.jar:lib/xom-1.2.6.jar:lib/uuid-3.2.jar:lib/cos.jar:lib/junit-4.5.jar:$(BUILD)/test/classes:$(BUILD)/web/WEB-INF/classes org.junit.runner.JUnitCore petascope.AllTests

# clean up
.PHONY: cleanup
clean:
	$(RM) $(BUILD) dist $(CATALINA_HOME)/webapps/petascope*

# Perform all tasks for a complete build
.PHONY: all
all: install javadoc


