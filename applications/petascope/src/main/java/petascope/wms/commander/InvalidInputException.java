/** ***********************************************************
 *
 * SOURCE: InvalidInputException.java
 *
 * PACKAGE: petascope.wms.commander
 * CLASS: InvalidInputException
 *
 * CHANGE HISTORY (append further entries):
 * when         who       what
 * ----------------------------------------------------------
 * 2007-feb-15  SS        created
 *
 * Copyright (C) 2007 Dr. Peter Baumann
 *
 *********************************************************** */


package petascope.wms.commander;

/**
 * InvalidInputException: invalid input passed from web form
 */
public class InvalidInputException extends CommanderException
{
    private static final long serialVersionUID = 1L;
    
    /**
     * Constructor for ConnectionFailedException
     * @param msg String
     */
    public InvalidInputException( String msg )
    {
        super( msg );
    }

}


