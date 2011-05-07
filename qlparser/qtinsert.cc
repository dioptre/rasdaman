/*************************************************************
 *
 * Copyright (C) 2003 Dr. Peter Baumann
 *
 * SOURCE: qtinsert.cc
 *
 * MODULE: qlparser
 * CLASS:  QtInsert
 *
 * PURPOSE:
 *
 * CHANGE HISTORY (append further entries):
 * when         who         what
 * ----------------------------------------------------------
 * 17-01-98     Ritsch      created
 * 2003-aug-25  PB          "insert into" type compatibility bug fixed (from K.Hahn)
 * 2008-nov-10  Shams       added storagelayout to the expression
 * COMMENTS:
 *
 ************************************************************/



#include <vector>
#include "raslib/dlist.hh"
//#include "rasodmg/stattiling.hh"
#include "rasodmg/interesttiling.hh"
#include "rasodmg/dirdecompose.hh"


static const char rcsid[] = "@(#)qlparser, QtInsert: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/qtinsert.cc,v 1.27 2003/12/27 20:40:21 rasdev Exp $";

#include "qlparser/qtinsert.hh"
#include "qlparser/qtdata.hh"
#include "qlparser/qtmdd.hh"

#include "mddmgr/mddcoll.hh"
#include "mddmgr/mddobj.hh"

#include <iostream>
#include <exception>

#include "servercomm/servercomm.hh"
#include "qlparser/qtmintervaldata.hh"
#include "raslib/basetype.hh"
#include "raslib/collectiontype.hh"

extern ServerComm::ClientTblElt* currentClientTblElt;

const QtNode::QtNodeType QtInsert::nodeType = QtNode::QT_INSERT;

QtInsert::QtInsert(const std::string& initCollectionName, QtOperation* initSource)
: QtExecute(), collectionName(initCollectionName), source(initSource) {
    source->setParent(this);
    stgLayout = NULL;
}

QtInsert::QtInsert(const std::string& initCollectionName, QtOperation* initSource, QtOperation* storage)
: QtExecute(), collectionName(initCollectionName), source(initSource), stgLayout(storage) {
    source->setParent(this);
}

QtInsert::~QtInsert() {
    if (source) {
        delete source;
        source = NULL;
    }
    if (stgLayout) {
        delete stgLayout;
        stgLayout = NULL;
    }
}

int
QtInsert::evaluate() {
    RMDBCLASS("QtInsert", "evaluate()", "qlparser", __FILE__, __LINE__)
    QtMddCfgOp* configOp = NULL;
    QtMDDConfig* mddConfig = NULL;
    // empty data list for evaluation of insert expression including constant
    QtNode::QtDataList* nextTupel = new QtNode::QtDataList(0);
    if (stgLayout) {
        configOp = (QtMddCfgOp*) stgLayout;
        mddConfig = configOp->getMddConfig();
    }
    // get the operands
    QtData* sourceData = source->evaluate(nextTupel);

    if (sourceData) {
        QtMDD* sourceMDD = (QtMDD*) sourceData;
        MDDObj* sourceObj = sourceMDD->getMDDObject();

        MDDColl* persColl = NULL;
        MDDColl* almost = NULL;

        try {
            almost = MDDColl::getMDDCollection(collectionName.c_str());
        } catch (...) {

            RMInit::logOut << "Error: QtInsert::evaluate() - collection name not found" << std::endl;

            // delete the operand
            if (sourceData) sourceData->deleteRef();

            parseInfo.setErrorNo(355);
            throw parseInfo;
        }
        if (!almost->isPersistent()) {
            RMInit::logOut << "QtInsert: User tries to insert into system table" << std::endl;
            if (sourceData) sourceData->deleteRef();

            parseInfo.setErrorNo(355);
            throw parseInfo;
        } else {
            persColl = (MDDColl*) almost;
        }

        //
        // check MDD and collection type for compatibility
        //
        int cellSize;
        RMDBGIF(3, RMDebug::module_qlparser, "QtInsert",  \
      char* collTypeStructure = persColl->getCollectionType()->getTypeStructure();  \
      char* mddTypeStructure = sourceObj->getMDDBaseType()->getTypeStructure();  \
      RMInit::dbgOut << std::endl << "Collection type structure.: " << collTypeStructure << std::endl \
                             << "MDD type structure........: " << mddTypeStructure << std::endl \
                             << "MDD domain................: " << sourceObj->getDefinitionDomain() << std::endl;  \
      free(collTypeStructure); collTypeStructure = NULL;  \
      free(mddTypeStructure); mddTypeStructure = NULL;)
      cellSize = (int)sourceObj->getMDDBaseType()->getBaseType()->getSize();
                // bug fix: "insert into" found claimed non-existing type mismatch -- PB 2003-aug-25, based on fix by K.Hahn
                // if( !persColl->getCollectionType()->compatibleWith( (Type*) sourceObj->getMDDBaseType() ) )
        if (!((MDDType*) sourceObj->getMDDBaseType())->compatibleWith(persColl->getCollectionType()->getMDDType())) {
            // free resources
            persColl->releaseAll();
            delete persColl;
            persColl = NULL;
            if (sourceData) sourceData->deleteRef(); // delete the operand

            // return error
            RMInit::logOut << "Error: QtInsert::evaluate() - MDD and collection types are incompatible" << std::endl;
            parseInfo.setErrorNo(959);
            throw parseInfo;
        }

        if (!persColl->getCollectionType()->getMDDType()->compatibleWithDomain(&(sourceObj->getDefinitionDomain()))) {
            // free resources
            persColl->releaseAll();
            delete persColl;
            persColl = NULL;
            if (sourceData) sourceData->deleteRef(); // delete the operand

            // return error
            RMInit::logOut << "Error: QtInsert::evaluate() - MDD and collection domains are incompatible" << std::endl;
            parseInfo.setErrorNo(959);
            throw parseInfo;
        }

        //
        // convert a transient MDD object to a persistent one
        //

        // allocate a new oid within the current db
        OId oid;
#ifdef BASEDB_O2
        if (!OId::allocateMDDOId(&oid)) {
#else
        OId::allocateOId(oid, OId::MDDOID);
#endif
            // get all tiles
            vector<Tile*>* sourceTiles = sourceObj->getTiles();

            // get a persistent type pointer
            MDDBaseType* persMDDType = (MDDBaseType*) TypeFactory::ensurePersistence((Type*) sourceObj->getMDDBaseType());

            if (!persMDDType) {
                RMInit::logOut << "Error: QtInsert::evaluate() - type not persistent" << std::endl;

                // delete dynamic data
                if (sourceData) sourceData->deleteRef();
                delete sourceTiles;
                sourceTiles = NULL;
                delete nextTupel;
                nextTupel = NULL;
                persColl->releaseAll();
                delete persColl;
                persColl = NULL;
                parseInfo.setErrorNo(964);
                throw parseInfo;
            }

            // create a persistent MDD object
            // need a StorageLayout here
            if(mddConfig!= NULL){
            if(mddConfig->getBorderThreshold() < 0)
                mddConfig->setBorderThreshold(r_Stat_Tiling::DEF_BORDER_THR);
            }
            r_Index_Type ri = getIndexType(mddConfig);
            StorageLayout tempStorageLayout;
            tempStorageLayout.setDataFormat(getDataFormat(mddConfig));
            r_Tiling_Scheme scheme = getTilingScheme(mddConfig);
            tempStorageLayout.setIndexType(ri);
            tempStorageLayout.setTilingScheme(scheme);
            // Base Information has been set
            tempStorageLayout.setTileSize
                    ((mddConfig != NULL && mddConfig->getTileSize() > 0) ? mddConfig->getTileSize() :
                    StorageLayout::DefaultTileSize);
            if(mddConfig!= NULL){
                tempStorageLayout.setInterestThreshold(mddConfig->getInterestThreshold());
                tempStorageLayout.setBorderThreshold(mddConfig->getBorderThreshold());
                if(mddConfig->getDirDecomp() != NULL)
                    tempStorageLayout.setDirDecomp(mddConfig->getDirDecomp());
                vector<r_Minterval>intervals = getIntervals(mddConfig);
                tempStorageLayout.setCellSize(cellSize);
                if(mddConfig->getTilingType() == QtMDDConfig::r_DRLDECOMP_TLG){
                    tempStorageLayout.resetSubTiling();
                }
                else if(mddConfig->getTilingType() == QtMDDConfig::r_DRLDECOMPSUBTILE_TLG){
                    tempStorageLayout.setSubTiling();
                }
                if(intervals.size() > 0){
                    tempStorageLayout.setBBoxes(intervals);
                }
            }

            r_Minterval tileCfg = getTileConfig(mddConfig);

            if (sourceObj->getDefinitionDomain().dimension() ==
                    tileCfg.dimension()) {
                tempStorageLayout.setTileConfiguration(tileCfg);
            }
            MDDObj* persMDDObj = new MDDObj(persMDDType, sourceObj->getDefinitionDomain(), oid,
                    tempStorageLayout);

            // iterate over source tiles
            for (vector<Tile*>::iterator sourceIt = sourceTiles->begin(); sourceIt != sourceTiles->end(); sourceIt++) {
                // create a new persistent tile, copy the transient data, and insert it into the target mdd object
                Tile* newPersTile = new Tile((*sourceIt)->getDomain(), persMDDType->getBaseType(), (*sourceIt)->getDataFormat());
                newPersTile->copyTile((*sourceIt)->getDomain(), *sourceIt, (*sourceIt)->getDomain());
                persMDDObj->insertTile(newPersTile);
            }

            // delete tile vector
            delete sourceTiles;
            sourceTiles = NULL;
            persColl->insert(persMDDObj);

#ifdef BASEDB_O2
        } else {
            RMInit::logOut << "Error: QtInsert::evaluate() - allocation of oid failed" << std::endl;

            // delete dynamic data
            if (sourceData) sourceData->deleteRef();
            delete nextTupel;
            nextTupel = NULL;
            persColl->releaseAll();
            delete persColl;
            persColl = NULL;
            parseInfo.setErrorNo(958);
            throw parseInfo;
        }
#else
#endif
        // free transient memory
        persColl->releaseAll();
        delete persColl;
        persColl = NULL;
    } else
        RMInit::logOut << "Error: QtInsert::evaluate() - insert data is invalid." << std::endl;

    // delete source operand
    if (sourceData) sourceData->deleteRef();

    // delete dummy tupel vector
    delete nextTupel;
    nextTupel = NULL;

    return 0;
}

QtNode::QtNodeList*
QtInsert::getChilds(QtChildType flag) {
    QtNodeList* resultList = NULL;

    RMDBGENTER(0, RMDebug::module_qlparser, "QtInsert", "QtInsert::getChilds()")

    if (source) {
        // allocate resultList
        if (flag == QT_DIRECT_CHILDS);
        resultList = new QtNodeList();

        if (flag == QT_LEAF_NODES || flag == QT_ALL_NODES)
            resultList = source->getChilds(flag);

        // add the nodes of the current level
        if (flag == QT_DIRECT_CHILDS || flag == QT_ALL_NODES)
            resultList->push_back(source);
    } else
        resultList = new QtNodeList();


    RMDBGEXIT(0, RMDebug::module_qlparser, "QtInsert", "QtInsert::getChilds()")
    return resultList;
}

void
QtInsert::printTree(int tab, std::ostream& s, QtChildType mode) {
    s << SPACE_STR(tab).c_str() << "QtInsert Object" << std::endl;

    if (mode != QtNode::QT_DIRECT_CHILDS) {
        if (source) {
            s << SPACE_STR(tab).c_str() << "source : " << std::endl;
            source->printTree(tab + 2, s);
        } else
            s << SPACE_STR(tab).c_str() << "no source" << std::endl;

        s << std::endl;
    }
}

void
QtInsert::printAlgebraicExpression(std::ostream& s) {
    s << "insert<";

    if (source)
        source->printAlgebraicExpression(s);
    else
        s << "<no source>";

    s << ">";
}

QtOperation*
QtInsert::getSource() {
    return source;
}

/*
void
QtInsert::preOptimize()
{
  if( source )
    source->optimizeLoad( new QtNode::QtTrimList );
}
 */


void
QtInsert::checkType() {
    RMDBCLASS("QtInsert", "checkType()", "qlparser", __FILE__, __LINE__)

            // check operand branches
    if (source) {

        // get input type
        const QtTypeElement& inputType = source->checkType();

        if (inputType.getDataType() != QT_MDD) {
            RMInit::logOut << "Error: QtInsert::checkType() - insert expression must be of type r_Marray<T>" << std::endl;
            parseInfo.setErrorNo(960);
            throw parseInfo;
        }
    } else
        RMInit::logOut << "Error: QtInsert::checkType() - operand branch invalid." << std::endl;
}

r_Data_Format
QtInsert::getDataFormat(QtMDDConfig* config) {
    if (!config)
        return StorageLayout::DefaultDataFormat;
    int dataType = config->getStorageType();
    switch (dataType) {
        case QtMDDConfig::r_DEFAULT_STG :
            return StorageLayout::DefaultDataFormat;
        case QtMDDConfig::r_ARRAY_STG :
            return r_Array;
        case QtMDDConfig::r_AUTO_STG :
            return r_Auto_Compression;
        case QtMDDConfig::r_BMP_STG :
            return r_BMP;
        case QtMDDConfig::r_DEM_STG :
            return r_DEM;
        case QtMDDConfig::r_HDF_STG :
            return r_HDF;
        case QtMDDConfig::r_NETCDF_STG :
            return r_NETCDF;
            //        case QtMDDConfig::r_HDF5_STG://need review
            //            return r_HDF;
        case QtMDDConfig::r_JPEG_STG :
            return r_JPEG;
        case QtMDDConfig::r_NTF_STG :
            return r_NTF;
        case QtMDDConfig::r_PACKBITS_STG :
            return r_Pack_Bits;
        case QtMDDConfig::r_PNG_STG :
            return r_PNG;
        case QtMDDConfig::r_PPM_STG :
            return r_PPM;
        case QtMDDConfig::r_RLE_STG :
            return r_RLE;
        case QtMDDConfig::r_RLESEP_STG :
            return r_Sep_RLE;
        case QtMDDConfig::r_TIFF_STG :
            return r_TIFF;
        case QtMDDConfig::r_TOR_STG :
            return r_TOR;
        case QtMDDConfig::r_VFF_STG :
            return r_VFF;
        case QtMDDConfig::r_WLTCOIFLETINT_STG :
            if (config->getWltValue() == 6)
                return r_Wavelet_Coiflet6;
            if (config->getWltValue() == 12)
                return r_Wavelet_Coiflet12;
            if (config->getWltValue() == 18)
                return r_Wavelet_Coiflet18;
            if (config->getWltValue() == 24)
                return r_Wavelet_Coiflet24;
            if (config->getWltValue() == 30)
                return r_Wavelet_Coiflet30;
            return StorageLayout::DefaultDataFormat; //may be null
        case QtMDDConfig::r_WLTDAUBECHIES_STG :
            return r_Wavelet_Daubechies;
        case QtMDDConfig::r_WLTDAUBECHIESINT_STG :
            if (config->getWltValue() == 6)
                return r_Wavelet_Daub6;
            if (config->getWltValue() == 8)
                return r_Wavelet_Daub8;
            if (config->getWltValue() == 10)
                return r_Wavelet_Daub10;
            if (config->getWltValue() == 12)
                return r_Wavelet_Daub12;
            if (config->getWltValue() == 14)
                return r_Wavelet_Daub14;
            if (config->getWltValue() == 16)
                return r_Wavelet_Daub16;
            if (config->getWltValue() == 18)
                return r_Wavelet_Daub18;
            if (config->getWltValue() == 20)
                return r_Wavelet_Daub20;
            return StorageLayout::DefaultDataFormat; //may be default
        case QtMDDConfig::r_WLTHAAR_STG :
            return r_Wavelet_Haar;
        case QtMDDConfig::r_WLTLEASTINT_STG :
            if (config->getWltValue() == 8)
                return r_Wavelet_Least8;
            if (config->getWltValue() == 10)
                return r_Wavelet_Least10;
            if (config->getWltValue() == 12)
                return r_Wavelet_Least12;
            if (config->getWltValue() == 14)
                return r_Wavelet_Least14;
            if (config->getWltValue() == 16)
                return r_Wavelet_Least16;
            if (config->getWltValue() == 18)
                return r_Wavelet_Least18;
            if (config->getWltValue() == 20)
                return r_Wavelet_Least20;
        case QtMDDConfig::r_WLTQHAAR_STG :
            return r_Wavelet_QHaar;
        case QtMDDConfig::r_ZLIB_STG :
            return r_ZLib;
        case QtMDDConfig::r_ZLIBSEP_STG :
            return r_Sep_ZLib;
        default:
            return StorageLayout::DefaultDataFormat;
    }

}

r_Index_Type
QtInsert::getIndexType(QtMDDConfig* config) {
    if (!config)
        return StorageLayout::DefaultIndexType;
    int indexType = config->getIndexType();

    switch (indexType) {
        case QtMDDConfig::r_A_INDEX :
            return r_Auto_Index;
        case QtMDDConfig::r_DEFAULT_INDEX :
            return StorageLayout::DefaultIndexType;
        case QtMDDConfig::r_D_INDEX :
            return r_Directory_Index;
        case QtMDDConfig::r_IT_INDEX :
            return r_Index_Type_NUMBER;
        case QtMDDConfig::r_RC_INDEX :
            return r_Reg_Computed_Index;
        case QtMDDConfig::r_RD_INDEX :
            return r_Reg_Directory_Index;
        case QtMDDConfig::r_RPT_INDEX :
            return r_RPlus_Tree_Index;
        case QtMDDConfig::r_RRPT_INDEX :
            return r_Reg_RPlus_Tree_Index;
        case QtMDDConfig::r_TC_INDEX :
            return r_Tile_Container_Index;
        default:
            return StorageLayout::DefaultIndexType;
    }
}

r_Tiling_Scheme
QtInsert::getTilingScheme(QtMDDConfig* cfg) {
    if (!cfg)
        return StorageLayout::DefaultTilingScheme;
    int tileType = cfg->getTilingType();
    switch (tileType) {
        case QtMDDConfig::r_ALIGNED_TLG :
            return r_AlignedTiling;
        case QtMDDConfig::r_AREAOFINTEREST_TLG :
            return r_InterestTiling;
        case QtMDDConfig::r_REGULAR_TLG :
            return r_RegularTiling;
        case QtMDDConfig::r_DRLDECOMPSUBTILE_TLG :
            return r_DirectionalTiling;
        case QtMDDConfig::r_DRLDECOMP_TLG :
            return r_DirectionalTiling;
        case QtMDDConfig::r_STATISTICSPARAM_TLG :
            return r_StatisticalTiling;
        case QtMDDConfig::r_STATISTICS_TLG :
            return r_StatisticalTiling;

        default:
            return StorageLayout::DefaultTilingScheme;
    }
}

vector<r_Minterval>
QtInsert::getIntervals(QtMDDConfig* cfg) {
    vector<r_Minterval> intervals;
    if (!cfg)
        return intervals;
    QtNode::QtOperationList* oplist = cfg->getBboxList();
    if (!oplist)
        return intervals;
    QtNode::QtDataList* nextTupel = new QtNode::QtDataList(0);
    QtOperationList::iterator iter;
    for (iter = oplist->begin(); iter != oplist->end(); iter++) {
        QtData* data = (*iter)->evaluate(nextTupel);
        QtMintervalData* intervalData = (QtMintervalData*) data;
        r_Minterval interval = intervalData->getMintervalData();
        intervals.push_back(interval);
    }
    return intervals;
}

r_Minterval
QtInsert::getTileConfig(QtMDDConfig* cfg) {
    r_Minterval tileConfig;
    if (!cfg || !(cfg->getTilingType() == QtMDDConfig::r_ALIGNED_TLG ||
    cfg->getTilingType() == QtMDDConfig::r_REGULAR_TLG))
        return StorageLayout::DefaultTileConfiguration;

    QtOperation* op = cfg->getTileCfg();
    if (!op)
        return tileConfig;
    QtNode::QtDataList* nextTupel = new QtNode::QtDataList(0);
    QtData* data = op->evaluate(nextTupel);
    QtMintervalData* intervalData = (QtMintervalData*) data;
    tileConfig = intervalData->getMintervalData();
    return tileConfig;
}
