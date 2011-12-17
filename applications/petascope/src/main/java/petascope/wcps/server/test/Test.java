
import org.apache.commons.io.IOUtils;
import petascope.ConfigManager;
import petascope.core.DbMetadataSource;
import petascope.wcps.server.core.ProcessCoveragesRequest;
import petascope.wcps.server.core.WCPS;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Dimitar Misev
 */
public class Test {

    public static void main(String[] args) {
        try {
            ConfigManager.getInstance();

            System.out.println("WCPS: initializing metadata database");
            DbMetadataSource meta =
                    new DbMetadataSource(ConfigManager.METADATA_DRIVER,
                    ConfigManager.METADATA_URL,
                    ConfigManager.METADATA_USER,
                    ConfigManager.METADATA_PASS, false);
            System.out.println("WCPS: initializing WCPS core");
            WCPS wcps = new WCPS(meta);
            
            String query = "";
            String xmlRequest = ProcessCoveragesRequest.abstractQueryToXmlQuery(query);
            ProcessCoveragesRequest pc = wcps.pcPrepare(
                    ConfigManager.RASDAMAN_URL, ConfigManager.RASDAMAN_DATABASE,
                    IOUtils.toInputStream(xmlRequest));
            String rasql = pc.getRasqlQuery();
            System.out.println("DONE");
            System.out.println("rasql: " + rasql);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}
