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
 * INCLUDE: ecw.hh
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_ECW
 *
 * COMMENTS:
 * Provides interface to convert data from ECW to r_Array
 *
*/

#ifndef _R_CONV_ECW_HH_
#define _R_CONV_ECW_HH_ 
#include "conversion/convertor.hh"

//@ManMemo: Module {\bf conversion}

/*@Doc:
  ECW convertor class.

  Supported parameters are

  \begin{tabular}{lcl}
  {\tt windowx} && uint && maximum number of colums to convert in one ecw call, default 1000\\
  {\tt windowy} && uint && maximum number of lines to convert in one ecw call, default 1000\\
  \end{tabular}
*/


class r_Conv_ECW	:	public r_Convertor
	{
	public:
		r_Conv_ECW(const char* source, const r_Minterval& lengthordomain, const r_Type* tp) throw(r_Error);

		r_Conv_ECW(const char* source, const r_Minterval& lengthordomain, int tp) throw(r_Error);

		r_convDesc& convertFrom(const char* options = NULL) throw (r_Error);

		r_convDesc& convertTo(const char* options = NULL) throw (r_Error);

		const char* get_name() const;

		r_Data_Format get_data_format() const;

		r_Convertor* clone() const;


	private:
		void initECW();
		
	};

#endif


