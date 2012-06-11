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
 * Interface for authenticating client requests to the ras WMSBasic server
 * An interface which allows the customer to implement his own authentication mechanism.
 * @version $Revision: 1.2 $
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

package petascope.wms;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.exceptions.WMSException;

public class WmsBasicAuthorisation implements WmsAuthorisation
{
    private static final Logger log = LoggerFactory.getLogger(WmsBasicAuthorisation.class);
    private String myMasterKey;
    private String myAuthorisationURLBegin;
    private String myAuthorisationURLEnd;
    
    public WmsBasicAuthorisation()
    {
        myMasterKey = "rasdaman";
    }
    
    /**
     * Pass any arguments to the authorisation class.
     * @param arguments An array of String of length 3.  The first String in it is the master key, the second is the first part of the url to get the authorisation from, and the third String is the second part of the url to get the authorisation from.  Between the first and the second part the request will be inserted.
     * @exception WMSException Is thrown if the arguments are not correct or any other error occured during initialisation.
     **/
    public void initialise(String[] arguments) throws WMSException
    {
        if (arguments != null)
        {
            if (arguments.length == 3)
            {
                myMasterKey = arguments[0];
                if (myMasterKey != null)
                    if (myMasterKey.length() == 0)
                        myMasterKey = null;
                myAuthorisationURLBegin = arguments[1];
                if (myAuthorisationURLBegin != null)
                    if (myAuthorisationURLBegin.length() == 0)
                        myAuthorisationURLBegin = null;
                myAuthorisationURLEnd = arguments[2];
                if (myAuthorisationURLEnd != null)
                    if (myAuthorisationURLEnd.length() == 0)
                        myAuthorisationURLEnd = null;
            }
            else	{
                log.warn( this.getClass().getName() + ":initialise expected two Strings, received: " + arguments );
                throw new WMSException( Globals.ERR_AuthParamIllegal );
            }
        }
    }
    
    /**
     * Create a new exception
     * @param theRequest The request which has to be authorised
     **/
    public void checkAuthorisation(WmsRequest theRequest) throws WMSException
    {
        switch (theRequest.getRequestType())
        {
            case WmsRequest.FEATURE:
                return;
            case WmsRequest.CAPABILITIES:
                return;
            default:
                //do nothing -> check the validity
        }
        String fees = theRequest.getFees();
        if (fees != null)
        {
            if (myMasterKey != null)
                if (fees.equals(myMasterKey))
                    return;
            if (fees.equals(encode(theRequest)))
                return;
        }
        StringBuffer msg = new StringBuffer();
        msg.append("The FEES you supplied are not correct.");
        if (myAuthorisationURLBegin != null && myAuthorisationURLEnd != null)
        {
            msg.append("Please visit:");
            msg.append(myAuthorisationURLBegin);
            msg.append(theRequest);
            msg.append(myAuthorisationURLEnd);
        }
        throw new WMSException( Globals.ERR_AuthFailure, msg.toString() );
    }
    
    /**
     * Create a key from the request.
     * @param theRequest the request which will be encoded
     **/
    private String encode(WmsRequest theRequest)
    {
        int retval = 0;
        StringBuffer buf = new StringBuffer(512);
        buf.append(theRequest.getClient());
        buf.append(theRequest.getHeight());
        buf.append(theRequest.getWidth());
        buf.append(theRequest.getSRS());
        buf.append(theRequest.getTime());
        buf.append(theRequest.getElevation());
        buf.append(theRequest.getBoundingBox());
        byte[] theBytes = buf.toString().getBytes();
        for (int i = 0; i < theBytes.length; i++)
            retval = retval + theBytes[i];
        return Integer.toString(retval);
    }
    
    /**
     * Generate a request that will be authorised.
     * @param theRequest the request that will be cahnged to be authorised by the isAuthorised method of the same class.
     **/
    public void authorise(WmsRequest theRequest)
    {
        theRequest.setFees(encode(theRequest));
    }
}

