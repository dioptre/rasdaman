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
 *
 * PURPOSE:
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

package petascope.wms;

import java.util.LinkedList;
import java.util.List;
import java.util.StringTokenizer;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;


/**
 * class for parsing the parameters in the openGISServlet URL
 * @version $Revision: 1.3 $
 */
public class ParseParameters
{
    // parameters
    /**  version number of map server (1.0.0) */
    String version;
    /**  request type: map, feature_info or capabilities */
    String request;
    /** list with layers and styles */
    List layerList;
    /** srs text that names a horizontal coordinate system reference code */
    String srs;
    /** bounding box corners: lower left (x,y) and upper right (x,y) */
    BoundingBox bbox;
    /** output width and height in pixels */
    int width, height;
    /** output format: JPEG, TIFF, PNG */
    String format;
    
    /**
     * method for parsing the parameters in the URL
     */
    public String parse(HttpServletRequest req) throws ServletException
    {
        version = "";
        request = "";
        layerList = null;
        srs = "";
        bbox = null;
        width = 0;
        height = 0;
        format = "";
        
        // optional parameters
        String trans = "";
        String bgcolor = "";
        String except = "";
        
        // version number
        version = req.getParameter(WmsRequest.WMSREQ_KEY_WMTVER);
        
        // request type
        request = req.getParameter(WmsRequest.WMSREQ_KEY_REQUEST);
        if(request == null)
            throw new ServletException("exception while parsing REQUEST parameter!!! null");
        
        // request for map
        if(request.equals(WmsRequest.WMSREQ_MAP))
        {
            // layers
            String layers = req.getParameter(WmsRequest.WMSREQ_KEY_LAYERS);
            if(layers == null)
                throw new ServletException("exception while parsing LAYERS parameter!!! null");
            
            // rendering styles
            String styles = req.getParameter(WmsRequest.WMSREQ_KEY_STYLES);
            if(styles == null)
                throw new ServletException("exception while parsing STYLES parameter!!! null");
            
            StringTokenizer strTok1 = new StringTokenizer(layers, ",");
            StringTokenizer strTok2 = new StringTokenizer(styles, ",");
            if(strTok1.countTokens() != strTok2.countTokens())
                throw new ServletException("unequal count of layers and styles!!! " + strTok1.countTokens() + " : " + strTok2.countTokens());
            List layerList = new LinkedList();
            LayerStyle  ls = null;
            while(strTok1.hasMoreTokens())
            {
                ls = new LayerStyle(strTok1.nextToken(), strTok2.nextToken());
                layerList.add(ls);
            }
            
            // coordinates
            // horizontal coordinate reference system code
            srs = req.getParameter(WmsRequest.WMSREQ_KEY_SRS);
            if(srs == null)
                throw new ServletException("exception while parsing SRS parameter!!! null");
            
            // bounding box: minimum x, minimum y, maximum x, maximum y
            String box= req.getParameter(WmsRequest.WMSREQ_KEY_BBOX);
            if(box == null)
                throw new ServletException("exception while parsing BBOX parameter!!! null");
            
            double minX, minY, maxX, maxY = 0;
            StringTokenizer strTok = new StringTokenizer(box, ",");
            try
            {
                bbox = new BoundingBox(Double.parseDouble(strTok.nextToken()), Double.parseDouble(strTok.nextToken()),
                        Double.parseDouble(strTok.nextToken()), Double.parseDouble(strTok.nextToken()));
            }
            catch(Exception e)
            {
                throw new ServletException("exception while parsing BBOX parameter!!! " + e.getMessage());
            }
            
            // output width and height in pixels
            try
            {
                width = Integer.parseInt(req.getParameter(WmsRequest.WMSREQ_KEY_WIDTH));
            }
            catch(Exception e)
            {
                throw new ServletException("exception while parsing WIDTH parameter!!! " + e.getMessage());
            }
            try
            {
                height = Integer.parseInt(req.getParameter(WmsRequest.WMSREQ_KEY_HEIGHT));
            }
            catch(Exception e)
            {
                throw new ServletException("exception while parsing HEIGHT parameter!!! " + e.getMessage());
            }
            
            // output format
            format = req.getParameter(WmsRequest.WMSREQ_KEY_FORMAT);
            if(format == null)
                throw new ServletException("exception while parsing FORMAT parameter!!! null");
            if(format.equals(WmsRequest.WMSREQ_FORMAT_JPEG) ||
                    format.equals(WmsRequest.WMSREQ_FORMAT_PNG) ||
                    format.equals(WmsRequest.WMSREQ_FORMAT_TIFF))
                // OK format supported
                ;
            else
                throw new ServletException("output format " + format + " is not supported, only JPEG, PNG, TIFF");
            
            //optional parameters
            // transparent
            trans = req.getParameter(WmsRequest.WMSREQ_KEY_TRANSPARENT);
            if(trans != null)
                throw new ServletException("TRANSPARENT parameter is not supported yet!!!");
            
            // background
            bgcolor = req.getParameter(WmsRequest.WMSREQ_KEY_BGCOLOR);
            if(bgcolor != null)
                throw new ServletException("BGCOLOR parameter is not supported yet!!!");
            
            // exceptions
            except = req.getParameter(WmsRequest.WMSREQ_KEY_EXCEPTIONS);
            if(except != null)
                throw new ServletException("EXCEPTIONS parameter is not supported yet!!!");
            
            return version + " " + request + " " +  layerList + " " +
                    srs + " " + bbox + " " + width + " " + height + " "+ format;
        }
        
        // request for feature_info or capabilities
        else if(request.equals(WmsRequest.WMSREQ_FEATURE_INFO))
            throw new ServletException("request type feature_info is not supported yet!!!");
        
        // request for capabilities
        else if(request.equals(WmsRequest.WMSREQ_CAPABILITIES))
            return version + " " + request;
        
        // unknown request type
        else
            throw new ServletException("unknown request type");
    }
}
