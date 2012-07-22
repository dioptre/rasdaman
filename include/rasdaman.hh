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

//------------------------------------------------------------
//                 RasDaMan Standard Include
//
//                       DO NOT EDIT
//------------------------------------------------------------

/**
*   @file rasdaman.hh
*
*   @ingroup Include
*/

#ifndef __RASDAMAN_HH_
#define __RASDAMAN_HH_

//------------------------------------------------------------
//  Includes
//------------------------------------------------------------

#include "rasodmg/ref.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "rasodmg/set.hh"
#include "rasodmg/iterator.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/oqlquery.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/alignedtiling.hh"
#include "rasodmg/interesttiling.hh"
#include "rasodmg/dirtiling.hh"
#include "rasodmg/stattiling.hh"

#include "raslib/endian.hh"
#include "raslib/odmgtypes.hh"
#include "raslib/mddtypes.hh"
#include "raslib/oid.hh"
#include "raslib/type.hh"
#include "raslib/minterval.hh"
#include "raslib/primitive.hh"
#include "raslib/complex.hh"
#include "raslib/structure.hh"
#include "raslib/primitivetype.hh"
#include "raslib/complextype.hh"
#include "raslib/structuretype.hh"
#include "raslib/rmdebug.hh"
#include "raslib/parseparams.hh"

#include "conversion/convertor.hh"
#include "conversion/convfactory.hh"

#endif
