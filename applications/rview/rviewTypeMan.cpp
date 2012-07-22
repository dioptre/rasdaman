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

// Standard wxWindows preamble.
#ifdef __GNUG__
#pragma implementation
#endif


// changed in wxWindows 2.4.2:
//#include "wx_prec.h"
#include <wx/wxprec.h>


#ifdef __BORLANDC__
#pragma hdrstop
#endif


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif




#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "rviewTypeMan.hh"
#include "raslib/primitivetype.hh"
#include "raslib/structuretype.hh"


// use extremely large default values, otherwise the window doesn't open
// correctly on Windows.
const int rviewTypeMan::tman_width = 1000;
const int rviewTypeMan::tman_height = 2000;
const int rviewTypeMan::tman_border = 8;
const int rviewTypeMan::tman_basewidth = 100;
const int rviewTypeMan::tman_cheight = 30;
const int rviewTypeMan::tman_bheight = 30;
const int rviewTypeMan::tman_bwidth = 50;

// Type keywords
const char rviewTypeMan::structName[] = "struct";
const char rviewTypeMan::marrayName[] = "marray";

// String constants for type names
const char rviewTypeMan::typeBool[] = "bool";
const char rviewTypeMan::typeChar[] = "char";
const char rviewTypeMan::typeOctet[] = "octet";
const char rviewTypeMan::typeShort[] = "short";
const char rviewTypeMan::typeUShort[] = "ushort";
const char rviewTypeMan::typeLong[] = "long";
const char rviewTypeMan::typeULong[] = "ulong";
const char rviewTypeMan::typeFloat[] = "float";
const char rviewTypeMan::typeDouble[] = "double";



void rviewTypeMan::initShare(rviewFrame *parentWindow)
{
    numStruct = 0;
    numMembers = 0;
    typeDepth = 0;
    structures = NULL;
    members = NULL;
    offsets = NULL;
    primtypes = NULL;
    typeNames = NULL;
    myType = NULL;
    panel = NULL;
    parent = parentWindow;
}


rviewTypeMan::rviewTypeMan(rviewFrame *parentWindow) : rviewFrame(NULL, lman->lookup("titleTypeMan"), 0, 0, tman_width, tman_height)
{
    initShare(parentWindow);
}


rviewTypeMan::rviewTypeMan(rviewFrame *parentWindow, const r_Type *type) : rviewFrame(NULL, lman->lookup("titleTypeMan"), 0, 0, tman_width, tman_height)
{
    initShare(parentWindow);
    setType(type);
}


rviewTypeMan::~rviewTypeMan(void)
{
    user_event usr;

    usr.type = usr_child_closed;
    usr.data = (void*)this;
    if (parent != NULL) parent->userEvent(usr);

    clearData();
}


void rviewTypeMan::unlinkParent(void)
{
    parent = NULL;
}


void rviewTypeMan::clearData(void)
{
    if (structures != NULL)
    {
        delete [] structures;
        structures = NULL;
    }
    if (members != NULL)
    {
        delete [] members;
        members = NULL;
    }
    if (offsets != NULL)
    {
        delete [] offsets;
        offsets = NULL;
    }
    if (primtypes != NULL)
    {
        delete [] primtypes;
        primtypes = NULL;
    }
    if (typeNames != NULL)
    {
        delete [] typeNames;
        typeNames = NULL;
    }

    if (myType != NULL)
    {
        delete myType;
        myType = NULL;
    }

    if (panel != NULL)
    {
        delete panel;
        panel = NULL;
    }

    initShare(parent);
}


// The name can't be read from cloned r_Types (not copied). Bug?
void rviewTypeMan::parsePrimitiveType(const r_Primitive_Type *tp, const char *name, unsigned int &numm, unsigned int offset, wxRect *bbox)
{
    if (members != NULL)
    {
        char buffer[STRINGSIZE];
        const char *tname = NULL;

        switch (tp->type_id())
        {
        case r_Primitive_Type::BOOL:
            tname = typeBool;
            break;
        case r_Primitive_Type::CHAR:
            tname = typeChar;
            break;
        case r_Primitive_Type::OCTET:
            tname = typeOctet;
            break;
        case r_Primitive_Type::SHORT:
            tname = typeShort;
            break;
        case r_Primitive_Type::USHORT:
            tname = typeUShort;
            break;
        case r_Primitive_Type::LONG:
            tname = typeLong;
            break;
        case r_Primitive_Type::ULONG:
            tname = typeULong;
            break;
        case r_Primitive_Type::FLOAT:
            tname = typeFloat;
            break;
        case r_Primitive_Type::DOUBLE:
            tname = typeDouble;
            break;
        default:
            break;
        }
        typeNames[numm] = tname;
        sprintf(buffer, "%s (%s)", name, (tname == NULL) ? "" : tname);
        members[numm]->SetLabel(buffer);
        members[numm]->SetSize(bbox->x, bbox->y, bbox->width, bbox->height);
        offsets[numm] = offset;
        primtypes[numm] = tp->type_id();
    }
    numm++;
}


void rviewTypeMan::parseStructType(const r_Structure_Type *tp, unsigned int &nums, unsigned int &numm, unsigned int depth, unsigned int offset, wxRect *bbox)
{
    wxRect pos;
    unsigned int thisStruct = nums++;

    if (bbox != NULL)
    {
        pos.x = bbox->x + tman_border;
        pos.width = bbox->width - 2*tman_border;
        pos.y = bbox->y + tman_border;
        pos.height = tman_cheight;
    }

    if (depth > typeDepth) typeDepth = depth;

    r_Structure_Type::attribute_iterator iter(tp->defines_attribute_begin());
    while (iter != tp->defines_attribute_end())
    {
        r_Type *newType;
        unsigned int off = offset + (*iter).offset();

        //(*iter).print_status(cout); cout << " --- " << (*iter).name() << endl;
        newType = (*iter).type_of().clone();
        if (newType->isStructType())
        {
            parseStructType((const r_Structure_Type*)newType, nums, numm, depth+1, off, &pos);
        }
        else
        {
            parsePrimitiveType((const r_Primitive_Type*)newType, (*iter).name(), numm, off, &pos);
            pos.y += pos.height;
        }
        delete newType;
        iter++;
    }
    if (structures != NULL)
    {
        structures[thisStruct]->SetSize(bbox->x, bbox->y, bbox->width, pos.y - bbox->y + tman_border/2);
        bbox->y = pos.y + tman_border;
    }
}


void rviewTypeMan::setType(const r_Type *type)
{
    unsigned int nums, numm, i;
    wxRect bbox;

    clearData();
    panel = new wxPanel(this, -1, -1, -1, -1);
    myType = type->clone();

    if (myType->isStructType())
        parseStructType((r_Structure_Type*)type, numStruct, numMembers, 1);
    else
        parsePrimitiveType((r_Primitive_Type*)type, "", numMembers);

    if (numStruct != 0)
    {
        structures = new wxGroupBox*[numStruct];
        for (i=0; i<numStruct; i++) structures[i] = new wxGroupBox(panel, "", -1, -1, -1, -1);
    }
    members = new rviewCheckBox*[numMembers];
    for (i=0; i<numMembers; i++)
    {
        members[i] = new rviewCheckBox(panel);
    }
    offsets = new unsigned int[numMembers];
    primtypes = new unsigned char[numMembers];
    typeNames = new const char*[numMembers];
    baseTypeLength = ((r_Base_Type*)myType)->size();

    nums = 0;
    numm = 0;
    bbox.x = tman_border;
    bbox.y = tman_border;
    bbox.width = tman_basewidth + 2*tman_border * typeDepth;
    bbox.height = numMembers * tman_cheight + 2*tman_border * numStruct;
    if (myType->isStructType())
        parseStructType((r_Structure_Type*)type, nums, numm, 1, 0, &bbox);
    else
        parsePrimitiveType((r_Primitive_Type*)type, "", numm, 0, &bbox);

    closeBut = new rviewButton(panel);
    convertBut = new rviewButton(panel);

    label();

    bbox.width += 2*tman_border;
    bbox.height += 2*tman_border;
    closeBut->SetSize(tman_border, bbox.height, tman_bwidth, tman_bheight);
    convertBut->SetSize(bbox.width - tman_border - tman_bwidth, bbox.height, tman_bwidth, tman_bheight);
    bbox.height += tman_bheight + tman_border + rview_window_extra_height;

    SetSize(0, 0, bbox.width, bbox.height);
    panel->SetSize(0, 0, bbox.width, bbox.height);

    //for (i=0; i<numMembers; i++) cout << (int)(offsets[i]) << ' '; cout << endl;

    Show(TRUE);
}


void rviewTypeMan::label(void)
{
    SetTitle(lman->lookup("titleTypeMan"));
    closeBut->SetLabel(lman->lookup("textClose"));
    convertBut->SetLabel(lman->lookup("textConvert"));
}


int rviewTypeMan::process(wxObject &obj, wxEvent &evt)
{
    int type = evt.GetEventType();

    if (type == wxEVENT_TYPE_BUTTON_COMMAND)
    {
        if (&obj == (wxObject*)closeBut)
        {
            Close(TRUE);
            return 1;
        }
        else if (&obj == (wxObject*)convertBut)
        {
            user_event usr;

            usr.type = usr_typeman_convert;
            usr.data = (void*)this;
            if (parent != NULL) parent->userEvent(usr);
            return 1;
        }
    }
    return 0;
}


void rviewTypeMan::OnSize(int w, int h)
{
    if (panel != NULL)
    {
        panel->SetSize(0, 0, w, h);
    }
}


/*
 *  Warning: this is in fact quit low-level. Assumes alignment -- handle with care!
 */
int rviewTypeMan::convert(r_Ref<r_GMarray> &src, r_Ref<r_GMarray> &dest)
{
    unsigned int i, newMembers=0;

    //cout << "rviewTypeMan::convert()" << endl;

    for (i=0; i<numMembers; i++)
    {
        if (members[i]->GetValue()) newMembers++;
    }
    if (newMembers == 0)
    {
        cerr << "No types selected; ignored." << endl;
        return -1;
    }
    const r_Type *srcBaseType = src->get_base_type_schema();
    if (srcBaseType == NULL)
    {
        cerr << "No base type information available for object; ignored." << endl;
        return -1;
    }
    if (((r_Base_Type*)srcBaseType)->size() != baseTypeLength)
    {
        cerr << "Base type incompatible; ignored." << endl;
        return -1;
    }

    unsigned int *newOff = new unsigned int[newMembers];
    unsigned int *srcIndex = new unsigned int[newMembers];
    unsigned int j;

    for (i=0, j=0; i<numMembers; i++)
    {
        if (members[i]->GetValue()) srcIndex[j++] = i;
    }
    //cout << "src index OK" << endl;

    unsigned int off;
    int needsAlign=1;

    for (i=0, off=0; i<newMembers; i++)
    {
        int increment = 1;

        // align certain types
        switch (primtypes[srcIndex[i]])
        {
        case r_Primitive_Type::SHORT:
        case r_Primitive_Type::USHORT:
            off = (off+1) & ~1;
            increment = 2;
            if (needsAlign < 2) needsAlign = 2;
            break;
        case r_Primitive_Type::LONG:
        case r_Primitive_Type::ULONG:
        case r_Primitive_Type::FLOAT:
            off = (off+3) & ~3;
            increment = 4;
            if (needsAlign < 4) needsAlign = 4;
            break;
        case r_Primitive_Type::DOUBLE:
            off = (off+3) & ~3;
            increment = 8;
            if (needsAlign < 4) needsAlign = 4;
            break;
        default:
            break;
        }
        newOff[i] = off;
        off += increment;
    }
    // needsAlign is now the most coarse alignment needed within the type. The start
    // address of the type itself must also be aligned to this coarseness (e.g.
    // struct {long, char})
    unsigned int length = (off + (needsAlign-1)) & ~(needsAlign-1);
    //cout << "off = " << off << ", length = " << length << endl;

    r_Minterval interv = src->spatial_domain();

#ifdef __VISUALC__
    // Need this rather roundabout syntax for Visual C
    dest = (r_Ref<r_GMarray>&)(new r_GMarray);
#else
    dest = new r_GMarray;
#endif

    int dim = interv.dimension();
    long cells = 1;
    for (i=0; i<(unsigned int)dim; i++)
    {
        cells *= (interv[i].high() - interv[i].low() + 1);
    }
    // claim a new array and initialize it, if necessary
    char *newArray = new char[cells * length];
    if (needsAlign != 1) memset(newArray, 0, cells*length);

    const char *srcArray = src->get_array();
    dest->set_array(newArray);
    dest->set_type_length(length);
    dest->set_spatial_domain(interv);
    dest->set_array_size(cells*length);

    unsigned int srcLength = src->get_type_length();
    long count;

    //cout << "start copying " << newMembers << " members..." << endl;
    for (i=0; i<newMembers; i++)
    {
        //cout << "index " << srcIndex[i] << endl;
        switch (primtypes[srcIndex[i]])
        {
        case r_Primitive_Type::BOOL:
        case r_Primitive_Type::CHAR:
        case r_Primitive_Type::OCTET:
        {
            const char *s = srcArray + offsets[srcIndex[i]];
            char *d = newArray + newOff[i];

            for (count=0; count<cells; count++)
            {
                *d = *s;
                s += srcLength;
                d += length;
            }
        }
        break;
        case r_Primitive_Type::SHORT:
        case r_Primitive_Type::USHORT:
        {
            const short *s = (const short*)(srcArray + offsets[srcIndex[i]]);
            short *d = (short*)(newArray + newOff[i]);

            for (count=0; count<cells; count++)
            {
                *d = *s;
                s = (const short*)(((char*)s) + srcLength);
                d = (short*)(((char*)d) + length);
            }
        }
        break;
        case r_Primitive_Type::LONG:
        case r_Primitive_Type::ULONG:
        case r_Primitive_Type::FLOAT:
        {
            const long *s = (const long*)(srcArray + offsets[srcIndex[i]]);
            long *d = (long*)(newArray + newOff[i]);

            for (count=0; count<cells; count++)
            {
                *d = *s;
                s = (const long*)(((char*)s) + srcLength);
                d = (long*)(((char*)d) + length);
            }
        }
        break;
        case r_Primitive_Type::DOUBLE:
        {
            const double *s = (const double*)(srcArray + offsets[srcIndex[i]]);
            double *d = (double*)(newArray + newOff[i]);

            for (count=0; count<cells; count++)
            {
                *d = *s;
                s = (const double*)(((char*)s) + srcLength);
                d = (double*)(((char*)d) + length);
            }
        }
        break;
        default:
            break;
        }
    }

    delete [] newOff;

    //cout << "build type..." << endl;
    int strLength = 0;
    for (i=0; i<newMembers; i++)
    {
        strLength += strlen(typeNames[srcIndex[i]]) + 2;
    }
    // in case it's a structure type we also need space for "struct { ... }"
    if (newMembers > 1) strLength += strlen(structName) + 5;
    // standard wrapper "marray < ... , dim >"
    strLength += strlen(marrayName) + 5 + 5;

    char *typeString = new char[strLength];
    char *b = typeString;
    b += sprintf(b, "%s < ", marrayName);
    if (newMembers > 1) b += sprintf(b, "%s { ", structName);
    for (i=0; i<newMembers; i++)
    {
        b += sprintf(b, "%s, ", typeNames[srcIndex[i]]);
    }
    if (newMembers > 1)
        strcpy(b-2, " }");
    else
        b -= 2;
    sprintf(b, ", %d >", dim);

    //cout << "TYPE: " << typeString << endl;
    dest->set_type_structure(typeString);

    //cout << "Checkpoint" << endl;

    // Query for the base type the first time only!
    if (strlen(baseTypeName) == 0)
    {
        // find the base type name...
        rviewBaseType baseType = rbt_none;
        if ((newMembers == 3) && (length == 3)) baseType = rbt_rgb;
        if (newMembers == 1)
        {
            switch (primtypes[srcIndex[0]])
            {
            case r_Primitive_Type::BOOL:
                baseType = rbt_bool;
                break;
            case r_Primitive_Type::CHAR:
                baseType = rbt_char;
                break;
            case r_Primitive_Type::OCTET:
                baseType = rbt_uchar;
                break;
            case r_Primitive_Type::SHORT:
                baseType = rbt_short;
                break;
            case r_Primitive_Type::USHORT:
                baseType = rbt_ushort;
                break;
            case r_Primitive_Type::LONG:
                baseType = rbt_long;
                break;
            case r_Primitive_Type::ULONG:
                baseType = rbt_ulong;
                break;
            case r_Primitive_Type::FLOAT:
                baseType = rbt_float;
                break;
            case r_Primitive_Type::DOUBLE:
                baseType = rbt_double;
                break;
            default:
                break;
            }
        }
        if ((baseType != rbt_none) && (dim < MAXIMUM_DIMENSIONS))
        {
            baseTypeName = rviewTypeNames[baseType][dim-1];
        }
        else
        {
            const char *prompt = lman->lookup("promptEnterType");
            char *msg = new char[strlen(prompt) + strlen(typeString) + 2];
            sprintf(msg, "%s %s", prompt, typeString);
            baseTypeName = ::wxGetTextFromUser(msg, lman->lookup("titleEnterType"));
            delete [] msg;
        }
    }
    dest->set_type_by_name(baseTypeName);
    //cout << "Base type name = " << baseTypeName << endl;

    /*dest->print_status();
    const r_Type *btype = dest->get_base_type_schema();
    if (btype == NULL) cout << "no base type schema" << endl;
    btype = r_Type::get_any_type(dest->get_type_structure());
    if (btype == NULL) cout << "string can't be parsed?!?" << endl;*/

    delete [] typeString;
    delete [] srcIndex;

    return 0;
}
