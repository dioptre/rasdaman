package rasj.test;

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
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
*/
/** ***********************************************************
 * <pre>
 *
 * PURPOSE: class for testing special MArrays
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

import java.io.*;
import java.net.*;
import rasj.clientcommhttp.*;
import rasj.odmg.*;
import org.odmg.*;
import rasj.*;
import java.util.*;

/**
 * class for testing special MArrays
 * for testing please enable test data output in toString method of RASGMArray
 * @version $$
 */
public class testMArrays
{
  public static void main(String[] args)
  {
    String serv = "localhost";
    String base = "RASBASE";
    String coll = "test";

    for (int i=args.length-1; i>=0; i--)
    {
      //System.out.println(args[i]);
      if (args[i].equals("-server"))
        serv = args[i+1];
      if (args[i].equals("-database"))
        base = args[i+1];
      if (args[i].equals("-collection"))
        coll = args[i+1];
    }
    //System.out.println(server+base+coll);
    testMArrays marrayTest = new testMArrays(serv);

  }

  public testMArrays(String server)
  {
    DBag resultBag = null;
    Object result = null;
    Transaction myTa = null;
    Database myDb = null;
    OQLQuery myQu = null;

    boolean equal = false;

    try
    {
      System.out.println("### Testing MArrays: ...");
      Implementation myApp = new RasImplementation("http://"+server+":7001");
      myDb = myApp.newDatabase();

      System.out.println("Opening database ...");
      myDb.open("RASBASE", Database.OPEN_READ_WRITE);
      myTa = myApp.newTransaction();

      int width, height, len;
      width = 18;
      height = 18;
      len = (width+1)*(height+1);
      RasMInterval domain = new RasMInterval("[0:"+width+",0:"+height+"]");
      RasMInterval domain2 = new RasMInterval("[1:3,1:3]");
      RasStorageLayout stl = new RasStorageLayout();
      stl.setTileSize(128);


      /**
       * test the GMArray
       */
      System.out.println("\n\n### Testing GMArray with OID: ################################################");
      myQu = myApp.newOQLQuery();

      myQu.create("drop collection test");
      myTa.begin();
      try
      {
        resultBag = (DBag)myQu.execute();
        myTa.commit();
        //System.out.println("collection dropped");
      }
      catch(ODMGException e)
      {
        myTa.abort();
        System.err.println("Collection test does not exist: " + e.getMessage());
      }

      myQu.create("create collection test GreySet");
      RasGMArray mddConst = new RasGMArray(domain, 1, stl);
      //RasGMArray mddConst = new RasGMArray(domain, 1);
      byte[] data = new byte[len];
      mddConst.setObjectTypeName("GreyImage");

      // test: get new OID from the server for GMArray and insert
      System.out.println("new OID from server: " + myApp.getObjectId(mddConst));

      for(int j = 0; j < data.length; j++)
        data[j] = (byte)j;

      mddConst.setArray(data);
      /*
      System.out.println("\nbyte array: ");
      for(int j=0; j<mddConst.getArray().length; j++)
      {
        System.out.print(" "+ mddConst.getArray()[j]);
      }
      */
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("\ncollection created");
      myQu.create("insert into test values $1 ");

      //System.out.println("mdd before sending: " + mddConst);
      myQu.bind(mddConst);
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection inserted");

      myQu = myApp.newOQLQuery();
      myQu.create("select img from test as img");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection selected");
      if (resultBag != null)
      {
	Iterator iter = resultBag.iterator();
	while (iter.hasNext())
	{
          result = iter.next();
          equal = true;
          for(int j=0; j<mddConst.getArray().length; j++)
          {
            if(mddConst.getArray()[j] != ((RasGMArray)result).getArray()[j])
              equal = false;
          }
          if(!(myApp.getObjectId(mddConst).toString()).equals(myApp.getObjectId((RasGMArray)result).toString()))
            equal = false;
          System.out.println("result mdd: " + ((RasGMArray)mddConst).toTestString());
          System.out.println("result mdd: " + ((RasGMArray)result).toTestString());
        }
          System.out.println("inserted and selected GMArray and OIDs are equal: " + equal);
          //System.out.println("All results for test GMArray");
      }


      // testing copy constructor
      System.out.println("### Testing copy constructor:");
      RasGMArray mddConst2 = new RasGMArray(mddConst);
      mddConst2.setArray(data);
      //System.out.println("\nbyte array2: ");
      data = null;
      equal = true;
      for(int j=0; j<mddConst.getArray().length; j++)
      {
        //System.out.print(" "+ mddConst2.getArray()[j]);
        if(mddConst.getArray()[j] != mddConst2.getArray()[j])
              equal = false;
      }
      System.out.println("Copy constructor is OK: " + equal);

      /**
       * test the MArrayByte
       */
      System.out.println("\n\n### Testing MArrayByte: ################################################");
      myQu = myApp.newOQLQuery();

      myQu.create("drop collection test");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection dropped");

      myQu.create("create collection test GreySet");
      RasMArrayByte mddByte = new RasMArrayByte(domain, stl);
      byte[] dataByte = new byte[len];
      mddByte.setObjectTypeName("GreyImage");

      for(int j = 0; j < dataByte.length; j++)
        dataByte[j] = (byte)j;

      mddByte.setArray(dataByte);

      //System.out.println("\nbyte array: ");
      //for(int j=0; j<mddByte.getArray().length; j++)
      //{
        //System.out.print(" "+ mddByte.getArray()[j]);
      //}

      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("\ncollection created");
      myQu.create("insert into test values $1 ");

      //System.out.println("mdd before sending: " + mddByte);
      myQu.bind(mddByte);
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection inserted");

      myQu = myApp.newOQLQuery();
      myQu.create("select img from test as img");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection selected");
      if (resultBag != null)
      {
	Iterator iter = resultBag.iterator();
	while (iter.hasNext())
	{
          result = iter.next();
          System.out.println("result mdd: " + ((RasGMArray)mddByte).toTestString());
          System.out.println("result mdd: " + ((RasGMArray)result).toTestString());
          equal = true;
          for(int j=0; j<mddByte.getArray().length; j++)
          {
            if(mddByte.getArray()[j] != ((RasGMArray)result).getArray()[j])
              equal = false;
          }
        }
        //System.out.println("All results for MArrayByte");
        System.out.println("inserted and selected MArrayByte are equal: " + equal);
      }

      // testing copy constructor
      System.out.println("### Testing copy constructor:");
      RasMArrayByte mddByte2 = new RasMArrayByte(mddByte);
      mddByte2.setArray(dataByte);
      //System.out.println("\nbyte array2: ");
      dataByte = null;
      equal = true;
      for(int j=0; j<mddByte.getArray().length; j++)
      {
        //System.out.print(" "+ mddByte2.getArray()[j]);
        if(mddByte.getArray()[j] != mddByte2.getArray()[j])
              equal = false;
      }
      System.out.println("Copy constructor is OK: " + equal);

      /**
       * test the MArrayInteger
       */
      System.out.println("\n\n### Testing MArrayInteger: ################################################");
      myQu = myApp.newOQLQuery();

      myQu.create("drop collection test");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection dropped");

      myQu.create("create collection test UShortSet");
      RasMArrayInteger mddInteger = new RasMArrayInteger(domain, stl);
      int[] dataInteger = new int[len];
      //byte[] dataInteger = new byte[144];
      mddInteger.setObjectTypeName("UShortImage");

      for(int j = 0; j < dataInteger.length; j++)
        dataInteger[j] = j;

      mddInteger.setArray(dataInteger);

      //System.out.println("\nbyte array: ");
      //for(int j=0; j<mddInteger.getArray().length; j++)
      //{
        //System.out.print(" "+ mddInteger.getArray()[j]);
      //}

      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("\ncollection created");
      myQu.create("insert into test values $1 ");

      //System.out.println("mdd before sending: " + mddInteger);
      myQu.bind(mddInteger);
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection inserted");

      myQu = myApp.newOQLQuery();
      myQu.create("select img from test as img");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection selected");
      if (resultBag != null)
      {
	Iterator iter = resultBag.iterator();
	while (iter.hasNext())
	{
          result = iter.next();
          System.out.println("result mdd: " + ((RasGMArray)mddInteger).toTestString());
          System.out.println("result mdd: " + ((RasGMArray)result).toTestString());
          equal = true;
          for(int j=0; j<mddInteger.getArray().length; j++)
          {
            if(mddInteger.getArray()[j] != ((RasGMArray)result).getArray()[j])
                equal = false;
          }
        }
        //System.out.println("All results for MArrayInteger");
        System.out.println("inserted and selected MArrayInteger are equal: " + equal);
      }

      // testing copy constructor
      System.out.println("### Testing copy constructor:");
      RasMArrayInteger mddInteger2 = new RasMArrayInteger(mddInteger);
      mddInteger2.setArray(dataInteger);
      dataInteger = null;
      equal = true;
      for(int j=0; j<mddInteger2.getArray().length; j++)
      {
        //System.out.print(" "+ mddInteger2.getArray()[j]);
        if(mddInteger.getArray()[j] != mddInteger2.getArray()[j])
          equal = false;
      }
      System.out.println("Copy constructor is OK: " + equal);

      /*
      System.out.println("\nspecial array2: ");
      for(int j=0; j<mddInteger2.getIntArray().length; j++)
      {
        System.out.print(" "+ mddInteger2.getIntArray()[j]);
      }
      */

      /**
       * test the MArrayDouble
       */
      System.out.println("\n\n### Testing MArrayDouble: ################################################");
      myQu = myApp.newOQLQuery();

      myQu.create("drop collection test");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection dropped");

      myQu.create("create collection test DoubleSet");
      RasMArrayDouble mddDouble = new RasMArrayDouble(domain, stl);
      double[] dataDouble = new double[len];
      //byte[] dataDouble = new byte[288];
      mddDouble.setObjectTypeName("DoubleImage");

      for(double j = 0; j < dataDouble.length; j++)
        dataDouble[(int)j] = j;

      mddDouble.setArray(dataDouble);

      //System.out.println("\nbyte array: ");
      //for(int j=0; j<mddDouble.getArray().length; j++)
      //{
        //System.out.print(" "+ mddDouble.getArray()[j]);
      //}

      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("\ncollection created");
      myQu.create("insert into test values $1 ");

      //System.out.println("mdd before sending: " + mddDouble);
      myQu.bind(mddDouble);
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection inserted");

      myQu = myApp.newOQLQuery();
      myQu.create("select img from test as img");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection selected");
      if (resultBag != null)
      {
	Iterator iter = resultBag.iterator();
	while (iter.hasNext())
	{
          result = iter.next();
          equal = true;
          for(int j=0; j<mddDouble.getDoubleArray().length; j++)
          {
            if(mddDouble.getArray()[j] != ((RasGMArray)result).getArray()[j])
                equal = false;
          }
          System.out.println("result mdd: " + ((RasGMArray)mddDouble).toTestString());
          System.out.println("result mdd: " + ((RasGMArray)result).toTestString());
        }
          //System.out.println("All results for MArrayDouble");
          System.out.println("inserted and selected MArrayDouble are equal: " + equal);
      }


      // testing copy constructor
      System.out.println("### Testing copy constructor:");
      RasMArrayDouble mddDouble2 = new RasMArrayDouble(mddDouble);
      mddDouble2.setArray(dataDouble);
      //System.out.println("byte array2: ");
      dataDouble = null;
      equal = true;
      for(int j=0; j<mddDouble2.getArray().length; j++)
      {
        //System.out.print(" "+ mddDouble2.getArray()[j]);
        if(mddDouble.getArray()[j] != mddDouble2.getArray()[j])
              equal = false;
      }
      System.out.println("Copy constructor is OK: " + equal);

      /*
      System.out.println("\nspecial array2: ");
      for(int j=0; j<mddDouble2.getDoubleArray().length; j++)
      {
        System.out.print(" "+ mddDouble2.getDoubleArray()[j]);
      }
      */

      /**
       * test the MArrayFloat
       */
      System.out.println("\n\n### Testing MArrayFloat: ################################################");
      myQu = myApp.newOQLQuery();

      myQu.create("drop collection test");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection dropped");

      myQu.create("create collection test FloatSet");
      RasMArrayFloat mddFloat = new RasMArrayFloat(domain, stl);
      float[] dataFloat = new float[len];
      //byte[] dataFloat = new byte[144];
      mddFloat.setObjectTypeName("FloatImage");

      for(float j = 0; j < dataFloat.length; j++)
        dataFloat[(int)j] = j;

      mddFloat.setArray(dataFloat);

      //System.out.println("\nbyte array: ");
      //for(int j=0; j<mddFloat.getArray().length; j++)
      //{
        //System.out.print(" "+ mddFloat.getArray()[j]);
      //}

      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("\ncollection created");
      myQu.create("insert into test values $1 ");

      //System.out.println("mdd before sending: " + mddFloat);
      myQu.bind(mddFloat);
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection inserted");

      myQu = myApp.newOQLQuery();
      myQu.create("select img from test as img");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection selected");
      if (resultBag != null)
      {
	Iterator iter = resultBag.iterator();
	while (iter.hasNext())
	{
          result = iter.next();
          equal = true;
          for(int j=0; j<mddFloat.getArray().length; j++)
          {
            if(mddFloat.getArray()[j] != ((RasGMArray)result).getArray()[j])
              equal = false;
          }
          System.out.println("result mdd: " + ((RasGMArray)mddFloat).toTestString());
          System.out.println("result mdd: " + ((RasGMArray)result).toTestString());
        }
          //System.out.println("All results for MArrayFloat");
          System.out.println("inserted and selected MArrayFloat are equal: " + equal);
      }

      // testing copy constructor
      System.out.println("### Testing copy constructor:");
      RasMArrayFloat mddFloat2 = new RasMArrayFloat(mddFloat);
      mddFloat2.setArray(dataFloat);
      //System.out.println("\nbyte array2: ");
      dataFloat = null;
      equal = true;
      for(int j=0; j<mddFloat2.getArray().length; j++)
      {
        //System.out.print(" "+ mddFloat2.getArray()[j]);
        if(mddFloat.getArray()[j] != mddFloat2.getArray()[j])
              equal = false;
      }
      System.out.println("Copy constructor is OK: " + equal);
      /*
      System.out.println("\nspecial array2: ");
      for(int j=0; j<mddFloat2.getFloatArray().length; j++)
      {
        System.out.print(" "+ mddFloat2.getFloatArray()[j]);
      }
      */

      /**
       * test the MArrayShort
       */
      System.out.println("\n\n### Testing MArrayShort: ################################################");
      myQu = myApp.newOQLQuery();

      myQu.create("drop collection test");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection dropped");

      myQu.create("create collection test ShortSet");
      RasMArrayShort mddShort = new RasMArrayShort(domain, stl);
      //RasMArrayShort mddShort4 = new RasMArrayShort(domain, stl);
      //RasMArrayShort mddShort3 = new RasMArrayShort(domain, stl);
      short[] dataShort = new short[len];
      //short[] dataShort4 = new short[len];
      //short[] dataShort3 = new short[len];
      //byte[] dataShort = new byte[72];
      mddShort.setObjectTypeName("ShortImage");
      //mddShort4.setObjectTypeName("ShortImage");
      //mddShort3.setObjectTypeName("ShortImage");

      //System.out.println("new OID from server: " + myApp.getObjectId(mddShort));
      //System.out.println("new OID from server: " + myApp.getObjectId(mddShort4));
      for(int j = 0; j < dataShort.length; j++)
        dataShort[j] = (short)j;

      //for(int j = 0; j < dataShort.length; j++)
        //dataShort[j] = 1;
      //for(int j = 0; j < dataShort4.length; j++)
        //dataShort4[j] = 2;
      //for(int j = 0; j < dataShort3.length; j++)
        //dataShort3[j] = 3;


      mddShort.setArray(dataShort);
      //mddShort4.setArray(dataShort4);
      //mddShort3.setArray(dataShort3);
      /*
      System.out.println("\nbyte array: ");
      for(int j=0; j<mddShort.getArray().length; j++)
      {
        System.out.print(" "+ mddShort.getArray()[j]);
      }
      */
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      myQu = myApp.newOQLQuery();
      //System.out.println("\ncollection created");
      //myQu.create("insert into test values $1");
      myQu.create("insert into test values $1");
      //System.out.println("mdd before sending: " + mddShort);
      //myQu.bind("test");
      myQu.bind(mddShort);
      //myQu.bind(mddShort4);
      //myQu.bind(mddShort3);
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      /*
      myTa.begin();
      myQu = myApp.newOQLQuery();
      myQu.create("insert into test values $1");
      myQu.bind(mddShort4);
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      myTa.begin();
      myQu = myApp.newOQLQuery();
      myQu.create("insert into test values $1");
      myQu.bind(mddShort3);
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      */
      //System.out.println("collection inserted");
      myQu = myApp.newOQLQuery();
      myQu.create("select img from test as img");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection selected");
      if (resultBag != null)
      {
	Iterator iter = resultBag.iterator();
	while (iter.hasNext())
	{
          result = iter.next();
          equal = true;
          for(int j=0; j<mddShort.getArray().length; j++)
          {
            if(mddShort.getArray()[j] != ((RasGMArray)result).getArray()[j])
              equal = false;
          }
          System.out.println("result mdd: " + ((RasGMArray)mddShort).toTestString());
          System.out.println("result mdd: " + ((RasGMArray)result).toTestString());
        }
          //System.out.println("All results for MArrayShort");
          System.out.println("inserted and selected MArrayShort are equal: " + equal);
      }

      // testing copy constructor
      System.out.println("### Testing copy constructor:");
      RasMArrayShort mddShort2 = new RasMArrayShort(mddShort);
      mddShort2.setArray(dataShort);
      //System.out.println("\nbyte array2: ");
      dataShort = null;
      equal = true;
      for(int j=0; j<mddShort2.getArray().length; j++)
      {
        //System.out.print(" "+ mddShort2.getArray()[j]);
        if(mddShort.getArray()[j] != mddShort2.getArray()[j])
              equal = false;
      }
      System.out.println("Copy constructor is OK: " + equal);
      /*
      System.out.println("\nspecial array2: ");
      for(int j=0; j<mddShort2.getShortArray().length; j++)
      {
        System.out.print(" "+ mddShort2.getShortArray()[j]);
      }
      */

      /**
       * test the MArrayLong
       */
      System.out.println("\n\n### Testing MArrayLong: ################################################");
      myQu = myApp.newOQLQuery();

      myQu.create("drop collection test");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection dropped");

      myQu.create("create collection test ULongSet");
      RasMArrayLong mddLong = new RasMArrayLong(domain, stl);
      long[] dataLong = new long[len];
      //byte[] dataLong = new byte[288];
      mddLong.setObjectTypeName("ULongImage");

      for(long j = 0; j < dataLong.length; j++)
        dataLong[(int)j] = j;
      mddLong.setArray(dataLong);

      /*
      System.out.println("\nbyte array: ");
      for(int j=0; j<mddLong.getArray().length; j++)
      {
        System.out.print(" "+ mddLong.getArray()[j]);
      }
      */

      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("\ncollection created");
      myQu.create("insert into test values $1 ");

      //System.out.println("mdd before sending: " + mddLong);
      myQu.bind(mddLong);
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection inserted");

      myQu = myApp.newOQLQuery();
      myQu.create("select img from test as img");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      //System.out.println("collection selected");
      if (resultBag != null)
      {
	Iterator iter = resultBag.iterator();
	while (iter.hasNext())
	{
          result = iter.next();
          equal = true;
          for(int j=0; j<((RasGMArray)result).getArray().length; j++)
          {
            if(mddLong.getArray()[j] != ((RasGMArray)result).getArray()[j])
              equal = false;
          }
          System.out.println("result mdd: " + ((RasGMArray)mddLong).toTestString());
          System.out.println("result mdd: " + ((RasGMArray)result).toTestString());
        }
        //System.out.println("All results for MArrayLong");
        System.out.println("inserted MArrayInteger and selected MArrayLong are equal: " + equal);
      }

      // testing copy constructor
      System.out.println("### Testing copy constructor:");
      RasMArrayLong mddLong2 = new RasMArrayLong(mddLong);
      mddLong2.setArray(dataLong);
      //System.out.println("\nbyte array2: ");
      dataLong = null;
      equal = true;
      for(int j=0; j<mddLong2.getArray().length; j++)
      {
        //System.out.print(" "+ mddLong2.getArray()[j]);
        if(mddLong.getArray()[j] != mddLong2.getArray()[j])
              equal = false;
      }
      System.out.println("Copy constructor is OK: " + equal);

      System.out.println("\nspecial array: ");
      for(int j=0; j<((RasMArrayLong)result).getLongArray().length; j++)
      {
        System.out.print(" "+ ((RasMArrayLong)result).getLongArray()[j]);
      }


      /**
       * testing intersection
       */
      System.out.println("\n\n### Testing intersection:");
      mddConst2.intersectionWith(domain2);
      mddByte2.intersectionWith(domain2);
      mddDouble2.intersectionWith(domain2);
      mddFloat2.intersectionWith(domain2);
      mddInteger2.intersectionWith(domain2);
      mddLong2.intersectionWith(domain2);
      mddShort2.intersectionWith(domain2);
      System.out.println("OK\n");

      /**
       * testing OIDs
       */
      myQu = myApp.newOQLQuery();
      myQu.create("SELECT oid(img) FROM test AS img");
      //myQu.create("SELECT img FROM test AS img where (oid(img)=231425)");
      myTa.begin();
      resultBag = (DBag)myQu.execute();
      myTa.commit();
      if (resultBag != null)
      {
	Iterator iter = resultBag.iterator();
	while (iter.hasNext())
	{
          result = iter.next();
          //System.out.println("OID: " + result);
        }
        //System.out.println("All results");

      }

      /**
       * get new OID
       */
      System.out.println("### Testing OIDs:");
      myTa.begin();
      myApp.getObjectId(new RasGMArray());
      myTa.commit();

      System.out.println( "Closing database ..." );
      myDb.close();

      // get new OID without open TA
      RasGMArray gmar = new RasGMArray();
      equal = true;
      if(!myApp.getObjectId(gmar).equals(gmar.getOID().toString()))
        equal = false;
      System.out.println("same OID on client and server side: " + equal);

    }
    catch (RasException e)
    {
      System.out.println("!!!!!!!!!!!!!!!!!!!!While testing!!!!!!!!!!!!!!!!!!");
      System.out.println("An RasException has occurred: " + e.getMessage());
      System.out.println("Try to abort the transaction ...");
      if(myTa != null) myTa.abort();

      try
      {
        System.out.println("Try to close the database ...");
	if(myDb != null) myDb.close();
      }
      catch ( org.odmg.ODMGException exp )
      {
        System.err.println("Could not close the database: " + exp.getMessage());
      }
    }
    catch (RasRuntimeException e)
    {
      System.out.println("!!!!!!!!!!!!!!!!!!!!While testing!!!!!!!!!!!!!!!!!!");
      System.out.println("An RasRuntimeException has occurred: " + e.getMessage());
      System.out.println("Try to abort the transaction ...");
      if(myTa != null) myTa.abort();

      try
      {
        System.out.println("Try to close the database ...");
	if(myDb != null) myDb.close();
      }
      catch ( org.odmg.ODMGException exp )
      {
        System.err.println("Could not close the database: " + exp.getMessage());
      }
    }
    catch (org.odmg.ODMGException e)
    {
      System.out.println("!!!!!!!!!!!!!!!!!!!!While testing!!!!!!!!!!!!!!!!!!");
      System.out.println("An ODMGException has occurred: " + e.getMessage());
      System.out.println("Try to abort the transaction ...");
      if(myTa != null) myTa.abort();

      try
      {
        System.out.println("Try to close the database ...");
	if(myDb != null) myDb.close();
      }
      catch ( org.odmg.ODMGException exp )
      {
        System.err.println("Could not close the database: " + exp.getMessage());
      }
    }
    System.out.println( "Done." );

  }
}

