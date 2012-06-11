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
 * The class that generates the xml output for the capabilities.
 * Its inner classes are responsible for creating smaller xml entities (Contact, Layer, Style, Capability).
 * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
 * This version returns version 1.1.0 compliant xml (hopefully).
 * @version $Revision: 1.5 $
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

package petascope.wms;

import java.util.Iterator;
import java.util.List;
import java.util.Map;
import petascope.exceptions.WMSException;

public class WmsCapabilities
{
    /**
     * The Abstract.
     * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
     **/
    protected String myAbstract;
    
    /**
     * The Keywords.
     * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
     **/
    protected List myKeywords;
    
    /**
     * The name of the machine that runs the ogc server.
     * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
     **/
    protected String myHostName;
    
    /**
     * The access constraints.
     * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
     **/
    protected String myAccessConstraints;
    
    /**
     * The update sequence of the capabilities.
     * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
     **/
    protected int myUpdateSequence;
    
    /**
     * The title.
     * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
     **/
    protected String myTitle;
    
    /**
     * The contact information on this wms server.
     * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
     **/
    protected WmsContact myContact;
    
    /**
     * The capability object on this wms server.
     * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
     **/
    protected WmsCapability myCapability;
    
    /**
     * The information on fees for using this service.
     * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
     **/
    protected String myFees;
    
    /**
     * The java representation of the xml contact element.
     **/
    public static class WmsContact
    {
        
        /**
         * The name of the contact person.
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        protected String myContactPersonName;
        
        /**
         * The name of the organization the contact person belongs to.
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        protected String myContactOrganizationName;
        
        /**
         * The position of the contact person in the contact persons organisation.
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        protected String myContactPosition;
        
        /**
         * The type (postal) of adress of the contact persons adress.
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        protected String myContactAddressType;
        
        /**
         * The actual address of the contact person.
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        protected String myContactAddress;
        
        /**
         * The city part of the contact person's address.
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        protected String myContactCity;
        
        /**
         * The state part of the contact person's address.
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        protected String myContactStateOrProvince;
        
        /**
         * The postal code of the contact person's address.
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        protected String myContactPostCode;
        
        /**
         * The country part of the contact person's address.
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        protected String myContactCountry;
        
        /**
         * The phone number of the contact person.
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        protected String myContactContactVoiceTelephone;
        
        /**
         * The fax number of the contact person.
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        protected String myContactContactFacsimileTelephone;
        
        /**
         * The email address of the contact person.
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        protected String myContactContactElectronicMailAddress;
        
        
        /**
         * Create a new contact person object.
         * @param theContactPersonName the Contact Person Name,
         * @param theContactOrganizationName the Contact Organization Name,
         * @param theContactPosition the Contact Position,
         * @param theContactAddressType the Contact Address Type,
         * @param theContactAddress the Contact Address,
         * @param theContactCity the Contact City,
         * @param theContactStateOrProvince the Contact State Or Province,
         * @param theContactPostCode the Contact Post Code,
         * @param theContactCountry the Contact Country,
         * @param theContactContactVoiceTelephone the Contact Voice Telephone,
         * @param theContactContactFacsimileTelephone the Contact Facsimile Telephone,
         * @param theContactContactElectronicMailAddress the Contact Electronic Mail Address,
         * Please also have a look at the <A HREF="http://www.opengis.org/techno/specs/01-047r2.pdf">ogc specification</A>.
         **/
        public WmsContact(String theContactPersonName, String theContactOrganizationName, String theContactPosition, String theContactAddressType, String theContactAddress, String theContactCity, String theContactStateOrProvince, String theContactPostCode, String theContactCountry, String theContactContactVoiceTelephone, String theContactContactFacsimileTelephone, String theContactContactElectronicMailAddress)
        {
            myContactPersonName = theContactPersonName;
            
            myContactOrganizationName = theContactOrganizationName;
            
            myContactPosition = theContactPosition;
            
            myContactAddressType = theContactAddressType;
            
            myContactAddress = theContactAddress;
            
            myContactCity = theContactCity;
            
            myContactStateOrProvince = theContactStateOrProvince;
            
            myContactPostCode = theContactPostCode;
            
            myContactCountry = theContactCountry;
            
            myContactContactVoiceTelephone = theContactContactVoiceTelephone;
            
            myContactContactFacsimileTelephone = theContactContactFacsimileTelephone;
            
            myContactContactElectronicMailAddress = theContactContactElectronicMailAddress;
        }
        
        /**
         * Create the xml representation of a contact element.
         * TODO: define String constants for XML tags.
         **/
        public String toString()
        {
            StringBuffer retval = new StringBuffer(1024);
            retval.append("<ContactInformation>\n");
            //not required
            if ((myContactPersonName != null) && (myContactOrganizationName != null))
            {
                retval.append(" <ContactPersonPrimary>\n");
                retval.append("  <ContactPerson>" + myContactPersonName + "</ContactPerson>\n");
                retval.append("  <ContactOrganization>" + myContactOrganizationName + "</ContactOrganization>\n");
                retval.append(" </ContactPersonPrimary>\n");
            }
            //not required
            if (myContactPosition != null)
                retval.append(" <ContactPosition>" + myContactPosition + "</ContactPosition>\n");
            //not required
            if ((myContactAddressType != null) && (myContactAddress != null) && (myContactCity != null) && (myContactStateOrProvince != null) && (myContactPostCode != null) && (myContactCountry != null))
            {
                retval.append(" <ContactAddress>\n");
                retval.append("  <AddressType>" + myContactAddressType + "</AddressType>\n");
                retval.append("  <Address>" + myContactAddress + "</Address>\n");
                retval.append("  <City>" + myContactCity + "</City>\n");
                retval.append("  <StateOrProvince>" + myContactStateOrProvince + "</StateOrProvince>\n");
                retval.append("  <PostCode>" + myContactPostCode + "</PostCode>\n");
                retval.append("  <Country>" + myContactCountry + "</Country>\n");
                retval.append(" </ContactAddress>\n");
            }
            //not required
            if (myContactContactVoiceTelephone != null)
                retval.append(" <ContactVoiceTelephone>" + myContactContactVoiceTelephone + "</ContactVoiceTelephone>\n");
            //not required
            if (myContactContactFacsimileTelephone != null)
                retval.append(" <ContactFacsimileTelephone>" + myContactContactFacsimileTelephone + "</ContactFacsimileTelephone>\n");
            //not required
            if (myContactContactElectronicMailAddress != null)
                retval.append(" <ContactElectronicMailAddress>" + myContactContactElectronicMailAddress + "</ContactElectronicMailAddress>\n");
            retval.append("</ContactInformation>\n");
            return retval.toString();
        }
    }
    
    /**
     * The java representation of the xml capability element.
     **/
    public static class WmsCapability
    {
        /**
         * The name of the machine that runs the wms server
         **/
        protected String myHostName;
        
        /**
         * The port of the wms server
         **/
        protected int myPort;
        
        /**
         * The path to the wms server
         **/
        protected String myPath;
        
        /**
         * list of supported image formats
         **/
        protected List myFormats;
        
        /**
         * does the server support xml exceptions?
         **/
        protected boolean supportXMLException;
        
        /**
         * does the server support exceptions that are written into an image?
         **/
        protected boolean supportInImageException;
        
        /**
         * does the server support blank images as an exception?
         **/
        protected boolean supportBlankException;
        
        /**
         * complete xml string that describes the vendor specific capabilities.
         **/
        protected String myVendorCapabilities;
        
        /**
         * Map of WmsLayers that are accessible through wms
         **/
        protected LayerContainer myLayers;
        
        /**
         * The title for all other layers that are included
         **/
        protected String myBaseLayerTitle;
        
        /**
         * Create a new java representation of a xml capability element.
         * @param theHostName the Host Name,
         * @param thePort the Port,
         * @param thePath the Path,
         * @param theFormats the Formats,
         * @param supportxmlexception support xml exception,
         * @param supportinimageexception support in image exception,
         * @param supportblankexception support blank exception,
         * @param theVendorCapabilities the Vendor Capabilities,
         * @param theLayers the Layers
         **/
        public WmsCapability(String theHostName, int thePort, String thePath, List theFormats, boolean supportxmlexception, boolean supportinimageexception, boolean supportblankexception, String theVendorCapabilities, LayerContainer theLayers, String theBaseLayerTitle)
        {
            myBaseLayerTitle = theBaseLayerTitle;
            
            myHostName = theHostName;
            
            myPort = thePort;
            
            myPath = thePath;
            
            myFormats = theFormats;
            
            supportXMLException = supportxmlexception;
            
            supportInImageException = supportinimageexception;
            
            supportBlankException = supportblankexception;
            
            myVendorCapabilities = theVendorCapabilities;
            
            myLayers = theLayers;
        }
        
        /**
         * return a reference to the map containing layer name, WmsLayer pairs.
         **/
        public LayerContainer getLayers()
        {
            return myLayers;
        }
        
        /**
         * return a list of strings which are the supported formats.
         **/
        public List getFormats()
        {
            return myFormats;
        }
        
        /**
         * generated the xml representation of this class.
         **/
        public String toString()
        {
            StringBuffer retval = new StringBuffer(7000);
            retval.append("<Capability>\n");
            retval.append(" <Request>\n");
            retval.append("  <GetCapabilities>\n");
            retval.append("   <Format>" + WmsRequest.WMSCAPABILITIES + "</Format>\n");
            retval.append("   <DCPType>\n");
            retval.append("    <HTTP>\n");
            retval.append("     <Get>\n");
            retval.append("      <OnlineResource xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\" xlink:href=\"http://" + myHostName + ":" + myPort + "/" + myPath + "\" />\n");
            retval.append("     </Get>\n");
            retval.append("    </HTTP>\n");
            retval.append("   </DCPType>\n");
            retval.append("  </GetCapabilities>\n");
            retval.append("  <GetMap>\n");
            Iterator it = myFormats.iterator();
            if (it.hasNext())
            {
                while (it.hasNext())
                {
                    retval.append("  <Format>");
                    int theImageType = ((Integer)it.next()).intValue();
                    switch (theImageType)
                    {
                        case WmsRequest.JPEG:
                            retval.append(WmsRequest.JPEGSTRING);
                            break;
                        case WmsRequest.PNG:
                            retval.append(WmsRequest.PNGSTRING);
                            break;
                        case WmsRequest.TIFF:
                            retval.append(WmsRequest.TIFFSTRING);
                            break;
                        default:
                            throw new Error("error in WmsCapabilities: image type " + theImageType + " is unknown");
                    }
                    retval.append("</Format>\n");
                }
            }
            retval.append("   <DCPType>\n");
            retval.append("    <HTTP>\n");
            retval.append("     <Get>\n");
            retval.append("      <OnlineResource xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\" xlink:href=\"http://" + myHostName + ":" + myPort + "/" + myPath + "\" />\n");
            retval.append("     </Get>\n");
            retval.append("    </HTTP>\n");
            retval.append("   </DCPType>\n");
            retval.append("  </GetMap>\n");
            //not required
            /*
             * <GetFeatureInfo>
             * <Format>
             * </Format>
             * <DCPType>
             * </DCPType>
             * </GetFeatureInfo>
             * <DescribeLayer>
             * <Format>
             * </Format>
             * <DCPType>
             * </DCPType>
             * </DescribeLayer>
             */
            retval.append(" </Request>\n");
            retval.append(" <Exception>\n");
            if (supportXMLException)
                retval.append("  <Format>" + WmsRequest.XMLSTRING + "</Format>\n");
            if (supportInImageException)
                retval.append("  <Format>" + WmsRequest.INIMAGESTRING + "</Format>\n");
            if (supportBlankException)
                retval.append("  <Format>" + WmsRequest.BLANKSTRING + "</Format>\n");
            retval.append(" </Exception>\n");
            //not required
            //if (myVendorCapabilities != null)
            //    retval.append(myVendorCapabilities);
            //not required
            /*
             * <UserDefinedSymbolization SupportSLD=\"
             * if (supportSLD)
             * 1
             * else
             * 0
             * \" UserLayer=\"
             * if (userLayer)
             * 1
             * else
             * 0
             * \" UserStyle=\"
             * if (usertStyle)
             * 1
             * else
             * 0
             * \" RemoteWFS=\"
             * if (remoteWFS)
             * 1
             * else
             * 0
             * \" />
             */
            /*
             * retval.append("<Layer>");
             * retval.append(" <Title>" +myBaseLayerTitle + "</Title>\n");
             * retval.append(" <Name>Hakon_Mosby</Name>\n");
             * retval.append(" <SRS>EPSG:25833</SRS>\n");
             * retval.append(" <LatLonBoundingBox minx=\"-0.260834\" miny=\"71.970469\" maxx=\"-0.289398\" maxy=\"71.997398\" />");
             * retval.append(" <BoundingBox SRS=\"EPSG:25833\" minx=\"466000.0\" miny=\"7984100.0\" maxx=\"490700.0\" maxy=\"7990022.0\" />");
             */
            /*
             * it = myLayers.entrySet().iterator();
             * WmsLayer layer = null;
             * //not required
             * while (it.hasNext())
             * {
             * layer = (WmsLayer)((Map.Entry)it.next()).getValue();
             * retval.append(layer);
             * }
             */
            
            WmsLayer layer = null;
            for(int i = 0; i< myLayers.getLayerCounter(); i++)
            {
                layer = myLayers.getLayerInfo(i);
                retval.append(layer);
            }
            
            retval.append("</Capability>\n");
            return retval.toString();
        }
    }
    
    /**
     * The class representing the xml layer element for wms.
     **/
    public static class WmsLayer
    {
        /**
         * The name of this layer
         **/
        protected String myName;
        
        /**
         * the human readable title of this layer
         **/
        protected String myTitle;
        
        /**
         * the coordinate system supported by this layer (theoretical there should be more than one possible...)
         **/
        protected String mySRS;
        
        /**
         * ? look at the specification an wonder...
         **/
        protected String myAuthorityURL;
        
        /**
         * the minimum x value which is covered by this layer.  only for the capabilities.
         **/
        protected double mySphericalMinX;
        
        /**
         * the minimum y value which is covered by this layer.  only for the capabilities.
         **/
        protected double mySphericalMinY;
        
        /**
         * the maximum x value which is covered by this layer.  only for the capabilities.
         **/
        protected double mySphericalMaxX;
        
        /**
         * the maximum y value which is covered by this layer.  only for the capabilities.
         **/
        protected double mySphericalMaxY;
        
        /**
         * the minimum x value which is covered by this layer.  superceded by myBox.
         **/
        protected double myMinX;
        
        /**
         * the minimum y value which is covered by this layer.  superceded by myBox.
         **/
        protected double myMinY;
        
        /**
         * the maximum x value which is covered by this layer.  superceded by myBox.
         **/
        protected double myMaxX;
        
        /**
         * the maximum y value which is covered by this layer.  superceded by myBox.
         **/
        protected double myMaxY;
        
        /**
         * further information on this layer can be found at this url.
         **/
        protected String myAttributionURL;
        
        /**
         * this layer's further information should get this as the title
         **/
        protected String myAttributionTitle;
        
        /**
         * you can also supply a logo here.
         **/
        protected int myLogoWidth;
        
        /**
         * you can also supply a logo here.
         **/
        protected int myLogoHeight;
        
        /**
         * you can also supply a logo here.
         **/
        protected String myLogoFormat;
        
        /**
         * you can also supply a logo here.
         **/
        protected String myLogoURL;
        
        /**
         * further information on the features of this layer
         **/
        protected String myFeatureURL;
        
        /**
         * map containing style name, WmsStyle pairs
         **/
        protected Map myStyles;
        
        /**
         * map containing persistent collection name, RasPersistentScaleLayer pairs
         **/
        protected Map myPersistentCollections;
        
        /**
         * should supercede the minX, minY, maxX, maxY attributes.
         **/
        protected BoundingBox myBox;
        
        /**
         * Create a new java object which represents the xml layer element.
         * @param theName the Name,
         * @param theTitle the Title,
         * @param theSRS theSRS,
         * @param theAuthorityURL the Authority URL,
         * @param theMinX the Min X,
         * @param theMinY the Min Y,
         * @param theMaxX the Max X,
         * @param theMaxY the Max Y,
         * @param theAttributionURL the Attribution URL,
         * @param theAttributionTitle the Attribution Title,
         * @param theLogoWidth the Logo Width,
         * @param theLogoHeight the Logo Height,
         * @param theLogoFormat the Logo Format,
         * @param theLogoURL the Logo URL,
         * @param theFeatureURL the Feature URL,
         * @param theStyles the Styles,
         * @param thePersistentCollections the Persistent Collections,
         * @param theSphericalMinX min x in spherical coordinates,
         * @param theSphericalMinY min y in spherical coordinates,
         * @param theSphericalMaxX max x in spherical coordinates,
         * @param theSphericalMaxY max y in spherical coordinates
         **/
        public WmsLayer(String theName, String theTitle, String theSRS, String theAuthorityURL, double theMinX, double theMinY, double theMaxX, double theMaxY, String theAttributionURL, String theAttributionTitle, int theLogoWidth, int theLogoHeight, String theLogoFormat, String theLogoURL, String theFeatureURL, Map theStyles, Map thePersistentCollections, double theSphericalMinX, double theSphericalMinY, double theSphericalMaxX, double theSphericalMaxY) throws WMSException
        {
            myPersistentCollections = thePersistentCollections;
            
            myName = theName;
            
            myTitle = theTitle;
            
            mySRS = theSRS;
            
            myAuthorityURL = theAuthorityURL;
            
            mySphericalMinX = theSphericalMinX;
            
            mySphericalMinY = theSphericalMinY;
            
            mySphericalMaxX = theSphericalMaxX;
            
            mySphericalMaxY = theSphericalMaxY;
            
            myMinX = theMinX;
            
            myMinY = theMinY;
            
            myMaxX = theMaxX;
            
            myMaxY = theMaxY;
            
            myBox = new BoundingBox(theMinX, theMinY, theMaxX, theMaxY);
            
            myAttributionURL = theAttributionURL;
            
            myAttributionTitle = theAttributionTitle;
            
            myLogoWidth = theLogoWidth;
            
            myLogoHeight = theLogoHeight;
            
            myLogoFormat = theLogoFormat;
            
            myLogoURL = theLogoURL;
            
            myFeatureURL = theFeatureURL;
            
            myStyles = theStyles;
        }
        
        /**
         * Return a reference to the bounding box of this layer
         **/
        public BoundingBox getBoundingBox()
        {
            return myBox;
        }
        
        /**
         * Return a reference to the map of styles of this layer
         **/
        public Map getStyles()
        {
            return myStyles;
        }
        
        /**
         * Return a reference to the name of this layer
         **/
        public String getName()
        {
            return myName;
        }
        
        /**
         * This might be neccessary for the hashmap to work correctly (the hash code of the name is used).
         **/
        public int hashCode()
        {
            return myName.hashCode();
        }
        
        /**
         * Return a reference to the map of persistent collection names to RasPersistentScaleLayer objects.
         **/
        public Map getCollections()
        {
            return myPersistentCollections;
        }
        
        /**
         * Return a reference to the coordinate system name of this layer
         **/
        public String getSRS()
        {
            return mySRS;
        }
        
        /**
         * Generate xml output on this layer
         **/
        public String toString()
        {
            StringBuffer retval = new StringBuffer(512);
            retval.append("<Layer");
            /*
             * if (isQueryable)
             * retval.append(" queryable=\"1\"");
             * if (isOpaque)
             * retval.append(" opaque=\"1\"");
             * if (hasNoSubsets)
             * retval.append(" noSubsets=\"1\"");
             * if (fixedWidth != 0)
             * retval.append(" fixedWidth=\"" + fixedWidth + "\"");
             * if (fixedHeight != 0)
             * retval.append(" fixedHeight=\"" + fixedHeight + "\"");
             */
            retval.append(">\n");
            if (myName != null)
                retval.append(" <Name>" +myName + "</Name>\n");
            retval.append(" <Title>" +myTitle + "</Title>\n");
            /*
             * if (myAbstract != null)
             * retval.append(" <Abstract>" + myAbstract + "</Abstract>");
             * if (myKeywords)
             * {
             * Iterator it = myKeywords.iterator();
             * //not required
             * if (it.hasNext())
             * {
             * retval.append("  <KeywordList>\n");
             * while (it.hasNext())
             * retval.append("   <Keyword>" + it.next() + "</Keyword>\n");
             * retval.append("  </KeywordList>\n");
             * }
             * }
             */
            if (mySRS != null)
                retval.append(" <SRS>" +mySRS + "</SRS>\n");
            //if ((myMinX != 0) && (myMinY != 0) && (myMaxX != 0) && (myMaxY != 0)) not correct, some of this values can be really '0'!!
            retval.append(" <LatLonBoundingBox minx=\"" + mySphericalMinX + "\" miny=\"" + mySphericalMinY + "\" maxx=\"" + mySphericalMaxX + "\" maxy=\"" +mySphericalMaxY + "\" />\n");
            retval.append(" <BoundingBox SRS=\"" + mySRS + "\" minx=\"" + myMinX + "\" miny=\"" + myMinY + "\" maxx=\"" + myMaxX + "\" maxy=\"" +myMaxY + "\" />\n");
            /*
             * if (myBBoxes != null)
             * {
             * Iterator it = myBBoxes.iterator();
             * //not required
             * BoundingBox b = null;
             * if (it.hasNext())
             * {
             * while (it.hasNext())
             * {
             * b = (BoundingBox)it.next();
             * retval.append("   " + it.next().toString() + "\n");
             * }
             * }
             * }
             * if (myDimensions != null)
             * {
             * Iterator it = myDimensions.iterator();
             * //not required
             * WmsDimension b = null;
             * if (it.hasNext())
             * {
             * while (it.hasNext())
             * {
             * b = (WmsDimension)it.next();
             * retval.append("   <Dimension name=\"" + b.name + "\" units=\"" + b.units + "\" unitSymbol=\"" + b.unitSymbol + "\" />\n");
             * }
             * }
             * }
             * if (myExtents != null)
             * {
             * Iterator it = myExtents.iterator();
             * //not required
             * WmsExtent b = null;
             * if (it.hasNext())
             * {
             * while (it.hasNext())
             * {
             * b = (WmsExtent)it.next();
             * retval.append("   <Extent name=\"" + b.name + "\" default=\"" + b.default + "\" >" + b.values + "</Extent>\n");
             * }
             * }
             * }
             */
            //not required
            //provider of a layer
            if ((myAttributionURL != null) || (myAttributionTitle != null) || ((myLogoURL != null) && (myLogoFormat != null)))
            {
                retval.append(" <Attribution>\n");
                if (myAttributionTitle != null)
                    retval.append("  <Title>" + myAttributionTitle + "</Title>\n");
                if (myAttributionURL != null)
                    retval.append("  <OnlineResource xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\" xlink:href=\"" + myAttributionURL + "\" />\n");
                if (myLogoWidth != 0 && myLogoHeight != 0 && myLogoFormat != null && myLogoURL != null)
                {
                    retval.append("  <LogoURL width=\"" + myLogoWidth + "\" height=\"" + myLogoHeight + "\">\n");
                    retval.append("   <Format>" + myLogoFormat + "</Format>\n");
                    retval.append("   <OnlineResource xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\" xlink:href=\"" + myLogoURL + "\" />\n");
                    retval.append("  </LogoURL>\n");
                }
                retval.append(" </Attribution>\n");
                
            }
            //not required.  there may be many of them
            if (myAuthorityURL != null)
            {
                retval.append("  <AuthorityURL name=\"DIF_ID\">\n");
                //http://gcmd.gsfc.nasa.gov/difguide/whatisadif.html
                retval.append("   <OnlineResource xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\" xlink:href=\"" + myAuthorityURL + "\" />\n");
                retval.append("  </AuthorityURL>\n");
            }
            /*
             * //not required.  there may be many of them
             * if ((myIdentifier != null) && (myIdentifierAuthorityName != null))
             * {
             * retval.append("  <Identifier authority=\"" + myIdentifierAuthorityName+ "\">" + myIdentifier + "  </Identifier>\n");
             * }
             * //not required.  there may be many of them
             * if ((myMetadataURL != null) && (myMetadataFormat != null) && (myMetadataType != null))
             * {
             * retval.append("  <MetadataURL type=\"" + myMetadataType + " \">\n");
             * retval.append("   <Format>" + myMetadataFormat + "</Format>\n");
             * retval.append("   <OnlineResource xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\" xlink:href=\"" + myMetadataURL + "\" />\n");
             * retval.append("  </MetadataURL>\n");
             * }
             * if ((myDataURL != null) && (myDataFormat != null))
             * {
             * retval.append("  <DataURL>\n");
             * retval.append("   <Format>" + myDataFormat + "</Format>\n");
             * retval.append("   <OnlineResource xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\" xlink:href=\"" + myDataURL + "\" />\n");
             * retval.append("  </DataURL>\n");
             * }
             */
            //there may be many of them
            if (myFeatureURL != null)
            {
                retval.append(" <FeatureListURL>\n");
                retval.append("  <Format>" + WmsRequest.XMLSTRING + "</Format>\n");
                retval.append("  <OnlineResource xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\" xlink:href=\"" + myFeatureURL + "\" />\n");
                retval.append(" </FeatureListURL>\n");
            }
            Iterator it = myStyles.entrySet().iterator();
            WmsStyle style = null;
            if (it.hasNext())
                while (it.hasNext())
                {
                    style = (WmsStyle)((Map.Entry)it.next()).getValue();
                    retval.append(style);
                }
            /*
             * if ((myScaleHintMin != 0) && (myScaleHintMax != 0))
             * retval.append("<ScaleHint min=\"" + myScaleHintMin + "\" max=\"" + myScaleHintMax + "\" />");
             * //sublayers
             */
            retval.append("</Layer>\n");
            return retval.toString();
        }
    }
    
    /**
     * Class representing the xml style element of the wms capabilities.
     * This can be used to hold RASQL queries the are integrated into GetMap requests in the servlet.
     **/
    public static class WmsStyle
    {
        /**
         * The name of this style
         **/
        protected String myName;
        
        /**
         * The human readable name of this style
         **/
        protected String myTitle;
        
        /**
         * Short information on what this style does
         **/
        protected String myAbstract;
        
        /**
         * Kind of logo information
         **/
        protected int myLegendWidth;
        
        /**
         * Kind of logo information
         **/
        protected int myLegendHeight;
        
        /**
         * Kind of logo information
         **/
        protected String myLegendURL;
        
        /**
         * Please look into the specification ( i have no idea )
         **/
        protected String myStyleSheetURL;
        
        /**
         * The RasQL operation.  In this String all occurnces of STYLENAME will be replaced by the scaled trans mdd.
         **/
        protected String myRasQL;
        
        /**
         * Create a new style object.
         * @param theName the Name,
         * @param theTitle the Title,
         * @param theAbstract the Abstract,
         * @param theLegendWidth the Legend Width,
         * @param theLegendHeight the Legend Height,
         * @param theLegendURL the Legend URL,
         * @param theStyleSheetURL the Style Sheet URL
         * @param theRasQL the RasQLQuery
         **/
        public WmsStyle(String theName, String theTitle, String theAbstract, int theLegendWidth, int theLegendHeight, String theLegendURL, String theStyleSheetURL, String theRasQL)
        {
            myName = theName;
            
            myTitle = theTitle;
            
            myAbstract = theAbstract;
            
            myLegendWidth = theLegendWidth;
            
            myLegendHeight = theLegendHeight;
            
            myLegendURL = theLegendURL;
            
            myStyleSheetURL = theStyleSheetURL;
            
            myRasQL = theRasQL;
        }
        
        /**
         * Return the RasQL Query.
         **/
        public String getRasQL()
        {
            return myRasQL;
        }
        
        /**
         * Return the Name.
         **/
        public String getName()
        {
            return myName;
        }
        
        /**
         * Generate xml output for this entity
         **/
        public String toString()
        {
            StringBuffer retval = new StringBuffer(256);
            retval.append("<Style>\n");
            retval.append(" <Name>" + myName + "</Name>\n");
            retval.append(" <Title>" + myTitle + "</Title>\n");
            if (myAbstract != null)
                retval.append(" <Abstract>" + myAbstract + "</Abstract>\n");
            //not required.  there may be many
            if (myLegendURL != null && myLegendWidth != 0 && myLegendHeight != 0)
            {
                retval.append("  <LegendURL width=\"" + myLegendWidth + "\" height=\"" + myLegendHeight + "\">\n");
                retval.append("   <OnlineResource xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\" xlink:href=\"" + myLegendURL + "\" />\n");
                retval.append("  </LegendURL>\n");
            }
            //not required.
            if (myStyleSheetURL != null)
            {
                retval.append("  <StyleSheetURL>\n");
                //this should be up to the user
                retval.append("   <Format>text/xsl</Format>\n");
                retval.append("   <OnlineResource xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\" xlink:href=\"" + myStyleSheetURL + "\" />\n");
                retval.append("  </StyleSheetURL>\n");
            }
            /*
             * //not required.
             * if ((myStyleURL != null) && (myStyleURLFormat != null))
             * {
             * retval.append("  <StyleSheetURL>\n");
             * retval.append("   <Format>" + myStyleURLFormat + "</Format>\n");
             * retval.append("   <OnlineResource xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\" xlink:href=\"" + myStyleURL + "\" />\n");
             * retval.append("  </StyleSheetURL>\n");
             * }
             */
            retval.append("</Style>\n");
            return retval.toString();
        }
    }
    
    /**
     * Create a new instance of the capabilities which represent the complete meta data of the wms server.
     * 
     * @param theUpdateSequence the Update Sequence,
     * @param theTitle the Title,
     * @param theAbstract the Abstract,
     * @param theKeywords the Keywords,
     * @param theHostName the Host Name,
     * @param theContact the Contact,
     * @param theFees the Fees,
     * @param theAccessConstraints the Access Constraints,
     * @param theCapability the Capability
     **/
    public WmsCapabilities(int theUpdateSequence, String theTitle, String theAbstract, List theKeywords, String theHostName, WmsContact theContact, String theFees, String theAccessConstraints, WmsCapability theCapability)
    {
        myContact = theContact;
        
        myCapability = theCapability;
        
        myUpdateSequence = theUpdateSequence;
        
        myTitle = theTitle;
        
        myAbstract = theAbstract;
        
        myKeywords = theKeywords;
        
        myHostName = theHostName;
        
        myFees = theFees;
        
        myAccessConstraints = theAccessConstraints;
        
        myCapability = theCapability;
    }
    
    /**
     * Return a reference to the capability entity
     **/
    public WmsCapability getCapability()
    {
        return myCapability;
    }
    
    /**
     * Get the update sequence of this capabilities
     **/
    public int getUpdateSequence()
    {
        return myUpdateSequence;
    }
    
    /**
     * Generate the complete xml output conforming to the capabilities dtd for a wms server version 1.1.0
     **/
    public String toString()
    {
        StringBuffer retval = new StringBuffer(8192);
        retval.append("<?xml version='1.0' encoding=\"UTF-8\" standalone=\"no\" ?>\n");
        retval.append("<!DOCTYPE WMT_MS_Capabilities SYSTEM \"http://schemas.opengis.net/wms/1.1.0/capabilities_1_1_0.dtd\">\n");
        retval.append("<WMT_MS_Capabilities version=\"1.1.0\" updateSequence=\"" + myUpdateSequence + "\">\n\n");
        retval.append(" <Service>\n");
        retval.append("  <Name>WMS:WMS</Name>\n");	// changed from "GetMap" to be conformant with 1.1.0 -- PB 2005-may-05
        retval.append("  <Title>" + myTitle + "</Title>\n");
        //not required
        if (myAbstract != null)
            retval.append("  <Abstract>" + myAbstract + "</Abstract>\n");
        //not required
        if (myKeywords != null)
        {
            Iterator it = myKeywords.iterator();
            if (it.hasNext())
            {
                retval.append("  <KeywordList>\n");
                while (it.hasNext())
                    retval.append("   <Keyword>" + it.next() + "</Keyword>\n");
                retval.append("  </KeywordList>\n");
            }
        }
        retval.append("  <OnlineResource xmlns:xlink=\"http://www.w3.org/1999/xlink\" xlink:type=\"simple\" xlink:href=\"http://" + myHostName + "\" />\n\n");
        //not required
        if (myContact != null)
        {
            retval.append(myContact);
            retval.append("\n\n");
        }
        //not required
        if (myFees != null)
            retval.append("  <Fees>" + myFees + "</Fees>\n");
        //not required
        if (myAccessConstraints != null)
            retval.append("  <AccessConstraints>" + myAccessConstraints + "</AccessConstraints>\n");
        retval.append(" </Service>\n\n");
        retval.append(myCapability);
        retval.append("</WMT_MS_Capabilities>");
        return retval.toString();
    }
}
