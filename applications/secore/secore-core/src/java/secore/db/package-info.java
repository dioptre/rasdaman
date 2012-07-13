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

/*
 * Code that handles connection to the back-end database.
 * <p>
 * {@link Database} declares a generic database interface that is used 
 * throughout secore. Concrete classes can implement details for different 
 * databases (or the same database in different ways). {@link BaseX} for example
 * implements the interface with code specific to the BaseX database.
 * <p>
 * The {@link DbManager} is used to set the specific {@link Database} instance
 * that will be used.
 */
package secore.db;
