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

import java.awt.Graphics;
import java.awt.Panel;
import org.odmg.*;
import rasj.*;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.URL;
import javax.imageio.ImageIO;
import javax.swing.JFrame;

/** Test class for various Rasdaman tasks. Comment/uncomment the lines in the
 * main() function and see what happens.
 *
 * @author Andrei Aiordachioaie
 */
public class TestRasdamanUtils {

    private static RasdamanUtils myUtils;

    public static void main(String[] args) throws ODMGException, RasResultIsNoIntervalException, IOException, Exception {
//		myUtils = new RasdamanUtils("http://kahlua.eecs.jacobs-university.de" + ":7001", "RASBASE");
        myUtils = new RasdamanUtils("http://localhost" + ":7001", "RASBASE");

        myUtils.init();

        /* Try to comment/uncomment lines below ... */
//      myUtils.insertGrayImageAsArray("andreiTest1");
//      myUtils.loadCoverage("mr");
//      myUtils.printAllCollections();
//      testLoadRasdaman("andreiTest1");
//		testLoadRasdaman("andreiFlickrColor");
//      insertImageFromInternet("andreiFlickr1", "http://farm4.static.flickr.com/3347/3500129555_137c537e75_m.jpg");
        myUtils.deleteCollection("andreiFlickrColor");
//      myUtils.commitAndClose();
//      myUtils.deleteCollection("flickr1");

//      updateImageFromInternet("andreiFlickr1", "http://localhost/peta/car.jpeg");

//        insertImageFromInternet("andreiFlickrColor", "http://localhost/peta/flickr1.jpg");

//      try
//      {
//          myUtils.deleteCollection("andreiFlickrColor");
//          myUtils.commitAndClose();
//      }
//      catch (Exception e)
//      {
//          e.printStackTrace();
//          myUtils.abortAndClose();
//      }

//      System.err.println("---------------------");
//      insertImageFromInternet("andreiWedding1", "http://farm3.static.flickr.com/2483/3758629356_d551cd5e6e.jpg");
//      System.err.println("---------------------");
//      testLoadRasdaman("andreiWedding1");

        /* Stop commenting/uncommenting lines here */

        try {
            myUtils.commitAndClose();
        } catch (Exception e) {
            e.printStackTrace();
            myUtils.abortAndClose();
        }

//        understandColorImage("http://localhost/peta/flickr1.jpg");
    }

    private static void insertImageFromInternet(String identifier, String href) {
        try {
            // Step 1: fetch the image from Internet
            URL url = new URL(href);
            BufferedImage img0 = ImageIO.read(url);
            BufferedImage img = myUtils.convertImageToGray(img0);

            // Display image for check
            displayImage(identifier, img);

            // Step 2: Insert image into rasdaman
            myUtils.insertGrayImageAsArray(identifier, img);
//			myUtils.insertColorImageAsArray(identifier, img);
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private static void updateImageFromInternet(String identifier, String href) {
        try {
            // Step 1: fetch the image from Internet
            URL url = new URL(href);
            BufferedImage img0 = ImageIO.read(url);
            BufferedImage img = myUtils.convertImageToGray(img0);

            // Display image for check
            displayImage(identifier, img);

            // Step 2: Update image into rasdaman
            myUtils.updateGrayImageWithArray(identifier, img, 0, 0, img.getWidth(), img.getHeight());
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private static void testLoadRasdaman(String identifier) {
        try {
            BufferedImage img = myUtils.loadCoverage(identifier);

            if (img == null) {
                throw new Exception("Did not receive image from MyRasUtils !");
            }
            displayImage(identifier, img);
        } catch (Exception e) {
            System.err.println("Error !");
            e.printStackTrace();
        }
    }

    public static void displayImage(String name, BufferedImage img) {
        JFrame f = new JFrame("Image " + name);

        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.getContentPane().add(new ShowImage(img));
        f.setSize(img.getWidth() + 50, img.getHeight() + 50);
        f.setVisible(true);
    }

    private static void understandColorImage(String href) {
        String identifier = "image1";
        try {
            // Step 1: fetch the image from Internet
            URL url = new URL(href);
            BufferedImage img0 = ImageIO.read(url);
            BufferedImage img = myUtils.convertImageToGray(img0);

            // Display image for check
            displayImage(identifier, img);

            // Step 2: Insert image into rasdaman
            System.out.println(img);        // gray-image
            System.out.println(img0);       // color image
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
            e.printStackTrace();
        }
    }

    /**
     * Panel to display loaded image
     *
     *
     * @version        09.Jul 2009
     * @author         Andrei Aiordachioaie
     */
    public static class ShowImage extends Panel {

        BufferedImage image;

        public ShowImage(BufferedImage img) {
            this.image = img;
        }

        public void paint(Graphics g) {
            if (image != null) {
                g.drawImage(image, 0, 0, null);
            }
        }
    }
}
