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
 * INCLUDE: rasgeo_error.hh
 *
 * MODULE:  
 * CLASS:   InitError
 *
 * CHANGE HISTORY (append further entries):
 * when         who	what
 * ----------------------------------------------------------
 * 2003-aug-24	PB	cloned from rasgeo/importools/rasgeo_error.hh
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _RASGEO_ERROR_HH_
#define _RASGEO_ERROR_HH_

#ifdef __VISUALC__
#pragma warning( disable : 4290 )
#endif

//@ManMemo: Module: {\bf raslib}

/*@Doc:

 This class ...
*/


/// valid error codes:
#define ALLDONE                   -1
#define OK                        0
#define NOCOLLNAME                1
#define INVALIDFLOAT              2
#define NOXMIN                    3
#define NOXMAX                    4
#define NOYMIN                    5
#define NOYMAX                    6
#define NORES                     7
#define ERRORPARSINGCOMMANDLINE   8
#define INVALIDBBOX               9
#define INVALIDLEVEL             10
#define ILLEGALMAPTYPE           11
#define NOMAPTYPE                12
#define INVALIDTILEEDGE          13
#define ALLOCFAIL                14
#define NOTINSERTED              15

class InitError // : public std::exception
{
  public:

    /// constructor receiving an error number
    InitError( unsigned int e );
    
    /// destructor
    virtual ~InitError();
  
    /// get an error description
    virtual const char * what(); 

  private:
    /// error information
    unsigned int errorCode;
};

#endif // _RASGEO_ERROR_HH_
