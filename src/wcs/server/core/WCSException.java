package wcs.server.core;
import net.opengis.ows.v_1_0_0.ExceptionReport;
import net.opengis.ows.v_1_0_0.ExceptionType;

/**
 * Exception class for the WCS server. 
 * This class can return an error report, than can be marshalled into a
 * WCS-standard compliant XML structure describing the error that has happened.
 *
 * @author Andrei Aiordachioaie
 */
public class WCSException extends Exception {
    
    private static final long serialVersionUID = 847843429L;

    private ExceptionReport report;
    private ExceptionType item;

    /**
     * Default (minimal) constructor
     * @param error Error Code
     * @param detail Detailed message about the error
     */
    public WCSException(String error, String detail)
    {
        report = new ExceptionReport();
        report.setLanguage("en");
        report.setVersion("1.0.0");
        if (error.equalsIgnoreCase("MissingParameterValue") ||
                error.equalsIgnoreCase("InvalidParameterValue") ||
                error.equalsIgnoreCase("NoApplicableCode") ||
                error.equalsIgnoreCase("UnsupportedCombination") ||
                error.equalsIgnoreCase("NotEnoughStorage"))
        {
            item = new ExceptionType();
            item.setExceptionCode(error);
            item.setLocator(detail);
            report.getException().add(item);
        }
        else
        {
            /* This WCS Exception code is not recognized. But should still be
             able to provide an XML ErrorReport */
            item = new ExceptionType();
            item.setExceptionCode("NoApplicableCode");
            item.setLocator("Internal Error Code: " + error + " ... Internal Error Message: " + detail);
            report.getException().add(item);
        }
    }

    /**
     * Convenience Constructor
     * @param error Error Code
     * @param detail Detailed message about the error
     * @param msg A message that will be output on *System.err*
     */
    public WCSException(String error, String detail, String msg)
    {
        this(error, detail);

        System.err.println(msg);
    }

    /**
     * Retrieves a data structure that can be later marshalled into a XML
     * "ExceptionReport" document.
     * @return ExceptionReport object
     */
    public ExceptionReport getReport()
    {
        return report;
    }
}