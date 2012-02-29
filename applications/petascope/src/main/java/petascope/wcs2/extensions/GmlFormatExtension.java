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
package petascope.wcs2.extensions;

import java.math.BigInteger;
import java.util.List;
import java.util.ListIterator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.core.DbMetadataSource;
import petascope.core.Metadata;
import petascope.exceptions.PetascopeException;
import petascope.exceptions.WCSException;
import petascope.util.Pair;
import petascope.util.WcsUtil;
import petascope.util.ras.RasQueryResult;
import petascope.wcps.server.core.CellDomainElement;
import petascope.wcs2.handlers.Response;
import petascope.wcs2.parsers.GetCoverageRequest;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionSlice;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionSubset;
import petascope.wcs2.parsers.GetCoverageRequest.DimensionTrim;
import petascope.wcs2.parsers.GetCoverageMetadata;
import petascope.wcs2.templates.Templates;

/**
 * Return coverage as pure GML.
 *
 * @author <a href="mailto:d.misev@jacobs-university.de">Dimitar Misev</a>
 */
public class GmlFormatExtension extends AbstractFormatExtension {

    private static final Logger log = LoggerFactory.getLogger(GmlFormatExtension.class);
    public static final String DATATYPE_URN_PREFIX = "urn:ogc:def:dataType:OGC:1.1:";
    protected static final String MULTIPOINTSCHEMA = "ps_multipoint";

    @Override
    public boolean canHandle(GetCoverageRequest req) {
        return req.getFormat() == null || (!req.isMultipart() && MIME_GML.equals(req.getFormat()));
        //return false;
    }

    @Override
    public Response handle(GetCoverageRequest request, DbMetadataSource meta)
            throws WCSException {
        GetCoverageMetadata m = new GetCoverageMetadata(request, meta);
        
        if (m.getCoverageType().equals(GetCoverageRequest.MULTIPOINT_COVERAGE)) {
            return handleMultiPoint(request, request.getCoverageId(), meta, m);
        }
        
        setBounds(request, m, meta);
        String gml = WcsUtil.getGML(m, Templates.GRID_COVERAGE, true);
        gml = addCoverageData(gml, request, meta, m);
        
        return new Response(gml);
    }
    
    protected String addCoverageData(String gml, GetCoverageRequest request, DbMetadataSource meta, GetCoverageMetadata m) throws WCSException {
        RasQueryResult res = new RasQueryResult(executeRasqlQuery(request, m, meta, CSV_ENCODING, null).fst);
        if (!res.getMdds().isEmpty()) {
            String data = new String(res.getMdds().get(0));
            data = WcsUtil.csv2tupleList(data);
            data = "<DataBlock><rangeParameters/><tupleList>" + data + "</tupleList></DataBlock>";
            gml = gml.replace("{coverageData}", data);
        }
        return gml;
    }

    /**
     * Handles a request for MultiPoint Coverages and returns a response XML
     * @param req
     * @param coverageID
     * @return
     * @throws WCSException
     */
    private Response handleMultiPoint(GetCoverageRequest req, String coverageID, DbMetadataSource meta, GetCoverageMetadata m)
            throws WCSException {
        Metadata cov = m.getMetadata();
        String ret = WcsUtil.getGML(m, Templates.MULTIPOINT_COVERAGE, false);
        String pointMembers = "";
        String low = "", high = "";

        try {

            List<CellDomainElement> cellDomainList = cov.getCellDomainList();

            /* check for subsetting */
            List<DimensionSubset> subsets = req.getSubsets();
            if (!subsets.isEmpty()) {

                /* subsetting ON: get coverage metadata */
                ListIterator<DimensionSubset> listIterator = subsets.
                        listIterator();
                while (listIterator.hasNext()) {

                    DimensionSubset subsetElement = listIterator.next();
                    String dimension = subsetElement.getDimension();
                    int dimIndex = cov.getDomainIndexByName(dimension);
                    CellDomainElement cellDomain = cellDomainList.get(dimIndex);

                    if (subsetElement instanceof DimensionTrim) {
                        DimensionTrim trim = (DimensionTrim) subsetElement;
                        cellDomain.setHi(new BigInteger(trim.getTrimHigh()));
                        cellDomain.setLo(new BigInteger(trim.getTrimLow()));
                        cellDomain.setSubsetElement(subsetElement);
                    }

                    if (subsetElement instanceof DimensionSlice) {
                        DimensionSlice slice = (DimensionSlice) subsetElement;
                        cellDomain.setHi(new BigInteger(slice.getSlicePoint()));
                        cellDomain.setLo(new BigInteger(slice.getSlicePoint()));
                        cellDomain.setSubsetElement(subsetElement);
                    }
                }
            }

            /* iterate the coverage metadata */
            for (CellDomainElement aCellDomainList : cellDomainList) {
                low += aCellDomainList.getLo() + " ";
                high += aCellDomainList.getHi() + " ";
            }

            /* get coverage data */
            pointMembers = meta.coverageData(MULTIPOINTSCHEMA, coverageID, req.
                    getCoverageId(), cellDomainList);
            Pair<String, String> pair = constructWcpsQuery(req, cov, CSV_ENCODING, null);

            /* generate the result */
            ret = ret.replaceAll("\\{pointMembers\\}", pointMembers).replaceAll("\\{low\\}", low).
                    replaceAll("\\{high\\}", high).replaceAll("\\{axisLabels\\}", pair.snd).
                    replaceAll("\\{mulUomLabels\\}", pair.snd);

        } catch (PetascopeException ex) {
            log.error("Error", ex);
        }

        return new Response(ret);
    }

    @Override
    public String getExtensionIdentifier() {
        return ExtensionsRegistry.ENCODING_IDENTIFIER;
    }
}
