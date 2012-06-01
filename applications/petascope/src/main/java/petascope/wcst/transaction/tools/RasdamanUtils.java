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
package petascope.wcst.transaction.tools;

import org.odmg.DBag;
import org.odmg.Database;
import org.odmg.Implementation;
import org.odmg.ODMGException;
import org.odmg.OQLQuery;
import org.odmg.Transaction;
import rasj.RasGMArray;
import rasj.RasImplementation;
import rasj.RasMInterval;
import rasj.RasResultIsNoIntervalException;
import petascope.exceptions.WCSException;
import petascope.exceptions.ExceptionCode;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.awt.image.DataBuffer;
import java.io.ByteArrayInputStream;
import java.util.Iterator;
import petascope.ConfigManager;
import petascope.exceptions.RasdamanException;
import javax.imageio.ImageIO;
import javax.imageio.stream.ImageInputStream;

/**
 * Utility class for doing various tasks that involve the Rasdaman server.
 *
 * @author Andrei Aiordachioaie
 */
public class RasdamanUtils {

    RasImplementation myApp = null;
    Database myDb = null;
    Transaction myTa = null;
    private String base;
    private String server;

    /**
     * Simple constructor
     *
     * @param s server string
     * @param b database name
     */
    public RasdamanUtils(String s, String b) {
        this.server = s;
        this.base = b;
    }

    public void log(String msg) {
        System.err.flush();
        System.out.println("Rasdaman Utils: " + msg);
        System.out.flush();
    }

    /**
     * Opens a new connection to the Rasdaman server, and starts a new transaction.
     * @throws WCSException on connection error
     */
    public void init() throws RasdamanException {
        try {
            if (myApp == null) {
                myApp = new RasImplementation(server );
                myApp.setUserIdentification(ConfigManager.RASDAMAN_USER, ConfigManager.RASDAMAN_PASS);
            }
            if (myDb == null) {
                myDb = myApp.newDatabase();
                log("Opening database...");
                myDb.open(base, Database.OPEN_READ_WRITE);
            }
            if (myTa == null) {
                log("Beginning new transaction ...");
                myTa = myApp.newTransaction();
                myTa.begin();
            }
        } catch (ODMGException e) {
            throw new RasdamanException(ExceptionCode.InternalComponentError,
                    "Could not connect to the Rasdaman server !", e);
        }
    }

    public void commitAndClose() throws ODMGException {
        if (myTa != null) {
            log("Commiting transaction ...");
            myTa.commit();
            myTa = null;
        }
        if (myDb != null) {
            log("Closing database...");
            myDb.close();
            myDb = null;
        }
        log("Finished !");
    }

    public void abortAndClose() throws ODMGException {
        if (myTa != null) {
            log("Aborting transaction...");
            myTa.abort();
            myTa = null;
        }
        if (myDb != null) {
            log("Closing database...");
            myDb.close();
            myDb = null;
        }
        log("Finished !");
    }

    public BufferedImage loadCoverage(String name) throws Exception {
        init();

        log("Running Load Coverage " + name + " query!");
        OQLQuery myQu = myApp.newOQLQuery();

        myQu.create("select jpeg(" + name + ") from " + name);
        DBag resultSet = (DBag) myQu.execute();

        if (resultSet != null) {
            log("Query successful !");
            Iterator iter = resultSet.iterator();

            if (iter.hasNext()) {
                RasGMArray result = (RasGMArray) iter.next();
                byte[] ba = result.getArray();
                ImageInputStream bas = ImageIO.createImageInputStream(ba);
                BufferedImage bufImg = null;

                try {
                    bufImg = javax.imageio.ImageIO.read(bas);
                } catch (Exception e) {
                    System.err.println("Error decoding the image !!!");
                }

                commitAndClose();

                return bufImg;
            }
        }

        abortAndClose();

        return null;
    }

    public RasGMArray createMDD() throws RasResultIsNoIntervalException {
        // create 2-D MDD with cell length 1, i.e., type “byte”:
        RasGMArray myMDD = new RasGMArray(new RasMInterval("[1:400,1:400]"), 1);
        // byte container for array data, matching in size:
        byte[] mydata = new byte[160000];

        // initialize array as grid of gray and black stripes:
        for (int y = 0; y < 400; y++) {
            for (int x = 0; x < 400; x++) {
                mydata[y * 399 + x] = (byte) ((y % 4) > 1
                        ? 80
                        : 0);
            }
        }
        // now insert byte array into MDD object
        // (sets only the pointer, no copying takes place!):
        myMDD.setArray(mydata);

        return myMDD;
    }

    public void insertGrayImageAsArray(String name) throws ODMGException {
        try {
            init();

            log("Creating grey image ...");
            RasGMArray myMDD = createMDD();
            // set up query object for collection creation:
            OQLQuery myQu = myApp.newOQLQuery();

            myQu.create("create collection " + name + " GreySet");
            // set the object type name (used for server type checking):
            myMDD.setObjectTypeName("GreyImage");
            // finally, execute “create collection” statement:
            log("Creating collection '" + name + "'");
            myQu.execute();
            // now create the insert statement:
            myQu.create("insert into " + name + " values $1");
            // let the server generate a new OID for the object to be
            // inserted, and remember this OID locally:
            String myNewOID = myApp.getObjectId(myMDD);

            // bind the MDD value which substitutes formal parameter $1:
            myQu.bind(myMDD);
            // …and ship the complete statement to the server:
            log("Inserting data into the collection");
            myQu.execute();
            log("Done !");

            commitAndClose();
        } catch (Exception e) {
            e.printStackTrace();
            abortAndClose();
        }
    }

    public void updateGrayImageWithArray(String name, BufferedImage img, int x1, int y1, int x2,
            int y2) {
        try {
            log("Building query for updating gray image ...");
            RasGMArray myMDD = createMddFromImage(img);

            myMDD.setObjectTypeName("GreyImage");
            // set up query object for collection creation:
            OQLQuery myQu = myApp.newOQLQuery();
            // Where to insert new image
            String interval = String.valueOf(x1) + ":" + String.valueOf(x2) + ","
                    + String.valueOf(y1) + ":" + String.valueOf(y2);
            // Size of new image
            String interval2 = String.valueOf(0) + ":" + String.valueOf(x2 - x1) + ","
                    + String.valueOf(0) + ":" + String.valueOf(y2 - y1);

            myQu.create("update " + name + " AS c SET c[" + interval + "] "
                    + "assign maaray x in [" + interval2 + "] values $1");
            // let the server generate a new OID for the object to be
            // inserted, and remember this OID locally:
            String myNewOID = myApp.getObjectId(myMDD);

            // bind the MDD value which substitutes formal parameter $1:
            myQu.bind(myMDD);
            // …and ship the complete statement to the server:
            log("Updating image ...");
            myQu.execute();
            log("Done !");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void insertGrayImageAsArray(String name, BufferedImage img) throws ODMGException, RasdamanException {
        log("Creating grey image ...");
        RasGMArray myMDD = createMddFromImage(img);
        // set up query object for collection creation:
        OQLQuery myQu = myApp.newOQLQuery();

        myQu.create("create collection " + name + " GreySet");
        // set the object type name (used for server type checking):
        myMDD.setObjectTypeName("GreyImage");
        // finally, execute “create collection” statement:
        log("Creating collection '" + name + "'");
        myQu.execute();
        // now create the insert statement:
        myQu.create("insert into " + name + " values $1");
        // let the server generate a new OID for the object to be
        // inserted, and remember this OID locally:
        String myNewOID = myApp.getObjectId(myMDD);

        // bind the MDD value which substitutes formal parameter $1:
        myQu.bind(myMDD);
        // …and ship the complete statement to the server:
        log("Inserting data into the collection");
        myQu.execute();
        log("Done !");
    }

    public void insertColorImageAsArray(String name, BufferedImage img) throws ODMGException {
        try {
            log("Creating grey image ...");
            RasGMArray myMDD = createMddFromImage(img);
            // set up query object for collection creation:
            OQLQuery myQu = myApp.newOQLQuery();

            myQu.create("create collection " + name + " RGBSet");
            // set the object type name (used for server type checking):
            myMDD.setObjectTypeName("RGBImage");
            // finally, execute “create collection” statement:
            log("Creating collection '" + name + "'");
            myQu.execute();
            // now create the insert statement:
            myQu.create("insert into " + name + " values $1");
            // let the server generate a new OID for the object to be
            // inserted, and remember this OID locally:
            String myNewOID = myApp.getObjectId(myMDD);

            // bind the MDD value which substitutes formal parameter $1:
            myQu.bind(myMDD);
            // …and ship the complete statement to the server:
            log("Inserting data into the collection");
            myQu.execute();
            log("Done !");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void insertImageAsJpeg(String name, byte[] img, int maxX, int maxY) throws ODMGException {
        try {
            init();

            log("Creating image from bytes ...");
            RasGMArray myMDD = createRasArrayFromBytes(img, maxX, maxY);
            // set up query object for collection creation:
            OQLQuery myQu = myApp.newOQLQuery();

            myQu.create("create collection " + name + " GreySet");
            // set the object type name (used for server type checking):
            myMDD.setObjectTypeName("GreyImage");
            // finally, execute “create collection” statement:
            log("Creating collection '" + name + "'");
            myQu.execute();
            // now create the insert statement:
            myQu.create("insert into " + name + " values inv_jpeg($1)");
            // let the server generate a new OID for the object to be
            // inserted, and remember this OID locally:
            String myNewOID = myApp.getObjectId(myMDD);

            // bind the MDD value which substitutes formal parameter $1:
            myQu.bind(myMDD);
            // …and ship the complete statement to the server:
            log("Inserting data into the collection");
            myQu.execute();
            log("Done !");

            commitAndClose();
        } catch (Exception e) {
            System.out.flush();
            e.printStackTrace();
            System.err.flush();
            abortAndClose();
        }
    }

    private RasGMArray createRasArrayFromBytes(byte[] bytes, int maxX, int maxY)
            throws RasResultIsNoIntervalException {
        // create 2-D MDD with cell length 1, i.e., type “byte”:
        String strX = String.valueOf(maxX), strY = String.valueOf(maxY);
        String interval = "[1:" + strX + "],1:" + strY + "]";
        // FIXME: maybe change cell type?
        int cellType = 1;

        // create the RasGMArray object
        RasGMArray myMDD = new RasGMArray(new RasMInterval(interval), cellType);

        myMDD.setArray(bytes);

        return myMDD;
    }

    public void deleteCollection(String name)
            throws ODMGException, ODMGException, ODMGException, Exception {
        try {
//          init();

            log("Creating query...");
            // set up query object for collection creation:
            OQLQuery myQu = myApp.newOQLQuery();

            myQu.create("drop collection " + name);
            // finally, execute “create collection” statement:
            log("Executing deletion query...");
            myQu.execute();
            // now create the insert statement:
            log("Query finished !");

//          commitAndClose();
        } catch (Exception e) {
            e.printStackTrace();

            throw e;
//          abortAndClose();
        }
    }

    public void printAllCollections() throws ODMGException {
        String query = "select r from RAS_COLLECTIONNAMES as r";

//      init();

        log("Querying for all available collections ...");
        OQLQuery myQu = myApp.newOQLQuery();

        myQu.create(query);

        DBag resultSet = (DBag) myQu.execute();

        if (resultSet != null) {
            Iterator iter = resultSet.iterator();

            while (iter.hasNext()) {
                RasGMArray result = (RasGMArray) iter.next();
                byte[] ba = result.getArray();
                String name = new String(ba);

                if (name.endsWith("\u0000")) {
                    name = name.substring(0, name.length() - (new String("\u0000")).length());
                }

                System.out.println("* " + name);
            }
        }

//      commitAndClose();
    }

    public RasGMArray createMddFromImage(BufferedImage img) throws RasdamanException {
        String interval = null;
        try {
            int cols = img.getWidth();
            int rows = img.getHeight();
            // create 2-D MDD with cell length 1, i.e., type “byte”:
            interval = "[1:" + cols + ",1:" + rows + "]";
            int cellType = 1;

            // create the RasGMArray object
            RasGMArray myMDD = new RasGMArray(new RasMInterval(interval), cellType);
            DataBuffer db = img.getData().getDataBuffer();
            byte[] bytes = new byte[cols * rows];


            System.out.println("Processing image with size: " + interval);
            System.out.println("Created vector with size: " + cols * rows);
            System.out.println("Processing data with size: " + db.getSize());
            int i = 0;

            /*
             *  Java image data-types store an image as a 1-D array, reading the
             * image row-wise. Rasdaman stores the image column-wise. So we need to
             * convert between the two representations.
             */
            for (int c = 0; c < cols; c++) {
                for (int r = 0; r < rows; r++) {
                    bytes[i++] = (byte) db.getElem(r * cols + c);
                }
            }
            myMDD.setArray(bytes);

            return myMDD;
        } catch (RasResultIsNoIntervalException e) {
            throw new RasdamanException(ExceptionCode.InternalComponentError, "Illegal Interval String: " + interval, e);
        }
    }

    public BufferedImage convertImageToGray(BufferedImage img) {
        BufferedImage image = new BufferedImage(img.getWidth(), img.getHeight(),
                BufferedImage.TYPE_BYTE_GRAY);
        Graphics g = image.getGraphics();

        g.drawImage(img, 0, 0, null);
        g.dispose();

        return image;
    }
}
