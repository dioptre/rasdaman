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
 *  defutil.hh
 *
 *  COMMENTS
 *  Common functions used by defxxx programs
*/


#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;

#include "raslib/mddtypes.hh"
#include "raslib/structuretype.hh"
#include "raslib/odmgtypes.hh"
#include "raslib/minterval.hh"
#include "raslib/parseparams.hh"
#include "conversion/convertor.hh"
#include "conversion/convfactory.hh"

//--I/O file
int readFile(const char* fileName, char **data, r_ULong& dataSize)
{
    FILE *pFile=NULL;

    if(fileName==NULL)
    {
        cout << "readFile(...) fileName is null" << endl;
        return EXIT_FAILURE;
    }

    if(*data!=NULL)
    {
        cout << "readFile(...) data is not null" << endl;
        return EXIT_FAILURE;
    }

    pFile=fopen(fileName, "rb");
    if(pFile==NULL)
    {
        cout << "readFile(...) unable to open file " << fileName << endl;
        return EXIT_FAILURE;
    }

    fseek(pFile, 0, SEEK_END);
    dataSize=ftell(pFile);

    *data=new char[dataSize];
    if(*data==NULL)
    {
        cout << "readFile(...) unable to claim memory for file " << fileName << endl;
        fclose(pFile);
        return EXIT_FAILURE;
    }

    fseek(pFile, 0, SEEK_SET);
    fread(*data, 1, dataSize, pFile);

    fclose(pFile);
    return EXIT_SUCCESS;
}

int writeFile(const char* fileName, const char **data, r_ULong& dataSize)
{
    FILE *pFile=NULL;

    if(fileName==NULL)
    {
        cout << "writeFile(...) fileName is null" << endl;
        return EXIT_FAILURE;
    }

    if(*data==NULL)
    {
        cout << "writeFile(...) data is null" << endl;
        return EXIT_FAILURE;
    }

    if(!dataSize)
    {
        cout << "writeFile(...) dataSize is zero" << endl;
        return EXIT_FAILURE;
    }

    pFile=fopen(fileName, "wb");
    if(pFile==NULL)
    {
        cout << "writeFile(...) unable to open file" << fileName << endl;
        return EXIT_FAILURE;
    }

    fwrite(*data, 1, dataSize, pFile);

    fclose(pFile);
    pFile=NULL;

    return EXIT_SUCCESS;
}

//ConvertFrom/To r_Array data
int convertFrom(r_Data_Format fmt, const char* fmtParams,
                const char* data, const r_Minterval& dataDom,
                const r_Type* dataType, r_convDesc& desc)
{
    r_Storage_Man_CPP mySM;
    r_Convertor *conv=NULL;

    if(data==NULL)
    {
        cout << "convertFrom(...) data is null" << endl;
        return EXIT_FAILURE;
    }

    if(dataType==NULL)
    {
        cout << "convertFrom(...) dataType is null" << endl;
        return EXIT_FAILURE;
    }

    try
    {
        conv=r_Convertor_Factory::create(fmt, data, dataDom, dataType);
        conv->set_storage_handler(mySM);
    }
    catch(r_Error& err)
    {
        cout << "convertFrom(...) request for convertor " << fmt << " failed" << endl
             << "Error " << err.get_errorno() << " : "  << err.what() << endl;
        return EXIT_FAILURE;
    }

    try
    {
        desc = conv->convertFrom(fmtParams);
    }
    catch(r_Error &err)
    {
        cout << "convertFrom(...) conversion from " << fmt << " format to r_Array format failed! " << endl
             << "Error " << err.get_errorno() << " : "  << err.what() << endl;
        if(conv)
        {
            delete conv;
            conv=NULL;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int convertTo(r_Data_Format fmt, const char* fmtParams,
              const char* data, const r_Minterval& dataDom,
              const r_Type* dataType, r_convDesc& desc)
{
    r_Storage_Man_CPP mySM;
    r_Convertor *conv=NULL;

    if(data==NULL)
    {
        cout << "convertTo(...) data is null" << endl;
        return EXIT_FAILURE;
    }

    if(dataType==NULL)
    {
        cout << "convertTo(...) dataType is null" << endl;
        return EXIT_FAILURE;
    }

    try
    {
        conv=r_Convertor_Factory::create(fmt, data, dataDom, dataType);
        conv->set_storage_handler(mySM);
    }
    catch(r_Error& err)
    {
        cout << "convertTo(...) request for convertor " << fmt << " failed! " << endl
             << "Error " << err.get_errorno() << " : "  << err.what() << endl;
        return EXIT_FAILURE;
    }

    try
    {
        desc = conv->convertTo(fmtParams);
    }
    catch(r_Error &err)
    {
        cout << "convertTo(...) conversion from " << fmt << " format to r_Array format failed! " << endl
             << "Error " << err.get_errorno() << " : "  << err.what() << endl;
        if(conv)
        {
            delete conv;
            conv=NULL;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


//init/deinit ConvDesc
void cleanConvDesc(r_convDesc& desc)
{
    desc.src=NULL;
    desc.srcType=NULL;
    if(desc.dest!=NULL)
    {
        delete []desc.dest;
        desc.dest=NULL;
    }
    if(desc.destType!=NULL)
    {
        delete desc.destType;
        desc.destType=NULL;
    }
    desc.baseType = r_Convertor::ctype_void;
}

void initConvDesc(r_convDesc& desc)
{
    desc.dest=NULL;
    desc.src=NULL;
    desc.srcType=NULL;
    desc.destType=NULL;
    desc.baseType = r_Convertor::ctype_void;
}

//decode a minterval from a string
int decodeMinterval(const char* src, r_Minterval& srcIv)
{
    if(!src)
    {
        cout << "decodeMinterval(...) src is null" << endl;
        return EXIT_FAILURE;
    }

    try
    {
        srcIv=r_Minterval(src);
    }
    catch(r_Error& err)
    {
        cout << "decodeMinterval(...) error while constructing the minterval from " << src << endl;
        cout << "Error " << err.get_errorno() << ":" << err.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//decode a type from a string
int decodeType(const char* src, r_Type*& srcType)
{
    if(!src)
    {
        cout << "decodeType(...) src is null" << endl;
        return EXIT_FAILURE;
    }

    if(srcType)
    {
        cout << "decodeType(...) src is not null" << endl;
        return EXIT_FAILURE;
    }

    try
    {
        srcType=r_Type::get_any_type(src);
    }
    catch(r_Error& err)
    {
        cout << "decodeType(...) error while constructing the type from " << src << endl;
        cout << "Error " << err.get_errorno() << ":" << err.what() << endl;
        return EXIT_FAILURE;
    }

    if(!srcType)
    {
        cout << "decodeType(...) the type retrived with r_Type::get_any_type(...) is null" << endl;
        return EXIT_FAILURE;
    }

    if(!srcType->isBaseType())
    {
        cout << "decodeType(...) the type retrived (" << srcType->type_id() << ") is not a base type" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//parse r_Array params 'domain=<domain_str>,type=<type_str>'
int parseArrayParams(const char* formatparams,
                     const char* fpDomain,
                     const char* fpType,
                     char*& domain,
                     char*& type)
{
    const int fpNo=2;
    r_Parse_Params params(fpNo);

//parameter validation
    if(formatparams==NULL)
    {
        cout << "parseArrayParams(...) formatparams is null" << endl;
        return EXIT_FAILURE;
    }
    if(fpDomain==NULL)
    {
        cout << "parseArrayParams(...) fpDomain is null" << endl;
        return EXIT_FAILURE;
    }
    if(fpType==NULL)
    {
        cout << "parseArrayParams(...) fpType is null" << endl;
        return EXIT_FAILURE;
    }
    if(domain!=NULL)
    {
        cout << "parseArrayParams(...) domain is not null" << endl;
        return EXIT_FAILURE;
    }
    if(type!=NULL)
    {
        cout << "parseArrayParams(...) type is not null" << endl;
        return EXIT_FAILURE;
    }

    params.add(fpDomain, &domain, r_Parse_Params::param_type_string);
    params.add(fpType, &type, r_Parse_Params::param_type_string);

    if(params.process(formatparams) != fpNo)
    {
        cout << "parseArrayParams(...) error parsing " << formatparams << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//compare 2 structure type
int compareStructure(r_Structure_Type* src, r_Structure_Type* dest)
{
    r_Structure_Type::attribute_iterator iterSrc, iterDest;

    if(src==NULL)
    {
        cout << "compareStructure(...) src is null !"<< endl;
        return EXIT_FAILURE;
    }

    if(dest==NULL)
    {
        cout << "compareStructure(...) dest is null !"<< endl;
        return EXIT_FAILURE;
    }

    iterSrc=src->defines_attribute_begin();
    iterDest=dest->defines_attribute_begin();
    while( iterSrc!=src->defines_attribute_end() || iterDest!=dest->defines_attribute_end())
    {
        r_Type::r_Type_Id typeSrcId=r_Type::UNKNOWNTYPE, typeDestId=r_Type::UNKNOWNTYPE;
        typeSrcId=(*iterSrc).type_of().type_id();
        typeDestId=(*iterDest).type_of().type_id();

        if(typeSrcId!=typeDestId)
        {
            cout << "comparaStructure(...) typeSrcId(" << typeSrcId
                 << ") != typeDestId(" << typeDestId << ") !"<< endl;
            return EXIT_FAILURE;
        }

        if((typeSrcId== r_Type::STRUCTURETYPE) &&
                (compareStructure((r_Structure_Type*)(&((*iterSrc).type_of())),
                                  (r_Structure_Type*)(&((*iterDest).type_of()))) != EXIT_SUCCESS))
            return EXIT_FAILURE;

        iterSrc++;
        iterDest++;
    }
    if((iterSrc==src->defines_attribute_end()) &&
            (iterDest==dest->defines_attribute_end()))
        return EXIT_SUCCESS;
    else
    {
        if(iterSrc!=src->defines_attribute_end())
            cout << "compareStructure(...) src has more atributes then dest!" << endl;
        else
            cout << "compareStructure(...) dest has more atributes then src!" << endl;
        return EXIT_FAILURE;
    }
}
