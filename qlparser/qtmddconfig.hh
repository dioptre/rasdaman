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
#ifndef _QTMDDCONFIG_HH_
#define _QTMDDCONFIG_HH_

#include "qlparser/qtconst.hh"
#include "rasodmg/dirdecompose.hh"
#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif
#include<fstream>


class QtMDDConfig: public QtData
{
private:
    int tilingType;
    int tileSize;
    int borderThreshold;
    double interestThreshold;
    QtOperation* tileCfg;
    QtNode::QtOperationList* box;
    std::vector<r_Dir_Decompose>* dDecomp;
    int indexType;
    int param;      // wavelet param

public:
    // Constructors
    QtMDDConfig();
    QtMDDConfig( int tilingType, int tileSize, int borderThreshold,
                 double interestThreshold, QtOperation* tileCfg,
                 QtNode::QtOperationList* box,std::vector<r_Dir_Decompose>* dDecomp,
                 int indexType);
    QtMDDConfig( int tilingType, int tileSize, int borderThreshold,
                 double interestThreshold, QtOperation* tileCfg,
                 QtNode::QtOperationList* box,std::vector<r_Dir_Decompose>* dDecomp);
    QtMDDConfig(int index);

    // Getters
    int getTileSize();
    int getBorderThreshold();
    double getInterestThreshold();
    std::vector<r_Dir_Decompose>* getDirDecomp();
    int getStorageType();
    int getWltValue();      // Check return type !
    int getIndexType();
    int getTilingType();
    QtNode::QtOperationList* getBboxList();
    QtOperation* getTileCfg();

    // Setters
    void setBorderThreshold(int threshold);

    // Pure virtual methods definition (from class QtData)
    virtual QtDataType getDataType() const;
    virtual char* getTypeStructure() const;
    virtual bool equal(const QtData*) const;
    virtual string getSpelling() const;

    enum QtIndexConstants
    {
        r_DEFAULT_INDEX,
        r_A_INDEX,
        r_D_INDEX,
        r_IT_INDEX,
        r_RC_INDEX,
        r_RD_INDEX,
        r_RPT_INDEX,
        r_RRPT_INDEX,
        r_TC_INDEX
    };

    enum QtTilingConstants
    {
        r_DEFAULT_TLG,
        r_ALIGNED_TLG,
        r_AREAOFINTEREST_TLG,
        r_DRLDECOMPSUBTILE_TLG,
        r_DRLDECOMP_TLG,
        r_REGULAR_TLG,
        r_STATISTICSPARAM_TLG,
        r_STATISTICS_TLG
    };

    enum QtStorageConstants
    {
        r_DEFAULT_STG,
        r_ARRAY_STG,
        r_AUTO_STG,
        r_BMP_STG,
        r_DEM_STG ,
        r_HDF5_STG,
        r_HDF_STG ,
        r_NETCDF_STG,
        r_JPEG_STG,
        r_NTF_STG,
        r_PACKBITS_STG,
        r_PPM_STG,
        r_PNG_STG,
        r_RLESEP_STG,
        r_RLE_STG,
        r_TIFF_STG,
        r_TOR_STG ,
        r_VFF_STG,
        r_WLTCOIFLETINT_STG,
        r_WLTDAUBECHIESINT_STG,
        r_WLTDAUBECHIES_STG,
        r_WLTHAAR_STG,
        r_WLTLEASTINT_STG,
        r_WLTQHAAR_STG,
        r_ZLIBSEP_STG,
        r_ZLIB_STG
    };
};

#endif
