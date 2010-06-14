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
package rasj.test;

import java.util.StringTokenizer;
import java.lang.Integer;
import java.lang.Long;
import rasj.*;
import rasj.odmg.*;

public class testEva
{

	public static void main(String argv[]) throws RasException
	{

          // Points and Intervals
          //////////////////////////////////////////////
          // create a new RasPoint
          RasPoint p1 = new RasPoint("[666, 777]");
          RasPoint p2 = new RasPoint(5,4);

          // test if two points are equal
          boolean b1;
          b1 = p1.equals(p2);

          // get the dimension of a Point
          int i;
          i = p1.dimension();



          // create a new RasSInterval
          RasSInterval s1 = new RasSInterval(100,200);
          RasSInterval s2 = new RasSInterval("150:400");

          // get upper bound
          long l1;
          l1 = s2.high();

          // test if lower bound is fixed
          boolean b2;
          b2 = s2.isLowFixed();

          // test if interval intersects with another interval
          // (the return value shows the kind of intersection)
          int j;
          j = s1.intersectsWith(s2);


          // create new RasMInterval
          RasMInterval m1 = new RasMInterval("[567:3253,666:777]");
          RasMInterval m2 = new RasMInterval(4);

          // get number of cells
          long l2 = 1;
          RasPoint m1Ext  = m1.getExtent();
          for (int dimi =0; dimi < m1Ext.dimension() ; dimi++)
          {
           l2 = l2 * m1Ext.item(dimi);
           }

          // Types
          ///////////////////////////////////////////
          String s = "marray <char, 1>";
          RasType rType = RasType.getAnyType(s);
          RasBaseType rb;

          if(rType.getClass().getName().equals("rasj.RasMArrayType"))
          {
            RasMArrayType tmp = (RasMArrayType)rType;
            rb = tmp.getBaseType();

            System.out.println(rb);
          }
          else
            System.out.println("element of MDD Collection is no MArray.");

          boolean b3;
          b3 = rType.isStructType();



          // StorageLayout
          ///////////////////////////////////////////
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

          myLayout.setTileSize(32);
          // you can either set the TileSize or the TileDomain
          myLayout.setTileDomain("[1:100,1:70]");
          //myMDD.setStorageLayout();


		i = p2.dimension();
		b1 = p1.equals(p2);
                //System.out.println(p1);
		//p1.setTo(p1.mult(p2));

		//sinterval//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		RasSInterval sint1 = new RasSInterval(100,200);
		RasSInterval sint2 = new RasSInterval("150 :400 ");
		RasSInterval sint3 = new RasSInterval("50:250");



                sint1.setHigh(500);
                sint1.setLow(400);
		sint1.setLow('*');
		sint1.setHigh('*');

		sint1.setInterval('*',500);

		l1 = sint1.low();
		l2 = sint1.high();
		b1 = sint1.isLowFixed();

		i = sint1.intersectsWith(sint1);

                //System.out.println(sint1);
                //System.out.println(l1);
                //System.out.println(l2);
                //System.out.println(b1);
                //System.out.println(i);

		sint3 = sint3.unionOf(sint1, sint2);
		sint3 = sint3.unionWith(sint2);
		sint3 = sint3.addToSelf(sint2);
		sint3 = sint3.createUnion(sint2);
		sint3 = sint3.add(sint2);
		//sint3 = sint3.differenceOf(sint2, sint1);
		//sint3 = sint3.differenceWith(sint1);
		sint3 = sint3.intersectionOf(sint2, sint1);
		sint3 = sint3.intersectionWith(sint2);
		sint3 = sint3.multWithSelf(sint2);
		sint3 = sint3.createIntersection(sint2);
		sint3 = sint3.mult(sint2);
		sint3 = sint3.closureOf(sint2, sint1);
		sint3 = sint3.closureWith(sint2);
		sint3 = sint3.createClosure(sint2);

		//Minterval//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		RasMInterval mint1 = new RasMInterval("[567 :3253,666 :777]");
		RasMInterval mint2 = new RasMInterval(4);

		//GMarray////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		RasGMArray array1 = new RasGMArray();
		RasGMArray array2 = new RasGMArray(mint1, 8);


		byte[] bya = new byte[8];



		l1 = 1;
        RasPoint mint1Ext  = mint1.getExtent();
        for (int dimi =0; dimi < mint1Ext.dimension() ; dimi++)
        {
         l1 = l1 * mint1Ext.item(dimi);
         }

		array1.setTo(array2);
		//bya = array2.getCell(p1);
		array2.intersectionWith(mint1);


		//Ausgabe////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		System.out.println("Array1 " + array1);
                System.out.println("Array2 " + array2);



		System.out.println("Punkt1 " + p1);
		System.out.println("Punkt2 " + p2);

		System.out.println("Sinterval1 " + s1);
		System.out.println("Sinterval2 " + s2);
		//System.out.println("Sinterval3 " + sint3);

		System.out.println("Minterval1 " + m1);
		System.out.println("Minterval2 " + m2);


		System.out.println("i = " + i);
                System.out.println("j = " + j);
		System.out.println("b1 = " + b1);
		System.out.println("b2 = " + b2);
                System.out.println("b3 = " + b3);
		System.out.println("l1 = " + l1);
		System.out.println("l2 = " + l2);
		//System.out.println(bya);

	}
}
