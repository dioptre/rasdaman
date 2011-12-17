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
package petascope.wcps.server.test;

import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import org.apache.commons.io.FileUtils;
import petascope.wcps.server.cli.grammar;

/**
 * Runs the available tests for the abstract syntax grammar, by calling the CLI.
 * Assumes test passes if no error is thrown from the grammar CLI.
 * @author Andrei Aiordachioaie
 */
public class GrammarTest {
    // Put new test cases in this folder following the current naming scheme

    String folder = "test/testcases-wcps_dollar/";
    // How many tests we have to run
    int numTests = 0;
    // tests
    File[] tests;
    // queries
    String[] queries;
    // success code
    boolean[] ok;
    // message for tests
    String[] errors;
    // How many successes?
    int passCount = 0;

    public void printResults() {

        for (int i = 0; i < numTests; i++) {
            String tname = tests[i].getName();
            tname = tname.substring(0, tname.length() - 5);
            if (ok[i] == true) {
                System.out.println("*** Test '" + tname + "' ok");
//                System.out.println("\t" + queries[i]);
            } else {
                System.out.println("*** Test '" + tname + "' FAILED");
                System.out.println("\t" + queries[i]);
                System.out.println("\t ERROR: " + errors[i]);
            }
        }
        System.out.println("\n\nRESULTS\n");
        System.out.println("Tests succeeded: " + String.valueOf(passCount));
        System.out.println("Tests failed: " + String.valueOf(numTests - passCount));
    }

    public GrammarTest() {
        // Find out how many tests we have to run
        File dir = new File(folder);
        System.out.println("Looking for tests in " + dir.getAbsolutePath() + "\n");
        GrammarFileFilter filter = new GrammarFileFilter();
        tests = dir.listFiles(filter);
        numTests = tests.length;
        ok = new boolean[numTests];
        errors = new String[numTests];
        queries = new String[numTests];
    }

    /* Accept all files with extension test. */
    private class GrammarFileFilter implements FilenameFilter {

        @Override
        public boolean accept(File dir, String name) {
            if (name.endsWith("test")) {
                return true;
            }
            return false;
        }
    }

    public void runAllTests() {
        String query = "";
        for (int i = 0; i < numTests; i++) {
            ok[i] = false;
            try {
                query = FileUtils.readFileToString(tests[i]);
                queries[i] = query;
            } catch (IOException e) {
                errors[i] = "Could not read file " + tests[i].getName();
                continue;
            }
            try {
                grammar.convertAbstractQueryToXml(query);
                ok[i] = true;
            } catch (Exception e) {
                errors[i] = e.getMessage();
            }
            if (ok[i] == true) {
                passCount++;
            }
        }
    }

    public static void main(String args[]) {
        GrammarTest tester = new GrammarTest();
        tester.runAllTests();
        tester.printResults();
    }
}
