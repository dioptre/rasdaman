#! /bin/bash
# Command-line utility for testing PetaScope WCPS
# Usage: ./full-test-online.sh [URL] [testFolder] [outputFolder]
# Parameters: 
# * URL: the URL of the PetaScope implementation. Defaults to the new implementation on kahlua
# * testFolder: the folder that contains abstract syntax query test cases (extension ".test")
# * outputFolder: the folder where the script should save intermediate and output files. 
# 
# WARNING: if you use relative paths for the folders, they should be relative to the project folder, not to the test folder !
<<COMMENT

/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */

COMMENT

CLASSPATH=.:lib/wcps-all.jar:lib/jaxb-api.jar:lib/jaxb-impl.jar:lib/postgresql-8.3-604.jdbc4.jar:lib/jsr173_api.jar:lib/rasj.jar:lib/commons-math-1.1.jar:lib/antlrworks-1.2.3.jar:lib/commons-io-1.3.2.jar

# cd .. && make jar && java -cp $CLASSPATH wcps.server.test.FullTestsOnline $1 $2 $3
cd .. && java -cp $CLASSPATH wcps.server.test.FullTestsOnline $1 $2 $3
