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
package petascope.util;

import java.io.IOException;
import java.util.Locale;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class MsgUtilTest {

    /**
     * Test of info method, of class MsgUtil.
     */
    @Test
    public void testInfo_String() {
        String key = "test";
        String expResult = "test";
        String result = MsgUtil.msg(key);
        assertEquals(expResult, result);
    }

    /**
     * Test of info method, of class MsgUtil.
     */
    @Test
    public void testInfo_String_ObjectArr() {
        String key = "test.format";
        Object[] args = new Object[]{"hello"};
        String expResult = "test hello";
        String result = MsgUtil.msg(key, args);
        assertEquals(expResult, result);
    }

    /**
     * Test of exc method, of class MsgUtil.
     */
    @Test
    public void testExc() {
        Exception ex = new IOException("IO exception with long long long long "
                + "title, bla bla bla bla bla bla bla bla bla bla bla bla bla bla bla bla bla bla "
                + "bla bla bla bla bla bla bla bla bla bla bla bla bla bla bla bla bla bla bla");
        String expResult = "IO exception with long long long long title, bla bla bla bla bla bla\n"
                + " bla bla bla bla bla bla bla bla bla bla bla bla bla bla bla bla bla\n"
                + " bla bla bla bla bla bla bla bla bla bla bla bla bla bla";
        String result = MsgUtil.exc(ex);
        assertEquals(expResult, result);
    }
}
