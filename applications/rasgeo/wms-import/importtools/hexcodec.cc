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

#include <cctype>
#include <sstream>

#include "hexcodec.hh"

// debug facility; relies on -DDEBUG at compile time
#include "debug/debug.hh"

using std::ostringstream;
using std::ios;
using std::endl;
using std::hex;

char
HexCodec::hexVal[] = { 0x00, 0x01, 0x02, 0x03,
                       0x04, 0x05, 0x06, 0x07,
                       0x08, 0x09, 0x0a, 0x0b,
                       0x0c, 0x0d, 0x0e, 0x0f,
                     };
const string
HexCodec::hexFig = "0123456789abcdef";

const string
HexCodec::hexId = "0x";

const string
HexCodec::emptyStr = "";

const char
HexCodec::hexBase = 16;

const char
HexCodec::hexPerByte = 2;

string
HexCodec::hexStr;

string
HexCodec::figStr;

string
HexCodec::convertTo(const string& figureStr) throw(ImportError)
{
    string errMsg;
    string::size_type sizeHexStr = 0, sizeFigStr = 0;
    string::size_type sizeHexId = 0, idxFig = 0, idxHexFig = 0;
    string::size_type idxHex=0;

    figStr=figureStr;
    hexStr=emptyStr;
    sizeFigStr=figStr.size();
    sizeHexId=hexId.size();

    if(sizeFigStr <= sizeHexId)
    {
        TALK( "HexCodec::ConvertTo(\"" + figStr + "\"), user input is not a hexadecimal string." );
        throw ImportError( INVALIDHEX );
    }

    if ((tolower(figStr[0]) != hexId[0]) ||
            (tolower(figStr[1]) != hexId[1]))
    {
        TALK( "HexCodec::ConverTo(\"" + figStr + "\"), \"" + hexId +"\" is missing for user input." );
        throw ImportError( INVALIDHEX );
    }

    sizeHexStr = (sizeFigStr - sizeHexId) / hexPerByte;
    if(sizeFigStr % hexPerByte)
        sizeHexStr++;

    hexStr.resize(sizeHexStr, '\0');

    //skip hexId tag
    idxFig+=sizeHexId;

    while (idxFig < sizeFigStr)
    {
        //check figure
        idxHexFig=hexFig.find(tolower(figStr[idxFig]));
        if(idxHexFig == string::npos)
        {
            TALK( "HexCodec::ConvertTo(\"" + figStr + "\"), \"" + figStr[idxFig] + "\" from user input is not a hexadecimal figure.") ;
            throw ImportError( INVALIDHEX );
        }

        //set value figure
        if(idxFig % hexPerByte)
        {
#if defined(LITTLE_ENDIAN)
            hexStr[idxHex] = hexStr[idxHex] * hexBase + hexVal[idxHexFig];
#else
            hexStr[idxHex] = hexStr[idxHex] + hexVal[idxHexFig] * hexBase;
#endif
            idxHex++;
        }
        else
            hexStr[idxHex] = hexVal[idxHexFig];

        //advance
        idxFig++;
    }

    return hexStr;
}

string
HexCodec::convertFrom(const string& hexaStr) throw(ImportError)
{
    string::size_type sizeHexStr = 0, idxHexStr = 0;
    string::size_type sizeFigStr = 0, idxFigStr = 0;

    hexStr = hexaStr;
    figStr = emptyStr;
    sizeHexStr = hexStr.size();
    sizeFigStr = sizeHexStr * hexPerByte;
    figStr.resize(sizeFigStr, '\0');

    for(idxHexStr=0, idxFigStr=0;
            idxHexStr < sizeHexStr;
            idxHexStr++)
    {
#if defined(LITTLE_ENDIAN)
        figStr[idxFigStr++] = hexFig[hexStr[idxHexStr] / hexBase];
        figStr[idxFigStr++] = hexFig[hexStr[idxHexStr] % hexBase];
#else
        figStr[idxFigStr++] = hexFig[hexStr[idxHexStr] % hexBase];
        figStr[idxFigStr++] = hexFig[hexStr[idxHexStr] / hexBase];
#endif
    }

    figStr = hexId + figStr;

    return figStr;
}

void
HexCodec::printStatus(ostream& s)
{
    string::size_type i=0,n=0;
    s << "FigureStr: '" << figStr << "' size " << figStr.size() << endl;
    s << "HexaStr: '" << convertFrom(hexStr) << "' size " << hexStr.size() <<  endl;
}

