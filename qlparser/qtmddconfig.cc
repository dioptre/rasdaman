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

/*************************************************************
 *
 * CHANGE HISTORY (append further entries):
 * when         who             what
 * ----------------------------------------------------------
 * 10-11-08     Shams      Created the class
 * 14-08-09     Andrei      Reimplemented the class
 * COMMENTS:
 *
 ************************************************************/

#include "qlparser/qtmddcfgop.hh"
#include "qlparser/qtmddconfig.hh"
#include "qlparser/qtconst.hh"
#include "qlparser/qtdata.hh"
#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif
#include <fstream>


QtMDDConfig::QtMDDConfig() : QtData()
{
}

QtDataType QtMDDConfig::getDataType() const
{
    return QT_MDD_CONFIG;
}

char* QtMDDConfig::getTypeStructure() const
{
    return NULL;
}

bool QtMDDConfig::equal(const QtData* p) const
{
    bool eq = false;
    return eq;
}

string QtMDDConfig::getSpelling() const
{
    return "";
}

QtMDDConfig::QtMDDConfig(int tilingType, int tileSize, int borderThreshold,
                         double interestThreshold, QtOperation* tileCfg, QtNode::QtOperationList* box, std::vector<r_Dir_Decompose>* dDecomp,
                         int indexType)
{
    this->tilingType = tilingType;
    this->tileSize = tileSize;
    this->borderThreshold = borderThreshold;
    this->interestThreshold = interestThreshold;
    this->tileCfg = tileCfg;
    this->box = box;
    this->dDecomp = dDecomp;
    this->indexType = indexType;
}

QtMDDConfig::QtMDDConfig(int tilingType, int tileSize, int borderThreshold,
                         double interestThreshold, QtOperation* tileCfg, QtNode::QtOperationList* box, std::vector<r_Dir_Decompose>* dDecomp)
{
    this->tilingType = tilingType;
    this->tileSize = tileSize;
    this->borderThreshold = borderThreshold;
    this->interestThreshold = interestThreshold;
    this->tileCfg = tileCfg;
    this->box = box;
    this->dDecomp = dDecomp;
    this->indexType = QtMDDConfig::r_DEFAULT_INDEX;
}

QtMDDConfig::QtMDDConfig(int index)
    : QtData()
{
    this->tilingType = this->r_DEFAULT_TLG;
    this->tileSize = -1;
    this->borderThreshold = -1;
    this->interestThreshold = -1;
    this->tileCfg = NULL;
    this->box = NULL;
    this->dDecomp = NULL;
    this->indexType = index;
}

int QtMDDConfig::getTileSize()
{
    return tileSize;
}

int QtMDDConfig::getBorderThreshold()
{
    return borderThreshold;
}

double QtMDDConfig::getInterestThreshold()
{
    return interestThreshold;
}

std::vector<r_Dir_Decompose>* QtMDDConfig::getDirDecomp()
{
    return dDecomp;
}

int QtMDDConfig::getStorageType()
{
    return QtMDDConfig::r_DEFAULT_STG;
}

int QtMDDConfig::getWltValue()
{
    return param;
}

int QtMDDConfig::getIndexType()
{
    return indexType;
}

int QtMDDConfig::getTilingType()
{
    return tilingType;
}

QtNode::QtOperationList* QtMDDConfig::getBboxList()
{
    return box;
}

QtOperation* QtMDDConfig::getTileCfg()
{
    return tileCfg;
}

void QtMDDConfig::setBorderThreshold(int threshold)
{
    borderThreshold = threshold;
}

