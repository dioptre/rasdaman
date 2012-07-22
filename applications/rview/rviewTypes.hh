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
 *  COMMENTS:
 *      This file is created automatically by the rasdl command.
 *      DO NOT EDIT
 */

/**
*   @file rviewTypes.hh
*
*   @ingroup Applications
*/

#ifndef _RVTYPES_HH_
#define _RVTYPES_HH_

//------------------------------------------------------------
//  Includes
//------------------------------------------------------------

#if (defined(EARLY_TEMPLATE) && !defined(__EXECUTABLE__))
#define __EXECUTABLE__
#define __UNDEF_EXEC_TYPES__
#endif

#include "rasodmg/ref.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "rasodmg/set.hh"
#include "rasodmg/iterator.hh"
#include "rasodmg/marray.hh"
#include "raslib/odmgtypes.hh"

#if (defined(EARLY_TEMPLATE) && defined(__UNDEF_EXEC_TYPES__))
#undef __EXECUTABLE__
#undef __UNDEF_EXEC_TYPES__
#endif

/*[2,33]*//* TYPEDEF ------------------------- GreyImage */
typedef r_Marray<r_Char/*[*:*,*:*]*/>  GreyImage;

/*[3,24]*//* TYPEDEF ------------------------- GreySet */
typedef r_Set<r_Ref<GreyImage> > GreySet;

/*[6,36]*//* TYPEDEF ------------------------- BoolImage */
typedef r_Marray<r_Boolean/*[*:*,*:*]*/>  BoolImage;

/*[7,24]*//* TYPEDEF ------------------------- BoolSet */
typedef r_Set<r_Ref<BoolImage> > BoolSet;

/*[10,1]*//* STRUCT -------------------------- RGBPixel */
struct RGBPixel
{
    r_Char    red;
    r_Char    green;
    r_Char    blue;
};
/*[11,37]*//* TYPEDEF ------------------------- RGBImage */
typedef r_Marray<RGBPixel/*[*:*,*:*]*/>  RGBImage;

/*[12,23]*//* TYPEDEF ------------------------- RGBSet */
typedef r_Set<r_Ref<RGBImage> > RGBSet;

#endif

