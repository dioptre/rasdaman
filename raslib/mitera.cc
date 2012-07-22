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
/
/**
 * SOURCE: mitera.cc
 *
 * MODULE: raslib
 * CLASS:  r_MiterArea
 *
*/

#include "raslib/mitera.hh"
#include "raslib/minterval.hh"
#include "raslib/rmdebug.hh"

r_MiterArea::r_MiterArea( const r_Minterval* newIterDom,
                          const r_Minterval* newImgDom ) throw(r_Error)
    : iterDom(newIterDom), imgDom(newImgDom), done(false)
{
    RMDBGENTER( 1, RMDebug::module_raslib, "r_MiterArea", "r_MiterArea()");

    if (imgDom->dimension() != iterDom->dimension())
    {
        //in this case we have an undefined situation
        RMInit::logOut << "r_MiterArea::rMiterArea(" << iterDom << ", " << imgDom << ") dimension mismatch" << endl;
        throw r_Error(INTERVALSWITHDIFFERENTDIMENSION);
    }

    if(!imgDom->is_origin_fixed() ||
            !imgDom->is_high_fixed())
    {
        //in this case we have an undefined situation
        RMInit::logOut << "r_MiterArea::rMiterArea(" << iterDom << ", " << imgDom << ") imgDom is opened" << endl;
        throw r_Error(INTERVALOPEN);
    }

    if(!iterDom->is_origin_fixed() ||
            !iterDom->is_high_fixed())
    {
        //in this case we have an undefined situation
        RMInit::logOut << "r_MiterArea::rMiterArea(" << iterDom << ", " << imgDom << ") iterDom is opened" << endl;
        throw r_Error(INTERVALOPEN);
    }


    // dimensionality of both iterDom and imgDom
    r_Dimension dim = imgDom->dimension();
    // stores the increments
    incArrIter = new incArrElem[dim];

    for(r_Dimension i=0; i<dim; i++ )
    {
        // used for counting in iteration, initialize with 0
        incArrIter[i].curr = 0;
        // how often is the iterDom moved inside the imgDom
        incArrIter[i].repeat = (imgDom->get_extent()[i] / iterDom->get_extent()[i]) +
                               ( imgDom->get_extent()[i] % iterDom->get_extent()[i] != 0 );

        RMDBGMIDDLE( 4, RMDebug::module_raslib, "r_MiterArea", "repeat dim " << i << ": " << incArrIter[i].repeat );
    }
    reset();
    RMDBGEXIT( 1, RMDebug::module_raslib, "r_MiterArea", "r_MiterArea()" );
}


r_MiterArea::~r_MiterArea()
{
    delete [] incArrIter;
}

void
r_MiterArea::reset()
{
    done = false;
    for( int i=0; i<iterDom->dimension(); i++ )
    {
        incArrIter[i].curr = 0;
    }
}

r_Minterval
r_MiterArea::nextArea()
{
    r_Dimension i = 0;

    if(done)
        return retVal;

    r_Minterval currDom(iterDom->dimension());

    // calculate new result domain here
    if(!done)
    {
        for(i=0; i < iterDom->dimension(); i++)
        {
            currDom << r_Sinterval( (*imgDom)[i].low() + incArrIter[i].curr*iterDom->get_extent()[i],
                                    (*imgDom)[i].low() + (incArrIter[i].curr+1)*iterDom->get_extent()[i]
                                    - 1 );
        }
    }
    retVal = currDom.intersection_with((*imgDom));

    // increment dimensions
    for(i=0; i < iterDom->dimension(); i++)
    {
        incArrIter[i].curr++;
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
    if( i == iterDom->dimension() )
    {
        // overflow in last dimension
        done = true;
    }

    return retVal;
}

bool
r_MiterArea::isDone()
{
    return done;
}
