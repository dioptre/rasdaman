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
 * test rasj class RasPoint.
 *
 *
 * COMMENTS:
 * - testing philosophy is to run all tests and report. no exit code / exception
 *   is used for error indication; look into the output for ERROR_TAG instead.
 * - unexpected exceptions are not caught, hence will make gmake fail and show the error
 * - every test that fails must print a line containing ERROR_TAG
 * </pre>
 *********************************************************** */

import java.util.StringTokenizer;
import java.lang.Integer;
import java.lang.Long;
import rasj.*;
import rasj.odmg.*;

public class TestRasPoint
  {
    /**
    * constants used in this test
    **/
    // prefixes for test output
    static final String PREFIX_PROGRAM  = "+++ +++ +++ ";
    static final String PREFIX_TESTSET  = "+++ +++ ";
    static final String PREFIX_TESTCASE = "+++ ";

    static final int COORD_1 = 11;
    static final int COORD_2 = 22;
    static final int COORD_3 = 33;
    static final int COORD_4 = 44;
    static final int COORD_5 = 55;

    /**
    * std error tag printed if a test fails
    **/
    static final String ERROR_TAG = "ERROR: ";

    /**
    * main program for testing
    * on error, an exception is thrown (java main() knows no exit status)
    **/
    public static void main(String argv[]) throws Exception
      {
        System.out.println( "rasdaman system test v5.1revC: testing class RasPoint." );
        System.out.println( PREFIX_PROGRAM + "system test started." );

        // -- START test cases -------------------------------------------------
        testConstructor();
        // testcomparedWith();
        // testEquals();
        // testStream();
        // testSetItem();
        // testSetTo();
        testAdd();
        // testmult();
        // -- END test cases ---------------------------------------------------

        System.out.println( PREFIX_PROGRAM + "system test done." );
        return;
      }

    /**
    * test the RasPoint class constructor
    * any eventual exception that is not caught here is an error, and will cause an abort
    **/
    static void testConstructor() throws RasDimensionMismatchException, RasIndexOutOfBoundsException
      {
        System.out.println( PREFIX_TESTSET + "testing constructor started." );

        // default constructor
        System.out.print( PREFIX_TESTCASE + "default constructor..." );
        RasPoint p_default = new RasPoint();
        if (p_default == null)
            System.out.println( ERROR_TAG + "result is null." );
        else if (! p_default.equals( p_default ) )
            System.out.println( ERROR_TAG + "point notEqual to itself." );
        else
            System.out.println( "OK, result=" + p_default + ", dimension is " + p_default.dimension() );

        // stream constructor
        // ------------------

        System.out.print( PREFIX_TESTCASE + "stream constructor, illegal dimension -1..." );
        try
          {
            RasPoint p_stream_m1 = new RasPoint( -1 );
            if (p_stream_m1 != null)
                System.out.println( ERROR_TAG + "result is not null." );
            else
                System.out.println( "OK." );
          }
        catch(NegativeArraySizeException e)
          {
            System.out.println( "OK: " + e.getMessage() );
          }

        System.out.print( PREFIX_TESTCASE + "stream constructor, dimension 0..." );
        RasPoint p_stream_0 = new RasPoint( 0 );
        if (p_stream_0.dimension() != 0)
            System.out.println( ERROR_TAG + "wrong dimension." );
        else
            System.out.println( "OK." );

        // --- easy-to-use constructor -------------------------------------------------------

        // 2-D constructor
        System.out.print( PREFIX_TESTCASE + "2D easy to use constructor..." );
        RasPoint p_2d_easy = new RasPoint( COORD_1, COORD_2 );
        if (p_2d_easy == null)
            System.out.println( ERROR_TAG + "result is null." );
        else if (p_2d_easy.dimension() != 2)
            System.out.println( ERROR_TAG + "dimension is not 2 but " + p_2d_easy.dimension() );
        else if (p_2d_easy.item(0) != COORD_1)
            System.out.println( ERROR_TAG + "item(0) returns wrong component " + p_2d_easy.item(0) );
        else if (p_2d_easy.item(1) != COORD_2)
            System.out.println( ERROR_TAG + "item(1) returns wrong component " + p_2d_easy.item(0) );
        else if (! p_2d_easy.equals( p_2d_easy ) )
            System.out.println( ERROR_TAG + "point not equal to itself." );
        else if ( p_2d_easy.notEquals( p_2d_easy ) )
            System.out.println( ERROR_TAG + "point notEqual to itself." );
        else
            System.out.println( "OK, result=" + p_2d_easy );

        // 3-D constructor
        System.out.print( PREFIX_TESTCASE + "3D easy to use constructor..." );
        RasPoint p_3d_easy= new RasPoint( COORD_1, COORD_2, COORD_3 );
        if (p_3d_easy== null)
            System.out.println( ERROR_TAG + "result is null." );
        else if (p_3d_easy.dimension() != 3)
            System.out.println( ERROR_TAG + "dimension is not 3 but " + p_3d_easy.dimension() );
        else if (p_3d_easy.item(2) != COORD_3)
            System.out.println( ERROR_TAG + "item(2) returns wrong component " + p_3d_easy.item(0) );
        else if (! p_3d_easy.equals( p_3d_easy) )
            System.out.println( ERROR_TAG + "point not equal to itself." );
        else if ( p_3d_easy.notEquals( p_3d_easy) )
            System.out.println( ERROR_TAG + "point notEqual to itself." );
        else
            System.out.println( "OK, result=" + p_3d_easy);

        // 4-D constructor
        System.out.print( PREFIX_TESTCASE + "4D easy to use constructor..." );
        RasPoint p_4d_easy = new RasPoint( COORD_1, COORD_2, COORD_3, COORD_4 );
        if (p_4d_easy == null)
            System.out.println( ERROR_TAG + "result is null." );
        else if (p_4d_easy.dimension() != 4)
            System.out.println( ERROR_TAG + "dimension is not 4 but " + p_4d_easy.dimension() );
        else if (p_4d_easy.item(3) != COORD_4)
            System.out.println( ERROR_TAG + "item(3) returns wrong component " + p_4d_easy.item(0) );
        else if (! p_4d_easy.equals( p_4d_easy ) )
            System.out.println( ERROR_TAG + "point not equal to itself." );
        else if ( p_4d_easy.notEquals( p_4d_easy ) )
            System.out.println( ERROR_TAG + "point notEqual to itself." );
        else
            System.out.println( "OK, result=" + p_4d_easy );

        // 5-D constructor
        System.out.print( PREFIX_TESTCASE + "5D easy to use constructor..." );
        RasPoint p_5d_easy = new RasPoint( COORD_1, COORD_2, COORD_3, COORD_4, COORD_5 );
        if (p_5d_easy == null)
            System.out.println( ERROR_TAG + "result is null." );
        else if (p_5d_easy.dimension() != 5)
            System.out.println( ERROR_TAG + "dimension is not 5 but " + p_5d_easy.dimension() );
        else if (p_5d_easy.item(4) != COORD_5)
            System.out.println( ERROR_TAG + "item(4) returns wrong component " + p_5d_easy.item(0) );
        else if (! p_5d_easy.equals( p_5d_easy ) )
            System.out.println( ERROR_TAG + "point not equal to itself." );
        else if ( p_5d_easy.notEquals( p_5d_easy ) )
            System.out.println( ERROR_TAG + "point notEqual to itself." );
        else
            System.out.println( "OK, result=" + p_5d_easy );

        // --- string constructor -------------------------------------------------------

        // illegal string format
        // FIXME: bug PB4
        System.out.println( ERROR_TAG + "PB4: string constructor, illegal string formats." );

        // System.out.print( PREFIX_TESTCASE + "string constructor, illegal string format (no [)..." );
        // RasPoint p_si = new RasPoint( COORD_1 + "," + COORD_2 + "]" );
        // System.out.println( "point is: " + p_si );
        // if (! p_si.equals( new RasPoint( "[" +  COORD_1 + "," + COORD_2 + "]" ) ) )
            // System.out.println( ERROR_TAG + "wrong point value." );

        // System.out.print( PREFIX_TESTCASE + "string constructor, illegal string format (no ])..." );
        // p_si = new RasPoint( "[" + COORD_1 + "," + COORD_2 );
        // System.out.println( "point is: " + p_si );
        // if (! p_si.equals( new RasPoint( "[" +  COORD_1 + "," + COORD_2 + "]" ) ) )
            // System.out.println( ERROR_TAG + "wrong point value." );

        // System.out.print( PREFIX_TESTCASE + "string constructor, illegal string format (no ,)..." );
        // p_si = new RasPoint( "[" + COORD_1 + " " + COORD_2 + "]" );
        // System.out.println( "point is: " + p_si );
        // if (! p_si.equals( new RasPoint( "[" + COORD_1 + "," + COORD_2 + "]" ) ) )
            // System.out.println( ERROR_TAG + "wrong point value." );

        // System.out.print( PREFIX_TESTCASE + "string constructor, illegal string format (bad int)..." );
        // p_si = new RasPoint( "[" + "abc" + "," + "xyz" + "]" );
        // System.out.println( "point is: " + p_si );
        // if (! p_si.equals( p_si ) )
            // System.out.println( ERROR_TAG + "point not equal to itself." );

        // System.out.print( PREFIX_TESTCASE + "string constructor, illegal string format (missing int)..." );
        // p_si = new RasPoint( "[" + "," + "]" );
        // System.out.println( "point is: " + p_si );
        // if (! p_si.equals( p_si ) )
            // System.out.println( ERROR_TAG + "point not equal to itself." );

        // 2-D constructor
        System.out.print( PREFIX_TESTCASE + "2D string constructor..." );
        RasPoint p_2d = new RasPoint( "[" + COORD_1 + "," + COORD_2 + "]" );
        if (p_2d == null)
            System.out.println( ERROR_TAG + "result is null." );
        else if (p_2d.dimension() != 2)
            System.out.println( ERROR_TAG + "dimension is not 2 but " + p_2d.dimension() );
        else if (p_2d.item(0) != COORD_1)
            System.out.println( ERROR_TAG + "item(0) returns wrong component " + p_2d.item(0) );
        else if (p_2d.item(1) != COORD_2)
            System.out.println( ERROR_TAG + "item(1) returns wrong component " + p_2d.item(0) );
        else if (! p_2d.equals( p_2d ) )
            System.out.println( ERROR_TAG + "point not equal to itself." );
        else if ( p_2d.notEquals( p_2d ) )
            System.out.println( ERROR_TAG + "point notEqual to itself." );
        else
            System.out.println( "OK, result=" + p_2d );

        // 3-D constructor
        System.out.print( PREFIX_TESTCASE + "3D string constructor..." );
        RasPoint p_3d = new RasPoint( "[" + COORD_1 + "," + COORD_2 + "," + COORD_3 + "]" );
        if (p_3d == null)
            System.out.println( ERROR_TAG + "result is null." );
        else if (p_3d.dimension() != 3)
            System.out.println( ERROR_TAG + "dimension is not 3 but " + p_3d.dimension() );
        else if (p_3d.item(2) != COORD_3)
            System.out.println( ERROR_TAG + "item(2) returns wrong component " + p_3d.item(0) );
        else if (! p_3d.equals( p_3d ) )
            System.out.println( ERROR_TAG + "point not equal to itself." );
        else if ( p_3d.notEquals( p_3d ) )
            System.out.println( ERROR_TAG + "point notEqual to itself." );
        else
            System.out.println( "OK, result=" + p_3d );

        // 4-D constructor
        System.out.print( PREFIX_TESTCASE + "4D string constructor..." );
        RasPoint p_4d = new RasPoint( "[" + COORD_1 + "," + COORD_2 + "," + COORD_3 + "," + COORD_4 + "]" );
        if (p_4d == null)
            System.out.println( ERROR_TAG + "result is null." );
        else if (p_4d.dimension() != 4)
            System.out.println( ERROR_TAG + "dimension is not 4 but " + p_4d.dimension() );
        else if (p_4d.item(3) != COORD_4)
            System.out.println( ERROR_TAG + "item(3) returns wrong component " + p_4d.item(0) );
        else if (! p_4d.equals( p_4d ) )
            System.out.println( ERROR_TAG + "point not equal to itself." );
        else if ( p_4d.notEquals( p_4d ) )
            System.out.println( ERROR_TAG + "point notEqual to itself." );
        else
            System.out.println( "OK, result=" + p_4d );

        // 5-D constructor
        System.out.print( PREFIX_TESTCASE + "5D string constructor..." );
        RasPoint p_5d = new RasPoint( "[" + COORD_1 + "," + COORD_2 + "," + COORD_3 + "," + COORD_4 + "," + COORD_5 + "]" );
        if (p_5d == null)
            System.out.println( ERROR_TAG + "result is null." );
        else if (p_5d.dimension() != 5)
            System.out.println( ERROR_TAG + "dimension is not 5 but " + p_5d.dimension() );
        else if (p_5d.item(4) != COORD_5)
            System.out.println( ERROR_TAG + "item(4) returns wrong component " + p_5d.item(0) );
        else if (! p_5d.equals( p_5d ) )
            System.out.println( ERROR_TAG + "point not equal to itself." );
        else if ( p_5d.notEquals( p_5d ) )
            System.out.println( ERROR_TAG + "point notEqual to itself." );
        else
            System.out.println( "OK, result=" + p_5d );

        // copy constructor
        System.out.print( PREFIX_TESTCASE + "copy constructor..." );
        RasPoint p_cp = new RasPoint( p_2d );
        if (p_cp == null)
            System.out.println( ERROR_TAG + "result is null." );
        else if (p_cp.dimension() != p_2d.dimension())
            System.out.println( ERROR_TAG + "dimension mismatch." );
        else if (p_cp.item(0) != p_2d.item(0))
            System.out.println( ERROR_TAG + "item(0) returns wrong component " + p_cp.item(0) );
        else if (p_cp.item(1) != p_2d.item(1))
            System.out.println( ERROR_TAG + "item(1) returns wrong component " + p_cp.item(1) );
        else if (! p_cp.equals( p_2d ) )
            System.out.println( ERROR_TAG + "point not equal to origin." );
        else if ( p_cp.notEquals( p_2d ) )
            System.out.println( ERROR_TAG + "point notEqual to origin." );
        else
            System.out.println( "OK, result=" + p_cp );

        System.out.println( PREFIX_TESTSET + "testing constructor done.\n" );
        return;
      } // testConstructor()

    /**
    * test Point addition -- TO BE DONE
    * any eventual exception that is not caught here is an error, and will cause an abort
    **/
    static void testAdd() throws RasDimensionMismatchException, RasIndexOutOfBoundsException, RasStreamInputOverflowException
      {
        System.out.println( PREFIX_TESTSET + "testing add started." );

          RasPoint p1 = new RasPoint(COORD_3,COORD_4);
          RasPoint p2 = new RasPoint(COORD_3,COORD_4);
          p2 = p2.add(p2);

          // test if two points are equal
          boolean b1 = false;
          if(p2.comparedWith(p1) == 0)
            b1 = p1.equals(p2);
          else
            b1 = false;
          System.out.println("points are equal: " + b1);

          // get the dimension of a Point
          int i;
          b1 = false;
          if((p1.dimension() == 2) && (2 == p2.dimension()))
            b1 = true;
          System.out.println("dimensions of points are correct: " + b1);

          b1 = false;
          p2.setItem(1, 48);
          if(p2.item(1) == 48)
            b1 = true;
          System.out.println("read and write access of points is OK: " + b1);

          b1 = false;
          p2.setTo(p2.mult(p1));
          if(p2.equals(new RasPoint(443556, 42624)))
            b1 = true;
          System.out.println("mult and setTo is OK: " + b1);

          b1 = false;
          RasPoint ps = new RasPoint( 3 );
          ps.stream( 42 );
          if(ps.item(0) == 42)
            b1 = true;
          System.out.println("stream initializing of points is OK: " + b1);

        System.out.println( PREFIX_TESTSET + "testing add done.\n" );
        return;
      } // testAdd()

  } // TestRasPoint
