/*************************************************************
 * <pre>
 * Wrapper for other exceptions or messages that are thrown
 * in the course of computing a ogc request.
 * Message codes must be >= 0.
 * A class which takes up all exceptions that are caught in the
 * WMS OGC code and carries them to the user.
 * It generates version 1.1.0 compliant XML (hopefully).
 *
 * CHANGE HISTORY (append further entries):
 * when        who       what
 * ----------------------------------------------------------
 * 06-07-2001  Beinhofer created
 * 2003-may-15 PB        code-embedded version string
 * 2003-may-25 PB        error texts now from WmsServlet, enum value passed to exception instead of string
 * 2005-jun-24 PB        error texts taken from property file using PropertyResourceBundle
 *                       changed error code prefix to "GWMS" (rasGeo/WMS)
 * 2005-jul-03 PB        ERR_Followup = -1
 * 2007-feb-21  PB      DTD URL digitalearth.com -> schemas.opengis.net
 *
 * COMMENTS:
 * - all response types supported (inimage/jpeg, blank, XML)
 *
 * Copyright (C) 2001 Dr. Peter Baumann
 * </pre>
 *********************************************************** */

package petascope.exceptions;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;
import java.util.MissingResourceException;
import java.util.PropertyResourceBundle;
import java.util.StringTokenizer;
import java.util.Vector;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.wms.Globals;
import petascope.wms.WmsServlet;
import javax.imageio.ImageIO;
import javax.imageio.stream.ImageInputStream;

// import rasj.*;

public class WMSException extends Exception
{
    private static final Logger log = LoggerFactory.getLogger(WMSException.class);
    // --- constants -------------------------------------
    
    private static final String srcFileVersion = "CVS version information: $Source: /home/rasdev/CVS-repository/rasdaman/rasgeo/WMS/WMSException.java,v $ $Revision: 1.2 $";
    
    /// maximum number of digits we expect from an error number as typed into properties file
    private static final int MAX_DIGITS = 5;
    /// text used when nothing else can be found in property file(s)
    private static final String NO_TEXT_AVAILABLE = "(no text available)";
    /// separator in property file between shorthand and long text
    private static final char SHORTHAND_SEPARATOR = ':';
    
    /// sizing parameters for writing of error message into image
    static final int FONTHEIGHT = 14;		// 14pt
    static final int LINEHEIGHT = FONTHEIGHT+4;	// character height + line spacing
    
    /// WMS specific error code prefix
    static final String PREFIX = "GWMS";
    
    // --- variables -------------------------------------
    
    /// resource bundle for property file(s) containing error messages
    private static PropertyResourceBundle messages = null;
    
    /// numeric (!) error code, should only contain values as defined in the code list above
    /// (init value different from any error code and ERR_Followup)
    private static int numericCode = Globals.ERR_Followup-1;
    
    /// shorthand text, as required by WMS WMS, pertaining to error stored in numericCode
    private static String shortHand = null;
    
    /// long description text pertaining to error stored in numericCode
    private static String longText = null;
    
    /// textual information of error as taken from properties file, potentially concatenated with specific text
    private static String errText = null;
    
    // --- methods -------------------------------------
    
    /**
     * Create a new exception
     * @param code the exception code (numeric, as string)
     **/
    public WMSException( int code )
    {
        super( Integer.toString(code) );
        
        if (code != Globals.ERR_Followup)		// remember only if it's not a followup to the "real" exception
        {
            numericCode = code;
            assignMessage( Integer.toString(code), null );	// assign shortHand and longText
        }
    }
    
    /**
     * Create a new exception.
     * @param code the exception code (see Global.java)
     * @param msg an extra message text
     **/
    public WMSException( int code, String msg )
    {
        super( Integer.toString(code) + (msg==null?"":(": " + msg)) );
        
        if (code != Globals.ERR_Followup)		// remember only if it's not a followup to the "real" exception
        {
            numericCode = code;
            assignMessage( Integer.toString(code), msg );	// assign shortHand and longText
        }
    }
    
    /**
     * Return the message text ready for output
     **/
    public String getMessage()
    {
        return longText;
    }
    
    /**
     * Generate version 1.1.0 conforming xml output
     **/
    public String toString()
    {
        StringBuffer retval = new StringBuffer(1024);
        retval.append("<?xml version='1.0' encoding=\"UTF-8\" standalone=\"no\" ?>\n");
        retval.append("<!DOCTYPE ServiceExceptionReport SYSTEM \"http://schemas.opengis.net/wms/1.1.0/exception_1_1_0.dtd\">\n");
        retval.append("<ServiceExceptionReport version=\"1.1.0\">\n");
        retval.append("  <ServiceException code=\"" + shortHand + "\">\n");
        retval.append("    " + longText + "\n");
        retval.append("  </ServiceException>\n");
        retval.append("</ServiceExceptionReport>\n");
        
        return retval.toString();
    }
    
    /**
     * Render message into image, insert line breaks where needed
     **/
    public void toJpeg(BufferedOutputStream os, int width, int height) throws IOException
    {
        // create color matte
        BufferedImage bImage = new BufferedImage(width,height,BufferedImage.TYPE_3BYTE_BGR);
        Graphics2D gr = bImage.createGraphics();
        
        // make background white
        Rectangle rect = new Rectangle(0,0,bImage.getWidth(),bImage.getHeight());
        gr.setColor(Color.white);
        gr.fill(rect);
        
        // write in red
        gr.setColor(Color.red);
        
        // set 14pt sans-serif font
        Font font = new Font("Sans-serif",Font.PLAIN,FONTHEIGHT);
        FontMetrics fm = gr.getFontMetrics(font);
        gr.setFont(font);
        
        // draw text lines into image
        Vector lines = getLinedMessage(width, fm);
        int ycoord = LINEHEIGHT;
        for (int i=0; i<lines.size();i++)
        {
            gr.drawString((String)lines.get(i),1,ycoord);
            ycoord += LINEHEIGHT;
        }

        // ship image as JPEG
        ImageIO.write(bImage,"jpeg",os);
    }
    
    /**
     * Set object variables shortHand, longText with message texts determined from message resource;
     * initialize resource bundle if not done.
     **/
    private void assignMessage( String code, String msg)
    {
        // try to allocate resources (=message texts), if not done already
        if (messages==null)
        {
            // discover message resources from file name passed in web.xml
            try
            {
                // - open file
                // Note: cannot use ResourceBundle, because it can only read classpath dirs + subdirs
                File messageFileHandle = new File( WmsServlet.messageFile );	// get a file handle
                FileInputStream messageStream = new FileInputStream( messageFileHandle );
                // - get error code list
                messages = new PropertyResourceBundle( (InputStream) messageStream );	// evaluate resources in property file
                log.info( "Exception message file " + WmsServlet.messageFile + " successfully opened, file contains " + messageFileHandle.length() + " bytes." );
            }
            catch (IOException io)
            {
                log.error( "WMSException::WMSException(" + code + "," + msg + "): warning: cannot read message file pertaining to resource " + WmsServlet.messageFile + "; no texts will be available." );
                messages = null;
            }
            catch (MissingResourceException e)
            {
                log.error( "WMSException::WMSException(" + code + "," + msg + "): warning: cannot find message file pertaining to resource " + WmsServlet.messageFile + "; no texts will be available." );
                messages = null;
            }
            catch (Exception e)
            {
                log.error( "WMSException::WMSException(" + code + "," + msg + "): warning: general exception while trying to open message file " + WmsServlet.messageFile + "; no texts will be available." );
                messages = null;
            }
        }
        
        // searchNumber is the numeric code, but successively extended with leading 0 until found...or not.
        String searchNumber = code;
        // text as retrieved from resource file
        String resourceText = null;
        // try this number, and if not found with leading 0s, until we lose interest
        // ...but only if resource bundle initialization was successful
        while (messages !=null && resourceText == null && searchNumber.length() <= MAX_DIGITS)
        {
            try
            {
                // try to find text in resource
                resourceText = messages.getString( searchNumber );
            }
            catch (MissingResourceException e)
            {
                // if not, try with adding a leading 0
                searchNumber = "0" + searchNumber;
            }
        }
        
        if (resourceText==null)
        {
            shortHand = NO_TEXT_AVAILABLE;
            longText = PREFIX + searchNumber + ": " + NO_TEXT_AVAILABLE;
        }
        else
        {
            // split shorthand from long text
            int splitPoint = resourceText.indexOf( SHORTHAND_SEPARATOR );
            shortHand      = resourceText.substring( 0, splitPoint );
            // determine long text, add msg if relevant
            longText       = PREFIX + searchNumber + ": " + resourceText.substring( splitPoint+1, resourceText.length() ) + (msg==null?"":msg);
        }
    }
    
    /**
     * Break message text into several lines so that it fits window
     **/
    private Vector getLinedMessage(int imageWidth, FontMetrics fm)
    {
        Vector lines = new Vector();
        String msg = getMessage();
        
        StringTokenizer tok = new StringTokenizer( msg );
        
        log.debug( "WMSException.getLinedMessage: imageWidth=" + imageWidth + ", message=" + msg );
        
        String line = "";
        int lineWidth = 1;
        while(tok.hasMoreTokens())
        {
            String word = tok.nextToken();
            int wWidth = fm.stringWidth(word);
            
            int spaceWidth = lineWidth==1 ? 0:fm.stringWidth(" ");;
            
            if( (lineWidth+wWidth) < imageWidth)
            {
                // log.debug( "WMSException.getLinedMessage: lineWidth=" + lineWidth + " wWidth =" + wWidth + " imageWidth=" + imageWidth + " -fits in line->" + line);
                lineWidth += spaceWidth + wWidth;
                line += " " + word;
            }
            else
            {
                // log.debug( "WMSException.getLinedMessage: lineWidth=" + lineWidth + " wWidth =" + wWidth + " -does NOT fit in line!");
                if( lineWidth == 1)
                {
                    lines.add(word);
                    line = ""; lineWidth=1;
                }
                else
                {
                    lines.add(line);
                    line = word;
                    lineWidth=wWidth+1;
                }
            }
        }
        
        if(lineWidth != 1)
            lines.add(line);
        
        return lines;
    } // getLinedMessage()
    
    
    /**
     * for testing purposes, provide standalone facility
     * Note: not yet - needs messageFile from WmsServlet + specific Debug.java + Globals.java
     **/
    public static void main( String[] args)
    {
        System.out.println( "TestMessages rasdaman WMS test application" );
        
        // --- parameter evaluation
        if (args.length != 3)
        {
            System.out.println( "usage: java _thisTestApplication_ messageFile messageNumber extraText" );
            System.out.println( "  where propertyFile does NOT have  extension" );
            System.exit(2);	// usage exit code, per Linux convention
        }
        WmsServlet.messageFile = args[0];
        String testError = args[1];
        String extraText = args[2];
        
        // --- action
        System.out.println( "" );
        WMSException myException = new WMSException( Integer.parseInt(testError), extraText );
        System.out.println( myException.getMessage() );
        System.out.println( myException.toString() );
        
        System.out.println( "TestMessages done." );
    }

} // WMSException

