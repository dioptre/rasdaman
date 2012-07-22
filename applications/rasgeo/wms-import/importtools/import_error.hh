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

#ifndef _IMPORT_ERROR_HH_
#define _IMPORT_ERROR_HH_

#ifdef __VISUALC__
#pragma warning( disable : 4290 )
#endif

//@ManMemo: Module: {\bf raslib}

/*@Doc:

 This class ...
*/


/// valid error codes:
#define ALLDONE                         -1
#define OK                              0
#define CREATEANDNOCREATE               1
#define NOVALIDDOMAIN                   2
#define FILEINACCESSIBLE                3
#define FILENAMETOOLONG                 4
#define NOCREATECREATECOLL              5
#define NOCOLLTYPE                      6
#define COLLOIDANDCOLLNAME              7
#define CREATECOLLWITHOID               8
#define CREATEMDDWITHOID                9
#define INVALIDTILESIZE                 10
#define DOMAINDATAMISMATCH              11
#define MDDDOMAINNOTSPECIFIED           12
#define FILENAMENOTSPECIFIED            13
#define NOCOLLNAMENOCOLLOID             14
#define MDDTYPEINVALID                  15
#define NOBASETYPE                      16
#define EXCEPTIONCREATECOLL             17
// removed, too general: #define EXCEPTIONADMIN                  18
#define COLLECTIONINACCESSIBLE          19
#define MDDINACCESSIBLE                 20
#define OIDINVALID                      21
#define MDDOIDANDCOLL                   22
#define ARGUMENTSMISSING                23
#define ARGUMENTSINCOMPLETE             24
#define EXCEPTIONEXECUTEQUERY           25
#define NOQUERY                         26
#define CONVERSIONNOTSUPPORTED          27
#define CONVERSIONEXCEPTION             28
#define UNKNOWNTILINGTYPE               29
#define TILINGPARAMETERSMISSING         30
#define TILINGPATAMETERSINCORRECT       31
#define CONVERSIONTYPENOTABASETYPE      32
#define CONVERSIONRETURNEDWRONGTYPE     33
#define POLYGONCREATIONFAILED           34
#define OVERLAYDOMAINSDONOTMATCH        35
#define OVERLAYTYPESIZESDONOTMATCH      36
#define POLYGONDOMAINTOOLARGE           37
#define NOCOLLNAMEDEFINED               38
#define NOMDDTYPEDEFINED                39
#define MDDTYPEOFGMARRAYNOTINITIALISED  40
#define SCALEDOMAINISNOTCORRECT         41
#define SCALELEVELSINCORRECT            42
#define GMARRAYSARENOTEQUAL             43
#define UNKNOWNSCALEFUNCTION            44
#define UNABLETOCLAIMRESOURCEFORFILE    45
#define ERRORPARSINGCOMMANDLINE         46
#define TILINGTYPENOTALLOWED            47
#define LAYERSNOTCORRECT                48
#define TOOMANYLAYERS                   49
#define OVERLAYDOMAINDOESNOTMATCH       50
#define TILEDUPDATEANDFILE              51
#define NOCOMPAREDESTINATION            52
#define CONVERSIONTYPEMISSING           53
#define MDDOIDMISSING                   54
#define INVALIDALIGN                    55
#define SIGNALCAUGHT                    56
#define DOMAINNOTTWODIMENSIONAL         57
#define INVALIDSAMPLINGMETHOD           58
#define UNKNOWNBASETYPE                 59
#define SCALEDOMAINPOSSIBLEUNDERFLOW    60
#define INVALIDSCALENAME                61
#define INVALIDSCALEFACTOR              62
#define INVALIDCOLLNAMECHAR             63
#define INVALIDFLOAT                    64
#define INVALIDRESOLUTION               65
#define INVALIDDATAFORMAT               66
#define INVALIDSKIP                     67
#define INVALIDHEX                      68
#define INVALIDBUFSIZ                   69
#define LAYERLISTNOTDEFINED         70
#define TOOMANYWILDCARDS                71
#define NOWILDCARD                      72
#define TKLAYERMISMATCH                 73
#define UNSUPPORTEDTYPESIZE             74
#define ILLEGALIMPORTTYPE               75
#define CANNOTACCESSCOLL                76
#define CANNOTUPDATECOLL                77
#define BLOCKEXTENTOFLO                 78
#define DOMAINRETRIEVAL                 79
#define USERNOTSPECIFIED                80
#define PASSWDNOTSPECIFIED              81
#define COORDMISSING                    82
#define RESMISSING                      83
#define TIFF_FILENAME                   84
#define TIFF_TAGS                       85
#define TIFF_BITMAPSONLY                86
#define TIFF_OUTPUTBYTES                87
#define BASETYPEMISMATCH                88
#define DOMAINEXTENTMISMATCH            89

class ImportError // : public std::exception
{
public:

    /// constructor receiving an error number
    ImportError( unsigned int e );

    /// destructor
    virtual ~ImportError();

    /// get an error description
    virtual const char * what();

private:
    /// error information
    unsigned int importErrno;
};

#endif // _IMPORT_ERROR_HH_
