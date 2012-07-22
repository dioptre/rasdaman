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
#include "conversion/jpeg.hh"

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

static int JpegErrorFileNumber;

int test_picture(int width, int height, int bpp, const char *params, int (*init)(int))
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

    cout << "Test: bpp = " << bpp << ", domain = " << interv << ", params = " << ((params == NULL) ? "" : params) << ", size = " << datasize << endl;

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
        }
    }

    r_Conv_JPEG *jpeg = NULL;
    char *dest = NULL;

    try
    {
        r_convDesc desc;
        int destsize;
        int status = 0;

        jpeg = new r_Conv_JPEG(data, interv, baseType);
        desc = jpeg->convertTo(params);
        destsize = desc.destInterv[0].high() - desc.destInterv[0].low() + 1;
        cout << "\tConverted to JPEG, domain " << desc.destInterv << endl;
        dest = desc.dest;
        r_Minterval destInterv(desc.destInterv);
        delete desc.destType;
        delete jpeg;
        jpeg = new r_Conv_JPEG(dest, destInterv, r_Convertor::ctype_char);
        desc = jpeg->convertFrom();
        cout << "\tConverted from JPEG, domain " << desc.destInterv << endl;

        if (interv == desc.destInterv)
        {
            unsigned char *b, *d;
            double totalError;
            unsigned char maxError;
            int i;

            // Bitmaps get expanded to greyscale...
            if (baseType == r_Convertor::ctype_bool)
            {
                for (i=0; i<datasize; i++)
                {
                    if (data[i] != 0) data[i] = 0xff;
                }
            }

            b = (unsigned char*)data;
            d = (unsigned char*)desc.dest;
            totalError = 0.0;
            maxError = abs(*b - *d);
            for (i=0; i<datasize; i++)
            {
                b[i] = abs(b[i] - d[i]);
                if (b[i] > maxError) maxError = b[i];
                totalError += (double)(b[i]);
            }

            r_Conv_JPEG *errpeg;
            r_convDesc cdsc;
            char errfile[256];
            FILE *fp;
            int errType;

            errType = (baseType == r_Convertor::ctype_bool) ? r_Convertor::ctype_char : baseType;
            sprintf(errfile, "jerror%d.jpg", JpegErrorFileNumber++);
            cout << "\tCreating error file <" << errfile << "> ..." << endl;
            cout << "\t(max error = " << (int)maxError << ", avg error = "
                 << totalError / (double)datasize << ")" << endl;

            errpeg = new r_Conv_JPEG(data, interv, errType);
            cdsc = errpeg->convertTo("quality=90");
            i = (int)(cdsc.destInterv[0].high() - cdsc.destInterv[0].low() + 1);
            if ((fp = fopen(errfile, "wb")) == NULL)
            {
                cerr << "\tUnable to open output file!" << endl;
            }
            else
            {
                fwrite(cdsc.dest, 1, i, fp);
                fclose(fp);
            }
            delete cdsc.destType;
            free(cdsc.dest);
            delete errpeg;
        }
        else
        {
            cout << "\t Domain differs!!!" << endl;
            status = -1;
        }

        delete jpeg;
        delete [] data;
        free(dest);
        free(desc.dest);
        delete desc.destType;

        return status;
    }
    catch (r_Error &err)
    {
        cerr << "Error: " << err.what() << endl;

        delete [] data;
        if (dest != NULL) delete [] dest;
        if (jpeg != NULL) delete jpeg;

        return -1;
    }
}

int main(int argc, char *argv[])
{
    int i;
    char jpegfile[256] = "";
    char params[256];
    const char *paramptr = NULL;

    i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-file") == 0)
        {
            strcpy(jpegfile, argv[++i]);
        }
        else if (strcmp(argv[i], "-quality") == 0)
        {
            sprintf(params, "quality=%s", argv[++i]);
            paramptr = params;
        }
        i++;
    }

    if (jpegfile[0] == '\0')
    {
        JpegErrorFileNumber = 0;
        test_picture(400, 300, 1, paramptr, init_identity);
        test_picture(400, 300, 8, paramptr, init_identity);
        test_picture(400, 300, 24, paramptr, init_identity);
        test_picture(701, 333, 1, paramptr, init_square);
        test_picture(701, 333, 8, paramptr, init_square);
        test_picture(701, 333, 24, paramptr, init_square);
        test_picture(3007, 3999, 1, paramptr, init_sine);
        test_picture(3007, 3999, 8, paramptr, init_sine);
        test_picture(3007, 3999, 24, paramptr, init_sine);
    }
    else
    {
        FILE *fp;
        long fileSize;
        char *data = NULL;
        char *cdata = NULL;
        r_convDesc desc;
        r_Type *baseType;
        r_Conv_JPEG *jpeg;

        if ((fp = fopen(jpegfile, "rb")) == NULL)
        {
            cerr << "Unable to open input file" << endl;
            return -1;
        }
        fseek(fp, 0, SEEK_END);
        fileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        data = new char[fileSize];
        fread(data, 1, fileSize, fp);
        fclose(fp);

        jpeg = NULL;
        baseType = NULL;
        try
        {
            r_Minterval interv(1);
            interv << r_Sinterval((r_Range)0, (r_Range)fileSize-1);

            jpeg = new r_Conv_JPEG(data, interv, 0);
            cout << "Converting file <" << jpegfile << "> to MDD..." << endl;
            desc = jpeg->convertFrom();

            r_Minterval imgInterv(desc.destInterv);
            delete [] data;
            data = NULL;
            cdata = desc.dest;
            baseType = desc.destType;
            delete jpeg;

            cout << "Resulting domain " << imgInterv << ", ";
            baseType->print_status();
            cout << endl;

            jpeg = new r_Conv_JPEG(cdata, imgInterv, baseType);
            cout << "Converting MDD back to JPEG..." << endl;
            desc = jpeg->convertTo(paramptr);
            free(cdata);
            cdata = desc.dest;

            cout << "Resulting domain " << desc.destInterv << ", ";
            desc.destType->print_status();
            cout << endl;

            fp = fopen("result.jpg", "wb");
            fileSize = desc.destInterv[0].high() - desc.destInterv[0].low() + 1;
            fwrite(cdata, 1, fileSize, fp);
            fclose(fp);

            delete baseType;
            free(cdata);

            delete desc.destType;
            delete jpeg;
        }
        catch (r_Error &err)
        {
            cerr << "Conversion failed: " << err.what() << endl;
            if (baseType != NULL) delete baseType;
            if (cdata != NULL) delete [] cdata;
            if (jpeg != NULL) delete jpeg;
        }
        if (data != NULL)
            delete [] data;
    }
    return 0;
}
