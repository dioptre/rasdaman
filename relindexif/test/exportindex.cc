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
/*
get mddoid, iterate domain, dataformat

select mdd
get domain
    {
    begin ta
    iterate over domain
    retrieve tiles
    alter tiles
    commit ta
    }
*/

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "server/template_inst.hh"
#endif

#include "reladminif/adminif.hh"
#include "reladminif/databaseif.hh"
#include "reladminif/transactionif.hh"
#include "reladminif/oidif.hh"
#include "cmlparser.hh"
#include "relindexif/indexid.hh"

#define ALLDONE         0
#define ERRORPARSINGCOMMANDLINE 1
#define DOMAINMISSING       2
#define OIDMISSING      3
#define OIDINVALID      4
#define DOMAINMISMATCH      5
#define FAILED          6
#define DOMAININVALID       7
#define UNKNOWNDATAFORMAT   8

char globalConnectId[256];
char globalDbUser[255] = {0};
char globalDbPasswd[255] = {0};

RMINITGLOBALS('S')

r_Minterval domain;
OId oid;

int
main(int argc, char** argv)
{
    RMInit::logOut.rdbuf(cout.rdbuf());
    RMInit::dbgOut.rdbuf(cout.rdbuf());
    int retval = 0;
    bool import = false;
    const char* fileName = NULL;
    CommandLineParser    &cmlInter      = CommandLineParser::getInstance();
    CommandLineParameter &clp_help      = cmlInter.addFlagParameter('h', "help", "show command line switches");
    CommandLineParameter &clp_import    = cmlInter.addStringParameter('i', "import", "import data");

    CommandLineParameter &clp_connect   = cmlInter.addStringParameter(CommandLineParser::noShortName, "connect", "database connect string", "/");
    CommandLineParameter &clp_oid       = cmlInter.addStringParameter(CommandLineParser::noShortName, "oid", "the oid of the mdd to operate on");
    try
    {
        cmlInter.processCommandLine(argc, argv);
    }
    catch(CmlException& err)
    {
        cmlInter.printHelp();
        cout << "Error parsing command line:" << endl;
        cout << err.what() << endl;
        return ERRORPARSINGCOMMANDLINE;
    }
    if (cmlInter.isPresent('h'))
    {
        cmlInter.printHelp();
        return ALLDONE;
    }
    if (cmlInter.isPresent("oid"))
    {
        oid = OId(atol(cmlInter.getValueAsString("oid")));
        if (oid.getType() != OId::MDDHIERIXOID)
        {
            cout << "oid is not a mdd oid" << endl;
            return OIDINVALID;
        }
    }
    else
    {
        if (cmlInter.isPresent('i'))
        {
            import = true;
            fileName = cmlInter.getValueAsString('i');
        }
        else
        {
            cout << "oid is missing" << endl;
            return OIDMISSING;
        }
    }
    strcpy((char*)globalConnectId, cmlInter.getValueAsString("connect"));
    cout << "connect " << globalConnectId << endl;
    cout << "set up done" << endl;
    AdminIf::instance();
    DatabaseIf d;
    TransactionIf t;
    if (import)
    {
        std::ifstream i;
        i.open(fileName);
        if (i.is_open())
        {
            i.seekg(0, std::ios::end);
            std::streampos end = i.tellg();
            size_t length = end;
            BinaryRepresentation brp;
            brp.binaryLength = length;
            brp.binaryName = new char[strlen(fileName) + 1];
            memcpy(brp.binaryName, fileName, strlen(fileName) + 1);
            brp.binaryData = new char[length];
            i.seekg(0, std::ios::beg);
            i.read(brp.binaryData, end);
            i.close();
            DBHierIndex* p = new DBHierIndex(0, false, false);
            p->setBinaryRepresentation(brp);
            delete [] brp.binaryData;
            brp.binaryData = NULL;
            delete [] brp.binaryName;
            brp.binaryName = NULL;
            p->printStatus();
            try
            {
                d.open("RASBASE");
                t.begin(&d, false);
                ObjectBroker::registerDBObject(p);
                t.commit();
                d.close();
            }
            catch (const r_Error& e)
            {
                cout << "Caught exception " << e.get_errorno() << " " << e.what() << endl;
                try
                {
                    t.abort();
                }
                catch (const r_Error& ee)
                {
                    cout << "Caugh exception while aborting " << ee.get_errorno() << " " << ee.what() << endl;
                    try
                    {
                        t.abort();
                    }
                    catch (const r_Error& eee)
                    {
                        cout << "Caugh exception while 2nd aborting " << eee.get_errorno() << " " << eee.what() << endl;
                    }
                }
                try
                {
                    d.close();
                }
                catch (const r_Error& eeee)
                {
                    cout << "Caugh exception while closing " << eeee.get_errorno() << " " << eeee.what() << endl;
                }
                return FAILED;
            }
        }
        else
        {
            cout << "Unable to open file " << fileName << endl;
        }
    }
    else
    {
        try
        {
            d.open("RASBASE");
            t.begin(&d, true);
            DBHierIndexId index(oid);
            BinaryRepresentation bin = index->getBinaryRepresentation();
            index->printStatus();
            std::ofstream o;
            o.open(bin.binaryName);
            if (o.is_open())
            {
                o.write(bin.binaryData, bin.binaryLength);
                o.flush();
                o.close();
            }
            else
            {
                cout << "Unable to open file " << bin.binaryName << endl;
            }
            delete [] bin.binaryName;
            delete [] bin.binaryData;
            t.commit();
            d.close();
        }
        catch (const r_Error& e)
        {
            cout << "Caught exception " << e.get_errorno() << " " << e.what() << endl;
            try
            {
                t.abort();
            }
            catch (const r_Error& ee)
            {
                cout << "Caugh exception while aborting " << ee.get_errorno() << " " << ee.what() << endl;
                try
                {
                    t.abort();
                }
                catch (const r_Error& eee)
                {
                    cout << "Caugh exception while 2nd aborting " << eee.get_errorno() << " " << eee.what() << endl;
                }
            }
            try
            {
                d.close();
            }
            catch (const r_Error& eeee)
            {
                cout << "Caugh exception while closing " << eeee.get_errorno() << " " << eeee.what() << endl;
            }
            return FAILED;
        }
    }
    return retval;
}
