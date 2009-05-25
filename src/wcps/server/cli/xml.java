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


package wcps.server.cli;

import wcps.server.core.CachedMetadataSource;
import wcps.server.core.DbMetadataSource;
import wcps.server.core.ProcessCoveragesRequest;
import wcps.server.core.WCPS;

import java.io.File;
import java.io.FileInputStream;

import java.util.Properties;

/**
 * This is a small application around the WCPS core. It takes XML requests as files and runs them
 * through WCPS. The resulting radaman queries are displayed, but not executed. This is very useful
 * for testing whether your metadata is valid.
 */


public class xml
{
	private static WCPS wcps;

	public static void main(String[] args)
	{
		if (args.length < 1)
		{
			System.err.println("WCPS CLI: no input files");

            System.err.println("\nWCPS CLI Usage: java wcps.server.cli.xml input.xml");
            System.err.println("Where input.xml contains a ProcessCoverages Request ");
            System.exit(1);
            
            args = new String[1];
            args[0] = "old_testing/testcases/1.test.xml";
		}
        if (args.length > 1)
        {
            System.err.println("WCPS: no input files");
            System.exit(1);
        }

		String pcSchemaFileName = "src" + File.separator + "conf" + File.separator + "xml"
                      + File.separator + "wcps"
                      + File.separator + "1.0.0" + File.separator + "wcpsProcessCoverages.xsd";
		File pcSchemaFile = new File(pcSchemaFileName);

		if (!pcSchemaFile.exists())
		{
			System.err.println("WCPS: could not find the WCPS ProcessCoverage schema ("
					   + pcSchemaFileName + ")");
			System.exit(1);
		}

		DbMetadataSource metadataSource = null;

		try
		{
			Properties dbParams = new Properties();

			dbParams.load(new FileInputStream("dbparams.properties"));
			metadataSource =
				new DbMetadataSource(dbParams.getProperty("metadata_driver"),
						     dbParams.getProperty("metadata_url"),
						     dbParams.getProperty("metadata_user"),
						     dbParams.getProperty("metadata_pass"), false);
			wcps = new WCPS(pcSchemaFile, new CachedMetadataSource(metadataSource));
		}
		catch (Exception e)
		{
			System.err.println("WCPS: could not initialize WCPS:");
			e.printStackTrace(System.err);
			System.exit(1);
		}

		for (int i = 0; i < args.length; i++)
		{
			File fileIn = null;

			try
			{
				fileIn = new File(args[i]);
			}
			catch (Exception fnfe)
			{
				System.err.println("WCPS: " + args[i]
						   + ": no such file or directory" + fnfe);
				System.exit(1);
			}

			boolean ok = processCoverage(fileIn, i);

			if (!ok)
			{
				System.err.println("WCPS: " + args[i] + " failed");
				System.exit(1);
			}
		}

		metadataSource.close();
		System.exit(0);

	}

	private static boolean processCoverage(File in, int i)
	{
		try
		{

			ProcessCoveragesRequest r =
				wcps.pcPrepare("http://kahlua.eecs.jacobs-university.de:7001",
					       "RASSERVICE", in);

			System.err.println("Request " + i);

			System.out.println(r.getRasqlQuery());


/*                        Iterator<byte[]> results = r.execute().iterator();

                        int j = 0;
                        while( results.hasNext() ) {
                            String outFileName = "WCPS-" + i + "-" + j++;
                            FileOutputStream out = new FileOutputStream( outFileName );
                            out.write( results.next() );
                            out.close();
                            System.out.println( "WCPS: " + outFileName + " written" );
                        }*/
		}
		catch (Exception e)
		{
			System.err.println("WCPS: request " + i
					   + " failed with the following exception:");
			e.printStackTrace(System.err);
		}

		return true;

	}
}
