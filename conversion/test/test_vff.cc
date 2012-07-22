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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "conversion/vff.hh"
#include "raslib/rminit.hh"
#include "raslib/basetype.hh"

#ifdef __GNUG__
#include "rasodmg/transaction.hh"
#include "raslib/template_inst.hh"
#else
RMINITGLOBALS('C')
#endif



static void PrintUsage(const char *name)
{
    cout << "Usage: " << name << " [-o <vffoutfile> -p <params> -h] <vffinfile>" << endl;
}


int main(int argc, char *argv[])
{
    const char *infile=NULL;
    const char *outfile=NULL;
    const char *params=NULL;
    int i;

    i=1;
    while (i<argc)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'o':
                outfile = argv[++i];
                break;
            case 'p':
                params = argv[++i];
                break;
            case 'h':
                PrintUsage(argv[0]);
                exit(0);
            default:
                cerr << "Unknown switch " << argv[i] << endl;
                break;
            }
        }
        else
        {
            if (infile != NULL)
                cerr << "More than one input file, ignored" << endl;
            else
                infile = argv[i];
        }
        i++;
    }

    if (infile == NULL)
    {
        PrintUsage(argv[0]);
        exit(-1);
    }

    FILE *fp;
    if ((fp = fopen(infile, "rb")) == NULL)
    {
        cerr << "Unable to open input file " << infile << endl;
        return -1;
    }

    size_t fsize;
    char *data;

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);

    if ((data = new char[fsize]) == NULL)
    {
        cerr << "Unable to claim memory for file" << endl;
        fclose(fp);
        return -1;
    }
    fseek(fp, 0, SEEK_SET);
    fread(data, 1, fsize, fp);
    fclose(fp);

    r_Minterval interv(1);
    interv << r_Sinterval((r_Range)0, (r_Range)fsize-1);
    r_Conv_VFF conv(data, interv, r_Convertor::ctype_char);
    r_convDesc desc;

    cout << "Convert from VFF... " << flush;
    try
    {
        desc = conv.convertFrom(params);
        cout << "OK" << endl;

        r_Conv_VFF convb(desc.dest, desc.destInterv, desc.destType);
        r_convDesc descb;

        cout << "Convert back to VFF... " << flush;
        try
        {
            descb = convb.convertTo(params);
            cout << "OK" << endl;

            if (outfile != NULL)
            {
                if ((fp = fopen(outfile, "wb")) == NULL)
                {
                    cerr << "Unable to write to file " << outfile << endl;
                }
                else
                {
                    fsize = (size_t)(descb.destInterv[0].high() - descb.destInterv[0].low() + 1);
                    fwrite(descb.dest, 1, fsize, fp);
                    fclose(fp);
                }
            }

            // endianness-safe comparison of binary data:
            // convert the just created VFF data back to an MDD and compare the
            // result with the first MDD thus converted.
            r_Conv_VFF convc(descb.dest, descb.destInterv, descb.destType);
            r_convDesc descc;

            cout << "Compare binary data... " << flush;
            try
            {
                descc = convc.convertFrom(params);
                if (memcmp(descc.dest, desc.dest, desc.destInterv.cell_count() * ((r_Base_Type*)desc.destType)->size()) == 0)
                    cout << "identical" << endl;
                else
                    cout << "differs!!!" << endl;

                delete descc.destType;
                free(descc.dest);
            }
            catch (r_Error &err)
            {
                cerr << "failed: " << err.what() << endl;
            }

            delete descb.destType;
            free(descb.dest);
        }
        catch (r_Error &err)
        {
            cerr << "failed: " << err.what() << endl;
        }
        delete desc.destType;
        free(desc.dest);
    }
    catch (r_Error &err)
    {
        cerr << "failed: " << err.what() << endl;
    }

    delete [] data;

    return 0;
}
