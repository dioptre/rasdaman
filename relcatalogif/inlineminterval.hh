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
#ifndef _INLINEMINTERVAL_HH_
#define _INLINEMINTERVAL_HH_

class InlineMinterval;

template<class T> class DBRef;

#include "raslib/minterval.hh"

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:
InlineMinterval is a subclass of r_Minterval which stores and
instantiates itself in char[].
This class is used by the index data structures to store their
domain data fast and efficiently.
*/

/**
  * \ingroup Relcatalogifs
  */
class InlineMinterval : public r_Minterval
{
public:
    InlineMinterval();

    InlineMinterval(r_Dimension dim);

    InlineMinterval(r_Dimension dim, r_Range* lowerbound, r_Range* upperbound, char* lowerfixed, char* upperfixed);
    /*@Doc:
    construct a InlineMinterval with dimension dim from the
    arrays lowerbound, upperbound, lowerfixed, upperfixed.
    */

    InlineMinterval(const r_Minterval& old);

    InlineMinterval(const InlineMinterval& old);

    virtual ~InlineMinterval();

    //replaces only the r_Minterval part of the object
    virtual InlineMinterval& operator=(const InlineMinterval& old);

    //replaces only the r_Minterval part of the object
    virtual InlineMinterval& operator=(const r_Minterval& old);

    virtual void insertInDb(r_Range* lowerbound, r_Range* upperbound, char* lowerfixed, char* upperfixed) const;
    /*@Doc:
    stores the attributes of the InlineMinterval into the four
    supplied arrays.  the arrays must be large enough.
    */
};

#endif
