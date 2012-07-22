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
 * SOURCE: test_polygon.cc
 *
 * MODULE: rasodmg
 *
 * COMMENTS:
 *      None
 */

#include "mymalloc/mymalloc.h"

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

extern "C" {
#include "tiffio.h"
}

#include <iostream>
#include <vector>

using std::vector;
using std::iterator;

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#if defined(SOLARIS)
#include <strings.h>
#else
#include <string.h>
#endif

#include "rasodmg/polygon.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/oqlquery.hh"
#include "rasodmg/fastscale.hh"
#include "raslib/basetype.hh"
#include "raslib/mitera.hh"
#include "raslib/shhopt.h"
#include "rasodmg/ref.hh"
#include "conversion/tiff.hh"

#include "cmlinterpreter.hh"
#include "cmloption.hh"
#include "cmlerror.hh"

// global limits
static const int MYSTRINGSIZE = 256;
static const int SUCCES       = 0;
static const int FAILED       = 1;

// global variable used in this program
r_Database db;
r_Transaction ta;
r_Float rScale;
char*  rBgr=NULL;
r_Polygon poly;

// default values & constants
static const char*    DefaultSrv    = "localhost";
static const char*    DefaultPort   = "7001";
static const char*    DefaultDb     = "RASBASE";
static const char*    DefaultUsr    = "rasguest";
static const char*    DefaultPasswd = "rasguest";
static const r_Float  DefaultScale  = 1.;




// parameters of this program
CmlInterpreter cmlInter;
CmlStringOption cmlSrv('s',"server", "srv-name", "name of machine running RasDaMan manager. Default: localhost");
CmlStringOption cmlPort(0,"port", "nnnn", "port number used by RasDaMan manager. Default: 7001");
CmlStringOption cmlDb('d',"database", "db-name", "name of database. Default: RASBASE).");
CmlStringOption cmlUsr(0, "user", "user-name", "name of user. Default rasguest");
CmlStringOption cmlPasswd(0,"passwd", "user-passwd", "password of user. Default rasguest");
CmlStringOption cmlColl('c',"collname", "coll-name", "name of collection. Mandatory");
CmlStringOption cmlDomain('r',"domain", "domain", "domain to be retrieved (e.g. \"[1000:8000,5000:10000]\"). Mandatory");
CmlStringOption cmlScale('f',"scale", "factor", "scale factor applied to domain. Default 1");
CmlStringOption cmlFile('t',"tiffile", "file-name", "name of TIFF file written. Mandatory");
CmlStringOption cmlPolygon('p',"polygon", "pol-desc","polygon for clipping (e.g. \"[1010,8200] [4000,8200] [3000,9800]\").");
CmlStringOption cmlBgr('b',"background", "bgr-desc", "background of TIFF file (e.g. 0x1f).");
CmlBoolOption   cmlHelp('h',"help","print this message.");



// This is a quick first try at a class for creating TIFFs stripe by stripe. It copies
// a lot of code from class r_Conv_TIFF in conversion/tiff.cc. Ideally this at some
// point should make use of Andreas' conversion framework.

class r_TIFFStripe
{
public:
    r_TIFFStripe(const char* newFileName, const r_Minterval& tiffDom);
    void openTiff(); //open the tiff file
    bool addArray(const r_GMarray& myArray); //write myArray to tiff file
    void closeTiff(); //close the tiff file
    ~r_TIFFStripe();
private:
    char* fileName; //filename of the tiff image
    TIFF* tiffFile; //handler of the tiff image
    uint16 bpp; // bits per pixel (24 for RGB)
    uint16 bps; // bits per sample (8 for RGB)
    unsigned long typeSize; // size of base type, will be retrieved with r_Base_Type::size()
    uint32 width; // image width
    uint32 height; //image height
    char* scanLine; // buffer for scanLine to be written to file
    uint32 tiffRow; // row's no of tiff image
};

r_TIFFStripe::r_TIFFStripe(const char* newFileName, const r_Minterval& tiffDom)
    : bpp(8), bps(8), typeSize(1), tiffRow(0), fileName(NULL), tiffFile(NULL), scanLine(NULL)
{
    fileName = strdup(newFileName);
    width  = (uint32)(tiffDom.get_extent()[0]);
    height = (uint32)(tiffDom.get_extent()[1]);
    // copied this formula from Andreas, do not fully get it. 31 seems to be an internal
    // buffer for tifflib and >> 5 together with uint32 instead of char seems to be used
    // to get the correct number of chars as oppose to bits.

    //allocate the buffer
    scanLine = (char*)mymalloc(width*typeSize);
}

void
r_TIFFStripe::openTiff()
{
    uint16 cmap[256];             // Colour map (for greyscale images)

    tiffFile = TIFFOpen(fileName, "w");

    // These fields are written to the file by TIFFWriteDirectory, which is automatically
    // called by TIFFClose and TIFFFlush

    TIFFSetField(tiffFile, TIFFTAG_ARTIST, "RasDaMan");
    TIFFSetField(tiffFile, TIFFTAG_DOCUMENTNAME, "Image");
    TIFFSetField(tiffFile, TIFFTAG_SOFTWARE, "RasDaMan");
    //TIFFSetField(tiffFile, TIFFTAG_SUBFILETYPE, (uint32)0);
    TIFFSetField(tiffFile, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tiffFile, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tiffFile, TIFFTAG_BITSPERSAMPLE, bps);
    // UNIX doesn't mind which fill-order. NT only understands this one.
    TIFFSetField(tiffFile, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
    // problem: LZW is no longer supported in current versions of libtiff.
    TIFFSetField(tiffFile, TIFFTAG_COMPRESSION, (uint16)COMPRESSION_NONE);
    TIFFSetField(tiffFile, TIFFTAG_ORIENTATION, (uint16)ORIENTATION_TOPLEFT);

    // Format-dependent tags, currently support only 8bit grey images
    TIFFSetField(tiffFile, TIFFTAG_PHOTOMETRIC, (uint16)PHOTOMETRIC_PALETTE);
    TIFFSetField(tiffFile, TIFFTAG_SAMPLESPERPIXEL, (uint16)1);
    TIFFSetField(tiffFile, TIFFTAG_PLANARCONFIG, (uint16)PLANARCONFIG_CONTIG);
    // TIFFSetField(tiffFile, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tiffFile, (uint32)-1));
    TIFFSetField(tiffFile, TIFFTAG_ROWSPERSTRIP, (uint32)1);
    //TIFFSetField(tiffFile, TIFFTAG_MINSAMPLEVALUE, (uint16)0);
    //TIFFSetField(tiffFile, TIFFTAG_MAXSAMPLEVALUE, (uint16)255);
    TIFFSetField(tiffFile, TIFFTAG_RESOLUTIONUNIT, (uint16)RESUNIT_INCH);

    // This will have to be adapted!
    TIFFSetField(tiffFile, TIFFTAG_XRESOLUTION, (float)90.0);
    TIFFSetField(tiffFile, TIFFTAG_YRESOLUTION, (float)90.0);
    TIFFSetField(tiffFile, TIFFTAG_XPOSITION, (float)0.0);
    TIFFSetField(tiffFile, TIFFTAG_YPOSITION, (float)0.0);

    // build the colour-map (greyscale, i.e. all 3 components identical)
    // TIFF needs 16 bit values for this (--> tools/tiffdither.c)
    for (int i=0; i<256; i++) cmap[i] = (uint16)(i*((1L << 16) - 1)/255);
    TIFFSetField(tiffFile, TIFFTAG_COLORMAP, cmap, cmap, cmap);
}

bool
r_TIFFStripe::addArray(const r_GMarray& myArray)
{
    // here we write the r_GMarray we get line by line into the TIFF. We
    // assume that the width of myArray (width = [0] r_Sinterval)
    // corresponds to the width of the whole TIFF.

    unsigned long imageHeight = myArray.spatial_domain().get_extent()[1];
    unsigned long imageWidth = myArray.spatial_domain().get_extent()[0];

    if(imageWidth != width)
    {
        cout << "Error Tiff file \"" << fileName << "\" initialised for "
             << width << ", not for " << imageWidth << " !" << endl;
        return false;
    }

    const char* linePtr = myArray.get_array(); // points to  myArray's data
    char* scanLinePtr = (char*)scanLine;


    for (int row = 0; row < imageHeight; row++)
    {
        // copy data (and transpose)
        for (int col=0; col < imageWidth; col++)
        {
            for(int i=0; i<typeSize; i++)
            {
                *scanLinePtr++ = *(linePtr + col*imageHeight*typeSize + row*typeSize + i);
            }
            // not really fast and does not work for all types! Good enough for now.
        }
        if(TIFFWriteScanline(tiffFile, (tdata_t)scanLine, tiffRow++, 0) < 0)
        {
            // error
            cerr << "Error writing line into file " << fileName << "." << endl;
            return false;
        }
        memset(scanLine, 0, imageWidth*typeSize);
        scanLinePtr = (char*)scanLine;
    }

    return true;
}

void
r_TIFFStripe::closeTiff()
{
    TIFFClose(tiffFile);
}

r_TIFFStripe::~r_TIFFStripe()
{
    if(fileName)
        free(fileName);
    if(scanLine)
        free(scanLine);
}

void
printUsage(char* name)
{
    static const char* ExColl="lva";
    static const char* ExDomain="[0:100,0:100]";
    static const char* ExFile="lva.tif";

    cout << name << " v1.0 - RasDaMan Export Utility for 2D marrays" << endl;
    cout << "Description: Exports data with background, from RasDaMan database from a fastscale collection" << endl;
    cout << "             for a domain with a scale." << endl;
    cout << "             Returns " << SUCCES << " for succes, otherwise " << FAILED << endl;
    cout << "Notes: If a polygon is specified, the result is a image which has data inside the polygon and"<< endl;
    cout << "       the rest is filled with background color." << endl;

    cout << "Usage options:" << endl;
    cmlInter.printHelp();

    cout << "For example:"   << endl;
    cout << name << "\t"   << cmlSrv.getLongFormTag()    << " " << DefaultSrv    << " ";
    cout               << cmlPort.getLongFormTag()   << " " << DefaultPort   << " ";
    cout               << cmlDb.getLongFormTag()     << " " << DefaultDb     << " ";
    cout               << cmlColl.getLongFormTag()   << " " << ExColl      << " ";
    cout << endl << "\t\t" << cmlDomain.getLongFormTag() << " " << ExDomain        << " ";
    cout               << cmlScale.getLongFormTag()  << " " << DefaultScale  << " ";
    cout               << cmlFile.getLongFormTag()   << " " << ExFile          << " ";
    cout << endl << "\t\t" << cmlUsr.getLongFormTag()    << " " << DefaultUsr    << " ";
    cout               << cmlPasswd.getLongFormTag() << " " << DefaultPasswd << " ";
    cout << endl;

    cout << "Report bugs to <support@active­knowledge.com>" << endl;
}

void
printStatus(char* name)
{
    cout << name << "'s parameters list:" << endl;
    cmlInter.printStatus();
}

void
defineParams()
{
    cmlInter.defineOption(&cmlSrv);
    cmlInter.defineOption(&cmlPort);
    cmlInter.defineOption(&cmlDb);
    cmlInter.defineOption(&cmlUsr);
    cmlInter.defineOption(&cmlPasswd);
    cmlInter.defineOption(&cmlColl);
    cmlInter.defineOption(&cmlDomain);
    cmlInter.defineOption(&cmlScale);
    cmlInter.defineOption(&cmlFile);
    cmlInter.defineOption(&cmlPolygon);
    cmlInter.defineOption(&cmlBgr);
    cmlInter.defineOption(&cmlHelp);
}

void
getParams()
{
    CmlOption* ptr=NULL;

    ptr=cmlInter.getOption(&cmlSrv);
    cmlSrv=*((CmlStringOption*)ptr);

    ptr=cmlInter.getOption(&cmlPort);
    cmlPort=*((CmlStringOption*)ptr);

    ptr=cmlInter.getOption(&cmlDb);
    cmlDb=*((CmlStringOption*)ptr);

    ptr=cmlInter.getOption(&cmlUsr);
    cmlUsr=*((CmlStringOption*)ptr);

    ptr=cmlInter.getOption(&cmlPasswd);
    cmlPasswd=*((CmlStringOption*)ptr);

    ptr=cmlInter.getOption(&cmlColl);
    cmlColl=*((CmlStringOption*)ptr);

    ptr=cmlInter.getOption(&cmlDomain);
    cmlDomain=*((CmlStringOption*)ptr);

    ptr=cmlInter.getOption(&cmlScale);
    cmlScale=*((CmlStringOption*)ptr);

    ptr=cmlInter.getOption(&cmlFile);
    cmlFile=*((CmlStringOption*)ptr);

    ptr=cmlInter.getOption(&cmlPolygon);
    cmlPolygon=*((CmlStringOption*)ptr);

    ptr=cmlInter.getOption(&cmlBgr);
    cmlBgr=*((CmlStringOption*)ptr);

    ptr=cmlInter.getOption(&cmlHelp);
    cmlHelp=*((CmlBoolOption*)ptr);

    ptr=NULL;
}

bool
parseParams(int argc, char** argv)
{
    try
    {
        defineParams();
        cmlInter.interpretArguments(argc, argv);
        getParams();
    }
    catch(CmlError& err)
    {
        cout << "Command Line Parsing Error:" << endl << err.getText() << endl;
        return false;
    }

//check rule
    if(cmlColl.isPresent()   &&
            cmlDomain.isPresent() &&
            cmlFile.isPresent()
      )
        return true;
    else
    {
        cout << "Error some of mandatory arguments are missing!" << endl;
        cout << "Please check your command line!" << endl;
        return false;
    }
}

bool
fillPolygon(r_Polygon& myPoly)
{

    char* startPos = (char*)cmlPolygon.getString();
    char* endPos=NULL;
    int pointStrLen=0, pointNo=0;
    char currPoint[MYSTRINGSIZE];
    r_Point myPoint;


    //cout << "start decoding polygon string"<< endl;

    while(true)
    {
        strcpy(currPoint, "");
        startPos = index(startPos, '[');
        if(!startPos)
        {
            // Did not find a closing [, that's it.
            break;
        }

        endPos = index(startPos, ']');
        if(!endPos)
        {
            // Did not find a closing ], that's it.
            break;
        }

        // try to add point
        pointStrLen = endPos - startPos + 1;
        strncpy(currPoint, startPos, pointStrLen);
        currPoint[pointStrLen+1] = '\0';

        try
        {
            myPoint=r_Point(currPoint);
            myPoly.addPoint( myPoint );
            pointNo++;
            //cout << "-point " << pointNo << " : " << currPoint << endl;
        }
        catch(r_Error& err)
        {
            cout << "Error decoding point \"" << currPoint << "\" from polygon string \""
                 << cmlPolygon.getString() << " !" << endl;
            return false;
        }
        startPos += pointStrLen;
    }

    if(!pointNo)
    {
        cout << "Error no points found, while decoding polygon string \"" << cmlPolygon.getString() << "\" !" << endl;
        return false;
    }

//cout << "end decoding"<< endl;

    myPoly.close();

    return true;
}

bool
parseBgr()
{
    static char hexval[]= { 0x00, 0x01, 0x02, 0x03,
                            0x04, 0x05, 0x06, 0x07,
                            0x08, 0x09, 0x0a, 0x0b,
                            0x0c, 0x0d, 0x0e, 0x0f,
                          };
    static char hexfig[]= { '0', '1', '2', '3',
                            '4', '5', '6', '7',
                            '8', '9', 'a', 'b',
                            'c', 'd', 'e', 'f',
                            '\0'
                          };

    static r_Bytes sizeBgrMin=2, sizeHexFig=strlen(hexfig);
    char* pBgr=(char*)cmlBgr.getString();
    r_Bytes sizeBgr=strlen(pBgr), sizerBgr=0, indexFigure=0, indexHexFig=0;

    if(sizeBgr <= sizeBgrMin)
    {
        cout << "Error decoding background \"" << pBgr << "\", is not a hex string !" << endl;
        return false;
    }

    if (pBgr[0] !='0' || (pBgr[1] != 'x' && pBgr[1]!='X'))
    {
        cout << "Error decoding background \"" << pBgr << "\", 0x or 0X is missing !" << endl;
        return false;
    }

    if(sizeBgr%sizeBgrMin)
    {
        sizerBgr=(sizeBgr-sizeBgrMin+1)/sizeBgrMin;
    }
    else
        sizerBgr=(sizeBgr-sizeBgrMin)/sizeBgrMin;

    rBgr=new char [sizeBgr+1];
    memset(rBgr,'\0', sizeBgr+1);

//skip tag 0x/0X
    pBgr+=sizeBgrMin;

    while (*pBgr)
    {
        //check figure
        indexHexFig=0;
        while(indexHexFig<sizeHexFig)
        {
            if (tolower(*pBgr) == hexfig[indexHexFig])
                break;
            indexHexFig++;
        }

        if(indexHexFig>=sizeHexFig)
        {
            cout << "Error decoding background \"" << pBgr << "\", \"" << *pBgr << "\" is no a hex value !" << endl;
            delete[] rBgr;
            return false;
        }

        //set value figure
        if(indexFigure%sizeBgrMin)
        {
#if defined(LITTLE_ENDIAN)
            rBgr[indexFigure/sizeBgrMin]=rBgr[indexFigure/sizeBgrMin] * 16 + hexval[indexHexFig];
#else
            rBgr[indexFigure/sizeBgrMin]=rBgr[indexFigure/sizeBgrMin]+ hexval[indexHexFig] * 16;
#endif
        }
        else
            rBgr[indexFigure/sizeBgrMin]=hexval[indexHexFig];

        //advance
        pBgr++;
        indexFigure++;
    }

    return true;
}


bool
detectParams()
{
    r_Long rPort=0;

// server name
    if(!cmlSrv.isPresent())
        cmlSrv.setValue(DefaultSrv);

// server port
    if(!cmlPort.isPresent())
        cmlPort.setValue(DefaultPort);
    else
    {
        try
        {
            rPort=cmlPort.getLong();
        }
        catch(CmlError& err)
        {
            cout << "Error decoding " << cmlPort.getLongForm() << " \"" << cmlPort.getString()
                 << "\" isn't a number!" << endl;
            return false;
        }
        if (rPort <= 0.)
        {
            cout << "Error decoding " << cmlPort.getLongForm() << " \"" << cmlPort.getString()
                 << "\" is negative or zero !" << endl;
            return false;
        }
    }

// database name
    if(!cmlDb.isPresent())
        cmlDb.setValue(DefaultDb);

// user name
    if(!cmlUsr.isPresent())
        cmlUsr.setValue(DefaultUsr);

// user passord
    if(!cmlPasswd.isPresent())
        cmlPasswd.setValue(DefaultPasswd);

// scale factor
    if(!cmlScale.isPresent())
        rScale=DefaultScale;
    else
    {
        try
        {
            rScale=cmlScale.getDouble();
        }
        catch(CmlError& err)
        {
            cout << "Error decoding " << cmlScale.getLongForm() << " \"" << cmlScale.getString()
                 << "\" isn't a number!" << endl;
            return false;
        }
        if (rScale <= 0.)
        {
            cout << "Error decoding " << cmlScale.getLongForm() << " \"" << cmlScale.getString()
                 << "\" is negative or zero !" << endl;
            return false;
        }
    }

// collection name
// nothing to check here

// collection domain
    try
    {
        r_Minterval test(cmlDomain.getString());
    }
    catch(r_Error& err)
    {
        cout << "Error while decoding " << cmlDomain.getLongForm() << " \"" << cmlDomain.getString() << "\" !" << endl;
        cout << "Error " << err.get_errorno() << " : " << err.what() << endl;
        return false;
    }

// filename
//nothing to be checked here

// polygon
    if(cmlPolygon.isPresent())
    {
        if(!fillPolygon(poly))
            return false;

        //FIXME
        //is convex?
        if(poly.detectPolygonType()!=r_Polygon::CONVEX)
        {
            cout << "We support only simple convex polygon now. Please check your polygon !" << endl;
            return false;
        }
    }

// background
    if(cmlBgr.isPresent())
        return parseBgr();

    return true;
}



bool
exportData()
{

    // At the moment we do just a quick check for the TIFF writing using r_Conversion.
    // Now we do a query and store the result as a TIFF using r_Conversion.

    // Ok, the whole TIFF conversion thing is done below. Now let's get an
    // array from RasDaMan here.
    r_Ref<r_GMarray> mddObj;
    r_Fast_Base_Scale *scaler=NULL;
    r_Minterval trimDom(cmlDomain.getString());
    r_Minterval tiffDom, currDom, insertionUnit(2), clipDom(2);
    r_Minterval collDom;
    r_Point trimExtent;
    r_Polygon myPoly;

    // 5 MB buffer for spooling images
    r_ULong bufSize = 5 * 1024 * 1024;
    //size of base type
    r_Bytes typeSize = 1;
    // no of scan lines in buffer
    r_ULong numScanLineInBuf=0;
    //tiff strip object
    r_TIFFStripe* myTiff=NULL;

    try
    {
        db.set_servername(cmlSrv.getString(), cmlPort.getLong());
        db.set_useridentification(cmlUsr.getString(), cmlPasswd.getString());
        db.open(cmlDb.getString());

        try
        {
            ta.begin( r_Transaction::read_only );
            scaler = new r_Fast_Scale<r_Char>(cmlColl.getString());
            collDom = scaler->get_full_domain();
        }
        catch( r_Error& errorObj )
        {
            cout << "Error while initializing collection "  << cmlColl.getString() << " !" << endl;
            cout << "Error " << errorObj.get_errorno() << " : "  << errorObj.what() << endl;
            ta.abort();
            db.close();
            if(rBgr)
            {
                delete[] rBgr;
                rBgr=NULL;
            }
            return false;
        }

        if (!collDom.covers(trimDom))
        {
            cout << "Error, the requested domain " << trimDom << " is not in the collection domain " << collDom << " !" <<  endl;

            if(scaler)
            {
                delete scaler;
                scaler=NULL;
            }

            ta.abort();
            db.close();

            if(rBgr)
            {
                delete[] rBgr;
                rBgr=NULL;
            }

            return false;
        }

        // Ok, now here we split the domain in stripes. We take a roughly 5 MB buffer. It
        // is calculated using the original domain of the area to be retrieved and scale^2
        // to calculate the memory used without calculating the domains back and forth.

        insertionUnit[0] = trimDom[0]; // we take the whole width of the image.

        trimExtent=trimDom.get_extent();
        numScanLineInBuf = bufSize / ( typeSize * trimExtent[0] * rScale * rScale);
        numScanLineInBuf = numScanLineInBuf > trimExtent[1] ? trimExtent[1] : numScanLineInBuf;

        if(numScanLineInBuf < 1) //if numScanLine is 0 as result of division
            numScanLineInBuf=1;

        insertionUnit[1] = r_Sinterval((r_Range)0, (r_Range)(numScanLineInBuf - 1));

        try
        {
            scaler->get_scaled_domain(trimDom, tiffDom, rScale);
        }
        catch( r_Error& errorObj )
        {
            cout << "Error while getting the scaled domain !" << endl;
            cout << "Error " << errorObj.get_errorno() << " : "  << errorObj.what() << endl;
            if(scaler)
            {
                delete scaler;
                scaler=NULL;
            }

            ta.abort();
            db.close();

            if(rBgr)
            {
                delete[] rBgr;
                rBgr=NULL;
            }
            return false;
        }


        if(cmlPolygon.isPresent())
        {
            // we scale the polygon according to the scale factor
            cout << "Defined Polygon: " << poly << endl;
            poly.scale(collDom.get_origin(), rScale);
            cout << "Scaled Polygon: " << poly << endl;
        }

        myTiff = new r_TIFFStripe(cmlFile.getString(), tiffDom);
        myTiff->openTiff();

        cout << "Retrieving area " << trimDom << " of object " << cmlColl.getString()
             << " with scale factor " << rScale << "." << endl;
        cout << "Retrieving units of shape " << insertionUnit << "." << endl;
        cout << "Tiff Image area " << tiffDom << endl;

        r_MiterArea myIter(&insertionUnit, &trimDom);
        while( !myIter.isDone() )
        {
            currDom = myIter.nextArea();
            cout << "  Getting " << currDom << " with scale factor " << rScale << "." << endl;
            try
            {
                mddObj = scaler->get_scaled_object(currDom, rScale, 1);
            }
            catch( r_Error& errorObj )
            {
                cout << "Error while getting the scaled domain !" << endl;
                cout << "Error " << errorObj.get_errorno() << " : "  << errorObj.what() << endl;
                if(myTiff)
                {
                    myTiff->closeTiff();
                    delete myTiff;
                    myTiff=NULL;
                }
                if(scaler)
                {
                    delete scaler;
                    scaler=NULL;
                }

                ta.abort();
                db.close();

                if(rBgr)
                {
                    delete[] rBgr;
                    rBgr=NULL;
                }
                return false;
            }

            // The polygonal cutout is currently not done. We will need a scale function with the
            // same semantics as fastscale for it to work. Then the polygon can be specified in
            // original pixel coordinates and will be scaled according to the scaling factor.

            if(cmlPolygon.isPresent())
            {
                // we clip the polygon it according to the scaled domain retrieved by r_Fast_Scale

                // original polygon is modified by clip function
                myPoly=poly;

                scaler->get_scaled_domain(currDom, clipDom, rScale);
                cout << "Domain for clipping: " << clipDom << endl;

                //FIXME
                //quick hack for test_polygon because get_scaled_image
                //returns the clipDom translated in currDom.get_origin()
                mddObj->set_spatial_domain(clipDom);

                myPoly.clip(clipDom);
                cout << "Clipped polygon: " << myPoly << endl;


                // we would have to transpose it here. This is done by the TIFF conversion.

                // REALLY IMPORTANT: The TA has to be open here! Otherwise neither
                // type info nor domain can be read.
                try
                {
                    myPoly.fillMArray(*mddObj, false, rBgr);
                }
                catch(r_Error& err)
                {
                    cout << "Error size of background " << strlen(rBgr) << " bytes "
                         << " is different from mdd base type size " <<  mddObj->get_type_length()
                         << " bytes !"  << endl;
                    mddObj.destroy();
                    throw err;
                }
            }//end if(usePolygon)

            // Ok, now we use our r_TIFFStripe class here
            if(!myTiff->addArray(*mddObj))
            {
                if(myTiff)
                {
                    myTiff->closeTiff();
                    delete myTiff;
                    myTiff=NULL;
                }

                if(rBgr)
                {
                    delete[] rBgr;
                    rBgr=NULL;
                }

                if(scaler)
                {
                    delete scaler;
                    scaler=NULL;
                }

                mddObj.destroy();

                ta.abort();
                db.close();

                return false;
            }
            mddObj.destroy();
        }//end while

        delete scaler;
        scaler=NULL;

        myTiff->closeTiff();
        delete myTiff;
        myTiff=NULL;

        delete[] rBgr;
        rBgr=NULL;

        ta.commit();
        db.close();
    }
    catch(r_Error & errObj)
    {
        cout << "Error while exporting the data !" << endl;
        cout << "Error " << errObj.get_errorno() << " : "  << errObj.what() << endl;
        if(scaler)
        {
            delete scaler;
            scaler=NULL;
        }

        ta.abort();
        db.close();

        if(rBgr)
        {
            delete[] rBgr;
            rBgr = NULL;
        }
        if(myTiff)
        {
            myTiff->closeTiff();
            delete myTiff;
            myTiff = NULL;
        }
        return false;
    }

    return true;
}

bool
processRequest(char* name)
{
    bool result=false;

    if(detectParams())
    {
#if defined(RMANDEBUG)
        printStatus(name);
#endif
        result=exportData();
    }

    return result;
}


int
main(int argc, char *argv[])
{
    int result=FAILED;

    if(!parseParams(argc, argv))
        printUsage(argv[0]);
    else if(cmlHelp.isPresent())
    {
        printUsage(argv[0]);
        result=SUCCES;
    }
    else if(processRequest(argv[0]))
        result=SUCCES;
    else
        printUsage(argv[0]);
    return result;

}

