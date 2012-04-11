/** ***********************************************************
 *
 * SOURCE: ConnectionFailedException.java
 *
 * PACKAGE: petascope.wms.commander
 * CLASS: ConnectionFailedException
 *
 * CHANGE HISTORY (append further entries):
 * when         who       what
 * ----------------------------------------------------------
 * 2007-jan-05  PB        created
 *
 * Copyright (C) 2007 Dr. Peter Baumann
 *
 *********************************************************** */

/*
 * RCS:
 *   $RCSfile: ConnectionFailedException.java,v $ $Revision: 1.1 $ $State: Exp $
 *   $Locker:  $
 */

package petascope.wms.commander;

/**
 * ConnectionFailedException: error connecting to database
 */
public class ConnectionFailedException extends CommanderException
{
    private static final long serialVersionUID = 1L;
    
    /**
     * Constructor for ConnectionFailedException
     * @param msg String
     */
    public ConnectionFailedException( String msg )
    {
        super( msg );
    }

}


