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


#include "storagemgr/stgmddconfig.hh"

using namespace std;

/****************************************************************************
 *
 *
 * INCLUDE: stgmddconfig.hh
 *
 * MODULE:  qlparser
 * CLASS:   StgMddConfig
 *
 * CHANGE HISTORY (append further entries):
 * when         who        what
 * -----------------------------------------------------------------------
 * 17-Aug-09	aaiordachi	create class (for adding rasql tiling options)
 * COMMENTS:
 *
 ****************************************************************************/

StgMddConfig::StgMddConfig()
{}
StgMddConfig::~StgMddConfig()
{}

// Setter functions 
void StgMddConfig::setBBoxes(const vector<r_Minterval>& input)
{
	bboxes = input;
}

void StgMddConfig::setSubTiling()
{
	subTiling = true;
}

void StgMddConfig::resetSubTiling()
{
	subTiling = false;
}

void StgMddConfig::setInterestThreshold(double i)
{
	interestThreshold = i;
}

void StgMddConfig::setBorderThreshold(unsigned int b)
{
	borderThreshold = b;
}

void StgMddConfig::setCellSize(int size)
{
	cellSize = size;
}

void StgMddConfig::setDirDecompose(const vector<r_Dir_Decompose>& input)
{
	dirDecomp = input;
}

// Getter functions 
vector<r_Minterval>& StgMddConfig::getBBoxes()
{
	return bboxes;
}

bool StgMddConfig::getSubTiling()
{
	return subTiling;
}

double StgMddConfig::getInterestThreshold()
{
	return interestThreshold;
}

unsigned int StgMddConfig::getBorderThreshold()
{
	return borderThreshold;
}

int StgMddConfig::getCellSize()
{
	return cellSize;
}

vector<r_Dir_Decompose>& StgMddConfig::getDirDecompose()
{
	return dirDecomp;
}
	
