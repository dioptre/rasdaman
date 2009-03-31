package wcs.server.core;

import java.io.File;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Stack;
import net.opengis.gml.v_3_1_1.TimePositionType;
import net.opengis.ows.v_1_0_0.BoundingBoxType;
import net.opengis.wcs.v_1_1_0.DomainSubsetType;
import net.opengis.wcs.v_1_1_0.GetCoverage;
import net.opengis.wcs.v_1_1_0.GridCrsType;
import net.opengis.wcs.v_1_1_0.RangeSubsetType;
import net.opengis.wcs.v_1_1_0.TimePeriodType;
import wcs.server.core.SliceCoverageExprType.SetInterpolationDefault;


/**
 * This class takes a WCS GetCoverage XML request and converts this request into
 * a WCPS ProcessCoverage XML request. It is the user's responsibility to pass
 * the WCPS request to a WCPS server in order to view to result.
 * NOTE: This class was tested with the WCPS server developed at Jacobs University,
 * with a web client available at http://kahlua.eecs.jacobs-university.de/wcps-maitai/.
 *
 * @author Andrei Aiordachioaie
 */
public class convertGetCoverage{
    
    private GetCoverage wcs;
    private ProcessCoverage wcps;
    private EncodeDataExprType encoding;
    private Stack<Object> stack;
    private boolean finished;
    private static String varname = "var";
    private static boolean printLog = false;
    private String pathDbSettings;		// Path to the "dbparams.properties" file
    private MetadataDb meta;

    /**
     * Record a log message to System.out
     * @param str Text of the message
     */
    static private void log(String str)
    {
        if (printLog)
            System.out.println(str);
    }

    /**
     * Default constructor
     * @param cov GetCoverage object, the input WCS request for retrieving a coverage
	 * @param metadataDbPath Path to the "dbparams.properties" file
     */
    public convertGetCoverage(GetCoverage cov, String MetadataDbPath) throws WCSException
    {
        wcs = cov;
        finished = false;
        wcps = new ProcessCoverage();
        encoding = new EncodeDataExprType();
        stack = new Stack<Object>();
		pathDbSettings = MetadataDbPath;
        meta = new MetadataDb(pathDbSettings);
    }
    
    /*** Main method of this class. Retrieves the WCPS request object that is 
     * equivalent to the WCS request. If it is not yet converted, it will 
     * perform the conversion.
     * 
     * @return the ProcessCoverage XML structure for WCPS
     * @throws wcs_web_service.WCSException
     */
    public ProcessCoverage get() throws WCSException 
    {
        try
        {
            if (!finished)
                process();
        }
        catch (WCSException e)
        {
            System.err.println("ERROR ! Could not convert the WCS xml into WCPS xml !!!");
            e.printStackTrace();
            throw e;
        }
        catch (Exception e)
        {
            System.err.println("Runtime ERROR ! ... see below!");
            e.printStackTrace();
        }
        
        return wcps;
    }

    /**
     * Converts WCS node 1 (Coverage Identifier) to WCPS info
     * @throws wcs_web_service.WCSException
     */
    private void convField1() throws WCSException
    {
        if (! wcs.isSetIdentifier())
            throw new WCSException("MissingParameterValue", "Identifier", 
                    "Identifier must be a child of GetCoverage !");
        wcps.coverageIterator = new ArrayList<ProcessCoverage.CoverageIterator>();
        ProcessCoverage.CoverageIterator var = new ProcessCoverage.CoverageIterator();
        var.setIteratorVar(varname);
        var.coverageName = new ArrayList<String>();
        String cname = wcs.getIdentifier().getValue();
        var.coverageName.add(cname);
        wcps.coverageIterator.add(var);
    }
    
    /**
     * Converts WCS node 2 (Domain subsetting) to WCPS info
     * @throws wcs_web_service.WCSException
     */
    private void convField2() throws WCSException
    {
        if (! wcs.isSetDomainSubset())
            throw new WCSException("MissingParameterValue", "DomainSubset",
                    "DomainSubset must be a child element of GetCoverage !");
        if (! wcs.getDomainSubset().isSetBoundingBox())          
            throw new WCSException("MissingParameterValue", "BoundingBox",
                    "BoundingBox must be a child element of DomainSubset !");
        
        DomainSubsetType domain = wcs.getDomainSubset();
        // DomainSubset->BoundingBox
        if (domain.isSetBoundingBox())
        {
            BoundingBoxType bbox = (BoundingBoxType) domain.getBoundingBox().getValue();
            if (bbox.getLowerCorner().size() != 2)
                throw new WCSException("InvalidParameterValue", "LowerCorner", 
                        "BoundingBox -> LowerCorner should have exactly two " +
                        "values, not " + bbox.getLowerCorner().size());
            if (bbox.getUpperCorner().size() != 2)
                throw new WCSException("InvalidParameterValue", "UpperCorner",
                        "BoundingBox -> UpperCorner should have exactly two " +
                        "values, not " + bbox.getUpperCorner().size());
                        
            // Set up trimming along X axis
            TrimCoverageExprType exprx = new TrimCoverageExprType();
            exprx.trimAxisSpec = new ArrayList<TrimCoverageExprType.TrimAxisSpec>();
            TrimCoverageExprType.TrimAxisSpec xaxis = new TrimCoverageExprType.TrimAxisSpec();
            xaxis.axis = "x";

            int strx1 = bbox.getLowerCorner().get(0).intValue();
            int strx2 = bbox.getUpperCorner().get(0).intValue();
            xaxis.getCoordinateType().add(strx1);
            xaxis.getCoordinateType().add(strx2);
            exprx.getTrimAxisSpec().add(xaxis);
            
            // Add the X-axis trimming to the stack
            stack.push(exprx);
//            log("WCPS>>> Added X-axis trimming ! (DomainSubset->BoundingBox)" + exprx.trimAxisSpec.get(0).getAxis());
            
            // Set up trimming along Y axis
            TrimCoverageExprType expry = new TrimCoverageExprType();
            expry.trimAxisSpec = new ArrayList<TrimCoverageExprType.TrimAxisSpec>();
            TrimCoverageExprType.TrimAxisSpec yaxis = new TrimCoverageExprType.TrimAxisSpec();
            yaxis.axis = "y";

            Integer stry1 = bbox.getLowerCorner().get(1).intValue();
            Integer stry2 = bbox.getUpperCorner().get(1).intValue();
            yaxis.getCoordinateType().add(stry1);
            yaxis.getCoordinateType().add(stry2);
            expry.getTrimAxisSpec().add(yaxis);

            // Add Y-axis trimming to the stack
            stack.push(expry);
//            log("WCPS>>> Added Y-axis trimming ! (DomainSubset->BoundingBox)" + expry.trimAxisSpec.get(0).getAxis());
            
            // If BoundingBox CRS is defined, then we need to convert the image to the specified CRS first,
            // before doing the trimming
            // NOTE! This assumes that the bounding box always spans axis "x" and "y". Check this !
            log("WCPS>>> Bounding Box CRS is : " + bbox.getCrs());
            if (bbox.getCrs() != null)
            {
                CrsTransformCoverageExprType tr = new CrsTransformCoverageExprType();
                CrsTransformCoverageExprType.CrsTransformAxisSpec axis1 = new CrsTransformCoverageExprType.CrsTransformAxisSpec();
                CrsTransformCoverageExprType.CrsTransformAxisSpec axis2 = new CrsTransformCoverageExprType.CrsTransformAxisSpec();
                axis1.setAxis("x");
                axis1.setCrs(bbox.getCrs());
                axis2.setAxis("y");
                axis2.setCrs(bbox.getCrs());
                tr.getCrsTransformAxisSpec().add(axis1);
                tr.getCrsTransformAxisSpec().add(axis2);
                // NOTE that since the fields are not known in advance, this "transform field spec" is left empty
                CrsTransformCoverageExprType.CrsTransformFieldSpec field = new CrsTransformCoverageExprType.CrsTransformFieldSpec();
                tr.getCrsTransformFieldSpec().add(field);
                stack.push(tr);
            }
        }
        // DomainSubset->TemporalSubset
        if (domain.isSetTemporalSubset())
        {
            // TemporalSubset  is of type TimeSequenceType = choice(gml:TimePosition, wcs:TimePeriodType)
            Object one = domain.getTemporalSubset().getTimePositionOrTimePeriod().get(0);
            log("Inside TemporalSubset there is " + one.getClass());
            if (one instanceof net.opengis.gml.v_3_1_1.TimePositionType)
            {
                // TemporalSubset = gml:TimePosition
                // use WCPS:slice
                SliceCoverageExprType expr = new SliceCoverageExprType();
                expr.sliceAxisSpec = new ArrayList<SliceCoverageExprType.SliceAxisSpec>();
                SliceCoverageExprType.SliceAxisSpec axis = new SliceCoverageExprType.SliceAxisSpec();
                axis.axis = "time";
                
                TimePositionType pos = (TimePositionType) one;
                log("TimePosition has length " + pos.getValue().size());
                if (pos.getValue().size() != 1)
//                    throw new Exception("ERROR ! The TimePosition element should have exactly one item!");
                    throw new WCSException("InvalidParameterValue", "TimePosition", "ERROR ! The TimePosition element should have exactly one item, and not " + pos.getValue().size());
                String str = pos.getValue().get(0);
                axis.setDomainCoord(str);
                
                // Add this slicing expression to the stack
                expr.sliceAxisSpec.add(axis);
                stack.push(expr);
//                log("WCPS>>> Added time-axis slicing ! ( DomainSubset->TemporalSubset->gml:TimePositionType)" + expr.sliceAxisSpec.get(0).getAxis());
            }
            else
            if (one instanceof net.opengis.wcs.v_1_1_0.TimePeriodType)
            {
                // TemporalSubset = wcs:TimePeriodType
                // use WCPS:trim
                TrimCoverageExprType expr = new TrimCoverageExprType();
                expr.trimAxisSpec = new ArrayList<TrimCoverageExprType.TrimAxisSpec>();
                TrimCoverageExprType.TrimAxisSpec timeaxis = new TrimCoverageExprType.TrimAxisSpec();
                timeaxis.axis = "time";

                TimePeriodType period = (TimePeriodType) one;
                
                TimePositionType pos1 = period.getBeginPosition();
                TimePositionType pos2 = period.getEndPosition();
                log("TimePeriod -> Start position, has length " + pos1.getValue().size());
                log("TimePeriod -> End position, has length " + pos2.getValue().size());
                if (pos1.getValue().size() != 1 || pos2.getValue().size() != 1)
//                    throw new Exception("ERROR ! The TimePeriod->TimePosition element should have exactly one item!");
                    throw new WCSException("InvalidParameterValue", "TimePosition", 
                            "ERROR ! The TimePeriod->TimePosition element should have exactly one item !");
                String str1 = pos1.getValue().get(0);
                String str2 = pos2.getValue().get(0);
                timeaxis.getCoordinateType().add(str1);
                timeaxis.getCoordinateType().add(str2);
                expr.trimAxisSpec.add(timeaxis);

                // Add this trimming expression to the stack
                stack.push(expr);
//                log("WCPS>>> Added time-axis trimming ! ( DomainSubset->TemporalSubset->wcs:TimePeriodType)"  + expr.trimAxisSpec.get(0).getAxis());
            }

        }

        
    }
    
    /**
     * Converts WCS node 3 (Range subsetting) to WCPS info
     */
    private void convField3() throws WCSException
    {

        if (wcs.isSetRangeSubset())
        {
            RangeSubsetType range = wcs.getRangeSubset();
            // combine several fields of the coverage together
            CombineRangeCoverageExprType combination = new CombineRangeCoverageExprType();
            
            Iterator<RangeSubsetType.FieldSubset> it = range.getFieldSubset().iterator();
            while (it.hasNext())
            {
                RangeSubsetType.FieldSubset field = it.next();
                CombineRangeCoverageExprType.Component component = new CombineRangeCoverageExprType.Component();
                component.setField(field.getIdentifier().getValue());
                component.setCoverage(varname);
                log("WCPS>>> RangeSubsetType->FieldSubset->Identifier is " + component.getField());
                log("WCPS>>> RangeSubsetType->FieldSubset->Interpolation is " + field.getInterpolationType());
                // Set the medatada for interpolating current field
                SetInterpolationDefault interp = new SetInterpolationDefault();
                interp.setField(component.getField());

                // Set up default values for interpolation method and null resistance
                interp.setInterpolationMethod("nearest neighbor");
                interp.setNullResistance("full");
                // Get null resistance from the DB for the current coverage, if available
                String query = "SELECT c.name, n.nullresistance FROM wcps_coverage AS c " +
                        "JOIN wcps_nullresistance AS n ON n.id = nullresistancedefault " +
                        "WHERE c.name = '" + wcs.getIdentifier().getValue() + "'";
                log(query);
                try
                {
                    ResultSet r = meta.executeQuery(query);
                    if (r.next())
                    {
                        String res = r.getString("nullresistance");
                        interp.setNullResistance(res);
                        log(" *** Null resistance = " + res);
                    }
                }
                catch (SQLException se)
                {
                    log("Error fetching default null resistance for coverage. See query below:");
                    log("SQL Error in query:\n " + query);
                }

                // Get default interpolation method from DB, if available
                query = "SELECT im.interpolationmethod FROM wcps_coverage AS c " +
                        "JOIN wcps_interpolationmethodlist AS iml ON iml.coverage = c.id " +
                        "JOIN wcps_interpolationmethod AS im ON iml.interpolationmethod = im.id " +
                        "WHERE c.name = '" + wcs.getIdentifier().getValue() + "'";
                log(query);
                try
                {
                    ResultSet r = meta.executeQuery(query);
                    if (r.next())
                    {
                        String inttype = r.getString("interpolationmethod");
                        interp.setInterpolationMethod(inttype);
                        log(" *** Interpolation type = " + inttype);
                    }
                }
                catch (SQLException se)
                {

                    log("Error fetching default interpolation method for coverage. See query below:");
                    log("SQL Error in query:\n " + query);
                }

                // Add the interpolation details
//                stack.push(interp);
                
                // FieldSubset->AxisSubset is optional, and will not be translated here

                // Select this field too in the combination (of fields of a given coverage)
                combination.getComponent().add(component);
            }
            
            stack.push(combination);
        }
        
    }
    
    /**
     * Converts WCS node 4 (Output) to WCPS info
     * @throws wcs_web_service.WCSException
     */
    private void convField4() throws WCSException
    {
        String query = "";
        if (! wcs.isSetOutput())
            throw new WCSException("MissingParameterValue", "Output",
                    "Output must be a child of GetCoverage !");

        if (wcs.getOutput().isSetGridCRS())
        {
            GridCrsType crs = wcs.getOutput().getGridCRS();
            throw new WCSException("NoApplicableCode", 
                    "Currently, the Output->GridCRS node is not supported !",
                    "Currently, the Output->GridCRS node is not supported !");
        }

        // encoding is initialized in convField2()
        try
        {
            File pwd = new File(".");
            String wcsFormat = wcs.getOutput().getFormat();
            query = "SELECT name FROM wcps_format WHERE mimetype='" + wcsFormat + "'";
            ResultSet r = meta.executeQuery(query);
            if (r.next())
            {
                String ename = r.getString("name");
                // Small hack: rasdaman does not recognize jpg, only jpeg
                if (ename.equals("jpg"))
                    ename = "jpeg";
                encoding.setFormat(ename);
                log("*** Output format is " + encoding.getFormat());
            }
            else
            {
                // error fetching the mimetype from the db ... try using the mimetype directly
                encoding.setFormat(wcs.getOutput().getFormat());
                log("Error fetching the image type from the database ... Continuing ...");
            }
        }
        catch (SQLException se)
        {
            encoding.setFormat(wcs.getOutput().getFormat());
            log("SQL Error while trying to find mimetype '" + wcs.getOutput().getFormat() + "' in the database! Continuing ...");
            throw new WCSException("NoApplicableCode",
                    "SQL Exception on the following query: " + query,
                    "SQL Exception on the following query: " + query);
        }
        catch (Exception e)
        {
            encoding.setFormat(wcs.getOutput().getFormat());
            log("Strange error while trying to find mimetype '" + wcs.getOutput().getFormat() + "' in the database! Continuing ...");
            throw new WCSException("NoApplicableCode",
                    "Exception with the following message: " + e.getMessage(),
                    "Exception with the following message: " + e.getMessage());
        }

        log("issetstore = " + wcs.getOutput().isSetStore());
        log("isstore = " + wcs.getOutput().isStore());
        if (wcs.getOutput().isSetStore())
        {
            if (wcs.getOutput().isStore())
                encoding.setStore(true);
            else
                encoding.setStore(false);
        }
        else
            encoding.setStore(false);
    }
    
    /**
     * Performs the conversion of the WCS request into a WCPS request.
     * @throws wcs_web_service.WCSException
     */
    @SuppressWarnings("static-access")
    public void process() throws WCSException
    {
        /*** Processing starts here ... with the nodes of the WCS ***/
        // Service Description
        wcps.setService("WCPS");
        wcps.setVersion("1.0.0");
        log("WCS service: \"" + wcs.SERVICE + "\"");
        log("WCS version: \"" + wcs.VERSION + "\"");
        if ( !wcs.SERVICE.equalsIgnoreCase("WCS"))
                throw new WCSException("InvalidParameterValue", "Service", "Only the WCS service is supported !!!");
        if ( !wcs.VERSION.equals("1.1.0") && ( !wcs.VERSION.equals("1.0.0")))
            throw new WCSException("InvalidParameterValue", "Version", "Only Version 1.1.0 is currently supported !!!");

        // First of all, error checking: is the coverage offered by the server?
        if (!existsCoverage(wcs.getIdentifier().getValue()))
            throw new WCSException("InvalidParameterValue", "Identifier",
                    "Coverage " + wcs.getIdentifier().getValue() + " is not served by this server !");

        // Convert all the child nodes of WCS GetCoverage XML node
        log("*** Going to build WCPS node 1 ...");
        convField1();
        log("*** Going to build WCPS node 2 ...");
        convField2();
        log("*** Going to build WCPS node 3 ...");
        convField3();
        log("*** Going to build WCPS node 4 ...");
        convField4();
        log("*** Last touches to the WCPS XML structure ...");
        buildWcpsStructure();
        log("*** Done with building the WCPS XML !");

        wcps.setEncode(encoding);

        /* Done processing */
        finished = true;
        log(">>> Done Converting WCS GetCoverage into WCPS ProcessCoverage...");
    }

    /*** Now we have a list of operations that we want the WCPS to perform,
     * stored in "stack". We need to add the variable name to the structure on 
     * top of the stack, and then we can build the structure to pass to WCPS.
     * NOTE: This function should be called after all details of the WCS request
     * have been processed.
     * 
     * @throws wcs_web_service.WCSException
     */
    private void buildWcpsStructure() throws WCSException
    {
        
        // Print the stack
        log(" ---------  STACK BOTTOM ----------");
        Iterator<Object> i = stack.iterator();
        while (i.hasNext())
        {
            Object o = i.next();
            if (o instanceof TrimCoverageExprType)
            {
                TrimCoverageExprType trim = (TrimCoverageExprType) o;
                log(" - Trim " + trim.trimAxisSpec.get(0).getAxis());
            }
            else if (o instanceof SliceCoverageExprType)
            {
                SliceCoverageExprType slice = (SliceCoverageExprType) o;
                log(" - Slice " + slice.sliceAxisSpec.get(0).getAxis());
            }
            else if (o instanceof CrsTransformCoverageExprType)
            {
                CrsTransformCoverageExprType tr = (CrsTransformCoverageExprType) o;
                log(" - CRS Transform into crs " + tr.getCrsTransformAxisSpec().get(0).getCrs());
            }
            else if (o instanceof CombineRangeCoverageExprType)
            {
                CombineRangeCoverageExprType comb = (CombineRangeCoverageExprType) o;
                log(" - Combination between several fields: ");
                Iterator<CombineRangeCoverageExprType.Component> it = comb.getComponent().iterator();
                while (it.hasNext())
                {
                    log(" \t * Component " + it.next().getField());
                }
            }
            else if (o instanceof SetInterpolationDefault)
            {
                SetInterpolationDefault interp = (SetInterpolationDefault) o;
                log(" - Interpolation Default for field " + interp.getField() + ": interp=" 
                        + interp.getInterpolationMethod() + ", null resistance=" + interp.getNullResistance());
            }
        }
        log(" ---------   STACK TOP   ----------");
        
        // Add the variable name to the top of the stack
        Object obj, newObj;
        TrimCoverageExprType trim = null, newTrim;
        SliceCoverageExprType slice = null, newSlice;
        CrsTransformCoverageExprType tr = null, newTr;
        CombineRangeCoverageExprType comb = null, newComb;
        SetInterpolationDefault interp = null, newInterp;
        
        obj = stack.pop();
        if (obj instanceof TrimCoverageExprType)
        {
            trim = (TrimCoverageExprType)obj;
            trim.setCoverage(varname);
            log("WCPS>>> (1) Popped a trim expression from stack !" + trim.trimAxisSpec.get(0).getAxis());
        }
        else
        if (obj instanceof SliceCoverageExprType)
        {
            slice = (SliceCoverageExprType)obj;
            slice.setCoverage(varname);
            log("WCPS>>> (1) Popped a slice expression from stack !" + slice.sliceAxisSpec.get(0).getAxis());
        }
        else 
        if (obj instanceof CrsTransformCoverageExprType)
        {
            tr = (CrsTransformCoverageExprType) obj;
            tr.setCoverage(varname);
            log("WCPS>>> (1) CRS Transform into crs " + tr.getCrsTransformAxisSpec().get(0).getCrs());
        }
        else
        if (obj instanceof CombineRangeCoverageExprType)
        {
            comb = (CombineRangeCoverageExprType) obj;
            // Variable name already set in convField3()
            log("WCPS>>> (1) Combining different range fields for thing with first element " 
                    + comb.getComponent().get(0).getField() + ". HOWTO !!!??? Don't know how to work with this !!!");
            log("WCPS>>> Component has coverage " + comb.getComponent().get(0).getCoverage() + "." + comb.getComponent().get(0).getField());
        }
        else 
        if (obj instanceof SetInterpolationDefault)
        {
            interp = (SetInterpolationDefault) obj;
            interp.setCoverage(varname);
            log("WCPS>>> (1) Interpolation Default for field " + interp.getField() 
                    + ": interp=" + interp.getInterpolationMethod() 
                    + ", null resistance=" + interp.getNullResistance());
        }
        else
            throw new WCSException("NoApplicableCode", "", 
                    "INTERNAL ERROR! Unknown datatype while popping stack !");
        // Pop the stack element by element, to construct the expression
        while (! stack.empty())
        {
            newSlice = null;
            newTrim = null;
            newTr = null;
            newComb = null;
            newInterp = null;
            newObj = stack.pop();
            if (newObj instanceof TrimCoverageExprType)
            {
                newTrim = (TrimCoverageExprType)newObj;
                log("WCPS>>> Popped a trim expression from stack !" + newTrim.trimAxisSpec.get(0).getAxis());
                if (trim != null)
                    newTrim.setTrim(trim);
                else
                if (slice != null)
                    newTrim.setSlice(slice);
                else
                if (tr != null)
                    newTrim.setCrsTransform(tr);
                else
                if (comb != null)
                    newTrim.setCombine(comb);
                else
                if (interp != null)
                    newTrim.setSetInterpolationDefault(interp);
            }
            else
            if (newObj instanceof SliceCoverageExprType)
            {
                newSlice = (SliceCoverageExprType)newObj;
                log("WCPS>>> Popped a slice expression from stack !" + newSlice.sliceAxisSpec.get(0).getAxis());
                if (trim != null)
                    newSlice.setTrim(trim);
                else
                if (slice != null)
                    newSlice.setSlice(slice);
                else
                if (tr != null)
                    newSlice.setCrsTransform(tr);
                else
                if (comb != null)
                    newSlice.setCombine(comb);
                else
                if (interp != null)
                    newSlice.setSetInterpolationDefault(interp);
            }
            else
            if (newObj instanceof CrsTransformCoverageExprType)
            {
                newTr = (CrsTransformCoverageExprType) newObj;
                log("WCPS>>> Popped a CRS Transform into crs " + newTr.getCrsTransformAxisSpec().get(0).getCrs());
                if (trim != null)
                    newTr.setTrim(trim);
                else
                if (slice != null)
                    newTr.setSlice(slice);
                else
                if (tr != null)
                    newTr.setCrsTransform(tr);
                else
                if (comb != null)
                    newTr.setCombine(comb);
                else
                if (interp != null)
                    newTr.setSetInterpolationDefault(interp);
            }
            else
            if (newObj instanceof CombineRangeCoverageExprType)
            {
                newComb = (CombineRangeCoverageExprType) newObj;
                if (trim != null)
                    newComb.getComponent().get(0).setTrim(trim);
                else
                if (slice != null)
                    newComb.getComponent().get(0).setSlice(slice);
                else
                if (tr != null)
                    newComb.getComponent().get(0).setCrsTransform(tr);
                else
                if (comb != null)
                    newComb.getComponent().get(0).setCombine(comb);
                else
                if (interp != null)
                    newComb.getComponent().get(0).setSetInterpolationDefault(interp);
                
                log("WCPS>>> Combining different range fields for thing with first element " 
                    + newComb.getComponent().get(0).getField() + ". Don't know how to work with this !!!");
            }
            else 
            if (newObj instanceof SetInterpolationDefault)
            {
                newInterp = (SetInterpolationDefault) newObj;
                log("WCPS>>> Interpolation Default for field " + newInterp.getField() 
                        + ": interp=" + newInterp.getInterpolationMethod() 
                        + ", null resistance=" + newInterp.getNullResistance());
                if (trim != null)
                    newInterp.setTrim(trim);
                else
                if (slice != null)
                    newInterp.setSlice(slice);
                else
                if (tr != null)
                    newInterp.setCrsTransform(tr);
                else
                if (comb != null)
                    newInterp.setCombine(comb);
                else
                if (interp != null)
                    newInterp.setSetInterpolationDefault(interp);
            }
            
            else
                throw new WCSException("NoApplicableCode", "", 
                    "INTERNAL ERROR! Unknown datatype while popping stack !");

            slice = newSlice;
            trim = newTrim;
            tr = newTr;
            comb = newComb;
            interp = newInterp;
        }
        
        // Add the structure represented on the stack to the class variable "encoding"
        if (trim != null)
            encoding.setTrim(trim);
        else
        if (slice != null)
            encoding.setSlice(slice);
        else
        if (tr != null)
            encoding.setCrsTransform(tr);
        else
        if (comb != null)
            encoding.setCombine(comb);
        else
        if (interp != null)
            encoding.setSetInterpolationDefault(interp);
        else
            throw new WCSException("NoApplicableCode", "", "ERROR ! Unknown " +
                    "datatype after processing the whole stack !");
        
        log("WCPS>>> Added the final processing structure to the XML tree... ");        
    }

    /**
     * Checks the metadata DB to ,ake sure that the WCPS server has coverage "name"
     * @param name the name of the coverage to check
     * @return boolean whether the coverage is available
     */
    private boolean existsCoverage(String name)
    {
        
        String query = "SELECT * FROM wcps_coverage AS cov " +
                "WHERE cov.name='" + name + "'";
        try
        {
            ResultSet r = meta.executeQuery(query);
            log(query);
            if (r.next())
            {
                log("Coverage " + name + " is in the database !");
            }
            else
            {
                log("Coverage " + name + " is NOT in the database !");
                return false;
            }
        }
        catch (Exception e)
        {
            return false;
        }

        return true;
    }
    
}
