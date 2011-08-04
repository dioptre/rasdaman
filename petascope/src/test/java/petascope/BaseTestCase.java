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
package petascope;

import java.io.File;
import java.io.IOException;
import nu.xom.Document;
import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.util.IOUtil;
import petascope.util.XMLUtil;

/**
 * Provides some utility methods and path constants to JUnit tests.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public abstract class BaseTestCase {
    
    private static final Logger log = LoggerFactory.getLogger(BaseTestCase.class);
    
    public static final String PETASCOPE_PATH =  getPetascopePath();
    public static final String TESTDATA_PATH = PETASCOPE_PATH +  "src/test/java/testdata";
    public static final String DB_SETTINGS = PETASCOPE_PATH +  "db/settings.properties";
    public static final String XML_OGC_PATH = PETASCOPE_PATH +  "xml/ogc/";
    public static final String WCS_PATH = XML_OGC_PATH + "wcs/2.0.0/";
    public static final String WCS_EXAMPLES_PATH = WCS_PATH + "examples/";
    public static final String WCS_GRID_COV_PATH = WCS_EXAMPLES_PATH + "discreteGridPointCoverage/";

    public static String getPetascopePath() {
        File ret = IOUtil.findFile("src", BaseTestCase.class.getResource(""), 10);
        ret = ret.getParentFile();
        return ret + "/";
    }

    public static void rm(String file) {
        if (file != null) {
            rm(new File(file));
        }
    }

    public static void rm(File file) {
        if (file != null) {
            file.delete();
            file.delete();
        }
    }

    public static void rmdir(String dir) {
        if (dir != null) {
            rmdir(new File(dir));
        }
    }

    public static void rmdir(File dir) {
        if (dir == null || !dir.isDirectory()) {
            return;
        }
        try {
            FileUtils.deleteDirectory(dir);
        } catch (IOException ex) {
            log.warn("Error deleting dir " + dir, ex);
        }
    }

    /**
     * @return the contents of file as String or empty string in case of an error
     */
    public static String toString(String file) {
        try {
            return toString(new File(file));
        } catch (Exception ex) {
            return "";
        }
    }

    /**
     * @return the contents of file as String or empty string in case of an error
     */
    public static String toString(File file) {
        try {
            return FileUtils.readFileToString(file);
        } catch (Exception ex) {
            return "";
        }
    }
    
    public static Document toDoc(String file) {
        try {
            return XMLUtil.buildDocument(new File(file));
        } catch (Exception ex) {
            log.warn("Error building document " + file, ex);
        }
        return null;
    }
    
    
}
