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

#include "conversion/convertor.hh"
#include "conversion/hdf.hh"
#include "raslib/minterval.hh"
#include "raslib/primitivetype.hh"


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

char *TypeIDToName(int tpid)
{
    switch (tpid)
    {
    case r_Primitive_Type::BOOL:
        return "bool";
        break;
    case r_Primitive_Type::CHAR:
        return "char";
        break;
    case r_Primitive_Type::OCTET:
        return "octet";
        break;
    case r_Primitive_Type::SHORT:
        return "short";
        break;
    case r_Primitive_Type::USHORT:
        return "ushort";
        break;
    case r_Primitive_Type::LONG:
        return "long";
        break;
    case r_Primitive_Type::ULONG:
        return "ulong";
        break;
    case r_Primitive_Type::FLOAT:
        return "float";
        break;
    case r_Primitive_Type::DOUBLE:
        return "double";
        break;
    default:
        return"?";
        break;
    }
}


int TestHDF(r_Minterval &domain, r_Type *tp, const char *params=NULL)
{
    r_Primitive_Type *prim;
    char *src, *dest;
    r_Minterval destInterv;
    r_Conv_HDF *hdf;
    r_Type *destType;
    r_convDesc desc;
    int i, j, k;
    int rank, array_size, datasize;
    int *dimsizes, *dimsteps, *dimidx;
    char **srcPtrs;
    int ptid, retid;

    if (tp->isStructType())
    {
        cerr << "No structured types allowed!" << endl;
        return -1;
    }
    prim = (r_Primitive_Type*)tp;
    ptid = prim->type_id();
    cout << "Source domain = " << domain << ", type = " << TypeIDToName(ptid) << endl;

    switch (ptid)
    {
    case r_Primitive_Type::BOOL:
    case r_Primitive_Type::CHAR:
    case r_Primitive_Type::OCTET:
        datasize = 1;
        break;
    case r_Primitive_Type::SHORT:
    case r_Primitive_Type::USHORT:
        datasize = 2;
        break;
    case r_Primitive_Type::LONG:
    case r_Primitive_Type::ULONG:
    case r_Primitive_Type::FLOAT:
        datasize = 4;
        break;
    case r_Primitive_Type::DOUBLE:
        datasize = 8;
        break;
    default:
        cerr << "Unrecognized base type" << endl;
        return -1;
    }

    rank = domain.dimension();
    dimsizes = new int[rank];
    dimsteps = new int[rank];
    dimidx = new int[rank];
    srcPtrs = new char*[rank];
    for (i=0; i<rank; i++)
    {
        dimsizes[i] = domain[i].high() - domain[i].low() + 1;
        dimidx[i] = 0;
    }
    array_size = datasize;
    for (i=rank-1; i>=0; i--)
    {
        dimsteps[i] = array_size;
        array_size *= dimsizes[i];
    }
    src = new char[array_size];

    for (i=0; i<rank; i++) srcPtrs[i] = src;

    k = 0;
    do
    {
        //cout << (int)(srcPtrs[0] - src) << endl;

        // A bit inefficient, but customized code for the entire loop for each type
        // would result in way too much code for testing purposes only.
        switch (ptid)
        {
        case r_Primitive_Type::BOOL:
        case r_Primitive_Type::CHAR:
        case r_Primitive_Type::OCTET:
            *((char*)srcPtrs[0]) = (char)k;
            break;
        case r_Primitive_Type::SHORT:
        case r_Primitive_Type::USHORT:
            *((short*)srcPtrs[0]) = (short)k;
            break;
        case r_Primitive_Type::LONG:
        case r_Primitive_Type::ULONG:
            *((long*)srcPtrs[0]) = (long)k;
            break;
        case r_Primitive_Type::FLOAT:
            *((float*)srcPtrs[0]) = (float)k;
            break;
        case r_Primitive_Type::DOUBLE:
            *((double*)srcPtrs[0]) = (double)k;
            break;
        default:
            break;
        }
        k++;
        i = 0;
        do
        {
            dimidx[i]++;
            if (dimidx[i] < dimsizes[i]) break;
            dimidx[i] = 0;
            i++;
        }
        while (i < rank);
        if (i < rank)
        {
            srcPtrs[i] += dimsteps[i];
            if (i > 0)
            {
                // Init the array with the value x_0 + x_1 + ... + x_(n-1)
                k = dimidx[0];
                for (j=1; j<rank; j++) k += dimidx[j];
                for (j=i; j>0; j--)
                {
                    srcPtrs[j-1] = srcPtrs[j];;
                }
            }
        }
    }
    while (i < rank);

    hdf = new r_Conv_HDF(src, domain, tp);

    dest = NULL;

    try
    {
        desc = hdf->convertTo(params);
        dest = desc.dest;
        destInterv = desc.destInterv;
        destType = desc.destType;
    }
    catch(r_Error &err)
    {
        cerr << "Exception! " << err.what() << endl;
    }

    cout << "Encoded interval " << desc.destInterv << endl;

    delete hdf;

    if (dest != NULL)
    {
        try
        {
            hdf = new r_Conv_HDF(dest, destInterv, destType);

            desc = hdf->convertFrom();

            cout << "retrieved type = ";
            desc.destType->print_status();
            retid = ((r_Primitive_Type*)(desc.destType))->type_id();
            cout << ", (" << ((retid == ptid) ? "OK" : "Differs") << ')' << endl;
            cout << "retrieved domain " << desc.destInterv << ' ';
            i = 0;
            if (desc.destInterv.dimension() == rank)
            {
                for (i=0; i<rank; i++)
                {
                    if (desc.destInterv[i].high() - desc.destInterv[i].low() != domain[i].high() - domain[i].low())
                        break;
                }
            }
            if (i < rank)
            {
                cout << "Incompatible!" << endl;
                return -1;
            }
            cout << "(OK)" << endl;

            for (i=0; i<array_size; i++)
            {
                if (src[i] != desc.dest[i]) break;
            }
            if (i == array_size)
            {
                cout << "Data identical" << endl;
            }
            else
            {
                cout << "Data differs at " << i << endl;
            }

            cout << endl;

            delete destType;
            delete desc.destType;

            free(desc.dest);  // HDF^-1 ( HDF (X) )
        }
        catch(r_Error &err)
        {
            cerr << "Exception! " << err.what() << endl;
        }
    }

    delete hdf;

    if (dest != NULL)
        free(dest); // HDF (X)

    delete [] src;    // X

    delete [] dimsizes;
    delete [] dimsteps;
    delete [] dimidx;
    delete [] srcPtrs;

    // Delete the base type passed from the caller too
    delete tp;

    return 0;
}





int main(int argc, char *argv[])
{
    r_Minterval interv;
    r_Type *tp;

    // 3D data set over char
    interv = r_Minterval(3);
    interv << r_Sinterval(r_Range(0), r_Range(19))
           << r_Sinterval(r_Range(0), r_Range(29))
           << r_Sinterval(r_Range(0), r_Range(39));
    tp = r_Type::get_any_type("char");
    TestHDF(interv, tp);

    // 2D data set over short
    interv = r_Minterval(2);
    interv << r_Sinterval(r_Range(0), r_Range(100))
           << r_Sinterval(r_Range(0), r_Range(200));
    tp = r_Type::get_any_type("short");
    TestHDF(interv, tp);

    // 1D data set over long
    interv = r_Minterval(1);
    interv << r_Sinterval(r_Range(0), r_Range(400));
    tp = r_Type::get_any_type("long");
    TestHDF(interv, tp);

    // 2D data set over float
    interv = r_Minterval(2);
    interv << r_Sinterval(r_Range(0), r_Range(200))
           << r_Sinterval(r_Range(0), r_Range(300));
    tp = r_Type::get_any_type("float");
    TestHDF(interv, tp);

    // 2D data set over double
    interv = r_Minterval(2);
    interv << r_Sinterval(r_Range(100), r_Range(300))
           << r_Sinterval(r_Range(100), r_Range(400));
    tp = r_Type::get_any_type("double");
    TestHDF(interv, tp);

    // 1D data set over octet
    interv = r_Minterval(1);
    interv << r_Sinterval(r_Range(0), r_Range(200));
    tp = r_Type::get_any_type("octet");
    TestHDF(interv, tp);

    // 1D data set over bool
    interv = r_Minterval(1);
    interv << r_Sinterval(r_Range(0), r_Range(200));
    tp = r_Type::get_any_type("boolean");
    TestHDF(interv, tp);

    // 1D data set over unsigned short
    interv = r_Minterval(1);
    interv << r_Sinterval(r_Range(0), r_Range(200));
    tp = r_Type::get_any_type("ushort");
    TestHDF(interv, tp);

    // 1D data set over unsigned long
    interv = r_Minterval(1);
    interv << r_Sinterval(r_Range(0), r_Range(200));
    tp = r_Type::get_any_type("ulong");
    TestHDF(interv, tp);

    // 4D data set over char
    interv = r_Minterval(4);
    interv << r_Sinterval(r_Range(100), r_Range(149))
           << r_Sinterval(r_Range(110), r_Range(184))
           << r_Sinterval(r_Range(120), r_Range(219))
           << r_Sinterval(r_Range(130), r_Range(204));
    tp = r_Type::get_any_type("char");
    TestHDF(interv, tp);

    return 0;
}
