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
 *          No comments
 */



// For early templates
#ifndef _RVIEW_MDD_CPP_
#define _RVIEW_MDD_CPP_



#ifdef __GNUG__
#pragma implementation
#endif



#include <limits.h>
#include <float.h>
#include <string.h>



// Was this file included from rviewMDD.hh?
#ifdef EARLY_TEMPLATE
#ifndef __EXECUTABLE__
#define __EXECUTABLE__
#define _RVIEW_MDD_EXECUTABLE_
#endif
#endif

#include "rasodmg/transaction.hh"
#include "raslib/primitivetype.hh"
#include "raslib/structuretype.hh"
#include "raslib/endian.hh"
#include "rasodmg/alignedtiling.hh"
#include "rasodmg/storagelayout.hh"
#include "rviewMDD.hh"
#include "rviewTypes.hh"

#ifdef _RVIEW_MDD_EXECUTABLE_
#undef __EXECUTABLE__
#endif




#define MDD_FIXPREC 16





// auxData interpreted as double[2] containing min, max, initialised externally
template<class T>
int objectRange_temp(T *dest, const T *src, const mdd_function_desc *mfd, int dim, int datastep, void *auxData)
{
    long *pos;
    char *srcdata;
    T **srcptr;
    int i, j;
    double min, max, val;

    pos = new long[dim];
    srcptr = new T *[dim];

    srcdata = objectCalcStart((const char *)src, mfd, dim);

    for (i=0; i<dim; i++)
    {
        pos[i] = mfd[i].useLow;
        srcptr[i] = (T*)srcdata;
    }

    min = ((double*)auxData)[0];
    max = ((double*)auxData)[1];

    do
    {
        val = (double)(*srcptr[dim-1]);
        if (val < min) min = val;
        if (val > max) max = val;
        i = dim-1;
        while (i >= 0)
        {
            pos[i]++;
            srcptr[i] = (T*)(((char*)(srcptr[i])) + mfd[i].srcoff);
            if (pos[i] <= mfd[i].useHigh) break;
            pos[i] = mfd[i].useLow;
            i--;
        }
        if (i >= 0)
        {
            for (j=i+1; j<dim; j++) srcptr[j] = srcptr[i];
        }
    }
    while (i >= 0);

    ((double*)auxData)[0] = min;
    ((double*)auxData)[1] = max;

    delete [] srcptr;
    delete [] pos;

    return 1;
}


// Additional template code for objectScaleInter template
inline void objectScaleInterICore(double *srcf, double *destf, double pos, int idx1, int idx2)
{
    destf[idx1] = srcf[idx1] + pos * (srcf[idx2] - srcf[idx1]);
}

template<class T>
int objectScaleInter_temp(T *dest, const T *src, const mdd_function_desc *mfd, int dim, int datastep, void *auxData)
{
    double *pos, *f, *fwork;
    long *lastpos;
    long *ipos;
    long *dp;
    char **srcptr;
    char *srcbase;
    long offset;
    int i, j, k;
    int slowOffset;
    T *srcdata, *destdata;
    long *cubeoff = (long*)auxData;

    f = new double[(1<<dim)];
    fwork = new double[(1<<dim)];
    pos = new double[dim];
    ipos = new long[dim];
    lastpos = new long[dim];
    srcptr = new char*[dim];
    dp = new long[dim];

    // Calculate the starting point
    srcbase = objectCalcStart((const char*)src, mfd, dim);

    for (i=0; i<dim; i++)
    {
        pos[i] = (double)(mfd[i].useLow);
        lastpos[i] = mfd[i].useLow;
        srcptr[i] = srcbase;
        dp[i] = mfd[i].newLow;
    }

    destdata = dest;

    // Main loop: iterate over all dimensions except for the last which is handled separately
    // in its own loop for efficiency reasons.
    do
    {
        pos[dim-1] = (double)(mfd[dim-1].useLow);
        dp[dim-1] = mfd[dim-1].newLow;

        // slowOffset determines whether the fast table-lookup offset or the slower dimension looping
        // offset calculation can be used. Table-lookup is always possible as long as the current
        // position in the cube is not at the cube's boundary. In the innermost loop slowOffset only
        // has to be updated, not recalculated, because there is only a (new) overflow possible in the
        // innermost loop-dimension.
        slowOffset = 0;
        for (i=0; i<dim; i++)
        {
            ipos[i] = (long)(pos[i]);
            // Check for high rather than useHigh because we only get problems if its > high.
            if (ipos[i] >= mfd[i].high)
            {
                ipos[i] = mfd[i].useHigh;
                slowOffset = 1;
            }
        }

        srcbase = srcptr[dim-1];

        if (slowOffset != 0)
        {
            for (i=0; i<(1<<dim); i++)
            {
                offset = 0;
                for (j=0; j<dim; j++)
                {
                    // Check against high, not useHigh (see above)
                    if (((i & (1<<j)) != 0) && (ipos[j] < mfd[j].high)) offset += mfd[j].srcoff;
                }
                srcdata = (T*)(srcbase + offset);
                f[i] = (double)(*srcdata);
                //if (srcdata+offset >= srchigh) cout << "Overflow1" << endl;
            }
        }
        else
        {
            for (i=0; i<(1<<dim); i++)
            {
                srcdata = (T*)(srcbase + cubeoff[i]);
                f[i] = (double)(*srcdata);
            }
        }

        lastpos[dim-1] = (long)(pos[dim-1]);
        // Innermost loop
        for (k = mfd[dim-1].newHigh - mfd[dim-1].newLow + 1; k>0; k--)
        {
            // n-linear interpolation
            for (j=0; j<(1<<dim); j+=2)
            {
                objectScaleInterICore(f, fwork, (double)(pos[0] - ipos[0]), j, j+1);
            }
            for (i=1; i<dim; i++)
            {
                for (j=0; j<(1<<dim); j+=(1<<(i+1)))
                {
                    objectScaleInterICore(fwork, fwork, (double)(pos[i] - ipos[i]), j, j + (1<<i));
                }
            }
            // We go in sequential order (highest coordinate first)
            *destdata = (T)(fwork[0]);
            destdata = (T*)(((char*)destdata) + datastep);

            // end of innermost loop
            pos[dim-1] += mfd[dim-1].step;
            offset = (long)(pos[dim-1]);
            if (offset != lastpos[dim-1])
            {
                offset -= lastpos[dim-1];
                lastpos[dim-1] = (long)(pos[dim-1]);
                // Check against high, not useHigh
                if (lastpos[dim-1] <= mfd[dim-1].high)
                {
                    srcbase += offset*mfd[dim-1].srcoff;
                    ipos[dim-1] = lastpos[dim-1];
                }
                // high, not useHigh
                if (lastpos[dim-1] >= mfd[dim-1].high) slowOffset = 1;
                offset = (1<<(dim-1));
                memcpy(f, f + offset, offset*sizeof(double));

                if (slowOffset != 0)
                {
                    for (i=(1<<(dim-1)); i<(1<<dim); i++)
                    {
                        offset = 0;
                        for (j=0; j<dim-1; j++)
                        {
                            // high, not useHigh
                            if (((i & (1<<j)) != 0) && (ipos[j] < mfd[j].high)) offset += mfd[j].srcoff;
                        }
                        srcdata = (T*)(srcbase + offset);
                        f[i] = (double)(*srcdata);
                        //if (srcdata+offset >= srchigh) cout << "Overflow2" << endl;
                    }
                }
                else
                {
                    for (i=(1<<(dim-1)); i<(1<<dim); i++)
                    {
                        srcdata = (T*)(srcbase + cubeoff[i - (1<<(dim-1))]);
                        f[i] = (double)(*srcdata);
                    }
                }
            }
        }
        i = dim-2;
        while (i >= 0)
        {
            dp[i]++;
            pos[i] += mfd[i].step;
            offset = (long)(pos[i]);
            if (offset != lastpos[i])
            {
                offset -= lastpos[i];
                lastpos[i] = (long)(pos[i]);
                // high, not useHigh
                if (lastpos[i] <= mfd[i].high)
                {
                    srcptr[i] += offset*mfd[i].srcoff;
                    ipos[i] = lastpos[i];
                }
            }
            if (dp[i] <= mfd[i].newHigh) break;
            dp[i] = mfd[i].newLow;
            pos[i] = (double)(mfd[i].useLow);
            lastpos[i] = mfd[i].useLow;
            i--;
        }
        if (i >= 0)
        {
            for (j=i+1; j<dim; j++) srcptr[j] = srcptr[i];
        }
    }
    while (i >= 0);

    delete [] pos;
    delete [] f;
    delete [] fwork;
    delete [] ipos;
    delete [] lastpos;
    delete [] dp;
    delete [] srcptr;

    return 1;
}




template<class T>
int objectScaleAverage_temp(T *dest, const T *src, const mdd_function_desc *mfd, int dim, int datastep, void *aux)
{
    long *pos, *subpos, *dp;
    T **srcptr, **subptr;
    long lastpos, number;
    double avg;
    int i, j;
    T *destdata;
    char *srcdata;

    pos = new long[dim];
    subpos = new long[dim];
    dp = new long[dim];
    srcptr = new T*[dim];
    subptr = new T*[dim];

    srcdata = objectCalcStart((const char*)src, mfd, dim);

    for (i=0; i<dim; i++)
    {
        pos[i] = (mfd[i].useLow << MDD_FIXPREC);
        dp[i] = mfd[i].newLow;
        srcptr[i] = (T*)srcdata;
    }

    destdata = dest;
    // Iterate over all cells in the destination object
    do
    {
        //for (i=0; i<dim; i++) cout << (pos[i]>>MDD_FIXPREC) << ':' << (void*)(srcptr[i]) << ' '; cout << endl;
        // start averaging over a subcube
        avg = 0;
        number = 0;
        // init subcube iteration
        for (i=0; i<dim; i++)
        {
            subpos[i] = pos[i];
            subptr[i] = srcptr[dim-1];    // no mistake!
        }
        do
        {
            avg += (double)(*(subptr[dim-1]));
            number++;
            i = dim-1;
            while (i >= 0)
            {
                subpos[i] += (1<<MDD_FIXPREC);
                subptr[i] = (T*)(((char*)(subptr[i])) + mfd[i].srcoff);
                if ((subpos[i] >> MDD_FIXPREC) < ((pos[i] + mfd[i].lstep) >> MDD_FIXPREC)) break;
                subpos[i] = pos[i];
                i--;
            }
            if (i >= 0)
            {
                for (j=i+1; j<dim; j++) subptr[j] = subptr[i];
            }
        }
        while (i >= 0);

        *destdata = (T)(avg/number);
        destdata = (T*)(((char*)destdata) + datastep);

        i = dim-1;
        do
        {
            dp[i]++;
            lastpos = (pos[i] >> MDD_FIXPREC);
            pos[i] += mfd[i].lstep;
            srcptr[i] = (T*)(((char*)(srcptr[i])) + ((pos[i] >> MDD_FIXPREC) - lastpos) * mfd[i].srcoff);
            if (dp[i] <= mfd[i].newHigh) break;
            dp[i] = mfd[i].newLow;
            pos[i] = (mfd[i].useLow << MDD_FIXPREC);
            i--;
        }
        while (i >= 0);
        if (i >= 0)
        {
            for (j=i+1; j<dim; j++) srcptr[j] = srcptr[i];
        }
    }
    while (i >= 0);

    delete [] subptr;
    delete [] srcptr;
    delete [] dp;
    delete [] subpos;
    delete [] pos;

    return 1;
}



template<class T>
int objectScaleSimple_temp(T *dest, const T *src, const mdd_function_desc *mfd, int dim, int datastep, void *aux)
{
    long *pos, *dp;
    long lastpos;
    T **srcptr;
    T *destdata;
    char *srcdata;
    int i, j;

    pos = new long[dim];
    dp = new long[dim];
    srcptr = new T*[dim];

    srcdata = objectCalcStart((const char*)src, mfd, dim);

    for (i=0; i<dim; i++)
    {
        pos[i] = (mfd[i].useLow << MDD_FIXPREC);
        dp[i] = mfd[i].newLow;
        srcptr[i] = (T*)srcdata;
    }
    destdata = dest;

    do
    {
        *destdata = *(srcptr[dim-1]);
        destdata = (T*)(((char*)destdata) + datastep);
        i=dim-1;
        while (i >= 0)
        {
            dp[i]++;
            lastpos = (pos[i] >> MDD_FIXPREC);
            pos[i] += mfd[i].lstep;
            srcptr[i] = (T*)(((char*)srcptr[i]) + ((pos[i] >> MDD_FIXPREC) - lastpos)*mfd[i].srcoff);
            if (dp[i] <= mfd[i].newHigh) break;
            dp[i] = mfd[i].newLow;
            pos[i] = (mfd[i].useLow << MDD_FIXPREC);
            i--;
        }
        if (i >= 0)
        {
            for (j=i+1; j<dim; j++) srcptr[j] = srcptr[i];
        }
    }
    while (i >= 0);

    delete [] srcptr;
    delete [] dp;
    delete [] pos;

    return 1;
}



/* Must not compile non-template code when this file was just included */
#ifndef __EXECUTABLE__

// in raslib/odmgtypes.hh there are the following typedefs:
//   typedef unsigned char r_Boolean;
//   typedef unsigned char r_Char;
// so the template instantiation is done twice for the same type. gcc 2.95.2 doesn't
// like this.

// Sadly, this is necessary with VisualC...
#define DECLARE_MDD_FUNC(f) \
//  template int f<r_Boolean> MDD_FUNCTION_SIGNATURE(r_Boolean); \
template int f<r_Char> MDD_FUNCTION_SIGNATURE(r_Char); 
\
template int f<r_Octet> MDD_FUNCTION_SIGNATURE(r_Octet);
\
template int f<r_Short> MDD_FUNCTION_SIGNATURE(r_Short);
\
template int f<r_UShort> MDD_FUNCTION_SIGNATURE(r_UShort);
\
template int f<r_Long> MDD_FUNCTION_SIGNATURE(r_Long);
\
template int f<r_ULong> MDD_FUNCTION_SIGNATURE(r_ULong);
\
template int f<r_Float> MDD_FUNCTION_SIGNATURE(r_Float);
\
template int f<r_Double> MDD_FUNCTION_SIGNATURE(r_Double);

DECLARE_MDD_FUNC(objectScaleInter_temp);
DECLARE_MDD_FUNC(objectScaleAverage_temp);
DECLARE_MDD_FUNC(objectScaleSimple_temp);
DECLARE_MDD_FUNC(objectRange_temp);



// Calculate the starting point in the source cube
char *objectCalcStart(const char *src, const mdd_function_desc *mfd, int dim)
{
    int i;
    char *d;

    d = (char*)src;
    for (i=0; i<dim; i++)
    {
        d += (mfd[i].useLow - mfd[i].low) * mfd[i].srcoff;
    }
    return d;
}


/*
 *  Wrapper functions for those templates
 */

#define CAST_MDDPTR(t, src) \
  r_Ref<r_Marray<t> > mddPtr = (r_Ref<r_Marray<t> >)(src)


int mdd_createSubcube(r_Ref<r_GMarray> srcMdd, r_Ref<r_GMarray> &newMdd, r_Minterval *domain, r_Database *db)
{
    r_Dimension dim;
    char *destdata, *srcdata;
    r_Ref<r_GMarray> newMddPtr;
    r_Minterval interv;
    r_Minterval newInterv;
    int tpsize;
    int i, j;
    long length;
    mdd_function_desc *mfd;
    long *pos;
    char **srcptr;

    interv = srcMdd->spatial_domain();
    dim = interv.dimension();

    if (dim < 1) return 0;

    tpsize = srcMdd->get_type_length();

    newInterv = r_Minterval(dim);
    for (i=0; i<(int)dim; i++)
    {
        if (domain == NULL)
        {
            newInterv << interv[i];
        }
        else
        {
            newInterv << (*domain)[i];
        }
    }

    if ((destdata = mdd_objectFunctionInitMdd(srcMdd, newMddPtr, newInterv, tpsize, dim, db)) == NULL)
    {
        cerr << "Couldn't create new MDD object" << endl;
        return 0;
    }

    mfd = mdd_objectFunctionInitData(interv, newInterv, newInterv, tpsize, MDD_OBJECT_INIT_NEWIV);

    srcdata = objectCalcStart(srcMdd->get_array(), mfd, (int)dim);

    pos = new long[dim];
    srcptr = new char *[dim];

    for (i=0; i<(int)dim; i++)
    {
        pos[i] = mfd[i].useLow;
        srcptr[i] = srcdata;
    }

    // Lenght of one strip of data to copy
    length = (mfd[dim-1].useHigh - mfd[dim-1].useLow + 1) * tpsize;
    do
    {
        memcpy(destdata, srcptr[dim-1], length);
        destdata += length;
        i = (int)dim-2;
        while (i >= 0)
        {
            pos[i]++;
            srcptr[i] += mfd[i].srcoff;
            if (pos[i] <= mfd[i].useHigh) break;
            pos[i] = mfd[i].useLow;
            i--;
        }
        if (i >= 0)
        {
            for (j=i+1; j<(int)dim; j++) srcptr[j] = srcptr[i];
        }
    }
    while (i >= 0);

    delete [] srcptr;
    delete [] pos;
    delete [] mfd;

    newMdd = newMddPtr;

    return 1;
}



// Support functions for (recursive) member-by-member resampling of a data cube via interpolation
int mdd_objectFunctionPrim(const mdd_function_pointers *mfp, r_Primitive_Type *primType, const char *src, char *dest, const mdd_function_desc *mfd, int dim, int tpsize, void *auxData)
{
    switch (primType->type_id())
    {
    case r_Primitive_Type::BOOL:
        if (mfp->mddf_bool != NULL)
            mfp->mddf_bool((r_Boolean*)dest, (const r_Boolean *)src, mfd, dim, tpsize, auxData);
        break;
    case r_Primitive_Type::CHAR:
        if (mfp->mddf_char != NULL)
            mfp->mddf_char((r_Char*)dest, (const r_Char *)src, mfd, dim, tpsize, auxData);
        break;
    case r_Primitive_Type::OCTET:
        if (mfp->mddf_octet != NULL)
            mfp->mddf_octet((r_Octet *)dest, (const r_Octet*)src, mfd, dim, tpsize, auxData);
        break;
    case r_Primitive_Type::SHORT:
        if (mfp->mddf_short != NULL)
            mfp->mddf_short((r_Short *)dest, (const r_Short*)src, mfd, dim, tpsize, auxData);
        break;
    case r_Primitive_Type::USHORT:
        if (mfp->mddf_ushort != NULL)
            mfp->mddf_ushort((r_UShort *)dest, (const r_UShort*)src, mfd, dim, tpsize, auxData);
        break;
    case r_Primitive_Type::LONG:
        if (mfp->mddf_long != NULL)
            mfp->mddf_long((r_Long *)dest, (const r_Long*)src, mfd, dim, tpsize, auxData);
        break;
    case r_Primitive_Type::ULONG:
        if (mfp->mddf_ulong != NULL)
            mfp->mddf_ulong((r_ULong *)dest, (const r_ULong*)src, mfd, dim, tpsize, auxData);
        break;
    case r_Primitive_Type::FLOAT:
        if (mfp->mddf_float != NULL)
            mfp->mddf_float((r_Float *)dest, (const r_Float*)src, mfd, dim, tpsize, auxData);
        break;
    case r_Primitive_Type::DOUBLE:
        if (mfp->mddf_double != NULL)
            mfp->mddf_double((r_Double *)dest, (const r_Double*)src, mfd, dim, tpsize, auxData);
        break;
    default:
        return 0;
    }
    return 1;
}


// Support functions for (recursive) member-by-member resampling of a data cube via interpolation
int mdd_objectFunctionStruct(const mdd_function_pointers *mfp, r_Structure_Type *structType, const char *src, char *dest, const mdd_function_desc *mfd, int dim, int tpsize, void *auxData)
{
    r_Type *newType;
    unsigned long offset;
    char *inc, *outc;

    r_Structure_Type::attribute_iterator iter(structType->defines_attribute_begin());
    while (iter != structType->defines_attribute_end())
    {
        newType = (*iter).type_of().clone();
        offset = (*iter).offset();
        inc = (char*)src + offset;
        outc = (char*)dest + offset;

        if (newType->isStructType())
        {
            r_Structure_Type *newStructType = (r_Structure_Type*)newType;
            mdd_objectFunctionStruct(mfp, newStructType, inc, outc, mfd, dim, tpsize, auxData);
        }
        else
        {
            r_Primitive_Type *newPrimType = (r_Primitive_Type*)newType;
            mdd_objectFunctionPrim(mfp, newPrimType, inc, outc, mfd, dim, tpsize, auxData);
        }
        delete newType;
        iter++;
    }
    return 1;
}


int mdd_objectFunctionType(const mdd_function_pointers *mfp, const r_Type *baseType, const char *src, char *dest, const mdd_function_desc *mfd, int dim, int tpsize, void *auxData)
{
    if (((r_Type*)baseType)->isStructType())
    {
        r_Structure_Type *structType = (r_Structure_Type*)baseType;
        return mdd_objectFunctionStruct(mfp, structType, src, dest, mfd, dim, tpsize, auxData);
    }
    else
    {
        r_Primitive_Type *primType = (r_Primitive_Type*)baseType;
        return mdd_objectFunctionPrim(mfp, primType, src, dest, mfd, dim, tpsize, auxData);
    }
}


char *mdd_objectFunctionInitMdd(r_Ref<r_GMarray> mddPtr, r_Ref<r_GMarray> &newMddPtr, r_Minterval &newInterv, int tpsize, r_Dimension dim, r_Database *db)
{
    char *destdata;
    const char *b;
    r_Storage_Layout *store;

    if (mddPtr->get_storage_layout() != NULL)
    {
        store = new r_Storage_Layout(*(mddPtr->get_storage_layout()));
    }
    else
    {
        r_Minterval tileInterv(dim);
        r_Aligned_Tiling *dfltTiling;
        int dim, i;

        dim = newInterv.dimension();
        for (i=0; i<dim; i++)
        {
            tileInterv << r_Sinterval((r_Range)0, (r_Range)256);
        }
        dfltTiling = new r_Aligned_Tiling(tileInterv, tpsize * tileInterv.cell_count());
        store = new r_Storage_Layout(dfltTiling);
    }

    if (db == NULL)
    {
        newMddPtr = new r_GMarray(newInterv, tpsize, store);
    }
    else
    {
        newMddPtr = new (db) r_GMarray(newInterv, tpsize, store);
    }

    destdata = newMddPtr->get_array();
    newMddPtr->set_current_format(mddPtr->get_current_format());
    if ((b = mddPtr->get_type_name()) != NULL) newMddPtr->set_type_by_name(b);
    if ((b = mddPtr->get_type_structure()) != NULL) newMddPtr->set_type_structure(b);

    return destdata;
}


mdd_function_desc *mdd_objectFunctionInitData(r_Minterval &interv, r_Minterval &useInterv, r_Minterval &newInterv, int tpsize, unsigned int flags)
{
    int i, offset;
    r_Dimension dim;
    mdd_function_desc *mfd;

    dim = interv.dimension();
    mfd = new mdd_function_desc[dim];

    // Calculate scaling factors
    for (i=0; i<(int)dim; i++)
    {
        mfd[i].low = interv[i].low();
        mfd[i].high = interv[i].high();
        mfd[i].useLow = useInterv[i].low();
        mfd[i].useHigh = useInterv[i].high();
        if ((flags & MDD_OBJECT_INIT_NEWIV) != 0)
        {
            mfd[i].newLow = newInterv[i].low();
            mfd[i].newHigh = newInterv[i].high();
            if ((flags & MDD_OBJECT_INIT_FPSTEP) == MDD_OBJECT_INIT_FPSTEP)
            {
                // In order to avoid doubling right-boundary pixels we have to use (h-l), not (h-l+1)
                // as nominator
                mfd[i].step = ((double)(mfd[i].useHigh - mfd[i].useLow)) / (mfd[i].newHigh - mfd[i].newLow + 1);
            }
            else
            {
                double h = ((double)(mfd[i].useHigh - mfd[i].useLow + 1)) / (mfd[i].newHigh - mfd[i].newLow + 1);
                mfd[i].lstep = (long)(h * (1<<MDD_FIXPREC));
            }
        }
    }

    offset = tpsize;
    for (i=(int)dim-1; i>=0; i--)
    {
        mfd[i].srcoff = offset;
        offset *= (mfd[i].high - mfd[i].low + 1);
    }

    return mfd;
}



int mdd_objectRange(r_Ref<r_GMarray> mddPtr, r_Minterval &useInterv, double &min, double &max)
{
    r_Dimension dim;
    double minmax[2];
    r_Ref<r_GMarray> newMddPtr;   // Dummy...
    r_Minterval newInterv;    // Dummy...
    r_Minterval interv;
    const r_Type *baseType;
    int tpsize;
    mdd_function_desc *mfd;
    mdd_function_pointers mfp;

    interv = mddPtr->spatial_domain();
    dim = interv.dimension();

    if (dim < 1) return 0;

    if ((baseType = mddPtr->get_base_type_schema()) == NULL)
    {
        cerr << "No schema information available!" << endl;
        return 0;
    }

    tpsize = mddPtr->get_type_length();

    MDD_INIT_FUNCTIONS(mfp, objectRange_temp);

    mfd = mdd_objectFunctionInitData(interv, useInterv, newInterv, tpsize, 0);

    minmax[0] = DBL_MAX;
    minmax[1] = -DBL_MAX;    // these are unsigned!!!

    mdd_objectFunctionType(&mfp, baseType, mddPtr->get_array(), NULL, mfd, (int)dim, tpsize, (void*)minmax);

    min = minmax[0];
    max = minmax[1];

    delete [] mfd;

    return 1;
}



// Scale an object using n-linear interpolation (recommended for magnification)
int mdd_objectScaleInter(r_Ref<r_GMarray> mddPtr, r_Minterval &useInterv, r_Ref<r_GMarray> &newMdd, r_Minterval &newInterv)
{
    r_Dimension dim;
    r_Minterval interv;
    r_Ref<r_GMarray> newMddPtr;
    mdd_function_desc *mfd;
    int i, j;
    long *cubeoff;
    char *srcdata, *destdata;
    int tpsize;
    long offset;
    const r_Type *baseType;
    mdd_function_pointers mfp;

    interv = mddPtr->spatial_domain();
    dim = interv.dimension();
    if ((dim < 1) || (dim != newInterv.dimension())) return 0;

    // Type information available?
    if ((baseType = mddPtr->get_base_type_schema()) == NULL)
    {
        cerr << "No schema information available!" << endl;
        return 0;
    }

    tpsize = mddPtr->get_type_length();

    if ((destdata = mdd_objectFunctionInitMdd(mddPtr, newMddPtr, newInterv, tpsize, dim)) == NULL)
        return 0;

    MDD_INIT_FUNCTIONS(mfp, objectScaleInter_temp);

    // create temporary arrays
    mfd = mdd_objectFunctionInitData(interv, useInterv, newInterv, tpsize, MDD_OBJECT_INIT_FPSTEP);
    cubeoff = new long[(1<<dim)];

    // Debugging
    /*T *srchigh;
    offset=1;
    for (i=0; i<dim; i++) offset *= (interv[i].high() - interv[i].low() + 1);
    srchigh = (T*)(mddPtr->get_array()) + offset;*/

    srcdata = (char*)(mddPtr->get_array());

    // Precalculate cube-offsets for more speed in inner loop
    for (i=0; i<(1<<dim); i++)
    {
        offset = 0;
        for (j=0; j<(int)dim; j++)
        {
            if ((i & (1<<j)) != 0) offset += mfd[j].srcoff;
        }
        cubeoff[i] = offset;
    }

    mdd_objectFunctionType(&mfp, baseType, srcdata, destdata, mfd, (int)dim, tpsize, (void*)cubeoff);

    delete [] cubeoff;
    delete [] mfd;

    newMdd = newMddPtr;

    return 1;
}


// Scale an object by averaging cell values -- recommended for scaling down
int mdd_objectScaleAverage(r_Ref<r_GMarray> mddPtr, r_Minterval &useInterv, r_Ref<r_GMarray> &newMdd, r_Minterval &newInterv)
{
    r_Dimension dim;
    r_Minterval interv;
    r_Ref<r_GMarray> newMddPtr;
    mdd_function_desc *mfd;
    char *srcdata, *destdata;
    int tpsize;
    const r_Type *baseType;
    mdd_function_pointers mfp;

    interv = mddPtr->spatial_domain();
    dim = interv.dimension();

    if ((dim < 1) || (dim != newInterv.dimension())) return 0;

    if ((baseType = mddPtr->get_base_type_schema()) == NULL)
    {
        cerr << "No schema information available!" << endl;
        return 0;
    }

    tpsize = mddPtr->get_type_length();

    if ((destdata = mdd_objectFunctionInitMdd(mddPtr, newMddPtr, newInterv, tpsize, dim)) == NULL)
        return 0;

    MDD_INIT_FUNCTIONS(mfp, objectScaleAverage_temp);

    mfd = mdd_objectFunctionInitData(interv, useInterv, newInterv, tpsize, MDD_OBJECT_INIT_NEWIV);

    srcdata = mddPtr->get_array();

    mdd_objectFunctionType(&mfp, baseType, srcdata, destdata, mfd, (int)dim, tpsize, NULL);

    delete [] mfd;

    newMdd = (r_Ref<r_GMarray>)newMddPtr;

    return 1;
}


int mdd_objectScaleSimple(r_Ref<r_GMarray> mddPtr, r_Minterval &useInterv, r_Ref<r_GMarray> &newMdd, r_Minterval &newInterv)
{
    r_Dimension dim;
    r_Minterval interv;
    r_Ref<r_GMarray> newMddPtr;
    mdd_function_desc *mfd;
    char *srcdata, *destdata;
    int tpsize;
    const r_Type *baseType;
    mdd_function_pointers mfp;

    interv = mddPtr->spatial_domain();
    dim = interv.dimension();

    if ((dim < 1) || (dim != newInterv.dimension())) return 0;

    if ((baseType = mddPtr->get_base_type_schema()) == NULL)
    {
        cerr << "No schema information available!" << endl;
        return 0;
    }

    tpsize = mddPtr->get_type_length();

    if ((destdata = mdd_objectFunctionInitMdd(mddPtr, newMddPtr, newInterv, tpsize, dim)) == NULL)
        return 0;

    MDD_INIT_FUNCTIONS(mfp, objectScaleSimple_temp);

    mfd = mdd_objectFunctionInitData(interv, useInterv, newInterv, tpsize, MDD_OBJECT_INIT_NEWIV);

    srcdata = mddPtr->get_array();

    mdd_objectFunctionType(&mfp, baseType, srcdata, destdata, mfd, (int)dim, tpsize, NULL);

    newMdd = (r_Ref<r_GMarray>)newMddPtr;

    delete [] mfd;

    return 1;
}


int mdd_objectChangeEndianness(r_Ref<r_GMarray> mddPtr, r_Minterval &useInterv, r_Ref<r_GMarray> *newMdd, r_Minterval *newInterv)
{
    char *srcdata;
    const r_Base_Type *baseType;
    r_Ref<r_GMarray> newMddPtr;   // only used in "new" mode
    r_Minterval dom, *iterDom;
    int tpsize;

    if ((baseType = (r_Base_Type*)(mddPtr->get_base_type_schema())) == NULL)
    {
        cerr << "No schema information available!" << endl;
        return 0;
    }

    tpsize = mddPtr->get_type_length();

    srcdata = mddPtr->get_array();

    dom = mddPtr->spatial_domain();

    if (newMdd == NULL)
    {
        r_Endian::swap_array( baseType, dom, dom, srcdata, srcdata );
    }
    else
    {
        void *destdata;
        r_Dimension dim = dom.dimension();

        if ((destdata = mdd_objectFunctionInitMdd(mddPtr, newMddPtr, *newInterv, tpsize, dim)) == NULL)
            return 0;

        r_Endian::swap_array( baseType, dom, *newInterv, *newInterv, *newInterv, srcdata, destdata );
    }

    if (newMdd != NULL)
    {
        *newMdd = newMddPtr;
    }

    return 1;
}

#endif

#endif
