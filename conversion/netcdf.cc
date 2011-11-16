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
 * MERCHANTrABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
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
 * SOURCE: netcdf.cc
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_NETCDF
 *
 * COMMENTS:
 *
 * Provides functions to convert data to NETCDF and back.
 */

#ifdef HAVE_NETCDF

#include "conversion/netcdf.hh"
#include "conversion/memfs.hh"
#include "raslib/error.hh"
#include "raslib/rminit.hh"
#include "raslib/parseparams.hh"
#include "raslib/primitivetype.hh"

#include "netcdfcpp.h"
#include "raslib/odmgtypes.hh"

#include <cstring>
#include <sstream>
#include <iostream>
#include <string>
#include <set>
#include <math.h>
#include <cmath>

#define DEFAULT_VAR "data"

using namespace std;

void r_Conv_NETCDF::initNETCDF(void) {
    RMInit::logOut << "initializing netcdf...";
    Conventions = "CF-1.4";
    Institution = "rasdaman.org, Jacobs University Bremen";
    
    variable = NULL;
    if (params == NULL)
      params = new r_Parse_Params();
    params->add("var", &variable, r_Parse_Params::param_type_string);
    RMInit::logOut << " done." << endl;
}

/// constructor using an r_Type object. Exception if the type isn't atomic.
r_Conv_NETCDF::r_Conv_NETCDF(const char *src, const r_Minterval &interv, const r_Type *tp) throw (r_Error)
: r_Convert_Memory(src, interv, tp, true) {
    initNETCDF();
    /// ToDo: Can be hacked by dividing it to its basic component. Or by using CXX-4 which is in the development phase
    if (tp->isStructType()) {
        RMInit::logOut << "r_Conv_NETCD::r_Conv_NETCDF(): structured types not supported." << endl;
        throw r_Error(r_Error::r_Error_General);
    }
}

/// constructor using convert_type_e shortcut
r_Conv_NETCDF::r_Conv_NETCDF(const char *src, const r_Minterval &interv, int tp) throw (r_Error)
: r_Convert_Memory(src, interv, tp) {
    initNETCDF();
}


/// destructor
r_Conv_NETCDF::~r_Conv_NETCDF(void) {
}

/// convert to NETCDF
r_convDesc &r_Conv_NETCDF::convertTo(const char *options) throw (r_Error) {
    char fileName[256];
    string dimNamePrefix = "dim_";
    int dimNo = 0;
    long *dimSizes;
    long dataSize = 1;
    size_t filesize = 0;
    FILE *fp = NULL;
    const char *src = desc.src;

    RMInit::logOut << "r_Conv_NETCDF::convertTo" << endl;
    
    if (options != NULL) {
      RMInit::logOut << "options: " << options << endl;
      params->process(options);
    }

    strncpy(fileName, tmpnam(NULL), 256);

    // Create the file. The Replace parameter tells netCDF to overwrite
    // this file, if it already exists.
    NcFile dataFile(fileName, NcFile::Replace);
    if (!dataFile.is_valid()) {
        RMInit::logOut << "r_Conv_NETCDF::convertTo(): unable to open output file." << endl;
        throw r_Error(r_Error::r_Error_General);
    }

    //Create netCDF dimensions
    dimNo = desc.srcInterv.dimension();
    dimSizes = new long[dimNo];
    const NcDim* dims[dimNo];
    if (dimSizes == NULL) {
        RMInit::logOut << "r_Conv_NETCDF::convertTo(): out of memory error!" << endl;
        throw r_Error(r_Error::r_Error_MemoryAllocation);
    }
    for (int i = 0; i < dimNo; i++) {
        dimSizes[i] = desc.srcInterv[i].high() - desc.srcInterv[i].low() + 1;
        dataSize *= dimSizes[i];
    }

    // Add the dimensions to the netcdf file, the dimension names are
    // Dimension_0, ..,Dimension_rank-1. Add also the dimension interval
    for (int i = 0; i < dimNo; i++) {
        stringstream s;
        s << dimNamePrefix << i;
        NcDim* dim = dataFile.add_dim(s.str().c_str(), dimSizes[i]);
        dims[i] = dim;
    }

    // Define a netCDF variable. The type of the variable depends on rasdaman type
    char* varName = DEFAULT_VAR;
    if (variable != NULL) {
      varName = variable;
    }
    switch (desc.baseType) {
        case ctype_char:
        case ctype_uint8:
        case ctype_int8:
        {
            r_Char *val = (r_Char* &) src;
            char *data = new char[dataSize];
            if (data == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertTo(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            for (int i = 0; i < dataSize; i++, val++) {
                data[i] = (char) val[0];
            }
            // Define a netCDF variable, in this case of type ncChar
            NcVar *ncVar = dataFile.add_var(varName, ncChar, dimNo, dims);
            // Write the data to the file.
            ncVar->put(&data[0], dimSizes);
            delete [] data;
            break;
        }
        case ctype_int16:
        case ctype_uint16:
        {
            r_Short *val = (r_Short* &) src;
            short *data = new short[dataSize];
            if (data == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertTo(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            for (int i = 0; i < dataSize; i++, val++) {
                data[i] = (short) val[0];
            }
            NcVar *ncVar = dataFile.add_var(varName, ncShort, dimNo, dims);
            ncVar->put(&data[0], dimSizes);
            delete [] data;
            break;
        }
        case ctype_int32:
        case ctype_uint32:
        case ctype_int64:
        case ctype_uint64:
        {
            r_Long *val = (r_Long* &) src;
            int *data = new int[dataSize];
            if (data == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertTo(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            for (int i = 0; i < dataSize; i++, val++) {
                data[i] = (int) val[0];
            }
            NcVar *ncVar = dataFile.add_var(varName, ncInt, dimNo, dims);
            ncVar->put(&data[0], dimSizes);
            delete [] data;
            break;
        }
        case ctype_float32:
        {
            r_Float *val = (r_Float* &) src;
            float *data = new float[dataSize];
            if (data == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertTo(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            for (int i = 0; i < dataSize; i++) {
                data[i] = (float) val[i];
            }
            NcVar *ncVar = dataFile.add_var(varName, ncFloat, dimNo, dims);
            ncVar->put(&data[0], dimSizes);
            ncVar->add_att("missing_value", "NaNf");
            delete [] data;
            break;
        }
        case ctype_float64:
        {
            r_Double *val = (r_Double* &) src;
            double *data = new double[dataSize];
            if (data == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertTo(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            for (int i = 0; i < dataSize; i++, val++) {
                data[i] = (double) val[0];
            }
            NcVar *ncVar = dataFile.add_var(varName, ncDouble, dimNo, dims);
            ncVar->put(&data[0], dimSizes);
            ncVar->add_att("missing_value", "NaN");
            delete [] data;
            break;
        }
        default:
        {
            RMInit::logOut << "This Type is not supported" << desc.baseType << endl;
            throw r_Error(r_Error::r_Error_General);
        }
    }

    dataFile.add_att("Conventions", Conventions);
    dataFile.add_att("Institution", Institution);
    dataFile.close();
    // The data file is written and closed

    // Pass the NetCDF file as a stream of char
    if ((fp = fopen(fileName, "rb")) == NULL) {
        RMInit::logOut << "r_Conv_NETCDF::convertTo(): unable to read back file." << endl;
        throw r_Error(r_Error::r_Error_General);
    }
    // Get the file size
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);

    if ((desc.dest = (char*) mystore.storage_alloc(filesize)) == NULL) {
        RMInit::logOut << "Error:  out of memory." << endl;
        throw r_Error(r_Error::r_Error_MemoryAllocation);
    }
    // Set the desc.dest content
    fseek(fp, 0, SEEK_SET);
    fread(desc.dest, 1, filesize, fp);
    fclose(fp);
    // Free Memory
    remove(fileName);
    delete [] dimSizes;
    //Set the interval and type
    desc.destInterv = r_Minterval(1);
    desc.destInterv << r_Sinterval((r_Range) 0, (r_Range) filesize - 1);
    desc.destType = r_Type::get_any_type("char");

    RMInit::logOut << "r_Conv_NETCDF::convertTo EXIT" << endl;
    return desc;
}

/// convert from NETCDF
r_convDesc &r_Conv_NETCDF::convertFrom(const char *options) throw (r_Error) {
    long dataSize = 1;
    long *dimSizes;
    char tmpFile [] = "/tmp/tmp.nc";

    RMInit::logOut << "r_Conv_NETCDF::convertFrom" << endl;
    
    if (options != NULL) {
      params->process(options);
    }

    const char* src = desc.src;
    // Just write the data to temp file.
    ofstream file;
    file.open(tmpFile);
    for (int i = 0; i < desc.srcInterv[0].high() - desc.srcInterv[0].low() + 1; i++, src++) {
        file << *src;
    }
    file.close();

    // Open the file. The ReadOnly parameter tells netCDF we want
    // read-only access to the file.
    NcFile dataFile(tmpFile, NcFile::ReadOnly);
    if (!dataFile.is_valid()) {
        RMInit::logOut << "r_Conv_NETCDF::convertFrom(): Can not open the file" << endl;
        throw r_Error(r_Error::r_Error_General);
    }

    // Number of dimensions and varaiables
    int numDims = dataFile.num_dims();
    int numVars = dataFile.num_vars();

    // Get a set of dimension names. This is used to get all the variables that are not dimensions
    set<string> dimNames;
    set<string> varNames;
    set<string>::iterator it;
    for (int i = 0; i < numDims; i++) {
        NcDim *dim = dataFile.get_dim(i);
        dimNames.insert(dim->name());
    }

    // Get a set of variable names that are not dimensions. and defined by all dimensions
    for (int i = 0; i < numVars; i++) {
        NcVar *var = dataFile.get_var(i);
        it = dimNames.find(var->name());
        if (it == dimNames.end() && var->num_dims() > 0) {
          if (variable == NULL || strcmp(var->name(), variable) == 0) {
            varNames.insert(var->name());
            if (variable != NULL)
              break;
          }
        }
    }
    if (varNames.empty()) {
      RMInit::logOut << "r_Conv_NETCDF::convertFrom(): no variable found to import." << endl;
      throw r_Error(r_Error::r_Error_General);
    }

    it = varNames.begin();
    string varName = *it;
    NcVar *var = dataFile.get_var(varName.c_str());
    numDims = var->num_dims();
    dimSizes = new long[numDims];
    for (int i = 0; i < numDims; i++) {
        NcDim *dim = var->get_dim(i);
        if (dim->is_unlimited()) {
            RMInit::logOut << "r_Conv_NETCDF::convertFrom(): unlimited dimensions can not be handled" << endl;
            throw r_Error(r_Error::r_Error_General);
        }
        dimSizes[i] = dim->size();
        dataSize *= dim->size();
    }
    switch (var->type()) {
        case ncByte:
        case ncChar:
        {
            char *data = new char[dataSize];
            if (data == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertFrom(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            var->get(&data[0], dimSizes);

            desc.destInterv = r_Minterval(numDims);
            // Ignor NetCDF dim interval and assume it always starts at zero
            // ToDo: Add a parse object to allow the user to control the dim interval, i.e. start at 0 or not
            for (int i = 0; i < numDims; i++)
                desc.destInterv << r_Sinterval((r_Range) 0, (r_Range) dimSizes[i] - 1);

            if ((desc.dest = (char*) mystore.storage_alloc(dataSize)) == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertFrom(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            memcpy(desc.dest, data, dataSize * sizeof (char));
            desc.destType = get_external_type(ctype_char);
            delete [] data;
            break;
        }
        case ncShort:
        {
            short *data = new short[dataSize];
            if (data == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertFrom(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            var->get(&data[0], dimSizes);

            desc.destInterv = r_Minterval(numDims);
            for (int i = 0; i < numDims; i++)
                desc.destInterv << r_Sinterval((r_Range) 0, (r_Range) dimSizes[i] - 1);

            if ((desc.dest = (char*) mystore.storage_alloc(dataSize * sizeof (short))) == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertFrom(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            memcpy(desc.dest, data, dataSize * sizeof (short));
            desc.destType = get_external_type(ctype_int16);
            delete [] data;
            break;
        }
        case ncInt:
        {
            int *data = new int[dataSize];
            if (data == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertFrom(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            var->get(&data[0], dimSizes);

            desc.destInterv = r_Minterval(numDims);
            for (int i = 0; i < numDims; i++)
                desc.destInterv << r_Sinterval((r_Range) 0, (r_Range) dimSizes[i] - 1);

            if ((desc.dest = (char*) mystore.storage_alloc(dataSize * sizeof (int))) == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertFrom(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            memcpy(desc.dest, data, dataSize * sizeof (int));
            desc.destType = get_external_type(ctype_int32);
            delete [] data;
            break;
        }
        case ncFloat:
        {
            float *data = new float[dataSize];
            if (data == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertFrom(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            var->get(&data[0], dimSizes);

            desc.destInterv = r_Minterval(numDims);
            for (int i = 0; i < numDims; i++)
                desc.destInterv << r_Sinterval((r_Range) 0, (r_Range) dimSizes[i] - 1);

            if ((desc.dest = (char*) mystore.storage_alloc(dataSize * sizeof (float))) == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertFrom(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            memcpy(desc.dest, data, dataSize * sizeof (float));
            desc.destType = get_external_type(ctype_float32);
            delete [] data;
            break;
        }
        case ncDouble:
        {
            double *data = new double[dataSize];
            if (data == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertFrom(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            var->get(&data[0], dimSizes);

            desc.destInterv = r_Minterval(numDims);
            for (int i = 0; i < numDims; i++)
                desc.destInterv << r_Sinterval((r_Range) 0, (r_Range) dimSizes[i] - 1);

            if ((desc.dest = (char*) mystore.storage_alloc(dataSize * sizeof (double))) == NULL) {
                RMInit::logOut << "r_Conv_NETCDF::convertFrom(): out of memory error!" << endl;
                throw r_Error(r_Error::r_Error_MemoryAllocation);
            }
            memcpy(desc.dest, data, dataSize * sizeof (double));
            desc.destType = get_external_type(ctype_float64);
            delete [] data;
            break;
        }
        default:
        {
            RMInit::logOut << "This Type is not supported" << desc.baseType << endl;
            throw r_Error(r_Error::r_Error_General);
        }

    }
    RMInit::logOut << "r_Conv_NETCDF::convertFrom EXIT" << endl;
    return desc;
}

/// cloning

r_Convertor *r_Conv_NETCDF::clone(void) const {
    return new r_Conv_NETCDF(desc.src, desc.srcInterv, desc.baseType);
}

/// identification

const char *r_Conv_NETCDF::get_name(void) const {
    return format_name_netcdf;
}

r_Data_Format r_Conv_NETCDF::get_data_format(void) const {
    return r_NETCDF;
}
/// For test purpose

template <class baseType, class castType> void r_Conv_NETCDF::print(baseType* val, int bufferZise) {

    for (int i = 0; i < bufferZise; ++i, val++)
        RMInit::logOut << (castType) val[0] << endl;
}

#endif

/* NcType is defined as enum as follows
Name			size		Comments
ncByte			1		8-bit signed integer
ncChar			1		8-bit unsigned integer
ncShort			2		signed 2 byte integer
ncInt			4		32-bit signed integer
ncLong			4		Deprecated
ncFloat			4		32-bit floating point
ncDouble		8		64-bit floating point
 */
