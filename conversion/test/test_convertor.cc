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

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "conversion/convertor.hh"
#include "conversion/tiff.hh"
#include "raslib/rminit.hh"
#include "rasodmg/gmarray.hh"

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

// define if you want to use r_Type variables to set the
// array type.
//#define TEST_CONV_USE_RTYPES


void ConvertToTIFFCore(r_Conv_TIFF *tiff, const char *save, const char *params)
{
    r_convDesc desc;
    FILE *tfile;
    long size;

    try
    {
        // desc will become invalid when the TIFF object is deleted.
        // The same applies to all data allocated by the TIFF object,
        // e.g. the buffer holding the resulting TIFF.
        desc = tiff->convertTo(params);

        size = (long)(desc.destInterv[0].high() - desc.destInterv[0].low() + 1);
        if ((tfile = fopen(save, "wb")) == NULL)
        {
            cerr << "Error opening file " << save << endl;
        }
        else
        {
            fwrite((void*)(desc.dest), (size_t)1, (size_t)size, tfile);
            fclose(tfile);
        }
        cout << "test_convertor: r_Conv_TIFF::convertTo successful. "
             << "size = " << size << ", type = ";
        desc.destType->print_status(cout);
        cout << endl;
        // This is the job of the client!
        free(desc.dest);
        delete desc.destType;
    }
    catch (r_Error &err)
    {
        cout << "An error occurred: " << err.what() << endl;
    }
}


void ConvertToTIFF(char *data, r_Minterval &iv, r_Type *type, const char *save, const char *params)
{
    r_Conv_TIFF *tiff;

    cout << "test_convertor (r_Type): ";
    if (type)
    {
        cout << "Base Type is ";
        type->print_status(cout);
        cout << endl;
    }
    else
    {
        cout << "Base type not defined!" << endl;
    }

    tiff = new r_Conv_TIFF(data, iv, type);
    ConvertToTIFFCore(tiff, save, params);
    delete tiff;
}


void ConvertToTIFF(char *data, r_Minterval &iv, int type, const char *save, const char *params)
{
    r_Conv_TIFF *tiff;

    cout << "test_convertor (int):" << endl;

    tiff = new r_Conv_TIFF(data, iv, type);
    ConvertToTIFFCore(tiff, save, params);
    delete tiff;
}


void ConvertFromTIFF(char *name, const char *params, const char *save_as = NULL)
{
    FILE *fp;
    long size;
    r_convDesc desc;
    r_Conv_TIFF *tiff;
    char *data;
    r_Minterval iv;

    fp = fopen(name, "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    data = new char[size];
    fseek(fp, 0, SEEK_SET);
    fread((void*)data, (size_t)1, (size_t)size, fp);
    fclose(fp);

    // Set interval
    iv = r_Minterval(1);
    iv << r_Sinterval(r_Range(0), r_Range(size - 1));

    // Base type is coded in TIFF
    tiff = new r_Conv_TIFF(data, iv, (int)0);

    desc = tiff->convertFrom();

    cout << "test_convertor: r_Conv_TIFF::convertFrom successful."
         << " domain = " << desc.destInterv << ", type = ";
    desc.destType->print_status(cout);
    cout << endl;

    delete [] data;

    // save file again as TIFF to check convertFrom validity?
    if (save_as != NULL)
    {
        cout << "test_convertor: Saving data again as <" << save_as << ">..." << endl;
        ConvertToTIFF(desc.dest, desc.destInterv, desc.baseType, save_as, params);
    }

    // The order in which objects are deleted is important!
    free(desc.dest);
    delete desc.destType;

    // Delete this last. From then on desc is invalid.
    delete tiff;
}



// Flag bits for main()
#define CONVERTOR_WRITE_BACK    1


// Calling convention: test_convertor [-x # -y # -c <compression> -v -h]
// Where -x: width, -y: height, -c: compression, -v: write again as TIFF after reading
// -h: Help on usage
// width, height default to 200, 100
int main (int argc, char *argv[])
{
    r_GMarray dummyArray; // need this for linking on Linux, don't know why
    char *data, *lineBase, *line;
    int width = 200, height = 100;
    unsigned int flags = 0;
    int i, j;
    char params[256];
    const char *paramptr = NULL;
#ifdef TEST_CONV_USE_RTYPES
    r_Type *type;
#endif
    struct nametable
    {
        char *write, *verify;
    } tiffnames[] =
    {
        {"grey.tif", "grey2.tif"},
        {"bool.tif", "bool2.tif"},
        {"rgb.tif",  "rgb2.tif"}
    };

    i = 1;
    while (i < argc)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'x':
                width = atoi(argv[++i]);
                break;
            case 'y':
                height = atoi(argv[++i]);
                break;
            case 'c':
                sprintf(params, "comptype=%s", argv[++i]);
                paramptr = params;
                break;
            case 'v':
                flags |= CONVERTOR_WRITE_BACK;
                break;
            case 'h':
                cout << "Usage: " << argv[0] << "[-w # -h # -c <string> -v -h]" << endl;
                cout << "\t-x #: width" << endl;
                cout << "\t-y #: height" << endl;
                cout << "\t-c <string>: compression (see r_Conv_TIFF::compNames)" << endl;
                cout << "\t-v: write the TIFF files just read back as TIFF files for verifying" << endl;
                cout << "\t-h: this help" << endl;
                exit(0);
            default:
                cout << "Bad switch -" << argv[i][1] << endl;
                break;
            }
        }
        i++;
    }

    cout << "test_convertor: use images of size " << width << " * " << height
         << ", compression: ";

    // Allocate enough room for _all_ types of data.
    if ((data = new char[3*width*height]) == NULL)
    {
        cout << "Out of memory error!" << endl;
        exit(0);
    }

    r_Minterval iv(2);
    iv << r_Sinterval(r_Range(0), r_Range(width - 1))
       << r_Sinterval(r_Range(0), r_Range(height - 1));

    // Take into account that MDD arrays are transposed compared to
    // images!
    cout << "Greyscale:" << endl;
    lineBase = data;
    for (j=0; j<height; j++, lineBase++)
    {
        line = lineBase;
        for (i=0; i<width; i++, line += height)
        {
            *line = (char)((i+j) & 0xff);
        }
    }

#ifdef TEST_CONV_USE_RTYPES
    type = r_Type::get_base_type("char");
    ConvertToTIFF(data, iv, type, tiffnames[0].write, paramptr);
    delete type;
#else
    ConvertToTIFF(data, iv, (int)(r_Convertor::ctype_char), tiffnames[0].write, paramptr);
#endif

    cout << "Bitmap:" << endl;
    lineBase = data;
    for (j=0; j<height; j++, lineBase++)
    {
        line = lineBase;
        for (i=0; i<width; i++, line += height)
        {
            *line = (char)((i + j) & 1);
        }
    }

#ifdef TEST_CONV_USE_RTYPES
    type = r_Type::get_base_type("boolean");
    ConvertToTIFF(data, iv, type, tiffnames[1].write, paramptr);
    delete type;
#else
    ConvertToTIFF(data, iv, (int)(r_Convertor::ctype_bool), tiffnames[1].write, paramptr);
#endif

    cout << "RGB:" << endl;
    lineBase = data;
    for (j=0; j<height; j++, lineBase += 3)
    {
        line = lineBase;
        for (i=0; i<width; i++, line += 3*height)
        {
            line[0] = ((i+j) & 0xff);
            line [1] = ((0xff - (i+j)) & 0xff);
            line[2] = (((i+j) >> 1) & 0xff);
        }
    }

#ifdef TEST_CONV_USE_RTYPES
    type = r_Type::get_base_type("struct{ char red, char green, char blue }");
    ConvertToTIFF(data, iv, type, tiffnames[2].write, paramptr);
    delete type;
#else
    ConvertToTIFF(data, iv, (int)(r_Convertor::ctype_rgb), tiffnames[2].write, paramptr);
#endif

    delete [] data;

    // Try the other way around: convert from TIFF
    ConvertFromTIFF("rgb.tif", paramptr,
                    ((flags & CONVERTOR_WRITE_BACK) != 0) ? tiffnames[2].verify : NULL);

    ConvertFromTIFF("bool.tif", paramptr,
                    ((flags & CONVERTOR_WRITE_BACK) != 0) ? tiffnames[1].verify : NULL);

    ConvertFromTIFF("grey.tif", paramptr,
                    ((flags & CONVERTOR_WRITE_BACK) != 0) ? tiffnames[0].verify : NULL);

    return 0;
}
