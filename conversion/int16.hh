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
 * INCLUDE: int16.hh
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_int16
 *
 * COMMENTS:
 * Provides interface to convert data to other formats.
*/

#ifndef _R_CONV_int16_HH_
#define _R_CONV_int16_HH_

#include <sstream>
#include <vector>
#include <string>
#include <cstdio>
using std::vector;
using std::ofstream;
using std::string;

#include "conversion/convertor.hh"
#include "raslib/odmgtypes.hh"

//@ManMemo: Module {\bf conversion}

/*@Doc:
  int16 convertor class.

  Supported parameters are

  \begin{tabular}{lcl}
  {\tt flipx}  && int    && flip image flag on x axis, default 0\\
  {\tt flipy}  && int    && flip image flag on y axis, default 1\\
  {\tt startx} && double && start value on x axis \\
  {\tt endx}   && double && end value on x axis \\
  {\tt sizex}  && int    && number of pixels in horizontal direction (int >0)
  {\tt sizey}  && int    && number of pixels in vertical direction (int >0)
  {\tt resx}   && double && resolution on x axis \\
  {\tt starty} && double && start value on y axis \\
  {\tt endy}   && double && end value on y axis \\
  {\tt resy}   && double && resolution on y axis \\
  {\tt hstep}  && double && resolution on h axis \\
  \end{tabular}

  The "flipx" parameter is a flag for mirroring the image on x axis.
  The "flipy" parameter is a flag for mirroring the image on y axis.
  [startx:endx, starty:endy] represents the geographical bounding box
  of the whole image. The corresponding pixel bounding box is calculated
  as follows:
  if flipy is disabled:
    [(minx-startx)/resx:(maxx-startx)/resx, (miny-starty)/resy:(maxy-starty)/resy]
  else
    [(minx-startx)/resx:(maxx-startx)/resx, (endy-maxy)/resy:(endy-miny)/resy]

  if flipx is disabled:
    [(minx-startx)/resx:(maxx-startx)/resx, (miny-starty)/resy:(maxy-starty)/resy]
  else
    [(endx-maxx)/resx:(endx-minx)/resx, (miny-starty)/resy:(maxy-starty)/resy]

  The pairs (startx, endx, resx), (starty, endy, resy) apply to the whole MDD object in the database
  whereas the pairs (minx,maxx, resx), (miny, maxy, resy) describe the part of image under consideration.
  They are used to compute the update part's position in RasDaMan coordinates.
  Value hstep is vertical resolution (multiplied into the value read during conversion into internal format).
*/


Format specification:
int16

input parameters:
geox    geo reference x of upper left point (float >0)
geoy    geo reference y of upper left point (float >0)
resx    horizontal resolution (pixel distance) in meters (float >0)
resy    vertical resolution (pixel distance) in meters (float >0)
hstep   factor by which pixel values have to be multiplied to obtain real height in meters (float >0)


An int16 file contains a sequence of sizex*sizey height values, advancing from west to east and from north to south. Each pixel consists of a 16 bit integer where the lower byte comes first in sequence (i.e., pixel value is byte[i]+byte[i+1]*256).
There is no file header, pixels start immediately at the beginning.

Points are defined as follows for pixel position (i,j) in file (starting with (0/0):
                geo position x = geox + i*resx
                                 geo position y = geoy + j*resy
                                         height = ( byte[ 2*i + 2*j*sizex] + byte[ 2*i + 2*j*sizex + 1] * 256 ) * hstep

                                                 where / denotes integer division

                                                 class r_Conv_int16  :   public r_Convertor
{
public:
    // constants to handle NULL
    static const r_Double NULL_DB;
    static const r_Double ZERO_DB;
    static const r_Double ZERO_int16;

    //inner class for convertor parameters
    class r_GeoBBox
    {
    public:
        r_Double startx, endx, resx;
        r_Double starty, endy, resy;
        r_ULong flipy, flipx;
    };

    r_Conv_int16(const char* source, const r_Minterval& lengthordomain, const r_Type* tp) throw(r_Error);

    r_Conv_int16(const char* source, const r_Minterval& lengthordomain, int tp) throw(r_Error);

    r_convDesc& convertFrom(const char* options = NULL) throw (r_Error);

    r_convDesc& convertTo(const char* options = NULL) throw (r_Error);

    const char* get_name() const throw();

    r_Data_Format get_data_format() const throw();

    r_Convertor* clone() const throw(r_Error);

    /// dimension of src domain accepted as input in convertFrom
    static const r_Dimension srcIntervDim;

    /// dimension of dest domain accepted as input in convertTo
    static const r_Dimension destIntervDim;

    /// decode convertor options
    static bool decodeOptions( const char* options,
                               r_GeoBBox& collBBox) throw();

    /// encode convertor options
    static string encodeOptions(const r_GeoBBox& collBBox) throw();

    /// destructor
    virtual ~r_Conv_int16( void );

    /// init convertor parameters to default value
    static void initGeoBBox( r_GeoBBox& cBBox );

private:


    /// check limits before converting
    void checkLimits() throw(r_Error);

    ///i/o src/dest stream
    void readFromSrcStream() throw(r_Error);
    void readToSrcStream() throw(r_Error);
    void writeFromDestStream() throw(r_Error);
    void writeToDestStream(ofstream& oFile) throw(r_Error);

    /// parameters
    r_GeoBBox collBBox;

    /// class constants
    static const r_ULong paramMin;
    static const char* paramSep;
    static const char* paramEq;
    static const char* paramFlipX;
    static const char* paramFlipY;
    static const char* paramStartX;
    static const char* paramEndX;
    static const char* paramResX;
    static const char* paramStartY;
    static const char* paramEndY;
    static const char* paramResY;


    /// internal data
    class int16Row
    {
    public:
        r_Double x,y,h;
    };

    typedef vector<int16Row> int16RowVec;

    int16Row  min, max;
    int16RowVec demRows;

};

#endif

