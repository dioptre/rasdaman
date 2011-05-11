/*
 * JOMDoc - A Java library for OMDoc documents (http://omdoc.org/jomdoc).
 *
 * Original author    Dimitar Misev <d.misev@jacobs-university.de>
 * Web                http://kwarc.info/dmisev/
 * Created            Dec 16, 2010, 10:29:24 AM
 *
 * Filename           $Id$
 * Revision           $Revision$
 * Last modified on   $Date$
 *               by   $Author$
 *
 * Copyright (C) 2007,2008 the KWARC group (http://kwarc.info)
 * Licensed under the GNU Public License v3 (GPL3).
 * For other licensing contact Michael Kohlhase <m.kohlhase@jacobs-university.de>
 */
package petascope.wcs2;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import javax.servlet.ServletException;
import org.apache.commons.io.FileUtils;
import org.junit.Assert;
import org.junit.Test;
import org.slf4j.LoggerFactory;
import petascope.BaseTestCase;
import petascope.ConfigManager;
import petascope.core.DbMetadataSource;
import petascope.exceptions.PetascopeException;
import petascope.util.IOUtil;
import petascope.wcs2.extensions.ExtensionsRegistry;
import petascope.wcs2.extensions.ProtocolExtension;
import petascope.wcs2.handlers.Response;

/**
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class ExamplesTest extends BaseTestCase {
    
    private static final org.slf4j.Logger log = LoggerFactory.getLogger(ExamplesTest.class);
    
    private static final boolean RUN = true;

    private List<File> files;
    private final DbMetadataSource meta;
    private PrintStream l;

    public ExamplesTest() throws PetascopeException, ServletException {
        files = new ArrayList<File>(FileUtils.listFiles(new File(TESTDATA_PATH), new String[]{"in"}, false));
        Collections.sort(files, new Comparator<File>() {

            @Override
            public int compare(File o1, File o2) {
                return o1.getName().compareTo(o2.getName());
            }
        });

        ConfigManager.getInstance(PETASCOPE_PATH + "db/settings.properties", PETASCOPE_PATH);
        meta = new DbMetadataSource(ConfigManager.METADATA_DRIVER,
                ConfigManager.METADATA_URL,
                ConfigManager.METADATA_USER,
                ConfigManager.METADATA_PASS, false);
        
        try {
            l = new PrintStream(new File(PETASCOPE_PATH + "test.log"));
        } catch (FileNotFoundException ex) {
            try {
                l = new PrintStream(new File("test.log"));
            } catch (FileNotFoundException ex1) {
                l = System.out;
            }
        }
    }

    @Test
    public void testAll() {
        for (File file : files) {
            try {
                String input = FileUtils.readFileToString(file);
                String fileName = IOUtil.removeExtension(file.getName());
                String[] s = fileName.split("-");
                l.println("REQUEST (Operation: " + s[0] + ", Protocol: " + s[1] + ")\n" + input + "\n\nRESPONSE");
                
                ProtocolExtension extension = ExtensionsRegistry.getProtocolExtension(input);
                Response res = extension.handle(input, meta);
                
                String response = humanReadableResponse(file, res);
                File exp = new File(IOUtil.removeExtension(file.getPath()) + ".exp");
                
                if (!RUN) {
                    FileUtils.writeStringToFile(exp, response);
                } else {
                    String expected = FileUtils.readFileToString(exp);
                    l.println(response);
                    Assert.assertEquals("Testing " + fileName + " failed", expected, response);
                    l.println("PASSED\n--------------------------------------------------------");
                }
            } catch (Exception ex) {
                log.warn("Error executing test for input file " + file.getName(), ex);
            }
        }
        
        System.out.println("");
        System.out.println("---------------------------------------------------");
        System.out.println("DONE");
        System.out.println("All tests passed, please see test.log for details");
    }
    
    private String humanReadableResponse(File input, Response res) throws IOException {
        String ret = "";
        
        if (res.getMimeType() != null) {
            if (res.getData() != null && res.getXml() != null && res.getMimeType() != null) {
                ret += "MEDIATYPE=multipart/mixed, FORMAT=" + res.getMimeType();
            } else {
                ret += "MEDIATYPE=" + res.getMimeType();
            }
            ret += "\n\n";
        }
        
        if (res.getXml() != null) {
            ret += "GML:\n" + res.getXml() + "\n\n";
        }
        if (res.getData() != null) {
            File output = new File(IOUtil.removeExtension(input.getPath()) + ".dat");
            FileUtils.writeByteArrayToFile(output, res.getData());
            ret += "Binary data of length " + output.length() + " in " + output + "\n";
        }
        
        return ret;
    }
}