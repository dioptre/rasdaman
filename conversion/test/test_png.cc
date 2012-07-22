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

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>


#include "conversion/convertor.hh"
#include "conversion/png.hh"

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

static int init_identity(int offset)
{
    return offset;
}

static int init_square(int offset)
{
    return offset * offset;
}

static int SinTabOK = 0;
static int SinusTable[512];

static int init_sine(int offset)
{
    if (SinTabOK == 0)
    {
        int i;

        for (i=0; i<512; i++)
        {
            SinusTable[i] = (int)(255*sin(M_PI * offset / 512.0));
        }
        SinTabOK = 1;
    }
    return SinusTable[offset & 511];
}

int test_picture(int width, int height, int bpp, int (*init)(int))
{
    char *data;
    r_Minterval interv(2);
    int datasize;
    int baseType;
    int bytespp;
    int i;

    switch (bpp)
    {
    case 1:
        baseType = r_Convertor::ctype_bool;
        bytespp = 1;
        break;
    case 8:
        baseType = r_Convertor::ctype_char;
        bytespp = 1;
        break;
    case 24:
        baseType = r_Convertor::ctype_rgb;
        bytespp = 3;
        break;
    default:
        cerr << "Unknown bpp value " << bpp << endl;
        return -1;
    }

    interv << r_Sinterval((r_Range)0, (r_Range)width-1)
           << r_Sinterval((r_Range)0, (r_Range)height-1);

    datasize = (width * bytespp * height);
    data = new char[datasize];

    cout << "Test: bpp = " << bpp << ", domain = " << interv << ", size = " << datasize << endl;

    if (bpp > 1)
    {
        for (i=0; i<datasize; i++) data[i] = (char)init(i);
    }
    else
    {
        int j, count;

        for (i=0; i<datasize; i+=8)
        {
            count = datasize - i;
            if (count > 8) count = 8;
            for (j=0; j<count; j++) data[i+j] = ((init(i>>3) & (1<<j)) == 0) ? 0 : 1;
            //for (j=0; j<count; j++) cout << (char)(data[i+j] + '0'); cout << endl;
        }
    }

    r_Conv_PNG *png = NULL;
    char *dest = NULL;

    try
    {
        r_convDesc desc;
        int destsize;
        int status = -1;

        png = new r_Conv_PNG(data, interv, baseType);
        desc = png->convertTo();
        destsize = desc.destInterv[0].high() - desc.destInterv[0].low() + 1;
        cout << "\tConverted to PNG, domain " << desc.destInterv << endl;
        dest = desc.dest;
        r_Minterval destInterv(desc.destInterv);
        delete desc.destType;
        delete png;
        png = new r_Conv_PNG(dest, destInterv, r_Convertor::ctype_char);
        desc = png->convertFrom();
        cout << "\tConverted from PNG, domain " << desc.destInterv << endl;

        if (interv == desc.destInterv)
            status = memcmp(data, desc.dest, datasize);

        delete png;
        delete [] data;
        free(dest);
        free(desc.dest);
        delete desc.destType;

        if (status != 0)
        {
            cout << "\t!!! Data not identical !!!" << endl;
        }

        return 0;
    }
    catch (r_Error &err)
    {
        cerr << "Error: " << err.what() << endl;

        delete [] data;
        if (dest != NULL) free(dest);
        if (png != NULL) delete png;

        return -1;
    }
}

int main(int argc, char *argv[])
{
    int i;
    char pngfile[256] = "";

    i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-file") == 0)
        {
            strcpy(pngfile, argv[++i]);
        }
        i++;
    }

    if (pngfile[0] == '\0')
    {
        // No PNG-file given, standard test-suite
        test_picture(400, 300, 1, init_identity);
        test_picture(400, 300, 8, init_identity);
        test_picture(400, 300, 24, init_identity);
        test_picture(701, 333, 1, init_square);
        test_picture(701, 333, 8, init_square);
        test_picture(701, 333, 24, init_square);
        test_picture(3007, 3999, 1, init_sine);
        test_picture(3007, 3999, 8, init_sine);
        test_picture(3007, 3999, 24, init_sine);
    }
    else
    {
        // Work on this file
        FILE *fp;
        size_t fsize;
        char *data;
        r_convDesc desc;
        r_Conv_PNG *png;
        r_Type *baseType;

        if ((fp = fopen(pngfile, "rb")) == NULL)
        {
            cerr << "Unable to open input file" << endl;
            return -1;
        }
        fseek(fp, 0, SEEK_END);
        fsize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        data = new char[fsize];
        fread(data, 1, fsize, fp);
        fclose(fp);

        png = NULL;
        baseType = NULL;
        try
        {
            r_Minterval interv(1);
            interv << r_Sinterval((r_Range)0, (r_Range)fsize-1);

            png = new r_Conv_PNG(data, interv, r_Convertor::ctype_char);
            cout << "Converting file <" << pngfile << "> to MDD..." << endl;
            desc = png->convertFrom();

            cout << "Resulting domain " << desc.destInterv << ", ";
            desc.destType->print_status();
            cout << endl;

            interv = r_Minterval(desc.destInterv);

            delete [] data;
            data = desc.dest;
            baseType = desc.destType;
            delete png;

            fsize = (interv[0].high() - interv[0].low() + 1) * (interv[1].high() - interv[1].low() + 1);

            png = new r_Conv_PNG(data, interv, baseType);
            cout << "Converting MDD back to PNG..." << endl;
            desc = png->convertTo();

            cout << "Resulting domain " << desc.destInterv << ", ";
            desc.destType->print_status();
            cout << endl;

            fp = fopen("result.png", "wb");
            fwrite(desc.dest, 1, (desc.destInterv[0].high() - desc.destInterv[0].low() + 1), fp);
            fclose(fp);

            delete baseType;

            free(data);
            free(desc.dest);
            delete desc.destType;
            delete png;
        }
        catch (r_Error &err)
        {
            cerr << "Conversion failed: " << err.what() << endl;
            if (baseType != NULL) delete baseType;
            if (data != NULL) delete [] data;
            if (png != NULL) delete png;
        }
    }

    return 0;
}
