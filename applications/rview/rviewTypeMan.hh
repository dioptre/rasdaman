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
 *  COMMENTS:
 *      None
 */

/**
*   @file rviewTypeMan.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_TYPE_MAN_H_
#define _RVIEW_TYPE_MAN_H_


#ifdef __GNUG__
#pragma interface
#endif


#include "rviewUtils.hh"
#include "rviewDisplay.hh"



class rviewTypeMan: public rviewFrame
{
public:

    rviewTypeMan(rviewFrame *parentWindow);
    rviewTypeMan(rviewFrame *parentWindow, const r_Type *type);
    ~rviewTypeMan(void);

    void unlinkParent(void);

    void setType(const r_Type *type);
    void label(void);
    int  process(wxObject &obj, wxEvent &evt);
    void OnSize(int w, int h);
    int convert(r_Ref<r_GMarray> &src, r_Ref<r_GMarray> &dest);


protected:

    void clearData(void);

    wxPanel *panel;
    wxGroupBox **structures;
    rviewCheckBox **members;
    unsigned int *offsets;
    unsigned char *primtypes;
    unsigned int numStruct, numMembers, typeDepth;
    unsigned int baseTypeLength;
    r_Type *myType;
    rviewFrame *parent;
    rviewButton *closeBut, *convertBut;
    const char **typeNames;
    DynamicString baseTypeName;


private:

    void initShare(rviewFrame *parentWindow);
    void parseStructType(const r_Structure_Type *tp, unsigned int &nums, unsigned int &numm, unsigned int depth, unsigned int offset=0, wxRect *bbox=NULL);
    void parsePrimitiveType(const r_Primitive_Type *tp, const char *name, unsigned int &numm, unsigned int offset=0, wxRect *bbox=NULL);

    static const char structName[];
    static const char marrayName[];
    static const char typeBool[];
    static const char typeChar[];
    static const char typeOctet[];
    static const char typeShort[];
    static const char typeUShort[];
    static const char typeLong[];
    static const char typeULong[];
    static const char typeFloat[];
    static const char typeDouble[];

    // constants
    static const int tman_width;
    static const int tman_height;
    static const int tman_border;
    static const int tman_basewidth;
    static const int tman_cheight;
    static const int tman_bheight;
    static const int tman_bwidth;
};

#endif
