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
 * INCLUDE: odmgtypes.hh
 *
 * MODULE:  rasodmg
 *
 * PURPOSE:
 *      The file contains ODMG type definitions.
 *
 * COMMENTS:
 *  For further porting please adapt the typedef for r_Long and r_Ulong
*/

#ifndef _D_ODMGTYPES_
#define _D_ODMGTYPES_

// for type-limits
#include <limits.h>
#include <float.h>

//@Man: r_Char
//@Type: typedef
//@Args: as unsigned char (1 byte)
//@Memo: Module: {\bf rasodmg}.

typedef unsigned char r_Char;

/**
  {\tt typedef unsigned char r_Char;}
*/



//@Man: r_Octet
//@Type: typedef
//@Args: as signed char (1 byte)
//@Memo: Module: {\bf rasodmg}.

typedef signed char r_Octet;

/**
  {\tt typedef signed char r_Octet;}
  (Stroustroup: sign of plain char is implementation-defined)
*/



//@Man: r_Short
//@Type: typedef
//@Args: as short (2 bytes)
//@Memo: Module: {\bf rasodmg}.

typedef short r_Short;

/**
  {\tt typedef short r_Short;}
*/



//@Man: r_UShort
//@Type: typedef
//@Args: as unsigned short (2 bytes)
//@Memo: Module: {\bf rasodmg}.

typedef unsigned short r_UShort;

/**
  {\tt typedef short r_UShort;}
*/



//@Man: r_Long
//@Type: typedef
//@Args: as long (4 bytes)
//@Memo: Module: {\bf rasodmg}.

typedef int r_Long;
/**
  {\tt typedef int r_Long;}
*/





//@Man: r_ULong
//@Type: typedef
//@Args: as unsigned long (4 bytes)
//@Memo: Module: {\bf rasodmg}.

typedef unsigned int r_ULong;
/**
  {\tt typedef unsigned long r_ULong;}
*/


//@Man: r_Float
//@Type: typedef
//@Args: as float
//@Memo: Module: {\bf rasodmg}.

typedef float r_Float;

/**
  {\tt typedef float r_Float;}
*/



//@Man: r_Double
//@Type: typedef
//@Args: as double
//@Memo: Module: {\bf rasodmg}.

typedef double r_Double;

/**
  {\tt typedef double r_Double;}
*/



//@Man: r_Boolean
//@Type: typedef
//@Args: as unsigned char (1 byte)
//@Memo: Module: {\bf rasodmg}.

typedef unsigned char r_Boolean;

/**
  {\tt typedef unsigned char r_Boolean;}
  Changed to unsigned char
*/


//@Man: get_limits()
//@Type: function
//@Args: as function
//@Memo: Module: {\bf rasodmg}

inline void get_limits( const r_Octet *tptr, double &min, double &max )
{
    min = (double)SCHAR_MIN;
    max = (double)SCHAR_MAX;
}

inline void get_limits( const r_Char *tptr, double &min, double &max )
{
    min = (double)0.0;
    max = (double)UCHAR_MAX;
}

inline void get_limits( const r_Short *tptr, double &min, double &max )
{
    min = (double)SHRT_MIN;
    max = (double)SHRT_MAX;
}

inline void get_limits( const r_UShort *tptr, double &min, double &max )
{
    min = (double)0.0;
    max = (double)USHRT_MAX;
}

inline void get_limits( const r_Long *tptr, double &min, double &max )
{
    min = (double)INT_MIN;
    max = (double)INT_MAX;
}

inline void get_limits( const r_ULong *tptr, double &min, double &max )
{
    min = (double)0.0;
    max = (double)UINT_MAX;
}

inline void get_limits( const r_Float *tptr, double &min, double &max )
{
    min = -((double)FLT_MAX);
    max = (double)FLT_MAX;
}

inline void get_limits( const r_Double *tptr, double &min, double &max )
{
    min = -((double)DBL_MAX);
    max = (double)DBL_MAX;
}

#endif
