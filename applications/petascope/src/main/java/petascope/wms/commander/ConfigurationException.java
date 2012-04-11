/** ***********************************************************
 *
 * SOURCE: ConfigurationException.java
 *
 * PACKAGE: petascope.wms.commander
 * CLASS: ConfigurationException
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
 *   $RCSfile: ConfigurationException.java,v $ $Revision: 1.1 $ $State: Exp $
 *   $Locker:  $
 */

package petascope.wms.commander;

/**
 * ConfigurationException: error evaluating configuration file
 */
public class ConfigurationException extends CommanderException
{
    private static final long serialVersionUID = 1L;
    
    /**
     * Constructor for ConfigurationException
     * @param msg String
     */
    public ConfigurationException( String msg )
    {
        super( msg );
    }

}

