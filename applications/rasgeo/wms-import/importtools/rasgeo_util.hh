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

#ifndef _RASGEO_UTIL_HH_
#define _RASGEO_UTIL_HH_

class r_Tiling;

// fopen, getc
#include <stdio.h>
#include <list>
#include <exception>

#include "raslib/mddtypes.hh"
#include "raslib/minterval.hh"
#include "raslib/scalar.hh"
#include "raslib/odmgtypes.hh"
#include "raslib/oid.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/set.hh"
#include "rasodmg/gmarray.hh"
#include "rasodmg/polycutout.hh"
#include "rasodmg/marray.hh"
#include "cmlparser.hh"

#include "import_error.hh"
#include "rasgeo_globals.hh"

/// sends a char representation of the 2D interval as stream
#define MINTERVAL_STREAM(myInt) \
	   "[" << (int) myInt[0].low() << ":" << (int) myInt[0].high() \
	<< "," << (int) myInt[1].low() << ":" << (int) myInt[1].high() << "]"

/// indicator to make RasgeoUtil know what kind of import is going on
typedef enum
{
	IMPORTTYPE_UNDEF,
	IMPORTTYPE_ORTHO,
	IMPORTTYPE_DEM,
	IMPORTTYPE_TK
} ImportType;


/// parameter evaluation procedure, to be provided by each tool individually
extern void parseParams(int argc, char** argv) throw (ImportError, r_Error);

class RasgeoUtil
{
	public:
		static void doStuff(int argc, char** argv, ImportType iType ) throw(ImportError, r_Error);
		static void doImport(const char* Query, const char* TransparentSelectQuery, r_Marray_Type*& mddType, r_Ref<r_GMarray>& fileMDD ) throw(ImportError, r_Error);

		static r_Marray_Type *getTypeFromDatabase( const char *mddTypeName ) throw(ImportError, r_Error);

		static char* getFile(const char *inputFile, size_t& dtaSize ) throw (ImportError);

		static void convertTo(r_Data_Format fmt, char*& src, size_t& size, r_Minterval& interv, r_Base_Type*& tp, const char* options) throw (ImportError);

		static void convertFrom(r_Data_Format fmt, char*& src, size_t& size, r_Minterval& interv, r_Base_Type*& tp, const char* options) throw (ImportError);

		static void readScaleLevels(const char* argv) throw (ImportError);

		static void initGMarray( r_Ref<r_GMarray>& tempMDD, const char *inputFile, r_Data_Format conversionFormat, r_Minterval& tempDataDomain, r_Marray_Type*& mddType, r_Tiling*& myTiling, const char* conversionParams) throw (r_Error, ImportError);

		static void overlayGMarrays( r_Ref<r_GMarray>& target, const r_Ref<r_GMarray>& replaceBlackMDD, const r_Ref<r_GMarray>& backgroundMDD) throw (ImportError);

		static void compareGMarrays(const r_Ref<r_GMarray>& baseMDD, r_Ref<r_GMarray>& topMDD) throw (ImportError);

		static void openDatabase() throw (r_Error);
		static void closeDatabase() throw (r_Error);

		static void openTransaction(bool readwrite = true) throw (r_Error);
		static void closeTransaction( bool doCommit ) throw (r_Error);

		static void compareScaledMDD(const r_Ref<r_GMarray>& baseMDD, const r_Minterval& clipDom, const r_Minterval& downScaledDomain, unsigned int length, const char* collectionName) throw (r_Error, ImportError);

		static void updateScaledMDD(const r_Ref<r_GMarray>& baseMDD, const r_Minterval& clipDom, const r_Minterval& downScaledDomain, unsigned int length, const char* collectionName) throw (r_Error, ImportError);

		static void scaleDomain(const r_Minterval& baseDomain, const r_Point& origin, double factor, r_Minterval& scaledDomain, r_Minterval& clipDomain, unsigned int& length, bool& underflow) throw (ImportError);

		static void saveData(const char* fileNamePat, const char* data, r_Bytes length, const r_Minterval& mddDomain) throw (ImportError);

		static const string generateImportQuery( const char *c, r_Minterval ovl, int useOId, r_OId oid ) throw(ImportError, r_Error);
		static const string generateSelectQuery( const char *c, r_Minterval ovl ) throw(ImportError, r_Error);

		static void executeQuery( r_OQL_Query& query, r_Set< r_Ref_Any > &result ) throw (r_Error, ImportError);
		static void executeQuery( r_OQL_Query& query ) throw (r_Error, ImportError);

		static void fast_scale_process_primitive_type(const r_Primitive_Type *primType, char *dest, const char *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &iterDom, unsigned int type_len, unsigned int length, r_Scale_Function func) throw(ImportError);

		static void fast_scale_process_structured_type(const r_Structure_Type *primType, char *dest, const char *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &iterDom, unsigned int type_len, unsigned int length, r_Scale_Function func) throw (ImportError);

		template<class T>
		static void fast_scale_resample_array(T *dest, const T *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &iterDom, unsigned int type_len, unsigned int length, bool round) throw (ImportError);

		template<class T>
		static void fast_scale_aggregate_array(T *dest, const T *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &iterDom, unsigned int type_len, unsigned int length) throw (ImportError);

		/// check if string passed is a valid rasdasman collection name
		static bool allowedCollNameChar( const char* collName );

		static const char* serverName;
		static r_ULong serverPort;		
		static const char* baseName;
		static const char* userName;
		static const char* passwd;
		static const char* conversionTypeName;
		static r_Data_Format inputFormat;
		static const char* inputFormatParams;
		static r_Data_Format transferFormat;
		static const char* transferFormatParams;
		static r_Data_Format storageFormat;
		static const char* storageFormatParams;
		static const char* collName;
		static const char* setTypeName;
		static const char* mddTypeName;
		static r_OId mddOId;
		static bool mddOIdDef;
		static const char* fileName;
		static r_Database db;
		static r_Transaction ta;
		static r_Minterval mddDomain;
		static bool mddDomainDef;
		static bool testBed;
		static r_Tiling* theTiling;
		static bool polygonDefined;
		static r_PolygonCutOut polygon;
		static int polygonShrinker;
		static bool transparent;
		static string outsidePatternSel;
		static bool outsidePatternSelDef;
		static string insidePatternSel;
		static bool insidePatternSelDef;
		static string outsidePattern;
		static bool outsidePatternDef;
		static string insidePattern;
		static bool insidePatternDef;
		static const int queryBufferLength;
		static std::list<std::pair<double, char*> >* scaleLevels;
		static int wrongBytes;
		static r_Scale_Function scaleFunction;
		static const string tilingDesc;
		static const string tilingParamsDesc;
		static size_t updateBufferSize;
		static const char* defaultImportBufferSize;
		static std::list<char*> *layerList;
		static unsigned int cellSize;
		static std::list<unsigned int> *patternsTrue;
		static std::list<unsigned int> *patternsFalse;
		static r_Minterval overlayDomain;
		static bool overlayDomainDef;
		static r_Range align;
		static bool tiledImport;
		static bool handleSignal;
		static bool debug;
		static double xmin, xmax, ymin, ymax;
		static double xres, yres;
		static unsigned int skipLines;
		static bool fileIsStdin;
		static bool noFlipY;
		static bool taSplit;
		static bool verbose;

	private:
		/// is database currently open?
		static bool dbIsOpen;

		/// is a transaction currently open?
		static bool taIsOpen;

}; // RasgeoUtil

#endif // _RASGEO_UTIL_HH_
