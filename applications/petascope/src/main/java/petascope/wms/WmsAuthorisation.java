/*************************************************************
 * <pre>
 * Interface for authenticating client requests to the ras wms server.
 * An interface which allows the customer to implement his own authentication mechanism.
 * @version $Revision: 1.2 $
 *
 * CHANGE HISTORY (append further entries):
 * when        who         what
 * ----------------------------------------------------------
 * 10-10-2001  hoefner   created
 * 2003-may-15  PB      code-embedded version string
 *
 *
 * COMMENTS:
 *
 * Copyright (C) 2001 Dr. Peter Baumann
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
