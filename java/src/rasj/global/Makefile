# -*-Makefile-*- (for Emacs)
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
# or contact Peter Baumann via <baumann@rasdaman.com>. # Top Level makefile. This points to the various modules that have to be build
# and/or deployed
# 
# MAKEFILE FOR:  
#   package RasJ.global
# 
# COMMENTS: 
# - FIXME: errtxts has been i18nalized, reflect it in the RasError code!
#
##################################################################
######################### Definitions ############################

# standard include with general options
include $(RMANBASE)/Makefile.inc

# error text file
ERRTXT_FILE = $(RMANBASE)/bin/errtxts_en

SRCS = RasErrorTexts.java RasGlobalDefs.java Debug.java BenchmarkTimer.java
OBJS = ${SRCS:%.java=%.class} Version.class
MISCCLEAN = *.class Version.java

# directory where HTML documentation is created
DOCDIR := $(DOCBASE)/java/rasj/global

########################### Targets ##############################

# compile everything
.PHONY : all
all: Version.java errortexts
	$(MAKE) $(OBJS)

.PHONY : errortexts
# create array of error messages in class RasErrorTexts
errortexts:
	if (test -f $(ERRTXT_FILE)) \
	then \
		grep "\^E\^" $(ERRTXT_FILE) \
		| sed -e '/^#/ d' \
		| sed -e 's+\^E\^+:+g' \
		| sed -e 's+^+"+g' \
		| sed -e 's+$$+\",+g' > tmpfile; \
		cat RasErrorTexts.template1 tmpfile RasErrorTexts.template2 > RasErrorTexts.java; \
	else \
		echo "Error: errtxts file $(ERRTXT_FILE) not found."; \
		exit 1; \
	fi
	-rm tmpfile

# always generate to have an up-to date version & generation time info:
.PHONY: Version.java
Version.java:
	@ sed  s/%%DATE%%/`date +%Y-%b-%d`/       <Version.java.template \
	| sed  s/%%VERSION%%/$(RMANVERSIONNAME)/                        \
	| sed "s/%%GENERATION_DATE%%/`date`/"     >Version.java

# delete all files
.PHONY: clean
clean:
	-rm -f $(SRCS) $(MISCCLEAN)

############################ Dependencies #######################
# general rules
include $(RMANBASE)/Makefile.rel

