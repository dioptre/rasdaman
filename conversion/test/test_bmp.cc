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


#include "conversion/convertor.hh"
#include "conversion/bmp.hh"

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

int main(int argc, char *argv[])
{
    char filename[256] = "Somewhere.bmp";
    long fsize=0;
    FILE *fp=NULL;
    char *data=NULL;
    r_Minterval interv(1);
    r_Minterval imgInterv;
    r_Conv_BMP *bmp=NULL;
    r_convDesc desc;
    r_Type *baseType=NULL;
    char *imgData=NULL;

    if (argc > 1)
        strcpy(filename, argv[1]);

    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        cerr << "Unable to open file " << filename << endl;
        exit(-1);
    }

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    data = new char[fsize];
    fread(data, 1, fsize, fp);
    fclose(fp);

    interv << r_Sinterval((r_Range)0, (r_Range)fsize-1);

    cout << "Convert from BMP..." << endl;
    bmp = new r_Conv_BMP(data, interv, r_Convertor::ctype_char);
    desc = bmp->convertFrom();
    baseType = desc.destType;
    imgData = desc.dest;
    imgInterv = desc.destInterv;
    delete [] data;
    data=NULL;
    delete bmp;
    bmp=NULL;

    cout << "Convert to BMP..." << endl;
    bmp = new r_Conv_BMP(imgData, imgInterv, baseType);
    desc = bmp->convertTo("compress=1");
    fsize = desc.destInterv[0].high() - desc.destInterv[0].low() + 1;

    fp = fopen("result.bmp", "wb");
    fwrite(desc.dest, 1, fsize, fp);
    fclose(fp);

    free(desc.dest);
    desc.dest=NULL;
    delete desc.destType;
    desc.destType=NULL;
    delete bmp;
    bmp=NULL;

    cout << "Clean up..." << endl;

    delete baseType;
    baseType=NULL;
    free(imgData);
    imgData=NULL;

    return 0;
}
