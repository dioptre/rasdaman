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
*  Central definitions of shared objects and constants, global tool functions.
*
*  - All menu codes used in the program (MENU_...)
*  - Global tool functions for handling collections, base types, projection
*    strings, X events, ... . See ``Global functions''.
*  - rviewFrame class which must be the base class of all frames used in rView.
*  - rviewFrameMgr class for handling all of rView's frames (dispatching events
*    etc)
*  - rviewMultiline class for displaying several lines of non-editable text.
*  - rviewDialog class which is the base class for all rView's dialog windows.
*  - rviewErrorbox class, derived from rviewDialog.
*  - rviewProgress class, derived from rviewDialog.
*  - rviewResults class for displaying database results and dispatching object
*    viewers. Relies on code provided by rviewMDD for scaling/resampling/endian
*    conversions.
*  - rviewAbout class for displaying information about rView itself.
*  - rviewStringSet class for displaying a set of strings in a scrollable
*    list box.
*
*  COMMENTS:
*        none
*/


// Was file included via header? Then mask out all non-template code
#if (!defined(EARLY_TEMPLATE) || !defined(__EXECUTABLE__))

// Standard wxWindows preamble.
#ifdef __GNUG__
#pragma implementation
#endif


// changed in wxWindows 2.4.2:
//#include "wx_prec.h"
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// #include "wb_timer.h" -- PB 2006-jan-01


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream.h>



#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif


#include "raslib/rmdebug.hh"
#include "raslib/primitivetype.hh"
#include "raslib/structuretype.hh"

#include "rviewUtils.hh"
#include "rviewMDD.hh"
#include "rviewPrefs.hh"
#include "rviewColMap.hh"
#include "rviewDModes.hh"
#include "rviewOSection.hh"
#include "rviewTypes.hh"
#include "rviewThumb.hh"
#include "rviewSound.hh"
#include "rviewTypeMan.hh"
#include "labelManager.hh"


#ifdef __VISUALC__
const int rview_window_extra_height = 24;
const int rview_choice_sub_width = 0;
#else
const int rview_window_extra_height = 0;
const int rview_choice_sub_width = 32;
#endif






/*
 *  Global data
 */

// Base type to string translation tables

#if (MAXIMUM_DIMENSIONS != 4)
#error "Adapt tables for MAXIMUM DIMENSIONS!"
#endif

// String names of base types
char *rviewBaseTypes[] =
{
    "none",
    "Boolean",
    "Char",
    "Octet",
    "Short",
    "UShort",
    "Long",
    "ULong",
    "RGBPixel",
    "Float",
    "Double"
};

// String names of objects of baseType x and dimension y (-> rviewBaseType)
char *rviewTypeNames[][MAXIMUM_DIMENSIONS] =
{
    {"?", "?", "?", "?"},
    {"BoolString", "BoolImage", "BoolCube", "BoolCube4"},
    {"GreyString", "GreyImage", "GreyCube", "GreyCube4"},
    {"OctetString", "OctetImage", "OctetCube", "OctetCube4"},
    {"ShortString", "ShortImage", "ShortCube", "ShortCube4"},
    {"UShortString", "UShortImage", "UShortCube", "UShortCube4"},
    {"LongString", "LongImage", "LongCube", "LongCube4"},
    {"ULongString", "ULongImage", "ULongCube", "ULongCube4"},
    {"RGBString", "RGBImage", "RGBCube", "RGBCube4"},
    {"FloatString", "FloatImage", "FloatCube", "FloatCube4"},
    {"DoubleString", "DoubleImage" "DoubleCube", "DoubleCube4"}
};

// The same for sets
char *rviewSetNames[][MAXIMUM_DIMENSIONS] =
{
    {"?", "?", "?", "?"},
    {"BoolSet1", "BoolSet", "BoolSet3", "BoolSet4"},
    {"GreySet1", "GreySet", "GreySet3", "GreySet4"},
    {"OctetSet1", "OctetSet", "OctetSet3", "OctetSet4"},
    {"ShortSet1", "ShortSet", "ShortSet3", "ShortSet4"},
    {"UShortSet1", "UShortSet", "UShortSet3", "UShortSet4"},
    {"LongSet1", "LongSet", "LongSet3", "LongSet4"},
    {"ULongSet1", "ULongSet", "ULongSet3", "ULongSet4"},
    {"RGBSet1", "RGBSet", "RGBSet3", "RGBSet4"},
    {"FloatSet1", "FloatSet", "FloatSet3", "FloatSet4"},
    {"DoubleSet1", "DoubleSet", "DoubleSet3", "DoubleSet4"}
};



unsigned char lowerCaseTable[256];



// Various support classes
rviewFrameMgr *frameManager = NULL;

labelManager *lman = NULL;






/*
 *  GLOBAL functions
 */

// Frees all memory allocated by a collection descriptor.
void rviewDeleteCollection(collection_desc *coll)
{
    if (coll != NULL)
    {
        int i;
        collection_desc *ptr = coll;

        if (coll->collName != NULL) delete [] coll->collName;
        if (coll->collType != NULL) delete [] coll->collType;
        if (coll->collInfo != NULL) delete [] coll->collInfo;

        for (i=0; i<coll->number; i++)
        {
            if (coll->mddObjs != NULL)
            {
                if (!coll->mddObjs[i].mdd.is_null())
                {
                    coll->mddObjs[i].mdd.destroy();
                }
            }
            else if (coll->strObjs != NULL)
            {
                if (coll->strObjs[i] != NULL)
                {
                    delete [] coll->strObjs[i];
                }
            }
        }
        if (coll->mddObjs != NULL) delete [] coll->mddObjs;
        if (coll->strObjs != NULL) delete [] coll->strObjs;

        delete coll;
    }
}



/*
 *  Support function for rviewParseProjection
 */
static const char *rviewParseIndexMapping(const char *s, int idx, int dims, r_Point *mapIndex)
{
    const char *b, *d;
    r_Range value;

    b = s+1;
    if (mapIndex == NULL)
    {
        cerr << "Mode doesn't support reordering of dimensions." << endl;
    }

    value = (r_Range)strtol(b, (char**)&d, 0);
    if (b == d) return NULL;

    if ((value < 0) || (value >= (r_Range)dims))
    {
        cerr << "Bad dimension index " << value << endl;
        value = (r_Range)idx;
    }
    if (mapIndex != NULL) (*mapIndex)[idx] = value;

    while ((*d == ' ') || (*d == '\t')) d++;
    if (*d == ']') return d+1;
    return NULL;
}


/*
 *  Translate a projection string (now RasDaMan style, i.e. things
 *  like *:* are also allowed) into two diagonal r_Points. Returns
 *  number of dimension of projection string for success, 0 for an error.
 */

int rviewParseProjection(const r_Minterval &interv, r_Point &pt1, r_Point &pt2, const char *projString, unsigned int *freeDims, r_Point *mapIndex)
{
    int dims, i;
    const char *b, *d;
    r_Range value;

    if (freeDims != NULL) *freeDims = 0;
    dims = interv.dimension();
    pt1 = r_Point(dims);
    pt2 = r_Point(dims);

    b = projString;
    i = 0;
    while (*b != '\0')
    {
        while ((*b == ' ') || (*b == '\t')) b++;
        if (*b == ',') b++;
        while ((*b == ' ') || (*b == '\t')) b++;
        if (*b == '\0') break;
        if (i >= dims) return 0;
        if (*b == '*')
        {
            pt1[i] = interv[i].low();
            pt2[i] = interv[i].high();
            b++;
        }
        else
        {
            value = (r_Range)strtol(b, (char**)&d, 0);
            // no valid number found?
            if (b == d) return 0;
            b = d;
            if ((value < interv[i].low()) || (value > interv[i].high())) return 0;
            pt1[i] = value;
            pt2[i] = value;
        }
        if (mapIndex != NULL) (*mapIndex)[i] = i;
        while ((*b == ' ') || (*b == '\t')) b++;
        if (*b == '[')  // Explicit index-mapping?
        {
            if ((b = rviewParseIndexMapping(b, i, dims, mapIndex)) == NULL) return 0;
        }
        else if (*b == ':') // the upper boundaries follow after a colon.
        {
            if (freeDims != NULL) *freeDims |= (1<<i);
            b++;
            if (*b == '\0') break;
            if (*b == '*')
            {
                value = interv[i].high();
                b++;
            }
            else
            {
                value = (r_Range)strtol(b, (char**)&d, 0);
                if (b == d) return 0;
                b = d;
                if ((value < interv[i].low()) || (value > interv[i].high())) return 0;
            }
            pt2[i] = value;
            while ((*b == ' ') || (*b == '\t')) b++;
            if (*b == '[')
            {
                if ((b = rviewParseIndexMapping(b, i, dims, mapIndex)) == NULL) return 0;
            }
        }
        i++;
    }
    return i;
}





/*
 *  Determine the base type of an MDD object, returning a simple identifier.
 */
rviewBaseType rviewGetBasetype(r_Object *obj)
{
    rviewBaseType baseType;
    const r_Type *bt;

    baseType = rbt_none;

    // New type schema...
    bt = obj->get_type_schema();
    if (bt->type_id() == r_Type::MARRAYTYPE)
        bt = ((r_GMarray*)obj)->get_base_type_schema();
    else if (bt->type_id() == r_Type::COLLECTIONTYPE)
        bt = ((r_Collection<r_Ref_Any>*)obj)->get_element_type_schema();

    if (bt != NULL)
    {
        if (((r_Type*)bt)->isStructType())
        {
            if (((r_Base_Type*)bt)->size() == 3)
                baseType = rbt_rgb;
            else
                baseType = rbt_none;
        }
        else
        {
            r_Primitive_Type *pt = (r_Primitive_Type *)bt;

            switch (pt->type_id())
            {
            case r_Primitive_Type::BOOL:
                baseType = rbt_bool;
                break;
            case r_Primitive_Type::CHAR:
                baseType = rbt_char;
                break;
            case r_Primitive_Type::OCTET:
                baseType = rbt_uchar;
                break;
            case r_Primitive_Type::SHORT:
                baseType = rbt_short;
                break;
            case r_Primitive_Type::USHORT:
                baseType = rbt_ushort;
                break;
            case r_Primitive_Type::LONG:
                baseType = rbt_long;
                break;
            case r_Primitive_Type::ULONG:
                baseType = rbt_ulong;
                break;
            case r_Primitive_Type::FLOAT:
                baseType = rbt_float;
                break;
            case r_Primitive_Type::DOUBLE:
                baseType = rbt_double;
                break;
            default:
                baseType = rbt_none;
                break;
            }
        }
    }
    else
    {
        char *name = (char*)(obj->get_type_name());
        if (name != NULL)
        {
            if (strcmp(name, "GreyImage") == 0) baseType = rbt_char;
            else if (strcmp(name, "BoolImage") == 0) baseType = rbt_bool;
            else if (strcmp(name, "RGBImage") == 0) baseType = rbt_rgb;
        }
    }
    return baseType;
}



/*
 *  rviewPrintTypedCell() prints the contents of a cell with a given base type into the buffer,
 *  returning the number of characters written.
 */

// For internal use only
static void printPrimitiveCell(r_Primitive_Type *primType, char **buff, char *data, int numberBase)
{
    char *b = *buff;

    switch (numberBase)
    {
    case 8:
        switch (primType->type_id())
        {
        case r_Primitive_Type::BOOL:
            b += sprintf(b, "%3o", *((r_Boolean*)data));
            break;
        case r_Primitive_Type::CHAR:
            b += sprintf(b, "%3o", *((r_Char*)data));
            break;
        case r_Primitive_Type::OCTET:
            b += sprintf(b, "%3o", *((r_Octet*)data));
            break;
        case r_Primitive_Type::SHORT:
            b += sprintf(b, "%6o", *((r_Short*)data));
            break;
        case r_Primitive_Type::USHORT:
            b += sprintf(b, "%6o", *((r_UShort*)data));
            break;
        case r_Primitive_Type::LONG:
            b += sprintf(b, "%11lo", *((r_Long*)data));
            break;
        case r_Primitive_Type::ULONG:
            b += sprintf(b, "%11lo", *((r_ULong*)data));
            break;
        case r_Primitive_Type::FLOAT:
            b += sprintf(b, "%011o", *((r_Long*)data));
            break;
        case r_Primitive_Type::DOUBLE:
            b += sprintf(b, "%011o:%011o", ((r_Long*)data)[0], ((r_Long*)data)[1]);
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (primType->type_id())
        {
        case r_Primitive_Type::BOOL:
            b += sprintf(b, "%02x", *((r_Boolean*)data));
            break;
        case r_Primitive_Type::CHAR:
            b += sprintf(b, "%02x", *((r_Char*)data));
            break;
        case r_Primitive_Type::OCTET:
            b += sprintf(b, "%02x", *((r_Octet*)data));
            break;
        case r_Primitive_Type::SHORT:
            b += sprintf(b, "%04x", *((r_Short*)data));
            break;
        case r_Primitive_Type::USHORT:
            b += sprintf(b, "%04x", *((r_UShort*)data));
            break;
        case r_Primitive_Type::LONG:
            b += sprintf(b, "%08x", *((r_Long*)data));
            break;
        case r_Primitive_Type::ULONG:
            b += sprintf(b, "%08x", *((r_ULong*)data));
            break;
        case r_Primitive_Type::FLOAT:
            b += sprintf(b, "%08x", *((r_Long*)data));
            break;
        case r_Primitive_Type::DOUBLE:
            b += sprintf(b, "%08x:%08x", ((r_Long*)data)[0], ((r_Long*)data)[1]);
            break;
        default:
            break;
        }
        break;
    default:
        switch (primType->type_id())
        {
        case r_Primitive_Type::BOOL:
            b += sprintf(b, "%3d", *((r_Boolean*)data));
            break;
        case r_Primitive_Type::CHAR:
            b += sprintf(b, "%3d", *((r_Char*)data));
            break;
        case r_Primitive_Type::OCTET:
            b += sprintf(b, "%3d", *((r_Octet*)data));
            break;
        case r_Primitive_Type::SHORT:
            b += sprintf(b, "%5d", *((r_Short*)data));
            break;
        case r_Primitive_Type::USHORT:
            b += sprintf(b, "%5d", *((r_UShort*)data));
            break;
        case r_Primitive_Type::LONG:
            b += sprintf(b, "%10ld", *((r_Long*)data));
            break;
        case r_Primitive_Type::ULONG:
            b += sprintf(b, "%10ld", *((r_ULong*)data));
            break;
        case r_Primitive_Type::FLOAT:
            b += sprintf(b, "%g", *((r_Float*)data));
            break;
        case r_Primitive_Type::DOUBLE:
            b += sprintf(b, "%g", *((r_Double*)data));
            break;
        default:
            break;
        }
        break;
    }
    *buff = b;
}

// For internal use only
void printStructuredCell(r_Structure_Type *structType, char **buff, char *data, int numberBase)
{
    r_Type *newType;
    unsigned long off;
    char *b;

    b = *buff;
    *b++ = '{';
    r_Structure_Type::attribute_iterator iter(structType->defines_attribute_begin());
    while (iter != structType->defines_attribute_end())
    {
        newType = (*iter).type_of().clone();
        off = (*iter).offset();

        if (newType->isStructType())
        {
            r_Structure_Type *newStructType = (r_Structure_Type*)newType;
            printStructuredCell(newStructType, &b, data + off, numberBase);
        }
        else
        {
            r_Primitive_Type *newPrimType = (r_Primitive_Type*)newType;
            printPrimitiveCell(newPrimType, &b, data + off, numberBase);
        }
        delete newType;
        *b++ = ',';
        iter++;
    }
    b[-1] = '}';
    *b = '\0';
    *buff = b;
}

int rviewPrintTypedCell(const r_Type *baseType, char *buffer, char *data, int numberBase)
{
    char *b = buffer;

    if (((r_Type*)baseType)->isStructType())
    {
        r_Structure_Type *structType = (r_Structure_Type*)baseType;
        printStructuredCell(structType, &b, data, numberBase);
    }
    else
    {
        r_Primitive_Type *primType = (r_Primitive_Type*)baseType;
        printPrimitiveCell(primType, &b, data, numberBase);
    }
    return (int)(b - buffer);
}




/*
 *  Quicksort a char *[]
 */

#define QUICKSORT_STRING_SWAP(x,y)  h=array[x]; array[x]=array[y]; array[y]=h;

void rviewQuicksortStrings(char *array[], int from, int to)
{
    while (from < to)
    {
        int i, j;
        char *h;

        j = (from+to)/2;
        QUICKSORT_STRING_SWAP(from, j);
        j=from;
        for (i=from+1; i<=to; i++)
        {
            if (strcmp(array[i], array[from]) < 0)
            {
                j++;
                QUICKSORT_STRING_SWAP(i, j);
            }
        }
        QUICKSORT_STRING_SWAP(from, j);

        if ((j-from) < (to-j))
        {
            rviewQuicksortStrings(array, from, j-1);
            from = j+1;
        }
        else
        {
            rviewQuicksortStrings(array, j+1, to);
            to = j-1;
        }
    }
}



/*
 *  Init character lookup tables (e.g. lower case table)
 */
void rviewInitCharacterTables(void)
{
    int i;

    for (i=0; i<256; i++) lowerCaseTable[i] = tolower(i);
}



#define LOOKUP_KEYWORD_CORE \
  while (count != 0) \
  { \
    b = (const unsigned char*)(kti[pos].keyword); \
    d = (const unsigned char*)key; \
    while (TRANSLATE_CHARACTER(b) == TRANSLATE_CHARACTER(d)) \
    { \
      if (*b == 0) break; b++; d++; \
    } \
    if ((*b == 0) && (*d == 0)) return kti[pos].ident; \
    if (TRANSLATE_CHARACTER(b) >= TRANSLATE_CHARACTER(d)) \
    { \
      pos -= step; if (pos < 0) pos = 0; \
    } \
    else \
    { \
      pos += step; if (pos >= tabsize) pos = tabsize - 1; \
    } \
    step = (step+1) >> 1; count >>= 1; \
  }

/*
 *  Lookup a keyword in a SORTED keyword_to_ident table. Case sensitivity
 *  is optional but has to match the sorting order of the table, of course.
 */
int rviewLookupKeyword(const char *key, const keyword_to_ident_c *kti, int tabsize, bool caseSensitive)
{
    int pos, step, count;
    const unsigned char *b, *d;

    count = tabsize;
    pos = (tabsize + 1) >> 1;
    if (pos >= tabsize) pos = tabsize - 1;
    step = (pos + 1) >> 1;
    if (caseSensitive)
    {
#define TRANSLATE_CHARACTER(x)  (*x)
        LOOKUP_KEYWORD_CORE;
#undef TRANSLATE_CHARACTER
    }
    else
    {
#define TRANSLATE_CHARACTER(x)  lowerCaseTable[(*x)]
        LOOKUP_KEYWORD_CORE;
#undef TRANSLATE_CHARACTER
    }
    return -1;
}



/* Helper function making sure min/max of the init structure is used by the
   colourspace mapper */

static void rviewEnsureCspaceRange(colourspaceMapper *csmap, const colourspace_params *cp)
{
    // make sure the min/max values are taken from the init structure as well
    if (cp != NULL)
    {
        // assume it's initialized if either values differs from 0
        if ((cp->minVal != 0.0) || (cp->maxVal != 0.0))
        {
            colourspace_params par;
            csmap->getParameters(&par);
            par.minVal = cp->minVal;
            par.maxVal = cp->maxVal;
            // do not do auto-update, otherwise we're stuck in an infinite loop
            csmap->colourspaceChanged(&par, FALSE);
            //cout << "MIN " << par.minVal << ", MAX " << par.maxVal << endl;
        }
    }
}


/*
 *  Check whether colourspace mapping is possible for the given
 *  base type and return 1 if so. *csmap must be a pointer to the
 *  colourspaceMapper to use or NULL if there isn't one yet in
 *  which case a new one will have been created on returning 1.
 *  If cmap == NULL the function just returns 0 or 1. If the int*
 *  arguments are not NULL they're set up correctly on exit too.
 */
int rviewCheckInitCspace(rviewBaseType baseType, colourspaceMapper **csmap, r_Ref<r_GMarray> &mddObj, bool fullRange, r_Minterval *domain, int w, int *newPitch, int *newDepth, int *newPad, int *virtualPitch, const colourspace_params *cp)
{
    colourspace_params par;

    switch (baseType)
    {
    case rbt_char:
    case rbt_uchar:
    case rbt_short:
    case rbt_ushort:
        if (csmap == NULL) return 1;
        if (*csmap == NULL)
        {
            memcpy(&par, (cp == NULL) ? &(prefs->csp) : cp, sizeof(colourspace_params));
            *csmap = new colourspaceMapper(mddObj, baseType, &par, fullRange, domain);
        }
        else
            (*csmap)->bindMapper(mddObj, baseType, fullRange, domain, cp);

        rviewEnsureCspaceRange(*csmap, cp);

        // always call this and let the mapper sort out whether it has to update
        (*csmap)->buildCSTab15();
        if (newPitch == NULL) return 1;
        if (*newPad < 32) *newPad = 32;
        *newDepth = 15;
        switch (*newPad)
        {
        case 32:
            *newPitch = (w*2 + 3) & ~3;
            break;
        case 64:
            *newPitch = (w*2 + 7) & ~7;
            break;
        case 128:
            *newPitch = (w*2 + 15) & ~15;
            break;
        case 256:
            *newPitch = (w*2 + 31) & ~31;
            break;
        default:
            cerr << "Bad pad " << *newPad << endl;
            return 0;
        }
        if (virtualPitch != NULL) *virtualPitch = *newPitch;
        return 1;
        break;
    case rbt_long:
    case rbt_ulong: // using 24bpp only makes sense for long
    case rbt_float:
    case rbt_double:    // ... or larger types
    {
        int baseWidth;

        baseWidth = 4*w;

        if (csmap == NULL) return 1;
        if (*csmap == NULL)
        {
            memcpy(&par, (cp == NULL) ? &(prefs->csp) : cp, sizeof(colourspace_params));
            *csmap = new colourspaceMapper(mddObj, baseType, &par, fullRange, domain);
        }
        else
            (*csmap)->bindMapper(mddObj, baseType, fullRange, domain, cp);

        rviewEnsureCspaceRange(*csmap, cp);

        (*csmap)->buildCSTab24();
        if (newPitch == NULL) return 1;
        if (*newPad < 32) *newPad = 32;
        *newDepth = 32;
        switch (*newPad)
        {
        case 32:
            *newPitch = (baseWidth + 3) & ~3;
            break;
        case 64:
            *newPitch = (baseWidth + 7) & ~7;
            break;
        case 128:
            *newPitch = (baseWidth + 15) & ~15;
            break;
        case 256:
            *newPitch = (baseWidth + 31) & ~31;
            break;
        default:
            cerr << "Bad pad " << *newPad << endl;
            return 0;
        }
        if (virtualPitch != NULL)
            *virtualPitch = (baseType == rbt_double) ? 2*(*newPitch) : (*newPitch);
        return 1;
    }
    break;
    default:
        break;
    }
    return 0;
}



/*
 *  Smart number conversion functions. They also understand the 0x
 *  prefix and convert the number correctly.
 */

long asctol(const char *str)
{
    return stringtol(str, NULL);
}

int asctoi(const char *str)
{
    return (int)stringtol(str, NULL);
}

double asctof(const char *str)
{
    return stringtof(str, NULL);
}

long stringtol(const char *str, char **endptr)
{
    const char *b = str;
    long value;

    while (isspace((unsigned int)(*b))) b++;
    if ((b[0] == '0') && ((b[1] == 'x') || (b[1] == 'X')))
    {
        b += 2;
        value = strtol(b, endptr, 16);
    }
    else
    {
        value = strtol(b, endptr, 10);
    }
    if ((endptr != NULL) && ((const char*)(*endptr) == b)) *endptr = (char*)str;
    return value;
}

double stringtof(const char *str, char **endptr)
{
    const char *b = str;
    double value;

    while (isspace((unsigned int)(*b))) b++;
    if ((b[0] == '0') && ((b[1] == 'x') || (b[1] == 'X')))
    {
        b += 2;
        value = (double)strtol(b, endptr, 16);
    }
    else
    {
        value = strtod(b, endptr);
    }
    if ((endptr != NULL) && ((const char*)(*endptr) == b)) *endptr = (char*)str;
    return value;
}





/*
 *  Dynamic string class for easy management of dynamically allocated string storage
 */

const char DynamicString::emptyString[] = "";


DynamicString::DynamicString(void)
{
    myString = NULL;
}

DynamicString::DynamicString(const DynamicString &ms)
{
    if (ms.myString == NULL)
    {
        myString = NULL;
    }
    else
    {
        myString = new char[strlen(ms.myString) + 1];
        strcpy(myString, ms.myString);
    }
}


DynamicString::DynamicString(const char *str)
{
    if (str == NULL)
    {
        myString = NULL;
    }
    else
    {
        myString = new char[strlen(str) + 1];
        strcpy(myString, str);
    }
}


DynamicString::~DynamicString(void)
{
    if (myString != NULL) delete [] myString;
}


DynamicString &DynamicString::first(const char *str, unsigned int num)
{
    if (myString != NULL)
    {
        delete [] myString;
        myString = NULL;
    }
    if (num != 0)
    {
        unsigned int len = strlen(str);
        if (len > num) len = num;
        myString = new char[len + 1];
        strncpy(myString, str, len);
        myString[len] = '\0';
    }
    return *this;
}


DynamicString &DynamicString::operator=(const DynamicString &ms)
{
    if (myString != NULL)
    {
        delete [] myString;
        myString = NULL;
    }
    if (ms.myString != NULL)
    {
        myString = new char[strlen(ms.myString) + 1];
        strcpy(myString, ms.myString);
    }
    return *this;
}


DynamicString &DynamicString::operator=(const char *str)
{
    if (myString != NULL)
    {
        delete [] myString;
        myString = NULL;
    }
    if (str != NULL)
    {
        myString = new char[strlen(str) + 1];
        strcpy(myString, str);
    }
    return *this;
}


const char *DynamicString::ptr(void) const
{
    if (myString == NULL) return emptyString;
    return myString;
}

DynamicString::operator const char*(void) const
{
    return ptr();
}


bool DynamicString::operator==(const DynamicString &str) const
{
    return (strcmp(ptr(), str.ptr()) == 0);
}


bool DynamicString::operator==(const char *str) const
{
    return (strcmp(ptr(), str) == 0);
}






/*
 *  Generic handler function for events like button- and key-presses.
 *  It passes the event to the frameManager which then broadcasts
 *  it to all the frames it knows. If a frame recognizes obj as one
 *  of its children it claims the event, thus aborting the broadcast.
 */

void rviewEventHandler(wxObject &obj, wxEvent &evt)
{
    if (frameManager != NULL)
        frameManager->broadcastEvent(obj, evt);
    //cout << endl;
}






/*
 *  rviewFrame member functions.
 *  This is an abstract class that is only used for deriving other
 *  (window/frame) classes from it.
 */

rviewFrame::rviewFrame(wxFrame *parent, char *title, int x, int y, int w, int h) : wxFrame(parent, title, x, y, w, h)
{
    RMDBGONCE( 3, RMDebug::module_applications, "rviewFrame", "rviewFrame( " << this << ", ...)" );

    if (frameManager != NULL)
        frameManager->registerFrame(this);

    parentFrame = NULL;
    // Init the size with illegal values to make sure the first OnSize gets through
    frameWidth = -1;
    frameHeight = -1;
    // All the frame's children should be destroyed when the frame is destroyed.
    frames = new rviewFrameMgr(TRUE);
}


rviewFrame::~rviewFrame(void)
{
    RMDBGONCE( 3, RMDebug::module_applications, "rviewFrame", "~rviewFrame()  " << this );

    delete frames;

    if (frameManager != NULL)
        frameManager->deregisterFrame(this);

    if (parentFrame != NULL)
    {
        parentFrame->deregisterChild(this);
    }
}


const char *rviewFrame::getFrameName(void) const
{
    return "rviewFrame";
}

rviewFrameType rviewFrame::getFrameType(void) const
{
    return rviewFrameTypeGeneric;
}


void rviewFrame::setParent(rviewFrame *parent)
{
    parentFrame = parent;
}


void rviewFrame::registerChild(rviewFrame *child)
{
    child->setParent(this);
    frames->registerFrame(child);
}


void rviewFrame::deregisterChild(rviewFrame *child)
{
    frames->deregisterFrame(child);
    child->setParent(NULL);
}


// Called by the global frame manager when the application is about to die.
int rviewFrame::requestQuit(int level)
{
    // Default action on a hard quit: die
    if (level != 0)
    {
        // Do not delete the children, that would upset the frame manager's loop.
        frames->setDeleteMode(FALSE);
        setParent(NULL);
        //delete this;
        Close(TRUE);
    }
    return 0;
}



// Called on a user event
int rviewFrame::userEvent(const user_event &ue)
{
    // Default action: nothing. Overload function in derived class if you're
    // interested in user messages.
    return 0;
}


// Called by the rviewFrameMgr object to determine which frame receives an event
int rviewFrame::checkobj(wxObject &obj)
{
    return checkobj_rec((wxWindow*)this, obj);
}


// Used internally by rviewFrame::checkobj(); don't access directly. Checks
// recursively all the children of a given frame against obj.
int rviewFrame::checkobj_rec(wxWindow *whence, wxObject &obj)
{
    if (&obj == (wxObject*)whence) return 1;
    else
    {
        int i, n;
        wxList *list;

        list = whence->GetChildren();
        n = list->Number();
        for (i=0; i<n; i++)
        {
            if (checkobj_rec((wxWindow*)(list->Nth(i)->Data()), obj) != 0) return 1;
        }
    }
    return 0;
}



// Virtual function. If a derived frame class should refuse to die in some
// circumstances, it must overload his function.
bool rviewFrame::OnClose(void)
{
    return TRUE;
}


void rviewFrame::childMouseEvent(wxWindow *child, wxMouseEvent &mevt)
{
    // by default this is ignored.
}






/*
 *  rviewFrameMgr member functions
 */

rviewFrameMgr::rviewFrameMgr(void)
{
    listLength = 0;
    deleteChildren = FALSE;
    frameList = NULL;
    tailList = NULL;
}


rviewFrameMgr::rviewFrameMgr(bool delChild)
{
    listLength = 0;
    deleteChildren = delChild;
    frameList = NULL;
    tailList = NULL;
}


rviewFrameMgr::~rviewFrameMgr(void)
{
    int i;
    frame_list *list, *last;

    // Deletes all the frames and their children
    // Unlink all frames first! Otherwise there'll be trouble because the Frame Manager
    // will loop over the frames and delete them; but if the frames have a valid parent
    // they will deregister there which will screw up the Frame Manager's frame list
    // _during the loop_ and hence crash.
    list = frameList;
    for (i=0; i<listLength; i++)
    {
        last = list;
        list = list->next;
        // If it's the global frame manager it shouldn't delete the children, that's
        // wxWindows' job. If it's a manager local to a frame it should delete all
        // that frame's children, however.
        if (deleteChildren)
        {
            last->frame->setParent(NULL);
            //delete last->frame;
            last->frame->Close(TRUE);
        }
        delete last;
    }
}


void rviewFrameMgr::registerFrame(rviewFrame *client)
{
    frame_list *entry;

    //cout << "frameManager::registerFrame(" << (void*)client << ")" << endl;
    if ((entry = new frame_list) == NULL)
    {
        //cerr << "frameManager: Unable to claim memory!\n" << endl;
        return;
    }

    entry->frame = client;
    entry->next = NULL;

    if (frameList == NULL)
    {
        frameList = entry;
        tailList = entry;
    }
    else
    {
        tailList->next = entry;
        tailList = entry;
    }
    listLength++;
    //cout << "Frame manager: Added frame to list. New length = " << listLength << endl;
    //cout << "(anchor = " << (void*)frameList << ", tail = " << (void*)tailList << ")" << endl;
}


void rviewFrameMgr::deregisterFrame(rviewFrame *client)
{
    int i;
    frame_list *list, *last;

    //cout << "frameManager::deregisterFrame(" << (void*)client << ")" << endl;
    list = frameList;
    last = NULL;
    for (i=0; i<listLength; i++)
    {
        if (list->frame == client)
        {
            list = list->next;
            if (last == NULL)
            {
                delete frameList;
                frameList = list;
                last = list;
            }
            else
            {
                delete last->next;
                last->next = list;
            }
            listLength--;
            if (last == NULL) tailList = NULL;
            else
            {
                while (last->next != NULL) last = last->next;
                tailList = last;
            }
            //cout << "Frame manager: Removed frame from list. New length = " << listLength << endl;
            //cout << "(anchor = " << (void*)frameList << ", tail = " << (void*)tailList << ")" << endl;
            return;
        }
        last = list;
        list = list->next;
    }
}



int rviewFrameMgr::numberOfFrames(void) const
{
    return listLength;
}



void rviewFrameMgr::setDeleteMode(bool delChild)
{
    deleteChildren = delChild;
}


// Issues re-label requests to all frames.
void rviewFrameMgr::labelAll(void)
{
    int i;
    frame_list *list;

    //cout << "frameManager::labelAll()" << endl;
    list = frameList;
    for (i=0; i<listLength; i++)
    {
        list->frame->label();
        list = list->next;
    }
}



/*
 *  Broadcasts the event to all frames until one claims the event by returning
 *  a value != 0. Warning: this function has to be re-entrant!
 */

void rviewFrameMgr::broadcastEvent(wxObject &obj, wxEvent &evt)
{
    int i;
    frame_list *list;

    // Note: the broadcast has to go in two steps, first determining which frame
    // knows the object, then terminating the loop and calling the object as the
    // last action. The reason is that this function is re-entrant and would
    // crash badly if the frame list changed during the loop.
    //cout << "frameManager::broadcastEvent()" << endl;
    list = frameList;
    for (i=0; i<listLength; i++)
    {
        //cout << "Broadcast to " << (void*)(list->frame) << endl;
        if (list->frame->checkobj(obj) != 0)
        {
            //cout << "Frame manager: object known by frame " << (void*)list->frame << endl;
            break;
        }
        list = list->next;
    }
    //cout << "Frame manager: broadcast " << ((list == NULL) ? "failed" : "successful") << endl;

    // Now call the frame.
    if (list != NULL)
    {
        list->frame->process(obj, evt);
    }
    // Uncomment this if there's any doubt it's re-entrant
    //cout << "frameManager::broadcastEvent done" << endl;
}



/*
 *  Broadcast a quit-message to all frames but the first one (the main frame). There
 *  are currently two levels, 0 means the frames can yet refuse to die by returning a
 *  value != 0, every other level doesn't give them this option. Returns 0 for failure.
 *  This must only be called for the _global_ frame manager!
 */

int rviewFrameMgr::broadcastQuit(int level)
{
    int i, status;
    frame_list *list;

    list = frameList->next;
    status = 1;
    for (i=1; i<listLength; i++)
    {
        //if (level > 0) cout << "delete " << (void*)list << endl;
        if (list->frame->requestQuit(level) != 0) status = 0;
        list = list->next;
    }
    if (level == 0) return status;

    //cout << "Hard quit OK" << endl;
    return 1;
}



/*
 *  Broadcast a user event to all frames.
 */

int rviewFrameMgr::broadcastUserEvent(const user_event &ue)
{
    int i, status;
    frame_list *list;

    list = frameList;
    status = 0;
    for (i=0; i<listLength; i++)
    {
        //cout << "broadcast to " << (void*)(list->frame) << endl;
        status += list->frame->userEvent(ue);
        list = list->next;
    }
    return status;    // number of clients that recognised the event.
}





/*
 *  rviewMultiline member functions
 */

const int rviewMultiline::multiline_ppc10 = 63;

void rviewMultiline::setupVariables(wxPanel *Panel, int X, int Y, int H, int Lines)
{
    int i;

    parent = Panel;
    lines = Lines;
    if ((msg = new wxMessage *[lines]) == NULL)
    {
        cerr << "rviewMultiline::setupVariables(): " << lman->lookup("errorMemory") << endl;
        return;
    }
    for (i=0; i<lines; i++) msg[i] = NULL;

    x = X;
    y = Y;
    lHeight = H;
}


rviewMultiline::rviewMultiline(wxPanel *Panel, int X, int Y, int H, int Lines)
{
    setupVariables(Panel, X, Y, H, Lines);
}


rviewMultiline::rviewMultiline(wxPanel *Panel, const char *Message, int X, int Y, int W, int H, int Lines)
{
    setupVariables(Panel, X, Y, H, Lines);

    rebuild(Message, W);
}


rviewMultiline::~rviewMultiline(void)
{
    // The wxMessage items have been created as children of the panel and will
    // be deleted by the panel later on. So just delete the array.
    delete [] msg;
}


void rviewMultiline::rebuild(const char *Message, int W)
{
    int length, i, cpl, pos;
    char *buffer, *b, *base;

    if (msg == NULL) return;

    //cout << "rviewMultiline::rebuild()" << endl;

    cpl = (10*W) / multiline_ppc10;

    if ((buffer = new char[cpl + 1]) == NULL)
    {
        cerr << "rviewMultiline::rebuild(): " << lman->lookup("errorMemory") << endl;
        return;
    }

    length = strlen(Message);
    base = (char*)Message;

    for (i=0, pos=y; (length > 0) && (i<lines); i++, pos+=lHeight)
    {
        if (length > cpl)
        {
            b = base + cpl;
            while ((*b > 32) && (b > base)) b--;
            if (b <= base) b = base + cpl;
            else b++;
        }
        else
        {
            b = base + length;
        }
        memcpy(buffer, base, (int)(b - base));
        buffer[(int)(b - base)] = '\0';
        //cout << buffer << endl;
        if (msg[i] == NULL)
        {
            msg[i] = new wxMessage(parent, buffer, x, pos, W, lHeight, 0, "message");
        }
        else
        {
            msg[i]->SetSize(x, pos, W, lHeight, 0);
            msg[i]->SetLabel(buffer);
        }
        length -= (int)(b - base);
        base = b;
    }
    while (i<lines)
    {
        if (msg[i] != NULL)
        {
            delete msg[i];
            msg[i] = NULL;
        }
        i++;
    }
    delete [] buffer;
}



int rviewMultiline::getMessageHeight(void) const
{
    int i;

    for (i=0; i<lines; i++) if (msg[i] == NULL) break;
    return(i*lHeight);
}



/*
 *  More convenient interface to standard widgets
 */

rviewText::rviewText(wxPanel *parent, const char *value, char *label, int x, int y, int w, int h) : wxText(parent, (wxFunction)rviewEventHandler, label, (char*)value, x, y, w, h, wxTE_PROCESS_ENTER)
{
}

rviewText::rviewText(long style, wxPanel *parent,  const char *value, char *label, int x, int y, int w, int h) : wxText(parent, (wxFunction)rviewEventHandler, label, (char*)value, x, y, w, h, style)
{
}

rviewText::rviewText(wxPanel *parent, const DynamicString &value, char *label, int x, int y, int w, int h) : wxText(parent, (wxFunction)rviewEventHandler, label, NULL, x, y, w, h, wxTE_PROCESS_ENTER)
{
    wxText::SetValue((char*)value.ptr());
}

rviewText::rviewText(wxPanel *parent, int value, char *label, int x, int y, int w, int h) : wxText(parent, (wxFunction)rviewEventHandler, label, NULL, x, y, w, h, wxTE_PROCESS_ENTER)
{
    char buffer[32];
    sprintf(buffer, "%d", value);
    wxText::SetValue(buffer);
}


rviewText::rviewText(wxPanel *parent, long value, char *label, int x, int y, int w, int h) : wxText(parent, (wxFunction)rviewEventHandler, label, NULL, x, y, w, h, wxTE_PROCESS_ENTER)
{
    char buffer[32];
    sprintf(buffer, "%ld", value);
    wxText::SetValue(buffer);
}


rviewText::rviewText(wxPanel *parent, double value, bool sciForm, char *label, int x, int y, int w, int h) : wxText(parent, (wxFunction)rviewEventHandler, label, NULL, x, y, w, h, wxTE_PROCESS_ENTER)
{
    char buffer[32];
    sprintf(buffer, (sciForm) ? "%g" : "%f", value);
    wxText::SetValue(buffer);
}

void rviewText::SetValue(char *value)
{
    wxText::SetValue(value);
}

void rviewText::SetValue(const char *value)
{
    wxText::SetValue((char*)value);
}

void rviewText::SetValue(const DynamicString &value)
{
    wxText::SetValue((char*)value.ptr());
}

void rviewText::SetValue(int value)
{
    char buffer[32];
    sprintf(buffer, "%d", value);
    wxText::SetValue(buffer);
}

void rviewText::SetValue(unsigned int value)
{
    char buffer[32];
    sprintf(buffer, "%u", value);
    wxText::SetValue(buffer);
}

void rviewText::SetValue(long value)
{
    char buffer[32];
    sprintf(buffer, "%ld", value);
    wxText::SetValue(buffer);
}

void rviewText::SetValue(double value, bool sciForm)
{
    char buffer[32];
    sprintf(buffer, (sciForm) ? "%g" : "%f", value);
    wxText::SetValue(buffer);
}

char *rviewText::GetValue(void)
{
    return wxText::GetValue();
}

void rviewText::GetValue(DynamicString &value)
{
    value = wxText::GetValue();
}

void rviewText::GetValue(int &value)
{
    value = asctoi(wxText::GetValue());
}

void rviewText::GetValue(long &value)
{
    value = asctol(wxText::GetValue());
}

void rviewText::GetValue(float &value)
{
    value = asctof(wxText::GetValue());
}

void rviewText::GetValue(double &value)
{
    value = asctof(wxText::GetValue());
}



rviewButton::rviewButton(wxPanel *parent, char *label, int x, int y, int w, int h, long style) : wxButton(parent, (wxFunction)rviewEventHandler, label, x, y, w, h, style)
{
}

rviewChoice::rviewChoice(wxPanel *parent, int n, char *choices[], char *label, int x, int y, int w, int h, long style) : wxChoice(parent, (wxFunction)rviewEventHandler, label, x, y, w, h, n, choices, style)
{
}

// hacked version to make up for wxWindows' lack of const
rviewChoice::rviewChoice(wxPanel *parent, int n, const char *choices[], char *label, int x, int y, int w, int h, long style) : wxChoice(parent, (wxFunction)rviewEventHandler, label, x, y, w, h, n, (char**)choices, style)
{
}

rviewCheckBox::rviewCheckBox(wxPanel *parent, char *label, int x, int y, int w, int h) : wxCheckBox(parent, (wxFunction)rviewEventHandler, label, x, y, w, h)
{
}

rviewRadioButton::rviewRadioButton(wxPanel *parent, char *label, bool value, int x, int y, int w, int h) : wxRadioButton(parent, (wxFunction)rviewEventHandler, label, value, x, y, w, h)
{
}

rviewScrollBar::rviewScrollBar(wxPanel *parent, int x, int y, int w, int h, long style) : wxScrollBar(parent, (wxFunction)rviewEventHandler, x, y, w, h, style)
{
}

rviewSlider::rviewSlider(wxPanel *parent, int value, int min_val, int max_val, int width, char *label, int x, int y, long style) : wxSlider(parent, (wxFunction)rviewEventHandler, label, value, min_val, max_val, x, y, style)
{
}



/*
 *  The special slider class when arbitrary mouse events are required.
 */

const int rviewSpecialSlider::dflt_barwidth = 30;
const int rviewSpecialSlider::dflt_barheight = 10;
const int rviewSpecialSlider::dflt_width = 200;
const int rviewSpecialSlider::dflt_height = rviewSpecialSlider::dflt_barheight + 2*rviewSpecialSlider::dflt_border;
const int rviewSpecialSlider::dflt_border = 4;

rviewSpecialSlider::rviewSpecialSlider(rviewFrame *logParent, wxPanel *parent, int val, int min, int max, int width, const char *label) :
    wxCanvas(parent, -1, -1, (width==-1) ? dflt_width : width, dflt_height, wxRETAINED),
    background(0x00,0x00,0x00),
    foreground(0x00,0xff,0x00),
    wellground(0xff,0xff,0xff),
    outline(0x00,0x00,0x00),
    labelColour(*(parent->GetLabelColour())),
    bback(background, wxTRANSPARENT),
    bfore(foreground, wxSOLID),
    bwell(wellground, wxSOLID),
    outlinePen(outline, 1, wxSOLID),
    labelFont(12, wxROMAN, wxNORMAL, wxNORMAL),
    myLabel(label)
{
    wxBrush *dcb;
    logicalParent = logParent;
    border = dflt_border;
    barwidth = dflt_barwidth;
    barheight = dflt_barheight;
    value = val;
    vmin = min;
    vmax = max;
    if (vmax <= vmin)
        vmax = vmin+1;
    dcb = ((wxPanel*)GetParent())->GetDC()->GetBackground();
    bback = wxBrush(dcb->GetColour(), dcb->GetStyle());
    SetBackground(&bback);
    SetFont(&labelFont);
    GetDC()->GetTextExtent(myLabel.ptr(), &textx, &texty);
}

rviewSpecialSlider::~rviewSpecialSlider(void)
{
}

int rviewSpecialSlider::GetMin(void) const
{
    return vmin;
}

int rviewSpecialSlider::GetMax(void) const
{
    return vmax;
}

int rviewSpecialSlider::GetValue(void) const
{
    return value;
}

void rviewSpecialSlider::SetRange(int min, int max)
{
    if (min < max)
    {
        vmin = min;
        vmax = max;
        if (value < vmin)
            value = vmin;
        if (value > vmax)
            value = vmax;
        Refresh(TRUE);
    }
}

void rviewSpecialSlider::SetValue(int val)
{
    if ((vmin <= val) && (val <= vmax))
    {
        float barx, bary, bheight, newbx;

        getBarParams(barx, bary, bheight);
        value = val;
        getBarParams(newbx, bary, bheight);
        updateWell(barx, newbx, bary, bheight);
    }
}

void rviewSpecialSlider::SetLabel(const char *label)
{
    myLabel = label;
    GetDC()->GetTextExtent(myLabel.ptr(), &textx, &texty);
    Refresh(TRUE);
}

bool rviewSpecialSlider::PositionInWell(float posx, float posy)
{
    int y0, y1;

    GetClientSize(&cwidth, &cheight);
    getWellVert(y0, y1);
    if (((float)y0 <= posy) && (posy <= (float)y1))
    {
        if ((posx >= textx + 2*border) && (posx <= (float(cwidth - border))))
            return TRUE;
    }
    return FALSE;
}


void rviewSpecialSlider::getWellVert(int &y0, int &y1)
{
    y0 = cheight - 2*border - barheight;
    if (y0 < 0)
        y0 = 0;
    y0 += border;
    y1 = cheight - border;
    if (y1 < y0)
        y1 = y0;
}

void rviewSpecialSlider::getBarParams(float &posx, float &posy, float &height)
{
    int y0, y1;

    GetClientSize(&cwidth, &cheight);

    posx = (float)((cwidth - 3*border - barwidth - textx) * (value-vmin)) / (vmax - vmin);
    if (posx < 0)
        posx = 0;
    /*cout << "cwidth " << cwidth << ", textx " << textx << ", value " << value << ", border " << border
         << ", barwidth " << barwidth << ", vmin " << vmin << ", vmax " << vmax << ", posx " << posx << endl;*/
    posx += (float)(2*border + textx);
    getWellVert(y0, y1);
    posy = (float)y0;
    height = (float)(y1 - y0);
}

int rviewSpecialSlider::calcNewValue(float posx, float posy, int &val, bool checky)
{
    int y0, y1;

    GetClientSize(&cwidth, &cheight);

    getWellVert(y0, y1);
    if (!checky || ((float)y0 <= posy) && (posy <= (float)y1))
    {
        float rem = (float)(cwidth - 3*border - barwidth - textx);
        if (rem < 1.0)
            rem = 1.0;
        val = (int)((((posx - 2*border - textx) * (vmax - vmin)) / rem) + 0.5) + vmin;
        if (val < vmin)
            val = vmin;
        if (val > vmax)
            val = vmax;

        return 0;
    }
    return -1;
}


void rviewSpecialSlider::getUpdateInterval(float oldx, float newx, float &clipx, float &clipw)
{
    if (oldx < newx)
    {
        clipx = oldx;
        clipw = newx - oldx + barwidth;
    }
    else
    {
        clipx = newx;
        clipw = oldx - newx + barwidth;
    }
}


void rviewSpecialSlider::updateWell(float oldx, float newx, float posy, float bheight)
{
    float clipx, clipw;

    getUpdateInterval(oldx, newx, clipx, clipw);
    //cout << "CLIP " << clipx << ", " << posy << ", " << clipw << ", " << bheight << endl;
    // need to enlarge the clipping region somewhat to make sure everything is redrawn
    BeginDrawing();
    SetClippingRegion(clipx, 0, clipw+2, cheight);
    //Clear();
    redrawCore(newx, posy, bheight);
    DestroyClippingRegion();
    EndDrawing();
}


void rviewSpecialSlider::redrawCore(float x, float y, float bheight)
{
    float extx, exty, nx, ny;
    char number[32];

    // first draw the label
    SetFont(&labelFont);
    SetTextForeground(&labelColour);
    DrawText(myLabel.ptr(), 0, y);

    SetPen(&outlinePen);
    SetBrush(&bwell);
    DrawRectangle(textx + border, y-border, (float)cwidth - textx - border, bheight + 2*border);
    SetBrush(&bfore);
    DrawRectangle(x, y, (float)barwidth, bheight);

    // then draw the current value on top
    sprintf(number, "%d", value);
    GetDC()->GetTextExtent(number, &extx, &exty);
    nx = x + (barwidth - extx)/2;
    ny = y - exty - border;
    SetPen(NULL);
    // must use a non-transparent brush. Finally found the right one!
    SetBrush(((wxPanel*)GetParent())->GetDC()->GetBackground());
    DrawRectangle(0, ny, (float)cwidth, exty);
    DrawText(number, nx, ny);
}


void rviewSpecialSlider::OnPaint(void)
{
    wxRect rect;
    float x, y, bheight;

    getBarParams(x, y, bheight);

    BeginDrawing();

    wxUpdateIterator upd(this);
    while (upd)
    {
        upd.GetRect(&rect);
        redrawCore(x, y, bheight);
        upd++;
    }
    EndDrawing();
}

void rviewSpecialSlider::OnEvent(wxMouseEvent &mevt)
{
    int type = mevt.GetEventType();
    float barx, bary, bheight;
    float mx, my;
    int newVal = value;

    getBarParams(barx, bary, bheight);
    mevt.Position(&mx, &my);
    if ((type == wxEVENT_TYPE_LEFT_DOWN) || (type == wxEVENT_TYPE_RIGHT_DOWN))
    {
        if (mx < barx)
        {
            if (calcNewValue(barx - barwidth, my, newVal, TRUE) != 0)
                newVal = value;
        }
        else if (mx > barx + barwidth)
        {
            if (calcNewValue(barx + barwidth, my, newVal, TRUE) != 0)
                newVal = value;
        }
    }
    else if (type == wxEVENT_TYPE_MOTION)
    {
        if (mevt.LeftIsDown() || mevt.RightIsDown())
        {
            if (calcNewValue(mx - barwidth/2, my, newVal, FALSE) != 0)
            {
                newVal = value;
            }
        }
    }
    if (newVal != value)
    {
        float newbx, newby, newbh;

        value = newVal;
        getBarParams(newbx, newby, newbh);
        updateWell(barx, newbx, bary, bheight);

        // additionally we build a regular command event
        wxCommandEvent cmd(wxEVENT_TYPE_SLIDER_COMMAND);
        //OnCommand(*GetParent(), cmd);
        logicalParent->process(*this, cmd);
    }
    // the entire point of this class is to always pass on the actual mouse event to the parent
    logicalParent->childMouseEvent(this, mevt);
}






/*
 *  rviewDialog member functions.
 */

const int rviewDialog::dialog_width = 300;
const int rviewDialog::dialog_height = 170;
const int rviewDialog::dialog_border = 8;
const int rviewDialog::dialog_buttonsx = 80;
const int rviewDialog::dialog_buttonsy = 30;
const int rviewDialog::dialog_bheight = rviewDialog::dialog_buttonsy + 16;
const int rviewDialog::dialog_lines = 8;
const int rviewDialog::dialog_lheight = 20;


// The strings passed to this function may be labels (first char is a backslash)
// or explicit text.
rviewDialog::rviewDialog(const char *title, const char *message, int buttonNo, const char *buttons[]):
    rviewFrame(NULL, "", 0, 0, dialog_width, dialog_height)
{
    int i, x, y;
    char *string;

    panel = NULL;
    buttonNumber = 0;
    buttonPressed = 0;

    buttonText = new char *[buttonNo + 2];
    but = new rviewButton *[buttonNo];

    if ((buttonText == NULL) || (but == NULL))
    {
        cerr << "rviewDialog::rviewDialog(): " << lman->lookup("errorMemory") << endl;
        if (buttonText != NULL) delete [] buttonText;
        if (but != NULL) delete [] but;
        return;
    }

    buttonNumber = buttonNo;

    buttonText[0] = (char*)title;
    buttonText[1] = (char*)message;
    for (i=0; i<buttonNumber; i++)
    {
        but[i] = NULL;
        buttonText[i+2] = (char*)buttons[i];
    }

    // Only memorize those strings that are a label identifier (first char a backslash)
    for (i=0; i<2+buttonNumber; i++)
    {
        if (buttonText[i] != NULL)
        {
            // Don't combine the two ifs with && : there is no defined evaluation order for && !
            // Always memorize the message text.
            if ((buttonText[i][0] == '\\') || (i == 1))
            {
                // We remove the backslash but need one additional char for the
                // line terminator ==> strlen -1 + 1.
                if ((string = new char[strlen(buttonText[i]) + ((i==1) ? 1 : 0)]) == NULL)
                {
                    cerr << "rviewDialog::rviewDialog(): " << lman->lookup("errorMemory") << endl;
                    while (--i >= 0)
                    {
                        if (buttonText[i] != NULL) delete [] buttonText[i];
                    }
                    delete [] buttonText;
                    buttonText = NULL;
                    delete [] but;
                    but = NULL;
                    buttonNumber = 0;
                    return;
                }
                strcpy(string, buttonText[i] + ((i==1) ? 0 : 1));
                buttonText[i] = string;
                //cout << "buttonText[" << i << "] = " << buttonText[i] << endl;
            }
            else buttonText[i] = NULL;
        }
    }

    GetClientSize(&x, &y);

    panel = new wxPanel((wxWindow*)this, 100, 100, 10, 10);

    msg = new rviewMultiline(panel, dialog_border, dialog_border, dialog_lheight, dialog_lines);

    //cout << x << ", " << y << endl;

    // If the strings are not labels then set them now statically, otherwise they will
    // be set in the following label() call.
    if (title[0] != '\\')
    {
        SetTitle((char*)title);
    }

    // Now handle all buttons.
    for (i=0; i<buttonNumber; i++)
    {
        but[i] = new rviewButton(panel);
        if (buttons[i][0] != '\\')
        {
            but[i]->SetLabel((char*)buttons[i]);
        }
        else    // do this for the button size.
        {
            but[i]->SetLabel(lman->lookup(buttons[i]+1));
        }
    }

    OnSize(x, y);

    label();

    Show(TRUE);
}



rviewDialog::~rviewDialog(void)
{
    int i;

    // Widgets that were created as children of other wx items are deleted by those items.
    // They mustn't be destroyed here!

    if (msg != NULL) delete msg;
    //if (panel != NULL) delete panel;

    if (but != NULL)
    {
        //for (i=0; i<buttonNumber; i++) {if (but[i] != NULL) delete but[i];}
        delete [] but;
    }

    if (buttonText != NULL)
    {
        for (i=0; i<2+buttonNumber; i++)
        {
            if (buttonText[i] != NULL) delete [] buttonText[i];
        }
        delete [] buttonText;
    }
}


const char *rviewDialog::getFrameName(void) const
{
    return "rviewDialog";
}

rviewFrameType rviewDialog::getFrameType(void) const
{
    return rviewFrameTypeDialog;
}


void rviewDialog::OnSize(int w, int h)
{
    int x, y, empty, pos, i;

    RMDBGONCE( 3, RMDebug::module_applications, "rviewDialog", "OnSize( " << x << ", " << h << " )" );

    GetClientSize(&x, &y);
    if ((frameWidth == x) && (frameHeight == y)) return;
    frameWidth = x;
    frameHeight = y;

    x -= 2*dialog_border;
    y -= dialog_border;
    panel->SetSize(0, 0, x, y);

    if (buttonText[1][0] == '\\')
        msg->rebuild(lman->lookup(buttonText[1]+1), x);
    else
        msg->rebuild(buttonText[1], x);

    // Space out the buttons on the available space.
    if (buttonNumber == 0) return;

    empty = (x - buttonNumber*dialog_buttonsx) / buttonNumber;
    if(empty <0 )
        empty=0;

    pos = empty/2;

    for (i=0; i<buttonNumber; i++)
    {
        but[i]->SetSize(pos, y - (dialog_bheight + dialog_buttonsy)/2, dialog_buttonsx, dialog_buttonsy);
        pos += empty + dialog_buttonsx;
    }
}



void rviewDialog::label(void)
{
    int i, x, y;

    //cout << "rviewDialog::label()" << endl;
    // If the widgets are labels then set their new values.

    panel->GetClientSize(&x, &y);
    if (buttonText[0] != NULL)
    {
        SetTitle(lman->lookup(buttonText[0]));
    }

    // The message is always stored.
    if (buttonText[1][0] == '\\')
        msg->rebuild(lman->lookup(buttonText[1]+1), x);
    else
        msg->rebuild(buttonText[1], x);

    for (i=0; i<buttonNumber; i++)
    {
        if (buttonText[2+i] != NULL)
        {
            but[i]->SetLabel(lman->lookup(buttonText[2+i]));
        }
    }
}




/*
 *  Process events which are broadcast by the frame manager.
 */

int rviewDialog::process(wxObject &obj, wxEvent &evt)
{
    int i, type;

    //cout << "rviewDialog::process()" << endl;
    for (i=0; i<buttonNumber; i++)
    {
        if (&obj == (wxObject*)but[i]) break;
    }

    if (i >= buttonNumber) return 0;

    type = evt.GetEventType();

    if (type == wxEVENT_TYPE_BUTTON_COMMAND)
    {
        buttonPressed = i+1;
    }
    else
    {
        buttonPressed = 0;
    }
    //cout << "rviewDialog::process():" << (void*)this << " Button " << buttonPressed << endl;

    return buttonPressed;
}



/*
 *  Polling interface to the dialog box. Returns the number of the button that
 *  was pressed, starting from 1 (0 means none). On reading the button number
 *  is reset to 0.
 */

int rviewDialog::GetButton(void)
{
    int val = buttonPressed;

    buttonPressed = 0;
    return val;
}





/*
 *  Error box
 *  (an intrinsically modal special case of a dialog box)
 */

char *rviewErrorboxDefaultButton[1] = {"\\textOK"};

rviewErrorbox::rviewErrorbox(const char *message): rviewDialog("\\errorFrom", message, 1, (const char**)rviewErrorboxDefaultButton)
{
    //place this in log file
    cerr << lman->lookup("errorFrom") << ' ' << message << endl;
}


rviewErrorbox::rviewErrorbox(const char *title, const char *message, int buttonNo, const char *buttons[]): rviewDialog(title, message, buttonNo, buttons)
{
    //place this in log file
    cerr << lman->lookup("errorFrom") << ' ' << message << endl;
}


rviewErrorbox::~rviewErrorbox(void)
{
    ;
}


const char *rviewErrorbox::getFrameName(void) const
{
    return "rviewErrorbox";
}

rviewFrameType rviewErrorbox::getFrameType(void) const
{
    return rviewFrameTypeErrorbox;
}


int rviewErrorbox::activate(void)
{
    int pressed;

    MakeModal(TRUE);
    while ((pressed = GetButton()) == 0) ::wxYield();
    MakeModal(FALSE);
    return pressed;
}


// static member functions
char *rviewErrorbox::buildErrorMessage(const char *message, const char *classname, const char *funcname)
{
    char *buffer, *b;
    int bsize = strlen(message) + 1;

    if (classname != NULL)
    {
        bsize += strlen(classname) + 3; // ": "

        // funcname is ignored unless class name is given
        if (funcname != NULL)
            bsize += strlen(funcname) + 3;    // "::"
    }

    buffer = new char[bsize];
    b = buffer;

    if (classname != NULL)
    {
        b += sprintf(b, "%s", classname);

        if (funcname != NULL)
            b += sprintf(b, "::%s", funcname);

        b += sprintf(b, ": ");
    }
    b += sprintf(b, "%s", message);

    return buffer;
}


rviewErrorbox *rviewErrorbox::newErrorbox(const char *message, const char *classname, const char *funcname)
{
    char *msg;
    rviewErrorbox *box;

    msg = buildErrorMessage(message, classname, funcname);
    box = new rviewErrorbox(msg);
    delete [] msg;

    return box;
}


rviewErrorbox *rviewErrorbox::newErrorbox(const char *title, const char *message, int buttonNo, const char *buttons[], const char *classname, const char *funcname)
{
    char *msg;
    rviewErrorbox *box;

    msg = buildErrorMessage(message, classname, funcname);
    box = new rviewErrorbox(msg);
    delete [] msg;

    return box;
}


int rviewErrorbox::reportError(const char *message, const char *classname, const char *funcname)
{
    rviewErrorbox *box;
    int but;

    box = newErrorbox(message, classname, funcname);
    but = box->activate();
    box->Close(TRUE);

    return but;
}


int rviewErrorbox::reportError(const char *title, const char *message, int buttonNo, const char *buttons[], const char *classname, const char *funcname)
{
    rviewErrorbox *box;
    int but;

    box = newErrorbox(title, message, buttonNo, buttons, classname, funcname);
    but = box->activate();
    box->Close(TRUE);

    return but;
}





/*
 *  rviewProgress members
 */

rviewProgress::rviewProgress(const char *message): rviewDialog("\\messageFrom", message, 1, (const char**)rviewErrorboxDefaultButton)
{
    int x, y;

    RMDBGONCE( 3, RMDebug::module_applications, "rviewProgress", "rviewProgress( " << lman->lookup("messageFrom") << message << " )" );

    GetSize(&x, &y);
    y = 2*dialog_border + msg->getMessageHeight() + dialog_bheight;
    SetSize(-1, -1, x, y);

    // Have to do this to make sure you actually see anything in the window
    // Crude, but nothing else seems to work...
    Refresh(TRUE);
    ::wxYield();
    // Wait 300ms
    ::wxStartTimer();
    while (::wxGetElapsedTime(FALSE) < 300) ::wxYield();
}




rviewProgress::~rviewProgress(void)
{
    ;
}


int rviewProgress::process(wxObject &obj, wxEvent &evt)
{
    if (rviewDialog::process(obj, evt) != 0)
    {
        this->Close(TRUE);
        return 1;
    }
    return 0;
}


const char *rviewProgress::getFrameName(void) const
{
    return "rviewProgress";
}

rviewFrameType rviewProgress::getFrameType(void) const
{
    return rviewFrameTypeProgress;
}




/*
 *  rviewResult members
 */

const int rviewResult::result_x = 0;
const int rviewResult::result_y = 0;
const int rviewResult::result_width = 320;
const int rviewResult::result_height = 296;
const int rviewResult::result_border = 8;
const int rviewResult::result_lheight = 20;
const int rviewResult::result_header = (8 * rviewResult::result_lheight);
const int rviewResult::result_cwidth = 150;
const int rviewResult::result_twidth = 90;
const int rviewResult::result_theight = 30;
const int rviewResult::result_bwidth = 60;
const int rviewResult::result_bheight = 30;

rviewResult::rviewResult(void): rviewFrame(NULL, "", result_x, result_y, result_width, result_height)
{
    RMDBGONCE( 3, RMDebug::module_applications, "rviewResult", "rviewResult() " << this);

    setupVariables();
    configureGrey();
    Show(FALSE);
}

rviewResult::rviewResult(collection_desc *collection): rviewFrame(NULL, "", result_x, result_y, result_width, result_height)
{
    setupVariables();
    setCollection(collection);
}


rviewResult::~rviewResult(void)
{
    RMDBGONCE( 3, RMDebug::module_applications, "rviewResult", "rviewResult() " << this );

    int i;
    collection_desc *ptr = coll;
    user_event ue;

    // give all viewers a chance to clean up
    ue.type = usr_mdd_dying;
    for (i=0; i<coll->number; i++)
    {
        ue.data = (void*)&(coll->mddObjs[i].mdd);
        frames->broadcastUserEvent(ue);
    }

    // All the widgets are deleted automatically.
    if (selectedItems != NULL) delete [] selectedItems;
    if (typeManager != NULL)
    {
        typeManager->unlinkParent();
        typeManager->Close(TRUE);
    }

    rviewDeleteCollection(coll);
}


const char *rviewResult::getFrameName(void) const
{
    return "rviewResult";
}

rviewFrameType rviewResult::getFrameType(void) const
{
    return rviewFrameTypeResult;
}


void rviewResult::setCollection(collection_desc *collection)
{
    int x, y, i;
    char res_string[STRINGSIZE];

    // In case this is called when the frame already displays a result (shouldn't
    // normally happen, though)
    if (coll != NULL)
    {
        rviewDeleteCollection(coll);
        delete list;
    }
    coll = collection;

    // Init flags
    for (i=0; i<coll->number; i++)
    {
        coll->mddObjs[i].flags = 0;
    }

    GetClientSize(&x, &y);

    list = new wxListBox(panel, (wxFunction)&rviewEventHandler, "", wxMULTIPLE, result_border, result_border + result_header, x, y - result_header, 0, NULL, wxALWAYS_SB | wxLB_MULTIPLE);

    ostrstream memstr(res_string, STRINGSIZE);
    for (i=0; i<coll->number; i++)
    {
        list->Append(mddDescriptorString(memstr, i));
    }

    if (selectedItems != NULL) delete [] selectedItems;
    i = (coll->number + 7) >> 3;
    if ((selectedItems = new char[i]) == NULL)
    {
        cerr << "rviewResult::setCollection(): " << lman->lookup("errorMemory") << endl;
        this->Close(TRUE);
        return;
    }
    memset(selectedItems, 0, i);

    configureGrey();

    label();

    // Force resize
    frameWidth = -1;
    frameHeight = -1;
    OnSize(x, y);

    Show(TRUE);
}



char *rviewResult::mddDescriptorString(ostrstream &memstr, int number)
{
    r_GMarray *mdd = coll->mddObjs[number].mdd.ptr();
    r_Data_Format fmt = mdd->get_current_format();

    memstr.width(3);
    memstr << number << ": (";
    memstr.width(0);
    memstr << mdd->get_type_length() << ") "
           << mdd->spatial_domain() << ' '
           << fmt << '\0';
    memstr.seekp(0);

    return memstr.str();
}


void rviewResult::label(void)
{
    char buffer[STRINGSIZE];

    //cout << "labelling " << (void*)mBar << endl;

    SetTitle(lman->lookup("titleResult"));

    if (mBar != NULL)
    {
        // Configure all the text in the menu bar
        mBar->SetLabel(MENU_RSLT_ITEM_OPENALL, lman->lookup("menRsltItemOpAll"));
        mBar->SetLabel(MENU_RSLT_ITEM_THUMBALL, lman->lookup("menRsltItemThumbAll"));
        mBar->SetLabel(MENU_RSLT_ITEM_CLOSE, lman->lookup("menRsltItemClose"));
        mBar->SetHelpString(MENU_RSLT_ITEM_OPENALL, lman->lookup("helpRsltItemOpAll"));
        mBar->SetHelpString(MENU_RSLT_ITEM_THUMBALL, lman->lookup("helpRsltItemThumbAll"));
        mBar->SetHelpString(MENU_RSLT_ITEM_CLOSE, lman->lookup("helpRsltItemClose"));

        mBar->SetLabel(MENU_RSLT_SLCT_SLCTALL, lman->lookup("menRsltSlctSlctAll"));
        mBar->SetLabel(MENU_RSLT_SLCT_CLEAR, lman->lookup("menRsltSlctClear"));
        mBar->SetLabel(MENU_RSLT_SLCT_OPEN, lman->lookup("menRsltSlctOpen"));
        mBar->SetLabel(MENU_RSLT_SLCT_THUMB, lman->lookup("menRsltSlctThumb"));
        mBar->SetLabel(MENU_RSLT_SLCT_DELETE, lman->lookup("menRsltSlctDel"));
        mBar->SetLabel(MENU_RSLT_SLCT_ENDIAN, lman->lookup("menRsltSlctEndian"));
        mBar->SetLabel(MENU_RSLT_SLCT_TYPEMAN, lman->lookup("menRsltSlctTypeMan"));
        mBar->SetLabel(MENU_RSLT_SLCT_INFO, lman->lookup("menRsltSlctInfo"));
        mBar->SetHelpString(MENU_RSLT_SLCT_SLCTALL, lman->lookup("helpRsltSlctSlctAll"));
        mBar->SetHelpString(MENU_RSLT_SLCT_CLEAR, lman->lookup("helpRsltSlctClear"));
        mBar->SetHelpString(MENU_RSLT_SLCT_OPEN, lman->lookup("helpRsltSlctOpen"));
        mBar->SetHelpString(MENU_RSLT_SLCT_THUMB, lman->lookup("helpRsltSlctThumb"));
        mBar->SetHelpString(MENU_RSLT_SLCT_DELETE, lman->lookup("helpRsltSlctDel"));
        mBar->SetHelpString(MENU_RSLT_SLCT_ENDIAN, lman->lookup("helpRsltSlctEndian"));
        mBar->SetHelpString(MENU_RSLT_SLCT_TYPEMAN, lman->lookup("helpRsltSlctTypeMan"));
        mBar->SetHelpString(MENU_RSLT_SLCT_INFO, lman->lookup("helpRsltSlctInfo"));

        mBar->SetLabelTop(0, lman->lookup("menRsltItem"));
        mBar->SetLabelTop(1, lman->lookup("menRsltSlct"));
    }

    if (list != NULL)
    {
        list->SetLabel(lman->lookup("textResult"));
    }

    if (group != NULL)
    {
        group->SetLabel(lman->lookup("textCollHeader"));
        sprintf(buffer, "%s: %s", lman->lookup("textCollName"), (coll->collName == NULL) ? "" : coll->collName);
        collName->SetLabel(buffer);
        sprintf(buffer, "%s: %s", lman->lookup("textCollType"), (coll->collType == NULL) ? "?" : coll->collType);
        collType->SetLabel(buffer);
        sprintf(buffer, (coll->collInfo == NULL) ? "" : coll->collInfo);
        collInfo->SetLabel(buffer);

        if (choice != NULL)
        {
            choice->SetLabel(lman->lookup("dispModeLabel"));
        }
    }
    resampBut->SetLabel(lman->lookup("textOK"));

    //cout << "labelled OK" << endl;
}



void rviewResult::OnSize(int w, int h)
{
    if (panel != NULL)
    {
        int x, y;

        RMDBGONCE( 3, RMDebug::module_applications, "rviewResult", "OnSize( " << w << ", " << h << " )");

        GetClientSize(&x, &y);
        if ((frameWidth == x) && (frameHeight == y)) return;
        frameWidth = x;
        frameHeight = y;

        panel->SetClientSize(x, y);
        x -= 2*result_border;
        y -= 2*result_border;

        if (list != NULL)
            list->SetSize(result_border,
                          result_lheight + result_border + result_header,
                          x,
                          y - result_header - result_lheight);

        if (group != NULL)
        {
            group->SetSize(result_border,
                           result_border,
                           x,
                           result_header);
            //group->GetClientSize(&x, &y);
            x -= 2*result_border;
            y -=2*result_border;
            if (collName != NULL)
                collName->SetSize(2*result_border,
                                  result_lheight + result_border,
                                  x,
                                  result_lheight);
            if (collType != NULL)
                collType->SetSize(2*result_border,
                                  2*result_lheight + result_border,
                                  x,
                                  result_lheight);
            if (collInfo != NULL)
                collInfo->SetSize(2*result_border,
                                  3*result_lheight + result_border,
                                  x,
                                  result_lheight);
            if (choice != NULL)
                choice->SetSize(2*result_border,
                                4*result_lheight + result_border,
                                result_cwidth - cbfactor,
                                result_lheight);
        }
        scaleMode->SetSize(3*result_border/2,
                           6*result_lheight + result_border,
                           result_cwidth - 2*cbfactor,
                           result_lheight);
        resampText->SetSize(3*result_border/2 + result_cwidth - cbfactor,
                            6*result_lheight + result_border,
                            result_twidth,
                            result_theight);
        resampBut->SetSize(3*result_border/2 + result_cwidth - cbfactor/2 + result_twidth,
                           6*result_lheight + result_border,
                           result_bwidth,
                           result_bheight);
    }
}



void rviewResult::openViewer(int itemNo)
{

    RMDBGONCE( 3, RMDebug::module_applications, "rviewResult", "openViewer( " << itemNo << " )" );

    if ((itemNo < 0) || (itemNo >= coll->number)) return;

    rviewDisplay *newDisplay=NULL;

    switch (prefs->lastDisplay)
    {
    case 0:
    {
        if ((coll->mddObjs[itemNo].flags & RVIEW_RESDISP_IMGFLAT) == 0)
        {
            newDisplay = new rviewFlatImage(coll->mddObjs + itemNo);
        }
    }
    break;
    case 1:
    {
        if ((coll->mddObjs[itemNo].flags & RVIEW_RESDISP_IMGVOLM) == 0)
        {
            newDisplay = new rviewVolumeImage(coll->mddObjs + itemNo);
        }
    }
    break;
    case 2:
    {
        if ((coll->mddObjs[itemNo].flags & RVIEW_RESDISP_IMGOSECT) == 0)
        {
            newDisplay = new rviewOSectionFullImage(coll->mddObjs + itemNo);
        }
    }
    break;
    case 3:
    {
        if ((coll->mddObjs[itemNo].flags & RVIEW_RESDISP_IMGHGHT) == 0)
        {
            newDisplay = new rviewHeightImage(coll->mddObjs + itemNo);
        }
    }
    break;
    case 4:
    {
        if ((coll->mddObjs[itemNo].flags & RVIEW_RESDISP_CHART) == 0)
        {
            newDisplay = new rviewChart(coll->mddObjs + itemNo);
        }
    }
    break;
    case 5:
    {
        if ((coll->mddObjs[itemNo].flags & RVIEW_RESDISP_TABLE) == 0)
        {
            newDisplay = new rviewTable(coll->mddObjs + itemNo);
        }
    }
    break;
    case 6:
    {
        if ((coll->mddObjs[itemNo].flags & RVIEW_RESDISP_SOUND) == 0)
        {
            newDisplay = new rviewSoundPlayer(coll->mddObjs + itemNo);
        }
    }
    break;
    case 7:
    {
        if ((coll->mddObjs[itemNo].flags & RVIEW_RESDISP_STRVIEW) == 0)
        {
            newDisplay = new rviewStringViewer(coll->mddObjs + itemNo);
        }
    }
    break;
    default:
        cerr << "Unsupported display mode " << prefs->lastDisplay << endl;
        return;
    }

    if (newDisplay != NULL)
    {
        if (newDisplay->openViewer() != 0)
            newDisplay->Close(TRUE);
        else
        {
            coll->mddObjs[itemNo].flags |= newDisplay->getViewerType();
            registerChild(newDisplay);
        }
    }
}



/*
 *  This function must be called whenever the selections of the list box
 *  change (click / double click) or items in the listbox are modified or
 *  deleted (this seems to be some wxWindows-internal bug that always
 *  deselects listbox items if an item was changed).
 */
int rviewResult::updateSelection(void)
{
    int i, changes, sel=-1;
    char val;

    if (selectedItems == NULL) return -1;

    changes = 0;
    for (i=0; i<coll->number; i++)
    {
        val = selectedItems[i>>3] & (1<<(i&7));
        if (list->Selected(i))
        {
            if (val == 0)
            {
                sel=i;
                changes++;
            }
            selectedItems[i>>3] |= (1<<(i&7));
        }
        else
        {
            if (val != 0)
            {
                sel=i;
                changes++;
            }
            selectedItems[i>>3] &= ~(1<<(i&7));
        }
    }
    if (changes > 1)
    {
        cerr << "Warning: more than one selection changed!" << endl;
    }

    configureGrey();

    return sel;
}



void rviewResult::operationPrologue(void)
{
    ::wxBeginBusyCursor();
    ::wxStartTimer();
}


void rviewResult::operationEpilogue(const char *opname)
{
    int elapsed = ::wxGetElapsedTime();

    ::wxEndBusyCursor();

    if (opname != NULL)
    {
        char buffer[STRINGSIZE];

        sprintf(buffer, "%s \"%s\": %dms", lman->lookup("textOpTime"), opname, elapsed);
        cout << buffer << endl;
        SetStatusText(buffer);
    }
}


int rviewResult::parseResampleString(const char *resStr, double *values)
{
    int i;
    const char *b;

    b = resStr;
    i = 0;
    while (*b != 0)
    {
        while ((*b == ' ') || (*b == '\t')) b++;
        if (*b == ',') b++;
        if (*b != 0)
        {
            if (values != NULL) values[i] = atof(b);
            while ((*b != ' ') && (*b != '\t') && (*b != ',') && (*b != 0)) b++;
            i++;
        }
    }
    return i;
}


int rviewResult::resampleSelection(void)
{
    int i, j, res;
    double *scale;
    r_Dimension dim;
    r_Minterval oldInterv, useInterv, newInterv;
    r_Ref<r_GMarray> newMdd;
    rviewBaseType baseType;
    char buffer[STRINGSIZE];
    user_event usr;
    char *valStr;
    int dimString;
    int smode;
    const char *operation=NULL;

    ostrstream memstr(buffer, STRINGSIZE);

    valStr = resampText->GetValue();
    dimString = parseResampleString(valStr, NULL);
    scale = new double[dimString];
    parseResampleString(valStr, scale);

    for (i=0; i<dimString; i++)
    {
        if (scale[i] <= 0.0) return 0;
        if (scale[i] != 1.0) break;
    }

    smode = scaleMode->GetSelection();

    operationPrologue();

    res = 0;
    for (i=0; i<coll->number; i++)
    {
        if (list->Selected(i))
        {
            double totalScale = 1.0;

            baseType = rviewGetBasetype((r_Object*)(&(*(coll->mddObjs[i].mdd))));
            oldInterv = (coll->mddObjs[i].mdd)->spatial_domain();
            dim = oldInterv.dimension();
            newInterv = r_Minterval(dim);
            useInterv = r_Minterval(dim);
            for (j=0; j<(int)dim; j++)
            {
                double h;

                // Use the entire source object for scaling
                useInterv << oldInterv[j];
                h = (j < dimString) ? scale[j] : scale[dimString-1];
                totalScale *= h;
                newInterv << r_Sinterval((r_Range)(oldInterv[j].low()), (r_Range)(oldInterv[j].low() + h*(oldInterv[j].high() - oldInterv[j].low() + 1) - 1));
            }
            if (smode == 0)   // resampling mode
            {
                if (totalScale > 1.0)
                {
                    operation = lman->lookup("operationUpsamp");
                    j = mdd_objectScaleInter(coll->mddObjs[i].mdd, useInterv, newMdd, newInterv);
                }
                else
                {
                    operation = lman->lookup("operationDownsamp");
                    j = mdd_objectScaleAverage(coll->mddObjs[i].mdd, useInterv, newMdd, newInterv);
                }
            }
            else  // simple scale mode
            {
                operation = lman->lookup("operationScale");
                j = mdd_objectScaleSimple(coll->mddObjs[i].mdd, useInterv, newMdd, newInterv);
            }
            if (j != 0)
            {
                usr.type = usr_mdd_dying;
                usr.data = (void*)(&((coll->mddObjs[i]).mdd));
                frames->broadcastUserEvent(usr);
                coll->mddObjs[i].flags = 0;
                (coll->mddObjs[i].mdd).destroy();
                coll->mddObjs[i].mdd = newMdd;
                res++;
                list->SetString(i, mddDescriptorString(memstr, i));
                selectedItems[i>>3] &= ~(1<<(i&7));
            }
        }
    }

    operationEpilogue(operation);

    delete [] scale;

    lastSelected = updateSelection();

    return res;
}


int rviewResult::process(wxObject &obj, wxEvent &evt)
{
    int type = evt.GetEventType();

    if (&obj == (wxObject*)list)
    {
        if (type == wxEVENT_TYPE_LISTBOX_COMMAND)
        {
            //cout << "rviewResult::process Listbox command" << endl;
            lastSelected = updateSelection();
            return 1;
        }
        if (type == wxEVENT_TYPE_LISTBOX_DCLICK_COMMAND)
        {
            if ((lastSelected >= 0) && (lastSelected < coll->number))
            {
                // This check is necessary
                if (!list->Selected(lastSelected))
                {
                    list->SetSelection(lastSelected, TRUE);
                    if (selectedItems != NULL) selectedItems[lastSelected>>3] |= (1<<(lastSelected&7));
                }
                openViewer(lastSelected);
            }
            return 1;
        }
    }
    if (&obj == (wxObject*)choice)
    {
        if (type == wxEVENT_TYPE_CHOICE_COMMAND)
        {
            prefs->lastDisplay = choice->GetSelection();
            prefs->markModified();
            return 1;
        }
    }
    if (&obj == (wxObject*)resampText)
    {
        if (type == wxEVENT_TYPE_TEXT_ENTER_COMMAND)
        {
            resampleSelection();
            return 1;
        }
    }
    if (&obj == (wxObject*)resampBut)
    {
        if (type == wxEVENT_TYPE_BUTTON_COMMAND)
        {
            resampleSelection();
            return 1;
        }
    }

    return 0;
}



int rviewResult::userEvent(const user_event &ue)
{
    if (ue.type == usr_viewer_closed)
    {
        mdd_frame *mf = (mdd_frame*)(ue.data);
        int i;

        //{FILE *fp=fopen("xxx", "a+"); fprintf(fp, "Received viewer closed %p --- %p, %d\n", mf->mdd.ptr(), coll->mddObjs[0].mdd.ptr(), mf->flags); fclose(fp);}
        for (i=0; i<coll->number; i++)
        {
            // must compare pointers, comparing r_Refs has strange effects on Win
            if (mf->mdd.ptr() == coll->mddObjs[i].mdd.ptr()) break;
        }
        if (i < coll->number)
        {
            coll->mddObjs[i].flags &= ~(mf->flags);
            return 1;
        }
    }
    else if (ue.type == usr_child_closed)
    {
        if (ue.data == (void*)typeManager) typeManager = NULL;
        return 1;
    }
    else if (ue.type == usr_typeman_convert)
    {
        convertSelectedItems();
        typeManager->Close(TRUE);
        typeManager = NULL;
        return 1;
    }
    else if (ue.type == usr_close_viewers)
    {
        Close(TRUE);
        return 1;
    }

    return 0;
}



void rviewResult::convertSelectedItems(void)
{
    int itemNo;
    user_event usr;
    char buffer[STRINGSIZE];
    ostrstream memstr(buffer, STRINGSIZE);

    operationPrologue();

    for (itemNo=0; itemNo < coll->number; itemNo++)
    {
        if ((list->Selected(itemNo)) && (!coll->mddObjs[itemNo].mdd.is_null()))
        {
            r_Ref<r_GMarray> newMdd;

            if (typeManager->convert(coll->mddObjs[itemNo].mdd, newMdd) == 0)
            {
                usr.type = usr_mdd_dying;
                usr.data = (void*)(&((coll->mddObjs[itemNo]).mdd));
                frames->broadcastUserEvent(usr);
                coll->mddObjs[itemNo].mdd.destroy();
                coll->mddObjs[itemNo].mdd = newMdd;
                list->SetString(itemNo, mddDescriptorString(memstr, itemNo));
            }
            else
            {
                newMdd.destroy();
            }
        }
    }

    operationEpilogue(lman->lookup("operationTypeProj"));

    lastSelected = updateSelection();
}



// Set up some variables and widgets in the frame.
void rviewResult::setupVariables(void)
{
    int x, y;
    char *displayModes[RVIEW_RESDISP_NUMBER];
    char *scaleModes[2];
    wxMenu *mbarMenus[2];
    char buffer[STRINGSIZE];

    coll = NULL;
    panel = NULL;
    list = NULL;
    collName = NULL;
    collType = NULL;
    group = NULL;
    mBar = NULL;
    typeManager = NULL;
    lastSelected = -1;
    selectedItems = NULL;

    CreateStatusLine(1);

    mbarMenus[0] = new wxMenu;
    mbarMenus[0]->Append(MENU_RSLT_ITEM_OPENALL, "");
    mbarMenus[0]->Append(MENU_RSLT_ITEM_THUMBALL, "");
    mbarMenus[0]->Append(MENU_RSLT_ITEM_CLOSE, "");

    mbarMenus[1] = new wxMenu;
    mbarMenus[1]->Append(MENU_RSLT_SLCT_SLCTALL, "");
    mbarMenus[1]->Append(MENU_RSLT_SLCT_CLEAR, "");
    mbarMenus[1]->Append(MENU_RSLT_SLCT_OPEN, "");
    mbarMenus[1]->Append(MENU_RSLT_SLCT_THUMB, "");
    mbarMenus[1]->Append(MENU_RSLT_SLCT_DELETE, "");
    mbarMenus[1]->Append(MENU_RSLT_SLCT_ENDIAN, "");
    mbarMenus[1]->Append(MENU_RSLT_SLCT_TYPEMAN, "");
    mbarMenus[1]->Append(MENU_RSLT_SLCT_INFO, "");

    mBar = new wxMenuBar;
    sprintf(buffer, "&%s", lman->lookup("menRsltItem"));
    mBar->Append(mbarMenus[0], buffer);
    sprintf(buffer, "&%s", lman->lookup("menRsltSlct"));
    mBar->Append(mbarMenus[1], buffer);

    GetClientSize(&x, &y);
    panel = new wxPanel((wxWindow*)this, 0, 0, x, y);
    x -= 2*result_border;
    y -= 2*result_border;

    // Create a group box surrounding the collection-items
    group = new wxGroupBox(panel, "", 0, 0, x, result_header, wxBORDER);
    x -= 2*result_border;
    y -= 2*result_border;
    // Create the message widget showing the collection name
    collName = new wxMessage(panel, "", 2*result_border, result_lheight + 2*result_border, x, result_lheight, 0, "message");
    // The same for the collection base type
    collType = new wxMessage(panel, "", 2*result_border, 2*result_lheight + 2*result_border, x, result_lheight, 0, "message");
    // The info string
    collInfo = new wxMessage(panel, "", 2*result_border, 3*result_lheight + 2*result_border, x, result_lheight, 0, "message");

    // And the choice item for the display modes.
    // In contrast to the menu stuff non-persistent objects are OK here.
    displayModes[0] = lman->lookup("dispModeImage");
    displayModes[1] = lman->lookup("dispModeVolume");
    displayModes[2] = lman->lookup("dispModeOrtho");
    displayModes[3] = lman->lookup("dispModeHeight");
    displayModes[4] = lman->lookup("dispModeChart");
    displayModes[5] = lman->lookup("dispModeTable");
    displayModes[6] = lman->lookup("dispModeSound");
    displayModes[7] = lman->lookup("dispModeString");
    choice = new rviewChoice(panel, RVIEW_RESDISP_NUMBER, displayModes, lman->lookup("dispModeLabel"));
    choice->SetSelection(prefs->lastDisplay);

    scaleModes[0] = lman->lookup("textResample");
    scaleModes[1] = lman->lookup("textScale");
    scaleMode = new rviewChoice(panel, 2, scaleModes);
    scaleMode->SetSelection(0);
    scaleMode->SetLabel("");

    sprintf(buffer, "%f", 1.0);
    resampText = new rviewText(panel, buffer);
    resampBut = new rviewButton(panel, lman->lookup("textOK"));

    choice->GetSize(&x, &y);
    cbfactor = x - result_cwidth;

    SetMenuBar(mBar);
}




void rviewResult::OnMenuCommand(int id)
{
    switch (id)
    {
    case MENU_RSLT_ITEM_OPENALL:
    {
        for (int i=0; i<coll->number; i++)
        {
            openViewer(i);
        }
    }
    break;
    case MENU_RSLT_ITEM_THUMBALL:
    {
        int i;
        rviewThumb *thumb;

        thumb = new rviewThumb;
        for (i=0; i<coll->number; i++)
        {
            thumb->addMDD(coll->mddObjs[i].mdd);
        }
        registerChild((rviewFrame*)thumb);
    }
    break;
    case MENU_RSLT_ITEM_CLOSE:
    {
        this->Close(TRUE);
    }
    break;
    case MENU_RSLT_SLCT_SLCTALL:
    {
        for (int i=0; i<coll->number; i++)
        {
            if (!list->Selected(i))
            {
                list->SetSelection(i, TRUE);
            }
        }
        if (selectedItems != NULL) memset(selectedItems, 0xff, (coll->number + 7) >> 3);
        configureGrey();
    }
    break;
    case MENU_RSLT_SLCT_CLEAR:
    {
        for (int i=0; i<coll->number; i++)
        {
            if (list->Selected(i))
            {
                list->SetSelection(i, FALSE);
            }
        }
        if (selectedItems != NULL) memset(selectedItems, 0, (coll->number + 7) >> 3);
        configureGrey();
    }
    break;
    case MENU_RSLT_SLCT_OPEN:
    {
        int i;

        for (i=0; i<coll->number; i++)
        {
            if (list->Selected(i))
                openViewer(i);
        }
    }
    break;
    case MENU_RSLT_SLCT_THUMB:
    {
        int i;

        for (i=0; i<coll->number; i++)
        {
            if (list->Selected(i)) break;
        }
        if (i < coll->number)
        {
            rviewThumb *thumb;

            thumb = new rviewThumb;
            for (i=0; i<coll->number; i++)
            {
                if (list->Selected(i))
                    thumb->addMDD(coll->mddObjs[i].mdd);
            }
            registerChild((rviewFrame*)thumb);
        }
    }
    break;
    case MENU_RSLT_SLCT_DELETE:
    {
        int itemNo, j;
        user_event usr;

        do
        {
            for (itemNo=0; itemNo < coll->number; itemNo++)
                if (list->Selected(itemNo)) break;

            if (itemNo >= coll->number) break;
            list->Delete(itemNo);
            // Notify all open display frames that a particular mdd object will die
            usr.type = usr_mdd_dying;
            usr.data = (void*)(&((coll->mddObjs[itemNo]).mdd));
            frames->broadcastUserEvent(usr);
            if (!coll->mddObjs[itemNo].mdd.is_null())
            {
                coll->mddObjs[itemNo].mdd.destroy();
            }
            (coll->number)--;
            for (j=itemNo; j<coll->number; j++)
            {
                coll->mddObjs[j] = coll->mddObjs[j+1];
                // Don't forget to copy the selection too
                selectedItems[j>>3] &= ~(1<<(j&7));
                if ((selectedItems[(j+1)>>3] & (1<<((j+1)&7))) != 0) selectedItems[j>>3] |= (1<<(j&7));
            }
        }
        while (1);
        lastSelected = updateSelection();
        configureGrey();
    }
    break;
    case MENU_RSLT_SLCT_ENDIAN:
    {
        int itemNo;
        user_event ue;

        operationPrologue();

        ue.type = usr_mdd_dying;
        for (itemNo = 0; itemNo < coll->number; itemNo++)
        {
            if ((list->Selected(itemNo)) && (!coll->mddObjs[itemNo].mdd.is_null()))
            {
                r_Minterval useInterv;

                ue.data = (void*)(&((coll->mddObjs[itemNo]).mdd));
                frames->broadcastUserEvent(ue); // close all open viewers
                useInterv = coll->mddObjs[itemNo].mdd->spatial_domain();
                mdd_objectChangeEndianness(coll->mddObjs[itemNo].mdd, useInterv);
            }
        }
        operationEpilogue(lman->lookup("operationEndian"));
    }
    break;
    case MENU_RSLT_SLCT_TYPEMAN:
        if (typeManager == NULL)
        {
            int itemNo;
            const r_Type *sampleType = NULL;

            for (itemNo = 0; itemNo < coll->number; itemNo++)
            {
                if ((list->Selected(itemNo)) && (!coll->mddObjs[itemNo].mdd.is_null()))
                {
                    sampleType = coll->mddObjs[itemNo].mdd->get_base_type_schema();
                    break;
                }
            }
            /*try {
              sampleType = r_Type::get_any_type("struct { struct { float u, float v, float w }, float p, float te, float ed, float den, float vis }");
            } catch (r_Error &err) {
              cerr << err.what() << endl;
            }*/
            if (sampleType != NULL)
            {
                if (typeManager == NULL)
                    typeManager = new rviewTypeMan(this, sampleType);
            }
        }
        break;
    case MENU_RSLT_SLCT_INFO:
        break;
    default:
        break;
    }
}



void rviewResult::configureGrey(void)
{
    int i;

    mBar->Enable(MENU_RSLT_ITEM_OPENALL, (coll->number > 0));
    mBar->Enable(MENU_RSLT_ITEM_THUMBALL, (coll->number> 0));

    for (i=0; i<coll->number; i++)
    {
        if (list->Selected(i)) break;
    }
    bool enable = (i < coll->number);

    mBar->Enable(MENU_RSLT_SLCT_CLEAR, enable);
    mBar->Enable(MENU_RSLT_SLCT_OPEN, enable);
    mBar->Enable(MENU_RSLT_SLCT_THUMB, enable);
    mBar->Enable(MENU_RSLT_SLCT_DELETE, enable);
    mBar->Enable(MENU_RSLT_SLCT_ENDIAN, enable);
    mBar->Enable(MENU_RSLT_SLCT_TYPEMAN, enable);
    mBar->Enable(MENU_RSLT_SLCT_INFO, enable);
}





/*
 *  rView about window
 */

const int rviewAbout::about_width = 300;
const int rviewAbout::about_height = 150;
const int rviewAbout::about_border = 8;
const int rviewAbout::about_pheight = 50;
const int rviewAbout::about_bwidth = 80;
const int rviewAbout::about_bheight = 30;
const int rviewAbout::about_mheight = 16;

rviewAbout::rviewAbout(void) : rviewFrame(NULL, "", -1, -1, about_width, about_height)
{
    labels = NULL;
    numlines = 0;
    panel = new wxPanel((wxWindow*)this, 0, 0, about_width, about_height);
    okBut = new rviewButton(panel);

    label();

    OnSize(about_width, about_height);
}


rviewAbout::~rviewAbout(void)
{
    // the messages themselves will be sorted out by the panel destructor.
    if (labels != NULL) delete [] labels;
}


const char *rviewAbout::getFrameName(void) const
{
    return "rviewAbout";
}

rviewFrameType rviewAbout::getFrameType(void) const
{
    return rviewFrameTypeAbout;
}


void rviewAbout::deleteLabels(void)
{
    if (labels != NULL)
    {
        int i;

        for (i=0; i<numlines; i++) delete labels[i];
        delete [] labels;
        labels = NULL;
        numlines = 0;
    }
}


void rviewAbout::label(void)
{
    int i, number, x, y;

    SetTitle(lman->lookup("titleAbout"));
    okBut->SetLabel(lman->lookup("textOK"));

    deleteLabels();

    GetClientSize(&x, &y);
    y = about_border;
    number = atoi(lman->lookup("rviewAboutLineNum"));
    if (number >= 1)
    {
        char lineName[64];

        numlines = number;
        labels = new wxMessage *[numlines];
        x -= 2*about_border;
        for (i=0; i<number; i++)
        {
            sprintf(lineName, "rviewAboutLine%d", i);
            labels[i] = new wxMessage(panel, lman->lookup(lineName), about_border, y, x, about_mheight, 0, "message");
            y += about_mheight;
        }
    }
    SetSize(-1, -1, x, y + about_pheight + rview_window_extra_height);
}


int rviewAbout::process(wxObject &obj, wxEvent &evt)
{
    if ((&obj == (wxObject*)okBut) && (evt.GetEventType() == wxEVENT_TYPE_BUTTON_COMMAND))
    {
        Show(FALSE);
        return 1;
    }
    return 0;
}


void rviewAbout::OnSize(int w, int h)
{
    int x, y;

    GetClientSize(&x, &y);

    panel->SetSize(0, 0, x, y);
    x -= 2*about_border;
    y = about_border;
    if (panel != NULL)
    {
        for (int i=0; i<numlines; i++)
        {
            labels[i]->SetSize(about_border, y, x, about_mheight);
            y += about_mheight;
        }
    }
    x -= about_bwidth;
    y += (about_pheight - about_bheight)/2;
    okBut->SetSize(x/2, y, about_bwidth, about_bheight);
}





/*
 *  rviewStringSet window
 */

const int rviewStringSet::strset_width = 300;
const int rviewStringSet::strset_height = 250;
const int rviewStringSet::strset_border = 8;
const int rviewStringSet::strset_reserve = 50;
const int rviewStringSet::strset_bwidth = 70;
const int rviewStringSet::strset_bheight = 40;
const int rviewStringSet::strset_mheight = 20;

rviewStringSet::rviewStringSet(collection_desc *desc) : rviewFrame(NULL, "", -1, -1, strset_width, strset_height)
{
    int w, h;
    char buffer[STRINGSIZE];

    panel = new wxPanel((wxWindow*)this, 0, 0, strset_width, strset_height);
    list = new wxListBox(panel, (wxFunction)rviewEventHandler, "", wxSINGLE, 0, 0, strset_width, strset_height, desc->number, desc->strObjs, wxALWAYS_SB);
    dismiss = new rviewButton(panel);

    sprintf(buffer, "%s: %s", lman->lookup("textCollName"), (desc->collName == NULL) ? "" : desc->collName);
    collName = new wxMessage(panel, buffer, 0, 0, 10, 10, 0, "message");
    sprintf(buffer, "%s: %s", lman->lookup("textCollType"), (desc->collType == NULL) ? "" : desc->collType);
    collType = new wxMessage(panel, buffer, 0, 0, 10, 10, 0, "message");
    char* tmpChar1 = "message";
    char* tmpChar2 = "";
    collInfo = new wxMessage(panel, (desc->collInfo == NULL) ? tmpChar2 : desc->collInfo, 0, 0, 10, 10, 0, tmpChar1);

    GetClientSize(&w, &h);

    label();

    OnSize(w, h);

    Show(TRUE);
}


rviewStringSet::~rviewStringSet(void)
{
}


const char *rviewStringSet::getFrameName(void) const
{
    return "rviewStringSet";
}

rviewFrameType rviewStringSet::getFrameType(void) const
{
    return rviewFrameTypeStrSet;
}


void rviewStringSet::label(void)
{
    SetTitle(lman->lookup("titleStringSet"));
    list->SetLabel(lman->lookup("strSetList"));
    dismiss->SetLabel(lman->lookup("textClose"));
}


void rviewStringSet::OnSize(int w, int h)
{
    int x, y;
    int head;

    GetClientSize(&x, &y);
    panel->SetSize(0, 0, x, y);
    head = 3*(strset_mheight + strset_border);
    x -= 2*strset_border;
    y -= 2*strset_border;
    collName->SetSize(strset_border, strset_border, x, strset_mheight);
    collType->SetSize(strset_border, strset_mheight + 2*strset_border, x, strset_mheight);
    collInfo->SetSize(strset_border, 2*strset_mheight + 3*strset_border, x, strset_mheight);

    list->SetSize(strset_border, strset_border + head, x, y - head - strset_reserve);
    dismiss->SetSize((x - strset_bwidth) / 2, y + 2*strset_border - (strset_reserve + strset_bheight) / 2, strset_bwidth, strset_bheight);
}


int rviewStringSet::process(wxObject &obj, wxEvent &evt)
{
    if (&obj == (wxObject*)dismiss)
    {
        this->Close(TRUE);
        return 1;
    }
    return 0;
}


int rviewStringSet::getNumItems(void)
{
    return list->Number();
}


void rviewStringSet::addItem(const char *string)
{
    list->Append((char*)string);
}


char *rviewStringSet::getItem(int number)
{
    return list->GetString(number);
}


#ifdef EARLY_TEMPLATE
#undef __EXECUTABLE__
#endif

#endif  // !defined(EARLY_TEMPLATE) || !defined(__EXECUTABLE__)



#if (!defined(EARLY_TEMPLATE) || defined(__EXECUTABLE__))

/*
 *  Templates
 */

// For placement new (DynamicStack template)
#include <new>

/*
 *  Use malloc/free, placement new and explicit calls to destructors.
 *  Use placement new when initializing on the stack because that's
 *  raw memory, use assignment when initializing parameters
 */
template<class T>
DynamicStack<T>::DynamicStack(unsigned int gran)
{
    number = 0;
    max = 0;
    if ((stack = (T*)malloc(gran * sizeof(T))) != NULL)
    {
        granularity = gran;
        max = granularity;
        memset(stack, 0, max * sizeof(T));
    }
}


template<class T>
DynamicStack<T>::DynamicStack(const DynamicStack<T> &src)
{
    number = 0;
    max = 0;
    if ((stack = (T*)malloc(src.max * sizeof(T))) != NULL)
    {
        granularity = src.granularity;
        max = src.max;
        memset(stack, 0, max*sizeof(T));
        number = src.number;

        unsigned int i;

        // use loop for class safety
        for (i=0; i<number; i++)
            new(stack + i) T(src.stack[i]);   // placement new
    }
}


template<class T>
DynamicStack<T>::~DynamicStack(void)
{
    unsigned int i;

    for (i=0; i<number; i++)
        stack[i].~T();  // direct destructor call

    if (stack != NULL)
        free(stack);
}


template<class T>
int DynamicStack<T>::push(const T &item)
{
    if (ensureFree() != 0) return -1;
    new(stack + number++) T(item);    // placement new
    return 0;
}


template<class T>
int DynamicStack<T>::pop(T &item)
{
    if (number == 0) return -1;
    item = stack[--number];   // assignment
    stack[number].~T();       // direct destructor call
    return 0;
}


template<class T>
int DynamicStack<T>::peek(T &item) const
{
    if (number == 0) return -1;
    item = stack[number-1];   // assignment
    return 0;
}


template<class T>
unsigned int DynamicStack<T>::getNumber(void) const
{
    return number;
}


template<class T>
int DynamicStack<T>::ensureFree(void)
{
    if (number >= max)
    {
        if ((stack = (T*)realloc(stack, (max + granularity) * sizeof(T))) == NULL)
        {
            max = 0;
            number = 0;
            return -1;
        }
        memset(stack + max, 0, granularity * sizeof(T));
        max += granularity;
    }
    return 0;
}

#endif
