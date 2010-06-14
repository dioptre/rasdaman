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
 * PURPOSE:
 *
 *
 * COMMENTS:
 * - on error, an exception is thrown (java main() knows no exit status)
 *
 * </pre>
 *********************************************************** */

import java.util.StringTokenizer;
import java.lang.Integer;
import java.lang.Long;
import rasj.*;
import rasj.odmg.*;

public class TestRasInterval
  {
    /**
    * constants used in this test
    **/
    // prefixes for test output
    static final String PREFIX_PROGRAM  = "+++ +++ +++ ";
    static final String PREFIX_TESTSET  = "+++ +++ ";
    static final String PREFIX_TESTCASE = "+++ ";

    /**
    * std error tag printed upon failure
    **/
    static final String ERROR_TAG = "ERROR: ";

    /**
    * main program for testing
    * on error, an exception is thrown (java main() knows no exit status)
    **/
    public static void main(String argv[]) throws Exception
      {
        System.out.println( "rasdaman system test v5.1revC: TestRasInterval." );
        System.out.println( PREFIX_PROGRAM + "system test started." );

        // -- START test cases -------------------------------------------------
        testInterval();
        // -- END test cases ---------------------------------------------------

        System.out.println( PREFIX_PROGRAM + "system test done." );
        return;
      }

    /**
    * test the RasInterval class (no db access)
    * any eventual exception that is not caught here is an error, and will cause an abort
    **/
    static void testInterval() throws RasResultIsNoIntervalException, RasDimensionMismatchException, RasIndexOutOfBoundsException, RasStreamInputOverflowException
      {
          // create a new RasSInterval
          System.out.println("\n############################ test sintervals: ");
          RasSInterval s1 = new RasSInterval(100,200);
          RasSInterval s2 = new RasSInterval("150:400");

          // get upper bound
          boolean b1 = false;
          if(s2.high() == 400)
            b1 = true;
          System.out.println("upper bound is correct: " + b1);

          // test if lower bound is fixed
          boolean b2;
          b1 = false;
          b1 = s2.isLowFixed();
          System.out.println("lower bound is fix: " + b1);

          // test if interval intersects with another interval
          // (the return value shows the kind of intersection)
          int j;
          j = s1.intersectsWith(s2);
          b1 = false;
          if(j != -1)
            b1 = true;
          System.out.println("s1 intersects with s2: " + b1);

          RasSInterval sint1 = new RasSInterval(100,200);
          RasSInterval sint2 = new RasSInterval("150 :400 ");
          RasSInterval sint3 = new RasSInterval("50:180");

          b1 = false;
          sint1.setHigh('*');
          sint1.setLow(400);
          if(sint1.low() == 400)
            b1 = true;
          sint1.setLow('*');
          sint1.setHigh(500);
          if((sint1.high() == 500) && b1)
            b1 = true;
          else
            b1 = false;
          System.out.println("setLow and setHigh are OK: " + b1);

          sint1.setInterval('*',500);
          b1 = !sint1.isLowFixed();
          b1 = sint1.isHighFixed()  && b1;
          System.out.println("low is open high is fixed: " + b1);

          b1 = false;
          if(sint1.intersectsWith(sint1) != -1)
            b1 = true;

          if(sint3.closureOf(sint2, sint1).equals(sint1) &&
          sint3.closureWith(sint2).equals(sint1) &&
          sint3.createClosure(sint2).equals(sint1))
            b1 = b1 && true;

          sint1.setInterval(90, 105);
          sint2.setInterval(100, 110);
          if(sint2.createDifference(sint1).equals(new RasSInterval(90, 100)) &&
          sint3.createIntersection(sint2).equals(new RasSInterval(100, 110)))
            b1 = b1 && true;

          if(sint3.createUnion(sint2).equals(sint1) &&
          sint1.differenceOf(sint1, sint2).equals(new RasSInterval(90, 100)) &&
          sint1.differenceWith(sint2).equals(new RasSInterval(100, 110)))
            b1 = b1 && true;

          if(sint3.intersectionOf(sint2, sint1).equals(new RasSInterval(100, 100)) &&
          sint3.intersectionWith(sint2).equals(new RasSInterval(100, 100)))
            b1 = b1 && true;

          b1 = b1 && !sint3.equals(sint2);
          if(sint3.unionOf(sint1, sint2).equals(new RasSInterval(90, 110)) &&
          sint3.unionWith(sint2).equals(new RasSInterval(90, 110)))
            b1 = b1 && true;
          System.out.println("operations of sintervals are correct: " + b1);



          // create new RasMInterval
          System.out.println("\n############################ test mintervals: ");
          RasMInterval mint1 = new RasMInterval("[567:3253,666:888]");
          RasMInterval mint2 = new RasMInterval("[678:4000,777:999]");
          RasMInterval mint3 = new RasMInterval("[777:900,888:1000]");
          RasMInterval mint4 = new RasMInterval(2);
          b1 = false;
          if((mint2.dimension() == 2) && (mint1.dimension() == 2))
            b1 = true;
          System.out.println("dimensions of mintervals are correct: " + b1);

          b1 = false;
          mint1 = new RasMInterval("[500:3000,600:800]");
          mint2 = new RasMInterval("[600:1000,700:750]");
          b1 = mint1.intersectsWith(mint2);
          mint1 = new RasMInterval("[567:3253,666:888]");
          mint2 = new RasMInterval("[678:4000,777:999]");
          if(mint3.closureOf(mint2, mint1).toString().equals(new RasMInterval("[567:4000,666:999]").toString()))
            b1 = b1 && true;
          else b1 = false;
          if(mint3.closureWith(mint2).toString().equals(new RasMInterval("[567:4000,666:999]").toString()))
            b1 = b1 && true;
          else b1 = false;
          if(mint3.createClosure(mint2).toString().equals(new RasMInterval("[567:4000,666:999]").toString()))
            b1 = b1 && true;
          else b1 = false;
          mint3 = new RasMInterval("[678:2000,777:888]");
          mint3 = mint3.createDifference(mint2);
          if(mint3.toString().equals(new RasMInterval("[2000:4000,888:999]").toString()))
            b1 = b1 && true;
          else b1 = false;
          mint3 = new RasMInterval("[678:2000,777:888]");
          mint3 = mint3.createDifference(mint2);
          if(mint3.toString().equals(new RasMInterval("[2000:4000,888:999]").toString()))
            b1 = b1 && true;
          else b1 = false;
          mint3 = new RasMInterval("[678:2000,777:888]");
          mint3 = mint3.createIntersection(mint2);
          if(mint3.toString().equals(new RasMInterval("[678:2000,777:888]").toString()))
            b1 = b1 && true;
          else b1 = false;
          mint3 = new RasMInterval("[678:2000,777:888]");
          mint2 = new RasMInterval("[1000:4000,750:999]");
          mint3 = mint3.createUnion(mint2);
          if(mint3.toString().equals(new RasMInterval("[678:4000, 750:999]").toString()))
            b1 = b1 && true;
          else b1 = false;
          mint2 = new RasMInterval("[1000:4000,800:999]");
          mint1 = mint1.differenceOf(mint3, mint2);
          if(mint1.toString().equals(new RasMInterval("[678:1000,750:800]").toString()))
            b1 = b1 && true;
          else b1 = false;
          mint3 = mint3.intersectionOf(mint2, mint1);
          if(mint3.toString().equals(new RasMInterval("[1000:1000,800:800]").toString()))
            b1 = b1 && true;
          else b1 = false;
          mint3 = mint3.intersectionWith(mint2);
          if(mint3.toString().equals(new RasMInterval("[1000:1000,800:800]").toString()))
            b1 = b1 && true;
          else b1 = false;
          b1 = b1 && !mint3.equals(mint2);
          mint3 = mint3.unionOf(mint1, mint2);
          if(mint3.toString().equals(new RasMInterval("[678:4000,750:999]").toString()))
            b1 = b1 && true;
          else b1 = false;
          mint3 = mint3.unionWith(mint2);
          if(mint3.toString().equals(new RasMInterval("[678:4000,750:999]").toString()))
            b1 = b1 && true;
          else b1 = false;
          mint2 = mint2.differenceWith(mint3);
          if(mint2.toString().equals(new RasMInterval("[678:1000,750:800]").toString()))
            b1 = b1 && true;
          else b1 = false;
          System.out.println("operations of mintervals are correct: " + b1);

          // Types
          ///////////////////////////////////////////
          System.out.println("\n############################ test types: ");
          String s = "marray <char, 1>";
          RasType rType = RasType.getAnyType(s);
          RasBaseType rb;

          if(rType.getClass().getName().equals("rasj.RasMArrayType"))
          {
            RasMArrayType tmp = (RasMArrayType)rType;
            rb = tmp.getBaseType();

            System.out.println("OK");
          }
          else
            System.out.println("element of MDD Collection is no MArray.");

          boolean b3;
          b3 = !rType.isStructType();
          System.out.println(b3);


          // StorageLayout
          ///////////////////////////////////////////
          System.out.println("\n############################ test storage layout: ");
          RasGMArray myMDD = new RasGMArray(new RasMInterval("[1:400,1:400]"),1);
          byte[] mydata = new byte[160000];
          for(int y=0; y<400; y++)
          {
                  for(int x=0; x<400; x++)
                  {
                          if((x>99 && x<151) || (x>299 && x<351))
                                  mydata[y*399+x]=100;
                          else
                                  mydata[y*399+x]=0;
                  }
          }

          myMDD.setArray(mydata);
          myMDD.setObjectTypeName("GreyImage");

          RasStorageLayout myLayout = new RasStorageLayout();

          myLayout.setTileDomain("[1:100,1:70]");
          // you can either set the TileSize or the TileDomain
          myLayout.setTileSize(32);
          myMDD.setStorageLayout(myLayout);
          if(myMDD.getStorageLayout().getTileSize() == 32)
            System.out.println("OK");


          //GMarray////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          RasGMArray array1 = new RasGMArray();
          RasGMArray array2 = new RasGMArray(mint1, 8);

          myMDD.getCell(new RasPoint(1,1));

          System.out.println("OK");


          //Ausgabe////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          /*
          System.out.println("Array1 " + array1);
          System.out.println("Array2 " + array2);

          System.out.println("Punkt1 " + p1);
          System.out.println("Punkt2 " + p2);

          System.out.println("Sinterval1 " + s1);
          System.out.println("Sinterval2 " + s2);
          System.out.println("Sinterval3 " + sint3);

          System.out.println("j = " + j);
          //System.out.println("b1 = " + b1);
          //System.out.println("b2 = " + b2);
          System.out.println("b3 = " + b3);
          */

    }

  } // TestRasInterval
