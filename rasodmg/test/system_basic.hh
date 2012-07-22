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

#ifndef _SYSTEMBASIC_HH_
#define _SYSTEMBASIC_HH_

class r_Tiling;

// fopen, getc
#include <stdio.h>
#include <list>
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
#include "cmlparser.hh"

class SystemBasic
{
public:
    static char* getData(FILE* file, size_t& dtaSize, int& retval);

    static void printScalar(const r_Scalar& scalar);

    static int checkArguments(int argc, const char** argv, const char* searchText, int& optionValueIndex);

    static void explainRetval(int retval);

    static void printOptions();

    static int parseParams(int argc, char** argv);

    static FILE* checkFile(const char* fileArg, int& retval);

    static int convertTo(r_Data_Format fmt, char*& src, size_t& size, r_Minterval& interv, r_Base_Type*& tp, const char* options);

    static int convertFrom(r_Data_Format fmt, char*& src, size_t& size, r_Minterval& interv, r_Base_Type*& tp, const char* options);

    static int readScaleLevels(const char* argv);

    static int initGMarray(r_Ref<r_GMarray>& tempMDD, FILE* tempFile, r_Data_Format conversionFormat, r_Minterval& tempDataDomain, const char* conversionParams) throw (r_Error);

    static int overlayGMarrays(r_Ref<r_GMarray>& target, const r_Ref<r_GMarray>& replaceBlackMDD, const r_Ref<r_GMarray>& backgroundMDD);

    static int compareGMarrays(const r_Ref<r_GMarray>& baseMDD, r_Ref<r_GMarray>& topMDD);

    static void openTransaction(bool readwrite = true) throw (r_Error);

    static int compareScaledMDD(const r_Ref<r_GMarray>& baseMDD, const r_Minterval& clipDom, const r_Minterval& downScaledDomain, unsigned int length, const char* collectionName) throw (r_Error);

    static int updateScaledMDD(const r_Ref<r_GMarray>& baseMDD, const r_Minterval& clipDom, const r_Minterval& downScaledDomain, unsigned int length, const char* collectionName) throw (r_Error);

    static int scaleDomain(const r_Minterval& baseDomain, const r_Point& origin, double factor, r_Minterval& scaledDomain, r_Minterval& clipDomain, unsigned int& length);

    static int saveData(const char* fileNamePat, const char* data, r_Bytes length, const r_Minterval& mddDomain);

    static const char* serverName;
    static r_ULong serverPort;
    static const char* baseName;
    static const char* userName;
    static const char* passwd;
    static bool printText;
    static const char* outputFileName;
    static const char* conversionTypeName;
    static r_Data_Format inputFormat;
    static const char* inputFormatParams;
    static r_Data_Format outputFormat;
    static const char* outputFormatParams;
    static r_Data_Format transferFormat;
    static const char* transferFormatParams;
    static r_Data_Format storageFormat;
    static const char* storageFormatParams;
    static const char* collName;
    static const char* setTypeName;
    static const char* mddTypeName;
    static r_OId mddOId;
    static bool mddOIdDef;
    static r_OId collOId;
    static bool collOIdDef;
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
    static const char* noUsageHeader;
    static const char* usageHeader;
    static const char* noUsageFooter;
    static const char* usageFooter;
    static size_t updateBufferSize;
    static const char* defaultUpdateBufferSize;
    static std::list<char*> layerList;
    static std::list<unsigned int> patternsTrue;
    static std::list<unsigned int> patternsFalse;
    static r_Minterval overlayDomain;
    static bool overlayDomainDef;
    static r_Range align;
    static bool tiledUpdate;
    static bool handleSignal;
    static bool force;
};

void signalHandler(int sig);

void installSignalHandlers();

#define QUERYBUFFERLENGTH       512

#define ALLDONE             -1
#define OK              0
#define CREATEANDNOCREATE       1
#define NOVALIDDOMAIN           2
#define FILEINACCESSIBLE        3
#define FILENAMETOLONG          4
#define NOCREATECREATECOLL      5
#define NOCOLLTYPE          6
#define COLLOIDANDCOLLNAME      7
#define CREATECOLLWITHOID       8
#define CREATEMDDWITHOID        9
#define INVALIDTILESIZE         10
#define DOMAINDATAMISMATCH      11
#define MDDDOMAINNOTSPECIFIED       12
#define FILENAMENOTSPECIFIED        13
#define NOCOLLNAMENOCOLLOID     14
#define MDDTYPEINVALID          15
#define NOBASETYPE          16
#define EXCEPTIONCREATECOLL     17
#define EXCEPTIONADMIN          18
#define COLLECTIONINACCESSIBLE      19
#define MDDINACCESSIBLE         20
#define OIDINVALID          21
#define MDDOIDANDCOLL           22
#define ARGUMENTSMISSING        23
#define ARGUMENTSINCOMPLETE     24
#define EXCEPTIONEXECUTEQUERY       25
#define NOQUERY             26
#define CONVERSIONNOTSUPPORTED      27
#define CONVERSIONEXCEPTION     28
#define UNKNOWNTILINGTYPE       29
#define TILINGPARAMETERSMISSING     30
#define TILINGPATAMETERSINCORRECT   31
#define CONVERSIONTYPENOTABASETYPE  32
#define CONVERSIONRETURNEDWRONGTYPE 33
#define POLYGONCREATIONFAILED       34
#define OVERLAYDOMAINSDONOTMATCH    35
#define OVERLAYTYPESIZESDONOTMATCH      36
#define POLYGONDOMAINTOOLARGE       37
#define NOCOLLNAMEDEFINED           38
#define NOMDDTYPEDEFINED        39
#define MDDTYPEOFGMARRAYNOTINITIALISED  40
#define SCALEDOMAINISNOTCORRECT     41
#define SCALELEVELSINCORRECT        42
#define GMARRAYSARENOTEQUAL         43
#define UNKNOWNSCALEFUNCTION        44
#define UNABLETOCLAIMRESOURCEFORFILE    45
#define ERRORPARSINGCOMMANDLINE         46
#define TILINGTYPENOTALLOWED            47
#define LAYERSNOTCORRECT                48
#define TOOMANYLAYERS                   49
#define OVERLAYDOMAINDOESNOTMATCH   50
#define TILEDUPDATEANDFILE          51
#define NOCOMPAREDESTINATION        52
#define CONVERSIONTYPEMISSING       53
#endif
