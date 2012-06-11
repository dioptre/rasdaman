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
/*************************************************************
 * <pre>
 * Interface for authenticating client requests to the ras wms server.
 * An interface which allows the customer to implement his own authentication mechanism.
 * @version $Revision: 1.2 $
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

package petascope.wms;

import petascope.exceptions.WMSException;

public interface WmsAuthorisation
{
    /**
     * Pass any arguments to the authorisation class.
     * @param arguments An array of String arguments
     * @exception WMSException Is thrown if the arguments are not correct or any other error occured during initialisation.
     **/
    public void initialise(String[] arguments) throws WMSException;
    
    /**
     * Authorise a specific request.
     * @param theRequest The request which has to be authorised
     * @exception WMSException Is thrown if there is a problem with the request.
     **/
    public void checkAuthorisation(WmsRequest theRequest) throws WMSException;
    
    /**
     * Create a request that will be authorised.
     * @param theRequest The request which will be changed so that it will be authorised
     * @exception WMSException Is thrown if there is a real problem with the request.
     **/
    public void authorise(WmsRequest theRequest) throws WMSException;
}
