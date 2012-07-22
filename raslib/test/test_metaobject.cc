/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
*/

/*************************************************************
 *
 * SOURCE: test_metaobject.cc
 *
 * MODULE: raslib
 *
 * PURPOSE:
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#include <iostream>

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "raslib/template_inst.hh"
#include "raslib/itertype.hh"
#include "raslib/structuretype.hh"
#include "raslib/attribute.hh"
#include "raslib/primitivetype.hh"
#include "raslib/rminit.hh"
#include "raslib/basetype.hh"
#include "raslib/marraytype.hh"

#include "raslib/error.hh"
#include "raslib/type.hh"
#include "raslib/structure.hh"
#include "raslib/primitive.hh"

using namespace std;

void testType( const char* stringType )
{
    r_Type* type = NULL;

    cout << "Create " << stringType << endl;

    try
    {
        type = r_Type::get_any_type( stringType );
    }
    catch( r_Error& errorObj )
    {
        cout << errorObj.what() << endl << endl;
    }

    cout << "  Type: ";

    if( type )
    {
        type->print_status( cout );
        cout << endl;
        cout << type->type_id() << endl;
    }
    else
    {
        cout << "<not available>" << endl;
    }
    cout << endl;
    delete type;

}

/*
void testEndian()
{
  r_Primitive_Type boolType("Bool", r_Primitive_Type::BOOL);
  r_Primitive_Type shortType("Short", r_Primitive_Type::SHORT);
  r_Primitive_Type uLongType("ULong", r_Primitive_Type::ULONG);
  r_Type* structType;
  structType = r_Type::get_any_type( "struct{bool e1, short e2, ulong e3}" );

  char cChar = 47;
  short cShort = 1065;
  unsigned long cULong = 92753;

  char *boolCell = (char*)&cChar;
  char *shortCell = (char*)&cShort;
  char *uLongCell = (char*)&cULong;
  char structCell[7] = { 1, 2, 3, 4, 5, 6, 7 };

  cout << "Before convertToLittleEndian:" << endl;
  cout << "Char: " << (long)*boolCell << endl
       << "Short: " << (long)*shortCell << " " << (long)*(shortCell+1) << endl
       << "ULong: " << (long)*uLongCell << " " << (long)*(uLongCell+1) << " "
                    << (long)*(uLongCell+2) << " " << (long)*(uLongCell+3)
                    << endl
       << "Struct: " << (long)*structCell << " " << (long)*(structCell+1)
                     << " " << (long)*(structCell+2) << " "
                     << (long)*(structCell+3) << " "
                     << (long)*(structCell+4) << " "
                     << (long)*(structCell+5) << " "
                     << (long)*(structCell+6) << " "
       << endl;

  boolType.convertToLittleEndian(boolCell, 1);
  shortType.convertToLittleEndian(shortCell, 1);
  uLongType.convertToLittleEndian(uLongCell, 1);
  structType->convertToLittleEndian(structCell, 1);

  cout << "After convertToLittleEndian:" << endl;
  cout << "Char: " << (long)*boolCell << endl
       << "Short: " << (long)*shortCell << " " << (long)*(shortCell+1) << endl
       << "ULong: " << (long)*uLongCell << " " << (long)*(uLongCell+1) << " "
                    << (long)*(uLongCell+2) << " " << (long)*(uLongCell+3)
                    << endl
       << "Struct: " << (long)*structCell << " " << (long)*(structCell+1)
                     << " " << (long)*(structCell+2) << " "
                     << (long)*(structCell+3) << " "
                     << (long)*(structCell+4) << " "
                     << (long)*(structCell+5) << " "
                     << (long)*(structCell+6) << " "
       << endl;

  boolType.convertToBigEndian(boolCell, 1);
  shortType.convertToBigEndian(shortCell, 1);
  uLongType.convertToBigEndian(uLongCell, 1);
  structType->convertToLittleEndian(structCell, 1);

  cout << "After convertToBigEndian:" << endl;
  cout << "Char: " << (long)*boolCell << endl
       << "Short: " << (long)*shortCell << " " << (long)*(shortCell+1) << endl
       << "ULong: " << (long)*uLongCell << " " << (long)*(uLongCell+1) << " "
                    << (long)*(uLongCell+2) << " " << (long)*(uLongCell+3)
                    << endl
       << "Struct: " << (long)*structCell << " " << (long)*(structCell+1)
                     << " " << (long)*(structCell+2) << " "
                     << (long)*(structCell+3) << " "
                     << (long)*(structCell+4) << " "
                     << (long)*(structCell+5) << " "
                     << (long)*(structCell+6) << " "
       << endl;

  delete structType;
}
*/

int main()
{
    cout << "Creating definining primitive types ..." << endl;
    r_Primitive_Type myBool("Bool", r_Primitive_Type::BOOL);
    myBool.print_status( cout );
    cout << endl;
    r_Primitive_Type myULong("ULong", r_Primitive_Type::ULONG);
    myULong.print_status( cout );
    cout << endl;

    r_Primitive_Type tmp = myBool;
    tmp.print_status( cout );
    cout << endl;

    r_Attribute tmpAtt("tmpAtt",tmp );
    tmpAtt.print_status( cout );
    cout << endl;

    cout << "Creating a struct out of them ..." << endl;
    r_Attribute myAttrs[2];
    myAttrs[0] = r_Attribute("Attr1", myBool );
    myAttrs[1] = r_Attribute("Attr2", myULong );

    r_Structure_Type myStruct("aStruct", 2, myAttrs);
    myStruct.print_status( cout );
    cout << endl;

    cout << "Iterating attributes of struct:" << endl;
    r_Structure_Type::attribute_iterator
    iter(myStruct.defines_attribute_begin());
    while(iter != myStruct.defines_attribute_end())
    {
        cout << "  Name of Attribute: " << (*iter).name() << endl;
        cout << "  Offset of Attribute: " << (*iter).offset() << endl;
        cout << "  Size of type of Attribute: "
             << (*iter).type_of().size() << endl;
        cout << "  Name of type of Attribute: "
             << (*iter).type_of().name() << endl;
        ++iter;
    }

    testType("char");

    testType("octet");
    testType("short");
    testType("ushort");
    testType("long");
    testType("ulong");
    testType("bool");
    testType("float");
    testType("double");

    testType("struct{ char   }");
    testType("struct{ char band1 }");

    testType("struct{ char, octet, ulong, short }");

    testType("struct{ char elem1, octet elem2, ulong elem3, short elem4 }");
    testType("struct{ char red, char green, char blue }" );
    testType("struct{char red, char green, char blue}" );

    testType("struct{ struct{ char, char, char }, ulong }");
    testType("struct{ struct{ char elem1, char elem2, char elem3 } record, ulong value }");


    testType("marray< char >");
    testType("marray< char  green>");
    testType("marray< struct{ char red} >");

    testType("marray< struct{char red, char green, char blue} >" );

    testType("set< marray< char > >");
    testType("set< marray< struct{ char red, char green, char blue } > >" );

    testType("interval");
    testType("minterval");
    testType("point");
    testType("oid");

    testType("set< interval  >");
    testType("set< minterval  >");
    testType("set< point  >");
    testType("set< oid  >");

    /* shouldn't work */
    cout << endl << "Testing combinations which are not allowed..." << endl;
    testType("set< marray< interval > >");
    testType("set< marray< minterval > >");
    testType("set< marray< point > >");
    testType("set< marray< oid > >");

    testType("interval<set< marray< char > > >");
    testType("interval<struct{ point blue, interval green}>");
    testType("set< marray{ char > >");
    testType("struct<char>");



    r_Type* type = NULL;
    char* stringType = "marray< char blue>";

    cout << "Create " << stringType << endl;

    try
    {
        type = r_Type::get_any_type( stringType );
    }
    catch( r_Error& errorObj )
    {
        cout << errorObj.what() << endl << endl;
    }

    cout << "  Type: ";

    if( type )
    {
        type->print_status( cout );
    }
    else
    {
        cout << "<not available>" << endl;
    }
    cout << endl;

    //  cout << ((r_Marray_Type*)type)->getBaseType() << endl;

    cout << "Erzeugen einer Kopie und Ausgabe..." << endl;
    r_Marray_Type  my_marray((r_Base_Type&) type);

    my_marray.print_status( cout );

    cout << endl;
    //  cout  << my_marray.getBaseType() <<  endl;

    delete type;

    r_Type* type2 = r_Type::get_any_type("struct{ short band1, char band2 }");

    if( type2->isBaseType() )
    {
        r_Base_Type* baseType2 = (r_Base_Type*)type2;

        cout << "Type: " << flush;
        baseType2->print_status();
        cout << endl;
        cout << "Size: " << baseType2->size() << endl;
    }

    struct structType
    {
        short band1r;
        char band2i;
    };

    structType structValue = { 1, 2 };

    r_Structure structObject( (const char*)&structValue, (const r_Structure_Type*)type2 );

    structObject.print_status( cout );

    cout << endl;

    delete type2;

    /* testEndian(); */
    return 0;
}


