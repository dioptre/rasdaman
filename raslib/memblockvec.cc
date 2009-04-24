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
 * INCLUDE: memblockvec.cc
 *
 * MODULE:  raslib
 * CLASS:   r_Memory_Block_Vector
 *
 * COMMENTS:
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raslib/memblockvec.hh"


r_Memory_Block_Vector::r_Memory_Block_Vector( r_Bytes bsize, unsigned int gran )
{
  blockSize = bsize;
  granularity = gran;

  numBlocks = 0; maxBlocks = granularity;
  blocks = new void*[granularity];
}

r_Memory_Block_Vector::~r_Memory_Block_Vector( void )
{
  free_data();
  delete [] blocks;
}

void* r_Memory_Block_Vector::operator[]( unsigned int idx ) const
{
  if (idx >= numBlocks)
    return NULL;

  return blocks[idx];
}

void* r_Memory_Block_Vector::add( void )
{
  if (numBlocks >= maxBlocks)
  {
    void** newBlocks = new void*[maxBlocks + granularity];
    memcpy(newBlocks, blocks, numBlocks * sizeof(void*));
    delete [] blocks; blocks = newBlocks;
    maxBlocks += granularity;
  }
  blocks[numBlocks++] = (void*)(new char[blockSize]);
  return blocks[numBlocks-1];
}

void r_Memory_Block_Vector::free_data( void )
{
  unsigned int i;

  for (i=0; i<numBlocks; i++)
  {
    delete [] blocks[i];
    blocks[i] = NULL;
  }
  numBlocks = 0;
}

r_Bytes r_Memory_Block_Vector::get_size( r_Bytes lastFill ) const
{
  return (numBlocks == 0) ? 0 : (numBlocks-1)*blockSize + lastFill;
}

void r_Memory_Block_Vector::copy_data( void* dest, r_Bytes lastFill ) const
{
  unsigned int i;
  void* destPtr = dest;

  if (numBlocks == 0) return;

  for (i=0; i<numBlocks-1; i++)
  {
    memcpy(destPtr, blocks[i], blockSize);
    destPtr = (void*)(((char*)destPtr) + blockSize);
  }
  memcpy(destPtr, blocks[numBlocks-1], lastFill);
}
