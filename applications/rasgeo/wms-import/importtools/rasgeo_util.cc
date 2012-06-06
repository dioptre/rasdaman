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
 * SOURCE: rasgeo_util.cc
 *
 * MODULE: rasgeo
 *
 * PURPOSE:
 *
 * CHANGE HISTORY (append further entries):
 * when         who         what
 * ----------------------------------------------------------
 * 2003-jul-02  PB          created
 * 2004-mar-17  PB          more differentiated error codes for DOMAINDATAMISMATCH
 * 2004-jul-16  PB          changed obsolete occurrences of "ImportOrtho" to "RasgeoUtil"
 * 2005-feb-13  PB          floor(uint) -> floorl(uint) for gcc3
 * 2005-sep-07  PB          conditional log output
 * 2006-jan-17  PB          DEFAULT_DB -> DEFAULT_DBNAME
 *
 * COMMENTS:
 * - built from old rasodmg/test/system_update
 * - generalized to be used by (at least) all import tools
 *
 * BUGS:
 * - --aling without --tiledupdate goes wild
 * - drop mddtype, deduce from collection type
 * - add new option --transpcolor 0xXXXXXX to set transparency
 ************************************************************/

static const char rcsid[] = "@(#)rasgeo/rasgeo_util,RasgeoUtil: $Id: rasgeo_util.cc,v 1.8 2006/01/17 09:18:33 rasdev Exp $";

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include <string>
#include <fstream>
#include <unistd.h>
#include <math.h>

#ifdef SOLARIS
#include <strings.h>
#endif

// this must use the rasdaman internal module
// as there is no official type retrieval interface yet,
// access to private function Database::getDComm() is required
#include "raslib/odmgtypes.hh"

#include "raslib/rminit.hh"
#include "raslib/mitera.hh"
#include "raslib/miterd.hh"
#include "raslib/type.hh"
#include "raslib/structure.hh"
#include "raslib/structuretype.hh"
#include "raslib/marraytype.hh"
#include "raslib/minterval.hh"
#include "raslib/primitivetype.hh"
#include "raslib/storageman.hh"
#include "raslib/mddtypes.hh"

#include "rasodmg/database.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/set.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/partinsert.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/gmarray.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/dirtiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "rasodmg/interesttiling.hh"
#include "rasodmg/alignedtiling.hh"
#include "rasodmg/stattiling.hh"
#include "rasodmg/oqlquery.hh"
#include "rasodmg/polygon.hh"
#include "rasodmg/polycutout.hh"

#include "conversion/convfactory.hh"
#include "conversion/convertor.hh"


#include "hexcodec.hh"
#include "rasgeo_signal.hh"
#include "rasgeo_util.hh"


// debug facility; relies on -DDEBUG at compile time
#include "debug/debug.hh"


/// parameter evaluation procedure, to be provided by each tool individually
extern void parseParams(int argc, char** argv) throw (ImportError, r_Error);



/*
 * member variables --------------------------------------------------
 */

// set by doStuff(), information provided through main() callers
ImportType importType = IMPORTTYPE_UNDEF;

const char* RasgeoUtil::serverName = DEFAULT_SERV;

r_ULong RasgeoUtil::serverPort = DEFAULT_PORT;

const char* RasgeoUtil::baseName = DEFAULT_DBNAME;

const char* RasgeoUtil::userName = DEFAULT_USER;

const char* RasgeoUtil::passwd = DEFAULT_PASSWD;

r_Data_Format RasgeoUtil::inputFormat = DEFAULT_DATAFORMAT;
const char* RasgeoUtil::inputFormatParams = NULL;

r_Data_Format RasgeoUtil::transferFormat = DEFAULT_TRANS;
const char* RasgeoUtil::transferFormatParams = NULL;

r_Data_Format RasgeoUtil::storageFormat = DEFAULT_STOR;
const char* RasgeoUtil::storageFormatParams = NULL;

const char* RasgeoUtil::collName = NULL;

const char* RasgeoUtil::setTypeName = NULL;

const char* RasgeoUtil::mddTypeName = NULL;

r_OId RasgeoUtil::mddOId;
bool RasgeoUtil::mddOIdDef = false;

const char* RasgeoUtil::fileName = NULL;
bool RasgeoUtil::fileIsStdin = false;

bool RasgeoUtil::noFlipY = false;

r_Database RasgeoUtil::db;

r_Transaction RasgeoUtil::ta;

r_Minterval RasgeoUtil::mddDomain;
bool RasgeoUtil::mddDomainDef = false;

const char *RasgeoUtil::conversionTypeName = "char";

bool RasgeoUtil::polygonDefined = false;
r_PolygonCutOut RasgeoUtil::polygon;

int
RasgeoUtil::polygonShrinker = DEFAULT_SHRINK;

bool
RasgeoUtil::transparent = false;

string
RasgeoUtil::outsidePatternSel;

bool
RasgeoUtil::outsidePatternSelDef = false;

string
RasgeoUtil::insidePatternSel;

bool
RasgeoUtil::insidePatternSelDef = false;

string
RasgeoUtil::outsidePattern;

bool
RasgeoUtil::outsidePatternDef = false;

string RasgeoUtil::insidePattern;

bool RasgeoUtil::insidePatternDef = false;

const int RasgeoUtil::queryBufferLength = 512;

std::list<std::pair<double, char*> >* RasgeoUtil::scaleLevels = NULL;

int RasgeoUtil::wrongBytes = 0;

r_Scale_Function RasgeoUtil::scaleFunction = r_SubSampling;

size_t RasgeoUtil::updateBufferSize = DEFAULT_BUF;

std::list<unsigned int>* RasgeoUtil::patternsTrue;

std::list<unsigned int>* RasgeoUtil::patternsFalse;

std::list<char*>* RasgeoUtil::layerList = NULL;
unsigned int RasgeoUtil::cellSize = 0;

bool RasgeoUtil::overlayDomainDef = false;

r_Minterval RasgeoUtil::overlayDomain;

r_Range RasgeoUtil::align = 0;

bool RasgeoUtil::tiledImport = false;

// tiling policy
r_Tiling* RasgeoUtil::theTiling = NULL;


/// if true: perform signal handling; if false; ignore signals
bool RasgeoUtil::handleSignal = true;

/// if true: log all queries to stdout
bool RasgeoUtil::debug = false;

/// is db currently open?
bool
RasgeoUtil::dbIsOpen = false;

/// is ta currently open?
bool
RasgeoUtil::taIsOpen = false;

///geographic bbox coordinates
double RasgeoUtil::xmin, RasgeoUtil::xmax, RasgeoUtil::ymin, RasgeoUtil::ymax; 

/// resolution
double RasgeoUtil::xres, RasgeoUtil::yres;

unsigned int RasgeoUtil::skipLines = DEFAULT_SKIP;

/// shall we split the image insert transaction (i.e., commit after every stripe) ?
bool RasgeoUtil::taSplit = false;

/// verbose log output?
bool RasgeoUtil::verbose = false;

// end member variables --------------------------------------------------

// conditional log output
// NB: beware when using in if stmts, embrace it in a {...} block!
#define LOG	if (verbose) cout


// this is not used, we don't write files, but do database updates
#if NOT_USED__
void
RasgeoUtil::saveData(const char* fileNamePat, const char* data, r_Bytes length, const r_Minterval& dom) throw (ImportError)
{
        ENTER( "saveData, fileNamePat=" << fileNamePat << ", data=" << data << ", length=" << length << ", dom=" << dom );

	r_Primitive_Type* tp = new r_Primitive_Type("Char", r_Type::CHAR);
	r_Convertor* conv = r_Convertor_Factory::create(r_PNG, data, dom, tp);
	r_convDesc desc = conv->convertTo(NULL);
	size_t dtaSize=desc.destInterv.cell_count()*tp->size();
	std::ofstream o;

	o.open(fileNamePat);
	if (!o.is_open())
	{
		TALK( "saveData: unable to open file for writing: " << fileNamePat ) ;
		throw ImportError( FILEINACCESSIBLE );
	}
	o.write(desc.dest, dtaSize);
	o.flush();
	o.close();

	free(desc.dest);
	delete desc.destType;
	delete conv;
	delete tp;

        LEAVE( "saveData" );
	return;
}
#endif // NOT_USED__


void
RasgeoUtil::openDatabase() throw (r_Error)
{
        ENTER( "openDatabase" );

	if (! dbIsOpen)
	{
		LOG << "opening database " << baseName << " at " << serverName << ":" << serverPort << "..." << flush;
        	db.set_servername(serverName, serverPort);
        	db.set_useridentification(userName, passwd);
        	TALK( "database was closed, opening database=" << baseName << ", server=" << serverName << ", port=" << serverPort << ", user=" << userName << ", passwd=" << passwd << "..." );
        	db.open(baseName);
		TALK( "done" );
		dbIsOpen = true;
		LOG << "ok" << endl << flush;
	}

        LEAVE( "openDatabase" );
}

void
RasgeoUtil::closeDatabase() throw (r_Error)
{
        ENTER( "closeDatabase" );

	if (dbIsOpen)
	{
        	TALK( "database was open, closing it" );
		db.close();
		dbIsOpen = false;
	}

        LEAVE( "closeDatabase" );
        return;
}

void
RasgeoUtil::openTransaction(bool readwrite) throw (r_Error)
{
        ENTER( "openTransaction, readwrite=" << (readwrite ? "rw" : "ro" ) );

	if (! taIsOpen)
	{
		if (readwrite)
		{
			TALK( "transaction was closed, opening rw..." );
			ta.begin(r_Transaction::read_write);
			TALK( "done" );
		}
		else
		{
			TALK( "transaction was closed, opening ro..." );
			ta.begin(r_Transaction::read_only);
			TALK( "done" );
		}

		TALK( "setting transfer format, format=" << transferFormat << ", params=" << (transferFormatParams?transferFormatParams:"(null)") << "..." );
		db.set_transfer_format(transferFormat, transferFormatParams);
		TALK( "done" );

		TALK( "setting storage format, format=" << storageFormat << ", params=" << (storageFormatParams?storageFormatParams:"(null)") << "..." );
		db.set_storage_format(storageFormat, storageFormatParams);
		TALK( "done" );

		taIsOpen = true;
	}

        LEAVE( "openTransaction" );
}

void 
RasgeoUtil::closeTransaction( bool doCommit ) throw (r_Error)
{
        ENTER( "closeTransaction, doCommit=" << doCommit );

	if (taIsOpen)
	{
		if (doCommit)
		{
			TALK( "transaction was open, committing it..." );
			ta.commit();
			TALK( "done" );
		}
		else
		{
			TALK( "transaction was open, aborting it..." );
			ta.abort();
			TALK( "done" );
		}
		taIsOpen = false;
	}

        LEAVE( "closeTransaction" );
	return;
}


/// convert byte array src, assuming data format fmt with options options
/// old src memory is freed
/// returns:
///	src	converted data
///	dtaSize	number of bytes converted
///	interv	extent of result object 
///	tp	base type recognized
void
RasgeoUtil::convertFrom(r_Data_Format fmt, char*& src, size_t& dtaSize, r_Minterval& interv, r_Base_Type*& tp, const char* options) throw (ImportError)
{
        ENTER( "convertFrom, fmt=" << fmt << ", size=" << dtaSize << ", interv=" << interv << ", baseType=" << tp << ", options=" << options );

	r_Minterval tmpInt(1);
	tmpInt << r_Sinterval((r_Range)0, (r_Range)dtaSize - 1);
	
	TALK( "convertFrom: domain in: " << tmpInt << ", type in: " );
#ifdef DEBUG
	if (debug)
	{
		tp->print_status(cout);
		cout << endl;
	}
#endif

	if (fmt == r_Array)
        {
		if (dtaSize != (tp->size() * interv.cell_count()))
		{
			TALK( "base type size " << tp->size() << " cells " << interv.cell_count() << " does not match size " << dtaSize );
			throw ImportError( BASETYPEMISMATCH );
		}
	}

	// create a new convertor using the factory
	r_Convertor* conv = NULL;
	try
	{
		conv = r_Convertor_Factory::create(fmt, src, tmpInt, tp);
	}
	catch (r_Error& e)
	{
		TALK( "convertor factory error: " << e.what() );
		// FIXME: that's a corageous guess:
		throw ImportError( CONVERSIONNOTSUPPORTED );
	}

	// check whether format requested can be served
	if ( ! r_Convertor_Factory::is_supported( fmt ) )
		throw ImportError( CONVERSIONNOTSUPPORTED );

	// set details of convertor
	r_Storage_Man_CPP mySM;
	conv->set_storage_handler(mySM);

	// now do conversion
	r_convDesc desc;
	try
	{
		desc = conv->convertFrom(options);
	}
	catch(...)
	{
		TALK( "convertor failed." );
		throw ImportError( CONVERSIONEXCEPTION );
	}

	// verify: result type is a base type? (not set etc.)
	if ( ! desc.destType->isBaseType())
		throw ImportError( CONVERSIONRETURNEDWRONGTYPE );

	// set type to what the convertor has recognized
	if (tp != NULL)
		delete tp;
	tp = (r_Base_Type*)desc.destType;

	// record number of bytes processed
	tmpInt = desc.destInterv;
	dtaSize=tmpInt.cell_count()*tp->size();

	// unlink source data, set ptr to result data
	if (src != NULL)
		delete [] src;
	src = desc.dest;

	if (conv != NULL)
	{
		delete conv;
		conv = NULL;
	}

	// verification: compare expected data volume against actually processed data
	if (interv.dimension() == 0)
		interv = tmpInt;
	else
	{
		if (tmpInt.cell_count() != interv.cell_count())
		{
			TALK( "Domains do not have the same number of cells: mdd " << interv << ", data " << tmpInt );
			throw ImportError( DOMAINDATAMISMATCH );
		}
		if (tmpInt.get_extent() != interv.get_extent())
		{
			TALK( "Domains do not have the same extents:mdd " << interv << ", data " << tmpInt );
			throw ImportError( DOMAINEXTENTMISMATCH );
		}
	}

	TALK( "convertFrom: domain out: " << interv << ", type out: " );
#ifdef DEBUG
	if (debug)
		tp->print_status(cout);		
#endif
	TALK( "" );
	
        LEAVE( "convertFrom" );
	return;
}

void
RasgeoUtil::convertTo(r_Data_Format fmt, char*& src, size_t& dtaSize, r_Minterval& interv, r_Base_Type*& tp, const char* options) throw (ImportError)
{
	ENTER( "convertTo, fmt=" << fmt << ", size=" << dtaSize << ", interv=" << interv << ", baseType=" << tp << ", options=" << options );
	TALK( "convertTo: domain in: " << interv << ", type in: " );
#ifdef DEBUG
	if (debug)
		tp->print_status( cout );
#endif
	TALK( "" );
	
	if (fmt == r_Array)
	{
		if (dtaSize != (tp->size() * interv.cell_count()))
		{
			TALK( "base type size " << tp->size() << " cells " << interv.cell_count() << " does not match size " << dtaSize );
			throw ImportError( BASETYPEMISMATCH );
		}
	}

	r_Convertor* conv = r_Convertor_Factory::create(fmt, src, interv, tp);
	r_convDesc desc = conv->convertTo(options);
	TALK( "Conversion type          : " );
#ifdef DEBUG
	if (debug)
		desc.destType->print_status( cout );
#endif
	TALK( "" );
	if ( ! desc.destType->isBaseType())
		throw ImportError( CONVERSIONEXCEPTION );

	delete tp;
	tp = (r_Base_Type*)desc.destType;
	dtaSize = desc.destInterv.cell_count() * tp->size(); // desc.destInterv[0].high() - desc.destInterv[0].low() + 1;
	interv=desc.destInterv;
	delete src;
	src = desc.dest;
	delete conv;
	conv = NULL;

	TALK( "domain out: " << interv << ", type out: " );
#ifdef DEBUG
	if (debug)
		tp->print_status( cout );
#endif
	TALK( "" );

	LEAVE( "convertTo" );
	return;
}

/// read file fileName into newly allocated buffer, return buffer ptr and its size (dtaSize)
char*
RasgeoUtil::getFile( const char *inputFile, size_t& dtaSize ) throw (ImportError)
{
	ENTER( "getData, inputFile=" << inputFile );
	char* dta = 0;
	size_t size = 0;

	FILE* fileD = fopen(inputFile, "r");
	if (fileD == NULL)
		throw ImportError( FILEINACCESSIBLE );

	fseek(fileD, 0, SEEK_END);
	size = ftell(fileD);

	try
	{
	   dta = new char[size];
	}
	catch(std::bad_alloc)
	{	
	   TALK( "Unable to claim memory: " << size << " Bytes" );  
	   throw ImportError( UNABLETOCLAIMRESOURCEFORFILE );
	}
	
	fseek(fileD, 0, SEEK_SET);
	fread(dta, 1, size, fileD);
	fclose(fileD);

	dtaSize = size;

	LEAVE( "getData, size=" << dtaSize );
	return dta;
}
	
void
RasgeoUtil::readScaleLevels(const char* startPos) throw (ImportError)
{
	ENTER( "readScaleLevels, startPos=" << startPos );

	scaleLevels = new std::list<std::pair<double, char*> >();
	const char* endPos = NULL;
	char* levelName = NULL;
	const char* factorName = NULL;
	double factor = 0;
	bool found = false;
	size_t length = 0;
	int i = 0;

	do
	{
		// TALK( "startpos=" << startPos << ", endpos=" << endPos << ", factor=" << factor );

		endPos = index(startPos, SCALEITEM_SEPARATOR );
		if (endPos == NULL)
		{
			delete scaleLevels;
			scaleLevels = NULL;
			throw ImportError( SCALELEVELSINCORRECT );
		}
		length = endPos - startPos;
		levelName = new char[length + 1];
		memset(levelName, 0, length + 1);
		strncpy(levelName, startPos, length);
		startPos = endPos + 1;
		factor = atof(startPos);

		// plausi check for each layer: name not empty, name only allowed chars, factor > 1.0
		if ( length == 0 )
		{
			TALK( "empty collectionname for factor " << factor );
			throw ImportError( INVALIDSCALENAME );
		}
		if ( ! allowedCollNameChar( levelName ) )
			throw ImportError( INVALIDCOLLNAMECHAR );
		if (factor < 1)
		{
			TALK( "illegal scale factor for collectionname " << levelName );
			delete scaleLevels;
			scaleLevels = NULL;
			throw ImportError( INVALIDSCALEFACTOR );
		}

		TALK( "Scale level " << i << ": " << levelName << ", factor=" << factor );

		factor = 1 / factor;
		scaleLevels->push_back(std::pair<double, char*>(factor, levelName));

		endPos = index(startPos, SCALELEVELS_SEPARATOR );
		startPos = endPos + 1;
		i++;
	} while ( endPos != NULL );

	LEAVE( "readScaleLevels" );
	return;
}

/// configure MDD appropriately to hold data, read data from file into this MDD, perform necessary format conversion
/// (DOP and DEM variant)
void
RasgeoUtil::initGMarray(r_Ref<r_GMarray>& tempMDD, const char *inputFile, r_Data_Format conversionFormat, r_Minterval& tempDataDomain, r_Marray_Type*& mddType, r_Tiling*& myTiling, const char* conversionParams) throw (r_Error, ImportError)
{
	ENTER( "initGMarray, conversionFormat=" << conversionFormat << ", conversionParams=" << conversionParams );

	char* mddData = NULL;
	size_t mddDataSize = 0;
	r_Type* tempType = NULL;
	r_Base_Type* conversionType = NULL;

	// --- preparatory checks -----------------------------------------------------
#if 0 // now explicitly set later
	const r_Base_Type* tempBaseType = (r_Base_Type*)tempMDD->get_base_type_schema();
	if (tempBaseType == NULL)
		throw ImportError( MDDTYPEOFGMARRAYNOTINITIALISED );
#endif
	// find out base type structure according to conversion type name specified
	try
	{
		TALK( "looking up type name " << conversionTypeName << "..." );
		tempType = r_Type::get_any_type(conversionTypeName);
		TALK( "done" );
	}
	catch (const r_Error& err)
	{
		TALK( "Resolving conversion type: " << err.get_errorno() << " " << err.what() );
		delete tempType;
		tempType = NULL;
		throw ImportError( CONVERSIONTYPENOTABASETYPE );
	}

	// this must be a valid _base_ type
	if (!tempType->isBaseType())
	{
		delete tempType;
		tempType = NULL;
		throw ImportError( CONVERSIONTYPENOTABASETYPE );
	}

	// got it, this will be our type
	conversionType = (r_Base_Type*)tempType;
	TALK( "determined conversion type; size is " << conversionType->size() );

	// --- now start inspecting and processing file ---------------------------
	mddData = getFile(inputFile, mddDataSize );
	TALK( "file read done, got " << mddDataSize << " bytes" );

	// --- do conversion ------------------------------------------------------
	convertFrom(conversionFormat, mddData, mddDataSize, tempDataDomain, conversionType, conversionParams);
	// DEM: convertFrom(r_DEM, s.c_str(), srcData, srcDom, srcType, descSrc);

	// --- prepare MDD --------------------------------------------------------
	size_t baseTypeLength = mddType->base_type().size();
	r_Storage_Layout* stl = new r_Storage_Layout( myTiling->clone() );

	// don't take name from here - often it is an anonymous type!
	// mddTypeName = mddType->name();

        TALK( "preparing MDD; domain=" << tempDataDomain << ", MDD type name=" << mddTypeName << ", base type length=" << baseTypeLength );
	tempMDD = new (mddTypeName)r_GMarray( tempDataDomain, baseTypeLength, stl);

	tempMDD->set_type_schema(mddType);
	tempMDD->set_array_size(mddDataSize);
	tempMDD->set_type_length(conversionType->size());
	// free temp space 
	delete [] tempMDD->get_array();
	// set MDD to new contents
	tempMDD->set_array(mddData);
	// clear ptr
	mddData = 0;

	LEAVE( "initGMarray" );
	return;
}


void
RasgeoUtil::compareGMarrays(const r_Ref<r_GMarray>& baseMDD, r_Ref<r_GMarray>& topMDD) throw (ImportError)
{
	ENTER( "compareGMarrays" );

	//r_GMarray* baseMDD = (r_GMarray*)&(*baseMDD2);
	const r_Minterval& tempDomain = baseMDD->spatial_domain();
	char* topMDDCells = topMDD->get_array();
	const char* baseMDDCells = baseMDD->get_array();
	wrongBytes = 0;
	
	TALK( "baseMDD domain: " << baseMDD->spatial_domain() << " type length: " << baseMDD->get_type_length() );
	TALK( "topMDD  domain: " << topMDD->spatial_domain() << " type length: " << topMDD->get_type_length() );

	if (tempDomain != topMDD->spatial_domain())
		throw ImportError( OVERLAYDOMAINSDONOTMATCH );

	r_Bytes typeLen = baseMDD->get_type_length();
	if (typeLen != topMDD->get_type_length())
		throw ImportError( OVERLAYTYPESIZESDONOTMATCH );

	r_Area numberCells = tempDomain.cell_count();
	for (size_t elemNum = 0; elemNum < numberCells; elemNum++)
	{
		if (memcmp(&(baseMDDCells[elemNum * typeLen]), &(topMDDCells[elemNum * typeLen]), typeLen) != 0)
			wrongBytes++;
	}
	wrongBytes*=typeLen;	

	if (wrongBytes != 0)
		throw ImportError( GMARRAYSARENOTEQUAL );

	LEAVE( "compareGMarrays" );
	return;
}

/// fill targetMDD by overlaying foregroundMDD over backgroundMDD (respecting transparency, if specified)
void
RasgeoUtil::overlayGMarrays(r_Ref<r_GMarray>& targetMDD, const r_Ref<r_GMarray>& foregroundMDD, const r_Ref<r_GMarray>& backgroundMDD) throw (ImportError)
{
	ENTER( "overlayGMarrays" );

	// aux var: domain (extent) of each MDD
	const r_Minterval& backgroundDomain = backgroundMDD->spatial_domain();
	const r_Minterval& foregroundDomain = foregroundMDD->spatial_domain();
	const r_Minterval& targetDomain = targetMDD->spatial_domain();

	// aux var: ptr to pixel array
	char* foregroundMDDCells = (char*)foregroundMDD->get_array();
	char* backgroundMDDCells = (char*)backgroundMDD->get_array();
	char* targetMDDCells     = (char*)targetMDD->get_array();

	TALK( "background MDD domain: " << backgroundMDD->spatial_domain() << " type length: " << backgroundMDD->get_type_length() );
	TALK( "foreground MDD domain: " << foregroundMDD->spatial_domain() << " type length: " << foregroundMDD->get_type_length() );
	TALK( "target MDD domain    : " << targetMDD->spatial_domain() << " type length: " << foregroundMDD->get_type_length() );

	// check match of pixel types
	r_Bytes typeLen = backgroundMDD->get_type_length();
	if ( ! ( (typeLen == foregroundMDD->get_type_length()) && (typeLen == targetMDD->get_type_length())) )
	{
		TALK( "type size mismatch: background has " << typeLen << ", background has " << foregroundMDD->get_type_length() << ", target has " << targetMDD->get_type_length() );
		throw ImportError( OVERLAYTYPESIZESDONOTMATCH );
	}

	// allocate reference pixel for transparent color, clear it to black
	// FIXME: allow additional cmd line parameter to determine what "transparent" means
	//	eg: "--transparent 0x000000" for RGB black
	char* reference = new char[typeLen];
	memset(reference, 0, typeLen);

	// background and target extent match?
	if (backgroundDomain.get_extent() == targetDomain.get_extent())
	{
		// ...and also match foreground?
		if (backgroundDomain.get_extent() == foregroundDomain.get_extent())
		{	// background and target and foreground cover same region
			TALK( "extents match, overlay using fast method" );

			// iterate over all cells
			// the ptr below are an optimization to do 1 inc instead of 3 mult + 3 add
			char *currentForegroundCell = foregroundMDDCells;
			char *currentBackgroundCell = backgroundMDDCells;
			char *currentTargetCell     = targetMDDCells;
			for (size_t elemNum = 0; elemNum < backgroundDomain.cell_count(); elemNum++)
			{
				// is fg pixel transparent?
				if (memcmp( (void*)reference,  (void*)currentForegroundCell, typeLen) == 0)
				{	// yes -> background shines through, copy it
					memcpy( (void*)currentTargetCell,  (void*)currentBackgroundCell, typeLen);
				}
				else
				{	// no -> background covered, take foreground pixel
					memcpy( (void*)currentTargetCell,  (void*)currentForegroundCell, typeLen);
				}
				currentForegroundCell   += typeLen;
				currentBackgroundCell   += typeLen;
				currentTargetCell       += typeLen;
			}
		}
		else	// background covers same region as target, but foreground doesn't match
		{
			// determine intersection of fg and bg, we know they are different
			r_Minterval overlayOn = foregroundDomain.create_intersection(backgroundDomain);
			TALK( "Overlay using slow method on " << overlayOn );

			// prepare basic layer for overlaying: set target to background
			if (backgroundMDDCells == targetMDDCells)
			{	// special case: background and target point to same MDD array, 
				TALK( "background and target equal in size - overlaying not neccessary" );
			}
			else	// general case, copy bg to target
			{
				TALK( "Overlay background and target using very fast method" );
				memcpy(targetMDDCells, backgroundMDDCells, backgroundDomain.cell_count() * typeLen);
			}

			// now target is set to bg; next, overlay target and fg

			// looks a little complicated to have a double loop over x and y -- PB
			r_Dimension dim = targetDomain.dimension();
			r_Range width = overlayOn[dim - 1].get_extent();
			r_MiterDirect foregroundIter((char*)foregroundMDDCells, foregroundDomain, overlayOn, typeLen);
			r_MiterDirect targetIter((char*)targetMDDCells, targetDomain, overlayOn, typeLen);
			while (!foregroundIter.isDone())
			{
				char *currentForegroundCell = (char*)foregroundIter.getData();
				char *currentTargetCell = (char*)targetIter.getData();
				for (size_t elemNum = 0; elemNum < width; elemNum++)
				{
					// fg transparent?
					if (memcmp( (void*) reference, (void*) currentForegroundCell, typeLen) != 0)
					{	// no -> set target pixel to fg
						memcpy( (void*) currentTargetCell, (void*) currentForegroundCell, typeLen);
					}
					currentForegroundCell += typeLen;
					currentTargetCell += typeLen;
				}
				foregroundIter.id[dim-1].pos += width;
				targetIter.id[dim-1].pos += width;
				++foregroundIter;
				++targetIter;
			}
		}
	}
	else
	{	// background and target are not the same size.  it does not matter if transparent is same as background
		// need to use a miter, done as in Tile::copyTile, res is foreground, op is background

		// determine intersection of foreground and background
		r_Minterval overlayOn = foregroundDomain.create_intersection(backgroundDomain);

		// this intersection matches target?
		if ( ! targetDomain.covers(overlayOn))
		{
			TALK( "target domain "<< targetDomain << " does not cover background/foreground intersection " << overlayOn );
			throw ImportError( OVERLAYDOMAINDOESNOTMATCH );
		}

		// --- now start action ----------------------------------------------------

		// first copy background on top of target
		TALK( "Overlay using slow method on " << overlayOn );
		r_Minterval copyBackgroundDomain = targetDomain.create_intersection(backgroundDomain);
		r_Dimension dim = copyBackgroundDomain.dimension();
		r_Range backgroundWidth = copyBackgroundDomain[dim - 1].get_extent();
		r_MiterDirect backgroundIter((char*)backgroundMDDCells, backgroundDomain, overlayOn, typeLen);
		r_MiterDirect targetIter((char*)targetMDDCells, targetDomain, overlayOn, typeLen);
		unsigned int cellSize = typeLen * backgroundWidth;
		while (!targetIter.isDone())
		{
			backgroundMDDCells = (char*)backgroundIter.getData();
			targetMDDCells     = (char*)targetIter.getData();

			// set target pixel to background value
			memcpy( (void*)targetMDDCells,  (void*)backgroundMDDCells, cellSize);

			backgroundIter.id[dim-1].pos += backgroundWidth;
			targetIter.id[dim-1].pos += backgroundWidth;
			++backgroundIter;
			++targetIter;
		}

		// I think this is redundant, see top of method -- PB 2003-jul-04
		targetMDDCells = targetMDD->get_array();

		// then overlay transparent over target
		r_Range width = overlayOn[dim - 1].get_extent();
		r_MiterDirect foregroundIter((char*)foregroundMDDCells, foregroundDomain, overlayOn, typeLen);
		r_MiterDirect targetIter2((char*)targetMDDCells, targetDomain, overlayOn, typeLen);
		while (!foregroundIter.isDone())
		{
			foregroundMDDCells = (char*)foregroundIter.getData();
			targetMDDCells     = (char*)targetIter2.getData();
			for (size_t elemNum = 0; elemNum < width; elemNum++)
			{	// FIXME: this body can be optimised -- PB
				// is foreground pixel transparent?
				if (memcmp(reference, &(foregroundMDDCells[elemNum * typeLen]), typeLen) != 0)
				{	// no -> copy it
					memcpy(&(targetMDDCells[elemNum * typeLen]), &(foregroundMDDCells[elemNum * typeLen]), typeLen);
				}
			}
			foregroundIter.id[dim-1].pos += width;
			targetIter2.id[dim-1].pos += width;
			++foregroundIter;
			++targetIter2;
		}
	}
		
	delete [] reference;
	reference = NULL;

	LEAVE( "overlayGMarrays" );
	return;
}

/// scale baseDomain with factor around point origin into scaledDomain
/// returns: length, clipDomain, scaledDomain, underflow (if scaled domain is empty)
void
RasgeoUtil::scaleDomain(const r_Minterval& baseDomain, const r_Point& origin, double factor, r_Minterval& scaledDomain, r_Minterval& clipDomain, unsigned int& length, bool& underflow) throw (ImportError)
{
	ENTER( "scaleDomain, baseDomain=" << baseDomain << ", origin=" << origin << ", factor=" << factor );

	r_Dimension dim = origin.dimension();
	r_Dimension i = 0;
	scaledDomain = r_Minterval(dim);
	clipDomain = r_Minterval(dim);
	r_Range low = 0;
	r_Range high = 0;
	r_Range baseSteps = 0;
	r_Range scaleSteps = 0;
	r_Range originVal = 0;

	if (baseDomain.dimension() != dim)
		throw ImportError( SCALEDOMAINISNOTCORRECT );

	length = floor(1.0 / factor + 0.5);

	underflow = false;
	try
	{
		for (i = 0; i < dim && ! underflow; i++)
		{
			const r_Sinterval& baseSinterval = baseDomain[i];
			originVal = origin[i];

			// simple trafo of low coordinate
			low = (r_Range)(originVal + floor((baseSinterval.low() - originVal) * factor));

			// for the high coordinate use the low coordinate of the _next_ tile
			// ( = baseDomain[i].high() + 1 ) and subtract 1 ==> seamless tilingfactor
			high = (r_Range)(originVal + floor((baseSinterval.high() + 1 - originVal) * factor) - 1);

			// number of steps in base interval
			baseSteps = floorl((baseSinterval.high() - baseSinterval.low() + length) / length);

			// number of steps in scaled interval
			scaleSteps = high - low + 1;

			// check for underflow (we need at least 1 pixel to avoid an empty interval)
			if (scaleSteps < 1)
			{
				TALK( "dim " << i << " interval underflow error: baseSteps=" << baseSteps << ", scaleSteps=" << scaleSteps );
				underflow = true;
			} else
			if (baseSteps == scaleSteps)
			{	// everything is ok
				scaledDomain << r_Sinterval(low, high);
				clipDomain << baseSinterval;
			}
			else if (baseSteps < scaleSteps)
			{	// the base is too small -> shrink the scale interval
				TALK( "WARNING: " << baseDomain << " * " << factor << " : clipping the scale interval" );
				scaledDomain << r_Sinterval(low, low + baseSteps - 1);
				clipDomain << baseSinterval;
			}
			else // (baseSteps > scaleSteps)
			{	// the scale is too small -> shrink the clip interval
				TALK( "WARNING: " << baseDomain << " * " << factor << " : clipping the base interval" );
				scaledDomain << r_Sinterval(low, high);
				clipDomain << r_Sinterval(baseSinterval.low(), (r_Range)(baseSinterval.low() + scaleSteps * length - 1));
			}
		}
	}
	catch (r_Error& err)
	{
		TALK( "scaleDomain: scaling underflow? baseDomain=" << baseDomain << ", scaledDomain=" << scaledDomain << ", factor=" << factor << ", origin=" << origin << ": Error " << err.get_errorno() << " " << err.what() );
		throw ImportError( SCALEDOMAINPOSSIBLEUNDERFLOW );
	}

	LEAVE( "scaleDomain, scaledDomain=" << scaledDomain << ", clipDomain=" << clipDomain << ", underflow=" << underflow );
	return;
}

void
RasgeoUtil::updateScaledMDD(const r_Ref<r_GMarray>& baseMDD, const r_Minterval& clipDomain, const r_Minterval& downScaleDomain, unsigned int length, const char* collectionName) throw (r_Error, ImportError)
{
	ENTER( "updateScaledMDD, clipDomain=" << clipDomain << ", downScaleDomain=" << downScaleDomain << ", length=" << length << ", collectionName=" << collectionName );
	const r_Minterval& baseDomain = baseMDD->spatial_domain();

	size_t tlen = baseMDD->get_type_length();
	r_Ref<r_GMarray> scaledMDD = new (baseMDD->get_type_name())r_GMarray(downScaleDomain, tlen);
	const r_Type* type = scaledMDD->get_base_type_schema();
	if (type == NULL) {
		throw ImportError( NOBASETYPE );}
	TALK( "base type is " << type );

	// perform scaling depending on base type
	if (type->isPrimitiveType())		// scalar cell type
	{
		TALK( "processing primitive type" );
		RasgeoUtil::fast_scale_process_primitive_type((const r_Primitive_Type*)type, scaledMDD->get_array(), baseMDD->get_array(), downScaleDomain, baseDomain, clipDomain, tlen, length, scaleFunction);
	}
	else if ( type->isStructType())
	{
		TALK( "processing struct type" );
		RasgeoUtil::fast_scale_process_structured_type((const r_Structure_Type*)type, scaledMDD->get_array(), baseMDD->get_array(), downScaleDomain, baseDomain, clipDomain, tlen, length, scaleFunction);
	}
	else					// something invalid, we can't process that
	{
		TALK( "illegal base type structure, type id: " << type->type_id() );
		throw ImportError( NOBASETYPE );
	}

	// now perform update collection
	// std::ostringstream stream;
	// stream << "UPDATE " << collectionName << " AS A SET A" << downScaleDomain << " ASSIGN $1";
	// r_OQL_Query query(stream.str().c_str());
	// TALK( "executing update query: " << stream.str().c_str() << "..." );
	string qs = generateImportQuery( collectionName, downScaleDomain, false, NULL );
	r_OQL_Query query( qs.c_str() );
	query << *scaledMDD;
	TALK( "executing update query: " << qs.c_str() << "..." );
	//printf("UPDATEQUERY: %s\n", qs.c_str()); // Debug
	executeQuery( query );
	TALK( "done" );

	scaledMDD.destroy();

	LEAVE( "updateScaledMDD" );
	return;
}

/*
 * scaling functions ------------------------------------------------------------
 */

void
RasgeoUtil::fast_scale_process_primitive_type(const r_Primitive_Type *primType, char *dest, const char *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &srcIter, unsigned int type_len, unsigned int length, r_Scale_Function func) throw (ImportError)
{
	ENTER( "fast_scale_process_primitive_type, primType=" << primType << ", dest=(ptr)" << ", src=(ptr)" << ", destIv=" << destIv << ", srcIv=" << srcIv << ", srcIter=" << srcIter << ", type_len=" << type_len << ", length=" << length << ", func=" << func );

	if (func == r_BitAggregation)
	{
		switch (primType->type_id())
		{
			case r_Primitive_Type::BOOL:
			case r_Primitive_Type::CHAR:
			case r_Primitive_Type::OCTET:
				RasgeoUtil::fast_scale_aggregate_array((char*)dest, (const char*)src, destIv, srcIv, srcIter, type_len, length);
				break;
			case r_Primitive_Type::SHORT:
			case r_Primitive_Type::USHORT:
				RasgeoUtil::fast_scale_aggregate_array((short*)dest, (const short*)src, destIv, srcIv, srcIter, type_len, length);
				break;
			case r_Primitive_Type::LONG:
			case r_Primitive_Type::ULONG:
			case r_Primitive_Type::FLOAT:
				RasgeoUtil::fast_scale_aggregate_array((int*)dest, (const int*)src, destIv, srcIv, srcIter, type_len, length);
				break;
			case r_Primitive_Type::DOUBLE:
				RasgeoUtil::fast_scale_aggregate_array((long long*)dest, (const long long*)src, destIv, srcIv, srcIter, type_len, length);
				break;
			default:
				TALK( "bitaggregation resampling: unknown primitive type" /* << primType->type_id() */ );
				throw ImportError( UNKNOWNBASETYPE );
				break;
		}
	}
	else if (func == r_SubSampling)
	{
		switch (primType->type_id())
		{
			case r_Primitive_Type::BOOL:
				RasgeoUtil::fast_scale_resample_array((r_Boolean*)dest, (const r_Boolean*)src, destIv, srcIv, srcIter, type_len, length, true);
				break;
			case r_Primitive_Type::CHAR:
				RasgeoUtil::fast_scale_resample_array((r_Char*)dest, (const r_Char*)src, destIv, srcIv, srcIter, type_len, length, true);
				break;
			case r_Primitive_Type::OCTET:
				RasgeoUtil::fast_scale_resample_array((r_Octet*)dest, (const r_Octet*)src, destIv, srcIv, srcIter, type_len, length, true);
				break;
			case r_Primitive_Type::SHORT:
				RasgeoUtil::fast_scale_resample_array((r_Short*)dest, (const r_Short*)src, destIv, srcIv, srcIter, type_len, length, true);
				break;
			case r_Primitive_Type::USHORT:
				RasgeoUtil::fast_scale_resample_array((r_UShort*)dest, (const r_UShort*)src, destIv, srcIv, srcIter, type_len, length, true);
				break;
			case r_Primitive_Type::LONG:
				RasgeoUtil::fast_scale_resample_array((r_Long*)dest, (const r_Long*)src, destIv, srcIv, srcIter, type_len, length, true);
				break;
			case r_Primitive_Type::ULONG:
				RasgeoUtil::fast_scale_resample_array((r_ULong*)dest, (const r_ULong*)src, destIv, srcIv, srcIter, type_len, length, true);
				break;
			case r_Primitive_Type::FLOAT:
				RasgeoUtil::fast_scale_resample_array((r_Float*)dest, (const r_Float*)src, destIv, srcIv, srcIter, type_len, length, false);
				break;
			case r_Primitive_Type::DOUBLE:
				RasgeoUtil::fast_scale_resample_array((r_Double*)dest, (const r_Double*)src, destIv, srcIv, srcIter, type_len, length, false);
				break;
			default:
				TALK( "subsampling resampling: unknown primitive type" /* << primType->type_id() */ );
				throw ImportError( UNKNOWNBASETYPE );
				break;
		}
	}
	else	// no valid sampling method - should not appear due to parameter evaluation procedure
	{
		TALK( "unknown sampling method: " << func );
		throw ImportError( INVALIDSAMPLINGMETHOD );
	}

	LEAVE( "fast_scale_process_primitive_type" );
	return;
}

/// scale an array having a struct base type; can handle recursive structs
void
RasgeoUtil::fast_scale_process_structured_type(const r_Structure_Type *structType, char *dest, const char *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &srcIter, unsigned int type_len, unsigned int length, r_Scale_Function func) throw (ImportError)
{
	ENTER( "fast_scale_process_structured_type, structType=" << structType << ", dest=(ptr)" << ", src=(ptr)" << ", destIv=" << destIv << ", srcIv=" << srcIv << ", srcIter=" << srcIter << ", type_len=" << type_len << ", length=" << length << ", func=" << func );

	r_Structure_Type::attribute_iterator iter(structType->defines_attribute_begin());
	// for each struct component
	while (iter != structType->defines_attribute_end())
	{
		r_Type *newType;
		unsigned long offset;

		newType = (*iter).type_of().clone();
		offset = (*iter).offset();

		// is component a struct itself?
		if (newType->isStructType())	// ...then recurse into (composite) struct component
		{
			r_Structure_Type *newStructType = (r_Structure_Type*)newType;
			RasgeoUtil::fast_scale_process_structured_type(newStructType, dest + offset, src + offset, destIv, srcIv, srcIter, type_len, length, func);
		}
		else				// is basic type
		{
			r_Primitive_Type *newPrimType = (r_Primitive_Type*)newType;
			RasgeoUtil::fast_scale_process_primitive_type(newPrimType, dest + offset, src + offset, destIv, srcIv, srcIter, type_len, length, func);
		}
		delete newType;
		iter++;
	}

	LEAVE( "fast_scale_process_structured_type" );
	return;
}

/// scales array using subsampling method
// FIXME: always iterate over the full dest domain, but not over the full src domain.
template<class T>
void RasgeoUtil::fast_scale_resample_array(T *dest, const T *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &iterDom, unsigned int type_len, unsigned int length, bool round) throw (ImportError)
{
	ENTER( "fast_scale_resample_array, destIv=" << MINTERVAL_STREAM(destIv)
		<< ", srcIv=" << MINTERVAL_STREAM(srcIv)
		<< ", iterDom=" << MINTERVAL_STREAM(iterDom)
		<< ", type_len=" << (int) type_len << ", length=" << (int) length << ", round=" << (int) round );

	r_MiterDirect destIter((void*)dest, destIv, destIv, type_len, 1);
	r_MiterDirect subIter((void*)src, srcIv, iterDom, type_len, 1);
	r_MiterDirect srcIter((void*)src, srcIv, iterDom, type_len, length);
	unsigned int dim = (unsigned int)srcIv.dimension();
	unsigned int i;

	// set low bound all zero
	for (i=0; i<dim; i++)
	{
		subIter.id[i].low = 0;
	}

	// process all src items
	while (srcIter.done == 0)
	{
		double sum = 0;
		unsigned int count = 1;

		// init sub iterator
		subIter.done = 0;
		for (i=0; i<dim; i++)
		{
			long rest;

			subIter.id[i].pos = 0;
			subIter.id[i].data = srcIter.getData();
			rest = srcIter.id[i].high - srcIter.id[i].pos;
			if (rest >= (long)length)
				rest = (long)length-1;
			subIter.id[i].high = rest;
			count *= rest+1;
		}

		// sum data
		while (subIter.done == 0)
		{
			sum += *((const T*)(subIter.getData()));
			++subIter;
		}

		// average data
		// use round to nearest for averaging
		if (round)
			*((T*)(destIter.getData())) = (T)(sum / count + 0.5);
		else
			*((T*)(destIter.getData())) = (T)(sum / count);
		// TALK( "current value: " << (long)(((const T*)(srcIter.getData())) - src) << " , " << (long)(((T*)(destIter.getData())) - dest) );
		++srcIter;
		++destIter;
	}

	LEAVE( "fast_scale_resample_array" );
	return;
}

/// scale array using bitaggregation method
// FIXME: always iterate over the full dest domain, but not over the full src domain.
template<class T>
void RasgeoUtil::fast_scale_aggregate_array(T *dest, const T *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &iterDom, unsigned int type_len, unsigned int length) throw (ImportError)
{
	ENTER( string( "fast_scale_aggregate_array, destIv=" ) << destIv << ", srcIv=" << srcIv << ", iterDom=" << iterDom << ", type_len=" << type_len << ", length=" << length );

	r_MiterDirect destIter((void*)dest, destIv, destIv, type_len, 1);
	r_MiterDirect subIter((void*)src, srcIv, iterDom, type_len, 1);
	r_MiterDirect srcIter((void*)src, srcIv, iterDom, type_len, length);
	unsigned int dim = (unsigned int)srcIv.dimension();
	unsigned int i;

	for (i=0; i<dim; i++)
	{
		subIter.id[i].low = 0;
	}
	while (srcIter.done == 0)
	{
		T sum = 0;
		unsigned int count = 1;

		// init sub iterator
		subIter.done = 0;
		for (i=0; i<dim; i++)
		{
			long rest;
	
			subIter.id[i].pos = 0;
			subIter.id[i].data = srcIter.getData();
			rest = srcIter.id[i].high - srcIter.id[i].pos;
			if (rest >= (long)length)
				rest = (long)length-1;
			subIter.id[i].high = rest;
			count *= rest+1;
		}
		while (subIter.done == 0)
		{
			sum |= *((const T*)(subIter.getData()));
			++subIter;
		}
		*((T*)(destIter.getData())) = sum;
		//cout << (long)(((const T*)(srcIter.getData())) - src) << " , " << (long)(((T*)(destIter.getData())) - dest) << endl;
		++srcIter;
		++destIter;
	}

	LEAVE( "fast_scale_aggregate_array" );
	return;
}

/// generate a rasql update query string from collection name and oid, if oid flag says so
const string RasgeoUtil::generateImportQuery( const char *c, r_Minterval ovl, int useOId, r_OId oid ) throw(ImportError, r_Error)
{
        ENTER( "RasgeoUtil::generateImportQuery, c=" << c << ", ovl=" << ovl << ", useOId=" << useOId << ", oid=" << oid );
        string myQuery = string( "UPDATE " ) + c + " AS a SET a" + ovl.get_string_representation() + " ASSIGN $1";

        if (useOId)                             // OId is to be used, so append WHERE clause
                myQuery += string( " WHERE oid(a)=" ) + oid.get_string_representation();

        LEAVE( "RasgeoUtil::generateImportQuery, retval=" << myQuery.c_str() );
        return myQuery.c_str();
} // generateImportQuery()

/// generate a rasql select query string
/// FIXME: why here no OID flag???
const string RasgeoUtil::generateSelectQuery( const char *c, r_Minterval ovl ) throw (ImportError, r_Error)
{
        ENTER( "RasgeoUtil::generateSelectQuery, c=" << c << ", ovl=" << ovl );

        string result = string( "SELECT a" ) + ovl.get_string_representation() + " FROM " + c + " AS a";

        LEAVE( "RasgeoUtil::generateSelectQuery, result=" << result.c_str() );
        return result;
} // generateSelectQuery()

// --------------------------------------------------------------------------

/// perform select query (with result), log to stdout if debug is activated
void
RasgeoUtil::executeQuery( r_OQL_Query& query, r_Set< r_Ref_Any > &result ) throw (r_Error, ImportError)
{
	if (RasgeoUtil::debug)
		cout << "\t" << query.get_parameterized_query() << endl << flush;
	r_oql_execute( query , result );
	return;
}

/// perform update query (no result), log to stdout if debug is activated
void
RasgeoUtil::executeQuery( r_OQL_Query& query ) throw (r_Error, ImportError)
{
	if (RasgeoUtil::debug)
		cout << "\t" << query.get_parameterized_query() << endl << flush;
	r_oql_execute( query );
	return;
}

// --------------------------------------------------------------------------

bool
RasgeoUtil::allowedCollNameChar( const char* collName )
{
	char testChar;
	bool allowedChar = true;

	for (int strPos = 0; strPos < strlen( collName) && allowedChar; strPos++)
	{
		testChar = collName[strPos];
		allowedChar = (testChar >= 'a' && testChar <= 'z') || (testChar >= 'A' && testChar <= 'Z')
		           || (testChar >= '0' && testChar <= '9') || (testChar == '_') || (testChar == '-');
				// not sure about '-', but rasdaman will complain if illegal
	}

	if ( ! allowedChar )
		TALK( "allowedCollNameChar: illegal char in scale collectionname name: " << testChar );

	return allowedChar;
}
/*
 * get database type structure from type name
 * returns ptr if an MDD type with the given name exists in the database, NULL otherwise
 * throws r_Error upon general database comm error
 */
r_Marray_Type * RasgeoUtil::getTypeFromDatabase( const char *mddTypeName ) throw(ImportError, r_Error)
{
	ENTER( "getTypeFromDatabase, mddTypeName=" << mddTypeName );
	r_Marray_Type *retval = NULL;
	char* typeStructure = NULL;

	// first, try to get type structure from database using a separate r/o transaction
	try
	{
		openTransaction(false);
		typeStructure = db.getComm()->getTypeStructure(mddTypeName, ClientComm::r_MDDType_Type);
		TALK( "type structure is " << typeStructure );
		closeTransaction( false );	// abort
	}
	catch (r_Error& err)
	{
		if (err.get_kind() == r_Error::r_Error_DatabaseClassUndefined)
		{
			TALK( "Type is not a well known type: " << typeStructure );
			typeStructure = new char[strlen(mddTypeName) + 1];
			// earlier code tried this one below, but I feel we better are strict -- PB 2003-jul-06
			// strcpy(typeStructure, mddTypeName);
			// TALK( "using instead: " << typeStructure );
			throw ImportError( MDDTYPEINVALID );
		}
		else	// unanticipated error
		{
			TALK( "Error during type retrieval from database: " << err.get_errorno() << " " << err.what() );
			throw;
		}
	}

	// next, find out whether it is an MDD type (and not a base or set type, eg)
	try
	{
		r_Type* tempType = r_Type::get_any_type(typeStructure);
		TALK( "get_any_type() for this type returns: " << tempType );
		if (tempType->isMarrayType())
		{
			retval = (r_Marray_Type*)tempType;
			tempType = NULL;
			TALK( "found MDD type: " << retval );
		}
		else
		{
			TALK( "type is not an marray type: " << typeStructure );
			delete tempType;
			tempType = NULL;
			retval = NULL;
			throw ImportError( MDDTYPEINVALID );
		}
	}
	catch (r_Error& err)
	{
		TALK( "Error during retrieval of MDD type structure (" << typeStructure << "): " << err.get_errorno() << " " << err.what() );
		throw;
	}

	delete [] typeStructure;
	typeStructure = NULL;

	LEAVE( "getTypeFromDatabase, retval=" << retval );
	return retval;
} // getTypeFromDatabase()


/*
 * 
 */
void RasgeoUtil::doImport(const char* queryString, const char* queryStringS, r_Marray_Type*& mddType, r_Ref<r_GMarray>& fileMDD ) throw(ImportError, r_Error)
{
	ENTER( "doImport, queryString=" << queryString << ", queryStringS=" << queryStringS );

	r_Set<r_Ref_Any> result;		// select query result
	r_Ref<r_GMarray> selectedMDD;		// MDD fetched from database for overlaying it

	r_Storage_Layout* stl;			// storage layout for update MDD
	r_Ref<r_GMarray> targetMDD;		// result MDD written into database

	// prepare overlay MDD
	TALK( "initializing targetMDD, tiling as above, domain " << overlayDomain );
	stl = new r_Storage_Layout(theTiling->clone());
	targetMDD = new (mddTypeName)r_GMarray(overlayDomain, mddType->base_type().size(), stl);
	targetMDD->set_type_schema(mddType);

	if (strlen(queryStringS) == 0)		// nothing from database to be overlaid
	{
		TALK( "overlay targetMDD (empty) with fileMDD into targetMDD" );
		overlayGMarrays(targetMDD, fileMDD, targetMDD);
	}
	else					// fetch area to be overlaid from database
	{
		LOG << LOG_INDENT << "fetching map information from database..." << flush;

		TALK( "executing select query to fetch overlay area: " << queryStringS << "..." );
		r_OQL_Query query(queryStringS);
		RasgeoUtil::executeQuery(query, result);
		TALK( "done" );

		r_Iterator< r_Ref_Any > iter = result.create_iterator();
		selectedMDD = r_Ref<r_GMarray>(*iter);
			// FIXME: what if result contains >1 elements?
		TALK( "domain of database MDD is " << selectedMDD->spatial_domain() );

		LOG << "ok" << endl << flush;

		// prepare polygon MDD for polygon clipping, if specified
		// FIXME: is polygon applied to other MDDs too?
		if (polygonDefined)
		{
			TALK( "considering polygon" );
			polygon.setMArray(*selectedMDD);
			if (insidePatternSelDef)
			{
				TALK( "- considering insidePattern" );
				polygon.fillMArrayInside(insidePatternSel);
			}
			if (outsidePatternSelDef)
			{
				TALK( "- considering outsidePattern" );
				polygon.fillMArrayOutside(outsidePatternSel);
			}
		}

		// now construct result MDD from input MDDs
		if (fileName)		// input file provided?
		{
			TALK( "overlay selectedMDD with fileMDD into targetMDD" );
			overlayGMarrays(targetMDD, fileMDD, selectedMDD);
		}
		else			//just copy selected over target
		{
			TALK( "overlay targetMDD (empty) with selectedMDD into targetMDD" );
			// FIXME: can we optimize this?
			overlayGMarrays(targetMDD, selectedMDD, targetMDD);
		}
	}

	// perform database update, do pyramid construction if specified
	if (scaleLevels == NULL)	// single query, no pyramid iteration
	{
		LOG << LOG_INDENT << "performing database update (no pyramid)..." << flush;

		r_OQL_Query query(queryString);
		query << *targetMDD;
		TALK( "executing query for target MDD: " << queryString << "..." );
		executeQuery( query );
		TALK( "done" );

		LOG << "ok" << endl << flush;
	}
	else				// pyramid iteration
	{
		LOG << LOG_INDENT << "performing database update with pyramid:" << endl << flush;

		std::list<std::pair<double, char*> >::iterator iter = scaleLevels->begin();
		std::list<std::pair<double, char*> >::iterator end = scaleLevels->end();
		r_Minterval scaledDomain;
		r_Minterval clipDomain;
		unsigned int length = 0;
		r_Dimension maxDim = mddDomain.dimension();
		r_Point origin(maxDim);
		double factor = 0;
		for (r_Dimension i = 0; i < maxDim; i++)
			origin[i] = 0;		// lower bound is always 0 in rasgeo

		TALK( "doing pyramid construction for target MDD scale levels" );
		while (iter != end)	// for each pyramid level
		{
			factor = iter->first;
			//if (factor == 1)
			//{
			//	LOG << LOG_INDENT << LOG_INDENT << "pyramid base layer " << collName << "..." << flush;

			//	TALK( "performing database update with scale factor 1: " << queryString << "..." );
			//	r_OQL_Query query(queryString);
			//	query << *targetMDD;
			//	executeQuery( query );
			//	TALK( "done" );

			//	LOG << "ok" << endl << flush;
			//}
			//else
			//{
				bool underflow = false;		// interval underflow due to large scale factor?

				scaleDomain(overlayDomain, origin, factor, scaledDomain, clipDomain, length, underflow);

				LOG << LOG_INDENT << LOG_INDENT << "pyramid layer " << iter->second << ", factor " << factor << ", target domain " << scaledDomain << "..." << flush;
				if (underflow)
					cout << "warning: pyramid interval underflow, skipping domain " << overlayDomain << endl << flush;
				else
				{
					TALK( "performing database update for layer " << iter->second << " with factor " << iter->first << ", scaled domain " << scaledDomain << ", " << " clip domain " << clipDomain );
					updateScaledMDD(targetMDD, clipDomain, scaledDomain, length, iter->second);

					LOG << "ok" << endl << flush;
				}
			//}
			iter++;
		}
		TALK( "database update done" );
	}

	// if (selectedMDD != NULL)
	//	selectedMDD.destroy();

	if (targetMDD != NULL)
		targetMDD.destroy();

	// if (stl != NULL)
	// 	delete stl;

	LEAVE( "doImport" );
	return;
} // doImport()


/*
 * handle import: read input file, do overlaying with preexisting data, run updates;
 * consider tiling, alignment, pyramid
 */
void
RasgeoUtil::doStuff(int argc, char** argv, ImportType iType ) throw(ImportError, r_Error)
{
	ENTER( "doStuff" );

#ifndef RMANDEBUG	
	installSignalHandlers();
#endif	

	r_Ref<r_GMarray> fileMDD;		// MDD contents to be read from file
	r_MiterArea* iter = NULL;
	r_Minterval tempDom;			// aux var, domain to be updated
	r_Minterval tileDom;			// update portion (align window for --align, complete domain otherwise)

	importType = iType;			// remember import type for subordinate code

	openDatabase();				// open database, set parameters

        // load MDD type structure from database
        LOG << "verifying type structure against database...";
        TALK( "load type definition from database" );
        r_Marray_Type *mddType = getTypeFromDatabase( mddTypeName );
        if (mddType == NULL)
                throw ImportError( MDDTYPEINVALID );
        LOG << "ok" << endl << flush;

	// prepare type and storage layout
	size_t baseTypeLength = mddType->base_type().size();
	TALK( "base type length is " << baseTypeLength );

	// load file contents into MDD, if specified (no TK support here!)
	if (fileName != NULL)
	{
		TALK( "initializing fileMDD, tiling as above, domain " << mddDomain << ", base type length " << baseTypeLength );

		// prepare fileMDD from file contents
		switch (importType)
		{
		  case IMPORTTYPE_ORTHO:
			LOG << "reading input file " << fileName << "..." << flush;
			initGMarray(fileMDD, fileName, inputFormat, mddDomain, mddType, theTiling, inputFormatParams);
			LOG << "ok" << endl << flush;
			break;
		  case IMPORTTYPE_DEM:
			LOG << "reading input file " << fileName << "..." << flush;
			initGMarray(fileMDD, fileName, inputFormat, mddDomain, mddType, theTiling, inputFormatParams);
			// in case of a DEM, bbox came through the input file contents -> set domain
			overlayDomain = mddDomain = fileMDD->spatial_domain();
			LOG << "ok" << endl << flush;
			break;
		  case IMPORTTYPE_TK:
		  default:
			TALK( "Error: unsupported import type: " << importType );
			throw ImportError( ILLEGALIMPORTTYPE );
			break;
		}

		// if polygon cutout is defined, apply it to converted MDD
        	if (polygonDefined)
        	{
			LOG << "applying polygon cutout..." << flush;
/* should be done in r_PolygonCutOut
                	if (fileMDD->spatial_domain().covers(polygon.getBoundingBox()))
                	{
*/
                        	polygon.setMArray(*fileMDD);
                        	if (insidePatternDef)
                                	polygon.fillMArrayInside(insidePattern);
                        	if (outsidePatternDef)
                                	polygon.fillMArrayOutside(outsidePattern);
/* should be done in r_PolygonCutOut
                	}
                	else
                	{
                        	TALK( "Polygon Bounding Box " << polygon.getBoundingBox() << " GMarray Bounding Box " << fileMDD->spatial_domain() );
                        	throw ImportError( POLYGONDOMAINTOOLARGE );
                	}
*/
			LOG << "ok" << endl << flush;
        	}
	}

	// if tiled update: prepare parameters
	if (tiledImport)
	{
		tempDom = overlayDomain;
// taken from system_update, but does not yield desired results: uses updateBufferSize, destroys align
#if 0 
		//taken from tiling.cc r_Size_Tiling
		r_Dimension dim = tempDom.dimension();
		r_Range edgeLength = (r_Range)std::max( (r_Range)floor(pow(updateBufferSize, 1/(double)dim) ), 1);
		if (align)
		{
			edgeLength = edgeLength - edgeLength%align;
			if (edgeLength < align)
				edgeLength = align;
			TALK( "tiledImport in combination with align parameter leads to edge length " << edgeLength );
		}
		tileDom = r_Minterval(dim);
		for (r_Dimension dimcnt = 0; dimcnt < dim; dimcnt++)
			tileDom << r_Sinterval(0, edgeLength - 1);
		iter = new r_MiterArea(&tileDom, &tempDom);
#else	// this disregards buffer size if , focuses only on align value
		// do we have to go through the MDD tile by tile (align gives edge length)?
		// NB: align parameter overrides buffer size settings
		r_Range edgeLength;
		if (align)			// align parameter specified?
		{				// yes -> define window by align value as edge length
			// build minterval to iterate (our arrays are 2D)
			edgeLength = align;
		}
		else				// no -> use buffer size to determine chunk size
		{	// sqrt(bufsize/pixsize) gives max edge length
			edgeLength = (r_Range)std::max(
				(r_Range)floor(pow(updateBufferSize / mddType->base_type().size(), 1/(double)2.0) ),
				1);
		}
		// window is 0..n-1 x 0..n-1
		tileDom = r_Minterval(2) << r_Sinterval(0, edgeLength - 1) << r_Sinterval(0, edgeLength - 1);
		iter = new r_MiterArea(&tileDom, &tempDom);
#endif
		TALK( "Tiling domain " << tempDom << ", into tiles " << tileDom );
	}

	LOG << "opening read-write transaction..." << flush;
	openTransaction( true );		// start update transaction
	LOG << "ok" << endl << flush;

	do	// pass once for non-tiled, iterate for tiled update
		// tiled update:
		//	- overlaydomain == mddDomain
		// otherwise:
		//	- mddDomain superset of overlayDomain
	{
		if (tiledImport)
		{
			mddDomain = iter->nextArea();		// position of next tile
			overlayDomain = mddDomain;		// set overlayDomain to the portion updated next
			tiledImport = !iter->isDone();		// here we set the continuation flag
			TALK( "Doing now " << mddDomain << ", overlayDomain=" << overlayDomain << "; continue loop=" << tiledImport );
		}
		// else: mddDomain / overlayDomain already set for single update

		// log output depending on verbosity: either a progress dot, or messages
		// NB: note {} around LOG, DON'T remove!
		if (verbose)
		{
			LOG << "working on domain " << mddDomain << ":" << endl << flush;
		}
		else
		{
			cout << "." << flush;
		}

		string selectQuery = "";
		if (transparent || !fileName)
		{
			if (! overlayDomain.covers(mddDomain))
			{
				TALK( "Overlay domain " << overlayDomain << " does not cover MDD domain " << mddDomain );
				throw ImportError( OVERLAYDOMAINDOESNOTMATCH );
			}
			selectQuery = generateSelectQuery( collName, overlayDomain );
		}
		TALK( "selectQuery=" << selectQuery );

		string updateQuery = generateImportQuery( collName, overlayDomain, mddOIdDef, mddOId );
		TALK( "updateQuery=" << updateQuery );
		doImport( updateQuery.c_str(), selectQuery.c_str(), mddType, fileMDD );

		// keep TAs small to save main mem
		if ( taSplit )
		{
			LOG << LOG_INDENT << "flushing transaction..." << flush;
			RasgeoUtil::closeTransaction( true );           // commit
			RasgeoUtil::openTransaction( true );		// r/w
			LOG << "ok" << endl;
		}

	} while (tiledImport);	// !iter->isDone()

	// log output depending on verbosity: an eol for the progress dots
	// NB: note {} around LOG, DON'T remove!
	if (!verbose)
		cout << endl;


	LOG << "committing transaction..." << flush;
	closeTransaction( true );		// commit changes
	LOG << "ok" << endl;

	closeDatabase();			// close database

	if (mddType != NULL)
	{
		delete mddType;
		mddType = NULL;
	}

	if (iter != NULL)
	{
		delete iter;
		iter = NULL;
	}

	if (fileMDD != NULL)
		fileMDD.destroy();

	LEAVE( "doStuff" );
	return;
} // doStuff()

