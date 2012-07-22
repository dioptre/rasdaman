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
 *  PURPOSE:
 *
 *  The rviewMDD module encapsulates various generic operations on
 *  MDD objects; it relies heavily on templates for this. All functions
 *  provided here are a) completely independent from the rest of rView
 *  and b) work on any kind of MDD, i.e. no restrictions whatsoever on
 *  dimensionality or base type. Most functions utilize
 *  mdd_objectFunctionType() for this which iterates over the MDD object
 *  basetype first and calls functions provided in the mdd_function_pointers
 *  structure which perform the required operation on each of the primitive
 *  basetypes.
 *
 *  Functions provided are:
 *
 *  - mdd_objectRange(): return the minimum and maximum cell values.
 *  - mdd_createSubcube(): creates a new MDD object by copying a subcube
 *    (or all of) a source MDD object; optionally persistent.
 *  - mdd_objectScaleInter(): creates new object by resampling (part of)
 *    the source object using n-linear interpolation. Use for upsampling.
 *  - mdd_objectScaleAverage(): creates new object by resampling (part of)
 *    the source object using averaging. Use for downsampling.
 *  - mdd_objectScaleSimple(): creates new object by scaling (part of)
 *    the source object using simple nearest neighbour. Fast but blocky.
 *  - mdd_objectChangeEndianness(): creates a new object where the
 *    endianness is inverted compared to the source object, or changes
 *    the object itself.
 *
 *  COMMENTS:
 *          None
 */

/**
*   @file rviewMDD.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_MDD_H_
#define _RVIEW_MDD_H_



#ifdef __GNUG__
#pragma interface
#endif


#include "rasodmg/ref.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/gmarray.hh"
#include "rasodmg/database.hh"
#include "raslib/odmgtypes.hh"

#include "raslib/primitivetype.hh"
#include "raslib/structuretype.hh"





/*
 *  Generic functionality on MDD objects -- data types and support functions
 */

// 1) Data types

// A descriptor of the mdd iteration
typedef struct mdd_function_desc
{
    double step;
    long lstep;
    long srcoff;
    long low; // Entire domain of source cube
    long high;
    long useLow;  // Domain that should be used, must be wholly contained in source cube
    long useHigh;
    long newLow;  // Domain of resulting cube
    long newHigh;
} mdd_function_desc;



// The signature of a basic mdd function
// Important note: VISUAL C needs the variable pointers first, followed by the
// constant pointers, or the variable pointers will be interpreted as constant
// ones too. No joke.
#define MDD_FUNCTION_SIGNATURE(type) \
  (type *dest, const type *src, const mdd_function_desc *mfd, int dim, int datastep, void *auxData)

typedef int (*mdd_func_bool)MDD_FUNCTION_SIGNATURE(r_Boolean);
typedef int (*mdd_func_char)MDD_FUNCTION_SIGNATURE(r_Char);
typedef int (*mdd_func_octet)MDD_FUNCTION_SIGNATURE(r_Octet);
typedef int (*mdd_func_short)MDD_FUNCTION_SIGNATURE(r_Short);
typedef int (*mdd_func_ushort)MDD_FUNCTION_SIGNATURE(r_UShort);
typedef int (*mdd_func_long)MDD_FUNCTION_SIGNATURE(r_Long);
typedef int (*mdd_func_ulong)MDD_FUNCTION_SIGNATURE(r_ULong);
typedef int (*mdd_func_float)MDD_FUNCTION_SIGNATURE(r_Float);
typedef int (*mdd_func_double)MDD_FUNCTION_SIGNATURE(r_Double);

// A structure of mdd functions for all atomic base types
typedef struct mdd_function_pointers
{
    mdd_func_bool mddf_bool;
    mdd_func_char mddf_char;
    mdd_func_octet mddf_octet;
    mdd_func_short mddf_short;
    mdd_func_ushort mddf_ushort;
    mdd_func_long mddf_long;
    mdd_func_ulong mddf_ulong;
    mdd_func_float mddf_float;
    mdd_func_double mddf_double;
} mdd_function_pointers;

// Init a mdd_function_pointers structure with a mdd function template (therefore the
// same function name for all members)
#define MDD_INIT_FUNCTIONS(mfp, tf) \
  mfp.mddf_bool = (mdd_func_bool)tf; \
  mfp.mddf_char = (mdd_func_char)tf; \
  mfp.mddf_octet = (mdd_func_octet)tf; \
  mfp.mddf_short = (mdd_func_short)tf; \
  mfp.mddf_ushort = (mdd_func_ushort)tf; \
  mfp.mddf_long = (mdd_func_long)tf; \
  mfp.mddf_ulong = (mdd_func_ulong)tf; \
  mfp.mddf_float = (mdd_func_float)tf; \
  mfp.mddf_double = (mdd_func_double)tf;



// 2) Support functions
extern char *objectCalcStart(const char *src, const mdd_function_desc *mfd, int dim);

// execute mdd functions over arbitrary objects
extern int mdd_objectFunctionPrim(const mdd_function_pointers *mfp, r_Primitive_Type *primType, const char *src, char *dest, const mdd_function_desc *mfd, int dim, int tpsize, void *auxData);
extern int mdd_objectFunctionStruct(const mdd_function_pointers *mfp, r_Structure_Type *structType, const char *src, char *dest, const mdd_function_desc *mfd, int dim, int tpsize, void *auxData);
extern int mdd_objectFunctionType(const mdd_function_pointers *mfp, const r_Type *baseType, const char *src, char *dest, const mdd_function_desc *mfd, int dim, int tpsize, void *auxData);

// Initialising mdd functions
#define MDD_OBJECT_INIT_NEWIV   1
#define MDD_OBJECT_INIT_FPSTEP  (2 | MDD_OBJECT_INIT_NEWIV)
extern char *mdd_objectFunctionInitMdd(r_Ref<r_GMarray> mddPtr, r_Ref<r_GMarray> &newMddPtr, r_Minterval &newInterv, int tpsize, r_Dimension dim, r_Database *db=NULL);
extern mdd_function_desc *mdd_objectFunctionInitData(r_Minterval &interv, r_Minterval &useInterv, r_Minterval &newInterv, int tpsize, unsigned int flags=0);






/*
 *  Wrapper functions for mdd function templates
 */
int mdd_objectRange(r_Ref<r_GMarray> mddObj, r_Minterval &useInterv, double &min, double &max);

int mdd_createSubcube(r_Ref<r_GMarray> srcMdd, r_Ref<r_GMarray> &newMdd, r_Minterval *domain, r_Database *db);

int mdd_objectScaleInter(r_Ref<r_GMarray> srcMdd, r_Minterval &useInterv, r_Ref<r_GMarray> &newMdd, r_Minterval &newInterv);

int mdd_objectScaleAverage(r_Ref<r_GMarray> srcMdd, r_Minterval &useInterv, r_Ref<r_GMarray> &newMdd, r_Minterval &newInterv);

int mdd_objectScaleSimple(r_Ref<r_GMarray> srcMdd, r_Minterval &useInterv, r_Ref<r_GMarray> &newMdd, r_Minterval &newInterv);

int mdd_objectChangeEndianness(r_Ref<r_GMarray> srcMdd, r_Minterval &useInterv, r_Ref<r_GMarray> *newMdd=NULL, r_Minterval *newInterv=NULL);

#if (defined(EARLY_TEMPLATE) && defined(__EXECUTABLE__))
#include "rviewMDD.cpp"
#endif

#endif
