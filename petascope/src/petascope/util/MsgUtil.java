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
package petascope.util;

import java.text.MessageFormat;
import java.util.Locale;
import java.util.ResourceBundle;

/**
 * Get messages from resource bundles.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class MsgUtil {

    private static ResourceBundle msgs;
    private static final String MESSAGES_PATH = "messages/";
    private static final String INFO_MESSAGES = "messages";

    static {
        configure(MESSAGES_PATH);
    }

    public static void configure(final String msgPath) {
        configure(msgPath, Locale.getDefault());
    }

    public static void configure(String msgPath, Locale locale) {
        assert locale != null;
        if (!msgPath.endsWith("/")) {
            msgPath += "/";
        }
        msgs = ResourceBundle.getBundle(msgPath + INFO_MESSAGES, locale);
    }

    public static String msg(String key) {
        assert key != null && msgs.containsKey(key);
        return msgs.getString(key);
    }

    /**
     * @return message formatted with <code>args</code>
     */
    public static String msg(String key, Object... args) {
        assert args.length != 0;
        return MessageFormat.format(msg(key), args);
    }

    /**
     * @return a wrapped exception message
     */
    public static String exc(Exception ex) {
        return StringUtil.wrap(ex.getLocalizedMessage(), 70);
    }
    // message keys
    public static final String SETTINGS_FAILED_LOADING = "settings.failed_loading";
    public static final String SETTINGS_INITERROR = "settings.initerror";
}
