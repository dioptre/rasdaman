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

#ifndef __HEXCODEC_HH__
#define __HEXCODEC_HH__

#include <string>
#include <iosfwd>

using std::string;
using std::ostream;

#include "mymalloc/mymalloc.h"
#include "import_error.hh"

class HexCodec
{
public:
    static string convertTo(const string& figureStr) throw(ImportError);
    static string convertFrom(const string& hexaStr) throw(ImportError);
    void printStatus(ostream& s);

private:
    //class members
    static char hexVal[16];
    static const string hexFig;
    static const string hexId;
    static const string emptyStr;
    static const char   hexBase;
    static const char   hexPerByte;
    static string hexStr;
    static string figStr;

};

#endif
