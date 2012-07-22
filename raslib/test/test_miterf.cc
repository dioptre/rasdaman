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
 * SOURCE: test_miterf.cc
 *
 * MODULE: raslib
 *
 * PURPOSE:
 *
 * COMMENTS:
 *
 ************************************************************/


#include <iostream>
#include <math.h>
#include <stdlib.h>

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "raslib/template_inst.hh"
#include "raslib/mddtypes.hh"
#include "raslib/miterf.hh"
#include "raslib/minterval.hh"
#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"

using namespace std;


int main(int argc, char *argv[])
{
    r_Dimension dim = 3;
    r_Minterval iv(dim);
    r_Dimension i;
    double *iterMin, *iterMax, *iterStep;
    int typeLength = 1;
    long totalSize = typeLength;
    long totalSteps = 1;
    double stepBy = 1.5;

    i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-s") == 0)
        {
            stepBy = atof(argv[++i]);
        }
        i++;
    }

    cout << "Step by " << stepBy << endl;

    iterMin = new double[dim];
    iterMax = new double[dim];
    iterStep = new double[dim];
    for (i=0; i<dim; i++)
    {
        int steps;

        iterStep[i] = stepBy;
        iterMin[i] = 0;
        iterMax[i] = (1<<(4+i)) - 1;
        iv << r_Sinterval((r_Range)(iterMin[i]), (r_Range)(iterMax[i]));
        totalSize *= (r_Range)(iterMax[i]) - (r_Range)(iterMin[i]) + 1;
        steps = (int)((iterMax[i] - iterMin[i]) / iterStep[i]);
        totalSteps *= (steps + 1);
        iterMax[i] = iterMin[i] + (steps + 0.5)*iterStep[i]; // rounding effects
    }

    char *srcData = new char[totalSize];

    cout << "Total size: 0x" << hex << totalSize
         << ", base address " << (void*)srcData << endl;

    r_MiterFloat iter(&iv, iterMin, iterMax, iterStep, typeLength, srcData);

    long steps = 0;
    while (!iter.isDone())
    {
        char *cell = iter.nextCell();

        if (cell + typeLength > srcData + totalSize)
        {
            cout << dec << "Overflow by " << (cell - srcData) - totalSize << endl;
        }
        steps++;
    }

    cout << dec << "Did " << steps << " steps out of " << totalSteps << endl;
    if (steps != totalSteps)
        cout << "!!! WRONG NUMBER OF STEPS !!!" << endl;

    delete [] srcData;

    delete [] iterMin;
    delete [] iterMax;
    delete [] iterStep;

    return 0;
}
