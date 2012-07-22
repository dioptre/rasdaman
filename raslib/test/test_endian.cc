/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
*/

/*************************************************************
 *
 * INCLUDE: test_endian.cc
 *
 * MODULE:  raslib
 *
 * PURPOSE:
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "raslib/template_inst.hh"
#include "raslib/basetype.hh"
#include "raslib/primitivetype.hh"
#include "raslib/minterval.hh"
#include "raslib/rminit.hh"
#include "raslib/endian.hh"


static void print_numbers(const unsigned char *data, int size)
{
    int i;

    for (i=0; i<size; i++)
    {
        if ((i & 15) == 0)
        {
            if (i != 0)
                printf("\n");

            printf("%04x: ", i);
        }
        printf("%02x ", data[i]);
    }
    printf("\n");
    fflush(stdout);
}

static void test_endian(const char *schema, const r_Minterval &dom)
{
    r_Base_Type *type;
    r_Primitive_Type *primType;
    unsigned char *srcArray;
    unsigned char *testArray;
    unsigned char *smallArray;
    unsigned long size;
    unsigned long smallSize;
    unsigned long i;

    std::cout << "test type <" << schema << ">" << endl;

    type = (r_Base_Type*)r_Type::get_any_type(schema);
    primType = (r_Primitive_Type*)r_Type::get_any_type("long");

    size = type->size() * dom.cell_count();

    srcArray = new unsigned char[size];

    for (i=0; i<size; i++)
        srcArray[i] = (unsigned char)(i & 0xff);

    r_Minterval iterDom(dom.dimension());
    for (i=0; i<dom.dimension(); i++)
        iterDom << r_Sinterval(dom[i].low(), (r_Range)(dom[i].low() + (dom[i].high() - dom[i].low() + 1) / 2));

    std::cout << "dom = " << dom << ", iterDom = " << iterDom << endl;

    print_numbers(srcArray, 64);

    smallSize = type->size() * iterDom.cell_count();

    testArray = new unsigned char[size];
    memset(testArray, 0, size);
    smallArray = new unsigned char[smallSize];
    memset(smallArray, 0, smallSize);

    std::cout << "Linear change..." << endl;
    r_Endian::swap_array(primType, size, srcArray, testArray);
    print_numbers(testArray, 64);

    std::cout << "Semi-generic change, full..." << endl;
    r_Endian::swap_array(type, dom, dom, srcArray, testArray);
    print_numbers(testArray, 64);

    std::cout << "Semi-generic change, half..." << endl;
    r_Endian::swap_array(type, dom, iterDom, srcArray, testArray);
    print_numbers(testArray, 64);

    std::cout << "Fully generic change, full..." << endl;
    r_Endian::swap_array(type, dom, dom, dom, dom, srcArray, testArray);
    print_numbers(testArray, 64);

    std::cout << "Fully generic change, half..." << endl;
    r_Endian::swap_array(type, dom, iterDom, iterDom, iterDom, srcArray, smallArray);
    print_numbers(smallArray, 64);

    delete type;
    delete primType;

    delete [] smallArray;
    delete [] testArray;
    delete [] srcArray;
}


int main(int argc, char *argv[])
{
    r_Minterval dom(2);

    dom << r_Sinterval((r_Range)0, (r_Range)123)
        << r_Sinterval((r_Range)10, (r_Range)456);

    test_endian("char", dom);
    test_endian("short", dom);
    test_endian("long", dom);
    test_endian("float", dom);
    test_endian("double", dom);
    test_endian("struct {short, short, long}", dom);

    return 0;
}
