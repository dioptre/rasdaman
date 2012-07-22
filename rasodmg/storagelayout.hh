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
 * INCLUDE: storagelayout.hh
 *
 * MODULE:  rasodmg
 * CLASS:   r_Storage_Layout, r_Domain_Storage_Layout
 *
 * COMMENTS:
 *      None
*/

#ifndef _R_STORAGELAYOUT_HH_
#define _R_STORAGELAYOUT_HH_


#include <iostream>
using std::cout;

#include "raslib/mddtypes.hh"
#include "rasodmg/set.hh"

// forward declarations
class r_Storage_Layout;
class r_GMarray;
class ClientComm;
class r_Tiling;
class r_Minterval;

//@ManMemo: Module: {\bf rasodmg}

/*@Doc:

    The {\tt r_Storage_Layout} class is used to express the storage options
    for {\tt r_Marray} objects. This is the superclass of different storage
    layout classes which may be used for different types of storage layout
    schemes. It is also used directly by the rasdaman client for
    default storage layout, i.e., for the storage layout for objects for
    which absolutely none has been defined.

  Notice: the tiling options are invalid if the rasdaman client is running
          with the option notiling. In that case, no tiling is done,
      independently of the storage layout chosen.
      For the time being, compression does not work.
*/

/**
  * \ingroup Rasodmgs
  */
class r_Storage_Layout
{
public:

    /// the dataformat is not used.  please use the database methods for this purpose.
    r_Storage_Layout(r_Data_Format init_format = r_Array, const char* formatParams = NULL);

    /// the dataformat is not used.  please use the database methods for this purpose.
    r_Storage_Layout(r_Tiling* ts, r_Data_Format init_format = r_Array, const char* formatParams = NULL);

    /// Copy constructor.
    r_Storage_Layout(const r_Storage_Layout& sl);

    ///
    virtual r_Storage_Layout* clone() const;

    /// virtual destructor
    virtual ~r_Storage_Layout();

    ///
    const r_Tiling* get_tiling() const;

    /// this does not do anything important.  please use the database methods for this purpose.
    r_Data_Format get_storage_format() const;

    /// this does not do anything important.  please use the database methods for this purpose.
    const char* get_storage_format_params() const;

    /// Function for decomposing large MDDs into a set of smaller tiles
    virtual r_Set< r_GMarray* >* decomposeMDD(const r_GMarray* mar) const throw (r_Error);

    /// writes the state of the object to the specified stream
    void print_status(std::ostream& s = cout) const;

    ///
    virtual bool is_compatible(const r_Minterval& obj_domain, r_Bytes celltypesize) const;

protected:

    friend class ClientComm;
    friend class r_GMArray;

    char* storage_params;

    /// the dataformat is not used.  please use the database methods for this purpose.
    r_Data_Format storage_format;

    /// Tiling scheme
    r_Tiling*  til;

};

//@ManMemo: Module: {\bf rasodmg }
/**
  Output stream operator for objects of type {\tt const} \Ref{r_Storage_Layout}.
*/
extern std::ostream& operator<<(std::ostream& s, const r_Storage_Layout& sl);

#if (defined(__VISUALC__) && !defined(__EXECUTABLE__))
#define __EXECUTABLE__
#include "raslib/dlist.hh"
#undef  __EXECUTABLE__
#else
#include "raslib/dlist.hh"
#endif
#endif
