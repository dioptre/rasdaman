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

#ifndef _D_TILER_
#define _D_TILER_

#include "raslib/minterval.hh"
#include <vector>
#include <utility>
//#include<pair>

class Tile;
class PersTile;
class TransTile;

typedef std::pair<r_Range,r_Range> RangePair;
typedef std::pair<const r_Range,const r_Range> ConstRangePair;

/**
  * \ingroup Tilemgrs
  */
class r_Tiler
{
public:
    r_Tiler(std::vector<r_Minterval>& sourceDomains, const std::vector<r_Minterval>& targetDomains);
    /*
    sourceDomains will be splitted.
    targetDomains will not be touched.
    */

    void split();
    /*
    will split the source domains.
    */

    void removeCoveredDomains();
    /*
    removes those splited domains which are covered by target domains.
    */

    void removeDoubleDomains();
    /*
    removes those splited domains which are covered by other splited domains.
    */

    void mergeDomains();
    /*
    tries to merge splited domains into larger domains.
    */

    std::vector<r_Minterval> getTiledDomains() const;
    /*
    returns the computed domains.  you must call split() first.
    */

    std::vector<Tile*> generateTiles(const std::vector<Tile*>& sourceTiles) const;
    /*
    uses the previously computed tiled domains to generate the output.
    call split, removeCoveredDomains, removeDoubleDomains, mergeDomains first!
    memory must be freed by the caller!
    */

private:

    std::vector<r_Minterval> splitMinterval(const r_Minterval& sourceDomain, std::vector<RangePair>& splitDimensions);

    std::vector<RangePair> computeSplitDimensions(const r_Minterval& sourceDomain) const;

    std::vector<r_Minterval> splitedDomains;

    std::vector<r_Minterval> sourceDomains;

    std::vector<r_Minterval> targetDomains;
};
#endif

