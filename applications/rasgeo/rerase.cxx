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
 * Copyright 2003 - 2011 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
 
/*
 * Contributed to rasdaman by Alexander Herzig, Landcare Research New Zealand
 */


#include "nmlog.h"
#include <sstream>
#include <string>
#include <algorithm>
#include "rerase.h"

using namespace std;

#ifdef DEBUG
int nmlog::nmindent = -1;
#endif


void showEraseHelp()
{
	std::cout << std::endl << "raserase v0.3" << std::endl << std::endl;

	std::cout << "Usage: raserase -coll <collection> [-oid <OID>] "
			"[-conn <connection file>]" << std::endl << std::endl;

	std::cout << "   -coll: name of rasdaman collection to delete" << std::endl;
	std::cout << "   -oid:  local object identifier (OID) of image "
			"to delete" << std::endl;

	std::cout << std::endl;
}


int main(int argc, char** argv)
{
	NMDebugCtx(ctxRerase, << "...");

	if (argc < 2)
	{
		showEraseHelp();
		return EXIT_SUCCESS;
	}

	string collname;
	double oid = -1;
	string connfile;

	// parse command line arguments
	int arg = 1;
	while (arg < argc-1)
	{
		string theArg = argv[arg];
		std::transform(theArg.begin(), theArg.end(),
				theArg.begin(), ::tolower);

		if (theArg == "-coll")
		{
			collname = argv[arg+1];
			if (collname.empty())
			{
				NMErr(ctxRerase, << "missing parameter for -coll: please "
						"specify a target collection!");
				return EXIT_FAILURE;
			}
		}
		else if (theArg == "-oid")
		{
			oid = std::atof(argv[arg+1]);
			if (oid <= 0)
			{
				NMMsg(<< "Invalid OID specified!" << std::endl);
				return EXIT_FAILURE;
			}
		}
		else if (theArg == "-conn")
		{
			connfile = argv[arg+1];
			if (access(connfile.c_str(), R_OK) != 0)
			{
				NMErr(ctxRerase, << "invalid parameter for -conn: could "
						"not access connection file '" << connfile << "'!");
				return EXIT_FAILURE;
			}
		}
		else if (theArg == "--help")
		{
			showEraseHelp();
			return EXIT_SUCCESS;
		}
		arg++;
	}

	// ---------------------------------------------------------------------
	// get the connection file and check readability
	if (connfile.empty())
	{
		connfile = string(getenv("HOME")) + "/.rmanman";
		if (access(connfile.c_str(), R_OK) != 0)
		{
			NMErr(ctxRerase, << "could not access connection file '"
					<< connfile << "'!");
			return EXIT_FAILURE;
		}
	}

	RasdamanConnector rasconn(connfile);
	RasdamanHelper2 helper(&rasconn);

	if (collname.empty())
	{
		NMErr(ctxRerase, << "please specify a collection or image to delete!");
		showEraseHelp();
		return EXIT_FAILURE;
	}

	try
	{
		if (helper.doesCollectionExist(collname) == -1)
		{
			NMMsg(<< "Couldn't find collection '" << collname << "' in the data base, "
					"so let's quit here!" << std::endl);
			NMDebugCtx(ctxRerase, << "done!");
			return EXIT_SUCCESS;
		}

		// drop either image or collection
		if (oid == -1)
			helper.dropCollection(collname);
		else
			helper.dropImage(collname, oid);

		// delete meta data from the data base
		helper.deletePSMetadata(collname);
		helper.deleteNMMetadata(collname, oid);
	}
	catch(r_Error& re)
	{
		NMErr(ctxRerase, << re.what());
		NMDebugCtx(ctxRerase, << "done!");
		return EXIT_FAILURE;
	}

	NMDebugCtx(ctxRerase, << "done!");
	return EXIT_SUCCESS;
}
