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
 * SOURCE: system_basic.cc
 *
 * MODULE: rasodmg/test
 *
 * COMMENTS:
 *      None
*/

static const char rcsid[] = "@(#)rasodmg/test,SystemBasic: $Id: system_basic.cc,v 1.37 2002/10/09 09:58:05 hoefner Exp $";

#ifdef TESTBASIC
#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif
#endif

#include "system_basic.hh"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#ifdef SOLARIS
#include <strings.h>
#endif

#include "raslib/rmdebug.hh"
#include "raslib/rminit.hh"
#include "raslib/type.hh"
#include "raslib/complex.hh"
#include "raslib/structure.hh"
#include "raslib/basetype.hh"
#include "raslib/structuretype.hh"
#include "raslib/collectiontype.hh"
#include "raslib/marraytype.hh"
#include "raslib/dlist.hh"
#include "raslib/primitivetype.hh"
#include "raslib/miterd.hh"
#include "raslib/storageman.hh"

#include "rasodmg/marray.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/dirtiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "rasodmg/interesttiling.hh"
#include "rasodmg/alignedtiling.hh"
#include "rasodmg/stattiling.hh"
#include "rasodmg/oqlquery.hh"

#include "conversion/convfactory.hh"
#include "conversion/convertor.hh"

#include "exportutils/hexcodec.cc"
#include "exportutils/error.cc"

void
fast_scale_process_primitive_type(const r_Primitive_Type *primType, char *dest, const char *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &iterDom, unsigned int type_len, unsigned int length, r_Scale_Function func);
void
fast_scale_process_structured_type(const r_Structure_Type *primType, char *dest, const char *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &iterDom, unsigned int type_len, unsigned int length, r_Scale_Function func);
template<class T>
void
fast_scale_resample_array(T *dest, const T *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &iterDom, unsigned int type_len, unsigned int length, bool round);

template<class T>
void
fast_scale_aggregate_array(T *dest, const T *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &iterDom, unsigned int type_len, unsigned int length);

void
signalHandler(int sig)
{
    cout << "**CLIENT** signal error " << sig << " ";
    switch (sig)
    {
    case SIGHUP:
        cout << "Hangup (POSIX).  ";
        break;
    case SIGINT:
        cout << "Interrupt (ANSI).";
        break;
    case SIGQUIT:
        cout << "Quit (POSIX).";
        break;
    case SIGILL:
        cout << "Illegal instruction (ANSI).";
        break;
    case SIGTRAP:
        cout << "Trace trap (POSIX).";
        break;
    case SIGABRT:
        cout << "Abort (ANSI) or IOT trap (4.2 BSD).";
        break;
    case SIGBUS:
        cout << "BUS error (4.2 BSD).";
        break;
    case SIGFPE:
        cout << "Floating-point exception (ANSI).";
        break;
    case SIGKILL:
        cout << "Kill, unblockable (POSIX).";
        break;
    case SIGUSR1:
        cout << "User-defined signal 1 (POSIX).";
        break;
    case SIGSEGV:
        cout << "Segmentation violation (ANSI).";
        break;
    case SIGUSR2:
        cout << "User-defined signal 2 (POSIX).";
        break;
    case SIGPIPE:
        cout << "Broken pipe (POSIX).";
        break;
    case SIGALRM:
        cout << "Alarm clock (POSIX).";
        break;
    case SIGTERM:
        cout << "Termination (ANSI).";
        break;
#ifndef SOLARIS
#ifndef DECALPHA
    case SIGSTKFLT:
        cout << "Stack fault.";
        break;
#endif
#endif
    case SIGCLD:
        cout << "SIGCHLD (System V) or child status has changed (POSIX).";
        break;
    case SIGCONT:
        cout << "Continue (POSIX).";
        break;
    case SIGSTOP:
        cout << "Stop, unblockable (POSIX).";
        break;
    case SIGTSTP:
        cout << "Keyboard stop (POSIX).";
        break;
    case SIGTTIN:
        cout << "Background read from tty (POSIX).";
        break;
    case SIGTTOU:
        cout << "Background write to tty (POSIX).";
        break;
    case SIGURG:
        cout << "Urgent condition on socket (4.2 BSD).";
        break;
    case SIGXCPU:
        cout << "CPU limit exceeded (4.2 BSD).";
        break;
    case SIGXFSZ:
        cout << "File size limit exceeded (4.2 BSD).";
        break;
    case SIGVTALRM:
        cout << "Virtual alarm clock (4.2 BSD).";
        break;
    case SIGPROF:
        cout << "Profiling alarm clock (4.2 BSD).";
        break;
    case SIGWINCH:
        cout << "Window size change (4.3 BSD, Sun).";
        cout << "Continue with operation.";
        return;
        break;
    case SIGPOLL:
        cout << "Pollable event occurred (System V) or I/O now possible (4.2 BSD).";
        break;
    case SIGPWR:
        cout << "Power failure restart (System V).";
        break;
    case SIGSYS:
        cout << "Bad system call.";
        break;
    default:
        cout << "Unknown signal.";
        break;
    }
    cout << endl;
    if (SystemBasic::handleSignal)
    {
        SystemBasic::handleSignal = false;
        try
        {
            cout << "Aborting transaction" << endl;;
            SystemBasic::ta.abort();
            cout << "Transaction aborted" << endl;
            cout << "Closing database" << endl;
            SystemBasic::db.close();
            cout << "Database closed" << endl;
        }
        catch (r_Error& err)
        {
            cout << "Error while aborting transaction/closing database: " << err.get_errorno() << " : " << err.what() << endl;
        }
    }
    else
    {
        //not doing it
    }
    exit(sig);
}

void
installSignalHandlers()
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGHUP, signalHandler);
    signal(SIGPIPE, signalHandler);
    signal(SIGHUP, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGTRAP, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGIOT, signalHandler);
    signal(SIGBUS, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGKILL, signalHandler);
    signal(SIGUSR1, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGUSR2, signalHandler);
    signal(SIGPIPE, signalHandler);
    signal(SIGALRM, signalHandler);
    signal(SIGTERM, signalHandler);
#ifndef SOLARIS
#ifndef DECALPHA
    signal(SIGSTKFLT, signalHandler);
#endif
#endif
    signal(SIGCLD, signalHandler);
    signal(SIGCHLD, signalHandler);
    signal(SIGCONT, signalHandler);
    signal(SIGSTOP, signalHandler);
    signal(SIGTSTP, signalHandler);
    signal(SIGTTIN, signalHandler);
    signal(SIGTTOU, signalHandler);
    signal(SIGURG, signalHandler);
    signal(SIGXCPU, signalHandler);
    signal(SIGXFSZ, signalHandler);
    signal(SIGVTALRM, signalHandler);
    signal(SIGPROF, signalHandler);
    signal(SIGWINCH, signalHandler);
    signal(SIGPOLL, signalHandler);
    signal(SIGIO, signalHandler);
    signal(SIGPWR, signalHandler);
    signal(SIGSYS, signalHandler);
#if !defined SOLARIS
#if !defined DECALPHA
    signal(SIGUNUSED, signalHandler);
#endif
#endif
}

r_Tiling*
SystemBasic::theTiling = 0;

bool
SystemBasic::testBed = false;

const char*
SystemBasic::serverName = "localhost";

r_ULong
SystemBasic::serverPort = 7001;

const char*
SystemBasic::baseName = "RASBASE";

const char*
SystemBasic::userName = "rasguest";

const char*
SystemBasic::passwd = "rasguest";

bool
SystemBasic::printText = false;

const char*
SystemBasic::outputFileName = NULL;

r_Data_Format
SystemBasic::outputFormat = r_Array;

const char*
SystemBasic::outputFormatParams = 0;

const char*
SystemBasic::conversionTypeName = "char";

r_Data_Format
SystemBasic::inputFormat = r_Array;

const char*
SystemBasic::inputFormatParams = 0;

r_Data_Format
SystemBasic::transferFormat = r_Array;

const char*
SystemBasic::transferFormatParams = 0;

r_Data_Format
SystemBasic::storageFormat = r_Array;

const char*
SystemBasic::storageFormatParams = 0;

const char*
SystemBasic::collName = NULL;

const char*
SystemBasic::setTypeName = NULL;

const char*
SystemBasic::mddTypeName = NULL;

r_OId
SystemBasic::mddOId;

bool
SystemBasic::mddOIdDef = false;

r_OId
SystemBasic::collOId;

bool
SystemBasic::collOIdDef = false;

const char*
SystemBasic::fileName = NULL;

r_Database
SystemBasic::db;

r_Transaction
SystemBasic::ta;

r_Minterval
SystemBasic::mddDomain;

bool
SystemBasic::mddDomainDef = false;

bool
SystemBasic::polygonDefined = false;

r_PolygonCutOut
SystemBasic::polygon;

int
SystemBasic::polygonShrinker;

bool
SystemBasic::transparent = false;

string
SystemBasic::outsidePatternSel;

bool
SystemBasic::outsidePatternSelDef = false;

string
SystemBasic::insidePatternSel;

bool
SystemBasic::insidePatternSelDef = false;

string
SystemBasic::outsidePattern;

bool
SystemBasic::outsidePatternDef = false;

string
SystemBasic::insidePattern;

bool
SystemBasic::insidePatternDef = false;

const int
SystemBasic::queryBufferLength = 512;

std::list<std::pair<double, char*> >*
SystemBasic::scaleLevels = NULL;

int
SystemBasic::wrongBytes = 0;

r_Scale_Function
SystemBasic::scaleFunction = r_SubSampling;

size_t
SystemBasic::updateBufferSize = 52428800;

const char*
SystemBasic::defaultUpdateBufferSize = "52428800";


std::list<char*>
SystemBasic:: layerList;

std::list<unsigned int>
SystemBasic::patternsTrue;

std::list<unsigned int>
SystemBasic::patternsFalse;

const char*
SystemBasic::noUsageHeader = "Please set SystemBasic::usageHeader to a meaningful value.\nThen you will help your users : )\n";

const char*
SystemBasic::usageHeader = SystemBasic::noUsageHeader;

const char*
SystemBasic::noUsageFooter = "Please set SystemBasic::usageFooter to a meaningful value.\nThen you will help your users : )\n";

const char*
SystemBasic::usageFooter = SystemBasic::noUsageFooter;

r_Minterval
SystemBasic::overlayDomain;

bool
SystemBasic::overlayDomainDef = false;

r_Range
SystemBasic::align = 0;

bool
SystemBasic::tiledUpdate = false;

bool
SystemBasic::force = false;

//--tiling params description
const string
SystemBasic::tilingDesc= string("") +
                         "<tiling-name> tiling strategy, specified as:" + CommandLineParameter::descLineSep +
                         "  " + tiling_name_notiling          + "," + CommandLineParameter::descLineSep +
                         "  " + tiling_name_sizetiling        + "," + CommandLineParameter::descLineSep +
                         "  " + tiling_name_alignedtiling     + "," + CommandLineParameter::descLineSep +
                         "  " + tiling_name_interesttiling    + "," + CommandLineParameter::descLineSep +
                         "  " + tiling_name_directionaltiling + "," + CommandLineParameter::descLineSep +
                         "  " + tiling_name_statisticaltiling;

const string
SystemBasic::tilingParamsDesc =  string("") +
                                 "<params> parameters for tiling strategy, specified as:" + CommandLineParameter::descLineSep +
                                 "... for " + tiling_name_notiling + CommandLineParameter::descLineSep +
                                 r_No_Tiling::description + CommandLineParameter::descLineSep +
                                 "... for " + tiling_name_sizetiling + CommandLineParameter::descLineSep +
                                 r_Size_Tiling::description + CommandLineParameter::descLineSep +
                                 "... for " + tiling_name_alignedtiling + CommandLineParameter::descLineSep +
                                 r_Aligned_Tiling::description + CommandLineParameter::descLineSep +
                                 "... for " + tiling_name_interesttiling + CommandLineParameter::descLineSep +
                                 r_Interest_Tiling::description + CommandLineParameter::descLineSep +
                                 "... for " + tiling_name_directionaltiling + CommandLineParameter::descLineSep +
                                 r_Dir_Tiling::description + CommandLineParameter::descLineSep +
                                 "... for " + tiling_name_statisticaltiling + CommandLineParameter::descLineSep +
                                 r_Stat_Tiling::description;

bool
SystemBasic::handleSignal = true;

int
SystemBasic::parseParams(int argc, char** argv)
{
    int retval = 0;
    //program interface
    CommandLineParser    &cmlInter      = CommandLineParser::getInstance();
    CommandLineParameter &clp_help      = cmlInter.addFlagParameter('h', "help", "show command line switches");
    CommandLineParameter &clp_tiling    = cmlInter.addStringParameter(CommandLineParser::noShortName, "tiling", tilingDesc.c_str(), tiling_name_sizetiling);
    CommandLineParameter &tilingParams  = cmlInter.addStringParameter(CommandLineParser::noShortName, "tilingparams", tilingParamsDesc.c_str(), "131072");
    CommandLineParameter &clp_testbed   = cmlInter.addFlagParameter(CommandLineParser::noShortName, "testbed", "testbed output flag.");
    CommandLineParameter &clp_transparent   = cmlInter.addFlagParameter('t', "transparent", "transparent update flag.\n\t\tIf it is specified black data will be treated as transparent");
    CommandLineParameter &clp_printtext = cmlInter.addFlagParameter(CommandLineParser::noShortName, "printtext", "output of char data will be printed as text - not numbers.\n\t\tUseful for printing the names of all collections/or text stored in mdds");
    CommandLineParameter &clp_server    = cmlInter.addStringParameter('s', "server", "<host-name> host's name where rasmgr runs.", serverName);
    CommandLineParameter &clp_port      = cmlInter.addStringParameter('p', "port", "<nnnn> port number used by rasmgr.", "7001");
    CommandLineParameter &clp_database  = cmlInter.addStringParameter('d', "database", "<db-name> name of database.", baseName);
    CommandLineParameter &clp_user      = cmlInter.addStringParameter(CommandLineParser::noShortName, "user", "<user-name> name of user", userName);
    CommandLineParameter &clp_passwd    = cmlInter.addStringParameter(CommandLineParser::noShortName,"passwd", "<user-passwd> password of user", passwd);
    CommandLineParameter &clp_storage   = cmlInter.addStringParameter(CommandLineParser::noShortName, "storageformat", "<format> storage format.", format_name_array);
    CommandLineParameter &clp_storageParams = cmlInter.addStringParameter(CommandLineParser::noShortName, "storageformatparams", "<params> parameters used for storing", NULL);
    CommandLineParameter &clp_transfer  = cmlInter.addStringParameter(CommandLineParser::noShortName, "transformat", "<format> transfer format.", format_name_array );
    CommandLineParameter &clp_transferParams    = cmlInter.addStringParameter(CommandLineParser::noShortName, "transformatparams", "<params> parameters used for transfer", NULL);
    CommandLineParameter &clp_input     = cmlInter.addStringParameter(CommandLineParser::noShortName, "inputformat", "<format> input format.", format_name_array );
    CommandLineParameter &clp_inputParams   = cmlInter.addStringParameter(CommandLineParser::noShortName, "inputformatparams", "<params> parameters used for input", NULL);
    CommandLineParameter &clp_output    = cmlInter.addStringParameter(CommandLineParser::noShortName, "outputformat", "<format> output format.", format_name_array );
    CommandLineParameter &clp_outputParams  = cmlInter.addStringParameter(CommandLineParser::noShortName, "outputformatparams", "<params> parameters used for output", NULL);
    CommandLineParameter &clp_outputFileName    = cmlInter.addStringParameter(CommandLineParser::noShortName, "outputfilename", "<name> file name of output file", NULL);
    CommandLineParameter &clp_polygon   = cmlInter.addStringParameter(CommandLineParser::noShortName, "polygon", "<polygon-path> polygon path used for delimiting the data.  The polygon has to be given in counter clockwise direction.  Do not forget to supply insidepattern and/or outsidepattern", NULL);
    CommandLineParameter &clp_shrink    = cmlInter.addStringParameter(CommandLineParser::noShortName, "shrink", "<number> pixels that will reduce the polygon.", "0");
    CommandLineParameter &clp_fillinside    = cmlInter.addStringParameter(CommandLineParser::noShortName, "outsidepattern", "<pattern> will be used to fill the outside of the polygon.", NULL);
    CommandLineParameter &clp_filloutside   = cmlInter.addStringParameter(CommandLineParser::noShortName, "insidepattern", "<pattern> will be used to fill the inside of the polygon.", NULL);
    CommandLineParameter &clp_fillinsidesel = cmlInter.addStringParameter(CommandLineParser::noShortName, "outsidepatternbackdrop", "<pattern> will be used to fill the outside of the polygon in the backdrop mdd.", NULL);
    CommandLineParameter &clp_filloutsidesel    = cmlInter.addStringParameter(CommandLineParser::noShortName, "insidepatternbackdrop", "<pattern> will be used to fill the inside of the polygon in the backdrop mdd.", NULL);
    CommandLineParameter &clp_collType  = cmlInter.addStringParameter(CommandLineParser::noShortName, "colltype", "<coll-type> type of collection");
    CommandLineParameter &clp_mddType   = cmlInter.addStringParameter(CommandLineParser::noShortName, "mddtype", "<mdd-type> type of marray");
    CommandLineParameter &clp_collName  = cmlInter.addStringParameter('c', "collname", "<coll-name> name of collection");
    CommandLineParameter &clp_mddoid    = cmlInter.addStringParameter(CommandLineParser::noShortName, "mddoid","<mdd-oid> oid of the marray to work on");
    CommandLineParameter &clp_colloid   = cmlInter.addStringParameter(CommandLineParser::noShortName, "colloid","<coll-oid> oid of the set to work on");
    CommandLineParameter &clp_mddDomain = cmlInter.addStringParameter(CommandLineParser::noShortName, "mdddomain","<mdd-domain> domain of marray");
    CommandLineParameter &clp_overlayDomain = cmlInter.addStringParameter(CommandLineParser::noShortName, "overlaydomain","<overlay-domain> domain that is updated in the db in case of an overlay, default is the domain of the image");
    CommandLineParameter &clp_align     = cmlInter.addStringParameter(CommandLineParser::noShortName, "align","<pixels> number of pixels to align the domain to.  May be used instead of overlaydomain.", "0");
    CommandLineParameter &clp_tiledupdate   = cmlInter.addFlagParameter(CommandLineParser::noShortName, "tiledupdate","do job in smaller pieces.  Use update buffer size to compute the size of each update.  May not be used with a file insertion!");
    CommandLineParameter &clp_read      = cmlInter.addStringParameter('r', "read", "<file-name> name of input file");
    CommandLineParameter &clp_conversiontypename    = cmlInter.addStringParameter(CommandLineParser::noShortName, "conversiontype","<type-name> name of type to be passed to the conversion module");
    CommandLineParameter &clp_scalelevels   = cmlInter.addStringParameter(CommandLineParser::noShortName, "scalelevels","<level-spec> list of scale levels: collection:1/scalefactor;...\n\t\te.g. coll:1;coll_1:2;coll_3:4;coll_4:8");
    string t = string("") + "<function-name> name of the scaling algorithm.\n\t\teither " + scale_function_name_subsampling + " or " + scale_function_name_bitaggregation;
    CommandLineParameter &clp_updatebuffer  = cmlInter.addStringParameter(CommandLineParser::noShortName, "buffersize", "<buffer-size> number of cells that can be written in one go when using tiledupdate.  If align is defined then the minimum update size is taken to be the value align in each dimension.", defaultUpdateBufferSize);
    CommandLineParameter &cml_layerlist     = cmlInter.addStringParameter(CommandLineParser::noShortName, "layerlist", "<list> list of files for merging: <\"layer[[;layer]...]\"> \n\t\tlayer:  <filename:TruePattern:FalsePattern>\n\t\tTruePattern / FalsePattern:\n\t\tbinary pattern expressed as decimal number(e.g.: 01001 = 9)");
    CommandLineParameter &clp_scalefunction = cmlInter.addStringParameter(CommandLineParser::noShortName, "scalefunction", t.c_str(), scale_function_name_subsampling);
    CommandLineParameter &clientcommSleep   = cmlInter.addStringParameter(CommandLineParser::noShortName, "clientcommsleep", "<seconds> number of seconds to wait till retry", "1");
    CommandLineParameter &clientcommMaxRetry= cmlInter.addStringParameter(CommandLineParser::noShortName, "clientcommretry", "<tries> number of retries before giving up", "10");
    CommandLineParameter &rpcMaxRetry   = cmlInter.addStringParameter(CommandLineParser::noShortName, "rpcretry", "<tries> number of retries before giving up", "5");
    CommandLineParameter &clp_Force     = cmlInter.addFlagParameter(CommandLineParser::noShortName, "force", "force writing of files");
    try
    {
        cmlInter.processCommandLine(argc, argv);
    }
    catch(CmlException& err)
    {
        cout << usageHeader;
        cmlInter.printHelp();
        cout << usageFooter;
        cout << "Error parsing command line:" << endl;
        cout << err.what() << endl;
        return ERRORPARSINGCOMMANDLINE;
    }
    try
    {
        if (cmlInter.isPresent('h'))
        {
            cout << usageHeader;
            cmlInter.printHelp();
            cout << usageFooter;
            return ALLDONE;
        }
        r_Tiling_Scheme ts = get_tiling_scheme_from_name(cmlInter.getValueAsString("tiling"));
        switch (ts)
        {
        case r_NoTiling:
            theTiling = new r_No_Tiling();
            break;
        case r_StatisticalTiling:
            theTiling = new r_Stat_Tiling(cmlInter.getValueAsString("tilingparams"));
            break;
        case r_InterestTiling:
            theTiling = new r_Interest_Tiling(cmlInter.getValueAsString("tilingparams"));
            break;
        case r_AlignedTiling:
            theTiling = new r_Aligned_Tiling(cmlInter.getValueAsString("tilingparams"));
            break;
        case r_DirectionalTiling:
            theTiling = new r_Dir_Tiling(cmlInter.getValueAsString("tilingparams"));
            break;
        case r_SizeTiling:
            theTiling = new r_Size_Tiling(cmlInter.getValueAsString("tilingparams"));
            break;
        case r_RegularTiling:
            return TILINGTYPENOTALLOWED;
        default:
            return UNKNOWNTILINGTYPE;
        }
        if (cmlInter.isPresent("scalelevels"))
        {
            retval = readScaleLevels(cmlInter.getValueAsString("scalelevels"));
            if (retval != 0)
                return retval;
        }
        testBed = cmlInter.isPresent("testbed");
        force = cmlInter.isPresent("force");
        transparent = cmlInter.isPresent("transparent");
        printText = cmlInter.isPresent("printtext");
        serverName = cmlInter.getValueAsString("server");
        serverPort = cmlInter.getValueAsLong("port");
        baseName = cmlInter.getValueAsString("database");
        userName = cmlInter.getValueAsString("user");
        passwd = cmlInter.getValueAsString("passwd");
        storageFormat = get_data_format_from_name(cmlInter.getValueAsString("storageformat"));
        storageFormatParams = cmlInter.getValueAsString("storageformatparams");
        transferFormat = get_data_format_from_name(cmlInter.getValueAsString("transformat"));
        transferFormatParams = cmlInter.getValueAsString("transformatparams");
        inputFormat = get_data_format_from_name(cmlInter.getValueAsString("inputformat"));
        inputFormatParams = cmlInter.getValueAsString("inputformatparams");
        outputFormat = get_data_format_from_name(cmlInter.getValueAsString("outputformat"));
        outputFormatParams = cmlInter.getValueAsString("outputformatparams");
        outputFileName = cmlInter.getValueAsString("outputfilename");
        RMInit::clientcommMaxRetry = clientcommMaxRetry.getValueAsLong();
        RMInit::logOut << "Clientcomm retry set to " << RMInit::clientcommMaxRetry << endl;
        RMInit::clientcommSleep = clientcommSleep.getValueAsLong();
        RMInit::logOut << "Clientcomm sleep set to " << RMInit::clientcommSleep << endl;
        RMInit::rpcMaxRetry = rpcMaxRetry.getValueAsLong();
        RMInit::logOut << "RPC retry set to " << RMInit::rpcMaxRetry << endl;
        if (cmlInter.isPresent("polygon"))
        {
            try
            {
                r_Polygon p(cmlInter.getValueAsString("polygon"));
                polygon.addPolygon(p);
                polygonDefined = true;
            }
            catch (r_Error& err)
            {
                RMInit::logOut << "Error reading the polygon: " << err.get_errorno() << " " << err.what() << endl;
                return POLYGONCREATIONFAILED;
            }
        }
        polygonShrinker = cmlInter.getValueAsLong("shrink");
        if (cmlInter.isPresent("outsidepattern"))
        {
            outsidePatternDef = true;
            outsidePattern = HexCodec::convertTo(cmlInter.getValueAsString("outsidepattern"));
        }
        if (cmlInter.isPresent("insidepattern"))
        {
            insidePatternDef = true;
            insidePattern = HexCodec::convertTo(cmlInter.getValueAsString("insidepattern"));
        }
        if (cmlInter.isPresent("outsidepatternbackdrop"))
        {
            outsidePatternSelDef = true;
            outsidePatternSel = HexCodec::convertTo(cmlInter.getValueAsString("outsidepatternbackdrop"));
        }
        if (cmlInter.isPresent("insidepatternbackdrop"))
        {
            insidePatternSelDef = true;
            insidePatternSel = HexCodec::convertTo(cmlInter.getValueAsString("insidepatternbackdrop"));
        }
        setTypeName = cmlInter.getValueAsString("colltype");
        mddTypeName = cmlInter.getValueAsString("mddtype");
        collName = cmlInter.getValueAsString("collname");
        if (cmlInter.isPresent("mddoid"))
        {
            mddOIdDef = true;
            mddOId = r_OId(cmlInter.getValueAsString("mddoid"));
        }
        if (cmlInter.isPresent("colloid"))
        {
            collOIdDef = true;
            cmlInter.getValueAsString("colloid");
        }
        if (cmlInter.isPresent("mdddomain"))
        {
            mddDomainDef = true;
            mddDomain = r_Minterval(cmlInter.getValueAsString("mdddomain"));
        }
        if (cmlInter.isPresent("overlaydomain"))
        {
            overlayDomainDef = true;
            overlayDomain = r_Minterval(cmlInter.getValueAsString("overlaydomain"));
        }
        else
        {
            overlayDomain = mddDomain;
        }
        align = cmlInter.getValueAsLong("align");
        if (align > 0)
        {
            overlayDomainDef = true;
            for (r_Dimension dim = 0; dim < overlayDomain.dimension(); dim++)
            {
                r_Range high = overlayDomain[dim].high();
                r_Range low = overlayDomain[dim].low();

                r_Range highd = high%align;
                if (high < 0)
                    high = high - highd - 1;
                else
                    high = high + align - highd - 1;

                r_Range lowd = overlayDomain[dim].low()%align;
                if (low < 0)
                    low = low - align - lowd;
                else
                    low = low - lowd;
                overlayDomain[dim].set_interval(low, high);
            }
            RMInit::logOut << "Alignment changed overlay domain to " << overlayDomain << endl;
        }
        fileName = cmlInter.getValueAsString("read");
        tiledUpdate = cmlInter.isPresent("tiledupdate");
        if (tiledUpdate && fileName)
        {
            return TILEDUPDATEANDFILE;
        }
        conversionTypeName = cmlInter.getValueAsString("conversiontype");
        scaleFunction = get_scale_function_from_name(cmlInter.getValueAsString("scalefunction"));
        updateBufferSize = cmlInter.getValueAsLong("buffersize");
        if (cmlInter.isPresent("layerlist"))
        {
            const char* endPos = NULL;
            char* layerName = NULL;
            const char* patternTName = NULL;
            unsigned int patternT = 0;
            unsigned int patternF = 0;
            bool found = false;
            size_t length = 0;
            const char* startPos = cmlInter.getValueAsString("layerlist");
            while (true)
            {
                endPos = index(startPos, ':');
                if (endPos == NULL)
                {
                    return LAYERSNOTCORRECT;
                }
                length = endPos - startPos;
                layerName = new char[length + 1];
                memset(layerName, 0, length + 1);
                strncpy(layerName, startPos, length);
                startPos = endPos + 1;
                endPos = index(startPos, ':');
                patternT = (unsigned int)atol(startPos);
                startPos = endPos + 1;
                endPos = index(startPos, ';');
                patternF = (unsigned int)atol(startPos);
                layerList.push_back(layerName);
                patternsTrue.push_back(patternT);
                patternsFalse.push_back(patternF);
                RMInit::logOut << "Layer level               : " << layerName << " (" << patternT << "," << patternF << ")" << endl;
                if (endPos == NULL)
                {
                    break;
                }
                startPos = endPos + 1;
            }
            RMInit::logOut << "Layer list                : ";
            std::list<char*>::iterator iterL = layerList.begin();
            std::list<unsigned int>::iterator iterT = patternsTrue.begin();
            std::list<unsigned int>::iterator iterF = patternsFalse.begin();
            while (iterL != layerList.end())
            {
                RMInit::logOut << *iterL << "(" << *iterT << "," << *iterF << ") ";
                iterL++;
                iterT++;
                iterF++;
            }
            RMInit::logOut << std::endl;
            if (layerList.size() > (sizeof(r_ULong) * 8))
            {
                std::list<char*>::iterator iterL = layerList.begin();
                while (iterL != layerList.end())
                {
                    delete[] *iterL;
                    ++iterL;
                }
                return TOOMANYLAYERS;
            }
        }
    }
    catch(CmlException& err)
    {
        cout << usageHeader;
        cmlInter.printHelp();
        cout << usageFooter;
        cout << "Error evaluating command line:" << endl;
        cout << err.what() << endl;
        return ERRORPARSINGCOMMANDLINE;
    }
    return retval;
}

int
SystemBasic::saveData(const char* fileNamePat, const char* data, r_Bytes length, const r_Minterval& dom)
{
    RMInit::logOut << "SystemBasic::saveData(" << fileNamePat << ", DATA, " << length << ", " << dom << ")" << std::endl;
    r_Primitive_Type* tp = new r_Primitive_Type("Char", r_Type::CHAR);
    r_Convertor* conv = r_Convertor_Factory::create(r_PNG, data, dom, tp);
    r_convDesc desc = conv->convertTo(NULL);
    size_t dtaSize=desc.destInterv.cell_count()*tp->size();
    std::ofstream o;
    o.open(fileNamePat);
    if (!o.is_open())
    {
        RMInit::logOut << "unable to open file " << fileNamePat << " for writing" << std::endl;
        return FILEINACCESSIBLE;
    }
    o.write(desc.dest, dtaSize);
    //fwrite(desc.dest, 1, dtaSize, tfile);
    o.flush();
    o.close();
    free(desc.dest);
    delete desc.destType;
    delete conv;
    delete tp;
    return 0;
}

void
SystemBasic::openTransaction(bool readwrite) throw (r_Error)
{
    db.set_servername(serverName, serverPort);
    db.set_useridentification(userName, passwd);
    db.open(baseName);
    if (readwrite)
        ta.begin(r_Transaction::read_write);
    else
        ta.begin(r_Transaction::read_only);
    db.set_transfer_format(transferFormat, transferFormatParams);
    db.set_storage_format(storageFormat, storageFormatParams);
}

void
SystemBasic::printScalar(const r_Scalar& scalar)
{
    switch (scalar.get_type()->type_id())
    {
    case r_Type::BOOL:
        cout << (((r_Primitive*)&scalar)->get_boolean() ? "T" : "F") << std::flush;
        break;

    case r_Type::CHAR:
        cout << (int)((r_Primitive*)&scalar)->get_char() << std::flush;
        break;

    case r_Type::OCTET:
        cout << (int)((r_Primitive*)&scalar)->get_octet() << std::flush;
        break;

    case r_Type::SHORT:
        cout << ((r_Primitive*)&scalar)->get_short() << std::flush;
        break;

    case r_Type::USHORT:
        cout << ((r_Primitive*)&scalar)->get_ushort() << std::flush;
        break;

    case r_Type::LONG:
        cout << ((r_Primitive*)&scalar)->get_long() << std::flush;
        break;

    case r_Type::ULONG:
        cout << ((r_Primitive*)&scalar)->get_ulong() << std::flush;
        break;

    case r_Type::FLOAT:
        cout << ((r_Primitive*)&scalar)->get_float() << std::flush;
        break;

    case r_Type::DOUBLE:
        cout << ((r_Primitive*)&scalar)->get_double() << std::flush;
        break;

    case r_Type::COMPLEXTYPE1:
    case r_Type::COMPLEXTYPE2:
        cout << "(" << ((r_Complex*)&scalar)->get_re() << ", " << ((r_Complex*)&scalar)->get_im() << ")" << std::flush;
        break;

    case r_Type::STRUCTURETYPE:
    {
        r_Structure* structValue = (r_Structure*)&scalar;
        cout << "	{ " << std::flush;
        for(int i=0; i<structValue->count_elements(); i++)
        {
            printScalar((*structValue)[i]);
            if (i < structValue->count_elements()-1) cout << ", " << std::flush;
        }
        cout << " 	}" << endl;
    }
    break;
    }
}

int
SystemBasic::convertFrom(r_Data_Format fmt, char*& src, size_t& dtaSize, r_Minterval& interv, r_Base_Type*& tp, const char* options)
{
    int retval = 0;

    r_Minterval tmpInt(1);
    tmpInt << r_Sinterval((r_Range)0, (r_Range)dtaSize - 1);

    RMInit::logOut << "convertFrom(...) " << endl;
    RMInit::logOut << "domain in      : " << tmpInt << endl;
    RMInit::logOut << "type   in      : ";
    tp->print_status(RMInit::logOut);
    RMInit::logOut << endl;

    if (fmt != r_Array)
    {
        try
        {
            r_Convertor* conv = r_Convertor_Factory::create(fmt, src, tmpInt, tp);
            r_Storage_Man_CPP mySM;
            conv->set_storage_handler(mySM);
            r_convDesc desc = conv->convertFrom(options);

            if (desc.destType->isBaseType())
            {
                delete tp;
                tp = (r_Base_Type*)desc.destType;
                tmpInt = desc.destInterv;
                dtaSize=tmpInt.cell_count()*tp->size();
                delete [] src;
                src = desc.dest;
                delete conv;
                conv = 0;
                if (interv.dimension() != 0)
                {
                    if (tmpInt.cell_count() != interv.cell_count())
                    {
                        RMInit::logOut << "Domains do not have the same number of cells (mdd " << interv << ", data " << tmpInt << ")" << endl;
                        retval = DOMAINDATAMISMATCH;
                    }
                    else
                    {
                        if (tmpInt.get_extent() != interv.get_extent())
                        {
                            RMInit::logOut << "Domains do not have the same extents (mdd " << interv << ", data " << tmpInt << ")" << endl;
                            retval = DOMAINDATAMISMATCH;
                        }
                    }
                }
                else
                {
                    interv = tmpInt;
                }
            }
            else
            {
                RMInit::logOut << "Error conversion type is not a base type" << endl;
                retval = CONVERSIONRETURNEDWRONGTYPE;
            }
        }
        catch (r_Error& obj)
        {
            RMInit::logOut << "Error (" << obj.get_errorno() << ") when converting from " << fmt << " : " << obj.what() << endl;
            retval = CONVERSIONEXCEPTION;
        }
    }
    else
    {
        if (dtaSize != (tp->size() * interv.cell_count()))
        {
            RMInit::logOut << "base type size " << tp->size() << " cells " << interv.cell_count() << " does not match size " << dtaSize << endl;
            retval = DOMAINDATAMISMATCH;
        }
    }

    RMInit::logOut << "domain out     : " << interv << endl;
    RMInit::logOut << "type   out     : ";
    tp->print_status(RMInit::logOut);
    RMInit::logOut << endl;

    return retval;
}

int
SystemBasic::convertTo(r_Data_Format fmt, char*& src, size_t& dtaSize, r_Minterval& interv, r_Base_Type*& tp, const char* options)
{
    int retval = 0;

    RMInit::logOut << "convertTo(...)"  << endl;
    RMInit::logOut << "domain in    : " << interv << endl;
    RMInit::logOut << "type   in    : " ;
    tp->print_status(RMInit::logOut);
    RMInit::logOut << endl;

    if (fmt != r_Array)
    {
        try
        {
            r_Convertor* conv = r_Convertor_Factory::create(fmt, src, interv, tp);
            r_convDesc desc = conv->convertTo(options);
            RMInit::logOut << "Conversion type          : ";
            desc.destType->print_status(RMInit::logOut);
            RMInit::logOut << endl;
            if (desc.destType->isBaseType())
            {
                delete tp;
                tp = (r_Base_Type*)desc.destType;
            }
            else
            {
                RMInit::logOut << "Error conversion type is not a base type" << endl;
                throw r_Error();
            }
            dtaSize = desc.destInterv.cell_count() * tp->size(); // desc.destInterv[0].high() - desc.destInterv[0].low() + 1;
            interv=desc.destInterv;
            delete src;
            src = desc.dest;
            delete conv;
            conv = 0;
        }
        catch (r_Error& obj)
        {
            RMInit::logOut << "Error (" << obj.get_errorno() << ") when converting to " << fmt << " : " << obj.what() << endl;
            retval = CONVERSIONEXCEPTION;
        }
    }
    else
    {
        if (dtaSize != (tp->size() * interv.cell_count()))
        {
            RMInit::logOut << "base type size " << tp->size() << " cells " << interv.cell_count() << " does not match size " << dtaSize << endl;
            retval = DOMAINDATAMISMATCH;
        }
    }

    RMInit::logOut << "domain out    : " << interv << endl;
    RMInit::logOut << "type   out    : " ;
    tp->print_status(RMInit::logOut);
    RMInit::logOut << endl;

    return retval;
}

FILE*
SystemBasic::checkFile(const char* fileN, int& retval)
{
    FILE* fileD = NULL;
    if ((fileD = fopen(fileN, "r")) == NULL)
        retval = FILEINACCESSIBLE;
    return fileD;
}

char*
SystemBasic::getData(FILE* file, size_t& dtaSize, int& retval)
{
    char* dta = 0;
    size_t size = 0;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    RMInit::logOut << "getData(...)" << endl;
    RMInit::logOut << "File size 	: " << size << endl;
    try
    {
        dta = new char[size];
    }
    catch(std::bad_alloc)
    {
        RMInit::logOut << "Unable to claim memory(" << size << "B)!"  << endl;
        retval = UNABLETOCLAIMRESOURCEFORFILE;
        return dta;
    }

    fseek(file, 0, SEEK_SET);
    fread(dta, 1, size, file);
    fclose(file);
    dtaSize = size;
    return dta;
}

int
SystemBasic::readScaleLevels(const char* startPos)
{
    int retval = 0;
    scaleLevels = new std::list<std::pair<double, char*> >();
    const char* endPos = NULL;
    char* levelName = NULL;
    const char* factorName = NULL;
    double factor = 0;
    bool found = false;
    size_t length = 0;
    while (true)
    {
        endPos = index(startPos, ':');
        if (endPos == NULL)
        {
            retval = SCALELEVELSINCORRECT;
            delete scaleLevels;
            scaleLevels = NULL;
            break;
        }
        length = endPos - startPos;
        levelName = new char[length + 1];
        memset(levelName, 0, length + 1);
        strncpy(levelName, startPos, length);
        startPos = endPos + 1;
        endPos = index(startPos, ';');
        factor = atof(startPos);
        factor = 1 / factor;
        scaleLevels->push_back(std::pair<double, char*>(factor, levelName));
        RMInit::logOut << "Scale level               : " << levelName << " (" << factor << ")" << endl;
        if (endPos == NULL)
        {
            break;
        }
        startPos = endPos + 1;
    }
    return retval;
}

void
SystemBasic::explainRetval(int retval)
{
    switch (retval)
    {
    case  EXCEPTIONEXECUTEQUERY:
        cout << "An exception occured during execution of the query" << endl;
        cout << "Check the RasDaMan exception." << endl;
        break;
    case  TILINGPATAMETERSINCORRECT:
        cout << "Tiling parameters incorrect" << endl;
        cout << "Check the specification of the tiling parameters for the tiling strategy you chose." << endl;
        break;
    case  UNKNOWNTILINGTYPE:
        cout << "Unknown tiling type specified" << endl;
        cout << "Please review your --tiling option." << endl;
        break;
    case  TILINGPARAMETERSMISSING:
        cout << "You specified a tiling option which requires additional parameters" << endl;
        cout << "Please specify a --tilingparameter option.  For information on the format consult the --help option.  For information on the meaning of the parameters consult the RasDaMan c++ documentation of the apropriate tiling." << endl;
        break;
    case CONVERSIONEXCEPTION:
        cout << "An exception occured during conversion" << endl;
        break;
    case CONVERSIONNOTSUPPORTED:
        cout << "The specified conversion format is not supported" << endl;
        break;
    case NOQUERY:
        cout << "Cannot read query from file" << endl;
        break;
    case ARGUMENTSMISSING:
        cout << "There are arguments missing" << endl;
        cout << "You must specify --arg1 in order to specify --arg2 ..." << endl;
        break;
    case ARGUMENTSINCOMPLETE:
        cout << "There are arguments missing" << endl;
        cout << "You must specify --arg? --arg?domain --arg?type, where ? can be 1, 2, 3 or 4" << endl;
        break;
    case CREATEANDNOCREATE:
        cout << "You specified create and no create" << endl;
        cout << "Please decide on one." << endl;
        break;
    case NOVALIDDOMAIN:
        cout << "No valid domain" << endl;
        cout << "Please use something like \"[0:*,-2:9]\"." << endl;
        break;
    case FILEINACCESSIBLE:
        cout << "Cannot access one of the files" << endl;
        break;
    case FILENAMETOLONG:
        cout << "File name too long" << endl;
        break;
    case NOCREATECREATECOLL:
        cout << "You specified to not create a collection, but want to create one" << endl;
        break;
    case NOCOLLTYPE:
        cout << "No collection type specified" << endl;
        break;
    case COLLOIDANDCOLLNAME:
        cout << "Do not specify collection name and collection oid" << endl;
        break;
    case CREATECOLLWITHOID:
        cout << "Cannot use user specified oid to create collection" << endl;
        break;
    case CREATEMDDWITHOID:
        cout << "Cannot use user specified oid to create marray" << endl;
        break;
    case INVALIDTILESIZE:
        cout << "Tile size is invalid" << endl;
        break;
    case DOMAINDATAMISMATCH:
        cout << "Domain extent does not match size of data file" << endl;
        break;
    case MDDDOMAINNOTSPECIFIED:
        cout << "Domain for marray not specified" << endl;
        break;
    case FILENAMENOTSPECIFIED:
        cout << "File name not specified" << endl;
        break;
    case NOCOLLNAMENOCOLLOID:
        cout << "Neither collection name nor collection oid specified" << endl;
        break;
    case MDDTYPEINVALID:
        cout << "Marray type not valid" << endl;
        break;
    case NOBASETYPE:
        cout << "No base type in marray type" << endl;
        break;
    case EXCEPTIONCREATECOLL:
        cout << "Exception while creating collection" << endl;
        break;
    case EXCEPTIONADMIN:
        cout << "Exception while performing administrativ action" << endl;
        break;
    case COLLECTIONINACCESSIBLE:
        cout << "Collection not accessible" << endl;
        break;
    case OIDINVALID:
        cout << "Specified oid is invalid" << endl;
        cout << "Please use something like \"Oracle|RASBASE|1025\"." << endl;
        break;
    case MDDINACCESSIBLE:
        cout << "Marray not accessible" << endl;
        break;
    case MDDOIDANDCOLL:
        cout << "MDD oid and collection specified" << endl;
        cout << "I can either update a whole collection or just one marray." << endl;
        break;
    case CONVERSIONTYPENOTABASETYPE:
        cout << "Conversion type is not a base type." << endl;
        cout << "Please supply a valid base type as a conversion type." << endl;
        break;
    case CONVERSIONRETURNEDWRONGTYPE:
        cout << "The conversion module returned a type that was not a base type." << endl;
        cout << "Please contact customer support." << endl;
        break;
    case POLYGONCREATIONFAILED:
        cout << "The string passed to the polygon switch was wrong." << endl;
        break;
    case OVERLAYDOMAINSDONOTMATCH:
        cout << "The domains of the MDD objects to overlay do not match." << endl;
        break;
    case OVERLAYTYPESIZESDONOTMATCH:
        cout << "The type sizes of the MDD objects to overlay do not match." << endl;
        break;
    case POLYGONDOMAINTOOLARGE:
        cout << "The domain of the MDD object does not cover the bounding box of the polygon." << endl;
        break;
    case NOCOLLNAMEDEFINED:
        cout << "No collection name defined." << endl;
        break;
    case NOMDDTYPEDEFINED:
        cout << "No mdd type defined." << endl;
        break;
    case MDDTYPEOFGMARRAYNOTINITIALISED:
        cout << "MDD type of GMarray not initialised." << endl;
        break;
    case SCALEDOMAINISNOTCORRECT:
        cout << "Scale domain is not correct." << endl;
        break;
    case SCALELEVELSINCORRECT:
        cout << "Scale levels are not correct." << endl;
        break;
    case GMARRAYSARENOTEQUAL:
        cout << "The GMarrays were not equal." << endl;
        break;
    case UNKNOWNSCALEFUNCTION:
        cout << "The supplied scale function is unknown." << endl;
        break;
    case UNABLETOCLAIMRESOURCEFORFILE:
        cout << "Unable to claim memory for the input files." << endl;
        break;
    case ERRORPARSINGCOMMANDLINE:
        cout << "Error parsing the command line." << endl;
        break;
    case TILINGTYPENOTALLOWED:
        cout << "The tiling type is not allowed." << endl;
        break;
    case LAYERSNOTCORRECT:
        cout << "The layer parameter is not correct." << endl;
        break;
    case TOOMANYLAYERS:
        cout << "There are too many layers defined for the merging." << endl;
        break;
    case OVERLAYDOMAINDOESNOTMATCH:
        cout << "The overlay domain does not cover the domain of the input file." << endl;
        break;
    case TILEDUPDATEANDFILE:
        cout << "Option tiledupdate and file specified." << endl;
        break;
    case NOCOMPAREDESTINATION:
        cout << "Need either a file or a collection name to get the source data for the compare operation." << endl;
        break;
    case CONVERSIONTYPEMISSING:
        cout << "Conversion type is missing." << endl;
        break;
    default :
        cout << "Unknown execution code: " << retval << endl;
        break;
    case  ALLDONE:
    case 0:
        cout << "No errors" << endl;
    }
    delete theTiling;
    theTiling = NULL;
    if (scaleLevels != NULL)
    {
        std::list<std::pair<double, char*> >::iterator iter = scaleLevels->begin();
        std::list<std::pair<double, char*> >::iterator end = scaleLevels->end();
        for (; iter != end; iter++)
        {
            delete [] (*iter).second;
            (*iter).second = NULL;
        }
        delete scaleLevels;
        scaleLevels = NULL;
    }
}

int
SystemBasic::initGMarray(r_Ref<r_GMarray>& tempMDD, FILE* tempFile, r_Data_Format conversionFormat, r_Minterval& tempDataDomain, const char* conversionParams) throw (r_Error)
{
    int retval = 0;
    char* mddData = NULL;
    size_t mddDataSize = 0;
    r_Type* tempType = NULL;
    r_Base_Type* conversionType = NULL;
    const r_Base_Type* tempBaseType = (r_Base_Type*)tempMDD->get_base_type_schema();
    if (tempBaseType != NULL)
    {
        mddData = getData(tempFile, mddDataSize, retval);
        if (retval != 0)
            return retval;
        try
        {
            tempType = r_Type::get_any_type(conversionTypeName);
        }
        catch (const r_Error& err)
        {
            RMInit::logOut << "Resolving conversion type: Exception in get_any_type: " << err.get_errorno() << " " << err.what() << endl;
            delete tempType;
            tempType = NULL;
            retval = CONVERSIONTYPENOTABASETYPE;
        }
        if (!tempType->isBaseType())
        {
            retval = CONVERSIONTYPENOTABASETYPE;
            delete tempType;
            tempType = NULL;
        }
        else
        {
            conversionType = (r_Base_Type*)tempType;
            retval = convertFrom(conversionFormat, mddData, mddDataSize, tempDataDomain, conversionType, conversionParams);
            if (retval == 0)
            {
                tempMDD->set_array_size(mddDataSize);
                tempMDD->set_type_length(conversionType->size());
                delete [] tempMDD->get_array();
                tempMDD->set_array(mddData);
                mddData = 0;
                if (polygonDefined)
                {
                    /* should be done in r_PolygonCutOut
                                        if (tempMDD->spatial_domain().covers(polygon.getBoundingBox()))
                                            {
                    */
                    polygon.setMArray(*tempMDD);
                    if (insidePatternDef)
                        polygon.fillMArrayInside(insidePattern);
                    if (outsidePatternDef)
                        polygon.fillMArrayOutside(outsidePattern);
                    /* should be done in r_PolygonCutOut
                                            }
                                        else    {
                                            RMInit::logOut << "Polygon Bounding Box " << polygon.getBoundingBox() << " GMarray Bounding Box " << tempMDD->spatial_domain() << std::endl;
                                            retval = POLYGONDOMAINTOOLARGE;
                                            }
                    */
                }
            }
            else
            {
                delete [] mddData;
                mddData = 0;
            }
            delete conversionType;
            conversionType = NULL;
        }
    }
    else
    {
        retval = MDDTYPEOFGMARRAYNOTINITIALISED;
    }
    return retval;
}

int
SystemBasic::compareGMarrays(const r_Ref<r_GMarray>& baseMDD, r_Ref<r_GMarray>& topMDD)
{
    int retval = 0;
    //r_GMarray* baseMDD = (r_GMarray*)&(*baseMDD2);
    const r_Minterval& tempDomain = baseMDD->spatial_domain();
    char* topMDDCells = topMDD->get_array();
    const char* baseMDDCells = baseMDD->get_array();
    wrongBytes = 0;

    RMInit::logOut << "compareGMarrays: " << std::endl;
    RMInit::logOut << "baseMDD domain: " << baseMDD->spatial_domain() << " type length: " << baseMDD->get_type_length() << std::endl;
    RMInit::logOut << "topMDD  domain: " << topMDD->spatial_domain() << " type length: " << topMDD->get_type_length() << std::endl;
    if (tempDomain == topMDD->spatial_domain())
    {
        r_Bytes typeLen = baseMDD->get_type_length();
        if (typeLen == topMDD->get_type_length())
        {
            r_Area numberCells = tempDomain.cell_count();

            for (size_t elemNum = 0; elemNum < numberCells; elemNum++)
            {
                if (memcmp(&(baseMDDCells[elemNum * typeLen]), &(topMDDCells[elemNum * typeLen]), typeLen) != 0)
                {
                    wrongBytes++;
                }
            }
            wrongBytes*=typeLen;
        }
        else
        {
            retval = OVERLAYTYPESIZESDONOTMATCH;
        }
    }
    else
    {
        retval = OVERLAYDOMAINSDONOTMATCH;
    }
    if (wrongBytes != 0)
    {
        retval = GMARRAYSARENOTEQUAL;
    }
    return retval;
}

int
SystemBasic::overlayGMarrays(r_Ref<r_GMarray>& targetMDD, const r_Ref<r_GMarray>& replaceBlackMDD, const r_Ref<r_GMarray>& backgroundMDD)
{
    int retval = 0;

    const r_Minterval& backgroundDomain = backgroundMDD->spatial_domain();
    const r_Minterval& replaceBlackDomain = replaceBlackMDD->spatial_domain();
    const r_Minterval& targetDomain = targetMDD->spatial_domain();

    const char* replaceBlackMDDCells = replaceBlackMDD->get_array();
    const char* backgroundMDDCells = backgroundMDD->get_array();
    char* targetMDDCells = targetMDD->get_array();

    RMInit::logOut << "overlayGMarrays: " << std::endl;
    RMInit::logOut << "background MDD domain   : " << backgroundMDD->spatial_domain() << " type length: " << backgroundMDD->get_type_length() << std::endl;
    RMInit::logOut << "replace black MDD domain: " << replaceBlackMDD->spatial_domain() << " type length: " << replaceBlackMDD->get_type_length() << std::endl;
    RMInit::logOut << "target MDD domain       : " << targetMDD->spatial_domain() << " type length: " << replaceBlackMDD->get_type_length() << std::endl;

    r_Bytes typeLen = backgroundMDD->get_type_length();
    if ((typeLen == replaceBlackMDD->get_type_length()) && (typeLen == targetMDD->get_type_length()))
    {
        char* reference = new char[typeLen];
        memset(reference, 0, typeLen);
        if (backgroundDomain.get_extent() == targetDomain.get_extent())
            //if ((backgroundDomain.get_extent() == replaceBlackDomain.get_extent()) && ((backgroundDomain.get_extent() == targetDomain.get_extent())))
        {
            if (backgroundDomain.get_extent() == replaceBlackDomain.get_extent())
            {
                //background and target and transparent are equal
                RMInit::logOut << "Overlay using fast method" << endl;
                r_Area numberCells = backgroundDomain.cell_count();
                for (size_t elemNum = 0; elemNum < numberCells; elemNum++)
                {
                    if (memcmp(reference, &(replaceBlackMDDCells[elemNum * typeLen]), typeLen) == 0)
                    {
                        memcpy(&(targetMDDCells[elemNum * typeLen]), &(backgroundMDDCells[elemNum * typeLen]), typeLen);
                    }
                    else
                    {
                        memcpy(&(targetMDDCells[elemNum * typeLen]), &(replaceBlackMDDCells[elemNum * typeLen]), typeLen);
                    }
                }
            }
            else     //background and target are equal in size, but not equal to transparent
            {
                if (backgroundMDDCells == targetMDDCells)
                {
                    RMInit::logOut << "Overlay background and target not neccessary" << endl;
                }
                else
                {
                    RMInit::logOut << "Overlay background and target using very fast method" << endl;
                    memcpy(targetMDDCells, backgroundMDDCells, backgroundDomain.cell_count() * typeLen);
                }
                //overlay target and transparent
                r_Minterval overlayOn = replaceBlackDomain.create_intersection(backgroundDomain);
                RMInit::logOut << "Overlay using slow method on " << overlayOn << endl;
                r_Dimension dim = targetDomain.dimension();
                r_Range width = overlayOn[dim - 1].get_extent();
                r_MiterDirect replaceBlackIter((char*)replaceBlackMDDCells, replaceBlackDomain, overlayOn, typeLen);
                r_MiterDirect targetIter((char*)targetMDDCells, targetDomain, overlayOn, typeLen);
                while (!replaceBlackIter.isDone())
                {
                    replaceBlackMDDCells = (const char*)replaceBlackIter.getData();
                    targetMDDCells = (char*)targetIter.getData();
                    for (size_t elemNum = 0; elemNum < width; elemNum++)
                    {
                        if (memcmp(reference, &(replaceBlackMDDCells[elemNum * typeLen]), typeLen) != 0)
                        {
                            memcpy(&(targetMDDCells[elemNum * typeLen]), &(replaceBlackMDDCells[elemNum * typeLen]), typeLen);
                        }
                    }
                    replaceBlackIter.id[dim-1].pos += width;
                    targetIter.id[dim-1].pos += width;
                    ++replaceBlackIter;
                    ++targetIter;
                }
            }
        }
        else     //background and target are not the same size.  it does not matter if transparent is same as background
        {
            //need to use a miter, done as in Tile::copyTile, res is replaceBlack, op is background
            r_Minterval overlayOn = replaceBlackDomain.create_intersection(backgroundDomain);
            if (targetDomain.covers(overlayOn))
            {
                //first copy background on top of target
                RMInit::logOut << "Overlay using slow method on " << overlayOn << endl;
                r_Minterval copyBackgroundDomain = targetDomain.create_intersection(backgroundDomain);
                r_Dimension dim = copyBackgroundDomain.dimension();
                r_Range backgroundWidth = copyBackgroundDomain[dim - 1].get_extent();
                r_MiterDirect backgroundIter((char*)backgroundMDDCells, backgroundDomain, overlayOn, typeLen);
                r_MiterDirect targetIter((char*)targetMDDCells, targetDomain, overlayOn, typeLen);
                while (!targetIter.isDone())
                {
                    backgroundMDDCells = (const char*)backgroundIter.getData();
                    targetMDDCells = (char*)targetIter.getData();

                    memcpy(targetMDDCells, backgroundMDDCells, typeLen * backgroundWidth);

                    backgroundIter.id[dim-1].pos += backgroundWidth;
                    targetIter.id[dim-1].pos += backgroundWidth;
                    ++backgroundIter;
                    ++targetIter;
                }
                targetMDDCells = targetMDD->get_array();

                //overlay transparent over target
                r_Range width = overlayOn[dim - 1].get_extent();
                r_MiterDirect replaceBlackIter((char*)replaceBlackMDDCells, replaceBlackDomain, overlayOn, typeLen);
                r_MiterDirect targetIter2((char*)targetMDDCells, targetDomain, overlayOn, typeLen);
                while (!replaceBlackIter.isDone())
                {
                    replaceBlackMDDCells = (const char*)replaceBlackIter.getData();
                    targetMDDCells = (char*)targetIter2.getData();
                    for (size_t elemNum = 0; elemNum < width; elemNum++)
                    {
                        if (memcmp(reference, &(replaceBlackMDDCells[elemNum * typeLen]), typeLen) != 0)
                        {
                            memcpy(&(targetMDDCells[elemNum * typeLen]), &(replaceBlackMDDCells[elemNum * typeLen]), typeLen);
                        }
                    }
                    replaceBlackIter.id[dim-1].pos += width;
                    targetIter2.id[dim-1].pos += width;
                    ++replaceBlackIter;
                    ++targetIter2;
                }
            }
            else
            {
                RMInit::logOut << "Target domain does not cover the overlay domain" << endl;
                retval = OVERLAYDOMAINDOESNOTMATCH;
            }
        }
        delete [] reference;
        reference = NULL;
    }
    else
    {
        retval = OVERLAYTYPESIZESDONOTMATCH;
    }
    return retval;
}

int
SystemBasic::scaleDomain(const r_Minterval& baseDomain, const r_Point& origin, double factor, r_Minterval& scaledDomain, r_Minterval& clipDomain, unsigned int& length)
{
    int retval = 0;
    r_Dimension dim = origin.dimension();
    r_Dimension i = 0;
    scaledDomain = r_Minterval(dim);
    clipDomain = r_Minterval(dim);
    r_Range low = 0;
    r_Range high = 0;
    r_Range baseSteps = 0;
    r_Range scaleSteps = 0;
    r_Range originVal = 0;
    length = floor(1.0 / factor + 0.5);
    if (baseDomain.dimension() == dim)
    {
        try
        {
            for (; i < dim; i++)
            {
                const r_Sinterval& baseSinterval = baseDomain[i];
                originVal = origin[i];
                // simple trafo of low coordinate
                low = (r_Range)(originVal + floor((baseSinterval.low() - originVal) * factor));
                // for the high coordinate use the low coordinate of the _next_ tile
                // ( = baseDomain[i].high() + 1 ) and subtract 1 ==> seamless tilingfactor
                high = (r_Range)(originVal + floor((baseSinterval.high() + 1 - originVal) * factor) - 1);
                // number of steps in base interval
                baseSteps = floor((baseSinterval.high() - baseSinterval.low() + length) / length);
                // number of steps in scaled interval
                scaleSteps = high - low + 1;
                if (baseSteps == scaleSteps)
                {
                    // everything is ok
                    scaledDomain << r_Sinterval(low, high);
                    clipDomain << baseSinterval;
                }
                else
                {
                    if (baseSteps < scaleSteps)
                    {
                        // the base is too small -> shrink the scale interval
                        RMInit::logOut << "WARNING: " << baseDomain << " * " << factor << " : clipping the scale interval" << endl;
                        scaledDomain << r_Sinterval(low, low + baseSteps - 1);
                        clipDomain << baseSinterval;
                    }
                    else     // the scale is too small -> shrink the clip interval
                    {
                        RMInit::logOut << "WARNING: " << baseDomain << " * " << factor << " : clipping the base interval" << endl;
                        scaledDomain << r_Sinterval(low, high);
                        clipDomain << r_Sinterval(baseSinterval.low(), (r_Range)(baseSinterval.low() + scaleSteps * length - 1));
                    }
                }
            }
        }
        catch (r_Error& err)
        {
            RMInit::logOut << "SystemBasic::scaleDomain(" << baseDomain << ", " << scaleDomain << ", " << factor << ", " << origin << ") caught error : " << err.get_errorno() << " " << err.what() << endl;
            retval = SCALEDOMAINISNOTCORRECT;
        }
    }
    else
    {
        retval = SCALEDOMAINISNOTCORRECT;
    }
    return retval;
}

int
SystemBasic::updateScaledMDD(const r_Ref<r_GMarray>& baseMDD, const r_Minterval& clipDomain, const r_Minterval& downScaleDomain, unsigned int length, const char* collectionName) throw (r_Error)
{
    RMInit::logOut << "SystemBasic::updateScaledMDD(MDD, " << clipDomain << ", " << downScaleDomain << ", " << length << ", " << collectionName << ")" << endl;
    int retval = 0;
    const r_Minterval& baseDomain = baseMDD->spatial_domain();
    if (retval == 0)
    {
        size_t tlen = baseMDD->get_type_length();
        r_Ref<r_GMarray> scaledMDD = new (baseMDD->get_type_name())r_GMarray(downScaleDomain, tlen);
        const r_Type* type = scaledMDD->get_base_type_schema();
        if (type != NULL)
        {
            if (type->isPrimitiveType())
            {
                fast_scale_process_primitive_type((const r_Primitive_Type*)type, scaledMDD->get_array(), baseMDD->get_array(), downScaleDomain, baseDomain, clipDomain, tlen, length, scaleFunction);
            }
            else
            {
                if (type->isStructType())
                {
                    fast_scale_process_structured_type((const r_Structure_Type*)type, scaledMDD->get_array(), baseMDD->get_array(), downScaleDomain, baseDomain, clipDomain, tlen, length, scaleFunction);
                }
                else
                {
                    retval = NOBASETYPE;
                }
            }
        }
        else
        {
            retval = NOBASETYPE;
        }
        if (retval == 0)
        {
            //update the collection
            std::ostringstream stream;
            stream << "UPDATE " << collectionName << " AS A SET A" << downScaleDomain << " ASSIGN $1";
            r_OQL_Query query(stream.str().c_str());
            query << *scaledMDD;
            RMDBGIF(20, RMDebug::module_tools, "WAITBEFOREQL", \
                    RMInit::dbgOut << "Waiting 10 sec before execute\n" << std::endl; \
                    sleep(10); \
                    RMInit::dbgOut << "Continue now\n" << std::endl; );
            r_oql_execute(query);
            RMDBGIF(20, RMDebug::module_tools, "WAITAFTERQL", \
                    RMInit::dbgOut << "Waiting 10 sec after execute\n" << std::endl; \
                    sleep(10); \
                    RMInit::dbgOut << "Continue now\n" << std::endl; );
        }
        scaledMDD.destroy();
    }
    return retval;
}

int
SystemBasic::compareScaledMDD(const r_Ref<r_GMarray>& baseMDD, const r_Minterval& clipDomain, const r_Minterval& downScaleDomain, unsigned int length, const char* collectionName) throw (r_Error)
{
    RMInit::logOut << "SystemBasic::compareScaledMDD(MDD, " << clipDomain << ", " << downScaleDomain << ", " << length << ", " << collectionName << ")" << endl;
    int retval = 0;
    const r_Minterval& baseDomain = baseMDD->spatial_domain();
    if (retval == 0)
    {
        size_t tlen = baseMDD->get_type_length();
        r_Ref<r_GMarray> scaledMDD = new (baseMDD->get_type_name())r_GMarray(downScaleDomain, tlen);
        const r_Type* type = ((r_GMarray*)&*baseMDD)->get_base_type_schema();
        if (type != NULL)
        {
            if (type->isPrimitiveType())
            {
                fast_scale_process_primitive_type((const r_Primitive_Type*)type, scaledMDD->get_array(), baseMDD->get_array(), downScaleDomain, baseDomain, clipDomain, tlen, length, scaleFunction);
            }
            else
            {
                if (type->isStructType())
                {
                    fast_scale_process_structured_type((const r_Structure_Type*)type, scaledMDD->get_array(), baseMDD->get_array(), downScaleDomain, baseDomain, clipDomain, tlen, length, scaleFunction);
                }
                else
                {
                    RMInit::logOut << "scaledMDD returned a type (" << type->name() << ") which is neither primitive nor structured" << endl;
                    retval = NOBASETYPE;
                }
            }
        }
        else
        {
            RMInit::logOut << "scaledMDD returned no base type schema" << endl;
            retval = NOBASETYPE;
        }
        if (retval == 0)
        {
            //update the collection
            std::ostringstream stream;
            stream << "SELECT A" << downScaleDomain << " FROM " << collectionName << " AS A";
            r_OQL_Query query(stream.str().c_str());
            r_Set< r_Ref_Any > result;
            r_oql_execute(query, result);
            r_Iterator< r_Ref_Any > iter = result.create_iterator();
            r_Ref<r_GMarray> selectedMDD = r_Ref<r_GMarray>(*iter);
            if (retval == 0)
            {
                retval = compareGMarrays(selectedMDD, scaledMDD);
                if ((outputFileName != NULL) && ((retval != 0) || (force)))
                {
                    char* tempName = new char[strlen(outputFileName) + strlen("image.png") + strlen(collectionName) + 1];
                    strcpy(tempName, outputFileName);
                    strcat(tempName, collectionName);
                    strcat(tempName, ".db.png");
                    saveData(tempName, selectedMDD->get_array(), selectedMDD->get_array_size(), selectedMDD->spatial_domain());
                    strcpy(tempName, outputFileName);
                    strcat(tempName, collectionName);
                    strcat(tempName, ".scaled.png");
                    saveData(tempName, scaledMDD->get_array(), scaledMDD->get_array_size(), scaledMDD->spatial_domain());
                    delete [] tempName;
                    tempName = NULL;
                }
            }
            selectedMDD.destroy();
        }
        scaledMDD.destroy();
    }
    return retval;
}

void
fast_scale_process_primitive_type(const r_Primitive_Type *primType, char *dest, const char *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &srcIter, unsigned int type_len, unsigned int length, r_Scale_Function func)
{
    if (func == r_BitAggregation)
    {
        switch (primType->type_id())
        {
        case r_Primitive_Type::BOOL:
        case r_Primitive_Type::CHAR:
        case r_Primitive_Type::OCTET:
            fast_scale_aggregate_array((char*)dest, (const char*)src, destIv, srcIv, srcIter, type_len, length);
            break;
        case r_Primitive_Type::SHORT:
        case r_Primitive_Type::USHORT:
            fast_scale_aggregate_array((short*)dest, (const short*)src, destIv, srcIv, srcIter, type_len, length);
            break;
        case r_Primitive_Type::LONG:
        case r_Primitive_Type::ULONG:
        case r_Primitive_Type::FLOAT:
            fast_scale_aggregate_array((int*)dest, (const int*)src, destIv, srcIv, srcIter, type_len, length);
            break;
        case r_Primitive_Type::DOUBLE:
            fast_scale_aggregate_array((long long*)dest, (const long long*)src, destIv, srcIv, srcIter, type_len, length);
            break;
        default:
            RMInit::logOut << "Resample Array: unknown primitive type " << primType->type_id() << endl;
            break;
        }
    }
    else
    {
        switch (primType->type_id())
        {
        case r_Primitive_Type::BOOL:
            fast_scale_resample_array((r_Boolean*)dest, (const r_Boolean*)src, destIv, srcIv, srcIter, type_len, length, true);
            break;
        case r_Primitive_Type::CHAR:
            fast_scale_resample_array((r_Char*)dest, (const r_Char*)src, destIv, srcIv, srcIter, type_len, length, true);
            break;
        case r_Primitive_Type::OCTET:
            fast_scale_resample_array((r_Octet*)dest, (const r_Octet*)src, destIv, srcIv, srcIter, type_len, length, true);
            break;
        case r_Primitive_Type::SHORT:
            fast_scale_resample_array((r_Short*)dest, (const r_Short*)src, destIv, srcIv, srcIter, type_len, length, true);
            break;
        case r_Primitive_Type::USHORT:
            fast_scale_resample_array((r_UShort*)dest, (const r_UShort*)src, destIv, srcIv, srcIter, type_len, length, true);
            break;
        case r_Primitive_Type::LONG:
            fast_scale_resample_array((r_Long*)dest, (const r_Long*)src, destIv, srcIv, srcIter, type_len, length, true);
            break;
        case r_Primitive_Type::ULONG:
            fast_scale_resample_array((r_ULong*)dest, (const r_ULong*)src, destIv, srcIv, srcIter, type_len, length, true);
            break;
        case r_Primitive_Type::FLOAT:
            fast_scale_resample_array((r_Float*)dest, (const r_Float*)src, destIv, srcIv, srcIter, type_len, length, false);
            break;
        case r_Primitive_Type::DOUBLE:
            fast_scale_resample_array((r_Double*)dest, (const r_Double*)src, destIv, srcIv, srcIter, type_len, length, false);
            break;
        default:
            break;
        }
    }
}

void
fast_scale_process_structured_type(const r_Structure_Type *structType, char *dest, const char *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &srcIter, unsigned int type_len, unsigned int length, r_Scale_Function func)
{
    r_Structure_Type::attribute_iterator iter(structType->defines_attribute_begin());
    while (iter != structType->defines_attribute_end())
    {
        r_Type *newType;
        unsigned long offset;

        newType = (*iter).type_of().clone();
        offset = (*iter).offset();

        if (newType->isStructType())
        {
            r_Structure_Type *newStructType = (r_Structure_Type*)newType;
            fast_scale_process_structured_type(newStructType, dest + offset, src + offset, destIv, srcIv, srcIter, type_len, length, func);
        }
        else
        {
            r_Primitive_Type *newPrimType = (r_Primitive_Type*)newType;
            fast_scale_process_primitive_type(newPrimType, dest + offset, src + offset, destIv, srcIv, srcIter, type_len, length, func);
        }
        delete newType;
        iter++;
    }
}


// always iterate over the full dest domain, but not over the full src domain.
template<class T>
void fast_scale_resample_array(T *dest, const T *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &iterDom, unsigned int type_len, unsigned int length, bool round)
{
    //cout << "here " << destIv << srcIv << iterDom << ", " << type_len << " " << length << endl;
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
        while (subIter.done == 0)
        {
            sum += *((const T*)(subIter.getData()));
            ++subIter;
        }
        // use round to nearest
        if (round)
            *((T*)(destIter.getData())) = (T)(sum / count + 0.5);
        else
            *((T*)(destIter.getData())) = (T)(sum / count);
        //cout << (long)(((const T*)(srcIter.getData())) - src) << " , " << (long)(((T*)(destIter.getData())) - dest) << endl;
        ++srcIter;
        ++destIter;
    }
}

// always iterate over the full dest domain, but not over the full src domain.
template<class T>
void fast_scale_aggregate_array(T *dest, const T *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &iterDom, unsigned int type_len, unsigned int length)
{
    //cout << "here " << destIv << srcIv << iterDom << ", " << type_len << " " << length << endl;
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
}

#ifdef TESTBASIC
int
main(int argc, const char** argv)
{
    r_Minterval baseDomain("[0:1024]");
    r_Point origin("[0]");
    r_Point translation("[256]");
    double factor = 0.5;
    r_Minterval scaledDomain;
    r_Minterval clipDomain;
    unsigned int length = 0;
    for (int o = 0; o < 1024; o++)
    {
        for (int i = 0; i < 1024; i++)
        {
            for (int a = 0; a < 8; a++)
            {
                SystemBasic::scaleDomain(baseDomain, origin, factor, scaledDomain, clipDomain, length);
                if (clipDomain != baseDomain)
                {
                    cout << "ERROR clip != base (" << factor << "): " << clipDomain << " " << baseDomain << endl;
                }
                factor = factor /2;
            }
            factor = 0.5;
            baseDomain.translate(translation);
        }
        baseDomain = r_Minterval("[256:511]");
        baseDomain[0].set_high(256 * (o + 2) - 1);
    }
}
#endif
