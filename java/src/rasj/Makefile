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
#   package RasJ
# 
# COMMENTS: 
# - FIXME: needs Makefile.rel for java.class rule, but Makefile.rel
#   should be reserved for RDBMS modules
##################################################################
######################### Definitions ############################

# standard include with general options
include $(RMANBASE)/Makefile.inc


SRCS = RasBaseType.java RasClientInternalException.java RasCollectionType.java \
	RasConnectionFailedException.java RasDimensionMismatchException.java \
	RasException.java RasFastScale.java RasGMArray.java RasIllegalULongValueException.java \
	RasIllegalUShortValueException.java RasImplementation.java RasImplementationInterface.java \
	RasIndexOutOfBoundsException.java RasInvalidNameException.java RasMArrayByte.java \
	RasMArrayDouble.java RasMArrayFloat.java RasMArrayInteger.java RasMArrayLong.java \
	RasMArrayShort.java RasMArrayType.java RasMInterval.java RasMIntervalType.java \
	RasOIDType.java RasPoint.java RasPointType.java RasPrimitiveType.java \
	RasQueryExecutionFailedException.java RasResultIsNoCellException.java \
	RasResultIsNoIntervalException.java RasRuntimeException.java RasSInterval.java \
	RasSIntervalType.java RasStorageLayout.java RasStreamInputOverflowException.java \
	RasStructureType.java RasType.java RasTypeInvalidException.java \
	RasTypeNotSupportedException.java RasTypeUnknownException.java
OBJS = ${SRCS:%.java=%.class}
MISCCLEAN = *.class

# directory where HTML documentation is created
DOCDIR_JAVA := $(DOCBASE)/rasj/

########################### Targets ##############################

# compile everything
.PHONY : all
all: $(OBJS)
	cd clientcommhttp; $(MAKE) 
	cd odmg; $(MAKE) 
	cd global; $(MAKE)
	cd rnp; $(MAKE)

# run test suite
.PHONY: systemtest
systemtest:
	cd test; $(MAKE) systemtest

# generate HTML doc
.PHONY: docu
docu:
	mkdir -p $(DOCDIR_RASJ)
	$(JAVADOC) -breakiterator -private -author -d $(DOCDIR_RASJ) -sourcepath $(RMANBASE)/java \
		rasj rasj.clientcommhttp rasj.odmg rasj.global rasdaview org.odmg examples
	chmod -R g+w $(DOCDIR_RASJ)
	cd test; $(MAKE) docu

# delete classes
.PHONY : clean
clean:
	-rm -f $(MISCCLEAN)
	cd clientcommhttp; $(MAKE) clean
	cd odmg; $(MAKE) clean
	cd global; $(MAKE) clean
	cd test; $(MAKE) clean
	cd rnp; $(MAKE) clean

# report environment used:
check:
	echo CLASSPATH=$(CLASSPATH)
	echo PATH=$(PATH)
	echo JAVA_HOME=$(JAVA_HOME)
	echo JSDK_HOME=$(JSDK_HOME)
	java -version

############################ Dependencies #######################

# FIXME: remove this dependency, establish .java.class rule in a general place
include $(RMANBASE)/Makefile.rel
