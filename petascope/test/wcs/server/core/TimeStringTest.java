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

package wcs.server.core;

//~--- non-JDK imports --------------------------------------------------------

import org.joda.time.DateTime;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import petascope.wcs.server.core.TimeString;

import static org.junit.Assert.*;

/**
 * Test class for TimeString ISO 8601 utility class. 
 * @author Andrei Aiordachioaie
 */
public class TimeStringTest
{
	public TimeStringTest() {}

	@BeforeClass
	public static void setUpClass() throws Exception {}

	@AfterClass
	public static void tearDownClass() throws Exception {}

    @Test
	public void testDate0()
	{
		System.out.println("date0");
        TimeString t = new TimeString("2000");
		DateTime date = t.getDateTime();

		assertEquals(date.getYear(), 2000);
	}

	@Test
	public void testDate1()
	{
		System.out.println("date1");
        TimeString t = new TimeString("2009-06");
		DateTime date = t.getDateTime();

		assertEquals(2009, date.getYear());
		assertEquals(6, date.getMonthOfYear());
	}

	@Test
	public void testDate2()
	{
		System.out.println("date2");
		DateTime date = TimeString.parse("2006-11-22");

		assertEquals(2006, date.getYear());
		assertEquals(11, date.getMonthOfYear());
		assertEquals(22, date.getDayOfMonth());
	}

    @Test
	public void testDate3()
	{
		System.out.println("date3");
		DateTime date = TimeString.parse("2006-11-22T08");
        long m = TimeString.parseMillis("2006-11-22T08");

		assertEquals(2006, date.getYear());
		assertEquals(11, date.getMonthOfYear());
		assertEquals(22, date.getDayOfMonth());
		assertEquals(8, date.getHourOfDay());
	}

    @Test
	public void testDate4()
	{
		System.out.println("date4");
		DateTime date = TimeString.parse("2006-11-22T08:11");
        long m = TimeString.parseMillis("2006-11-22T08:11");

		assertEquals(2006, date.getYear());
		assertEquals(11, date.getMonthOfYear());
		assertEquals(22, date.getDayOfMonth());
		assertEquals(8, date.getHourOfDay());
        assertEquals(11, date.getMinuteOfHour());
	}

    @Test
	public void testDate5()
	{
		System.out.println("date5");
		DateTime date = TimeString.parse("2006-11-22T08:55:00");
        long m = TimeString.parseMillis("2006-11-22T08:55:00");

		assertEquals(2006, date.getYear());
		assertEquals(11, date.getMonthOfYear());
		assertEquals(22, date.getDayOfMonth());
		assertEquals(8, date.getHourOfDay());
        assertEquals(55, date.getMinuteOfHour());
        assertEquals(0, date.getSecondOfMinute());
	}

	@Test
	public void testDate6()
	{
		System.out.println("date6");
		DateTime date = TimeString.parse("1988-03-01T13:00:00.000Z");
        long m = TimeString.parseMillis("2006-11-22T08:00:00.000Z");

		assertEquals(1988, date.getYear());
		assertEquals(3, date.getMonthOfYear());
		assertEquals(1, date.getDayOfMonth());
		assertEquals(14, date.getHourOfDay());
        assertEquals(0, date.getMinuteOfHour());
        assertEquals(0, date.getSecondOfMinute());
        assertEquals(0, date.getMillisOfSecond());
	}

}
