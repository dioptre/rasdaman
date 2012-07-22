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
 * SOURCE: mddtypes.cc
 *
 * MODULE: raslib
 * CLASS:
 *
 * COMMENTS:
 *
*/


static const char rcsid[] = "@(#)raslib, mddtypes: $Header: /home/rasdev/CVS-repository/rasdaman/raslib/mddtypes.cc,v 1.26 2005/09/03 20:33:10 rasdev Exp $";



#ifdef AIX
#include <strings.h>
#else
#include <string.h>
#endif

#include "raslib/mddtypes.hh"
#include "raslib/rminit.hh"


/*
 *  The names of all data formats
 */
const char *format_name_array = "Array";
const char *format_name_tiff = "TIFF";
const char *format_name_jpeg = "JPEG";
const char *format_name_hdf = "HDF";
const char *format_name_netcdf = "NETCDF";
const char *format_name_cvs = "CVS";
const char *format_name_png = "PNG";
const char *format_name_zlib = "ZLib";
const char *format_name_auto_compression = "AutoCompression";
const char *format_name_bmp = "BMP";
const char *format_name_ppm = "PPM";
const char *format_name_rle = "RLE";
const char *format_name_wavelet_haar = "HaarWavelet";
const char *format_name_wavelet_daubechies = "DaubechiesWavelet";
const char *format_name_sep_zlib = "SepZLib";
const char *format_name_sep_rle = "SepRLE";
const char *format_name_wavelet_daub6 = "Daubechies6Wavelet";
const char *format_name_wavelet_daub8 = "Daubechies8Wavelet";
const char *format_name_wavelet_daub10 = "Daubechies10Wavelet";
const char *format_name_wavelet_daub12 = "Daubechies12Wavelet";
const char *format_name_wavelet_daub14 = "Daubechies14Wavelet";
const char *format_name_wavelet_daub16 = "Daubechies16Wavelet";
const char *format_name_wavelet_daub18 = "Daubechies18Wavelet";
const char *format_name_wavelet_daub20 = "Daubechies20Wavelet";
const char *format_name_wavelet_least8 = "LeastAsym8Wavelet";
const char *format_name_wavelet_least10 = "LeastAsym10Wavelet";
const char *format_name_wavelet_least12 = "LeastAsym12Wavelet";
const char *format_name_wavelet_least14 = "LeastAsym14Wavelet";
const char *format_name_wavelet_least16 = "LeastAsym16Wavelet";
const char *format_name_wavelet_least18 = "LeastAsym18Wavelet";
const char *format_name_wavelet_least20 = "LeastAsym20Wavelet";
const char *format_name_wavelet_coiflet6 = "Coiflet6Wavelet";
const char *format_name_wavelet_coiflet12 = "Coiflet12Wavelet";
const char *format_name_wavelet_coiflet18 = "Coiflet18Wavelet";
const char *format_name_wavelet_coiflet24 = "Coiflet24Wavelet";
const char *format_name_wavelet_coiflet30 = "Coiflet30Wavelet";
const char *format_name_vff = "VFF";
const char *format_name_wavelet_qhaar = "QHaarWavelet";
const char *format_name_tor = "TOR";
const char *format_name_dem = "DEM";
const char *format_name_pack_bits = "PACKBITS";
const char *format_name_ecw = "ECW";
const char *format_name_tmc = "TMC";
const char *format_name_ntf = "NTF";

const char *all_data_format_names[r_Data_Format_NUMBER] =
{
    format_name_array,
    format_name_tiff,
    format_name_jpeg,
    format_name_cvs,
    format_name_hdf,
    format_name_netcdf,
    format_name_png,
    format_name_zlib,
    format_name_auto_compression,
    format_name_bmp,
    format_name_rle,
    format_name_wavelet_haar,
    format_name_wavelet_daubechies,
    format_name_sep_zlib,
    format_name_sep_rle,
    format_name_wavelet_daub6,
    format_name_wavelet_daub8,
    format_name_wavelet_daub10,
    format_name_wavelet_daub12,
    format_name_wavelet_daub14,
    format_name_wavelet_daub16,
    format_name_wavelet_daub18,
    format_name_wavelet_daub20,
    format_name_wavelet_least8,
    format_name_wavelet_least10,
    format_name_wavelet_least12,
    format_name_wavelet_least14,
    format_name_wavelet_least16,
    format_name_wavelet_least18,
    format_name_wavelet_least20,
    format_name_wavelet_coiflet6,
    format_name_wavelet_coiflet12,
    format_name_wavelet_coiflet18,
    format_name_wavelet_coiflet24,
    format_name_wavelet_coiflet30,
    format_name_vff,
    format_name_wavelet_qhaar,
    format_name_ppm,
    format_name_tor,
    format_name_dem,
    format_name_pack_bits,
    format_name_ecw,
    format_name_tmc,
    format_name_ntf
};


const char *get_name_from_data_format( r_Data_Format fmt )
{
    unsigned int idx = (unsigned int)fmt;

    if (idx >= (unsigned int)r_Data_Format_NUMBER)
        return "???";

    return all_data_format_names[idx];
}


r_Data_Format get_data_format_from_name( const char *name )
{
    if(!name)
    {
        RMInit::logOut << "get_data_format_from_name(" << (name?name: "NULL") << ")" << endl;
        return r_Data_Format_NUMBER;
    }

    unsigned int i=r_Data_Format_NUMBER;

    for (i=0; i<(unsigned int)r_Data_Format_NUMBER; i++)
    {
        if (strcasecmp(name, all_data_format_names[i]) == 0)
            break;
    }
    return (r_Data_Format)i;
}



std::ostream& operator<<( std::ostream& s, r_Data_Format& d )
{
    s << (const r_Data_Format)d;
    return s;
}

std::ostream& operator<<( std::ostream& s, const r_Data_Format& d )
{
    s << get_name_from_data_format(d);

    return s;
}

const char* scale_function_name_subsampling = "subsampling";
const char* scale_function_name_bitaggregation = "bitaggregation";

const char* all_scale_function_names[r_Scale_Function_NUMBER] =
{
    scale_function_name_subsampling,
    scale_function_name_bitaggregation
};

const char *get_name_from_scale_function(r_Scale_Function fmt)
{
    unsigned int idx = (unsigned int)fmt;

    if (idx >= (unsigned int)r_Scale_Function_NUMBER)
        return "???";

    return all_scale_function_names[idx];
}


r_Scale_Function get_scale_function_from_name(const char *name)
{
    if(!name)
    {
        RMInit::logOut << "get_scale_function_from_name(" << (name?name: "NULL") << ")" << endl;
        return r_Scale_Function_NUMBER;
    }

    unsigned int i=r_Scale_Function_NUMBER;

    for (i=0; i<(unsigned int)r_Scale_Function_NUMBER; i++)
    {
        if (strcasecmp(name, all_scale_function_names[i]) == 0)
            break;
    }
    return (r_Scale_Function)i;
}

std::ostream& operator<<( std::ostream& s, const r_Scale_Function& d )
{
    s << get_name_from_scale_function(d);

    return s;
}

/*
 * The names of all index type
 */

const char *index_name_auto="auto";
const char *index_name_directory="dir";
const char *index_name_regdirectory="rdir";
const char *index_name_rplustree="nrp";
const char *index_name_regrplustree="rnrp";
const char *index_name_tilecontainer="tc";
const char *index_name_regcomputed="rc";

const char *all_index_type_names[r_Index_Type_NUMBER] =
{
    index_name_auto,
    index_name_directory,
    index_name_regdirectory,
    index_name_rplustree,
    index_name_regrplustree,
    index_name_tilecontainer,
    index_name_regcomputed
};

const char *get_name_from_index_type( r_Index_Type it )
{
    unsigned int idx = (unsigned int)it;

    if (idx >= (unsigned int)r_Index_Type_NUMBER)
        return "UNKNOWN r_Index_Type";

    if (idx == (unsigned int)r_Invalid_Index)
        return "r_Invalid_Index";

    return all_index_type_names[idx];
}

r_Index_Type get_index_type_from_name( const char *name )
{
    if(!name)
    {
        RMInit::logOut << "get_index_type_from_name(" << (name?name: "NULL") << ")" << endl;
        return r_Index_Type_NUMBER;
    }

    unsigned int i=r_Index_Type_NUMBER;

    for (i=0; i<(unsigned int)r_Index_Type_NUMBER; i++)
    {
        if (strcasecmp(name, all_index_type_names[i]) == 0)
            break;
    }
    return (r_Index_Type)i;
}


std::ostream& operator<<( std::ostream& s, r_Index_Type d )
{
    switch( d )
    {
    case r_Invalid_Index:
        s << "r_Invalid_Index";
        break;
    case r_Auto_Index:
        s << "r_Auto_Index";
        break;
    case r_Directory_Index:
        s << "r_Directory_Index";
        break;
    case r_Reg_Directory_Index:
        s << "r_Reg_Directory_Index";
        break;
    case r_RPlus_Tree_Index:
        s << "r_RPlus_Tree_Index";
        break;
    case r_Reg_RPlus_Tree_Index:
        s << "r_Reg_RPlus_Tree_Index";
        break;
    case r_Tile_Container_Index:
        s << "r_Tile_Container_Index";
        break;
    case r_Reg_Computed_Index:
        s << "r_Reg_Computed_Index";
        break;
    default:
        s << "UNKNOWN r_Index_Type " << d;
        break;
    }

    return s;
}

/*
 *  The names of all tiling schemes
 */

const char *tiling_name_notiling = "NoTiling";
const char *tiling_name_regulartiling = "RegularTiling";
const char *tiling_name_statisticaltiling = "StatisticalTiling";
const char *tiling_name_interesttiling = "InterestTiling";
const char *tiling_name_alignedtiling = "AlignedTiling";
const char *tiling_name_directionaltiling = "DirectionalTiling";
const char *tiling_name_sizetiling = "SizeTiling";

const char *all_tiling_scheme_names[r_Tiling_Scheme_NUMBER] =
{
    tiling_name_notiling,
    tiling_name_regulartiling,
    tiling_name_statisticaltiling,
    tiling_name_interesttiling,
    tiling_name_alignedtiling,
    tiling_name_directionaltiling,
    tiling_name_sizetiling
};

const char *get_name_from_tiling_scheme( r_Tiling_Scheme ts )
{
    unsigned int idx = (unsigned int)ts;

    if (idx >= (unsigned int)r_Tiling_Scheme_NUMBER)
        return "UNKNOWN r_Tiling_Scheme";

    return all_tiling_scheme_names[idx];
}

r_Tiling_Scheme get_tiling_scheme_from_name( const char *name )
{
    if(!name)
    {
        RMInit::logOut << "get_tiling_scheme_from_name(" << (name?name: "NULL") << ")" << endl;
        return r_Tiling_Scheme_NUMBER;
    }

    unsigned int i=r_Tiling_Scheme_NUMBER;

    for (i=0; i<(unsigned int)r_Tiling_Scheme_NUMBER; i++)
    {
        if (strcasecmp(name, all_tiling_scheme_names[i]) == 0)
            break;
    }
    return (r_Tiling_Scheme)i;
}

std::ostream& operator<<(std::ostream& s, r_Tiling_Scheme d)
{
    s << get_name_from_tiling_scheme(d);
    return s;
}

std::ostream& operator<<( std::ostream& s, const r_Clustering_Scheme d )
{
    switch( d )
    {
    case r_Insertion_Order_Clustering:
        s << "r_Insertion_Order_Clustering";
        break;
    case r_Coords_Order_Clustering:
        s << "r_Coords_Order_Clustering";
        break;
    case r_Index_Cluster_Clustering:
        s << "r_Index_Cluster_Clustering";
        break;
    case r_Based_Cluster_Stat_Clustering:
        s << "r_Based_Cluster_Stat_Clustering";
        break;
    default:
        s << "UNKNOWN r_Clustering_Scheme " << d;
        break;
    }

    return s;
}


#ifdef __VISUALC__
#include <ctype.h>
int strcasecmp(const char *s1, const char *s2)
{
    const char *b, *d;

    b = s1;
    d = s2;
    while ((*b != '\0') && (*d != '\0'))
    {
        if (tolower((unsigned int)*b) != tolower((unsigned int)*d))
            break;
        b++;
        d++;
    }
    if ((*b == '\0') && (*d == '\0'))
        return 0;
    if (tolower((unsigned int)*b) < tolower((unsigned int)*d))
        return -1;
    return 1;
}
#endif
