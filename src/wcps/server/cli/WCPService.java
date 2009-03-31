package wcps.server.cli;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.util.Iterator;
import java.util.List;
import java.util.Properties;

import wcps.server.core.CachedMetadataSource;
import wcps.server.core.DbMetadataSource;
import wcps.server.core.ProcessCoverageRequest;
import wcps.server.core.WCPS;

// This is a small application around the WCPS core. It takes XML requests as files and runs them through WCPS. The resulting radaman queries are displayed, but not executed. This is very useful for testing whether your metadata is valid.

public class WCPService {

    private static WCPS wcps;

    public static void main( String[] args ) {

        if( args.length != 1 ) {
            System.err.println( "WCPS: no input files" );
            System.exit( 1 );
        }

        String pcSchemaFileName = ".." + File.separator + "xml" + File.separator + "ogc" + File.separator + "wcps" + File.separator + "1.0.0" + File.separator + "wcpsProcessCoverage.xsd";
        File pcSchemaFile = new File( pcSchemaFileName );
        if( !pcSchemaFile.exists() ) {
            System.err.println( "WCPS: could not find the WCPS ProcessCoverage schema (" + pcSchemaFileName + ")" );
            System.exit( 1 );
        }

        DbMetadataSource metadataSource = null;
        try {
            Properties dbParams = new Properties();
            dbParams.load( new FileInputStream( ".." + File.separator + "dbparams.properties" ) );
            metadataSource = new DbMetadataSource( dbParams.getProperty( "metadata_driver" ), dbParams.getProperty( "metadata_url" ), dbParams.getProperty( "metadata_user" ), dbParams.getProperty( "metadata_pass" ), false );
            wcps = new WCPS( pcSchemaFile, new CachedMetadataSource( metadataSource ) );
        }
        catch( Exception e ) {
            System.err.println( "WCPS: could not initialize WCPS:" );
            e.printStackTrace( System.err );
            System.exit( 1 );
        }

        for( int i = 0; i < args.length; i++ ) {
            File fileIn = null;
            try {
                fileIn = new File( args[i] );
            }
            catch( Exception fnfe ) {
                System.err.println( "WCPS: " + args[i] + ": no such file or directory" + fnfe );
                System.exit( 1 );
            }
            boolean ok = processCoverage( fileIn, i );
            if( !ok ) {
                System.err.println( "WCPS: " + args[i] + " failed" );
                System.exit( 1 );
            }
        }

        metadataSource.close();
        System.exit( 0 );

    }

    // TODO: Here's a fun idea: make this function multithreaded! ;)
    private static boolean processCoverage( File in, int i ) {

        try {

            ProcessCoverageRequest r = wcps.pcPrepare("http://kahlua.eecs.jacobs-university.de:7001", "RASSERVICE", in );

            System.err.println( "Request " + i );

            System.out.println( r.getQuery() );


/*            Iterator<byte[]> results = r.execute().iterator();

            int j = 0;
            while( results.hasNext() ) {
                String outFileName = "WCPS-" + i + "-" + j++;
                FileOutputStream out = new FileOutputStream( outFileName );
                out.write( results.next() );
                out.close();
                System.out.println( "WCPS: " + outFileName + " written" );
            }*/

        }
        catch( Exception e ) {
            System.err.println( "WCPS: request " + i + " failed with the following exception:" );
            e.printStackTrace( System.err );
        }

        return true;

    }

}
