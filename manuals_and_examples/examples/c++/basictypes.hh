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
//  This file is created automatically by the rasdl processor.
//
//                       DO NOT EDIT
//------------------------------------------------------------

#ifndef __BASICTYPES_HH_
#define __BASICTYPES_HH_

//------------------------------------------------------------
//  Includes
//------------------------------------------------------------

#include "rasdaman.hh"

/*[2,25]*//* TYPEDEF ------------------------- GreyImage */
typedef r_Marray<r_Char>  GreyImage;

/*[3,24]*//* TYPEDEF ------------------------- GreySet */
typedef r_Set<r_Ref<GreyImage> > GreySet;

/*[6,28]*//* TYPEDEF ------------------------- BoolImage */
typedef r_Marray<r_Boolean>  BoolImage;

/*[7,24]*//* TYPEDEF ------------------------- BoolSet */
typedef r_Set<r_Ref<BoolImage> > BoolSet;

/*[10,1]*//* STRUCT -------------------------- RGBPixel */
struct RGBPixel
{
    r_Char    red;
    r_Char    green;
    r_Char    blue;
};
/*[11,29]*//* TYPEDEF ------------------------- RGBImage */
typedef r_Marray<RGBPixel>  RGBImage;

/*[12,23]*//* TYPEDEF ------------------------- RGBSet */
typedef r_Set<r_Ref<RGBImage> > RGBSet;

/*[15,35]*//* TYPEDEF ------------------------- ULongImage */
typedef r_Marray<r_ULong >  ULongImage;

/*[16,25]*//* TYPEDEF ------------------------- ULongSet */
typedef r_Set<r_Ref<ULongImage> > ULongSet;

/*[19,26]*//* TYPEDEF ------------------------- GreyCube */
typedef r_Marray<r_Char>  GreyCube;

/*[20,23]*//* TYPEDEF ------------------------- GreySet3 */
typedef r_Set<r_Ref<GreyCube> > GreySet3;

/*[24,29]*//* TYPEDEF ------------------------- BoolString */
typedef r_Marray<r_Boolean>  BoolString;

/*[25,25]*//* TYPEDEF ------------------------- BoolSet1 */
typedef r_Set<r_Ref<BoolString> > BoolSet1;

/*[27,29]*//* TYPEDEF ------------------------- BoolCube */
typedef r_Marray<r_Boolean>  BoolCube;

/*[28,23]*//* TYPEDEF ------------------------- BoolSet3 */
typedef r_Set<r_Ref<BoolCube> > BoolSet3;

/*[30,26]*//* TYPEDEF ------------------------- GreyString */
typedef r_Marray<r_Char>  GreyString;

/*[31,25]*//* TYPEDEF ------------------------- GreySet1 */
typedef r_Set<r_Ref<GreyString> > GreySet1;

/*[33,27]*//* TYPEDEF ------------------------- ShortString */
typedef r_Marray<r_Short >  ShortString;

/*[34,26]*//* TYPEDEF ------------------------- ShortSet1 */
typedef r_Set<r_Ref<ShortString> > ShortSet1;

/*[36,27]*//* TYPEDEF ------------------------- ShortImage */
typedef r_Marray<r_Short >  ShortImage;

/*[37,25]*//* TYPEDEF ------------------------- ShortSet */
typedef r_Set<r_Ref<ShortImage> > ShortSet;

/*[39,27]*//* TYPEDEF ------------------------- ShortCube */
typedef r_Marray<r_Short >  ShortCube;

/*[40,24]*//* TYPEDEF ------------------------- ShortSet3 */
typedef r_Set<r_Ref<ShortCube> > ShortSet3;

/*[42,36]*//* TYPEDEF ------------------------- UShortString */
typedef r_Marray<r_UShort >  UShortString;

/*[43,27]*//* TYPEDEF ------------------------- UShortSet1 */
typedef r_Set<r_Ref<UShortString> > UShortSet1;

/*[45,36]*//* TYPEDEF ------------------------- UShortImage */
typedef r_Marray<r_UShort >  UShortImage;

/*[46,26]*//* TYPEDEF ------------------------- UShortSet */
typedef r_Set<r_Ref<UShortImage> > UShortSet;

/*[48,36]*//* TYPEDEF ------------------------- UShortCube */
typedef r_Marray<r_UShort >  UShortCube;

/*[49,25]*//* TYPEDEF ------------------------- UShortSet3 */
typedef r_Set<r_Ref<UShortCube> > UShortSet3;

/*[51,26]*//* TYPEDEF ------------------------- LongString */
typedef r_Marray<r_Long >  LongString;

/*[52,25]*//* TYPEDEF ------------------------- LongSet1 */
typedef r_Set<r_Ref<LongString> > LongSet1;

/*[54,26]*//* TYPEDEF ------------------------- LongImage */
typedef r_Marray<r_Long >  LongImage;

/*[55,24]*//* TYPEDEF ------------------------- LongSet */
typedef r_Set<r_Ref<LongImage> > LongSet;

/*[57,26]*//* TYPEDEF ------------------------- LongCube */
typedef r_Marray<r_Long >  LongCube;

/*[58,23]*//* TYPEDEF ------------------------- LongSet3 */
typedef r_Set<r_Ref<LongCube> > LongSet3;

/*[60,35]*//* TYPEDEF ------------------------- ULongString */
typedef r_Marray<r_ULong >  ULongString;

/*[61,26]*//* TYPEDEF ------------------------- ULongSet1 */
typedef r_Set<r_Ref<ULongString> > ULongSet1;

/*[63,35]*//* TYPEDEF ------------------------- ULongCube */
typedef r_Marray<r_ULong >  ULongCube;

/*[64,24]*//* TYPEDEF ------------------------- ULongSet3 */
typedef r_Set<r_Ref<ULongCube> > ULongSet3;

/*[66,30]*//* TYPEDEF ------------------------- RGBString */
typedef r_Marray<RGBPixel>  RGBString;

/*[67,24]*//* TYPEDEF ------------------------- RGBSet1 */
typedef r_Set<r_Ref<RGBString> > RGBSet1;

/*[69,30]*//* TYPEDEF ------------------------- RGBCube */
typedef r_Marray<RGBPixel>  RGBCube;

/*[70,22]*//* TYPEDEF ------------------------- RGBSet3 */
typedef r_Set<r_Ref<RGBCube> > RGBSet3;

/*[72,27]*//* TYPEDEF ------------------------- FloatString */
typedef r_Marray<r_Float>  FloatString;

/*[73,26]*//* TYPEDEF ------------------------- FloatSet1 */
typedef r_Set<r_Ref<FloatString> > FloatSet1;

/*[75,27]*//* TYPEDEF ------------------------- FloatImage */
typedef r_Marray<r_Float>  FloatImage;

/*[76,25]*//* TYPEDEF ------------------------- FloatSet */
typedef r_Set<r_Ref<FloatImage> > FloatSet;

/*[78,27]*//* TYPEDEF ------------------------- FloatCube */
typedef r_Marray<r_Float>  FloatCube;

/*[79,24]*//* TYPEDEF ------------------------- FloatSet3 */
typedef r_Set<r_Ref<FloatCube> > FloatSet3;

/*[81,28]*//* TYPEDEF ------------------------- DoubleString */
typedef r_Marray<r_Double>  DoubleString;

/*[82,27]*//* TYPEDEF ------------------------- DoubleSet1 */
typedef r_Set<r_Ref<DoubleString> > DoubleSet1;

/*[84,28]*//* TYPEDEF ------------------------- DoubleImage */
typedef r_Marray<r_Double>  DoubleImage;

/*[85,26]*//* TYPEDEF ------------------------- DoubleSet */
typedef r_Set<r_Ref<DoubleImage> > DoubleSet;

/*[87,28]*//* TYPEDEF ------------------------- DoubleCube */
typedef r_Marray<r_Double>  DoubleCube;

/*[88,25]*//* TYPEDEF ------------------------- DoubleSet3 */
typedef r_Set<r_Ref<DoubleCube> > DoubleSet3;

#endif
