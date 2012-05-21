/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.util;

import java.io.File;
import java.io.FileInputStream;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarInputStream;
import org.apache.commons.io.filefilter.FileFileFilter;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * I/O utility methods.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class IOUtil {

    private static final Logger log = LoggerFactory.getLogger(IOUtil.class);

    public static String wrapDir(String dir) {
        if (dir.endsWith(File.separator)) {
            return dir;
        } else {
            return dir + File.separator;
        }
    }

    public static String removeScheme(String uri) {
        if (uri == null || uri.equals("")) {
            return "";
        }
        int ind = uri.indexOf(":");
        if (ind != -1) {
            return uri.substring(ind + 1);
        }
        return uri;
    }

    public static String removeExtension(String uri) {
        return removeLastPart(uri, ".");
    }

    public static String removeFragment(String uri) {
        return removeLastPart(uri, "#");
    }

    /**
     * @return <code>uri</code> without the part starting with delimiter
     */
    public static String removeLastPart(String uri, String delimiter) {
        if (uri == null || uri.equals("")) {
            return "";
        }
        int ind = uri.lastIndexOf(delimiter);
        if (ind != -1) {
            return uri.substring(0, ind);
        }
        return uri;
    }

    /**
     * @return the scheme part of <code>uri</code>, or null if it doesn't have a scheme.
     */
    public static String getScheme(String uri) {
        String ret = removeLastPart(uri, ":");
        if (!ret.equals(uri)) {
            return ret;
        } else {
            return null;
        }
    }

    public static List<String> filesInDir(String dir) {
        List<String> ret = new ArrayList<String>();
        URL uri = IOUtil.class.getClassLoader().getResource(dir);
        if (uri == null) {
            return ret;
        }
        String path = uri.toString();
        if (path.startsWith("jar:")) {
            return filesInJarDir(dir);
        }

        String[] res = new File(removeScheme(path)).list(FileFileFilter.FILE);
        for (String f : res) {
            ret.add(wrapDir(dir) + f);
        }
        return ret;
    }

    public static List<String> filesInJarDir(String jarDir) {
        List<String> ret = new ArrayList<String>();
        JarInputStream jfile = null;
        try {
            // path to the jar
            String jarPath = IOUtil.class.getResource("").getPath();
            jarPath = jarPath.substring(0, jarPath.indexOf("!"));

            File file = new File(new URI(jarPath));
            jfile = new JarInputStream(new FileInputStream(file));
            JarEntry entry = null;
            do {
                try {
                    entry = jfile.getNextJarEntry();
                    if (entry == null) {
                        continue;
                    }
                    String sentry = entry.toString();
                    if (("/" + sentry).contains(jarDir) && !sentry.endsWith("/")) {
                        ret.add(sentry);
                    }
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            } while (entry != null);
        } catch (Exception ex) {
            ex.printStackTrace();
        } finally {
            try {
                jfile.close();
            } catch (Exception ex) {
            }
            return ret;
        }
    }

    /**
     * @return all filenames in dir that match the regular expression re
     */
    public static List<String> filesInPackage(String dir, String ext) {
        List<String> ret = null;
        try {
            ret = filesInDir(dir);
        } catch (Exception ex) {
        }
        List<String> res = new ArrayList<String>();
        if (ret != null) {
            for (int i = 0; i < ret.size(); i++) {
                String f = ret.get(i);
                if (f.endsWith(ext)) {
                    res.add(removeExtension(getFilename(f)));
                }
            }
        }
        return res;
    }

    public static String getPath(String path) {
        File file = new File(path);
        if (file.isDirectory()) {
            return wrapDir(file.getPath());
        }
        String parent = file.getParent();
        if (parent != null) {
            return wrapDir(parent);
        } else {
            return "";
        }
    }

    public static String getFilename(String path) {
        int ind = path.lastIndexOf(File.separator);
        if (ind != -1) {
            return path.substring(ind + 1);
        }
        return path;
    }

    /**
     * Find a file in currentDir or its parent directories
     *
     * @param fileName
     * @param currentDir
     * @param depth limit the number of parent directories to this depth, or -1 is infinite
     * @return the found file, or null
     */
    public static File findFile(String fileName, File currentDir, int depth) {
        File ret = null;
        int i = 0;

        while (currentDir != null && (depth == -1 || i++ < depth)) {
            ret = new File(currentDir.getPath() + File.separator + fileName);
            if (ret != null && ret.exists()) {
                return ret;
            }
            currentDir = currentDir.getParentFile();
        }

        return null;
    }

    /**
     * Find a file in currentDir or its parent directories
     *
     * @param fileName
     * @param currentDir
     * @param depth limit the number of parent directories to this depth, or -1 is infinite
     * @return the found file, or null
     */
    public static File findFile(String fileName, URL currentDir, int depth) {
        try {
            return findFile(fileName, new File(currentDir.toURI()), depth);
        } catch (URISyntaxException ex) {
            log.warn("URI error", ex);
            return null;
        }
    }
}
