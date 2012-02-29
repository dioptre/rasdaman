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
package petascope.wcst.transaction;

import java.sql.SQLException;
import net.opengis.ows.v_1_0_0.BoundingBoxType;
import net.opengis.wcs.ows.v_1_1_0.InterpolationMethodType;
import net.opengis.wcs.v_1_1_0.CoverageDescriptionType;
import net.opengis.wcs.v_1_1_0.CoverageDescriptions;
import net.opengis.wcs.v_1_1_0.CoverageSummaryType;
import net.opengis.wcs.v_1_1_0.FieldType;
import net.opengis.wcs.v_1_1_0.RangeType;
import org.apache.commons.io.IOUtils;
import petascope.exceptions.PetascopeException;
import petascope.exceptions.RasdamanException;
import petascope.exceptions.WCPSException;
import petascope.wcps.server.core.SDU;
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
import petascope.core.DbMetadataSource;
import petascope.core.Metadata;
import petascope.exceptions.ExceptionCode;
import petascope.ConfigManager;
import petascope.wcps.server.core.CellDomainElement;
import petascope.wcps.server.core.DomainElement;
import petascope.wcps.server.core.InterpolationMethod;
import petascope.wcps.server.core.RangeElement;
import petascope.exceptions.WCSTException;
import petascope.util.AxisTypes;
import wcst.transaction.schema.CodeType;
import wcst.transaction.schema.CoverageType;
import wcst.transaction.schema.KeywordsType;
import wcst.transaction.schema.LanguageStringType;
import wcst.transaction.schema.ManifestType;
import wcst.transaction.schema.ReferenceType;
import wcst.transaction.schema.TransactionResponseType;
import wcst.transaction.schema.TransactionType;
import petascope.wcst.transaction.tools.RasdamanUtils;
import petascope.util.CrsUtil;

/**
 * This class takes a WCS-T Transaction XML request and executes the request,
 * building the corresponding XML respose. Use this class for synchronous processing.
 *
 * @author Andrei Aiordachioaie
 */
public class executeTransaction {

    private static Logger log = LoggerFactory.getLogger(executeTransaction.class);
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
    public executeTransaction(TransactionType tr, DbMetadataSource source) throws WCSTException, RasdamanException {
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
     * @throws WCSTException
     */
    public TransactionResponseType get() throws WCSTException, WCPSException, PetascopeException {
        try {
            if (finished == false) {
                metaDb.ensureConnection();
                process();
            }
        } catch (SQLException e) {
            throw new WCSTException(ExceptionCode.InternalSqlError, "Could not ensure connection to database is valid", e);
        }
        if (finished == false) {
            throw new WCSTException(ExceptionCode.NoApplicableCode, "Could not execute the Transaction request! "
                    + "Please see the other errors...");
        }

        return output;
    }

    /**
     * Computes the response to the Transaction request given to the constructor.
     */
    public void process() throws WCSTException, WCPSException, PetascopeException {
        if (!input.getService().equalsIgnoreCase("WCS")) {
            throw new WCSTException(ExceptionCode.InvalidParameterValue, "Service. Explanation: Service must be \"WCS\" !");
        }
        if (!input.getVersion().equalsIgnoreCase("1.1")) {
            throw new WCSTException(ExceptionCode.InvalidParameterValue, "Service. Explanation: Service Version must be \"1.1\" !");
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
                log.debug("Commit rasdaman changes ...");
                rasUtils.commitAndClose();
                log.debug("Rasdaman coverages saved successfully !");
            } catch (ODMGException e) {
                throw new WCSTException(ExceptionCode.InternalComponentError, "Could not commit Rasdaman changes !", e);
            }

            /* Commit metadata changes */
            try {
                log.debug("Commit metadata changes ...");
                metaDb.commitAndClose();
                log.debug("Metadata has been saved !");
            } catch (SQLException e) {
                throw new WCSTException(ExceptionCode.InternalSqlError, "Could not commit metadata changes", e);
            }
        } catch (WCSTException e) {
            // One action failed, therefore all actions have failed

            /* Abort metadata changes */
            finished = false;
            try {
                log.debug("Rolling back metadata database changes ...");
                metaDb.abortAndClose();
                log.debug("Metadata rollback completed!");
            } catch (SQLException ex) {
                log.error("Could not rollback metadata changes.");
                e.appendExceptionText(" Could not rollback metadata changes!");
            }

            /* Abort rasdaman changes */
            try {
                log.debug("Aborting rasdaman changes ...");
                rasUtils.abortAndClose();
                log.debug("Rasdaman changes aborted !");
            } catch (ODMGException ex) {
                log.error("Could not abort rasdaman changes.");
                e.appendExceptionText(" Could not rollback rasdaman changes!");
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
            log.trace("Deleting coverage from Rasdaman ...");
            rasUtils.deleteCollection(identifier);
            log.trace("Rasdaman Collection '" + identifier + "' is now deleted !");
        } catch (ODMGException e) {
            log.error("Failed to delete rasdaman collection " + identifier);
            throw new WCSTException(ExceptionCode.InternalComponentError, "Failed to delete collection from Rasdaman !", e);
        }
    }

    /**
     * Insert pixel data for a coverage into RasDaMan DB system
     *
     * @param identifier Identifier of the coverage
     * @param href The location of the pixels for the new image
     */
    private void insertImageIntoRasdaman(String identifier, BufferedImage img)
            throws WCSTException, RasdamanException {
        log.trace("Inserting image into Rasdaman raster server...");
        try {
            rasUtils.insertGrayImageAsArray(identifier, img);
            log.debug("Inserted image into Rasdaman !");
        } catch (ODMGException e) {
            log.error("Could not insert image into Rasdaman !");
            throw new WCSTException(ExceptionCode.InternalComponentError, "Could not insert image into Rasdaman.", e);
        }
    }

    /** Load a BufferedImage from a ReferenceType object.
     *
     * @param pixels Reference object
     * @return available image
     * @throws WCSTException
     */
    private BufferedImage loadPixelsReference(ReferenceType pixels) throws WCSTException {
        URL url = null;
        BufferedImage img = null;

        try {
            url = new URL(pixels.getHref());
        } catch (MalformedURLException e) {
            log.error("URL " + url.toString() + " is not valid.");
            throw new WCSTException(ExceptionCode.InvalidParameterValue, "Reference pixels. "
                    + "Explanation: URL " + url.toString() + " is not valid.");
        }

        try {
            img = ImageIO.read(url);
            if (img == null) {
                throw new IOException("Empty stream while reading image.");
            }
        } catch (IOException e) {
            log.error(e.getMessage());
            throw new WCSTException(ExceptionCode.IOConnectionError, "Reference Pixels. Explanation: "
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
     * @throws WCSTException
     */
    private CoverageDescriptionType loadDescriptionReference(String identifier,
            ReferenceType desc) throws WCSTException, WCSTException {
        URL url = null;
        String xmlString = null;
        CoverageDescriptions descs = null;
        CoverageDescriptionType desc0 = null;

        // Load the URL
        try {
            url = new URL(desc.getHref());
        } catch (MalformedURLException e) {
            log.error("URL " + url.toString() + " is not valid !");
            throw new WCSTException(ExceptionCode.InvalidParameterValue, "Reference pixels. Explanation: "
                    + "URL " + url.toString() + " is not valid.");
        }

        // Read the contents of the URL
        try {
            URLConnection conn = url.openConnection();
            BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));

            xmlString = IOUtils.toString(in);
        } catch (IOException ex) {
            throw new WCSTException(ExceptionCode.IOConnectionError, "Description Reference. Explanation: error loading the "
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
                log.error("Coverage description metadata is not a valid xml document.");
                throw new WCSTException(ExceptionCode.XmlNotValid, "Coverage "
                        + "description metadata is not a valid xml document.");
            }

        } catch (javax.xml.bind.JAXBException ex) {
            throw new WCSTException(ExceptionCode.XmlStructuresError, "Could not marshall/unmarshall XML structures.", ex);
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
            throw new WCSTException(ExceptionCode.InvalidRequest, "Could not find a CoverageDescription for coverage: " + identifier);
        }

        return desc0;
    }

    private CoverageSummaryType loadSummaryReference(ReferenceType pixels) throws WCSTException {
        URL url = null;
        String xmlString = null;
        CoverageSummaryType xml = null;

        try {
            url = new URL(pixels.getHref());
        } catch (MalformedURLException e) {
            throw new WCSTException(ExceptionCode.InvalidParameterValue, "Reference summary. Explanation: URL is not valid.", e);
        }

        // Read the contents of the URL
        try {
            URLConnection conn = url.openConnection();
            BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));

            xmlString = IOUtils.toString(in);
        } catch (IOException ex) {
            throw new WCSTException(ExceptionCode.IOConnectionError, "Summary Reference. Explanation: "
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
                throw new WCSTException(ExceptionCode.XmlNotValid, "Coverage Summary metadata is not a valid xml document.");
            }
        } catch (javax.xml.bind.JAXBException ex) {
            throw new WCSTException(ExceptionCode.XmlStructuresError, "Could not marshall/unmarshall XML structures.", ex);
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
            CoverageSummaryType summary) throws WCSTException {
        log.debug("Updating metadata with values from Coverage Summary...");

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
        log.error("Partial update is not yet implemented");
        throw new UnsupportedOperationException("Partial Rasdaman update is not yet implemented.");
    }

    /**
     * Inserts default metadata values for the given coverage.
     *
     * @param identifier ID of the coverage
     * @param img The image, fetched from external reference
     * @throws WCSTException on error
     */
    private Metadata createNewCoverageMetadata(String identifier, BufferedImage img) throws WCPSException, PetascopeException {
        Metadata m = null;
        log.debug("Creating metadata with default values...");

        // TODO: When we accept multi-band images, update nullDefault
        String nullDefault = "0";

        // Cell domains
        BigInteger lowX = new BigInteger("0");
        BigInteger highX = new BigInteger(String.valueOf(img.getHeight() - 1));
        BigInteger lowY = new BigInteger("0");
        BigInteger highY = new BigInteger(String.valueOf(img.getWidth() - 1));
        CellDomainElement cellX = new CellDomainElement(lowX, highX, AxisTypes.X_AXIS);
        CellDomainElement cellY = new CellDomainElement(lowY, highY, AxisTypes.Y_AXIS);
        List<CellDomainElement> cellList = new ArrayList<CellDomainElement>(2);
        cellList.add(cellX);
        cellList.add(cellY);

        // Domains
        Set<String> crsSet = new HashSet<String>(1);
        crsSet.add(CrsUtil.IMAGE_CRS);
        String str1 = null, str2 = null;
        /* Since we currently do not use the Domain sizes, we can set them to 0 and 1 */
        DomainElement domX = new DomainElement(AxisTypes.X_AXIS, AxisTypes.X_AXIS, 0.0, 1.0, str1, str2, crsSet, metaDb.getAxisNames(), null);
        DomainElement domY = new DomainElement(AxisTypes.Y_AXIS, AxisTypes.Y_AXIS, 0.0, 1.0, str1, str2, crsSet, metaDb.getAxisNames(), null);
        List<DomainElement> domList = new ArrayList<DomainElement>(2);
        domList.add(domX);
        domList.add(domY);

        // Ranges
            /* TODO: When multiple-field images are supported, update ranges */
        RangeElement range = new RangeElement("intensity", ConfigManager.WCST_DEFAULT_DATATYPE, null);
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
        String type = "GridCoverage"; // FIXME
        String keywords = null;


        m = new Metadata(cellList, rList, nullSet, nullDefault, interpList,
                interp, identifier, type, domList, null, title, abstr, keywords);

        log.debug("Done creating default metadata");
        return m;
    }

    /**
     * Processes one <Coverage> node from the input XML
     *
     * @param elem the JAXB node equivalent to the <Coverage> node
     */
    private void processInputCoverageNode(CoverageType elem) throws WCSTException, WCPSException, PetascopeException {
        if (elem.getAction() == null) {
            throw new WCSTException(ExceptionCode.InvalidParameterValue, "Action. Explanation: "
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
            throw new WCSTException(ExceptionCode.OperationNotSupported, "\"UpdateDataPart\" is not supported yet.");
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
    private void actionUpdateAll(String identifier, List references) throws WCSTException, PetascopeException {
        log.trace("Executing action Update All ...");
        actionUpdateDataPart(identifier, references);
        actionUpdateMetadata(identifier, references);
        log.trace("Finished action Update All!");
    }

    /**
     * Updates the Metadata DB with the information contained in the CoverageDescriptions XML object
     *
     * @param identifier ID of the coverage
     * @param desc object that contains the coverage description.
     */
    private Metadata updateMetadataWithDescription(Metadata meta, CoverageDescriptionType desc) throws WCPSException, WCSTException {
        log.debug("Updating metadata with values from CoverageDescription...");

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
            log.debug("Updating range information...");
            Iterator<FieldType> i = fields.iterator();
            ArrayList<RangeElement> rangeList = new ArrayList<RangeElement>();

            while (i.hasNext()) {
                FieldType field = i.next();

                String name = field.getIdentifier();
                String datatype = field.getDefinition().getDataType().getValue();
                RangeElement fieldRange = new RangeElement(name, datatype, null); // FIXME uom = null
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
            meta.setInterpolationSet(interpSet);
        }

        /* (D) Table ps_coverage: Update default interpolation method and null resistance */

        /*
         *  We store interpolation methods at coverage level, not field level.
         * So we only look at the interpolation method list of the first field,
         * and use it on the whole coverage
         */
        if (desc.isSetRange()) {
            log.debug("Updating default interpolation method...");
            InterpolationMethodType def1 = desc.getRange().getField().get(0).getInterpolationMethods().getDefaultMethod();
            String method = def1.getValue();
            String resist = def1.getNullResistance();

            InterpolationMethod meth = new InterpolationMethod(method, resist);
            meta.setDefaultInterpolation(meth);
        }

        /* (E) Table ps_celldomain: Update cell domain of the coverage. */
        /* NOTE: Only works for 2-D (x/y) or 3-D (x/y/t) coverages */

        if (desc.isSetDomain()) {
            log.debug("Updating spatial bounding box of coverage ...");
            List<JAXBElement<? extends BoundingBoxType>> list =
                    desc.getDomain().getSpatialDomain().getBoundingBox();
            if (list.size() == 1) {
                BoundingBoxType bbox = (BoundingBoxType) list.get(0).getValue();
                if (bbox.getCrs() == null || bbox.getCrs().equals(CrsUtil.IMAGE_CRS)) {
                    meta = updateImageCrsBoundingBox(meta, bbox);
                } else {
                    throw new WCSTException(ExceptionCode.InvalidParameterValue, "crs. Explanation: Unknown CRS " + bbox.getCrs());
                }
            } else {
                Iterator i = list.iterator();
                while (i.hasNext()) {
                    BoundingBoxType bbox = (BoundingBoxType) i.next();
                    if (bbox.getCrs().equals(CrsUtil.IMAGE_CRS)) {
                        meta = updateImageCrsBoundingBox(meta, bbox);
                    }
                    // TODO: Implement WGS84 update
                    //                    if (bbox.getCrs().equals(DomainElement.WGS84_CRS))
                    //                        updateWgs84CrsBoundingBox(meta, bbox);
                }
            }
        }

        if (desc.getDomain().isSetTemporalDomain()) {
            log.debug("Updating temporal bounding box of coverage ...");
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
            throw new WCSTException("InvalidParameterValue", "TemporalDomain",
            "Exactly one time-period should be present in the " +
            "Temporal Domain of coverage: " + meta.getCoverageName());
            }
            }
            catch (InvalidMetadataException e)
            {
            throw new WCSTException("InvalidParameterValue", "Unknown, please look at the root cause exception.", e);
            }
             */
            log.error("This server did not implement the parsing of 'TimePeriod' nodes.");
            throw new WCSTException(ExceptionCode.NodeParsingNotImplemented,
                    "This server did not implement the parsing of 'TimePeriod' nodes.");
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
    private void actionUpdateDataPart(String identifier, List references) throws WCSTException, PetascopeException {
        log.trace("Executing action UpdateDataPart ...");

        // Error checking
        // Only change the metadata for an existing coverage
        Metadata m = metaDb.read(identifier);

        // Obtain the references
        ReferenceType pixels, desc;

        pixels = getPixelsRef(references);
        desc = getDescriptionRef(references);

        // References check. We are updating a coverage values, mandatory are: pixels, description
        if (pixels == null) {
            throw new WCSTException(ExceptionCode.MissingParameterValue, "Reference role='" + getUrnCode("pixels") + "'");
        }
        if (desc == null) {
            throw new WCSTException(ExceptionCode.MissingParameterValue, "Reference role='" + getUrnCode("description") + "'");
        }

        // (2) Do the actual processing
        try {
            insertSomePixelsIntoRasdaman(identifier, pixels.getHref(), desc.getHref());
        } catch (Exception e) {
            throw new WCSTException(ExceptionCode.NoApplicableCode, e.getMessage(), e);
        }
    }

    /**
     * Performs the action "UpdateMetadata", as part of the Transaction operation
     *
     * @param identifier
     * @param references
     * @throws wcs.server.core.WCSTException
     */
    private void actionUpdateMetadata(String identifier, List references) throws WCSTException {
        log.trace("Executing action Update Metadata...");

        // Only change the metadata for an existing coverage
        Metadata m = null;
        try {
            m = metaDb.read(identifier);
        } catch (Exception e) {
            throw new WCSTException(ExceptionCode.InternalComponentError, "Invalid metadata for coverage: " + identifier, e);
        }

        // Obtain the references
        ReferenceType descRef, summRef;

        descRef = getDescriptionRef(references);
        summRef = getSummaryRef(references);

        // References check. We are updating metadata, mandatory is only the description
        if (descRef == null) {
            throw new WCSTException(ExceptionCode.MissingParameterValue, "Reference role='" + getUrnCode("description") + "'");
        }

        log.trace("Loading reference: coverage description ...");
        CoverageDescriptionType desc = loadDescriptionReference(identifier, descRef);

        CoverageSummaryType summ = null;

        if (summRef != null) {
            log.trace("Loading reference: coverage summary ...");
            summ = loadSummaryReference(summRef);
        }

        log.trace("Done loading references !");

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
            throw new WCSTException(ExceptionCode.NoApplicableCode, "Error while updating metadata.", e);
        }

        log.trace("Finished action Update Metadata !");
    }

    /**
     * Performs the action "Add", as part of the Transaction operation
     *
     * @param identifier
     * @param references
     * @throws wcs.server.core.WCSTException
     */
    private void actionAddCoverage(String identifier, List references) throws WCSTException, WCPSException, PetascopeException {
        log.trace("Executing action AddCoverage ...");

        // Obtain the references
        ReferenceType pixelsRef, descRef, summRef;

        pixelsRef = getPixelsRef(references);
        descRef = getDescriptionRef(references);
        summRef = getSummaryRef(references);

        // References check. We are adding a coverage, mandatory are: pixels, description
        if (pixelsRef == null) {
            throw new WCSTException(ExceptionCode.MissingParameterValue, "Reference role='" + getUrnCode("pixels") + "'");
        }
        if (descRef == null) {
            throw new WCSTException(ExceptionCode.MissingParameterValue, "Reference role='" + getUrnCode("description") + "'");
        }

        log.trace("Loading reference: coverage pixels ...");
        BufferedImage img = loadPixelsReference(pixelsRef);

        log.trace("Loading reference: coverage description ...");
        CoverageDescriptionType desc = loadDescriptionReference(identifier, descRef);

        CoverageSummaryType summ = null;

        if (summRef != null) {
            log.trace("Loading reference: coverage summary ...");
            summ = loadSummaryReference(summRef);
        }

        log.trace("Done loading references !");

        /**
         * (1) Check coverage name
         */
        boolean changeId = false;

        if (newCoverages.contains(identifier)) {
            throw new WCSTException(ExceptionCode.InvalidParameterValue, "Identifier: You cannot use the same identifier twice.");
        }

        if (metaDb.existsCoverageName(identifier)) {
            changeId = true;
            log.warn("Changing coverage identifier since coverage '" + identifier + "' already exists !");
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
                throw new WCSTException(ExceptionCode.MultiBandImagesNotSupported,
                        "This server currently only supports one-band images (grayscale). "
                        + "This coverage has " + img.getColorModel().getNumComponents() + " bands.");
            }
            insertImageIntoRasdaman(identifier, img);
        } catch (Exception e) {
            throw new WCSTException(ExceptionCode.InternalComponentError, "Error while inserting image in rasdaman.", e);
        }

        /**
         * (3) Build the metadata object and store it in the db.
         */
        Metadata m = createNewCoverageMetadata(identifier, img);
        m = updateMetadataWithDescription(m, desc);
        /* Top level descriptions overwrite other metadata sources */
        if (summ != null) {
            m = updateMetadataWithSummary(m, summ);
        }

        metaDb.insertNewCoverageMetadata(m, false);

        /**
         * (4) Indicate success: Add this ID to the output XML document
         */
        CodeType id = new CodeType();
        id.setValue(identifier);
        output.getIdentifier().add(id);
        log.trace("Finished action Add !");
    }

    /**
     * Performs the action "Delete", as part of the Transaction operation
     *
     * @param identifier
     * @param references
     * @throws wcs.server.core.WCSTException
     */
    private void actionDeleteCoverage(String identifier, List references) throws WCSTException {
        log.trace("Executing action Delete Coverage ...");

        if (metaDb.existsCoverageName(identifier) == false) {
            throw new WCSTException(ExceptionCode.InvalidParameterValue, "Identifier");
        }

        // (2) Do the actual processing
        try {
            Metadata m = metaDb.read(identifier);
            deleteCoverageFromRasdaman(identifier);
            metaDb.delete(m, false);
        } catch (Exception e) {
            throw new WCSTException(ExceptionCode.NoApplicableCode, "Error while deleting coverage.", e);
        }

        // Indicate success: Add this ID to the output XML document
        CodeType id = new CodeType();

        id.setValue(identifier);
        output.getIdentifier().add(id);

        log.trace("Finished action Delete !");
    }

    /**
     * Retrieve the full URN code of a string
     *
     * @param key Internal representation of a URN code
     * @return the URN code
     */
    private String getUrnCode(String key) throws WCSTException {
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

        throw new WCSTException(ExceptionCode.InternalComponentError, "Unknown URN key '" + key + "'");
    }

    /**
     * Get the Pixels Reference
     *
     * @param references List of References
     * @return the Pixels Reference
     */
    private ReferenceType getPixelsRef(List references) throws WCSTException {
        ReferenceType result = searchReferenceList("pixels", references);

        return result;
    }

    /**
     * Get the Coverage Description Reference
     *
     * @param references List of References
     * @return the Coverage Description Reference
     */
    private ReferenceType getDescriptionRef(List references) throws WCSTException {
        ReferenceType result = searchReferenceList("description", references);

        return result;
    }

    /**
     * Get the Coverage Summary Reference
     *
     * @param references List of References
     * @return the Coverage Summary Reference
     */
    private ReferenceType getSummaryRef(List references) throws WCSTException {
        ReferenceType result = searchReferenceList("summary", references);

        return result;
    }

    /**
     * Get the Georeferencing Transform Reference
     *
     * @param references List of References
     * @return the Georeferencing Transform Reference
     */
    private ReferenceType getTransformRef(List references) throws WCSTException {
        ReferenceType result = searchReferenceList("transform", references);

        return result;
    }

    /**
     * Get the "Other" Reference
     *
     * @param references List of References
     * @return the "Other" Reference
     */
    private ReferenceType getOtherRef(List references) throws WCSTException {
        ReferenceType result = searchReferenceList("other", references);

        return result;
    }

    /**
     * Search a references list for a particular term
     * @param key internal name of reference
     * @param references List of references
     * @return a Reference object
     */
    private ReferenceType searchReferenceList(String key, List references) throws WCSTException {
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

    private Metadata updateImageCrsBoundingBox(Metadata meta, BoundingBoxType bbox) throws WCPSException {
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

        CellDomainElement cellX = new CellDomainElement(BigInteger.valueOf(loX), BigInteger.valueOf(hiX), AxisTypes.X_AXIS);
        CellDomainElement cellY = new CellDomainElement(BigInteger.valueOf(loY), BigInteger.valueOf(hiY), AxisTypes.Y_AXIS);

        List<CellDomainElement> list = new ArrayList<CellDomainElement>();
        list.add(cellX);
        list.add(cellY);

        meta.setCellDomain(list);
        return meta.clone();
    }
}
