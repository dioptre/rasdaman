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
 * SOURCE: convertor.cc
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Convertor_Factory
 * 	Create convertors out of data formats
 *
 * COMMENTS:
 * - temporary, for debugging
 */

#include "raslib/rminit.hh"
#include "debug.hh"

#include "conversion/convfactory.hh"

// all the conversion types, for easy creation
#include "tiff.hh"
#include "hdf.hh"
#include "png.hh"
#include "jpeg.hh"
#include "bmp.hh"
#include "vff.hh"
#include "tor.hh"
#include "dem.hh"
#include "ecw.hh"
#include "ntf.hh"
#include "csv.hh"
#include "netcdf.hh"


bool r_Convertor_Factory::is_supported( r_Data_Format fmt )
{
	ENTER( "r_Convertor_Factory::is_supported( " << fmt << " )" );

	bool retval=false;
	switch (fmt)
	{
	case r_TIFF:
	case r_PNG:
	case r_JPEG:
	case r_BMP:
	case r_VFF:
	case r_TOR:
	case r_DEM:
	case r_ECW:
#ifdef HAVE_HDF
	case r_HDF:
#endif
		// case r_NTF:
	case r_NETCDF:
		retval=true;
		break;
	default:
		retval=false;
		break;
	}

	LEAVE( "r_Convertor_Factory::is_supported() -> " << retval );
	return retval;
}

r_Convertor *r_Convertor_Factory::create( r_Data_Format fmt, const char *src, const r_Minterval &interv, const r_Type *tp) throw(r_Error)
{
	ENTER( "r_Convertor_Factory::create( fmt=" << fmt << ", &src=" << ((r_Ptr) src) << ", interval=" << interv << ", &type=" << ((r_Ptr) tp) << " )" );
	r_Convertor *result = NULL;

	switch (fmt)
	{
	case r_TIFF:
		result = new r_Conv_TIFF(src, interv, tp);
		break;
	case r_NETCDF:
		result = new r_Conv_NETCDF(src, interv, tp);
		break;
	case r_PNG:
		result = new r_Conv_PNG(src, interv, tp);
		break;
	case r_CSV:
		result = new r_Conv_CSV(src, interv, tp);
		break;
	case r_JPEG:
		result = new r_Conv_JPEG(src, interv, tp);
		break;
	case r_BMP:
		result = new r_Conv_BMP(src, interv, tp);
		break;
	case r_VFF:
		result = new r_Conv_VFF(src, interv, tp);
		break;
	case r_TOR:
		result = new r_Conv_TOR(src, interv, tp);
		break;
	case r_DEM:
		result = new r_Conv_DEM(src, interv, tp);
		break;      
	case r_ECW:
		result = new r_Conv_ECW(src, interv, tp);
		break; 
#ifdef HAVE_HDF
	case r_HDF:
		result = new r_Conv_HDF(src, interv, tp);
		break;
#endif
		// case r_NTF:
		//   TALK( "creating NTF converter..." );
		//   result = new r_Conv_NTF(src, interv, tp);
		//   break;      
	default:
		RMInit::logOut << "Error: in conversion factory during create: unsupported format: " << fmt << endl;
		r_Error err(CONVERSIONFORMATNOTSUPPORTED);
		throw(err);
	}

	LEAVE( "r_Convertor_Factory::create() -> " << result );
	return result;
}


r_Convertor *r_Convertor_Factory::create( r_Data_Format fmt, const char *src, const r_Minterval &interv, int type ) throw(r_Error)
{
	ENTER( "r_Convertor_Factory::create( fmt=" << fmt << ", &src=" << ((r_Ptr) src) << ", interval=" << interv << ", type=" << type << " )" );

	r_Convertor *result = NULL;

	switch (fmt)
	{
	case r_TIFF:
		result = new r_Conv_TIFF(src, interv, type);
		break;
	case r_NETCDF:
		result = new r_Conv_NETCDF(src, interv, type);
		break;
	case r_PNG:
		result = new r_Conv_PNG(src, interv, type);
		break;
	case r_JPEG:
		result = new r_Conv_JPEG(src, interv, type);
		break;
	case r_BMP:
		result = new r_Conv_BMP(src, interv, type);
		break;
	case r_VFF:
		result = new r_Conv_VFF(src, interv, type);
		break;
	case r_TOR:
		result = new r_Conv_TOR(src, interv, type);
		break;      
	case r_DEM:
		result = new r_Conv_DEM(src, interv, type);
		break;            
	case r_ECW:
		result = new r_Conv_ECW(src, interv, type);
		break; 
#ifdef HAVE_HDF
	case r_HDF:
		result = new r_Conv_HDF(src, interv, type);
		break;
#endif
		// case r_NTF:
		//   TALK( "creating NTF converter..." );
		//   result = new r_Conv_NTF(src, interv, type);
		//   break;      
	default:
		RMInit::logOut << "Error: in conversion factory during create: unsupported format: " << fmt << endl;
		r_Error err(CONVERSIONFORMATNOTSUPPORTED);
		throw(err);
	}

	LEAVE( "r_Convertor_Factory::create() -> " << result );
	return result;
}

