/*
 * This file is part of PetaScope.
 *
 * PetaScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * PetaScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with PetaScope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.PetaScope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */


package wcst.transaction.tools;

//~--- non-JDK imports --------------------------------------------------------

import org.odmg.*;

import rasj.*;

import wcst.transaction.executeTransaction.ShowImage;

//~--- JDK imports ------------------------------------------------------------

import java.awt.image.BufferedImage;

import java.io.IOException;

import java.net.URL;

import javax.imageio.ImageIO;

import javax.swing.JFrame;

/**
 *
 * @author Andrei Aiordachioaie
 */
public class TestRasdamanUtils
{
	private static RasdamanUtils myUtils;

	public static void main(String[] args) throws ODMGException, RasResultIsNoIntervalException, IOException, Exception
	{
		myUtils = new RasdamanUtils("http://kahlua.eecs.jacobs-university.de" + ":7001", "RASBASE");

		myUtils.init();

//      myUtils.insertGrayImageAsArray("andreiTest1");
//      myUtils.loadCoverage("mr");
//      myUtils.printAllCollections();
//      testLoadRasdaman("rgb");
		testLoadRasdaman("andreiWedding1");
//      insertImageFromInternet("andreiFlickr1", "http://farm4.static.flickr.com/3347/3500129555_137c537e75_m.jpg");
//      myUtils.deleteCollection("andreiFlickr1");
//      myUtils.commitAndClose();
//      myUtils.deleteCollection("flickr1");

//      updateImageFromInternet("andreiFlickr1", "http://localhost/peta/car.jpeg");

//      try
//      {
//          myUtils.deleteCollection("andreiFlickr1");
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

		try
		{
			myUtils.commitAndClose();
		}
		catch (Exception e)
		{
			e.printStackTrace();
			myUtils.abortAndClose();
		}
	}

	private static void insertImageFromInternet(String identifier, String href)
	{
		try
		{
			// Step 1: fetch the image from Internet
			URL url = new URL(href);
			BufferedImage img0 = ImageIO.read(url);
			BufferedImage img = myUtils.convertImageToGray(img0);

			// Display image for check
			displayImage(identifier, img);

			// Step 2: Insert image into rasdaman
			myUtils.insertGrayImageAsArray(identifier, img);
		}
		catch (Exception e)
		{
			System.err.println("Error: " + e.getMessage());
			e.printStackTrace();
		}
	}

	private static void updateImageFromInternet(String identifier, String href)
	{
		try
		{
			// Step 1: fetch the image from Internet
			URL url = new URL(href);
			BufferedImage img0 = ImageIO.read(url);
			BufferedImage img = myUtils.convertImageToGray(img0);

			// Display image for check
			displayImage(identifier, img);

			// Step 2: Update image into rasdaman
			myUtils.updateGrayImageWithArray(identifier, img, 0, 0, img.getWidth(), img.getHeight());
		}
		catch (Exception e)
		{
			System.err.println("Error: " + e.getMessage());
			e.printStackTrace();
		}
	}

	private static void testLoadRasdaman(String identifier)
	{
		try
		{
			BufferedImage img = myUtils.loadCoverage(identifier);

			if ( img == null )
			{
				throw new Exception("Did not receive image from MyRasUtils !");
			}
			displayImage(identifier, img);
		}
		catch (Exception e)
		{
			System.err.println("Error !");
			e.printStackTrace();
		}
	}

	public static void displayImage(String name, BufferedImage img)
	{
		JFrame f = new JFrame("Image " + name);

		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		f.getContentPane().add(new ShowImage(img));
		f.setSize(img.getWidth() + 50, img.getHeight() + 50);
		f.setVisible(true);
	}
}
