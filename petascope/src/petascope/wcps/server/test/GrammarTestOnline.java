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

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import org.apache.commons.io.FileUtils;

/** Runs the available Grammar tests against a deployed version of Petascope,
 * available at some URL.
 *
 * Assumes that a tests succeeds if the server does not throw or display
 * an error.
 *
 * @author Andrei Aiordachioaie
 */
public class GrammarTestOnline {
    // Put new test cases in this folder
//    public final String PetascopeURL = "http://localhost:8080/PetaScope/WCPService";
//    public final String PetascopeURL = "http://localhost:8080/petascope/wcps/";

    public final String PetascopeURL = "http://kahlua.eecs.jacobs-university.de:8080/petascope-new/wcps/";
    String folder = "test/testcases-wcps/";
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
        System.out.println("\n \nRESULTS \n");

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
        System.out.println("Tested PetaScope implementation from: " + PetascopeURL);
        System.out.println("Tests succeeded: " + String.valueOf(passCount));
        System.out.println("Tests failed: " + String.valueOf(numTests - passCount));
    }

    public GrammarTestOnline() {
        // Find out how many tests we have to run
        File dir = new File(folder);
        System.out.println("Looking for tests in " + dir.getAbsolutePath() + "\n");
        TestFileFilter filter = new TestFileFilter();
        tests = dir.listFiles(filter);
        numTests = tests.length;
//        numTests = 3;
        ok = new boolean[numTests];
        errors = new String[numTests];
        queries = new String[numTests];
    }

    /* Accept all files with extension TEST. */
    private class TestFileFilter implements FilenameFilter {

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
        String tname = "";
        for (int i = 0; i < numTests; i++) {
            ok[i] = false;
            tname = tests[i].getName();
            tname = tname.substring(0, tname.length() - 5);
            System.out.println("Running test '" + tname + "'...");
            try {
                query = FileUtils.readFileToString(tests[i]);
                queries[i] = query;
            } catch (IOException e) {
                errors[i] = "Could not read file " + tests[i].getName();
                continue;
            }
            try {
                String err = runOneTest("query", query);
                if (err == null) {
                    ok[i] = true;
                } else {
                    errors[i] = err;
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
            if (ok[i] == true) {
                passCount++;
            }
        }
    }

    /** Send an XML request to the WCPS server. Hopefully it will succeed.
     * Returns a message on error or null otherwise.
     **/
    public String runOneTest(String param, String query)
            throws MalformedURLException, IOException {

//        System.out.println("--------------------");
//        System.out.println(query);
//        System.out.println("\t--------------------");

        // connect to the servlet
        URL servlet = new URL(PetascopeURL);
        HttpURLConnection conn = (HttpURLConnection) servlet.openConnection();

        // inform the connection that we will send output and accept input
        conn.setDoInput(true);
        conn.setDoOutput(true);

        // Don't use a cached version of URL connection.
        conn.setUseCaches(false);
        conn.setDefaultUseCaches(false);

        // Send POST request
        conn.setRequestMethod("POST");

        // Specify the content type that we will send binary data
        conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");

        String data = param + "=" + URLEncoder.encode(query);
        DataOutputStream out = new DataOutputStream(conn.getOutputStream());
        out.writeBytes(data);
        out.flush();
        out.close();

        BufferedReader cgiOutput = new BufferedReader(
                new InputStreamReader(conn.getInputStream()));
        String line1 = cgiOutput.readLine();
        String line2 = cgiOutput.readLine();
        String line3 = cgiOutput.readLine();
        System.out.println("\t" + line1);
        System.out.println("\t" + line2);
        System.out.println("\t" + line3);

        if (line1 != null && line2 != null && line3 != null
                && line2.equals("<h1>An error has occured</h1>")) {
            while (cgiOutput.ready()) {
                System.out.println("\t" + cgiOutput.readLine());
            }
//            System.out.println("Error executing query: ");
            String error = line3.substring(10, line3.length() - 4);
//            System.out.println("\t" + error);
            return error;
        } else {
            return null;
        }

    }

    public static void main(String args[]) {
        GrammarTestOnline tester = new GrammarTestOnline();
        tester.runAllTests();
        tester.printResults();
    }
}
