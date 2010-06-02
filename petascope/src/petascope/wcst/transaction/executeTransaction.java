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
 * Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
 rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.wcst.transaction;

//~--- non-JDK imports --------------------------------------------------------
import java.sql.SQLException;
import net.opengis.ows.v_1_0_0.BoundingBoxType;
import net.opengis.wcs.ows.v_1_1_0.InterpolationMethodType;
import net.opengis.wcs.v_1_1_0.CoverageDescriptionType;
import net.opengis.wcs.v_1_1_0.CoverageDescriptions;
import net.opengis.wcs.v_1_1_0.CoverageSummaryType;
import net.opengis.wcs.v_1_1_0.FieldType;
import net.opengis.wcs.v_1_1_0.RangeType;

import org.apache.commons.io.IOUtils;

import petascope.wcps.server.core.SDU;

//~--- JDK imports ------------------------------------------------------------

import java.awt.image.BufferedImage;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StringReader;

import java.math.BigInteger;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Random;
import java.util.Set;
import java.util.UUID;

import javax.imageio.ImageIO;


import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.Unmarshaller;
import net.opengis.wcs.ows.v_1_1_0.InterpolationMethods;
import org.odmg.ODMGException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.wcps.server.core.DbMetadataSource;
import petascope.wcps.server.core.Metadata;
import petascope.wcs.server.exceptions.WCSException;
import petascope.ConfigManager;
import petascope.wcps.server.core.CellDomainElement;
import petascope.wcps.server.core.DomainElement;
import petascope.wcps.server.core.InterpolationMethod;
import petascope.wcps.server.exceptions.InvalidMetadataException;
import petascope.wcps.server.exceptions.InvalidWcpsRequestException;
import petascope.wcps.server.core.RangeElement;
import petascope.wcps.server.exceptions.ResourceException;
import petascope.wcs.server.exceptions.ActionNotSupportedException;
import petascope.wcs.server.exceptions.InputOutputException;
import petascope.wcs.server.exceptions.InternalComponentException;
import petascope.wcs.server.exceptions.InternalSqlException;
import petascope.wcs.server.exceptions.InvalidParameterValueException;
import petascope.wcs.server.exceptions.InvalidRequestException;
import petascope.wcs.server.exceptions.MissingParameterValueException;
import petascope.wcs.server.exceptions.MultiBandImagesNotSupportedException;
import petascope.wcs.server.exceptions.NoApplicableCodeException;
import petascope.wcs.server.exceptions.NodeParsingNotImplementedException;
import petascope.wcs.server.exceptions.XmlNotValidException;
import petascope.wcs.server.exceptions.XmlStructuresException;
import wcst.transaction.schema.CodeType;
import wcst.transaction.schema.CoverageType;
import wcst.transaction.schema.KeywordsType;
import wcst.transaction.schema.LanguageStringType;
import wcst.transaction.schema.ManifestType;
import wcst.transaction.schema.ReferenceType;
import wcst.transaction.schema.TransactionResponseType;
import wcst.transaction.schema.TransactionType;
import petascope.wcst.transaction.tools.RasdamanUtils;

/**
 * This class takes a WCS-T Transaction XML request and executes the request,
 * building the corresponding XML respose. Use this class for synchronous processing.
 *
 * @author Andrei Aiordachioaie
 */
public class executeTransaction {

    private static Logger LOG = LoggerFactory.getLogger(executeTransaction.class);
//	private static boolean printLog = true;
    private boolean finished;
    private TransactionType input;
    protected TransactionResponseType output;
    private RasdamanUtils rasUtils;
    private String requestId;
    private DbMetadataSource metaDb;
    private HashSet<String> newCoverages;

    /**
     * Default constructor. Initialize internal variables.
     * @param tr Transaction object, a WCS-T request
     * @param metadataDbPath Path to the "dbparams.properties" file
     */
    public executeTransaction(TransactionType tr, DbMetadataSource source) throws WCSException {
        input = tr;
        output = new TransactionResponseType();
        finished = false;
        metaDb = source;
        newCoverages = new HashSet<String>();

        String server = ConfigManager.RASDAMAN_URL;
        String db = ConfigManager.RASDAMAN_DATABASE;

        rasUtils = new RasdamanUtils(server, db);
        rasUtils.init();

        // In case no-one will ever call this method and we need it
        generateRequestId();
    }

    /**
     * Generate a new Request ID string, and return it. If the transaction request
     * does not include a request ID, the last generated string will be used.
     * @return String Request ID
     */
    public String generateRequestId() {
        requestId = String.valueOf(UUID.randomUUID());
        return requestId;
    }

    /**
     * Main method of this class: Computes the response to the TransactionResponse
     * request given to the constructor. If needed, it also calls <b>process()</b>
     * @return a TransactionResponse object.
     * @throws WCSException
     */
    public TransactionResponseType get() throws WCSException {
        try {
            if (finished == false) {
                metaDb.ensureConnection();
                process();
            }
        } catch (SQLException e) {
            throw new InternalSqlException("Could not ensure connection to database is valid", e);
        }
        if (finished == false) {
            throw new NoApplicableCodeException("Could not execute the Transaction request! "
                    + "Please see the other errors...");
        }

        return output;
    }

    /**
     * Computes the response to the Transaction request given to the constructor.
     */
    public void process() throws WCSException {
        if (!input.getService().equalsIgnoreCase("WCS")) {
            throw new InvalidParameterValueException("Service. Explanation: Service must be \"WCS\" !");
        }
        if (!input.getVersion().equalsIgnoreCase("1.1")) {
            throw new InvalidParameterValueException("Service. Explanation: Service Version must be \"1.1\" !");
        }

        // Set the output request ID
        String reqID = input.getRequestId();
        if (reqID == null) {
            reqID = "Request_" + requestId;
            generateRequestId();
        }
        output.setRequestId(reqID);

        // All actions succeed or fail as one group.
        try {
            ManifestType covs = input.getInputCoverages();
            List l = covs.getReferenceGroup();

            for (int i = 0; i < l.size(); i++) {
                // This object is the XML element "InputCoverages"
                Object obj = ((JAXBElement) l.get(i)).getValue();
                CoverageType cov = (CoverageType) obj;

                // Each action adds something to the output XML document
                processInputCoverageNode(cov);
            }

            finished = true;

            /*  Commit rasdaman changes */
            try {
                LOG.debug("Commit rasdaman changes ...");
                rasUtils.commitAndClose();
                LOG.debug("Rasdaman coverages saved successfully !");
            } catch (ODMGException e) {
                throw new InternalComponentException("Could not commit Rasdaman changes !", e);
            }

            /* Commit metadata changes */
            try {
                LOG.debug("Commit metadata changes ...");
                metaDb.commitAndClose();
                LOG.debug("Metadata has been saved !");
            } catch (SQLException e) {
                throw new InternalSqlException("Could not commit metadata changes", e);
            }
        } catch (WCSException e) {
            // One action failed, therefore all actions have failed

            /* Abort metadata changes */
            finished = false;
            try {
                LOG.debug("Rolling back metadata database changes ...");
                metaDb.abortAndClose();
                LOG.debug("Metadata rollback completed!");
            } catch (SQLException ex) {
                LOG.error("Could not rollback metadata changes.");
                e.appendErrorDetail(" Could not rollback metadata changes!");
            }

            /* Abort rasdaman changes */
            try {
                LOG.debug("Aborting rasdaman changes ...");
                rasUtils.abortAndClose();
                LOG.debug("Rasdaman changes aborted !");
            } catch (ODMGException ex) {
                LOG.error("Could not abort rasdaman changes.");
                e.appendErrorDetail(" Could not rollback rasdaman changes!");
            }

            throw e;
        }
    }

    /**
     * Delete a coverage from the Rasdaman server.
     * @param identifier Name of coverage
     * @throws Exception
     */
    private void deleteCoverageFromRasdaman(String identifier) throws Exception {
        try {
            LOG.trace("Deleting coverage from Rasdaman ...");
            rasUtils.deleteCollection(identifier);
            LOG.trace("Rasdaman Collection '" + identifier + "' is now deleted !");
        } catch (ODMGException e) {
            LOG.error("Failed to delete rasdaman collection " + identifier);
            throw new InternalComponentException("Failed to delete collection from Rasdaman !", e);
        }
    }

    /**
     * Insert pixel data for a coverage into RasDaMan DB system
     *
     * @param identifier Identifier of the coverage
     * @param href The location of the pixels for the new image
     */
    private void insertImageIntoRasdaman(String identifier, BufferedImage img)
            throws WCSException {
        LOG.trace("Inserting image into Rasdaman raster server...");
        try {
            rasUtils.insertGrayImageAsArray(identifier, img);
            LOG.debug("Inserted image into Rasdaman !");
        } catch (ODMGException e) {
            LOG.error("Could not insert image into Rasdaman !");
            throw new InternalComponentException("Could not insert image into Rasdaman.", e);
        }
    }

    /** Load a BufferedImage from a ReferenceType object.
     *
     * @param pixels Reference object
     * @return available image
     * @throws WCSException
     */
    private BufferedImage loadPixelsReference(ReferenceType pixels) throws WCSException {
        URL url = null;
        BufferedImage img = null;

        try {
            url = new URL(pixels.getHref());
        } catch (MalformedURLException e) {
            LOG.error("URL " + url.toString() + " is not valid.");
            throw new InvalidParameterValueException("Reference pixels. "
                    + "Explanation: URL " + url.toString() + " is not valid.");
        }

        try {
            img = ImageIO.read(url);
            if (img == null) {
                throw new IOException("Empty stream while reading image.");
            }
        } catch (IOException e) {
            LOG.error(e.getMessage());
            throw new InputOutputException("Reference Pixels. Explanation: "
                    + "Could not read image from URL '" + url, e);
        }

        return img;
    }

    /**
     * Load a Coverage Description XML object from a Reference
     *
     * @param identifier Name of coverage
     * @param desc Reference to a CoverageDescriptions xml
     * @return coverage description
     * @throws WCSException
     */
    private CoverageDescriptionType loadDescriptionReference(String identifier,
            ReferenceType desc) throws WCSException {
        URL url = null;
        String xmlString = null;
        CoverageDescriptions descs = null;
        CoverageDescriptionType desc0 = null;

        // Load the URL
        try {
            url = new URL(desc.getHref());
        } catch (MalformedURLException e) {
            LOG.error("URL " + url.toString() + " is not valid !");
            throw new InvalidParameterValueException("Reference pixels. Explanation: "
                    + "URL " + url.toString() + " is not valid.");
        }

        // Read the contents of the URL
        try {
            URLConnection conn = url.openConnection();
            BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));

            xmlString = IOUtils.toString(in);
        } catch (IOException ex) {
            throw new InputOutputException("Description Reference. Explanation: error loading the "
                    + "coverage description from URL " + url.toString(), ex);
        }

        // Unmarshall the XML string into a Java Object
        try {
            JAXBContext jaxbCtx = JAXBContext.newInstance(CoverageDescriptions.class.getPackage().getName());
            Unmarshaller unmarshaller = jaxbCtx.createUnmarshaller();
            Object obj = unmarshaller.unmarshal(new StringReader(xmlString));

            if (obj instanceof JAXBElement) {
                descs = (CoverageDescriptions) ((JAXBElement) obj).getValue();
            } else if (obj instanceof CoverageDescriptions) {
                descs = (CoverageDescriptions) obj;
            } else {
                LOG.error("Coverage description metadata is not a valid xml document.");
                throw new XmlNotValidException("Coverage "
                        + "description metadata is not a valid xml document.");
            }

        } catch (javax.xml.bind.JAXBException ex) {
            throw new XmlStructuresException("Could not marshall/unmarshall XML structures.", ex);
        }

        // Filter by coverage name
        desc0 = null;
        Iterator<CoverageDescriptionType> i = descs.getCoverageDescription().iterator();

        while (i.hasNext()) {
            CoverageDescriptionType d = i.next();

            if (d.getIdentifier().equals(identifier)) {
                desc0 = d;
                break;
            }
        }

        if (desc0 == null) {
            throw new InvalidRequestException("Could not find a CoverageDescription for coverage: " + identifier);
        }

        return desc0;
    }

    private CoverageSummaryType loadSummaryReference(ReferenceType pixels) throws WCSException {
        URL url = null;
        String xmlString = null;
        CoverageSummaryType xml = null;

        try {
            url = new URL(pixels.getHref());
        } catch (MalformedURLException e) {
            throw new InvalidParameterValueException("Reference summary. Explanation: URL is not valid.", e);
        }

        // Read the contents of the URL
        try {
            URLConnection conn = url.openConnection();
            BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));

            xmlString = IOUtils.toString(in);
        } catch (IOException ex) {
            throw new InputOutputException("Summary Reference. Explanation: "
                    + "Error loading the " + "coverage summary from URL " + url.toString(), ex);
        }

        // Unmarshall the XML string into a Java Object
        try {
            JAXBContext jaxbCtx = JAXBContext.newInstance(CoverageSummaryType.class.getPackage().getName());
            Unmarshaller unmarshaller = jaxbCtx.createUnmarshaller();
            Object obj = unmarshaller.unmarshal(new StringReader(xmlString));

            if (obj instanceof JAXBElement) {
                xml = (CoverageSummaryType) ((JAXBElement) obj).getValue();
            } else if (obj instanceof CoverageSummaryType) {
                xml = (CoverageSummaryType) obj;
            } else {
                throw new XmlNotValidException("Coverage Summary metadata is not a valid xml document.");
            }
        } catch (javax.xml.bind.JAXBException ex) {
            throw new XmlStructuresException("Could not marshall/unmarshall XML structures.", ex);
        }

        return xml;
    }

    /**
     * Updates the coverage metadata: textual descriptions. The title and the abstract
     * could be specified in multiple languages, but we only store english.
     *
     * @param meta Metadata object to be modified
     * @param summary summary object, that contains title, abstract and coverage keywords
     * @return modified metadata object
     */
    private Metadata updateMetadataWithSummary(Metadata meta,
            CoverageSummaryType summary) throws WCSException {
        LOG.debug("Updating metadata with values from Coverage Summary...");

        String title = null, abstr = null, keywords = null;

        title = summary.getTitle();
        abstr = summary.getAbstract();
        List<net.opengis.ows.v_1_0_0.KeywordsType> keywordL = summary.getKeywords();
        List<String> kList = new ArrayList<String>();
        Iterator i = keywordL.iterator();

        while (i.hasNext()) {
            KeywordsType keywordInMultipleLangs = (KeywordsType) i.next();
            String keyword = filterAcceptedLanguage(keywordInMultipleLangs.getKeyword());

            kList.add(keyword);
        }
        keywords = SDU.string2str(kList);

        meta.setTitle(title);
        meta.setKeywords(keywords);
        meta.setAbstract(abstr);

        return meta.clone();
    }

    /**
     * Retrieve only the string in the accepted language from a multiple-language list
     *
     * @param list List of strings in several languages
     * @return String in the accepted language, or null if none is found
     */
    private String filterAcceptedLanguage(List<LanguageStringType> list) {
        String result = null;
        Iterator i = list.iterator();

        while (i.hasNext()) {
            LanguageStringType a = (LanguageStringType) i.next();

            if (a.getLang().equals(ConfigManager.WCST_LANGUAGE)) {
                result = a.getValue();
            }
        }

        return result;
    }

    /**
     * Partially updates a Rasdaman coverage with pixels from pixHref. Info about
     * the bounding box is available from descHref.
     *
     * @param identifier ID of the coverage to be updates
     * @param pixHref URL for the pixels data
     * @param descHref URL for the metadata
     */
    private void insertSomePixelsIntoRasdaman(String identifier, String pixHref, String descHref) {
        // TODO: Implement !
        LOG.error("Partial update is not yet implemented");
        throw new UnsupportedOperationException("Partial Rasdaman update is not yet implemented.");
    }

    /**
     * Inserts default metadata values for the given coverage.
     *
     * @param identifier ID of the coverage
     * @param img The image, fetched from external reference
     * @throws WCSException on error
     */
    private Metadata createNewCoverageMetadata(String identifier, BufferedImage img)
            throws InvalidMetadataException {
        Metadata m = null;
        LOG.debug("Creating metadata with default values...");

        // TODO: When we accept multi-band images, update nullDefault
        String nullDefault = "0";

        // Cell domains
        BigInteger lowX = new BigInteger("0");
        BigInteger highX = new BigInteger(String.valueOf(img.getHeight() - 1));
        BigInteger lowY = new BigInteger("0");
        BigInteger highY = new BigInteger(String.valueOf(img.getWidth() - 1));
        CellDomainElement cellX = new CellDomainElement(lowX, highX);
        CellDomainElement cellY = new CellDomainElement(lowY, highY);
        List<CellDomainElement> cellList = new ArrayList<CellDomainElement>(2);
        cellList.add(cellX);
        cellList.add(cellY);

        // Domains
        Set<String> crsSet = new HashSet<String>(1);
        crsSet.add(DomainElement.IMAGE_CRS);
        String str1 = null, str2 = null;
        /* Since we currently do not use the Domain sizes, we can set them to 0 and 1 */
        DomainElement domX = new DomainElement("x", "x", 0.0, 1.0, str1, str2, crsSet, metaDb.getAxisNames());
        DomainElement domY = new DomainElement("y", "y", 0.0, 1.0, str1, str2, crsSet, metaDb.getAxisNames());
        List<DomainElement> domList = new ArrayList<DomainElement>(2);
        domList.add(domX);
        domList.add(domY);

        // Ranges
            /* TODO: When multiple-field images are supported, update ranges */
        RangeElement range = new RangeElement("intensity", ConfigManager.WCST_DEFAULT_DATATYPE);
        List<RangeElement> rList = new ArrayList<RangeElement>(1);
        rList.add(range);

        // Interpolation methods: only the default
        String interpMeth = ConfigManager.WCST_DEFAULT_INTERPOLATION;
        String nullRes = ConfigManager.WCST_DEFAULT_NULL_RESISTANCE;
        InterpolationMethod interp = new InterpolationMethod(interpMeth, nullRes);
        Set<InterpolationMethod> interpList = new HashSet<InterpolationMethod>(1);
        interpList.add(interp);

        // Null sets
            /* TODO: update for multi-band images */
        String nullVal = "0";
        Set<String> nullSet = new HashSet<String>(1);
        nullSet.add(nullVal);

        // Descriptions
        String abstr = null;
        String title = "Coverage " + identifier;
        String keywords = null;


        m =
                new Metadata(cellList, rList, nullSet, nullDefault, interpList,
                interp, identifier, domList, null, title, abstr, keywords);

        LOG.debug("Done creating default metadata");
        return m;
    }

    /**
     * Processes one <Coverage> node from the input XML
     *
     * @param elem the JAXB node equivalent to the <Coverage> node
     */
    private void processInputCoverageNode(CoverageType elem) throws WCSException {
        if (elem.getAction() == null) {
            throw new InvalidParameterValueException("Action. Explanation: "
                    + "Every <Coverage> node must contain an <Action> child node.");
        }

        String action = elem.getAction().getValue();
        String identifier = null;
        List references = null;

        if (elem.getIdentifier() == null) {
            throw new InvalidParameterException("Identifier");
        }

        identifier = elem.getIdentifier().getValue();
        references = elem.getAbstractReferenceBase();

        if (action.equalsIgnoreCase("Add")) {
            actionAddCoverage(identifier, references);
        } else if (action.equalsIgnoreCase("UpdateMetadata")) {
            actionUpdateMetadata(identifier, references);
        } else if (action.equalsIgnoreCase("Delete")) {
            actionDeleteCoverage(identifier, references);
        } else if (action.equalsIgnoreCase("UpdateAll")) {
            actionUpdateAll(identifier, references);
        } else if (action.equalsIgnoreCase("UpdateDataPart")) {
            throw new ActionNotSupportedException("Action \"UpdateDataPart\" is not supported yet.");
            /* TODO: UpdateDataPart is not yet functional. The Rasdaman server
             * returns with an unexpected internal error (code: 10000) when
             * a partial update query is sent. */
//			actionUpdateDataPart(identifier, references);
        }
    }

    /**
     * Performs the action "UpdateAll", as part of the Transaction operation
     *
     * @param identifier Name of coverage to update
     * @param references List of references with data for update
     */
    private void actionUpdateAll(String identifier, List references) throws WCSException {
        LOG.trace("Executing action Update All ...");
        actionUpdateDataPart(identifier, references);
        actionUpdateMetadata(identifier, references);
        LOG.trace("Finished action Update All!");
    }

    /**
     * Updates the Metadata DB with the information contained in the CoverageDescriptions XML object
     *
     * @param identifier ID of the coverage
     * @param desc object that contains the coverage description.
     */
    private Metadata updateMetadataWithDescription(Metadata meta, CoverageDescriptionType desc) throws WCSException {
        LOG.debug("Updating metadata with values from CoverageDescription...");

        /* (B) Table ps_descriptions: Update coverage title, abstract, keywords */
        String title = desc.getTitle();
        String abstr = desc.getAbstract();
        String keywords = desc.getKeywords().toString();

        meta.setAbstract(abstr);
        meta.setKeywords(keywords);
        meta.setTitle(title);

        /* (C) Table ps_range: Update field name, types, and interpolation methods */

        if (desc.getRange() != null) {
            Set<InterpolationMethod> interpSet = new HashSet<InterpolationMethod>();
            RangeType range = desc.getRange();
            List<FieldType> fields = range.getField();
            try {
                LOG.debug("Updating range information...");
                Iterator<FieldType> i = fields.iterator();
                ArrayList<RangeElement> rangeList = new ArrayList<RangeElement>();

                while (i.hasNext()) {
                    FieldType field = i.next();

                    String name = field.getIdentifier();
                    String datatype = field.getDefinition().getDataType().getValue();
                    RangeElement fieldRange = new RangeElement(name, datatype);
                    rangeList.add(fieldRange);

                    InterpolationMethods methods = field.getInterpolationMethods();
                    String interpType = methods.getDefaultMethod().getValue();
                    String nullResist = methods.getDefaultMethod().getNullResistance();
                    InterpolationMethod interp = new InterpolationMethod(interpType, nullResist);
                    interpSet.add(interp);

                    Iterator<InterpolationMethodType> it = methods.getOtherMethod().iterator();
                    while (it.hasNext()) {
                        InterpolationMethodType imt = it.next();
                        String type = imt.getValue();
                        String resis = imt.getNullResistance();
                        interp = new InterpolationMethod(type, resis);
                        interpSet.add(interp);
                    }
                }
                meta.setRange(rangeList);
            } catch (InvalidMetadataException e) {
                throw new InternalComponentException("Invalid metadata.", e);
            }
            meta.setInterpolationSet(interpSet);
        }

        /* (D) Table ps_coverage: Update default interpolation method and null resistance */

        /*
         *  We store interpolation methods at coverage level, not field level.
         * So we only look at the interpolation method list of the first field,
         * and use it on the whole coverage
         */
        if (desc.isSetRange()) {
            try {
                LOG.debug("Updating default interpolation method...");
                InterpolationMethodType def1 = desc.getRange().getField().get(0).getInterpolationMethods().getDefaultMethod();
                String method = def1.getValue();
                String resist = def1.getNullResistance();

                InterpolationMethod meth = new InterpolationMethod(method, resist);
                meta.setDefaultInterpolation(meth);
            } catch (InvalidMetadataException e) {
                throw new InternalComponentException("Invalid metadata.", e);
            }
        }

        /* (E) Table ps_celldomain: Update cell domain of the coverage. */
        /* NOTE: Only works for 2-D (x/y) or 3-D (x/y/t) coverages */

        if (desc.isSetDomain()) {
            LOG.debug("Updating spatial bounding box of coverage ...");
            try {
                List<JAXBElement<? extends BoundingBoxType>> list =
                        desc.getDomain().getSpatialDomain().getBoundingBox();
                if (list.size() == 1) {
                    BoundingBoxType bbox = (BoundingBoxType) list.get(0).getValue();
                    if (bbox.getCrs() == null || bbox.getCrs().equals(DomainElement.IMAGE_CRS)) {
                        meta = updateImageCrsBoundingBox(meta, bbox);
                    } else {
                        throw new InvalidParameterValueException("crs. Explanation: Unknown CRS " + bbox.getCrs());
                    }
                } else {
                    Iterator i = list.iterator();
                    while (i.hasNext()) {
                        BoundingBoxType bbox = (BoundingBoxType) i.next();
                        if (bbox.getCrs().equals(DomainElement.IMAGE_CRS)) {
                            meta = updateImageCrsBoundingBox(meta, bbox);
                        }
                        // TODO: Implement WGS84 update
                        //                    if (bbox.getCrs().equals(DomainElement.WGS84_CRS))
                        //                        updateWgs84CrsBoundingBox(meta, bbox);
                    }
                }
            } catch (InvalidMetadataException e) {
                throw new InternalComponentException("Invalid metadata.", e);
            }
        }

        if (desc.getDomain().isSetTemporalDomain()) {
            LOG.debug("Updating temporal bounding box of coverage ...");
            /*
            try
            {
            List<Object> list =
            desc.getDomain().getTemporalDomain().getTimePositionOrTimePeriod();
            if (list.size() == 1)
            {
            Object obj = list.get(0);
            if (obj instanceof TimePeriodType)
            {
            TimePeriodType period = (TimePeriodType) obj;
            TimePositionType start = period.getBeginPosition();
            TimePositionType end = period.getEndPosition();
            }
            }
            else
            {
            throw new WCSException("InvalidParameterValue", "TemporalDomain",
            "Exactly one time-period should be present in the " +
            "Temporal Domain of coverage: " + meta.getCoverageName());
            }
            }
            catch (InvalidMetadataException e)
            {
            throw new WCSException("InvalidParameterValue", "Unknown, please look at the root cause exception.", e);
            }
             */
            LOG.error("This server did not implement the parsing of 'TimePeriod' nodes.");
            throw new NodeParsingNotImplementedException("This server did not implement the parsing of 'TimePeriod' nodes.");
        }

        /* (F) Table ps_crss: Update supported CRS */
        // TODO later ... we don't support CRSs as of yet

        return meta;
    }

    /**
     * Performs the action "UpdateDataPart", as part of the Transaction operation
     *
     * @param identifier
     * @param references
     */
    private void actionUpdateDataPart(String identifier, List references) throws WCSException {
        LOG.trace("Executing action UpdateDataPart ...");

        // Error checking
        // Only change the metadata for an existing coverage
        Metadata m = null;
        try {
            m = metaDb.read(identifier);
        } catch (ResourceException e) {
            throw new InternalComponentException("Invalid metadata for coverage: " + identifier, e);
        } catch (InvalidWcpsRequestException e) {
            throw new InternalComponentException("Could not read metadata for coverage: " + identifier, e);
        }

        // Obtain the references
        ReferenceType pixels, desc;

        pixels = getPixelsRef(references);
        desc = getDescriptionRef(references);

        // References check. We are updating a coverage values, mandatory are: pixels, description
        if (pixels == null) {
            throw new MissingParameterValueException("Reference role='" + getUrnCode("pixels") + "'");
        }
        if (desc == null) {
            throw new MissingParameterValueException("Reference role='" + getUrnCode("description") + "'");
        }

        // (2) Do the actual processing
        try {
            insertSomePixelsIntoRasdaman(identifier, pixels.getHref(), desc.getHref());
        } catch (Exception e) {
            throw new NoApplicableCodeException(e.getMessage(), e);
        }
    }

    /**
     * Performs the action "UpdateMetadata", as part of the Transaction operation
     *
     * @param identifier
     * @param references
     * @throws wcs.server.core.WCSException
     */
    private void actionUpdateMetadata(String identifier, List references) throws WCSException {
        LOG.trace("Executing action Update Metadata...");

        // Only change the metadata for an existing coverage
        Metadata m = null;
        try {
            m = metaDb.read(identifier);
        } catch (Exception e) {
            throw new InternalComponentException("Invalid metadata for coverage: " + identifier, e);
        }

        // Obtain the references
        ReferenceType descRef, summRef;

        descRef = getDescriptionRef(references);
        summRef = getSummaryRef(references);

        // References check. We are updating metadata, mandatory is only the description
        if (descRef == null) {
            throw new MissingParameterValueException("Reference role='" + getUrnCode("description") + "'");
        }

        LOG.trace("Loading reference: coverage description ...");
        CoverageDescriptionType desc = loadDescriptionReference(identifier, descRef);

        CoverageSummaryType summ = null;

        if (summRef != null) {
            LOG.trace("Loading reference: coverage summary ...");
            summ = loadSummaryReference(summRef);
        }

        LOG.trace("Done loading references !");

        // (2) Do the actual processing
        try {
            Metadata oldMeta = m;
            Metadata newMeta = updateMetadataWithDescription(oldMeta, desc);
            if (summ != null) {
                Metadata tempMeta = newMeta;
                newMeta = updateMetadataWithSummary(newMeta, summ);
            }

            metaDb.updateCoverageMetadata(m, false);
        } catch (Exception e) {
            throw new NoApplicableCodeException("Error while updating metadata.", e);
        }

        LOG.trace("Finished action Update Metadata !");
    }

    /**
     * Performs the action "Add", as part of the Transaction operation
     *
     * @param identifier
     * @param references
     * @throws wcs.server.core.WCSException
     */
    private void actionAddCoverage(String identifier, List references) throws WCSException {
        LOG.trace("Executing action AddCoverage ...");

        // Obtain the references
        ReferenceType pixelsRef, descRef, summRef;

        pixelsRef = getPixelsRef(references);
        descRef = getDescriptionRef(references);
        summRef = getSummaryRef(references);

        // References check. We are adding a coverage, mandatory are: pixels, description
        if (pixelsRef == null) {
            throw new MissingParameterValueException("Reference role='" + getUrnCode("pixels") + "'");
        }
        if (descRef == null) {
            throw new MissingParameterValueException("Reference role='" + getUrnCode("description") + "'");
        }

        LOG.trace("Loading reference: coverage pixels ...");
        BufferedImage img = loadPixelsReference(pixelsRef);

        LOG.trace("Loading reference: coverage description ...");
        CoverageDescriptionType desc = loadDescriptionReference(identifier, descRef);

        CoverageSummaryType summ = null;

        if (summRef != null) {
            LOG.trace("Loading reference: coverage summary ...");
            summ = loadSummaryReference(summRef);
        }

        LOG.trace("Done loading references !");

        /**
         * (1) Check coverage name
         */
        boolean changeId = false;

        if (newCoverages.contains(identifier)) {
            throw new InvalidParameterValueException("Identifier: You cannot use the same identifier twice.");
        }

        if (metaDb.existsCoverageName(identifier)) {
            changeId = true;
            LOG.warn("Changing coverage identifier since coverage '" + identifier + "' already exists !");
        }

        // Generate new coverage name ?
        while (changeId) {
            identifier = "coverage_" + Integer.toString((new Random()).nextInt());
            changeId = metaDb.existsCoverageName(identifier);
        }

        /**
         * (2) Do the actual processing. Stores the image in rasdaman.
         */
        try {
            /* Currently we only support one-band (gray-scale) images. */
            if (img.getColorModel().getNumComponents() != 1) {
                throw new MultiBandImagesNotSupportedException(
                        "This server currently only supports one-band images (grayscale). "
                        + "This coverage has " + img.getColorModel().getNumComponents() + " bands.");
            }
            insertImageIntoRasdaman(identifier, img);
        } catch (Exception e) {
            throw new InternalComponentException("Error while inserting image in rasdaman.", e);
        }

        /**
         * (3) Build the metadata object and store it in the db.
         */
        try {
            Metadata m = createNewCoverageMetadata(identifier, img);
            m = updateMetadataWithDescription(m, desc);
            /* Top level descriptions overwrite other metadata sources */
            if (summ != null) {
                m = updateMetadataWithSummary(m, summ);
            }

            metaDb.insertNewCoverageMetadata(m, false);
        } catch (InvalidMetadataException e) {
            throw new InternalComponentException("Metadata to be inserted is invalid.", e);
        } catch (ResourceException e) {
            throw new InternalComponentException("Could not insert metadata into database.", e);
        }

        /**
         * (4) Indicate success: Add this ID to the output XML document
         */
        CodeType id = new CodeType();
        id.setValue(identifier);
        output.getIdentifier().add(id);
        LOG.trace("Finished action Add !");
    }

    /**
     * Performs the action "Delete", as part of the Transaction operation
     *
     * @param identifier
     * @param references
     * @throws wcs.server.core.WCSException
     */
    private void actionDeleteCoverage(String identifier, List references) throws WCSException {
        LOG.trace("Executing action Delete Coverage ...");

        if (metaDb.existsCoverageName(identifier) == false) {
            throw new InvalidParameterValueException("Identifier");
        }

        // (2) Do the actual processing
        try {
            Metadata m = metaDb.read(identifier);
            deleteCoverageFromRasdaman(identifier);
            metaDb.delete(m, false);
        } catch (Exception e) {
            throw new NoApplicableCodeException("Error while deleting coverage.", e);
        }

        // Indicate success: Add this ID to the output XML document
        CodeType id = new CodeType();

        id.setValue(identifier);
        output.getIdentifier().add(id);

        LOG.trace("Finished action Delete !");
    }

    /**
     * Retrieve the full URN code of a string
     *
     * @param key Internal representation of a URN code
     * @return the URN code
     */
    private String getUrnCode(String key) throws WCSException {
        if (key.equalsIgnoreCase("pixels")) {
            return "urn:ogc:def:role:WCS:1.1:Pixels";
        }
        if (key.equalsIgnoreCase("description")) {
            return "urn:ogc:def:role:WCS:1.1:CoverageDescription";
        }
        if (key.equalsIgnoreCase("summary")) {
            return "urn:ogc:def:role:WCS:1.1:CoverageSummary";
        }
        if (key.equalsIgnoreCase("transform")) {
            return "urn:ogc:def:role:WCS:1.1:GeoreferencingTransformation";
        }
        if (key.equalsIgnoreCase("other")) {
            return "urn:ogc:def:role:WCS:1.1:OtherSource";
        }

        throw new InternalComponentException("Unknown URN key '" + key + "'");
    }

    /**
     * Get the Pixels Reference
     *
     * @param references List of References
     * @return the Pixels Reference
     */
    private ReferenceType getPixelsRef(List references) throws WCSException {
        ReferenceType result = searchReferenceList("pixels", references);

        return result;
    }

    /**
     * Get the Coverage Description Reference
     *
     * @param references List of References
     * @return the Coverage Description Reference
     */
    private ReferenceType getDescriptionRef(List references) throws WCSException {
        ReferenceType result = searchReferenceList("description", references);

        return result;
    }

    /**
     * Get the Coverage Summary Reference
     *
     * @param references List of References
     * @return the Coverage Summary Reference
     */
    private ReferenceType getSummaryRef(List references) throws WCSException {
        ReferenceType result = searchReferenceList("summary", references);

        return result;
    }

    /**
     * Get the Georeferencing Transform Reference
     *
     * @param references List of References
     * @return the Georeferencing Transform Reference
     */
    private ReferenceType getTransformRef(List references) throws WCSException {
        ReferenceType result = searchReferenceList("transform", references);

        return result;
    }

    /**
     * Get the "Other" Reference
     *
     * @param references List of References
     * @return the "Other" Reference
     */
    private ReferenceType getOtherRef(List references) throws WCSException {
        ReferenceType result = searchReferenceList("other", references);

        return result;
    }

    /**
     * Search a references list for a particular term
     * @param key internal name of reference
     * @param references List of references
     * @return a Reference object
     */
    private ReferenceType searchReferenceList(String key, List references) throws WCSException {
        String urn = getUrnCode(key);
        Iterator i = references.iterator();

        while (i.hasNext()) {
            Object obj = i.next();
            JAXBElement jelem = (JAXBElement) obj;
            ReferenceType ref = (ReferenceType) jelem.getValue();
            String role = ref.getRole();

            if (role.equalsIgnoreCase(urn)) {
                return ref;
            }
        }

        return null;
    }

    private Metadata updateImageCrsBoundingBox(Metadata meta, BoundingBoxType bbox) throws WCSException, InvalidMetadataException {
        List<Double> lower = bbox.getLowerCorner();
        List<Double> upper = bbox.getUpperCorner();

        if (lower.size() != 2) {
            throw new InvalidParameterException("LowerCorner. Explanation: Should contain only two numbers.");
        }
        if (upper.size() != 2) {
            throw new InvalidParameterException("UpperCorder. Explanation: Should contain only two numbers.");
        }
        long loX = lower.get(0).longValue();
        long loY = lower.get(1).longValue();
        long hiX = upper.get(0).longValue();
        long hiY = upper.get(1).longValue();

        CellDomainElement cellX = new CellDomainElement(BigInteger.valueOf(loX), BigInteger.valueOf(hiX));
        CellDomainElement cellY = new CellDomainElement(BigInteger.valueOf(loY), BigInteger.valueOf(hiY));

        List<CellDomainElement> list = new ArrayList<CellDomainElement>();
        list.add(cellX);
        list.add(cellY);

        meta.setCellDomain(list);
        return meta.clone();
    }
}
