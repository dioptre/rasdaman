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
 * INCLUDE: mddtypes.hh
 *
 * MODULE:  raslib
 *
 * PURPOSE:
 *      The file cotains MDD type definitions.
 *
 * COMMENTS:
 * - always append new data formats to remain compatible with earlier compiled code
 *
*/

#ifndef _D_MDDTYPES_
#define _D_MDDTYPES_


#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <sstream>
#endif

//typedef unsigned int uint32;
//typedef int int32;

//@Man: r_Bytes
//@Type: typedef
//@Args: as unsigned int
//@Memo: Module: {\bf raslib}.

typedef unsigned int r_Bytes;

/**
  {\tt typedef unsigned int r_Bytes;}

  The typedef \Ref{r_Bytes} is used as type for the number of bytes in an tile or mdd or type.
*/

//@Man: r_Ptr
//@Type: typedef
//@Args: as unsigned long
//@Memo: Module: {\bf raslib}.

typedef unsigned long r_Ptr;

/**
  {\tt typedef unsigned long r_Ptr;}

  The typedef \Ref{r_Ptr} was introduced to handle correctly convertions from
  pointers to integer variables on 64bit architectures.
*/



//@Man: r_Area
//@Type: typedef
//@Args: as unsigned int
//@Memo: Module: {\bf raslib}.

typedef unsigned int r_Area;

/**
  {\tt typedef unsigned int r_Area;}

  The typedef \Ref{r_Area} is used as type for the number of cells in an mdd object or tile.
*/



//@Man: r_Range
//@Type: typedef
//@Args: as int
//@Memo: Module: {\bf raslib}.

typedef int r_Range;

/**
  {\tt typedef int r_Range;}

  The typedef \Ref{r_Range} is used as type for the point set
  of one dimension of a spatial domain. This means that lower
  and upper bounds of \Ref{r_Sinterval}, the projection value,
  and the cooridnate values of \Ref{r_Point} are of this type.
*/



//@Man: r_Dimension
//@Type: typedef
//@Args: as unsigned int
//@Memo: Module: {\bf raslib}.

typedef unsigned int r_Dimension;

/**
  {\tt typedef unsigned int r_Dimension;}

  This is used as type for the number of dimensions in
  \Ref{r_Point} and \Ref{r_Minterval}.
*/


//@Man: r_Data_Format
//@Type: enum
//@Args:
//@Memo: Module: {\bf raslib}.

enum r_Data_Format
{
    r_Array,
    r_TIFF,
    r_JPEG,
    r_HDF,
    r_NETCDF,
    r_CSV,
    r_PNG,
    r_ZLib,
    r_Auto_Compression,
    r_BMP,
    r_RLE,
    r_Wavelet_Haar,
    r_Wavelet_Daubechies, // = Daubechies 4 tap
    r_Sep_ZLib,
    r_Sep_RLE,
    r_Wavelet_Daub6,
    r_Wavelet_Daub8,
    r_Wavelet_Daub10,
    r_Wavelet_Daub12,
    r_Wavelet_Daub14,
    r_Wavelet_Daub16,
    r_Wavelet_Daub18,
    r_Wavelet_Daub20,
    r_Wavelet_Least8,
    r_Wavelet_Least10,
    r_Wavelet_Least12,
    r_Wavelet_Least14,
    r_Wavelet_Least16,
    r_Wavelet_Least18,
    r_Wavelet_Least20,
    r_Wavelet_Coiflet6,
    r_Wavelet_Coiflet12,
    r_Wavelet_Coiflet18,
    r_Wavelet_Coiflet24,
    r_Wavelet_Coiflet30,
    r_VFF,
    r_Wavelet_QHaar,
    r_PPM,
    r_TOR,
    r_DEM,
    r_Pack_Bits,
    r_ECW,
    r_TMC,
    r_NTF,
    r_Data_Format_NUMBER
};

/**
  {\tt enum r_Data_Format}

  \begin{tabular}{lll}
  {\ttr_Array}              && no compression, row-major memory representation\\

  {\ttr_TIFF}               && TIFF format (see \Ref{r_Conv_TIFF})\\
  {\ttr_JPEG}               && JPEG format (see \Ref{r_Conv_JPEG})\\
  {\ttr_HDF}                && HDF  format (see \Ref{r_Conv_HDF})\\
  {\ttr_PNG}                && PNG  format (see \Ref{r_Conv_PNG})\\
  {\ttr_BMP}                && BMP  format (see \Ref{r_Conv_BMP})\\
  {\ttr_VFF}                && VFF  format (see \Ref{r_Conv_VFF})\\
  {\ttr_PPM}            && PPM  format (see \Ref{r_Conv_PPM})\\
  {\ttr_TOR}            && TOR  format (see \Ref{r_Conv_TOR})\\
  {\ttr_DEM}            && DEM  format (see \Ref{r_Conv_DEM})\\
  {\ttr_ECW}            && ECW  format (see \Ref{r_Conv_ECW})\\
  {\ttr_NTF}            && NITF  format (see \Ref{r_Conv_NTF})\\
  {\ttr_NETCDF}             && NETCDF  format (see \Ref{r_Conv_NETCDF})\\

  {\ttr_Auto_Compression}   && automatic compression\\
  {\ttr_ZLib}               && ZLIB compresion  (see \Ref{r_Tile_Comp_RLE})\\
  {\ttr_Pack_Bits}          && Packbits rle compresion  (see \Ref{r_Tile_Comp_Packbits})\\
  {\ttr RLE}                && RLE compression  (see \Ref{r_Tile_Comp_RLE})\\
  {\ttr_Wavelet_Haar}       && Haar Wavelet compression  (see \Ref{r_Haar_Wavelet_Compression})\\
  {\ttr_Wavelet_Daubechies} && Daubechies 4-tap Wavelet compression  (see \Ref{r_Daubechies_Wavelet_Compression})\\
  {\ttr_Sep_ZLib}           && ZLIB compression, compress base types separately  (see \Ref{r_Tile_Separate_ZLIB})\\
  {\ttr_Sep_RLE}            && RLE compression, compress base types separately  (see \Ref{r_Tile_Separate_RLE})\\
  {\ttr_Wavelet_Daub<n>}    && Daubechies n-tap Wavelet compression, n=6,8,...,18,20  (see \Ref{r_Ortho_Wavelet_Factory})\\
  {\ttr_Wavelet_Least<n>}   && Least asymmetric n-tap Wavelet comp., n=8,10,...,18,20  (see \Ref{r_Ortho_Wavelet_Factory})\\
  {\ttr_Wavelet_Coiflet<n>} && Coiflet n-tap Wavelet compression, n=6,12,18,24,30  (see \Ref{r_Ortho_Wavelet_Factory})\\
  {\ttr_Wavelet_QHaar}      && Lossy Haar Wavelet compression  (see \Ref{r_Haar_QWavelet_Compression})\\

  \end{tabular}
*/

//@ManMemo: Module: {\bf raslib}
/**
   The names of all data types, to avoid redundant storage and inconsistencies.
   The variable name convention is the prefix format_name_ followed by the name
   of the data format in lower case without the r_ prefix, i.e. for r_Wavelet_Haar
   format_name_wavelet_haar.
   In addition there's an array of names all_data_format_names where the data format
   can be used as index to get the name.
*/

extern const char *format_name_array;
extern const char *format_name_tiff;
extern const char *format_name_jpeg;
extern const char *format_name_hdf;
extern const char *format_name_netcdf;
extern const char *format_name_png;
extern const char *format_name_zlib;
extern const char *format_name_auto_compression;
extern const char *format_name_bmp;
extern const char *format_name_ppm;
extern const char *format_name_rle;
extern const char *format_name_wavelet_haar;
extern const char *format_name_wavelet_daubechies;
extern const char *format_name_sep_zlib;
extern const char *format_name_sep_rle;
extern const char *format_name_wavelet_daub6;
extern const char *format_name_wavelet_daub8;
extern const char *format_name_wavelet_daub10;
extern const char *format_name_wavelet_daub12;
extern const char *format_name_wavelet_daub14;
extern const char *format_name_wavelet_daub16;
extern const char *format_name_wavelet_daub18;
extern const char *format_name_wavelet_daub20;
extern const char *format_name_wavelet_least8;
extern const char *format_name_wavelet_least10;
extern const char *format_name_wavelet_least12;
extern const char *format_name_wavelet_least14;
extern const char *format_name_wavelet_least16;
extern const char *format_name_wavelet_least18;
extern const char *format_name_wavelet_least20;
extern const char *format_name_wavelet_coiflet6;
extern const char *format_name_wavelet_coiflet12;
extern const char *format_name_wavelet_coiflet18;
extern const char *format_name_wavelet_coiflet24;
extern const char *format_name_wavelet_coiflet30;
extern const char *format_name_vff;
extern const char *format_name_tor;
extern const char *format_name_dem;
extern const char *format_name_pack_bits;
extern const char *format_name_wavelet_qhaar;
extern const char *format_name_tmc;
extern const char *format_name_ntf;

extern const char *all_data_format_names[r_Data_Format_NUMBER];

//@ManMemo: Module: {\bf raslib}
/**
   Get a data format name for a data format
*/
const char *get_name_from_data_format( r_Data_Format fmt );

//@ManMemo: Module: {\bf raslib}
/**
  Get a data format for a data format name
*/
r_Data_Format get_data_format_from_name ( const char *name );


//@ManMemo: Module: {\bf raslib}
/**
  Output stream operator for objects of type {\tt const} \Ref{r_Data_Format}.
*/
extern std::ostream& operator<<( std::ostream& s, const r_Data_Format& d );



//@ManMemo: Module: {\bf raslib}
/**
  Output stream operator for objects of type \Ref{r_Data_Format}.
*/
extern std::ostream& operator<<( std::ostream& s, r_Data_Format& d );

//@Man: r_Scale_Function
//@Type: enum
//@Args:
//@Memo: Module: {\bf raslib}.

enum r_Scale_Function
{
    r_SubSampling,
    r_BitAggregation,
    r_Scale_Function_NUMBER
};

extern const char *scale_function_name_subsampling;
extern const char *scale_function_name_bitaggregation;

extern const char *all_scale_function_names[r_Scale_Function_NUMBER];

//@ManMemo: Module: {\bf raslib}
/**
   Get a scale function name for a scale  function
*/
const char *get_name_from_scale_function(r_Scale_Function func);

//@ManMemo: Module: {\bf raslib}
/**
  Get a scale function from a scale function name
*/
r_Scale_Function get_scale_function_from_name(const char *name);


//@ManMemo: Module: {\bf raslib}
/**
  Output stream operator for objects of type {\tt const} \Ref{r_Scale_Function}.
*/
extern std::ostream& operator<<( std::ostream& s, const r_Scale_Function& d );


//@Man: r_Index_Type
//@Type: enum
//@Args:
//@Memo: Module: {\bf raslib}.

enum r_Index_Type
{
    r_Invalid_Index = -1,
    r_Auto_Index = 0,
    r_Directory_Index = 1,
    r_Reg_Directory_Index = 2,
    r_RPlus_Tree_Index = 3,
    r_Reg_RPlus_Tree_Index = 4,
    r_Tile_Container_Index = 5,
    r_Reg_Computed_Index = 6,
    r_Index_Type_NUMBER = 7
};

extern std::ostream& operator<<(std::ostream& in, r_Index_Type type);

//@ManMemo: Module: {\bf raslib}
/**
   The names of all index type, to avoid redundant storage and inconsistencies.
   The variable name convention is the prefix index_name_ followed by the name
   of the index type in lower case without the r_ prefix, i.e. for r_Auto_Index
   index_name_auto.
   In addition there's an array of names all_index_type_names where the index type
   can be used as index to get the name.
*/

extern const char *index_name_auto;
extern const char *index_name_directory;
extern const char *index_name_regdirectory;
extern const char *index_name_rplustree;
extern const char *index_name_regrplustree;
extern const char *index_name_tilecontainer;
extern const char *index_name_regcomputed;

extern const char *all_index_type_names[r_Index_Type_NUMBER];

//@ManMemo: Module: {\bf raslib}
/**
   Get a index type name for a index type
*/
const char *get_name_from_index_type( r_Index_Type it );

//@ManMemo: Module: {\bf raslib}
/**
   Get a index type  for a index type name
*/
r_Index_Type get_index_type_from_name ( const char *name );

//@Man: r_Tiling_Scheme
//@Type: enum
//@Args:
//@Memo: Module: {\bf raslib}.

enum r_Tiling_Scheme
{
    r_NoTiling = 0,
    r_RegularTiling = 1,
    r_StatisticalTiling = 2,
    r_InterestTiling = 3,
    r_AlignedTiling = 4,
    r_DirectionalTiling = 5,
    r_SizeTiling = 6,
    r_Tiling_Scheme_NUMBER = 7
};
/**
    Tiling of the object:

    \begin{tabular}{lll}
    NoTiling && no tiling is done unless the object is too big;
    in that case, tiling is done along the first direction only;
    for objects which are to be accessed always as a whole \\
    {\bf Aligned} &&    aligned tiling, needs tileConfig \\
    LowVariationAreas && according to areas of low cell value variation \\
    BasedTilesStat && based on statistics regarding access  to this MDD object
    \end{tabular}

    In addition, it is possible to have a tiling according to areas of
    interest, {\bf AreasInterest} mode.
    The {\tt AreasInterest} mode is indicated by a non - null value of the
    {\tt areasInterestPath} attribute.
    This mode is not an alternative mode in {\tt TilingScheme} because it is
    compatible with the other modes. For instance, an aligned tiling may be
    adopted outside the areas of interest.
    */

//@ManMemo: Module: {\bf raslib}
/**
   The names of all tiling schems, to avoid redundant storage and inconsistencies.
   The variable name convention is the prefix tiling_name_ followed by the name
   of the tiling scheme in lower case without the r_ prefix, i.e. for r_SizeTiling
   tiling_name_sizetiling.
   In addition there's an array of names all_tiling_scheme_names where the tile scheme
   can be used as index to get the name.
*/

extern const char *tiling_name_notiling;
extern const char *tiling_name_regulartiling;
extern const char *tiling_name_statisticaltiling;
extern const char *tiling_name_interesttiling;
extern const char *tiling_name_alignedtiling;
extern const char *tiling_name_directionaltiling;
extern const char *tiling_name_sizetiling;

extern const char *all_tiling_scheme_names[r_Tiling_Scheme_NUMBER];

//@ManMemo: Module: {\bf raslib}
/**
   Get a tiling scheme name for a tiling scheme
*/
const char *get_name_from_tiling_scheme( r_Tiling_Scheme ts );

//@ManMemo: Module: {\bf raslib}
/**
   Get a tiling scheme for a tiling scheme name
*/
r_Tiling_Scheme get_tiling_scheme_from_name ( const char *name );

//@ManMemo: Module: {\bf raslib}
/**
  Output stream operator for objects of type {\tt const} \Ref{r_Tiling_Scheme}.
*/
extern std::ostream& operator<<(std::ostream& in, r_Tiling_Scheme type);

//@Man: r_Clustering_Scheme
//@Type: enum
//@Args:
//@Memo: Module: {\bf raslib}.
enum r_Clustering_Scheme
{
    r_Insertion_Order_Clustering = 1,
    r_Coords_Order_Clustering = 2,
    r_Index_Cluster_Clustering = 3,
    r_Based_Cluster_Stat_Clustering = 4
};
/**
Clustering of the Tiles according to:

\begin{tabular}{lll}
{\bf InsertionOrder } && the order of insertion of the tiles \\
CoordsOrder &&  the coordinates of the tiles \\
IndexCluster && the index structure \\
BasedClusterStat && statistics about access to the object
\end{tabular}

There is the additional {\bf PathCluster} mode, where clustering is
done according to a path of access to areas of interest.
The {\tt PathCluster} mode is indicated by setting the {\tt pathCluster}
attribute and a non - null value of the {\tt areasInterest}.
This mode is not an alternative mode in {\tt ClusteringScheme} because
it is compatible with the other modes.
*/
extern std::ostream& operator<<(std::ostream& in, r_Clustering_Scheme type);

#ifdef __VISUALC__
extern int strcasecmp( const char *str1, const char *str2 );
#endif

#endif
