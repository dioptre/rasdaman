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
/** ***********************************************************
 * <pre>
 *
 * PURPOSE:
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

package rasj.global;

public interface RasGlobalDefs
{
    static final String rcsid = "@(#)Package rasj, class RasGlobalDefs: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/global/RasGlobalDefs.java,v 1.23 2006/01/17 23:49:11 rasdev Exp $";

    // rasj version string (similar to executables startup message) -- can't we get rasdaman version info automatically?
    // static final String RASJ_VERSION = "rasj: rasdaman Java API. rasdaman v5.1revC -- generated Mit Jun 25 10:46:59 CEST 2003.";
    static final String RASJ_VERSION = "rasj: rasdaman Java API. rasdaman " + Version.VERSION + " -- generated " + Version.DATE + ".";

    // fastscale predefines; FIXME: should be used dynamically! -- PB 2003-jun-25
    static final int NUM_FAST_PRE_SCALE = 8;
    static final double[] FAST_PRE_SCALE_FACTORS = {1.0/2, 1.0/4, 1.0/8, 1.0/16, 1.0/32, 1.0/64, 1.0/128, 1.0/256};
    static final String[] FAST_PRE_SCALE_SUFFIXES = {"_1", "_2", "_3", "_4", "_5", "_6", "_7", "_8"};

    //RasDataFormat
    static final int RAS_ARRAY = 0;           // no compression, row-major memory representation
    static final int RAS_TIFF = RAS_ARRAY + 1;           // TIFF format
    static final int RAS_JPEG = RAS_TIFF + 1;             // JPEG format
    static final int RAS_HDF = RAS_JPEG + 1;              // HDF format
    static final int RAS_PNG = RAS_HDF + 1;               // PNG format
    static final int RAS_ZLIB = RAS_PNG + 1;              // ZLIB format
    static final int RAS_AUTO_COMPRESSION = RAS_ZLIB + 1; // automatic compression

    //RasTypeID
    static final int RAS_UNKNOWN = 0;
    static final int RAS_MARRAY = RAS_UNKNOWN + 1;
    static final int RAS_COLLECTION = RAS_MARRAY + 1;
    static final int RAS_SINTERVAL = RAS_COLLECTION + 1;
    static final int RAS_MINTERVAL = RAS_SINTERVAL + 1;
    static final int RAS_POINT = RAS_MINTERVAL + 1;
    static final int RAS_OID = RAS_POINT + 1;
    static final int RAS_STRUCTURE = RAS_OID + 1;
    static final int RAS_RGB = RAS_STRUCTURE + 1;
    static final int RAS_INT = RAS_RGB + 1;
    static final int RAS_SHORT = RAS_INT + 1;
    static final int RAS_USHORT = RAS_SHORT + 1;
    static final int RAS_LONG = RAS_USHORT + 1;
    static final int RAS_ULONG = RAS_LONG + 1;
    static final int RAS_FLOAT = RAS_ULONG + 1;
    static final int RAS_DOUBLE = RAS_FLOAT + 1;
    static final int RAS_BOOLEAN = RAS_DOUBLE + 1;
    static final int RAS_BYTE = RAS_BOOLEAN + 1;
    static final int RAS_CHAR = RAS_BYTE + 1;
    static final int RAS_STRING = RAS_CHAR + 1;
    static final int RAS_COMPLEX1 = RAS_STRING +1;


    //object status
    public static final int NO_STATUS = 0;
    public static final int DELETED   = NO_STATUS + 1;
    public static final int CREATED   = DELETED + 1;
    public static final int MODIFIED  = CREATED + 1;
    public static final int READ      = MODIFIED + 1;
    public static final int TRANSIENT = READ + 1;

    // java BaseType sizes
    public final int SIZE_OF_INTEGER = 4;
    public final int SIZE_OF_LONG = 8;
    public final int SIZE_OF_FLOAT = 4;
    public final int SIZE_OF_BYTE = 1;
    public final int SIZE_OF_SHORT = 2;
    public final int SIZE_OF_CHAR = 2;
    public final int SIZE_OF_BOOLEAN = 1;
    public final int SIZE_OF_DOUBLE = 8;

    // RasPrimitiveType sizes
    public final int SIZE_OF_RAS_LONG = 4;
    public final int SIZE_OF_RAS_ULONG = 4;
    public final int SIZE_OF_RAS_FLOAT = 4;
    public final int SIZE_OF_RAS_BYTE = 1;
    public final int SIZE_OF_RAS_SHORT = 2;
    public final int SIZE_OF_RAS_USHORT = 2;
    public final int SIZE_OF_RAS_CHAR = 1;
    public final int SIZE_OF_RAS_BOOLEAN = 1;
    public final int SIZE_OF_RAS_DOUBLE = 8;

    // error codes - these match the errtxts file
    final static int INTERNAL_DL_PARSE_EXCEPTION =  100;

    final static int RESULT_IS_NO_INTERVAL =  201;
    final static int INDEX_OUT_OF_BOUNDS   =  202;
    final static int DIMENSION_MISMATCH    =  203;
    final static int STREAM_INPUT_OVERFLOW =  204;
    final static int RESULT_IS_NO_CELL     =  205;
    final static int BASE_DBS_ERROR        =  206;
    final static int INTERNAL_CLIENT_ERROR =  207;
    final static int TYPE_INVALID          =  208;
    final static int TYPE_UNKNOWN          =  209;
    final static int TYPE_NOT_SUPPORTED    =  210;
    final static int DB_NOT_OPEN           =  211;
    final static int DB_CONN_FAILED        =  212;
    final static int URL_FORMAT_ERROR      =  213;
    final static int ILLEGAL_ULONG_VALUE   =  214;
    final static int ILLEGAL_USHORT_VALUE  =  215;

    final static int MANAGER_CONN_FAILED   =  800;
    final static int MANAGER_BUSY          =  801;
    final static int ACC_DEN_INC_LOGIN     =  802;
    final static int ACC_DEN_NO_PERMISSION =  803;
    final static int ACC_DEN_CAPABILITY_WRONG = 804;
    final static int NO_ACTIVE_SERVERS        = 805;
    final static int WRITE_TRANS_IN_PROGRESS  = 806;
    final static int DATABASE_NOT_DEFINED     = 807;
    final static int REQUEST_FORMAT_ERROR     = 808;
    
    final static int INVALID_OBJECT_NAME_ERROR  = 1014;

    // time constants for retries in RasODMGImplementation.getFreeServer():
    // if we sum up we achieve a total of (.2 + .4 +...+ 3.2) = 6.3 secs maximum wait time. that looks ok.
    final static int MAX_GETFREESERVER_ATTEMPTS   =   5;	// # of tries in total until giving up on current request
    final static int GETFREESERVER_WAIT_INITIAL   = 100;	// first waiting period
    final static int GETFREESERVER_WAIT_INCREMENT =   2;	// multiplication factor for subsequent wait cycles

    // server connection constants
    final static int RASMGRPORT_DEFAULT = 7001;			// default port used to caontact rasmgr
    final static String GUESTIDENT_DEFAULT = "rasguest:8e70a429be359b6dace8b5b2500dedb0";
								// default read-only user

    public final static String KEYWORD_TAB     = "\t";
    public final static String KEYWORD_CLASS   = "$class";
    public final static String KEYWORD_METHOD  = "$method";
    public final static String KEYWORD_CODE    = "$code";
    public final static String KEYWORD_URL     = "$url";
    public final static String KEYWORD_DIM1    = "$dim1";
    public final static String KEYWORD_DIM2    = "$dim2";
    public final static String KEYWORD_VAL     = "$val";
    public final static String KEYWORD_LOW     = "$low";
    public final static String KEYWORD_HIGH    = "$high";
    public final static String KEYWORD_INDEX   = "$index";
    public final static String KEYWORD_INVNAME = "$InvName";
    public final static String KEYWORD_TOKEN   = "$token";
    public final static String KEYWORD_ERRNO   = "$errorNo";
    public final static String KEYWORD_LINENO  = "$lineNo";
    public final static String KEYWORD_COLNO   = "$columnNo";
    public final static String KEYWORD_ATYPE   = "$aType";
    public final static String KEYWORD_BTYPE   = "$bType";
    public final static String KEYWORD_TYPE    = "$type";
}
