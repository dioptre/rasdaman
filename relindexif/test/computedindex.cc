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
#include <stdlib.h>
#include "raslib/rmdebug.hh"
#include "raslib/rminit.hh"
#include "raslib/minterval.hh"

RMINITGLOBALS('C')

/*
    You need to know the defined domain:
        check if the tile config fits the defined domain
        check if the requested/inserted tile fits into the defined domain
    You need to know the tile config:
        check if the inserted tile has the proper domain
    You need to know the normalized domain:
        it is translated to the origin
        its extent is divided by the extent of the tile config
    You request a point in the index:
        the point is translated to the normalized domain
        the point is normalized (divided by the tile config extent)
        you get the cell offset of this point from the normalized domain
        you add the base counter to the cell offset
    You request a domain in the index
        the domain is translated to the normalized domain
        the domain is normalized (divided by the tile config extent)
        for each point in the resulting domain you do a point query
*/

r_Minterval
createNormalizedDomain(const r_Point& mddDomainExtent, const r_Point& tileConfigExtent)
{
    r_Dimension theDim = mddDomainExtent.dimension();
    r_Minterval normalizedDomain(theDim);
    r_Range normalized = 0;

    for (r_Dimension dim = 0; dim < theDim; dim++)
    {
        normalized = (r_Range)(mddDomainExtent[dim]/tileConfigExtent[dim]) - 1;
        cout << "mdd domain extent [" << dim << "]  " << mddDomainExtent[dim] << endl;
        cout << "tile config extent [" << dim << "] " << tileConfigExtent[dim] << endl;
        cout << "division                           " << mddDomainExtent[dim]/tileConfigExtent[dim] << endl;
        if ((normalized + 1)* tileConfigExtent[dim] != mddDomainExtent[dim])
        {
            cout << "got you" << endl;
        }
        normalizedDomain[dim] = r_Sinterval(0, normalized);
    }
    return normalizedDomain;
}

r_Point
createNormalizedPoint(const r_Point& toNormalize, const r_Point& tileConfigExtent, const r_Point& mddDomainOrigin)
{
    r_Dimension theDim = mddDomainOrigin.dimension();
    r_Point normalizedPoint(theDim);

    for (r_Dimension dim = 0; dim < theDim; dim++)
    {
        normalizedPoint[dim] = (r_Range)((toNormalize[dim] - mddDomainOrigin[dim])/tileConfigExtent[dim]);
    }
    return normalizedPoint;
}

void
main(unsigned int argc, const char** argv)
{
    unsigned int baseCounter = 0;
    unsigned int entryType = 0;
    r_Area index = 0;

    r_Minterval tileConfig("[0:1,0:2]");

    r_Minterval mddDomain("[10:21,0:11]");

    r_Point whereToSearch("[11,9]");

    r_Minterval normalizedDomain = createNormalizedDomain(mddDomain.get_extent(), tileConfig.get_extent());
    cout << "normalized domain " << normalizedDomain << endl;

    r_Point normalizedSearch = createNormalizedPoint(whereToSearch, tileConfig.get_extent(), mddDomain.get_origin());
    cout << "normalized search " << normalizedSearch << endl;
    cout << "index of search point " << normalizedDomain.cell_offset(normalizedSearch) << endl;

}

