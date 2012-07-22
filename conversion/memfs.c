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
 * SOURCE: memfs.c
 *
 * MODULE: conversion
 *
 * PURPOSE:
 *      Memory Filing System used by some of the convertor modules
 *
 * COMMENTS
 *          None
*/

#include "mymalloc/mymalloc.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "conversion/memfs.hh"


/* can't use RMDBGOUT because this is C, not C++ */
const int MEMFSDBGLEVEL = 4;

extern int RManDebug;



/* This function for internal use only */
int memfs_ensure(thandle_t handle, toff_t off)
{
    memFSContext *memFS = (memFSContext *)handle;
    char **mam2=NULL;
    int mamSize2=0, i=0;

#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_ensure: %d\n", off);
        fflush(stdout);
    }
#endif
    /* Do we have to allocate a bigger mam? */
    mamSize2 = (off >> MEMFS_LD_BLOCKSIZE);
    if (mamSize2 >= memFS->mamSize)
    {
        /* Always allocate mam in powers of 2. That ensures that if we run out
           of space the new mam will be twice as big as the old one. */
        i = 0;
        while (mamSize2 != 0)
        {
            mamSize2 >>= 1;
            i++;
        }
        mamSize2 = (1 << i);
#ifdef RMANDEBUG
        if (RManDebug >= MEMFSDBGLEVEL)
        {
            printf("memfs_ensure: growing mam from %d to %d\n", memFS->mamSize, mamSize2);
            fflush(stdout);
        }
#endif
        if ((mam2 = (char **)mymalloc(mamSize2 * sizeof(char*))) == NULL)
            return -1;
        /* Copy existing mam entries */
        memcpy ((void*)mam2, (void*) memFS->mam, memFS->mamSize * sizeof(char*));
        /* Init new mam entries */
        for (i=memFS->mamSize; i < mamSize2; i++)
        {
            mam2[i] = NULL;
        }
        /* free old mam */
        free(memFS->mam);
        memFS->mam = mam2;
        memFS->mamSize = mamSize2;
    }
    /* Calculate again because its value might have been changed by the
       above block */
    mamSize2 = (off >> MEMFS_LD_BLOCKSIZE);
    if ((memFS->mam)[mamSize2] == NULL)
    {
        /* We don't just have to allocate this one new block but all the
           ones with lower addresses that aren't defined yet as well */
        for (i=memFS->mamHighest+1; i <= mamSize2; i++)
        {
            if (((memFS->mam)[i] = (char*)mymalloc((1 << MEMFS_LD_BLOCKSIZE) * sizeof(char))) == NULL)
                return -1;
        }
        memFS->mamHighest = mamSize2;
    }
    /* All done, the memFS can now hold an object of size off */
    return 0;
}


/* Initialise the memory filing system */
int memfs_initfs(thandle_t handle)
{
    memFSContext *memFS = (memFSContext *)handle;
    int i=0;

#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_initfs\n");
        fflush(stdout);
    }
#endif
    memFS->pos = 0;
    memFS->high = 0;
    memFS->mamSize = MEMFS_MAM_ENTRIES;
    if ((memFS->mam = (char **)mymalloc(MEMFS_MAM_ENTRIES * sizeof(char *))) == NULL)
        return -1;
    if (((memFS->mam)[0] = (char*)mymalloc((1 << MEMFS_LD_BLOCKSIZE) * sizeof(char))) == NULL)
        return -1;
    memFS->mamHighest = 0;
    for (i=1; i<MEMFS_MAM_ENTRIES; i++)
    {
        (memFS->mam)[i] = NULL;
    }
    return 0;
}


/* Kill the memory filing system, freeing all its resources */
void memfs_killfs(thandle_t handle)
{
    memFSContext *memFS = (memFSContext *)handle;
    int i=0;

#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_killfs\n");
        fflush(stdout);
    }
#endif
    for (i=0; i < memFS->mamSize; i++)
    {
        if ((memFS->mam)[i] == NULL) break;
        free((memFS->mam)[i]);
    }
    free(memFS->mam);
}


/* Reset file pointers, leave memory setup */
void memfs_newfile(thandle_t handle)
{
    memFSContext *memFS = (memFSContext *)handle;

#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_newfile\n");
        fflush(stdout);
    }
#endif
    memFS->pos = 0;
    memFS->high = 0;
}


tsize_t memfs_read(thandle_t handle, tdata_t mem, tsize_t size)
{
    tsize_t todo=0, transfered = 0;
    int block=0, offset=0, x=0;
    memFSContext *memFS = (memFSContext *)handle;

    /* Don't read over the end of the "file" */
    todo = memFS->high - memFS->pos;
#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_read: (%d, left: %d)\n", todo, memFS->high);
        fflush(stdout);
    }
#endif
    if (todo > size) todo = size;
    while (todo > 0)
    {
        block = (memFS->pos >> MEMFS_LD_BLOCKSIZE);
        offset = memFS->pos - (block << MEMFS_LD_BLOCKSIZE);
        /* Space left in this buffer */
        x = (1 << MEMFS_LD_BLOCKSIZE) - offset;
        if (x > todo) x = todo;
        memcpy((void*)mem, (void*)(((memFS->mam)[block]) + offset), x);
        /* tdata_t is some kind of void *, so we have to do this cast */
        mem = (tdata_t)(((char*)mem) + x);
        memFS->pos += x;
        transfered += x;
        todo -= x;
    }
    return transfered;
}


tsize_t memfs_write(thandle_t handle, tdata_t mem, tsize_t size)
{
    tsize_t transfered = 0;
    memFSContext *memFS = (memFSContext *)handle;
    int block=0, offset=0, x=0;

    /* Make sure there's enough room for this write */
    if (memfs_ensure(handle, memFS->pos + size) < 0) return 0;
#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_write (%d)\n",size);
        fflush(stdout);
    }
#endif
    while (size > 0)
    {
        /* See memfs_read */
        block = (memFS->pos >> MEMFS_LD_BLOCKSIZE);
        offset = memFS->pos - (block << MEMFS_LD_BLOCKSIZE);
        x = (1 << MEMFS_LD_BLOCKSIZE) - offset;
        if (x > size) x = size;
        memcpy((void*)(((memFS->mam)[block]) + offset), (void*)mem, x);
        mem = (tdata_t)(((char *)mem) + x);
        memFS->pos += x;
        transfered += x;
        size -= x;
    }
    if (memFS->pos > memFS->high)
    {
        memFS->high = memFS->pos;
    }
    return transfered;
}


toff_t memfs_seek(thandle_t handle, toff_t offset, int mode)
{
    memFSContext *memFS = (memFSContext *)handle;

    switch (mode)
    {
    case SEEK_SET:
        memFS->pos = offset;
        break;
    case SEEK_CUR:
        memFS->pos += offset;
        break;
    case SEEK_END:
        memFS->pos = memFS->high + offset;
        break;
    default:
        break;
    }
    if (memFS->pos < 0) memFS->pos = 0;
    /* Don't limit to end of file (this actually caused problems!) */
    memfs_ensure(handle, memFS->pos);
    if (memFS->pos > memFS->high) memFS->high = memFS->pos;
#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_seek: Set pos to %d\n", memFS->pos);
        fflush(stdout);
    }
#endif
    return memFS->pos;
}


int memfs_close(thandle_t handle)
{
#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_close:\n");
        fflush(stdout);
    }
#endif
    return 1; /* = success? */
}


toff_t memfs_size(thandle_t handle)
{
#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_size:\n");
        fflush(stdout);
    }
#endif
    return (((memFSContext *)handle)->high);
}


int memfs_map(thandle_t handle, tdata_t *memp, toff_t *top)
{
#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_map: %p, %d\n", *memp, *top);
        fflush(stdout);
    }
#endif
    return 0;
}


void memfs_unmap(thandle_t handle, tdata_t mem, toff_t to)
{
#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_unmap: %p, %d\n", mem, to);
        fflush(stdout);
    }
#endif
}






/* Read-only from memory (simple chunky model, not block-oriented) */
void memfs_chunk_initfs(thandle_t handle, char *src, r_Long size)
{
    memFSContext *memFS = (memFSContext *)handle;

#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_chunk_initfs: %p, %d\n", src, size);
        fflush(stdout);
    }
#endif
    memFS->pos = 0;
    memFS->chunk = src;
    memFS->high = size;
}


tsize_t memfs_chunk_read(thandle_t handle, tdata_t mem, tsize_t size)
{
    tsize_t todo=0;
    memFSContext *memFS = (memFSContext *)handle;

    todo = memFS->high - memFS->pos;
#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_chunk_read: %d (left %d)\n", size, todo);
        fflush(stdout);
    }
#endif
    if (todo > size) todo = size;
    if (todo > 0)
    {
        memcpy((void*)mem, (void*)(memFS->chunk + memFS->pos), todo);
        memFS->pos += todo;
    }
    return todo;
}


toff_t memfs_chunk_seek(thandle_t handle, toff_t offset, int mode)
{
    memFSContext *memFS = (memFSContext *)handle;

    switch (mode)
    {
    case SEEK_SET:
        memFS->pos = offset;
        break;
    case SEEK_CUR:
        memFS->pos += offset;
        break;
    case SEEK_END:
        memFS->pos = memFS->high + offset;
        break;
    default:
        break;
    }
    if (memFS->pos < 0) memFS = 0;
    /* Since file can't be extended this is OK here */
    if (memFS->pos > memFS->high) memFS->pos = memFS->high;
#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_chunk_seek: Position to %d\n", memFS->pos);
        fflush(stdout);
    }
#endif
    return memFS->pos;
}


int memfs_chunk_close(thandle_t handle)
{
#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_chunk_close:\n");
        fflush(stdout);
    }
#endif
    return 1;
}


toff_t memfs_chunk_size(thandle_t handle)
{
#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_chunk_size:\n");
        fflush(stdout);
    }
#endif
    return (((memFSContext *)handle)->high);
}


/* Map file to memory -- since we already have it in memory in the
   first place this is very simple. */
int memfs_chunk_map(thandle_t handle, tdata_t *memp, toff_t *top)
{
    memFSContext *memFS = (memFSContext *)handle;

#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_chunk_map:\n");
        fflush(stdout);
    }
#endif
    *memp = (tdata_t)(memFS->chunk);
    *top = (toff_t)(memFS->high);
    return 1; /* Success? */
}

void memfs_chunk_unmap(thandle_t handle, tdata_t mem, toff_t to)
{
#ifdef RMANDEBUG
    if (RManDebug >= MEMFSDBGLEVEL)
    {
        printf("memfs_chunk_unmap: %p, %d\n", mem, to);
        fflush(stdout);
    }
#endif
}
