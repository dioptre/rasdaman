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

#include "tiler.hh"
#include "tilemgr/tile.hh"

r_Tiler::r_Tiler(std::vector<r_Minterval>& sourceDomain2s, const std::vector<r_Minterval>& targetDomain2s)
    :   sourceDomains(sourceDomain2s),
        targetDomains(targetDomain2s)
{
}

void
r_Tiler::mergeDomains()
{
    std::vector<r_Minterval>::iterator splitedIt;
    std::vector<r_Minterval> temp;
    r_Minterval tempDom;
    bool merged = false;
    while (!splitedDomains.empty())
    {
        merged = false;
        tempDom = *(splitedDomains.begin());
        splitedDomains.erase(splitedDomains.begin());
        for (splitedIt = splitedDomains.begin(); splitedDomains.end() != splitedIt; splitedIt++)
        {
            if (tempDom.is_mergeable(*splitedIt))
            {
                //cout << "is mergeable " << tempDom << " " << *splitedIt << endl;
                tempDom.closure_with(*splitedIt);
                //cout << "closure " << tempDom << endl;
                splitedDomains.erase(splitedIt);
                merged = true;
                break;
            }
            else
            {
                //cout << "is not mergeable " << tempDom << " " << *splitedIt << endl;
            }
        }
        if (merged)
            splitedDomains.push_back(tempDom);
        else
            temp.push_back(tempDom);
    }
    splitedDomains = temp;
}

void
r_Tiler::split()
{
    std::vector<r_Minterval>::iterator sourceIt;
    std::vector<r_Minterval>::iterator retvalIt;
    std::vector<r_Minterval> splits;
    std::vector<RangePair> points;

    for (sourceIt = sourceDomains.begin(); sourceIt != sourceDomains.end(); sourceIt++)
    {
        //cout << "starting with source domain " << *sourceIt << endl;
        points = computeSplitDimensions(*sourceIt);
        splits = splitMinterval(*sourceIt, points);
        for (retvalIt = splits.begin(); retvalIt != splits.end(); retvalIt++)
        {
            splitedDomains.push_back(*retvalIt);
        }
        //it is either splitted and in retval or not splitted and then there is an error, because not splitted were inserted
    }
}

std::vector<RangePair>
r_Tiler::computeSplitDimensions(const r_Minterval& sourceDomain) const
{
    r_Dimension dim = 0;
    r_Range slow = 0;
    r_Range shigh = 0;
    r_Range ilow = 0;
    r_Range ihigh = 0;

    std::vector<r_Minterval> intersects;
    std::vector<r_Minterval>::iterator intersectIt;
    std::vector<r_Minterval>::const_iterator targetIt;

    std::vector<RangePair> points;
    std::vector<RangePair>::iterator pointIt;
    RangePair pair;

    //cout << "\tfinding the intersections of current source with targets" << endl;
    for (targetIt = targetDomains.begin(); targetIt != targetDomains.end(); targetIt++)
    {
        if ((*targetIt).intersects_with(sourceDomain))
        {
            //cout << "\t\tsource intersected target " << *targetIt << endl;
            intersects.push_back((*targetIt).create_intersection(sourceDomain));
        }
        else
        {
            //cout << "\t\tsource did not intersect target " << *targetIt << endl;
        }
    }

    //cout << "\titerating through intersecting domains to determine the spliting dimensions" << endl;
    for (intersectIt = intersects.begin(); intersectIt != intersects.end(); intersectIt++)
    {
        //cout << "\t\tstarting to compute split dimensions with " << *intersectIt << endl;
        for (dim = 0; dim < (*intersectIt).dimension(); dim++)
        {
            ilow = (*intersectIt)[dim].low();
            ihigh = (*intersectIt)[dim].high();
            slow = (sourceDomain)[dim].low();
            shigh = (sourceDomain)[dim].high();
            //cout << "\t\t\tdimension " << dim << endl;
            //cout << "\t\t\tsource low  " << (sourceDomain)[dim].low() << "=" << slow << ", intersect low  " << (*intersectIt)[dim].low() << "=" << ilow << endl;
            //cout << "\t\t\tsource high " << (sourceDomain)[dim].high() << "=" << shigh << ", intersect high " << (*intersectIt)[dim].high() << "=" << ihigh << endl;
            if ((slow <= ilow) & (shigh >= ilow))
            {
                pair.first = dim;
                pair.second = ilow - 1;
                //cout << "\t\t\tadding " << pair.second << endl;
                points.push_back(pair);
            }
            if ((slow <= ihigh) & (shigh >= ihigh))
            {
                pair.first = dim;
                pair.second = ihigh;
                //cout << "\t\t\tadding " << pair.second << endl;
                points.push_back(pair);
            }
        }
        //cout << "\t\tfound all split dimensions for the current source with intersect " << *intersectIt << endl;
    }
    //cout << "\t\t\tfound all split dimensions for source " << sourceDomain << endl;
    //for (pointIt = points.begin(); pointIt != points.end(); pointIt++)
    //cout << "\t\t\tsplit in dimension " << (*pointIt).first << " at coordinate " << (*pointIt).second << endl;
    //cout << "try " << endl;
    std::vector<RangePair> temp;
    std::vector<RangePair>::iterator tempIt;
    RangePair tempP;
    bool tempadd = true;
    temp.swap(points);
    //for (pointIt = points.begin(); pointIt != points.end(); pointIt++)
    //cout << "\t\t\tsplit in dimension " << (*pointIt).first << " at coordinate " << (*pointIt).second << endl;
    //cout << "try end" << endl;
    while (!temp.empty())
    {
        tempadd = true;
        tempP = *(temp.begin());
        temp.erase(temp.begin());
        for (tempIt = temp.begin(); tempIt != temp.end(); tempIt++)
        {
            if ((tempP.first == (*tempIt).first) & (tempP.second == (*tempIt).second))
            {
                //cout << "matched" << endl;
                tempadd = false;
                break;
            }
        }
        if (tempadd)
            points.push_back(tempP);
    }
    //cout << "try end 2" << endl;
    //for (pointIt = points.begin(); pointIt != points.end(); pointIt++)
    //cout << "\t\t\tsplit in dimension " << (*pointIt).first << " at coordinate " << (*pointIt).second << endl;
    //cout << "try end 3" << endl;
    return points;
}

std::vector<r_Minterval>
r_Tiler::splitMinterval(const r_Minterval& sourceTile, std::vector<RangePair>& points)
{
    std::vector<r_Minterval> splits;
    std::vector<r_Minterval> split2s;
    std::vector<r_Minterval>::iterator splitIt;
    std::vector<r_Minterval>::iterator split2It;

    r_Minterval splitInterval1;
    r_Minterval splitInterval2;

    std::vector<RangePair>::iterator pointIt;

    bool addDomain = false;

    r_Dimension dim = 0;
    r_Range low = 0;
    r_Range high = 0;
    r_Range splitCoordinate = 0;

    //initialize the split std::vector.  the result will contain a list of splitted domains.
    splits.push_back(sourceTile);

    //cout << "\t\tstarting to actually split" << endl;
    for (pointIt = points.begin(); pointIt != points.end(); pointIt++)
    {
        split2s = std::vector<r_Minterval>();
        //cout << "\t\t\tstarting to split at dimension " << (*pointIt).first << " coordinate " << (*pointIt).second << endl;
        //cout << "\t\t\titerating through all the tiles to be split, stemming from current source tile" << endl;
        while (!splits.empty())
        {
            splitIt = splits.begin();
            //cout << "\t\t\t\tsplitting " << (*splitIt) << endl;
            addDomain = false;
            splitInterval1 = r_Minterval(((*splitIt)).dimension());
            splitInterval2 = r_Minterval(((*splitIt)).dimension());
            //cout << "\t\t\t\ttrying split" << endl;
            for (dim = 0; dim < ((*splitIt)).dimension(); dim++)
            {
                if ((dim == (*pointIt).first))
                {
                    high = (*splitIt)[dim].high();
                    low = (*splitIt)[dim].low();
                    splitCoordinate = (*pointIt).second;
                    if ((splitCoordinate < high) & (splitCoordinate > low))
                    {
                        //cout << "\t\t\t\t\tsplit middle adding [" << low << ":" << splitCoordinate << "]" << endl;
                        splitInterval1 << r_Sinterval(low, splitCoordinate);
                        //cout << "\t\t\t\t\tsplit middle adding [" << splitCoordinate+1 << ":" << high << "]" << endl;
                        splitInterval2 << r_Sinterval(splitCoordinate + 1, high);
                        addDomain = true;
                    }
                    else
                    {
                        if (splitCoordinate == high)
                        {
                            addDomain = true;
                            if (low == high)
                            {
                                //cout << "\t\t\t\t\tsplit high adding [" << low << ":" << low << "]" << endl;
                                splitInterval1 << r_Sinterval(low, high);
                            }
                            else
                            {
                                //cout << "\t\t\t\t\tsplit high adding [" << low << ":" << high-1<<"]" << endl;
                                splitInterval1 << r_Sinterval(low, splitCoordinate - 1);
                            }

                            splitInterval2 << r_Sinterval(splitCoordinate, splitCoordinate);
                        }
                        else
                        {
                            if ((*pointIt).second == ((*splitIt))[dim].low())
                            {
                                addDomain = true;
                                if (low == high)
                                {
                                    //cout<<"\t\t\t\t\tsplit low adding ["<<low<<":"<<high<< "]"<<endl;
                                    splitInterval1 << r_Sinterval(low, high);
                                }
                                else
                                {
                                    //cout<<"\t\t\t\t\tsplit low adding ["<<low+1<<":"<< high<< "]"<<endl;
                                    splitInterval1 << r_Sinterval(low + 1, high);
                                }
                                splitInterval2<< r_Sinterval((*pointIt).second,(*pointIt).second);
                            }
                            else
                            {
                                //cout << "\t\t\t\t\tno match, break" << endl;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    //cout << "\t\t\t\t\tdimension unmatched, adding " << ((*splitIt))[dim] << endl;
                    splitInterval1 << ((*splitIt))[dim];
                    splitInterval2 << ((*splitIt))[dim];
                }
            }
            if (addDomain)
            {
                //cout << "\t\t\t\tadding split domain 1 " << splitInterval1 << endl;
                split2s.push_back(splitInterval1);
                //cout << "\t\t\t\tadding split domain 2 " << splitInterval2 << endl;
                split2s.push_back(splitInterval2);
            }
            else
            {
                //cout << "\t\t\t\tnot adding split domains " << splitInterval1 << " " << splitInterval1 << endl;
                //cout << "\t\t\t\tadding because of no splits " << (*splitIt) << endl;
                split2s.push_back(*splitIt);
            }
            splits.erase(splitIt);
        }
        splits = split2s;
    }
    //cout << "splitMinterval computed" << endl;
    //for (splitIt = splits.begin(); splitIt != splits.end(); splitIt++)
    //  {
    //cout << (*splitIt) << endl;
    //  }
    return splits;
}

void
r_Tiler::removeCoveredDomains()
{
    r_Minterval temp;
    std::vector<r_Minterval> retval;
    bool kill = false;
    std::vector<r_Minterval>::iterator targetIt;
    retval.swap(splitedDomains);
    while (!retval.empty())
    {
        temp = *(retval.begin());
        retval.erase(retval.begin());
        kill = false;
        for (targetIt = retval.begin(); targetIt != retval.end(); targetIt++)
        {
            if ((*targetIt).intersects_with(temp))
            {
                kill = true;
                break;
            }
        }
        if (!kill)
            splitedDomains.push_back(temp);
    }
}

void
r_Tiler::removeDoubleDomains()
{
    r_Minterval temp;
    std::vector<r_Minterval> retval;
    bool kill = false;
    std::vector<r_Minterval>::iterator targetIt;
    while (!splitedDomains.empty())
    {
        temp = *(splitedDomains.begin());
        splitedDomains.erase(splitedDomains.begin());
        kill = false;
        for (targetIt = targetDomains.begin(); targetIt != targetDomains.end(); targetIt++)
        {
            if ((*targetIt).intersects_with(temp))
            {
                kill = true;
                break;
            }
        }
        if (!kill)
            retval.push_back(temp);
    }
    retval.swap(splitedDomains);
}

std::vector<r_Minterval>
r_Tiler::getTiledDomains() const
{
    return splitedDomains;
}

std::vector<Tile*>
r_Tiler::generateTiles(const std::vector<Tile*>& sourceTiles) const
{
    std::vector<r_Minterval>::const_iterator splitedDomIt;
    std::vector<Tile*>::const_iterator sourceTileIt;
    std::vector<Tile*> retval;
    r_Minterval dummy;
    Tile* p = 0;
    const BaseType* basetype = (*sourceTiles.begin())->getType();
    r_Data_Format dataformat = (*sourceTiles.begin())->getDataFormat();

    for (splitedDomIt = splitedDomains.begin(); splitedDomIt != splitedDomains.end(); splitedDomIt++)
    {
        dummy = *splitedDomIt;
        p = new Tile(dummy, basetype, dataformat);
        //std::cout << "new tile " << dummy << " " << basetype->getName() << " " << dataformat << " size " << p->getSize() << " other size " << p->getDBTile()->getSize() << std::endl;
        for (sourceTileIt = sourceTiles.begin(); sourceTileIt != sourceTiles.end(); sourceTileIt++)
        {
            //std::cout << " the other tile domain " << (*sourceTileIt)->getDomain() << " type " << (*sourceTileIt)->getType()->getName() << std::endl;
            if (dummy.intersects_with((*sourceTileIt)->getDomain()))
            {
                const r_Minterval& updateDomain = dummy.create_intersection((*sourceTileIt)->getDomain());
                //std::cout << "  they intersect.  on " << updateDomain << std::endl;
                //UnaryOp* tempOp = Ops::getUnaryOp(Ops::OP_IDENTITY, p->getType(), (*sourceTileIt)->getType(), 0, 0);
                //causes fmr/abr/umr
                p->copyTile(updateDomain, *sourceTileIt, updateDomain);
                //p->execUnaryOp(tempOp, dummy.create_intersection((*sourceTileIt)->getDomain()), *sourceTileIt, dummy.create_intersection((*sourceTileIt)->getDomain()));
                //delete tempOp;
            }
        }
        retval.push_back(p);
    }
    return retval;
}

