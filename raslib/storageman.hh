/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
*/

/**
 * INCLUDE: storageman.hh
 *
 * MODULE: raslib
 *
 * CLASSES: r_Storage_Man, r_Storage_Man_C, r_Storage_Man_CPP
 *
 * COMMENTS:
 *
 * Transparent handling of malloc/free vs. new/delete
 *
*/

#ifndef _R_STORAGE_MAN_
#define _R_STORAGE_MAN_


// for size_t
#include <stdlib.h>

#include "raslib/error.hh"



//@ManMemo: Module {\bf raslib}

/*@Doc:
  r_Storage_Man provides an interface to a pair of heap management
  functions for allocating and freeing blocks. I want to be able
  to use constructs like sman1 = sman2, therefore I can't use
  virtual functions to implement this.
*/

class r_Storage_Man
{
public:

    /// types of storage management functions
    typedef void *(*storage_man_alloc)( size_t );
    typedef void (*storage_man_free)( void *data );

    /// default constructor, switches to c-style allocation
    r_Storage_Man( void );
    /// constructor setting the storage functions
    r_Storage_Man( storage_man_alloc a, storage_man_free f );
    /// copy constructor
    r_Storage_Man( const r_Storage_Man &src );
    /// destructor
    ~r_Storage_Man( void );
    /// setting the storage functions
    void set_storage_functions( storage_man_alloc a, storage_man_free f );
    /// assignment
    r_Storage_Man &operator=( const r_Storage_Man &src );
    /// allocation
    void *storage_alloc( size_t size ) const throw(r_Error);
    /// deallocation
    void storage_free( void *data ) const;


protected:
    /// the storage functions
    storage_man_alloc myalloc;
    storage_man_free myfree;
};


/*@Doc:
  r_Storage_Man_C implements C-style allocation using malloc/free
*/

class r_Storage_Man_C : public r_Storage_Man
{
public:
    /// default constructor
    r_Storage_Man_C( void );
    /// default destructor
    ~r_Storage_Man_C( void );
};


/*@Doc:
  r_Storage_Man_CPP implements C++-style allocation using new/delete
*/

class r_Storage_Man_CPP : public r_Storage_Man
{
public:
    /// default constructor
    r_Storage_Man_CPP( void );
    /// destructor
    ~r_Storage_Man_CPP( void );
};

#endif
