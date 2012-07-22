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

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "raslib/template_inst.hh"
#include "raslib/miter.hh"
#include "raslib/minterval.hh"
#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"

using namespace std;

// structure storing information on iteration for each dimension
// (perhaps add dimension for reordering later)
typedef struct
{
    int repeat; // total number of repeats
    int inc;    // increment per repeat
    int curr;   // current repeat
} incArrElem;

// for repeating inside the test functions
const int numRepeat = 100;

r_Minterval
createCube(int size, int dim)
{
    int i;
    int c = pow((double)size, 1.0/(double)dim);

    r_Minterval res(dim);

    for(i=0; i<dim; i++)
        res << r_Sinterval(0, c-1);

    i = 0;
    while(res.cell_count() < size)
    {
        res[i].set_high(res[i].high() + 1);
        i++;
    }
    if(res.cell_count() > size)
        res[i-1].set_high(res[i-1].high() - 1);

    return res;
}

void
test_Miter( r_Minterval& m1, r_Minterval& m2, char* data )
{
    char* currCell;

    // just to do something inside the loop
    unsigned long sum = 0;

    cout <<"Iteration r_Miter: ";

    // for performance measurement
    RMTimer mIterTimer("Iterator","r_Miter" );
    mIterTimer.start( );

    r_Miter iter(&m2, &m1, 4, (char*)data);
    for(int i=0; i<numRepeat; i++)
    {
        iter.reset();
        while(!iter.isDone())
        {
            currCell = iter.nextCell();
            sum += *(long*)currCell;
        }
    }

    mIterTimer.stop( );
    cout << sum << endl;
}

void
test_DirectIter( r_Minterval& m1, r_Minterval& m2, char* data )
{
    int opSize = 4;
    int dim = m1.dimension();
    int i;
    char* currCell;

    // just to do something inside the loop
    unsigned long sum = 0;

    incArrElem* incArrIter;

    cout <<"Iteration direct: ";

    // for performance measurement
    RMTimer iterTimer("Iterator","directIter" );
    iterTimer.start( );

    for(int r=0; r<numRepeat; r++)
    {

        // stores the increments
        incArrIter = new incArrElem[dim];

        currCell = (char*)data;

        // the following initializes incArrIter and calculates the first offset
        int tIncIter = 1;     // total increment for current dimension
        int prevTIncIter = 1; // total increment for previous dimension
        int incIter = 4;      // current increment, corresponds to cell size
        int firstOff = 0;

        for( i=0; i<dim; i++ )
        {
            // in RasDaMan the order of dimensions is the other way round!
            int r = dim - i - 1;
            // used for counting in iteration, initialize with 0
            incArrIter[i].curr = 0;
            // how often is the increment added?
            incArrIter[i].repeat = m2[r].high() - m2[r].low() + 1;
            // the increment for the result tile (higher dimensions calculated
            // further down)
            incArrIter[i].inc = incIter;

            // calculate starting offset and increments for higher dimensions
            // firstOff is the offset in chars of the first cell
            firstOff += (m2[r].low()-m1[r].low()) * prevTIncIter * 4;
            // tInc is the increment if the dimension would be skipped
            tIncIter = (m1[r].high() - m1[r].low()+1) * prevTIncIter;
            // inc is the real increment, after some cells in the dimensions
            // have been iterated through.
            incIter = (tIncIter - incArrIter[i].repeat*prevTIncIter) * 4;
            // remember total increment of last dimension
            prevTIncIter = tIncIter;
        }

        currCell += firstOff;

        int done = 0;
        // get first adresses

        while(!done)
        {
            // iterate through lowest dimension
            for(i=0; i<incArrIter[0].repeat; i++)
            {
                // execute operation
                sum += *(long*)currCell;
                // increment adresses
                currCell += incArrIter[0].inc;
            }
            // increment other dimensions
            for(i=1; i<dim; i++)
            {
                incArrIter[i].curr++;
                currCell += incArrIter[i].inc;
                if(incArrIter[i].curr < incArrIter[i].repeat)
                {
                    // no overflow in this dimension
                    break;
                }
                else
                {
                    // overflow in this dimension
                    incArrIter[i].curr = 0;
                }
            }
            if( i == dim )
            {
                // overflow in last dimension
                done = 1;
            }
        }
        delete [] incArrIter;
    }
    iterTimer.stop();
    cout << sum << endl;
}

void
test_OldIter( r_Minterval& m1, r_Minterval& m2, char* data )
{
    // just to do something inside the loop
    unsigned long sum = 0;

    cout <<"Iteration oldIter: ";
    // for performance measurement
    RMTimer oldIterTimer("Iterator","oldIter" );
    oldIterTimer.start( );

    r_Point pOp(m2.dimension());
    int done;
    int recalc;
    int i, j;
    const int opSize = 4;
    int dim = m2.dimension();
    int innerExtent = (m2.get_extent())[dim-1];
    char* cellOp;

    // initialize points
    for(i = 0; i < dim; i++)
    {
        pOp << 0;
    }

    for(int r=0; r<numRepeat; r++)
    {
        done = 0;
        recalc = 0;

        // initialize points
        for(i = 0; i < dim; i++)
        {
            pOp[i] = m2[i].low();
        }

        cellOp = (char*)data + m1.cell_offset(pOp)*4;

        // iterate over all cells
        while(!done)
        {
            if( recalc )
            {
                cellOp = (char*)data + m1.cell_offset(pOp)*4;
                recalc = 0;
            }

            // iterate through innermost dimension
            for(j = 0; j < innerExtent; j++ )
            {
                // execute operation on cell
                sum += *(long*)cellOp;
                cellOp += opSize;
            }

            // increment coordinates
            i = dim - 2;
            // special case! 1-D operands!
            if( i < 0 )
                break;
            ++pOp[i];
            recalc = 1;
            while( pOp[i] > m2[i].high() )
            {
                pOp[i] = m2[i].low();
                i--;
                if(i < 0)
                {
                    done = 1;
                    break;
                }
                ++pOp[i];
            }
        }
    }

    oldIterTimer.stop();
    cout << sum << endl;
}

void
test_CppIter( unsigned long cells, char* data )
{
    unsigned long sum = 0;
    RMTimer cppIterTimer("Iterator", "cppIter");
    cppIterTimer.start();

    for(int r=0; r<numRepeat; r++)
    {
        for(int i = 0; i<1048576; i++)
        {
            sum += data[i];
        }
    }

    cppIterTimer.stop();
    cout <<"Iteration C++: " << sum << endl;
}

int main()
{
    const unsigned long noCells = 1048576;
    unsigned long* data;

    for(int dim=1; dim<=7; dim++)
    {
        r_Minterval m2;
        r_Minterval m1(dim);

        m2 =  createCube(noCells, dim);

        for(int i=0; i<dim; i++)
        {
            m1 << r_Sinterval(m2[i].low()-1, m2[i].high()+1);
        }

        // as basis for the operations
        data = new unsigned long[m1.cell_count()];
        for(int i=0; i < m1.cell_count(); i++)
        {
            data[i] = i;
        }

        cout <<"Iterate through " << m2 << " in "<< m1 << endl;

        RMInit::bmOut << "Dimensionality: " << dim << ", cells: "
                      << m2.cell_count() << endl;

        for(int i=0; i<5; i++)
        {
            test_Miter( m1, m2, (char*)data );
            test_OldIter( m1, m2, (char*)data );
            test_DirectIter( m1, m2, (char*)data );
        }
        delete [] data;
    }

    RMInit::bmOut << "1-D Iteration in C++:" << endl;

    data = new unsigned long[noCells];

    for(int i=0; i < noCells; i++)
    {
        data[i] = i;
    }

    for(int i=0; i<5; i++)
    {
        test_CppIter( noCells, (char*)data );
    }
    delete [] data;
}
