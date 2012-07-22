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
#include "mddmgr/mddobj.hh"
#include "tilemgr/tile.hh"
#include "cmlparser.hh"
#include "raslib/mitera.hh"

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
    CommandLineParser    &cmlInter      = CommandLineParser::getInstance();
    CommandLineParameter &clp_help      = cmlInter.addFlagParameter('h', "help", "show command line switches");
    CommandLineParameter &clp_connect   = cmlInter.addStringParameter(CommandLineParser::noShortName, "connect", "database connect string", "/");
    CommandLineParameter &clp_mdddomain = cmlInter.addStringParameter(CommandLineParser::noShortName, "mdddomain", "the domain to do the changes in.");
    CommandLineParameter &clp_domain    = cmlInter.addStringParameter(CommandLineParser::noShortName, "domain", "the extent to be used when selecting data from the mdd.");
    CommandLineParameter &clp_oid       = cmlInter.addStringParameter(CommandLineParser::noShortName, "oid", "the oid of the mdd to operate on");
    CommandLineParameter &clp_comptype  = cmlInter.addStringParameter(CommandLineParser::noShortName, "storageformat", "name of storage format", "Array");
    CommandLineParameter &clp_readonly  = cmlInter.addFlagParameter('r', "readonly", "read only check");
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
    r_Data_Format storageFormat = r_Array;
    if (cmlInter.isPresent("storageformat"))
    {
        storageFormat = get_data_format_from_name(cmlInter.getValueAsString("storageformat"));
        if (storageFormat == r_Data_Format_NUMBER)
        {
            cout << "unknown data format " << cmlInter.getValueAsString("storageformat") << endl;
            return UNKNOWNDATAFORMAT;
        }
    }
    if (!cmlInter.isPresent("domain"))
    {
        cout << "domain is missing" << endl;
        return DOMAINMISSING;
    }
    if (!cmlInter.isPresent("oid"))
    {
        cout << "oid is missing" << endl;
        return OIDMISSING;
    }
    strcpy((char*)globalConnectId, cmlInter.getValueAsString("connect"));
    cout << "connect " << globalConnectId << endl;
    try
    {
        domain = r_Minterval(cmlInter.getValueAsString("domain"));
    }
    catch (const r_Error& e)
    {
        cout << "domain is not valid: " << e.what() << " " << e.get_errorno() << endl;
        return DOMAININVALID;
    }
    oid = OId(atol(cmlInter.getValueAsString("oid")));
    if (oid.getType() != OId::MDDOID)
    {
        cout << "oid is not a mdd oid" << endl;
        return OIDINVALID;
    }
    bool readonly = cmlInter.isPresent("readonly");
    if (readonly)
    {
        cout << "performing read only check" << std::endl;
    }
    else
    {
        cout << "performing recompression" << std::endl;
    }
    cout << "set up done" << endl;
    AdminIf::instance();
    DatabaseIf d;
    TransactionIf t;
    MDDObj* mdd = NULL;
    r_Minterval completeDomain;
    r_MiterArea* miter = NULL;
    try
    {
        d.open("RASBASE");
        t.begin(&d, true);
        mdd = new MDDObj(oid);
        if (mdd->getCurrentDomain().dimension() == domain.dimension())
        {
            completeDomain = mdd->getCurrentDomain();
            if (cmlInter.isPresent("mdddomain"))
            {
                try
                {
                    r_Minterval temp(cmlInter.getValueAsString("mdddomain"));
                    if (temp.dimension() == completeDomain.dimension())
                    {
                        cout << "Not using complete mdd domain (" << completeDomain << ") but " << temp << endl;
                        completeDomain = temp;
                    }
                    else
                    {
                        cout << "Domains do not have same number of dimensions" << endl;
                        delete mdd;
                        mdd = NULL;
                        t.commit();
                        d.close();
                        return DOMAINMISMATCH;
                    }
                }
                catch (const r_Error& e2)
                {
                    cout << "mdddomain parameter is not correct: " << e2.get_errorno() << " " << e2.what() << endl;
                    delete mdd;
                    mdd = NULL;
                    t.commit();
                    d.close();
                    return DOMAININVALID;
                }
            }
            miter = new r_MiterArea(&domain, &completeDomain);
        }
        else
        {
            t.commit();
            d.close();
            return DOMAINMISMATCH;
        }
        delete mdd;
        mdd = NULL;
        t.commit();
        d.close();
    }
    catch (const r_Error& e)
    {
        delete miter;
        miter = NULL;
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
    std::vector<Tile*>* tiles = NULL;
    std::vector<Tile*>::iterator here;
    std::vector<Tile*>::iterator end;
    r_Minterval currentDomain;
    while (!miter->isDone())
    {
        currentDomain = miter->nextArea();
        try
        {
            d.open("RASBASE");
            t.begin(&d, readonly);
            mdd = new MDDObj(oid);
            tiles = mdd->intersect(currentDomain);
            end = tiles->end();
            if (tiles)
            {
                cout << "working on " << currentDomain << " with " << tiles->size() << " tiles" << endl;
                for (here = tiles->begin(); here < end; here++)
                {
                    cout << " tile " << (*here)->getDomain() << " " << (*here)->getDBTile()->getOId() << " " << (*here)->getDataFormat() << " " << (*here)->getCompressedSize();
                    if (!(*here)->decompress())
                    {
                        cout << " decompress failed";
                    }
                    if (((*here)->getDataFormat() != storageFormat) && (!readonly))
                    {
                        cout << " changing storage format";
                        (*here)->setCompressionFormat(storageFormat);
                        cout << " compressed size " << (*here)->getCompressedSize();
                    }
                    cout << endl;
                }
            }
            else
            {
                cout << currentDomain << " no tiles" << endl;
            }
            delete mdd;
            mdd = NULL;
            delete tiles;
            tiles = NULL;
            t.commit();
            d.close();
        }
        catch (const r_Error& e)
        {
            delete miter;
            miter = NULL;
            delete tiles;
            tiles = NULL;
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
    delete tiles;
    tiles = NULL;
    delete miter;
    miter = NULL;
    return retval;
}
