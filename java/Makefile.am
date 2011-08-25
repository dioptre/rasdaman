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
#   utility programs
# 
# COMMENTS: 
# - Various insertion-tools for special client data
# 
##################################################################

EXTRA_DIST = src/org/odmg/*.java src/rasj/*.java src/rasj/global/*.java src/rasj/clientcommhttp/*.java src/rasj/odmg/*.java src/rasj/rnp/*.java

all : src/org/odmg/*.java src/rasj/*.java src/rasj/global/*.java src/rasj/clientcommhttp/*.java src/rasj/odmg/*.java src/rasj/rnp/*.java
	mkdir -p bin dist
	javac -d bin src/org/odmg/*.java src/rasj/*.java src/rasj/global/*.java src/rasj/clientcommhttp/*.java src/rasj/odmg/*.java src/rasj/rnp/*.java
	jar -cf dist/rasj.jar -C bin .

examples: src/examples/*.java all
	mkdir -p bin
	javac -d bin src/examples/*.java -cp dist/rasj.jar
	
clean:
	rm -rf bin dist
