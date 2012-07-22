#ifndef _DBSTORAGELAYOUT_HH_
#define _DBSTORAGELAYOUT_HH_

#include <string>
#include <vector>
#include "relcatalogif/dbminterval.hh"
#include "reladminif/dbobject.hh"
#include "reladminif/dbref.hh"

class DBStorageLayout;
template<class T> class DBRef;
typedef DBRef<DBStorageLayout> DBStorageLayoutId;

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

/****************************************************************************
 *
 *
 * COMMENTS:
 *
 ****************************************************************************/

//@ManMemo: Module: {\bf relstorageif}
/*@Doc:
    Each instance of the {\tt DBStorageLayout} class describes a physical
    storage layout for an MDD object or collection.
    Every storage parameter which is not defined using the proper set* methods will result in a default value to be returned.  the supports* methods will tell if the value is a default value defined at instantiation time through the static storagemgr/StorageLayout attributes or an explicitly defined value.
    For information on the meaning of these attributes refere to storagemgr/storagelayout
*/
/**
  * \defgroup Relstorageifs Relstorageif Classes
  */

/**
  * \ingroup Relstorageifs
  */

class DBStorageLayout : public DBObject
{
public:
    //@Man: Creation
    //@{

    /// Construct object that uses system defaults.
    DBStorageLayout();

    //@}

    void printStatus(unsigned int level = 0,std::ostream& stream = std::cout) const;

    //@Man: check operations
    //@{
    bool supportsTileSize() const;

    bool supportsPCTMin() const;

    bool supportsPCTMax() const;

    bool supportsIndexSize() const;

    bool supportsIndexType() const;

    bool supportsTilingScheme() const;

    //is checked by OId::INVALID on tilingConfiguration
    bool supportsTileConfiguration() const;

    bool supportsDataFormat() const;
    //@}

    //@Man: Get operations
    //@{

    r_Bytes getPCTMin() const;

    r_Bytes getPCTMax() const;

    unsigned int getIndexSize() const;

    r_Index_Type getIndexType() const;

    r_Tiling_Scheme getTilingScheme() const;

    r_Bytes getTileSize() const;

    r_Minterval getTileConfiguration() const;

    r_Data_Format getDataFormat() const;

    //@}

    //@Man: Set operations
    //@{

    void setPCTMin(r_Bytes bytes);

    void setPCTMax(r_Bytes bytes);

    void setIndexSize(unsigned int entries);

    void setIndexType(r_Index_Type it);

    void setTilingScheme(r_Tiling_Scheme ts);

    void setTileSize(r_Bytes ts);

    void setTileConfiguration(const r_Minterval& tc);

    void setDataFormat(r_Data_Format df);

    //@}


    //@Man: Destruction
    //@{
    ///
    ~DBStorageLayout();
    //@}

protected:
    DBStorageLayout(const OId& id) throw (r_Error);

    friend class ObjectBroker;

    //@Man: Operations
    //@{

    virtual void readFromDb() throw (r_Error);

    virtual void insertInDb() throw (r_Error);

    virtual void deleteFromDb() throw (r_Error);

    virtual void updateInDb() throw (r_Error);


    //@}

private:

    //@Man: Actual Parameters:
    //@{

    /// Name of the storage layout represented by this object
    // char* stName;

    //@Man: Index Structure:
    //@{
    /// Which type of index should be used
    r_Index_Type indexType;

    unsigned int indexSize;
    //@}

    //@Man: Tiling:
    //@{
    /// How the object should be tiled
    r_Tiling_Scheme tilingScheme;

    r_Bytes pctMin;

    r_Bytes pctMax;

    /// Tile size in bytes.
    r_Bytes tileSize;

    /// Default configuration of the tiles.
    DBMintervalId tileConfiguration;
    /**
        Describe the shape of the tiles. For instance, [2:4,0:1,0:2].
        The tiling will start at the point [2,0,0].
        Tiles will be appended from there according to the tileConfig.
    */
    //@}

    //@Man: DataFormat
    //@{
    /// How the tiles of the object should be compressed
    r_Data_Format dataFormat;
    //@}

    //@}
    bool _supportsTileSize;

    bool _supportsPCTMin;

    bool _supportsPCTMax;

    bool _supportsIndexSize;

    bool _supportsIndexType;

    bool _supportsTiling;

    bool _supportsDataFormat;

    bool _supportsTileConfiguration;
};

#endif
