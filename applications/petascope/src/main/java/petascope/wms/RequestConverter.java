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
 * <PRE>
 * constructs a rasql query from a request passed. *
 *
 * COMMENT:
 * - FIXME: for export, large zoom out has not been inspected yet
 * - FIXME: user/password should not be exported!
 * - test of exception capabilities is not necessary, as code supports it anyway
 *   so exception capabilities should always be set to true in raswms.cfg
 *
 * </PRE>
 ************************************************************/

package petascope.wms;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.Vector;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.exceptions.WMSException;

public class RequestConverter
{
    private static final Logger log = LoggerFactory.getLogger(RequestConverter.class);
    
    // Rasql constants
    public static final String RASQL_SELECT = "SELECT";
    public static final String RASQL_FROM = "FROM";
    public static final String RASQL_AS = "AS";
    public static final String RASQL_AND = "AND";
    public static final String RASQL_JPEG = "jpeg";
    public static final String RASQL_PNG = "png";
    public static final String RASQL_TIFF = "tiff";
    public static final String RASQL_OVERLAY = "overlay";
    public static final String RASQL_DYNAMIC = "dynamic";
    public static final String RASQL_MARRAY = "marray";
    public static final String RASQL_VALUES = "values";
    public static final String RASQL_CHAR = "char";
    public static final String RASQL_IN = "in";
    public static final String RASQL_SCALE = "scale";
    public static final String RASQL_EXTEND = "extend";
    
    // TODO: define constants for all other Rasql keywords: SELECT, AS, etc.
    /////////////////////////////////////////////
    /**
     * stem of the FROM clause variable names (numbers will be appended, such as img1, img2, ...)
     **/
    private static final String FROM_VAR_TEMPLATE = "img";
    
    private WmsCapabilities.WmsCapability myCapability = null;
    
    private List wmsLayers = null;
    private List wmsStyles = null;
    String imageFormatName = null;
    String customDem       = null;
    
    RequestConverter(WmsCapabilities.WmsCapability ourCapability)
    {
        myCapability = ourCapability;
    }
    
    String getImageFormatName()
    {
        return imageFormatName;
    }
    
    String buildExpoInfo(WmsRequest request) throws WMSException
    {
        log.info( "buildExpoInfo start");
        
        WmsConfig config = WmsConfig.getInstance();
        
        String user     = config.getRequiredParameter(WmsConfig.PARAMETER_USER);
        String passwd   = config.getRequiredParameter(WmsConfig.PARAMETER_PASSWD);
        String server   = config.getRequiredParameter(WmsConfig.PARAMETER_SERVER);
        String database = config.getRequiredParameter(WmsConfig.PARAMETER_DATABASE);
        String port     = config.getRequiredParameter(WmsConfig.PARAMETER_PORT);
        
        customDem = request.getCustomDem();
        
        String connectionInfo = server + ',' + port + ',' + database + ',' + user + ',' + passwd;
        
        int imageWidth = request.getWidth();
        if (imageWidth <= 0)
            throw new WMSException( Globals.ERR_WidthNotPositive, Integer.toString(imageWidth) );
        
        int imageHeight = request.getHeight();
        if (imageHeight <= 0)
            throw new WMSException( Globals.ERR_HeightNotPositive, Integer.toString(imageHeight) );
        
        BoundingBox boundingBox = request.getBoundingBox();
        double extentX = boundingBox.maxX - boundingBox.minX;
        double extentY = boundingBox.maxY - boundingBox.minY;
        
        //compute the scale factor.
        
        double scaleFactorX = extentX/imageWidth;
        double scaleFactorY = extentY/imageHeight;
        double scaleFactor  = scaleFactorX <scaleFactorY ? scaleFactorX : scaleFactorY;
        
        buildLayersAndStyles(request);
        
        Iterator it = wmsLayers.iterator();
        Iterator its = wmsStyles.iterator();
        
        WmsCapabilities.WmsLayer wmslayer = null;
        WmsCapabilities.WmsStyle wmsstyle = null;
        RasPersistentScaleLayer pslayer = null;
        
        /*
         * ATTENTION: here we build the export info, making a tiny correction.
         * We correct the exported bbox so it fits in an full number of pixel of the collection and than size if the image so the
         * scale factor is preserved. But: the final values are taken from the uppermost layer, which is the last here.
         */
        // we use the initial bbox to be sure we have something in this string
        String correctedBBox  = "#" + boundingBox.minX + ',' + boundingBox.minY + ',' + boundingBox.maxX + ',' + boundingBox.maxY;
        Vector exportedLayers = new Vector();
        
        int imgNum = 0;
        while (it.hasNext())
        {	//iterate through the requested layers
            //create the renamed variable
            wmslayer = (WmsCapabilities.WmsLayer)it.next();
            wmsstyle = (WmsCapabilities.WmsStyle)its.next();
            
            //find smallest ground resolution in persistent scale layers
            pslayer = getNearestScaleLayer(wmslayer, boundingBox, scaleFactor);
            if (pslayer == null)
                throw new WMSException( Globals.ERR_NoLayerDef, wmslayer.getName() );
            
            String pLayerName = pslayer.getName();
            //compute the scale from persistent collection ground resolution to requested ground resolution
            double pLayerFactor = pslayer.getGroundResolutionX();
            //compute the ras coordinates
            BoundingBox pLayerBox = pslayer.getBoundingBox();
            //remember:  y coordinates are flipped
            double dMinX = (boundingBox.minX - pLayerBox.minX) / pLayerFactor;
            double dMinY = (pLayerBox.maxY - boundingBox.maxY) / pLayerFactor;
            double dMaxX = (boundingBox.maxX - pLayerBox.minX) / pLayerFactor;
            double dMaxY = (pLayerBox.maxY - boundingBox.minY) / pLayerFactor;
            
            {	// correction for Schuetze - bounding box should be given from full pixel to full pixel
                dMinX = Math.floor(dMinX);
                dMinY = Math.floor(dMinY);
                dMaxX = Math.ceil(dMaxX);
                dMaxY = Math.ceil(dMaxY);
                
                double corrBBoxMinX = dMinX * pLayerFactor + pLayerBox.minX;
                double corrBBoxMinY = pLayerBox.maxY - dMaxY * pLayerFactor;
                double corrBBoxMaxX = dMaxX * pLayerFactor + pLayerBox.minX;
                double corrBBoxMaxY = pLayerBox.maxY - dMinY * pLayerFactor;
                
                // computed for all layers, but we keep only the last
                correctedBBox = "#" + corrBBoxMinX + ',' + corrBBoxMinY + ',' + corrBBoxMaxX + ',' + corrBBoxMaxY;
                
                if (pLayerFactor == scaleFactor && it.hasNext()==false)
                {	// for export we allow an correction of image height and width, according to the new bounding box
                    // but only if the resolution match and this is the last layer. If not, it doesn't matter
                    imageWidth  = (int)((corrBBoxMaxX - corrBBoxMinX)/scaleFactor);
                    imageHeight = (int)((corrBBoxMaxY - corrBBoxMinY)/scaleFactor);
                }
            }
            
            long minX = (long)dMinX;
            long minY = (long)dMinY;
            long maxX = (long)dMaxX;
            long maxY = (long)dMaxY;
            
            String imgName = new String( FROM_VAR_TEMPLATE + imgNum);
            imgNum++;
            
            String fullImgExp = buildFullImgExp(wmsstyle, imgName);
            
            exportedLayers.add("#" + pLayerName + ";[" + minX + ":" + maxX + "," + minY + ":" + maxY + "];");
            exportedLayers.add( imgName + ";" + fullImgExp);
            
        }
        
        
        String exportInfo = connectionInfo + correctedBBox;
        String imageSize = "[0:" + (imageWidth-1) + ",0:" + (imageHeight-1) + "];";
        for(int i=0;i < exportedLayers.size(); i++,i++)
        {
            if( i != 0)
                exportInfo += " " + RASQL_OVERLAY;
            exportInfo += (String)exportedLayers.get(i) + imageSize + (String)exportedLayers.get(i+1);
            
        }
        log.info( "buildExpoInfo done.");
        
        return  exportInfo;
    } // buildExpoInfo()
    
    String buildRasQLQuery(WmsRequest request) throws WMSException
    {
        customDem = request.getCustomDem();
        
        int requestFormat = request.getImageFormat();
        
        imageFormatName = getImageFormatName(requestFormat);
        
        // obsolete since all exception types are supported -- PB 2005-jul-08
        // testExceptionType(request.getExceptionType());
        
        int imageWidth = request.getWidth();
        if (imageWidth <= 0)
            throw new WMSException( Globals.ERR_WidthNotPositive, Integer.toString(imageWidth) );
        
        int imageHeight = request.getHeight();
        if (imageHeight <= 0)
            throw new WMSException( Globals.ERR_HeightNotPositive, Integer.toString(imageHeight) );
        
        BoundingBox boundingBox = request.getBoundingBox();
        double extentX = boundingBox.maxX - boundingBox.minX;
        double extentY = boundingBox.maxY - boundingBox.minY;
        
        //compute the scale factor.
        
        double scaleFactorX = extentX/imageWidth;
        double scaleFactorY = extentY/imageHeight;
        double scaleFactor  = scaleFactorX <scaleFactorY ? scaleFactorX : scaleFactorY;
        
        buildLayersAndStyles(request);
        
        /* now we checked everything and can build the query:
         * scaleFactor is the factor from extent width / image width
         * boundingBox is the requested box
         * extentX     is the extent of the request box
         * extentY     is the extent of the request box
         * imageWidth  is the width of the image
         * imageHeight is the height of the image
         * wmslayers   is list of WmsLayers needed
         * wmsstyles   is list of WmsStyles needed
         */
        
        String query= generateQuery(boundingBox, imageWidth, imageHeight, scaleFactor, wmsLayers, wmsStyles, requestFormat, request.bgColorRed, request.bgColorGreen, request.bgColorBlue, request.isTransparent() );
        
        return query;
    } // buildRasQLQuery()
    
    /**
     * This method generates the actual RasDaMan query.
     * @param boundingBox the requested area, in geo coords; no longer required to be within map boundaries (-> arbitrary zoom out possible now)
     * @param requestedGroundResolution the resolution (aka zoom factor)
     * @param wmslayers the layers that were requested
     * @param wmsstyles the styles that belong to the layers
     * @param imageFormat the type of image to return
     **/
    
    protected String generateQuery(BoundingBox boundingBox, int imageWidth, int imageHeight, double requestedGroundResolution, List wmslayers, List wmsstyles, int imageFormat, int bgColorRed, int bgColorGreen, int bgColorBlue, boolean hasTransparency ) throws WMSException
    {
        log.info( "generateQuery start");
        
        StringBuffer layerStack = new StringBuffer(2000);	//
        StringBuffer selectClause = new StringBuffer(2000);	// rasql select clause
        StringBuffer fromClause = new StringBuffer(1000);	// rasql from clause
        String imgName = null;	// symbolic name in query ("as") for maps, suffixed with stack layer number
        String imgExp = null;					// interim image expression of one map layer,
        // ...w/o WMS rasql fragment
        String fullImgExp = null;				// img expression expanded with WMS rasql fragment
        // ...delivers an RGB image
        String pLayerName = null;				// name of current layer
        RasPersistentScaleLayer pslayer = null;
        int imgNum = 0;						// position of map layer in stack
        double rasScaleFactor = 0;
        //for computation of rascoordinates
        double pLayerFactor = 0;				// MDD scale factor of current layer, depending on pyramid level
        BoundingBox pLayerBox = null;				// bbox of current layer
        
        WmsCapabilities.WmsLayer wmslayer = null;		// layer currently inspected
        WmsCapabilities.WmsStyle wmsstyle = null;		// style currently inspected
        
        // canonical target box (for each layer shifted to its particular position)
        String targetBox = "[0:" + (imageWidth-1) + ",0:" + (imageHeight-1) + "]" ;
        
        // initialize select clause string
        // - format encoding
        String buf = RASQL_SELECT + " " + convertImageFormat(imageFormat) + "( ";
        selectClause.append(buf);
        // - set background
        buf = "(" + RASQL_SCALE + "(" + RASQL_MARRAY + " x " + RASQL_IN + " [1:1,1:1] " + 
                RASQL_VALUES + " {" + bgColorRed + "c," + bgColorGreen + "c," + bgColorBlue + "c}, " + targetBox + "))";
        selectClause.append(buf);
        // initialize from clause string
        fromClause.append( " " + RASQL_FROM + " " );				// initialize FROM clause
        boolean firstFrom = true;				// first FROM variable? otherwise need to append ","
        
        // iterate over layer stack to compose query
        Iterator it = wmsLayers.iterator();
        Iterator its = wmsStyles.iterator();
        while (it.hasNext())
        {	//iterate through the requested layers
            //create the renamed variable
            imgName = FROM_VAR_TEMPLATE + imgNum;
            wmslayer = (WmsCapabilities.WmsLayer)it.next();
            wmsstyle = (WmsCapabilities.WmsStyle)its.next();
            
            //find nearest ground resolution in persistent scale layers, ie, best fitting layer
            pslayer = getNearestScaleLayer(wmslayer, boundingBox, requestedGroundResolution);
            if (pslayer == null)
                throw new WMSException( Globals.ERR_NoPyramidLayer, 
                        "layer=" + wmslayer.getName() + ", boundingBox=" + boundingBox + ", resolution=" + requestedGroundResolution);
            
            pLayerName = pslayer.getName();
            //compute scale from persistent collection ground resolution to requested ground resolution
            pLayerFactor = pslayer.getGroundResolutionX();
            //compute ras coordinates
            pLayerBox = pslayer.getBoundingBox();
            if ( pLayerBox.covers( boundingBox ) )	// bbox is within map layer
            {
                log.debug( "generateQuery: request bbox within this layer." );
                //remember:  y coordinates are flipped
                long boxPixMinX = (long)((boundingBox.minX - pLayerBox.minX) / pLayerFactor);
                long boxPixMinY = (long)((pLayerBox.maxY - boundingBox.maxY) / pLayerFactor);
                long boxPixMaxX = (long)((boundingBox.maxX - pLayerBox.minX) / pLayerFactor);
                long boxPixMaxY = (long)((pLayerBox.maxY - boundingBox.minY) / pLayerFactor);
                imgExp = RASQL_SCALE + "( " + imgName + "[" + 
                        boxPixMinX + ":" + boxPixMaxX + "," + boxPixMinY + ":" + boxPixMaxY + "]," + targetBox + " )";
                fullImgExp=buildFullImgExp(wmsstyle, imgExp);	// add WMS rasql op fragment
            }
            else if ( pLayerBox.disjoint(boundingBox) )
                // bbox is completely outside of layer ("far far zoom")
                // -> add transparent layer (don't omit, it might be the only one!)
            {
                log.debug( "generateQuery: request bbox completely outside this layer " + pLayerBox + ", adding all-transparent layer." );
                fullImgExp = "(" + RASQL_SCALE + "(" + RASQL_MARRAY + " x " + 
                        RASQL_IN + " [1:1,1:1] " + RASQL_VALUES + " {0c,0c,0c}, " + targetBox + "))";
                // thought imgExpr is adequate, but that leaves fullImgExpr undefined -- PB 2005-oct-07
                // note: here no WMS rasql op fragment to add, we're complete already
            }
            else	// bbox is partially outside map layer ("far zoom")
                // NB: do not use sdom(layer).hi in rasql, because initgeo rounds up map size to multiples of, eg, 512!
                // we first extend and then scale; less efficient, but easier & more stable (placement of scaled map!) -- PB 2005-jun-19
            {
                // scale factor resulting from prescaled image + requested scale factor
                double remainingScaleFactor = pLayerFactor / requestedGroundResolution;
                log.debug( "generateQuery: request bbox not within this layer " + pLayerBox + ", need to scale by " + remainingScaleFactor );
                // like above, but instead of boundingBox the whole image size
                // note: we may no longer assume that the lower left of the bbox is (0/0)!
                // note: needed due to a QL bug where bbox larger than sdom is silently cut down to sdom
                
                // - first, again pixel coords of bbox (which now is outside map, at least partially)
                // for each side, check for inside/outside of layer = which edge determines trim box
                double trimEdge;	// holds either bbox or layer edge, whatever is more "inside"
                trimEdge = (boundingBox.minX < pLayerBox.minX) ? pLayerBox.minX : boundingBox.minX;
                long trimPixMinX = (long)((trimEdge - pLayerBox.minX) / pLayerFactor);
                trimEdge = (boundingBox.maxX > pLayerBox.maxX) ? pLayerBox.maxX : boundingBox.maxX;
                long trimPixMaxX = (long)((trimEdge - pLayerBox.minX) / pLayerFactor);
                //remember:  y coordinates are flipped
                trimEdge = (boundingBox.maxY > pLayerBox.maxY) ? pLayerBox.maxY : boundingBox.maxY;
                long trimPixMinY = (long)((pLayerBox.maxY - trimEdge) / pLayerFactor);
                trimEdge = (boundingBox.minY < pLayerBox.minY) ? pLayerBox.minY : boundingBox.minY;
                long trimPixMaxY = (long)((pLayerBox.maxY - trimEdge) / pLayerFactor);
                
                // - then, overall box to be constructed (using extend()) to fill requested area
                long boxPixMinX = (long)((boundingBox.minX - pLayerBox.minX) / pLayerFactor);
                long boxPixMaxX = (long)((boundingBox.maxX - pLayerBox.minX) / pLayerFactor);
                long boxPixMinY = (long)((pLayerBox.maxY - boundingBox.maxY) / pLayerFactor);
                long boxPixMaxY = (long)((pLayerBox.maxY - boundingBox.minY) / pLayerFactor);
                
                imgExp = RASQL_SCALE + "( " + RASQL_EXTEND + "( " + 
                        imgName + "[" + trimPixMinX + ":" + trimPixMaxX + "," + trimPixMinY + ":" + trimPixMaxY + "], [" + 
                        boxPixMinX + ":" + boxPixMaxX + "," + boxPixMinY + ":" + boxPixMaxY + "] ), " + targetBox + " )";
                fullImgExp=buildFullImgExp(wmsstyle, imgExp);	// add WMS rasql op fragment
            }
            
            log.debug( "generateQuery: fullImgExp=" + fullImgExp );
            
            if(fullImgExp.equals(""))
                continue; //possible from DEM
            
            // only if this is a subsequent MDD variable we add, then we need a comma separator
            if (!firstFrom)
            {
                fromClause.append( ", " );
            }
            firstFrom = false;
            
            // append collection iterator (even if map not used - we otherwise might end up without any one, leading to a rasql error)
            buf = pLayerName + " " + RASQL_AS + " " + imgName;
            fromClause.append(buf);
            buf = " " + RASQL_OVERLAY + " (" + fullImgExp + ")";
            selectClause.append(buf);
            
            imgNum++;
        }
        
        // close parenthesis in "FORMAT(...)";
        // on transparency, add resp. option if it's PNG (currently only format ssupported which allows transparency) -- PB 2005-jul-13
        if ( hasTransparency && imageFormat==WmsRequest.PNG)
        {					// add transparency parameters to function call
            buf = ", \"tRNS=(" + bgColorRed + ";" + bgColorGreen + ";" + bgColorBlue + ")\" )";
            selectClause.append(buf);
        }
        else					// simply close FORMAT() function call
            selectClause.append( " )" );
        
        String query = selectClause.toString() + fromClause.toString();
        log.info( "generateQuery done -> " + query.toString() );
        return query.toString();
    } // generateQuery()
    
    // obsolete, color value is constructed in request parser
    private String convertColorToStruct(String colorStr)
    {
        String model = "000000";
        if(colorStr.length() < 6 )
        {
            colorStr = model.substring(0,6 - colorStr.length()) + colorStr;
        }
        
        String rS=colorStr.substring(0,2);
        String gS=colorStr.substring(2,4);
        String bS=colorStr.substring(4,6);
        
        int rI=Integer.parseInt(rS,16);
        int gI=Integer.parseInt(gS,16);
        int bI=Integer.parseInt(bS,16);
        
        return "{" + rI + "c, " + gI + "c, " + bI + "c }";
    }
    
    /**
     * this is a specialized function for creating a dynamic rasql query from the undocumented parameter CUSTOMDEM
     * custmDem is a comma separated list of from,to,color - records
     **/
    private String buildCustomDem(String customDem, String styleName)
    {
        log.info( "buildCustomDem start: customDem=" + customDem + " styleName=" + styleName);
        
        String query="";
        StringTokenizer tok = new StringTokenizer(customDem,",");
        
        boolean needOVERLAY=false;
        
        if(tok.countTokens() > 3)
        {
            while(tok.hasMoreTokens())
            {
                String fromStr = tok.nextToken();
                String toStr   = tok.nextToken();
                String colorStr= tok.nextToken();
                
                if(colorStr.equals(WmsRequest.WMSREQ_CUSTOMDEM_T))
                    continue; // transparent
                
                colorStr = convertColorToStruct(colorStr);
                
                if(needOVERLAY) query += " " + RASQL_OVERLAY + " ";
                boolean needAND = false;
                query +="(";
                if(fromStr.equals(WmsRequest.WMSREQ_CUSTOMDEM_MINLEVEL)==false)
                {
                    query += "(" + styleName + ">=" + fromStr + ")";
                    needAND = true;
                }
                
                if(toStr.equals(WmsRequest.WMSREQ_CUSTOMDEM_MAXLEVEL)==false)
                {
                    if(needAND) query +=" " + RASQL_AND + " ";
                    query += "(" + styleName + "<" + toStr + ")";
                }
                query +=")*" + colorStr;
                needOVERLAY=true;
            }
        }
        else
        {	// only 3 tokens, means minLevel,maxLevel, something
            String fromStr = tok.nextToken();
            String toStr   = tok.nextToken();
            String colorStr= tok.nextToken();
            
            if(colorStr.equals("T"))
            {
                query += "(((" + RASQL_CHAR + ")" + styleName + ")*0c)*{0c,0c,0c}";
            }
            else
            {
                colorStr = convertColorToStruct(colorStr);
                
                query += "(((" + RASQL_CHAR + ")" + styleName + "*0c+1c)*" + colorStr;
                
            }
        }
        
        log.info( "buildCustomDem done. query=" + query );
        return query;
    } // buildCustomDem()
    
    /**
     * build one full image expression (i.e., layer)
     **/
    private String buildFullImgExp(WmsCapabilities.WmsStyle wmsstyle, String imgExp)
    {
        log.info( "buildFullImgExp start: imgExp =" + imgExp );
        
        String rasql = wmsstyle.getRasQL();
        String styleName = wmsstyle.getName();
        
        if(rasql.equals(RASQL_DYNAMIC))
        {
            log.debug( "buildFullImgExp: keyword 'dynamic' -> custom DEM.");
            rasql = buildCustomDem(customDem,styleName);
        }
        
        int startStyleName  = rasql.indexOf(styleName);
        int styleNameLength = styleName.length();
        StringBuffer mddOp  = new StringBuffer(rasql);
        
        log.debug( "buildFullImgExp: mddOp init to: " + mddOp );
        while (startStyleName != -1)
        {	//this is where the image part of a rasql statement is replaced by the scaled expression
            mddOp.replace(startStyleName, startStyleName + styleNameLength, imgExp);
            startStyleName = mddOp.toString().indexOf(styleName);
            log.debug( "buildFullImgExp: mddOp now: " + mddOp );
        }
        
        log.info( "buildFullImgExp done: " + mddOp.toString());
        
        return mddOp.toString();
    } // buildFullImgExp()
    
    /**
     * Compute the best matching layer (i.e., involving minimum scale effort) for a specific request.
     * @param wmslayer the requested layer
     * @param boundingBox the area requested
     * @param requestedGroundResolution the ground resolution requested
     **/
    private RasPersistentScaleLayer getNearestScaleLayer(WmsCapabilities.WmsLayer wmslayer, BoundingBox boundingBox, double requestedGroundResolution)
    {
        log.debug( "getNearestScaleLayer(): boundingBox=" + boundingBox + ", requestedGroundResolution=" + requestedGroundResolution );
        
        RasPersistentScaleLayer retval = null;
        RasPersistentScaleLayer tempLayer = null;
        Iterator it = wmslayer.getCollections().entrySet().iterator();
        double preScaleFactor = -1;
        double currentScaleFactor = -1;
        while (it.hasNext())
        {
            tempLayer = (RasPersistentScaleLayer)((Map.Entry)it.next()).getValue();
            
            log.debug("LAYER: " + tempLayer.getName() + "/" + tempLayer.getGroundResolutionX() + "\n");

            // check disabled because we always want a layer (even for "far zoom out") -- PB 2005-jun-19
            // if (tempLayer.getBoundingBox().covers(boundingBox))
            {	//this is checked because we support also one huge map with a low resolution and many smaller maps with much higher resolutions.
            if (retval == null)
            {	//initialise with any one that fits
                retval = tempLayer;
                currentScaleFactor = retval.getGroundResolutionX();
            }
            else
            {
                preScaleFactor = tempLayer.getGroundResolutionX();
                /*
                 * we need:
                 * a smaller ground resolution than the requested ground resolution
                 * and
                 * a bigger ground resolution than the current ground resolution
                 * log.info( "prescale < requested " + (preScaleFactor < requestedGroundResolution));
                 * log.info( "prescale > current   " + (preScaleFactor > currentScaleFactor));
                 */
                if ((preScaleFactor < requestedGroundResolution) && (preScaleFactor > currentScaleFactor))
                {
                    retval = tempLayer;
                    currentScaleFactor = retval.getGroundResolutionX();
                }
            }
        }
        }
        
        log.info( "getNearestScaleLayer(): chose layer with resolution " + retval.getGroundResolutionX() );
        return retval;
    }
    
    /**
     * Retrieve the lowest (finest) resolution pyramid level of the map layer passed
     * @param wmslayer the requested layer
     * @returns the layer if successful, null if there is none
     **/
    private RasPersistentScaleLayer getLowestScaleLayer(WmsCapabilities.WmsLayer wmslayer)
    {
        RasPersistentScaleLayer retval = null;
        RasPersistentScaleLayer tempLayer = null;
        Iterator it = wmslayer.getCollections().entrySet().iterator();
        double currentScaleFactor = -1;
        while (it.hasNext())
        {
            tempLayer = (RasPersistentScaleLayer)((Map.Entry)it.next()).getValue();
            
            if (retval == null)
            {	//initialise with any one that fits
                retval = tempLayer;
                currentScaleFactor = retval.getGroundResolutionX();
            }
            else
            {
                /*
                 * we need the smallest ground resolution
                 */
                if (tempLayer.getGroundResolutionX() < currentScaleFactor)
                {
                    retval = tempLayer;
                    currentScaleFactor = retval.getGroundResolutionX();
                }
            }
        }
        if (retval==null)
            log.error( "RequestConverter::getLowestScaleLayer(): error: found layer " + wmslayer.getName() + " having no pyramid, cannot display anything.");
        return retval;
    }
    
    /**
     * Retrieve the highest (coarsest) resolution pyramid level of the map layer passed
     * @param wmslayer the requested layer
     * @returns the layer if successful, null if there is none
     **/
    private RasPersistentScaleLayer getHighestScaleLayer(WmsCapabilities.WmsLayer wmslayer)
    {
        RasPersistentScaleLayer retval = null;
        RasPersistentScaleLayer tempLayer = null;
        Iterator it = wmslayer.getCollections().entrySet().iterator();
        double currentScaleFactor = -1;
        while (it.hasNext())
        {
            tempLayer = (RasPersistentScaleLayer)((Map.Entry)it.next()).getValue();
            
            if (retval == null)
            {	//initialise with any one that fits
                retval = tempLayer;
                currentScaleFactor = retval.getGroundResolutionX();
            }
            else
            {
                /*
                 * we need the coarsest ground resolution
                 */
                if (tempLayer.getGroundResolutionX() > currentScaleFactor)
                {
                    retval = tempLayer;
                    currentScaleFactor = retval.getGroundResolutionX();
                }
            }
        }
        
        if (retval==null)
            log.error( "RequestConverter::getHighestScaleLayer(): error: found layer " + wmslayer.getName() + " having no pyramid, cannot display anything.");
        return retval;
    }
    
    private String convertImageFormat(int imageFormat)
    {
        String format = null;
        switch (imageFormat)
        {
            case WmsRequest.JPEG:
                format = RASQL_JPEG;
                break;
            case WmsRequest.PNG:
                format = RASQL_PNG;
                break;
            case WmsRequest.TIFF:
                format = RASQL_TIFF;
                break;
        }
        return format;
    }
    
    private void buildLayersAndStyles(WmsRequest request) throws WMSException
    {
        Iterator layerstylesit = request.getLayerList().iterator();
        LayerStyle layerstyle = null;
        String layerName = null;
        String styleName = null;
        LayerContainer layerMap = myCapability.getLayers();
        Map styleMap = null;
        
        BoundingBox boundingBox = request.getBoundingBox();
        
        WmsCapabilities.WmsLayer wmslayer = null;
        WmsCapabilities.WmsStyle wmsstyle = null;
        
        wmsLayers = new LinkedList();
        wmsStyles = new LinkedList();
        
        while (layerstylesit.hasNext())
        {
            layerstyle = (LayerStyle)layerstylesit.next();
            layerName = layerstyle.getLayer();
            styleName = layerstyle.getStyle();
            
            wmslayer = (WmsCapabilities.WmsLayer)layerMap.getLayerInfo(layerName);
            if (wmslayer == null)
                throw new WMSException( Globals.ERR_LayerNotDefined, layerName );
            
            String defaultStyle = "";
            
            if (styleName == null)
                styleName = defaultStyle;
            
            wmsLayers.add(wmslayer);
            
            styleMap = wmslayer.getStyles();
            
            // added: check for presence, use empty style ("") for nondeterministic selection
            // NB: null shouldn't occur any more due to the fixes at 2004-feb-10
            if (styleName==null || styleName.equals(""))
            {
                Iterator stylesIterator = styleMap.values().iterator();	// iterator over style map as collection
                if (stylesIterator.hasNext())	// does layer have any styles?
                    wmsstyle = (WmsCapabilities.WmsStyle) stylesIterator.next();	// get one
                else
                    wmsstyle = null;	// to be caught just below
            }
            else
                wmsstyle = (WmsCapabilities.WmsStyle)styleMap.get(styleName);
            
            if (wmsstyle == null)
                throw new WMSException( Globals.ERR_NoSuchStyle, "layer=" + layerName + ", style=" + styleName );
            else
                log.debug( "selected style '" + wmsstyle.getName() + "' for layer '" + layerName + "'" );
            
            /* we now also support "far" zoom out -- PB 2005-06-19
             * if (!wmslayer.getBoundingBox().covers(boundingBox))
             * throw new WMSException( Globals.ERR_LayerOutOfBBox, "layer=" + layerName );
             */
            
            //do for each layer
            if (!request.getSRS().equals(wmslayer.getSRS()))
                throw new WMSException( Globals.ERR_SrsNotSupportedByLayer, "layer=" + wmslayer.getName() + ", SRS=" + request.getSRS() );
            wmsStyles.add(wmsstyle);
        }
    }
    
    
    String getImageFormatName(int requestFormat) throws WMSException
    {
        Iterator formatIter = myCapability.getFormats().iterator();
        
        String formatName = null;
        
        while (formatIter.hasNext())
        {
            if (((Integer)formatIter.next()).intValue() == requestFormat)
            {
                switch (requestFormat)
                {
                    case WmsRequest.JPEG:
                        formatName = WmsRequest.JPEGSTRING;
                        break;
                    case WmsRequest.PNG:
                        formatName = WmsRequest.PNGSTRING;
                        break;
                    case WmsRequest.TIFF:
                        formatName = WmsRequest.TIFFSTRING;
                        break;
                    default:
                        throw new WMSException( Globals.ERR_FormatLost );
                }
            }
        }
        
        if (formatName == null)
        {
            throw new WMSException( Globals.ERR_ImageFormatNotSupported );
        }
        
        return formatName;
    }

} // RequestConverter

