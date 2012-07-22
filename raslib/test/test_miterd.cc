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
 * SOURCE: test_miter.cc
 *
 * MODULE: raslib
 *
 ************************************************************/

#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "raslib/miterd.hh"
#include "raslib/minterval.hh"
#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"

RMINITGLOBALS('C')

using namespace std;

int
main(int i, char** argv)
{
    unsigned short src[][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    for (r_Dimension i = 0; i < 4; i++)
    {
        for (r_Dimension c = 0; c < 4; c++)
        {
            cout << src[i][c] << " ";
        }
        cout << endl;
    }
    for (r_Dimension i = 0; i < 4; i++)
    {
        for (r_Dimension c = 0; c < 4; c++)
        {
            cout << src[c][i] << " ";
        }
        cout << endl;
    }
    /*
    r_MiterDirect iter(src, r_Minterval("[0:3,0:3]"), r_Minterval("[0:3,0:3]"), 2, 0);
    r_Dimension order[] = {1,1};
    unsigned int step[] = {1,0};
    r_Dimension order2[] = {0};
    while (!iter.isDone())
        {
        iter.iterateUserOrder(order, step);
        cout << *(unsigned short*)(iter.getData()) << " " << endl;;
        cout << "pos " << iter << endl;
    //  cout << *(unsigned short*)(iter.getData()) << " ";
        iter.operator++();
        }
    */
}
