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
 * Copyright 2003 - 2011 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
/*************************************************************
 *
 * PURPOSE: Contains general defs used by the rasgeo tools and
 * provides uniform command line parameter definitions (names,
 * defaults, help texts) for the parameters used.
 *
 ************************************************************/

#ifndef _RASGEO_GLOBALS_HH_
#define _RASGEO_GLOBALS_HH_

#ifdef __VISUALC__
#pragma warning( disable : 4290 )
#endif

//@ManMemo: Module: {\bf raslib}

/*@Doc:
 This class contains general defs used by the rasgeo tools and
 provides uniform command line parameter definitions (names,
 defaults, help texts) for the parameters used.
*/

// includes needed for the rasdaman types
#include "globals.hh"

#include "raslib/rminit.hh"
#include "raslib/mitera.hh"
#include "raslib/type.hh"
#include "raslib/marraytype.hh"
#include "raslib/minterval.hh"
#include "raslib/primitivetype.hh"
#include "raslib/storageman.hh"
#include "raslib/mddtypes.hh"

#include "rasodmg/marray.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/set.hh"
#include "rasodmg/database.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/partinsert.hh"
#include "rasodmg/gmarray.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/dirtiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "rasodmg/interesttiling.hh"
#include "rasodmg/alignedtiling.hh"
#include "rasodmg/stattiling.hh"
#include "rasodmg/oqlquery.hh"
#include "rasodmg/polygon.hh"
#include "rasodmg/polycutout.hh"


/// program exit codes
#define EXIT_SUCCESS    0
#define EXIT_USAGE  2
#define EXIT_FAILURE    -1

/// log output indentation
#define LOG_INDENT "   "


/*
 * all known options
 */

#define PARAM_XMIN  "xmin"
#define PARAM_XMAX  "xmax"
#define PARAM_YMIN  "ymin"
#define PARAM_YMAX  "ymax"
#define PARAM_XRES  "xres"
#define PARAM_YRES  "yres"

#define PARAM_HELP_FLAG 'h'
#define PARAM_HELP  "help"
#define HELP_HELP   "show command line switches"


#define PARAM_MDDTYPE_FLAG  'm'
#define PARAM_MDDTYPE       "mddtype"
// database type for DEM
#define MDDTYPE_DEM "DoubleImage"
// database types for TK
#define MDDTYPE_TK_1    "GreyImage"
#define MDDTYPE_TK_2    "UShortImage"
#define MDDTYPE_TK_4    "ULongImage"
// size of pixel types in bytes:
#define MDDYTPE_TK_1_SIZE 1
#define MDDYTPE_TK_2_SIZE 2
#define MDDYTPE_TK_4_SIZE 4
#define HELP_MDDTYPE    "<mdd-type> type of marray (mandatory)"
#define HELP_MDDTYPE_TK "<mdd-type> type of marray; deprecated, will be ignored"

#define PARAM_COLL_FLAG 'c'
#define PARAM_COLL  "collname"
#define HELP_COLL   "<coll-name> name of collection (mandatory)"

#define PARAM_DOMAIN    "mdddomain"
#define HELP_DOMAIN "<mdd-domain> domain of marray, format: \'[x0:x1,y0:y1]\' (mandatory)"

#define PARAM_FILE_FLAG 'f'
#define PARAM_FILE  "file"
#define HELP_FILE   "<file-name> input file"

#define PARAM_READ_FLAG 'r'
#define PARAM_READ  "read"
#define HELP_READ   "<file-name> input file (deprecated)"

#define PARAM_SERV_FLAG 's'
#define PARAM_SERV  "server"
#define DEFAULT_SERV    "localhost"
#define HELP_SERV   "<host-name> rasdaman server"

#define PARAM_PORT_FLAG 'p'
#define PARAM_PORT  "port"
#define DEFAULT_PORT    7001
#define HELP_PORT   "<p> rasmgr port number"

#define PARAM_DB_FLAG   'd'
#define PARAM_DB    "database"
#define HELP_DB     "<db-name> name of database"

#define PARAM_USER  "user"
#define HELP_USER   "<user-name> name of user (mandatory)"

#define PARAM_PASSWD    "passwd"
#define HELP_PASSWD "<user-passwd> password of user (mandatory)"

#define PARAM_MDDOID    "mddoid"
#define DEFAULT_MDDOID      "none, apply to all objects in collection"
#define HELP_MDDOID "<mdd-oid> oid of the marray to work on, format: \'dbms|db|number\'. (*)"

#define HELP_OVERLAY    "<overlay-domain> domain updated in the db in case of overlay, format: \'[x0:x1,y0:y1]\' (*)"

#define PARAM_CONV  "conf"
#define DEFAULT_CONV    "char"
#define DEFAULT_CONV_ORTHO  "char"
#define DEFAULT_CONV_DEM    "double"
#define HELP_CONV   "<type-name> conversion type, one of: char (for images), float (for DEMs)"

#define PARAM_ALIGN "align"
#define HELP_ALIGN  "<pixels> extend update area to align with grid units of this edge length in pixels (good if this is db tile size)"

#define INPUT_ASC   "ASC"
#define INPUT_INT16 "int16"
const string help_input = string( "inputformat, one of " )
                          + format_name_tiff + ", " + format_name_png + ", " + format_name_bmp
                          + ", " + format_name_jpeg
                          + ", " + format_name_tor + ", " + format_name_dem + " (or " + INPUT_ASC + "), " + INPUT_INT16 + " (unsigned; low byte first)"
                          + ", " + format_name_hdf + ", " + format_name_vff + ", " + format_name_ntf;
#define HELP_INPUT  help_input.c_str()

#define HELP_INPUT_P    "<params> input file format parameters (see documentation)"

#define PARAM_TILED "tiledupdate"
#define HELP_TILED  "do update in tiles; use --align to specify tile size"

#define PARAM_TRANS "transformat"
#define DEFAULT_TRANS   r_Array
#define DEFAULT_TRANS_STR   format_name_array

#define PARAM_TRANS_P   "transformatparams"

#define PARAM_TRANSP_FLAG 't'
#define PARAM_TRANSP    "transparent"
#define HELP_TRANSP "transparent update flag. If specified, black (0x0) data are treated as transparent during overlaying"

#define PARAM_POLYGON   "polygon"
#define HELP_POLYGON    "<polygon-path> counter clockwise polygon path to clip away outside area. Needs insidepattern and/or outsidepattern"

#define PARAM_SHRINK    "shrink"
#define DEFAULT_SHRINK      0
#define HELP_SHRINK "<number> pixels that shrink polygon"

#define PARAM_SCALE "scalelevels"
#define HELP_SCALE  "<level-spec> list of scale levels, format: \'collection1:scalefactor1;collection2:scalefactor2;...\'"
#define SCALEITEM_SEPARATOR ':'
#define SCALELEVELS_SEPARATOR   ';'

#define HELP_SCALEFUNC  "<function-name> scaling algorithm, one of: " + scale_function_name_subsampling + ", " + scale_function_name_bitaggregation

// tiling based on tile byte size (used with 2-D rasgeo tools)
#define PARAM_TILING    "tiling"
#define DEFAULT_TILING tiling_name_sizetiling
#define DEFAULT_TILING      131072

const string help_tiling = string( "<tiling-name> tiling strategy, one of: " ) +
                           tiling_name_notiling          + ", " +
                           tiling_name_sizetiling        + ", " +
                           tiling_name_alignedtiling     + ", " +
                           tiling_name_interesttiling    + ", " +
                           tiling_name_directionaltiling + ", " +
                           tiling_name_statisticaltiling;
#define HELP_TILING help_tiling.c_str()

const string help_tiling_p = string( "<params> parameters for tiling strategy, one of (see documentation):\n" ) +
                             "\t\t- for " + tiling_name_notiling + ": " + r_No_Tiling::description + "\n" +
                             "\t\t- for " + tiling_name_sizetiling + ": " + r_Size_Tiling::description + "\n" +
                             "\t\t- for " + tiling_name_alignedtiling + ": " + r_Aligned_Tiling::description + "\n" +
                             "\t\t- for " + tiling_name_interesttiling + ": " + r_Interest_Tiling::description + "\n" +
                             "\t\t- for " + tiling_name_directionaltiling + ": " + r_Dir_Tiling::description + "\n" +
                             "\t\t- for " + tiling_name_statisticaltiling + ": " + r_Stat_Tiling::description;

#define PARAM_TILING_P  "tilingparams"
#define HELP_TILING_P help_tiling_p.c_str()

#define PARAM_STOR  "storageformat"
#define DEFAULT_STOR        r_Array
#define DEFAULT_STOR_STR    format_name_array
#define DEFAULT_STOR_ORTHO  r_ZLib
#define DEFAULT_STOR_STR_ORTHO  format_name_zlib
#define DEFAULT_STOR_DEM    r_ZLib
#define DEFAULT_STOR_STR_DEM    format_name_zlib
#define DEFAULT_STOR_TK     r_ZLib
#define DEFAULT_STOR_STR_TK format_name_zlib
#define HELP_STOR   "<format> server storage format, one of: Array, ECW, Auto_Compression, ZLib, Pack_Bits, RLE, Sep_ZLib, Sep_RLE (see documentation for more choices)"

#define PARAM_STOR_P    "storageformatparams"
#define HELP_STOR_P "<params> parameters used for database storage (see documentation)"

#define HELP_TRANS  "<format> client/server transfer format, one of: Array, ECW, Auto_Compression, ZLib, Pack_Bits, RLE, Sep_ZLib, Sep_RLE (see documentation for more choices)"
#define HELP_TRANS_P    "<params> parameters used for transfer; see documentation"

#define PARAM_DATAFORMAT    "inputformat"
#define DEFAULT_DATAFORMAT_STR  format_name_tiff
#define DEFAULT_DATAFORMAT  r_TIFF
#define DEFAULT_DATAFORMAT_DEM_STR  format_name_dem
#define DEFAULT_DATAFORMAT_DEM  r_DEM
#define HELP_DATAFORMAT "input format, one of TIFF, PNG, BMP, JPEG, TOR, DEM, HDF, VFF"

#define PARAM_DATAFORMAT_P  "inputformatparams"
#define HELP_DATAFORMAT_P   "input file format parameters (see documentation)"

#define PARAM_INPAT "insidepattern"
#define HELP_INPAT  "<pattern> hex string defining pattern to fill interior of polygon. (*)"

#define PARAM_OUTPAT    "outsidepattern"
#define HELP_OUTPAT "<pattern> hex string defining pattern to fill exterior of polygon. (*)"

#define PARAM_BUF   "buffersize"
#define HELP_BUF    "<buffer-size> number of cells written in one go with --tiledupdate.  If --align is set then the minimum update size is the align value in each dimension. (*)"
#define DEFAULT_BUF      52428800

#define PARAM_COLLTYPE  "colltype"
#define HELP_COLLTYPE   "<type-name> collection type name; deprecated, will be ignored"

#define PARAM_V_FLAG    'v'
#define PARAM_VERBOSE   "verbose"
#define HELP_VERBOSE    "verbose log output"

#define PARAM_DEBUG "debug"
#define HELP_DEBUG  "print debugging information"

#define PARAM_LAYER "layerlist"
#define HELP_LAYER  "<list> file merge list, with bit layer position; format: 'layer_1:truePattern_1:falsePattern_1;layer_2:truePattern_2:falsePattern_2;...' where truePattern_i and falsePattern_i represent binary patterns expressed as decimal/hex/octal number"
#define LAYERITEM_SEPARATOR ':'
#define LAYER_WILDCARD  '%'
#define LAYERLIST_SEPARATOR ';'

#define DEFAULT_SKIP    0

// --- for coordtrans ------------------------------------------------------

#define PARAM_IN    "infmt"
#define HELP_IN     "<fmt> input format specifier"

#define PARAM_X_FLAG    'x'
#define PARAM_X     ""
#define HELP_X      "<coord-value> horizontal input coordinate component for transformation (float number)"

#define PARAM_Y_FLAG    'y'
#define PARAM_Y     ""
#define HELP_Y      "<coord-value> vertical input coordinate component for transformation (float number)"

#define PARAM_S_FLAG    's'
#define PARAM_S     "stripe"
#define HELP_S      "<s> GK stripe (mandatory if '--infmt gk' is specified"

#define PARAM_OUT   "outfmt"
#define HELP_OUT    "<fmt> output format specifier"

#define PARAM_TASPLIT   "tasplit"
#define HELP_TASPLIT    "split insert transaction by doing intermediate commits (decreases memory hunger)"

// --- for exporttif ------------------------------------------------------

#define FLAG_BG     'b'
#define PARAM_BG    "background"
#define HELP_BG     "<hexnum> background of TIFF file as 6-digit hexadecimal RGB value"
#define DEFAULT_BG  0xFFFFFF

#define PARAM_STREAMBUF "buffer"
#define HELP_STREAMBUF   "<n> streaming buffer size (in bytes)"
#define DEFAULT_STREAMBUF 5242880L

#define FLAG_SPECFILE   'i'
#define PARAM_SPECFILE  "specfile"
#define HELP_SPEC   "<filename> export specification file (mandatory); for a spec 'x.tif', file(s) 'xN.tif' will be generated with N running number"
#define DEFAULT_SPECFILE NULL

#define FLAG_POLYGON    'p'
#define PARAM_POLYGON   "polygon"
#define HELP_POLYGON    "<filename> polygon cutout file; pixels outside the polygon will be set to background value"

#define PARAM_TIFUNIT   "tifunit"
#define HELP_TIFUNIT    "<domain> edge size (in pixels) of an output mosaic; by default, a single image is generated"

#define FLAG_TIF    't'
#define PARAM_TIF   "tiffile"
#define HELP_TIF    "<filename> name of output TIFF file (mandatory)"

#define PARAM_TIFTYPE   "tiftype"
#define HELP_TIFTPYE    "<tiftype> TIFF image type, one of: binary, gray, rgb, 32bit"
#define DEFAULT_TIFTYPE_STR "rgb"
#define DEFAULT_TIFTYPE r_Convertor::ctype_rgb

#define PARAM_TIFCOMP   "tifcomp"
// see rasgeo/exporttools/hexcodec.cc, there admissible types are checked
#define HELP_TIFCOMP    "<comp-name> TIFF compression to be applied, one of: none, ccittrle (binary); none, lzw (gray); none, lzw (rgb)"

#define DEFAULT_TIFCOMP "none"

#define PARAM_TIFRESUNIT    "tifresunit"
#define HELP_TIFRESUNIT "<unit> TIFF file unit of resolution (mandatory)"

#define PARAM_TIFRESVAL "tifresval"
#define HELP_TIFRESVAL  "<n> TIFF file resolution (mandatory)"

#define PARAM_TIFFTAG   "tiftag"    // need to remain consistent with other tif flags...
#define HELP_TIFFTAG    "<k=t:v> set TIFF tag with numeric key k to value v of type t where t is one of: BYTE, ASCII, SHORT, LONG, RATIONAL, SBYTE, SSHORT, SLONG, SRATIONAL, FLOAT, DOUBLE."

#define FLAG_GEOTIFTAG  'g'
#define PARAM_GEOTIFTAG "geotiftag" // need to remain consistent with other tif flags...
#define HELP_GEOTIFTAG  "<xpixsize,yrot,xrot,ypixsize,xoffset,yoffset> TFW parameters (in TFW sequence), to be put into GeoTIFF tags."

// --- for importdem ------------------------------------------------------

#define PARAM_NOFLIPY   "noflipy"
#define HELP_NOFLIPY    "do not mirror y coordinates before inserting (default: flip vertically)"

// --- for exportdem ------------------------------------------------------

#define FLAG_DEMFILE    't'
#define PARAM_DEMFILE   "demfile"
#define HELP_DEMFILE    "<filename> name of DEM output file (mandatory)"
#define DEFAULT_DEMFILE NULL

#define FLAG_DEMPAR CommandLineParser::noShortName
#define PARAM_DEMPAR    "demparams"
#define HELP_DEMPAR "<dem-params> DEM parameter string, format: \"startx=<geocoord>,endx=<geocoord>,resx=<float>,starty=<geocoord>,endy=<geocoord>,resy=<float>\" (mandatory)"
#define DEFAULT_DEMPAR  NULL

#define FLAG_DEMMOS CommandLineParser::noShortName
#define PARAM_DEMMOS    "unitdem"
#define HELP_DEMMOS "<domain> mosaic domain of output DEM files (default: generate one file)"

// --- for importsegy ------------------------------------------------------

// tiling based on tile extent (used with 2-D rasgeo tools)
#define PARAM_TILING_3D     "tiling"
#define DEFAULT_TILING_3D   "\'[0:128,0:128,0:128]\'"
#define HELP_TILING_3D      "<tile-size> size of the tile to use, format: \'[0:x,0:y,0:z]\'"

// list SEGY header info
#define FLAG_LIST   'l'
#define PARAM_LIST  "list"
#define HELP_LIST   "list contents of the EBCDIC header, the binary header, all the trace headers; then quit"
#define PARAM_NOTRACEHEADERS "noTraceHeaders"
#define HELP_NOTRACEHEADERS  "If ping is present this will supress the output of the trace headers, otherwise it will be disregarded"


#endif // _RASGEO_GLOBALS_HH_

