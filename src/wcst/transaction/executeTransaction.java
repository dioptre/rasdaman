/*
 * This file is part of PetaScope.
 *
 * PetaScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * PetaScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with PetaScope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.PetaScope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */


package wcst.transaction;

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

import wcst.server.core.CodeType;
import wcst.server.core.CoverageType;
import wcst.server.core.KeywordsType;
import wcst.server.core.LanguageStringType;
import wcst.server.core.ManifestType;
import wcst.server.core.ReferenceType;
import wcst.server.core.TransactionResponseType;
import wcst.server.core.TransactionType;

import wcst.transaction.tools.SDU;

//~--- JDK imports ------------------------------------------------------------

import java.awt.Graphics;
import java.awt.Panel;
import java.awt.image.BufferedImage;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StringReader;

import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Random;
import java.util.UUID;

import javax.imageio.ImageIO;


import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.Unmarshaller;
import org.odmg.ODMGException;
import wcps.server.core.Metadata;
import wcst.server.ConfigManager;
import wcst.transaction.tools.RasdamanUtils;

/**
 * This class takes a WCS-T Transaction XML request and executes the request,
 * building the corresponding XML respose. Use this class for synchronous processing.
 *
 * @author Andrei Aiordachioaie
 */
public class executeTransaction
{
	private static boolean printLog = true;
	private boolean finished;
	private TransactionType input;
	private MetadataDb meta;
	protected TransactionResponseType output;
	private String metadataSettingsPath;
	private RasdamanUtils rasUtils;
    private String requestId;

	/**
	 * Default constructor. Initialize internal variables.
	 * @param tr Transaction object, a WCS-T request
	 * @param metadataDbPath Path to the "dbparams.properties" file
	 */
	public executeTransaction(TransactionType tr, String metadataDbPath) throws WCSTException
	{
		input = tr;
		output = new TransactionResponseType();
		finished = false;
		metadataSettingsPath = metadataDbPath;
		meta = new MetadataDb(metadataSettingsPath);

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
    public String generateRequestId()
    {
        requestId = String.valueOf(UUID.randomUUID());
        return requestId;
    }

	/**
	 * Log a message to System.out
	 * @param str Text of the message
	 */
	private static void log(String str)
	{
		if ( printLog )
		{
			System.out.println("<WCS Transaction> " + str);
		}
	}

	/**
	 * Main method of this class: Computes the response to the TransactionResponse
	 * request given to the constructor. If needed, it also calls <b>process()</b>
	 * @return a TransactionResponse object.
	 * @throws WCSTException
	 */
	public TransactionResponseType get() throws WCSTException
	{
		try
		{
			if ( finished == false )
			{
                process();
			}
		}
		catch (WCSTException e)
		{
			e.printStackTrace();

			throw e;
		}
		if ( finished == false )
		{
			throw new WCSTException("NoApplicableCode", "Could not execute the Transaction request! " + "Please see the other errors...");
		}

		return output;
	}

	/**
	 * Computes the response to the Transaction request given to the constructor.
	 */
	public void process() throws WCSTException
	{
		if ( ! input.getService().equalsIgnoreCase("WCS") )
		{
			throw new WCSTException("InvalidParameterValue", "Service", "Service must be \"WCS\" !");
		}
		if ( ! input.getVersion().equalsIgnoreCase("1.1") )
		{
			throw new WCSTException("InvalidParameterValue", "Service", "Service Version must be \"1.1\" !");
		}

		// Set the output request ID
		String reqID = input.getRequestId();
		if ( reqID == null )
		{
			reqID = requestId;
		}
		output.setRequestId(reqID);

		// All actions succeed or fail as one group.
		try
		{
			ManifestType covs = input.getInputCoverages();
			List l = covs.getReferenceGroup();

			for (int i = 0; i < l.size(); i++)
			{
				// This object is the XML element "InputCoverages"
				Object obj = ((JAXBElement) l.get(i)).getValue();
				CoverageType cov = (CoverageType) obj;

				// Each action adds something to the output XML document
				processInputCoverageNode(cov);
			}
			
            finished = true;

            /*  Commit rasdaman changes */
            try
            {
                log("Commit rasdaman changes ...");
                rasUtils.commitAndClose();
                log("Rasdaman coverages saved successfully !");
            }
            catch (ODMGException e)
            {
                log("Could not commit rasdaman changes: " + e.getMessage());
                throw new WCSTException("NoApplicableCode", "Could not commit Rasdaman changes !");
            }            

            /* Commit metadata changes */
            try
            {
                log("Commit metadata changes ...");
                meta.commitChangesAndClose();
                log("Metadata has been saved !");
            }
            catch (SQLException ex)
            {
                log("Could not commit metadata changes: " + ex.getMessage());
                throw new WCSTException("NoApplicableCode", "Could not commit metadata changes !");
            }
		}
		catch (WCSTException e)
		{
			// One action failed, therefore all actions have failed
			e.printStackTrace();

            /* Abort metadata changes */
			finished = false;
            try
            {
                log("Rolling back metadata database changes ...");
                meta.abortChangesAndClose();
                log("Metadata rollback completed!");
            }
            catch (SQLException ex)
            {
                log("Could not rollback metadata changes: " + ex.getMessage());
            }

            /* Abort rasdaman changes */
            try
            {
                log("Aborting rasdaman changes ...");
                rasUtils.abortAndClose();
                log("Rasdaman changes aborted !");
            }
            catch (ODMGException ex)
            {
                log("Could not abort rasdaman changes: " + ex.getMessage());
            }

			throw e;
		}
        catch (Exception e)
        {
            // Unknown error handling
            e.printStackTrace();
            throw new WCSTException("NoApplicableCode", "Runtime error: " + e.getMessage());
        }
	}

    /**
     * Delete a coverage from the Rasdaman server.
     * @param identifier Name of coverage
     * @throws Exception
     */
	private void deleteCoverageFromRasdaman(String identifier) throws Exception
	{
		try
		{
			log("Deleting coverage from Rasdaman ...");
			rasUtils.deleteCollection(identifier);
			log("Rasdaman Collection '" + identifier + "' is now deleted !");
		}
		catch (ODMGException e)
		{
            e.printStackTrace();
			log("Failed to delete rasdaman collection " + identifier);

			throw new ODMGException("Failed to delete collection from Rasdaman !");
		}
	}

	/**
	 * Delete all records of a coverage from the metadata DB
	 *
	 * @param identifier ID of the coverage
	 */
	private void deleteCoverageMetadata(String identifier) throws Exception
	{
		log("Deleting coverage " + identifier + " from the metadata DB");
		int id = MetadataUtils.getCoverageID(meta, identifier);
		String strId = Integer.toString(id);

        // These auxiliary metadata are automatically deleted by the DB (via CASCADING) on
        // deletion of the main entry in ps_coverage
        /*
		// Delete auxiliary declarations
		MetadataUtils.deleteRowFromTable(meta, "ps_celldomain", "coverage", strId);
		MetadataUtils.deleteRowFromTable(meta, "ps_domain", "coverage", strId);
		MetadataUtils.deleteRowFromTable(meta, "ps_interpolationmethodlist", "coverage", strId);
		MetadataUtils.deleteRowFromTable(meta, "ps_interpolationset", "coverage", strId);
		MetadataUtils.deleteRowFromTable(meta, "ps_nullset", "coverage", strId);
		MetadataUtils.deleteRowFromTable(meta, "ps_range", "coverage", strId);
        */

        // Delete main entry from "ps_coverage"
		MetadataUtils.deleteRowFromTable(meta, "ps_coverage", "name", identifier);

        log("Coverage " + identifier + " is now deleted from the Metadata !");
	}

	/**
	 * Insert pixel data for a coverage into RasDaMan DB system
	 *
	 * @param identifier Identifier of the coverage
	 * @param href The location of the pixels for the new image
	 */
	private void insertImageIntoRasdaman(String identifier, BufferedImage img)
	{
		log("Inserting image into Rasdaman raster server...");
		try
		{
			// Display image for check
            rasUtils.insertGrayImageAsArray(identifier, img);
            log("Inserted image into Rasdaman !");
		}
		catch (Exception e)
		{
			System.err.println("Error !");
			e.printStackTrace();
		}
	}

    /** Load a BufferedImage from a ReferenceType object.
     *
     * @param pixels Reference object
     * @return available image
     * @throws WCSTException
     */
	private BufferedImage loadPixelsReference(ReferenceType pixels) throws WCSTException
	{
		URL url = null;
		BufferedImage img = null;

		try
		{
			url = new URL(pixels.getHref());
		}
		catch (MalformedURLException e)
		{
			log("URL " + url.toString() + " is not valid !");
			throw new WCSTException("InvalidParameterValue", "Reference pixels");
		}

		try
		{
			img = ImageIO.read(url);
		}
		catch (IOException ex)
		{
			throw new WCSTException("NoApplicableCode", "The URL '" + url + "' does not contain a valid image.");
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
	private CoverageDescriptionType loadDescriptionReference(String identifier, ReferenceType desc) throws WCSTException
	{
		URL url = null;
		String xmlString = null;
		CoverageDescriptions descs = null;
		CoverageDescriptionType desc0 = null;

		// Load the URL
		try
		{
			url = new URL(desc.getHref());
		}
		catch (MalformedURLException e)
		{
			log("URL " + url.toString() + " is not valid !");

			throw new WCSTException("InvalidParameterValue", "Reference pixels");
		}

		// Read the contents of the URL
		try
		{
			URLConnection conn = url.openConnection();
			BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));

			xmlString = IOUtils.toString(in);
		}
		catch (IOException ex)
		{
			throw new WCSTException("NoApplicableCode", "Error loading the " + "coverage description from URL " + url.toString());
		}

		// Unmarshall the XML string into a Java Object
		try
		{
			JAXBContext jaxbCtx = JAXBContext.newInstance(CoverageDescriptions.class.getPackage().getName());
			Unmarshaller unmarshaller = jaxbCtx.createUnmarshaller();
			Object obj = unmarshaller.unmarshal(new StringReader(xmlString));

			if ( obj instanceof JAXBElement )
				descs = (CoverageDescriptions) ((JAXBElement) obj).getValue();
			else if ( obj instanceof CoverageDescriptions )
				descs = (CoverageDescriptions) obj;
			else
				throw new WCSTException("NoApplicableCode", "Coverage " + "Description metadata could not be loaded !");
		}
		catch (javax.xml.bind.JAXBException ex)
		{
			log("Could not unmarshall the CoverageDescription XML document: " + ex.getMessage());
			throw new WCSTException("NoApplicableCode", "Could not unmarshall " + "the CoverageDescription XML document: " + ex.getMessage());
		}

		// Filter by coverage name
		desc0 = null;
		Iterator<CoverageDescriptionType> i = descs.getCoverageDescription().iterator();

		while (i.hasNext())
		{
			CoverageDescriptionType d = i.next();

			if ( d.getIdentifier().equals(identifier) )
			{
				desc0 = d;
				break;
			}
		}

        if (desc0 == null)
            throw new WCSTException("NoApplicableCode", "Could not find a CoverageDescription for coverage: " + identifier);

		return desc0;
	}

	private CoverageSummaryType loadCoverageSummary(ReferenceType pixels) throws WCSTException
	{
		URL url = null;
		String xmlString = null;
		CoverageSummaryType xml = null;

		try
		{
			url = new URL(pixels.getHref());
		}
		catch (MalformedURLException e)
		{
			log("URL " + url.toString() + " is not valid !");

			throw new WCSTException("InvalidParameterValue", "Reference summary");
		}

		// Read the contents of the URL
		try
		{
			URLConnection conn = url.openConnection();
			BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));

			xmlString = IOUtils.toString(in);
		}
		catch (IOException ex)
		{
			throw new WCSTException("NoApplicableCode", "Error loading the " + "coverage summary from URL " + url.toString());
		}

		// Unmarshall the XML string into a Java Object
		try
		{
			JAXBContext jaxbCtx = JAXBContext.newInstance(CoverageSummaryType.class.getPackage().getName());
			Unmarshaller unmarshaller = jaxbCtx.createUnmarshaller();
			Object obj = unmarshaller.unmarshal(new StringReader(xmlString));

			if ( obj instanceof JAXBElement )
				xml = (CoverageSummaryType) ((JAXBElement) obj).getValue();
			else if ( obj instanceof CoverageSummaryType )
				xml = (CoverageSummaryType) obj;
			else
				throw new WCSTException("NoApplicableCode", "Coverage " + "Summary metadata could not be loaded !");
		}
		catch (javax.xml.bind.JAXBException ex)
		{
			log("Could not unmarshall the CoverageSummaryXML document !");

			throw new WCSTException("NoApplicableCode", "Could not unmarshall " + "the CoverageSummary XML document !");
		}

		return xml;
	}

	/**
	 * Updates the coverage metadata: textual descriptions. The title and the abstract
	 * are specified in multiple languages, but we only store english.
	 *
	 * @param identifier ID of the coverage
	 * @param title title to set
	 * @param covAbstract abstract to set
	 * @param keywords list of keywords
	 */
	private void updateCoverageMetadataFromSummary(String identifier, CoverageSummaryType summary) throws WCSTException
	{
		log("Updating metadata with values from Coverage Summary...");

		String title = null, abstr = null, keywords = null;

		title = summary.getTitle();
		abstr = summary.getAbstract();
		List<net.opengis.ows.v_1_0_0.KeywordsType> keywordL = summary.getKeywords();
		List<String> kList = new ArrayList<String>();
		Iterator i = keywordL.iterator();

		while (i.hasNext())
		{
			KeywordsType keywordInMultipleLangs = (KeywordsType) i.next();
			String keyword = filterAcceptedLanguage(keywordInMultipleLangs.getKeyword());

			kList.add(keyword);
		}
		keywords = SDU.string2str(kList);

		int covId = MetadataUtils.getCoverageID(meta, identifier);

		if ( MetadataUtils.updateDescriptionMetadata(meta, covId, title, abstr, keywords) == false )
			throw new WCSTException("NoApplicableCode", "Could not update textual description metadata !");
	}

	/**
	 * Retrieve only the string in the accepted language from a multiple-language list
	 *
	 * @param list List of strings in several languages
	 * @return String in the accepted language, or null if none is found
	 */
	private String filterAcceptedLanguage(List<LanguageStringType> list)
	{
		String result = null;
		Iterator i = list.iterator();

		while (i.hasNext())
		{
			LanguageStringType a = (LanguageStringType) i.next();

			if ( a.getLang().equals(ConfigManager.LANGUAGE) )
				result = a.getValue();
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
	private void insertSomePixelsIntoRasdaman(String identifier, String pixHref, String descHref)
	{
        // TODO: Implement !
		throw new UnsupportedOperationException("Not yet implemented");
	}

	/**
	 * Inserts default metadata values for the given coverage.
	 *
	 * @param identifier ID of the coverage
	 * @param img The image, fetched from external reference
     * @throws WCSTException on error
	 */
	private void insertDefaultCoverageMetadata(String identifier, BufferedImage img) throws WCSTException
	{
		log("Inserting default metadata values ...");

		// (a) Table ps_coverage: Set default interpolation type + default null resistance + null default
		log("Adding default metadata for coverage: " + identifier);
		int definterp = MetadataUtils.getInterpolationCalledNone(meta);
		int defnullresist = MetadataUtils.getNullResistanceCalledNone(meta);
		String sinterp = Integer.toString(definterp);
		String snullresist = Integer.toString(defnullresist);

		if ( definterp == -1 )
		{
			throw new WCSTException("NoApplicableCode", "Could not find default" + " interpolation method: 'none'");
		}
		if ( defnullresist == -1 )
		{
			throw new WCSTException("NoApplicableCode", "Could not find default null resistance: 'none'");
		}

		if ( MetadataUtils.setAllCoverageMetadata(meta, identifier, null, null, "0", sinterp, snullresist) == false )
		{
			throw new WCSTException("NoApplicableCode", "Could not insert default metadata for coverage: " + identifier);
		}

		// (b) Table ps_domain: insert default values for the X and Y axes
		log("Adding domain metadata (axis X) for coverage: " + identifier);
		int coverageNumId = MetadataUtils.getCoverageID(meta, identifier);
		int axisX = MetadataUtils.getIdByNameGeneral(meta, "ps_axistype", "axistype", "x");
		int axisY = MetadataUtils.getIdByNameGeneral(meta, "ps_axistype", "axistype", "y");
		if ( axisX == -1 )
		{
			throw new WCSTException("NoApplicableCode", "Could not find axis X in axis-types !");
		}
		if ( axisY == -1 )
		{
			throw new WCSTException("NoApplicableCode", "Could not find axis Y in axis-types !");
		}
		if ( MetadataUtils.setDomain(meta, coverageNumId, 0, "x", axisX, (Double) 0.0, (Double) 1.0, null, null) == false )
		{
			throw new WCSTException("NoApplicableCode", "Could not insert Domain Metadata for axis X!");
		}
		log("Adding domain metadata (axis Y) for coverage: " + identifier);
		if ( MetadataUtils.setDomain(meta, coverageNumId, 1, "y", axisY, (Double) 0.0, (Double) 1.0, null, null) == false )
		{
			throw new WCSTException("NoApplicableCode", "Could not insert Domain Metadata for axis Y!");
		}

		// (c) Table ps_celldomain: insert default values for the x and y axes
		log("Adding cell domain metadata (axis X) for coverage: " + identifier);
		if ( MetadataUtils.setCellDomain(meta, coverageNumId, 0, 0, img.getHeight() - 1) == false )
		{
			throw new WCSTException("NoApplicableCode", "Could not insert Cell Domain Metadata for axis X!");
		}
		log("Adding cell domain metadata (axis Y) for coverage: " + identifier);
		if ( MetadataUtils.setCellDomain(meta, coverageNumId, 1, 0, img.getWidth() - 1) == false )
		{
			throw new WCSTException("NoApplicableCode", "Could not insert Cell Domain Metadata for axis Y!");
		}

		// (d) Table ps_ranges: insert values for the only field of the image
		// TODO: Update this to handle multiple fields
		// Datatype: 0..255 = unsigned char
		log("Adding Range metadata for gray-scale coverage: " + identifier);
		int datatypeId = MetadataUtils.getIdByNameGeneral(meta, "ps_datatype", "datatype", "unsigned char");

		if ( datatypeId == -1 )
		{
			throw new WCSTException("NoApplicableCode", "Cannot find datatype 'unsigned char', needed for gray images !");
		}
		if ( MetadataUtils.setRange(meta, coverageNumId, 0, "gray-intensity", datatypeId) == false )
		{
			throw new WCSTException("NoApplicableCode", "Could not insert Range Metadata!");
		}

		// (e) Table ps_interpolationsets: Insert default interpolation tuples
		log("Adding default interpolation tuples for coverage: " + identifier);
		if ( MetadataUtils.setInterpolationSets(meta, coverageNumId, definterp, defnullresist) == false )
		{
			throw new WCSTException("NoApplicableCode", "Could not insert Interpolation Set metadata!");
		}

		// (f) Table ps_nullsets: Insert null value for coverage
		log("Adding null values for coverage: " + identifier);
		if ( MetadataUtils.setNullSets(meta, coverageNumId, "0") == false )
		{
			throw new WCSTException("NoApplicableCode", "Could not insert Null Sets metadata!");
		}

		// (g) Table ps_descriptions: Insert default values for textual descriptions
		log("Adding textual description for coverage: " + identifier);
		if ( MetadataUtils.insertDescription(meta, coverageNumId, "Coverage " + identifier, "Available coverage", identifier) == false )
		{
			throw new WCSTException("NoApplicableCode", "Could not insert textual description metadata!");
		}

        log("Done inserting default metadata");
	}

	/**
	 * Processes one <Coverage> node from the input XML
	 *
	 * @param elem the JAXB node equivalent to the <Coverage> node
	 */
	private void processInputCoverageNode(CoverageType elem) throws WCSTException
	{
		if ( elem.getAction() == null )
		{
			throw new WCSTException("InvalidParameterValue", "Action", "Every <Coverage> node must contain an <Action> child node !!!");
		}

		String action = elem.getAction().getValue();
		String identifier = null;
		List references = null;

		if ( elem.getIdentifier() == null )
		{
			throw new WCSTException("InvalidParameter", "Identifier");
		}

		identifier = elem.getIdentifier().getValue();
		references = elem.getAbstractReferenceBase();

		if ( action.equalsIgnoreCase("Add") )
		{
			actionAddCoverage(identifier, references);
		}
		else if ( action.equalsIgnoreCase("UpdateMetadata") )
		{
			actionUpdateMetadata(identifier, references);
		}
		else if ( action.equalsIgnoreCase("Delete") )
		{
			actionDeleteCoverage(identifier, references);
		}
		else if ( action.equalsIgnoreCase("UpdateAll") )
		{
			actionUpdateAll(identifier, references);
		}
		else if ( action.equalsIgnoreCase("UpdateDataPart") )
		{
            throw new WCSTException("ActionNotSupported",
                    "Action \"UpdateDataPart\" is not supported yet.");
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
	private void actionUpdateAll(String identifier, List references) throws WCSTException
	{
		log("Executing action Update All ...");
        // Updating everything is equivalent to deleting old coverage and adding the new one
		actionDeleteCoverage(identifier, references);
        actionAddCoverage(identifier, references);
        
		log("Finished action Update All!");
	}

	/**
	 * Updates the Metadata DB with the information contained in the CoverageDescriptions XML object
	 *
	 * @param identifier ID of the coverage
	 * @param desc object that contains the coverage description.
	 */
	private void updateCoverageMetadataFromDescription(String identifier, CoverageDescriptionType desc) throws WCSTException
	{
		log("Updating metadata with values from CoverageDescription...");

		if ( MetadataUtils.existsCoverage(meta, identifier) == false )
			throw new WCSTException("NoApplicableCode", "Inexistent coverage: " + identifier);
		int coverageId = MetadataUtils.getCoverageID(meta, identifier);

		log("Updating metadata for coverage name: " + identifier);

		/* (A) Table ps_coverage: Error check for Coverage Name */
		if ( MetadataUtils.existsCoverage(meta, identifier) == false)
            throw new WCSTException("NoApplicableCode", "Could not find metadata for coverage : " + identifier);

		/* (B) Table ps_descriptions: Update coverage title, abstract, keywords */
		String title = desc.getTitle();
		String abstr = desc.getAbstract();
		String keywords = desc.getKeywords().toString();

		log("Using new keywords: " + keywords);
		if ( MetadataUtils.updateDescriptions(meta, coverageId, title, abstr, keywords) == false )
			throw new WCSTException("NoApplicableCode", "Could not update textual descriptions for coverage: " + identifier);

		/* (C) Table ps_range: Update field name and interpolation information */
		log("Updating field information...");
		// delete all previous information
		MetadataUtils.deleteRowFromTable(meta, "ps_range", "coverage", String.valueOf(coverageId));
		// and insert new info from XML
		RangeType range = desc.getRange();
		List<FieldType> fields = range.getField();
		Iterator i = fields.iterator();
		int fieldCount = 0;

		while (i.hasNext())
		{
			FieldType field = (FieldType) i.next();

			String name = field.getIdentifier();
			String datatype = field.getDefinition().getDataType().getValue();
			int typeId = MetadataUtils.getIdByNameGeneral(meta, "ps_datatype", "datatype", datatype);

			if ( typeId == -1 )
				throw new WCSTException("NoApplicableCode", "Unknown datatype: " + datatype);

			if ( MetadataUtils.setRange(meta, coverageId, fieldCount, name, typeId) == false )
				throw new WCSTException("NoApplicableCode", "Could not add information for field '" + name + "' !");
			fieldCount++;
		}

		/* (D) Table ps_coverage: Update default interpolation method and null resistance */

		/*
		 *  We store interpolation methods at coverage level, not field level.
		 * So we only look at the interpolation method list of the first field,
		 * and use it on the whole coverage
		 */
		log("Updating default interpolation and null resistance...");
		InterpolationMethodType defInterp = fields.get(0).getInterpolationMethods().getDefaultMethod();
		String method = defInterp.getValue();
		String resist = defInterp.getNullResistance();
		int interpId = MetadataUtils.getIdByNameGeneral(meta, "ps_interpolationtype", "interpolationtype", method);

		if ( interpId == -1 )
			throw new WCSTException("NoApplicableCode", "Unknown interpolation method: " + method);
		int nullId = MetadataUtils.getIdByNameGeneral(meta, "ps_nullresistance", "nullresistance", resist);

		if ( nullId == -1 )
			throw new WCSTException("NoApplicableCode", "Unknown null resistance: " + resist);
		if ( MetadataUtils.setDefaultCoverageInterpolation(meta, identifier, interpId, nullId) == false )
			throw new WCSTException("NoApplicableCode", "Could not update default interpolation for coverage: " + identifier);

		/* (E) Table ps_celldomain: Update cell domain of the coverage. */
		/* NOTE: Only works for 2-D coverages */
		log("Updating bounding box of coverage ...");
		List<JAXBElement<? extends BoundingBoxType>> list = desc.getDomain().getSpatialDomain().getBoundingBox();

		if ( list.size() != 1 )
			throw new WCSTException("NoApplicableCode", "Exactly 1 bounding box should be present for coverage: " + identifier);
		BoundingBoxType bbox = list.get(0).getValue();
		List<Double> lower = bbox.getLowerCorner();
		List<Double> upper = bbox.getUpperCorner();

		if ( lower.size() != 2 )
			throw new WCSTException("InvalidParameter", "LowerCorner");
		if ( upper.size() != 2 )
			throw new WCSTException("InvalidParameter", "UpperCorder");
		long loX = lower.get(0).longValue();
		long loY = lower.get(1).longValue();
		long hiX = upper.get(0).longValue();
		long hiY = upper.get(1).longValue();

		int xAxisId = MetadataUtils.getAxisNumberByCoverageAndName(meta, coverageId, "x");

		if ( MetadataUtils.updateCellDomain(meta, coverageId, xAxisId, loX, hiX) == false )
			throw new WCSTException("NoApplicableCode", "Could not update X axis limits for coverage: " + identifier);
		int yAxisId = MetadataUtils.getAxisNumberByCoverageAndName(meta, coverageId, "y");

		if ( MetadataUtils.updateCellDomain(meta, coverageId, yAxisId, loY, hiY) == false )
			throw new WCSTException("NoApplicableCode", "Could not update Y axis limits for coverage: " + identifier);

		/* (F) Table ps_crss: Update supported CRS */
		// FIXME later ... we don't support CRSs as of yet
	}

	/**
	 * Performs the action "UpdateDataPart", as part of the Transaction operation
	 *
	 * @param identifier
	 * @param references
	 */
	private void actionUpdateDataPart(String identifier, List references) throws WCSTException
	{
		log("Executing action UpdateDataPart ...");

		// Error checking
		if ( MetadataUtils.existsCoverage(meta, identifier) == false )
		{
			throw new WCSTException("InvalidParameter", "Identifier");
		}

		// Obtain the references
		ReferenceType pixels, desc;

		pixels = getPixelsRef(references);
        desc = getDescriptionRef(references);

		// References check. We are updating a coverage values, mandatory are: pixels, description
		if ( pixels == null )
		{
			throw new WCSTException("MissingParameterValue", "Reference role='" + getUrnCode("pixels") + "'");
		}
        if (desc == null)
        {
            throw new WCSTException("MissingParameterValue", "Reference role='" + getUrnCode("description") + "'");
        }

		// (2) Do the actual processing
		try
		{
			insertSomePixelsIntoRasdaman(identifier, pixels.getHref(), desc.getHref());
		}
		catch (Exception e)
		{
            e.printStackTrace();
			throw new WCSTException(e.getMessage(), e.getStackTrace().toString());
		}
	}

	/**
	 * Performs the action "UpdateMetadata", as part of the Transaction operation
	 *
	 * @param identifier
	 * @param references
	 * @throws wcs.server.core.WCSTException
	 */
	private void actionUpdateMetadata(String identifier, List references) throws WCSTException
	{
		log("Executing action Update Metadata...");

		// Only change the metadata for an existing coverage
		if ( MetadataUtils.existsCoverage(meta, identifier) == false )
		{
			throw new WCSTException("InvalidParameter", "Identifier");
		}

		// Obtain the references
		ReferenceType descRef, summRef;

		descRef = getDescriptionRef(references);
		summRef = getSummaryRef(references);

		// References check. We are updating metadata, mandatory is only description
		if ( descRef == null )
		{
			throw new WCSTException("MissingParameterValue", "Reference role='" + getUrnCode("description") + "'");
		}

		log("Loading reference: coverage description ...");
		CoverageDescriptionType desc = loadDescriptionReference(identifier, descRef);

		CoverageSummaryType summ = null;

		if ( summRef != null )
		{
			log("Loading reference: coverage summary ...");
			summ = loadCoverageSummary(summRef);
		}

		log("Done loading references !");

		// (2) Do the actual processing
		try
		{
			updateCoverageMetadataFromDescription(identifier, desc);
            if (summ != null)
                updateCoverageMetadataFromSummary(identifier, summ);
		}
		catch (Exception e)
		{
            e.printStackTrace();
			throw new WCSTException("NoApplicableCode", e.getMessage());
		}

        log("Finished action Update Metadata !");
	}

	/**
	 * Performs the action "Add", as part of the Transaction operation
	 *
	 * @param identifier
	 * @param references
	 * @throws wcs.server.core.WCSTException
	 */
	private void actionAddCoverage(String identifier, List references) throws WCSTException
	{
		log("Executing action AddCoverage ...");

		// Obtain the references
		ReferenceType pixelsRef, descRef, summRef;

		pixelsRef = getPixelsRef(references);
		descRef = getDescriptionRef(references);
		summRef = getSummaryRef(references);

		// References check. We are adding a coverage, mandatory are: pixels, description
		if ( pixelsRef == null )
		{
			throw new WCSTException("MissingParameterValue", "Reference role='" + getUrnCode("pixels") + "'");
		}
		if ( descRef == null )
		{
			throw new WCSTException("MissingParameterValue", "Reference role='" + getUrnCode("description") + "'");
		}

		log("Loading reference: coverage pixels ...");
		BufferedImage img = loadPixelsReference(pixelsRef);

		log("Loading reference: coverage description ...");
		CoverageDescriptionType desc = loadDescriptionReference(identifier, descRef);

		CoverageSummaryType summ = null;

		if ( summRef != null )
		{
			log("Loading reference: coverage summary ...");
			summ = loadCoverageSummary(summRef);
		}

		log("Done loading references !");

		/**
		 * (1) Insert metadata
		 */
		boolean changeId = false;

		if ( MetadataUtils.existsCoverage(meta, identifier) )
		{
			changeId = true;
			log("Changing coverage identifier since coverage '" + identifier + "' already exists !");
		}

		// Generate new coverage name ?
		while (changeId)
		{
			identifier = "coverage_" + Integer.toString((new Random()).nextInt());
			changeId = MetadataUtils.existsCoverage(meta, identifier);
		}

		/**
		 * (2) Do the actual processing. Stores the image in rasdaman and then the
         * metadata in the db. 
		 *
		 *         Metadata is processed as follows:
		 *         (a) default values are inserted
		 *         (b) metadata is updated with values from CoverageDescription
		 *         (c) metadata is updated with values from CoverageSummary
		 */
		try
		{
            /* Currently we only support one-band (gray-scale) images. */
            if (img.getColorModel().getNumComponents() != 1)
                throw new WCSTException("MultiBandImagesNotSupported", "This server currently only supports one-band images (grayscale).");
			insertImageIntoRasdaman(identifier, img);
            
			insertDefaultCoverageMetadata(identifier, img);
			updateCoverageMetadataFromDescription(identifier, desc);
			/* Top level descriptions overwrite other metadata sources */
			if ( summ != null )
				updateCoverageMetadataFromSummary(identifier, summ);

            /* Check metadata validity */
            checkMetadataValidity(identifier);
		}
		catch (WCSTException e)
		{
			throw e;
		}
		catch (Exception e)
		{
			e.printStackTrace();
			throw new WCSTException("NoApplicableCode", e.getMessage());
		}

		/** 
         * (3) Indicate success: Add this ID to the output XML document
         */
		CodeType id = new CodeType();
		id.setValue(identifier);
		output.getIdentifier().add(id);
		log("Finished action Add !");
	}

	/**
	 * Performs the action "Delete", as part of the Transaction operation
	 *
	 * @param identifier
	 * @param references
	 * @throws wcs.server.core.WCSTException
	 */
	private void actionDeleteCoverage(String identifier, List references) throws WCSTException
	{
		log("Executing action Delete Coverage ...");

		// Error checking
		if ( MetadataUtils.existsCoverage(meta, identifier) == false )
		{
			throw new WCSTException("InvalidParameterValue", "Identifier");
		}

		// (2) Do the actual processing
		try
		{
			deleteCoverageFromRasdaman(identifier);
			deleteCoverageMetadata(identifier);
		}
		catch (Exception e)
		{
			throw new WCSTException(e.getMessage(), e.getStackTrace().toString());
		}

		// Indicate success: Add this ID to the output XML document
		CodeType id = new CodeType();

		id.setValue(identifier);
		output.getIdentifier().add(id);

        log("Finished action Delete !");
	}

	/**
	 * Retrieve the full URN code of a string
	 *
	 * @param key Internal representation of a URN code
	 * @return the URN code
	 */
	private String getUrnCode(String key) throws WCSTException
	{
		if ( key.equalsIgnoreCase("pixels") )
		{
			return "urn:ogc:def:role:WCS:1.1:Pixels";
		}
		if ( key.equalsIgnoreCase("description") )
		{
			return "urn:ogc:def:role:WCS:1.1:CoverageDescription";
		}
		if ( key.equalsIgnoreCase("summary") )
		{
			return "urn:ogc:def:role:WCS:1.1:CoverageSummary";
		}
		if ( key.equalsIgnoreCase("transform") )
		{
			return "urn:ogc:def:role:WCS:1.1:GeoreferencingTransformation";
		}
		if ( key.equalsIgnoreCase("other") )
		{
			return "urn:ogc:def:role:WCS:1.1:OtherSource";
		}

		throw new WCSTException("NoApplicableCode", "Unknown URN key '" + key + "'");
	}

	/**
	 * Get the Pixels Reference
	 *
	 * @param references List of References
	 * @return the Pixels Reference
	 */
	private ReferenceType getPixelsRef(List references) throws WCSTException
	{
		ReferenceType result = searchReferenceList("pixels", references);

		return result;
	}

	/**
	 * Get the Coverage Description Reference
	 *
	 * @param references List of References
	 * @return the Coverage Description Reference
	 */
	private ReferenceType getDescriptionRef(List references) throws WCSTException
	{
		ReferenceType result = searchReferenceList("description", references);

		return result;
	}

	/**
	 * Get the Coverage Summary Reference
	 *
	 * @param references List of References
	 * @return the Coverage Summary Reference
	 */
	private ReferenceType getSummaryRef(List references) throws WCSTException
	{
		ReferenceType result = searchReferenceList("summary", references);

		return result;
	}

	/**
	 * Get the Georeferencing Transform Reference
	 *
	 * @param references List of References
	 * @return the Georeferencing Transform Reference
	 */
	private ReferenceType getTransformRef(List references) throws WCSTException
	{
		ReferenceType result = searchReferenceList("transform", references);

		return result;
	}

	/**
	 * Get the "Other" Reference
	 *
	 * @param references List of References
	 * @return the "Other" Reference
	 */
	private ReferenceType getOtherRef(List references) throws WCSTException
	{
		ReferenceType result = searchReferenceList("other", references);

		return result;
	}

	/**
	 * Search a references list for a particular term
	 * @param key internal name of reference
	 * @param references List of references
	 * @return a Reference object
	 */
	private ReferenceType searchReferenceList(String key, List references) throws WCSTException
	{
		String urn = getUrnCode(key);
		Iterator i = references.iterator();

		while (i.hasNext())
		{
			Object obj = i.next();
			JAXBElement jelem = (JAXBElement) obj;
			ReferenceType ref = (ReferenceType) jelem.getValue();
			String role = ref.getRole();

			if ( role.equalsIgnoreCase(urn) )
			{
				return ref;
			}
		}

		return null;

	}

    /**
     * Check that the coverage metadata is consistent, and will not break the
     * PetaScope implementation. Throws an exception if metadata is not valid.
     *
     * This function uses code from PetaScope
     * implementation of WCPS.
     * @param identifier Name of the coverage
     * @throws WCSTException on error
     */
    private void checkMetadataValidity(String identifier) throws WCSTException
    {
        try
        {
            // Try building a metadata object.
            Metadata metadata = MetadataUtils.checkCoverageMetadata(meta, identifier);
            // Now we have a valid Metadata object, so everything is ok :-) Chillax. 
        }
        catch (WCSTException wcse)
        {
            throw wcse;
        }
        catch (Exception e)
        {
            e.printStackTrace();
            throw new WCSTException("NoApplicableCode", "Metadata is not valid " +
                    "for coverage " + identifier + ", because: " + e.getMessage());
        }
    }

	/**
	 * Panel to display loaded image
	 *
	 *
	 * @version        09.Jul 2009
	 * @author         Andrei Aiordachioaie
	 */
	public static class ShowImage extends Panel
	{
		BufferedImage image;

		public ShowImage(BufferedImage img)
		{
			this.image = img;
		}

		public void paint(Graphics g)
		{
			if ( image != null )
			{
				g.drawImage(image, 0, 0, null);
			}
		}
	}
}
