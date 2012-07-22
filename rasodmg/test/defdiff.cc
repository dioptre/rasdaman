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
/*
 *  defdiff.cc
 *
 *  Compare to DEF files and returns 0 for succes, otherwise 1
 *
 *  Parameters:
 *
 *  --inputfilename <src-file>
 *     the filename of the src file, mandatory
 *  --outputfilename <dest-file>
 *     the filename of the dest file, mandatory
 *
 *  --inputformat name
 *     name of DEF fromat(see data_foramat from mddtypes.hh)
 *  --inputformatparams params
 *     params used by convertFrom methods(see the heder file for you DEF from conversion)
 *  --outputformat name
 *     name of DEF fromat(see data_foramat from mddtypes.hh)
 *  --outputformatparams params
 *     params used by convertFrom methods(see the heder file for you DEF from conversion)
 *
 *  --help
 *     print usage and exit
 */

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif

#include "defutil.hh"

const char* diffFileName = "defdiff.tif";

const char* paramPrgHelp="--help";
bool defPrgHelp=false;
char* prgHelp=NULL;

const char* paramSrcFileName="--srcfilename";
bool defSrcFileName=false;
char* srcFileName=NULL;

const char* paramSrcFormat="--srcformat";
bool defSrcFormat=false;
char* srcFormatc=NULL;
r_Data_Format srcFormat=r_Array;

const char* paramSrcFormatParams="--srcformatparams";
bool defSrcFormatParams=false;
char *srcFormatParams=NULL;

const char* paramDestFileName="--destfilename";
bool defDestFileName=false;
char *destFileName=NULL;

const char* paramDestFormat="--destformat";
bool defDestFormat=false;
char *destFormatc=NULL;
r_Data_Format destFormat=r_Array;

const char* paramDestFormatParams="--destformatparams";
bool defDestFormatParams=false;
char *destFormatParams=NULL;

//structures used for parseParam
const int paramsNo=7;

const char *paramsName[paramsNo]= { paramPrgHelp, paramSrcFileName, paramSrcFormat, paramSrcFormatParams,
                                    paramDestFileName, paramDestFormat, paramDestFormatParams
                                  };

bool *paramsPresent[paramsNo]= { &defPrgHelp, &defSrcFileName, &defSrcFormat, &defSrcFormatParams,
                                 &defDestFileName, &defDestFormat, &defDestFormatParams
                               };

char **paramsValue[paramsNo]= { &prgHelp, &srcFileName, &srcFormatc, &srcFormatParams,
                                &destFileName, &destFormatc, &destFormatParams
                              };

void printStatus()
{
    cout << "defdiff parameters list:" << endl;
    for (int i=0; i<paramsNo; i++)
        cout << "--Name='" << paramsName[i]
             << "' Present=" << ((*paramsPresent[i])? "true": "false")
             << " Value='" << (*paramsValue[i]?*paramsValue[i]: "null")  << "'" <<  endl;
}

void printUsage()
{
    char* fileNameSrc="test.bmp";
    char* fileNameDest="test.tiff";

    cout << "defdiff v 0.1 - RasDaMan Data Exchange Format Difference Utility" << endl;
    cout << "Description: Returns " << EXIT_SUCCESS << " for succes, otherwise " << EXIT_FAILURE << endl;
    cout << "If it is possible it will create a tiff file with the difference, called " << diffFileName << endl;
    cout << "Usage options:" << endl;
    cout << "        " << paramSrcFileName << " <src-file>        ... the filename used as src mandatory" << endl;
    cout << "        " << paramSrcFormat << " name                ... name of DEF fromat(see data_foramat from mddtypes.hh)," << endl;
    cout << "                                              default Array" << endl;
    cout << "        " << paramSrcFormatParams << " params        ... params used by convertFrom methods(see the header file" << endl;
    cout << "                                              for you DEF from conversion), default NULL" << endl;
    cout << "        " << paramDestFileName << " <dest-file>      ... the filename used as dest mandatory" << endl;
    cout << "        " << paramDestFormat << " name               ... name of DEF fromat(see data_foramat from mddtypes.hh)," << endl;
    cout << "                                              default Array" << endl;
    cout << "        " << paramDestFormatParams << " params        ... params used by convertFrom methods(see the header file" << endl;
    cout << "                                              for you DEF from conversion), default NULL" << endl;
    cout << "        " << paramPrgHelp << "             ... this help" << endl;
    cout << "For example:" << endl;
    cout << "defdiff " << paramSrcFileName << " " << fileNameSrc << " " << paramSrcFormat << " " << format_name_bmp <<" "
         << paramDestFileName << " " << fileNameDest  << " " << paramDestFormat <<" " << format_name_tiff << endl;
    cout << "Report bugs to liviu.coman@active­knowledge.com" << endl;

}

int checkParam(int& paramIndex, char** param)
{
    int i=0;
    while(i<paramsNo)
    {
        if(!strcmp(param[paramIndex], paramsName[i]))
            break;
        i++;
    }

    if(i==paramsNo)
    {
        cout << "checkParam(...) error processing parameters: parameter "
             << param[paramIndex] << " is unknown!" << endl;
        return EXIT_FAILURE;
    }
    if(*paramsPresent[i])
    {
        cout << "checkParam(...) error processing parameters: parameter "
             << param[paramIndex] << " is present more than once!" << endl;
        return EXIT_FAILURE;
    }

    if(paramsName[i]!=paramPrgHelp)
    {
        *paramsPresent[i]=true;
        paramIndex++;
        *paramsValue[i]=param[paramIndex];
    }
    else
        *paramsPresent[i]=true;

    return EXIT_SUCCESS;
}

int parseParams(int argc, char** argv)
{
    int argIndex=0;
    if (argc==1)
    {
        cout << "parseParams(...) parameters " << paramSrcFileName << " <src-file> and "
             << paramDestFileName << " <dest-file> are mandatatory!" << endl;
        return EXIT_FAILURE;
    }
    else
    {
        argIndex=1;
        while(argIndex<argc)
        {
            if(checkParam(argIndex, argv)!=EXIT_SUCCESS)
                break;
            argIndex++;
        }
        if(argIndex<argc)
            return EXIT_FAILURE;

        //check rule
        if((defSrcFileName && defDestFileName) || (defPrgHelp))
            return EXIT_SUCCESS;
        else
        {
            cout << "parseParams(...) parameters " << paramSrcFileName << " <src-file> and "
                 << paramDestFileName << " <dest-file> are mandatatory!" << endl;
            return EXIT_FAILURE;
        }
    }
}

int validateParams()
{
    if(defSrcFormat)
    {
        srcFormat = get_data_format_from_name(srcFormatc);
        if((srcFormat!=r_Array) &&
                (!r_Convertor_Factory::is_supported(srcFormat)))
        {
            cout << "validateParams() conversion of " << srcFormatc << " not supported" << endl;
            return EXIT_FAILURE;
        }
    }
    if(defDestFormat)
    {
        destFormat = get_data_format_from_name(destFormatc);
        if((destFormat!=r_Array) &&
                (!r_Convertor_Factory::is_supported(destFormat)))
        {
            cout << "validateParams() conversion " << destFormatc << " not supported" << endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int compareDEFs()
{
    char *dataSrc=NULL, *dataDest=NULL;
    r_ULong dataSrcSize=0, dataDestSize=0;
    r_ULong arraySrcSize=0, arrayDestSize=0;
    r_Bytes basetypeSrcSize=0, basetypeDestSize=0;
    r_convDesc descSrc, descDest;
    r_Type::r_Type_Id basetypeSrcId=r_Type::UNKNOWNTYPE, basetypeDestId=r_Type::UNKNOWNTYPE;

//init conversion result data
    initConvDesc(descSrc);
    initConvDesc(descDest);

//read files
    if(readFile(srcFileName, &dataSrc, dataSrcSize)!=EXIT_SUCCESS)
        return EXIT_FAILURE;

    if(readFile(destFileName, &dataDest, dataDestSize)!=EXIT_SUCCESS)
    {
        delete [] dataSrc;
        dataSrc=NULL;
        return EXIT_FAILURE;
    }

//convert if necessary
    if(srcFormat !=r_Array)
    {
        r_Minterval srcIv(1);
        r_Type* srcType=NULL;

        srcIv << r_Sinterval((r_Range)0, (r_Range)(dataSrcSize -1));
        srcType=r_Convertor::get_external_type(r_Convertor::ctype_char);

        if(convertFrom(srcFormat, srcFormatParams, dataSrc,
                       srcIv, srcType, descSrc) != EXIT_SUCCESS)
        {
            delete srcType;
            srcType=NULL;
            delete [] dataSrc;
            dataSrc=NULL;
            delete [] dataDest;
            dataDest=NULL;
            return EXIT_FAILURE;
        }
        else
        {
            //we don't need this anymore
            delete [] dataSrc;
            dataSrc=NULL;
            delete srcType;
            srcType=NULL;
        }
    }

    if(destFormat !=r_Array)
    {
        r_Minterval destIv(1);
        r_Type* destType=NULL;

        destIv << r_Sinterval((r_Range)0, (r_Range)(dataDestSize -1));
        destType=r_Convertor::get_external_type(r_Convertor::ctype_char);

        if(convertFrom(destFormat, destFormatParams, dataDest,
                       destIv, destType, descDest) != EXIT_SUCCESS)
        {
            delete destType;
            destType=NULL;
            delete [] dataDest;
            dataDest=NULL;
            if(srcFormat==r_Array)
            {
                delete [] dataSrc;
                dataSrc=NULL;
            }
            else
            {
                dataSrc=NULL;
                cleanConvDesc(descSrc);
            }

            return EXIT_FAILURE;
        }
        else
        {
            //we don't need this anymore
            delete [] dataDest;
            dataDest=NULL;
            delete destType;
            destType=NULL;
        }
    }

//check domain
    if((srcFormat!=r_Array) &&
            (destFormat!=r_Array) &&
            (descSrc.destInterv != descDest.destInterv))
    {
        cout << "compareDEFs() source domain " << descSrc.destInterv << " != destination domain " << descDest.destInterv << endl;
        cleanConvDesc(descSrc);
        cleanConvDesc(descDest);
        return EXIT_FAILURE;
    }

//check if celltype is a basetype
    if((srcFormat!=r_Array) &&
            (!descSrc.destType->isBaseType()))
    {
        cout << "compareDEFs() source type " << basetypeSrcId << " is not a base type" << endl;
        if(destFormat==r_Array)
        {
            delete [] dataDest;
            dataDest=NULL;
        }
        else
            cleanConvDesc(descDest);
        cleanConvDesc(descSrc);
        return EXIT_FAILURE;
    }

    if((destFormat!=r_Array) &&
            (!descDest.destType->isBaseType()))
    {
        cout << "compareDEFs() destination type " << basetypeDestId << " is not a base type" << endl;
        if(srcFormat==r_Array)
        {
            delete [] dataSrc;
            dataSrc=NULL;
        }
        else
            cleanConvDesc(descSrc);
        cleanConvDesc(descDest);
        return EXIT_FAILURE;
    }

//stoare info about types
    if(srcFormat!=r_Array)
    {
        basetypeSrcId=descSrc.destType->type_id();
        basetypeSrcSize=((r_Base_Type*)descSrc.destType)->size();
    }
    if(destFormat!=r_Array)
    {
        basetypeDestId=descDest.destType->type_id();
        basetypeDestSize=((r_Base_Type*)descDest.destType)->size();
    }

    if((srcFormat!=r_Array) &&
            (destFormat!=r_Array))
    {
        //FIXME basetypeSrcId!=basetypeDestId to powerfull
        if(basetypeSrcId!=basetypeDestId)
            cout << "compareDEFs() warning: source type " << basetypeSrcId
                 << " != destination type " << basetypeDestId << endl;

        if(basetypeSrcSize!=basetypeDestSize)
        {
            cout << "comparetDEFs() source type " << descSrc.destType->name() << " != destination type " << descDest.destType->name() << endl;
            cout << "source type size" << basetypeSrcSize << " != destination type size" << basetypeDestSize << endl;
            cleanConvDesc(descSrc);
            cleanConvDesc(descDest);
            return EXIT_FAILURE;
        }

        if((basetypeSrcId == r_Type::STRUCTURETYPE) &&
                (basetypeDestId == r_Type::STRUCTURETYPE) &&
                (compareStructure((r_Structure_Type*)descSrc.destType, (r_Structure_Type*)descDest.destType) !=EXIT_SUCCESS))
        {
            cout << "compareDEFs() source structure type != destination structure type" << endl;
            cout << "Source type is: " << (*(r_Structure_Type*)(descSrc.destType)) << endl;
            cout << "Destination type is:" << (*(r_Structure_Type*)(descSrc.destType)) << endl;
            cleanConvDesc(descSrc);
            cleanConvDesc(descDest);
            return EXIT_FAILURE;
        }
    }

    if(srcFormat!=r_Array)
    {
        arraySrcSize=descSrc.destInterv.cell_count() * basetypeSrcSize;
        dataSrc=descSrc.dest;
    }
    else
        arraySrcSize=dataSrcSize;

    if(destFormat!=r_Array)
    {
        arrayDestSize=descDest.destInterv.cell_count() * basetypeDestSize;
        dataDest=descDest.dest;
    }
    else
        arrayDestSize=dataDestSize;

    if(arraySrcSize!=arrayDestSize)
    {
        cout << "compareDEFs() source data size " << arraySrcSize << " != " << " destination data size " << arrayDestSize << endl;
        delete [] dataSrc;
        dataSrc=NULL;
        delete [] dataDest;
        dataDest=NULL;
        cleanConvDesc(descSrc);
        cleanConvDesc(descDest);
        return EXIT_FAILURE;
    }

    if(memcmp(dataSrc, dataDest, arraySrcSize))
    {
        cout << "compareDEFs() source and destination data are different" << endl;
        r_convDesc descDiff;
        r_ULong diffSize=0;
        initConvDesc(descDiff);
        //do diff
        for(int i=0; i< arraySrcSize; i++)
            dataDest[i]-=dataSrc[i];
        if(convertTo(r_TIFF, NULL, dataDest, descDest.destInterv,
                     descDest.destType, descDiff) != EXIT_SUCCESS)
        {
            cout << "compareDEFs() error building the difference tiff file" << endl;;
        }
        else
        {
            diffSize=descDiff.destInterv.cell_count() * ((r_Base_Type*)descDiff.destType)->size();
            if(writeFile(diffFileName, (const char**)&descDiff.dest, diffSize)!=EXIT_SUCCESS)
                cout << "compareDEFs() error building the difference tiff file" << endl;;
            cleanConvDesc(descDiff);
        }

        //clean up
        if(srcFormat==r_Array)
        {
            delete [] dataSrc;
            dataSrc=NULL;
        }
        else
        {
            dataSrc=NULL;
            cleanConvDesc(descSrc);
        }
        if(destFormat==r_Array)
        {
            delete [] dataDest;
            dataDest=NULL;
        }
        else
        {
            dataDest=NULL;
            cleanConvDesc(descDest);
        }
        return EXIT_FAILURE;
    }

    cout << "compareDEFs() source and destination data are identic" << endl;

//clean up
    if(srcFormat==r_Array)
    {
        delete [] dataSrc;
        dataSrc=NULL;
    }
    else
    {
        dataSrc=NULL;
        cleanConvDesc(descSrc);
    }
    if(destFormat==r_Array)
    {
        delete [] dataDest;
        dataDest=NULL;
    }
    else
    {
        dataDest=NULL;
        cleanConvDesc(descDest);
    }

    return EXIT_SUCCESS;
}

int processRequest()
{
    if(validateParams() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    return compareDEFs();
}

int main(int argc, char **argv)
{
    if(parseParams(argc, argv)!=EXIT_SUCCESS)
    {
        printUsage();
        return EXIT_FAILURE;
    }
    else
    {
        if(!defPrgHelp)
        {
#ifdef RMANDEBUG
            printStatus();
#endif
            return processRequest();
        }
        else
        {
            printUsage();
            return EXIT_SUCCESS;
        }
    }
}
