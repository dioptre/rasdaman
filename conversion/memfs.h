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
 * INCLUDE: memfs.hh
 *
 * MODULE: conversion
 *
 * PURPOSE:
 * Memory Filing System used by some of the convertor modules.
 *
 *  COMMENTS:
 *          None
*/

#ifndef _MEMFS_HH_
#define _MEMFS_HH_

/* For data types used by the memfs */
#include "tiffio.h"
#include "raslib/odmgtypes.hh"


/* Claim blocks in 4k chunks */
const int MEMFS_LD_BLOCKSIZE = 12;
/* Initially preserve enough room for 16 blocks */
const int MEMFS_MAM_ENTRIES = 16;

typedef struct memFSContext
{
    r_Long pos, high;
    int mamSize, mamHighest;
    char **mam;
    char *chunk;
} memFSContext;


#ifdef __cplusplus
extern "C" {
#endif

    /* Flexible, read-write memFS */
    int memfs_initfs(thandle_t handle);
    void memfs_killfs(thandle_t handle);
    void memfs_newfile(thandle_t handle);
    tsize_t memfs_read(thandle_t handle, tdata_t mem, tsize_t size);
    tsize_t memfs_write(thandle_t handle, tdata_t mem, tsize_t size);
    toff_t memfs_seek(thandle_t handle, toff_t offset, int mode);
    int memfs_close(thandle_t handle);
    toff_t memfs_size(thandle_t handle);
    int memfs_map(thandle_t handle, tdata_t *memp, toff_t *top);
    void memfs_unmap(thandle_t handle, tdata_t mem, toff_t top);

    /* Simple, read-only memFS */
    void memfs_chunk_initfs(thandle_t handle, char *src, r_Long size);
    tsize_t memfs_chunk_read(thandle_t handle, tdata_t mem, tsize_t size);
    toff_t memfs_chunk_seek(thandle_t handle, toff_t offset, int mode);
    int memfs_chunk_close(thandle_t handle);
    toff_t memfs_chunk_size(thandle_t handle);
    int memfs_chunk_map(thandle_t handle, tdata_t *memp, toff_t *top);
    void memfs_chunk_unmap(thandle_t handle, tdata_t mem, toff_t to);

#ifdef __cplusplus
}
#endif

#endif
