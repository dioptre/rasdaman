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
package petascope.wps.n52;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.n52.wps.io.data.IData;
import org.n52.wps.io.data.binding.complex.PlainStringBinding;
import org.n52.wps.io.data.binding.complex.ArrayDataBinding;
import org.n52.wps.io.data.binding.literal.LiteralStringBinding;
import org.n52.wps.server.AbstractAlgorithm;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import petascope.ConfigManager;
import petascope.core.DbMetadataSource;
import petascope.exceptions.RasdamanException;
import petascope.exceptions.WCPSException;
import petascope.util.ListUtil;
import petascope.util.ras.RasQueryResult;
import petascope.util.ras.RasUtil;
import petascope.wcps.server.core.WCPS;

/**
 *
 * @author Dimitar Misev
 */
public class ProcessCoverages extends AbstractAlgorithm {

    private static Logger log = LoggerFactory.getLogger(ProcessCoverages.class);
    
    private String inputIDQuery = "Query";
    private String outputIDCoverageList = "CoverageList";
    private String outputIDScalarList = "ScalarList";
    private List<String> errors;
    
    private WCPS wcps;
    private DbMetadataSource meta;

    public ProcessCoverages() {
        super();
        errors = new ArrayList<String>();

        try {
            log.info("WCPS: initializing metadata database");
            ConfigManager.getInstance();
            meta = new DbMetadataSource(ConfigManager.METADATA_DRIVER,
                    ConfigManager.METADATA_URL,
                    ConfigManager.METADATA_USER,
                    ConfigManager.METADATA_PASS, false);

            log.debug("WCPS: initializing WCPS core");
            wcps = new WCPS(meta);

            log.info("WCPS: initialization complete");
        } catch (Exception e) {
            log.error("Stack trace: {}", e);
            throw new RuntimeException("WCPS initialization error", e);
        }
    }

    @Override
    public List<String> getErrors() {
        return errors;
    }

    @Override
    public Class<?> getInputDataType(String id) {
        if (id.equals(inputIDQuery)) {
            return PlainStringBinding.class;
        }
        return null;
    }

    @Override
    public Class<?> getOutputDataType(String id) {

        if (id.equals(outputIDCoverageList)) {
            return ArrayDataBinding.class;
        } else if (id.equals(outputIDScalarList)) {
            return LiteralStringBinding.class;
        }
        return null;
    }

    @Override
    public Map<String, IData> run(Map<String, List<IData>> inputData) {
        errors.clear();

        List<IData> queryDataList = inputData.get(inputIDQuery);
        if (queryDataList == null || queryDataList.size() != 1) {
            errors.add("Error while allocating input parameters");
            throw new RuntimeException("Error while allocating input parameters");
        }
        String query = ((PlainStringBinding) queryDataList.get(0)).getPayload().toString();
        Object wcpsResult = null;
        try {
            wcpsResult = RasUtil.executeWcpsQuery(query, wcps);
        } catch (WCPSException ex) {
            errors.add(ex.getMessage());
            throw new RuntimeException(ex);
        } catch (RasdamanException ex) {
            errors.add(ex.getMessage());
            throw new RuntimeException(ex);
        }

        HashMap<String, IData> ret = new HashMap<String, IData>();
        RasQueryResult res = new RasQueryResult(wcpsResult);
        if (!res.getMdds().isEmpty()) {
            ret.put(outputIDCoverageList, new ArrayDataBinding(res.getMdds()));
        }
        if (!res.getScalars().isEmpty()) {
            String scalars = ListUtil.ltos(res.getScalars(), ",");
            ret.put(outputIDScalarList, new LiteralStringBinding(scalars));
        }

        return ret;
    }
}