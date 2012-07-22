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
 * INCLUDE: memblockvec.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Memory_Block_Vector
 *
 * COMMENTS:
 *
*/

#ifndef _MEMORY_BLOCK_VECTOR_H_
#define _MEMORY_BLOCK_VECTOR_H_

#include "raslib/mddtypes.hh"


//@ManMemo: Module {\bf raslib}

/*@Doc:
  Auxiliary class, realizes a set of memory blocks of equal size that can
  be extended to any size. Used by some children of r_LinCompStream.
*/

class r_Memory_Block_Vector
{
public:
    /// constructor, receiving the size of each memory block and the granularity
    /// for extending the number of blocks.
    r_Memory_Block_Vector( r_Bytes bsize=4096, unsigned int gran=8 );
    /// destructor
    ~r_Memory_Block_Vector( void );
    /// return number of blocks
    inline unsigned int get_number( void ) const
    {
        return numBlocks;
    }
    /// return block size
    inline r_Bytes get_block_size( void ) const
    {
        return blockSize;
    }
    /// return granularity
    inline unsigned int get_granularity( void ) const
    {
        return granularity;
    }
    /// get a block
    void* operator[]( unsigned int idx ) const;
    /// add a new block and return a pointer to it
    void* add( void );
    /// free all blocks (but not the vector, call the destructor for that)
    void free_data( void );
    /// get number of bytes stored. lastFill is the number of bytes used
    /// in the last block
    r_Bytes get_size( r_Bytes lastFill ) const;
    /// Copy the data stored in blocks into linear memory. lastFill is the
    /// number of bytes in the last block
    void copy_data( void* dest, r_Bytes lastFill ) const;

protected:
    /// the array of memory block pointers
    void** blocks;
    unsigned int numBlocks;
    unsigned int maxBlocks;
    /// the size of the blocks
    r_Bytes blockSize;
    /// the granularity
    unsigned int granularity;
};

#endif
