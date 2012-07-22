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
 * SOURCE: test_marray.cc
 *
 * MODULE: raslib
 *
 * COMMENTS:
 *      None
*/

#include <iostream>

#include "rasodmg/marray.hh"

#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"
#include "raslib/point.hh"
#include "raslib/error.hh"
#include "raslib/type.hh"
#include "raslib/basetype.hh"
#include "raslib/attribute.hh"
#include "raslib/structuretype.hh"
#include "raslib/primitivetype.hh"


struct myStructType
{
    char   red;
    char   green;
    short  east;
    float  middle;
};

struct myStructType3
{
    double  feature_1 ;
    double  feature_2 ;
    double  feature_3 ;
};

struct myStructTypeCom
{
    long                 greyvalue;
    long                 greyvalue2;
    myStructType3  features;
};

static int INIT = 0;
static float FINIT = 0;
static char CINIT = 0;
static double DINIT = 0;
static long LINIT = 0;

int initFunction( const r_Point& /*pt*/ )
{
    /*
    int value=0;

    for( int i=0; i< pt.dimension(); i++ )
     value += pt[i];

    return value;
    */
    return 3*(INIT++)/2;
}

float  initFunctionFloat( const r_Point& /*pt*/ )
{
    return 3*(FINIT++)/2;
}


char  initFunctionChar( const r_Point& /*pt*/ )
{
    return 3*(CINIT++)/2;
}

double  initFunctionDouble( const r_Point& /*pt*/ )
{
    return 3*(DINIT++)/2;
}

long  initFunctionLong( const r_Point& /*pt*/ )
{
    return (LINIT++);
}


myStructType  initFunctionStruct( const r_Point& /*pt*/ )
{
    myStructType myStruct;

    myStruct.red   = 3;
    myStruct.green = 4;
    myStruct.east  = 123;
    myStruct.middle = 7.4;

    return myStruct;
}


myStructType3  initFunctionStruct3( const r_Point& pt)
{
    myStructType3 myStruct;

    myStruct.feature_1 = 0.5* initFunctionDouble( pt );
    DINIT--;
    myStruct.feature_2 = 3* initFunctionDouble( pt );
    DINIT--;
    myStruct.feature_3 = initFunctionDouble( pt );

    return myStruct;
}


myStructTypeCom  initFunctionStructCom( const r_Point& pt)
{
    myStructTypeCom  myStructCom;

    myStructCom.greyvalue =  initFunctionLong( pt );
    LINIT--;
    myStructCom.greyvalue2 =  3*initFunctionLong( pt );

    //myStructCom.features =  initFunctionStruct3( pt );

    myStructCom.features.feature_1 = 0.5* initFunctionDouble( pt );
    DINIT--;
    myStructCom.features.feature_2 = 3* initFunctionDouble( pt );
    DINIT--;
    myStructCom.features.feature_3 = initFunctionDouble( pt );


    return myStructCom;
}



int main()
{
    cout << endl << endl;
    cout << "Marray Examples" << endl;
    cout << "===============" << endl << endl;

    cout << "Initialization of Marray<int, [3:5,6:8]> with init function:" << endl;
    r_Marray<int> a( r_Minterval("[3:5,6:8]"), &initFunction );
    a.set_type_structure("marray<long>");
    cout << "OK" << endl;
    a.print_status( cout );


    cout << "Initialization of Marray<float, [3:5,6:8]> with init function float:" << endl;
    r_Marray<float> b( r_Minterval("[3:5,6:8]"), &initFunctionFloat );
    b.set_type_structure("marray<float>");
    cout << "OK" << endl;
    b.print_status( cout );


    cout << "Initialization of Marray<char, [3:5,6:8]> with init function char:" << endl;
    r_Marray<char> c( r_Minterval("[3:5,6:8]"), &initFunctionChar );
    c.set_type_structure("marray<char>");
    cout << "OK" << endl;
    c.print_status( cout );


    cout << "Initialization of Marray<double, [3:5,6:8]> with init function double:" << endl;
    r_Marray<double> d( r_Minterval("[3:5,6:8]"), &initFunctionDouble );
    d.set_type_structure("marray<double>");
    cout << "OK" << endl;
    d.print_status( cout );

    FINIT = 0;
    CINIT = 0;
    cout << "Initialization of Marray<struct{char, char, short, float}, [3:5,6:8]> with init function struct:" << endl;
    r_Marray<myStructType> st( r_Minterval("[3:5,6:8]"), &initFunctionStruct );
    st.set_type_structure("marray<struct{char,char,short,float}>");
    cout << "OK" << endl;
    st.print_status( cout );

    DINIT = 0;
    cout << "Initialization of Marray<struct{double, double, double}, [3:5,6:8]> with init function struct3:" << endl;
    r_Marray<myStructType3> st3( r_Minterval("[3:5,6:8]"), &initFunctionStruct3 );
    st3.set_type_structure("marray<struct{double,double,double}>");
    cout << "OK" << endl;
    st3.print_status( cout );

    DINIT = 0;
    cout << "Initialization of Marray<struct{ long, long, struct{double, double, double} }, [3:5,6:8]> with init function structCom:" << endl;
    r_Marray<myStructTypeCom> stCom( r_Minterval("[3:5,6:8]"), &initFunctionStructCom );
    stCom.set_type_structure("marray<struct{long,long,struct{double,double,double}}>");
    cout << "OK" << endl;
    stCom.print_status( cout );
    cout << endl;

    // print single cell
    r_GMarray& gstCom = (r_GMarray&)stCom;

    const r_Base_Type& baseType = *(gstCom.get_base_type_schema());
    cout << "gmarray_stCom[4,6] = " << flush;
    baseType.print_value( gstCom[r_Point(4,6)] );
    cout << endl;

    cout << "access second double of inner struct: " << flush;
    const r_Structure_Type& outerStruct     = (const r_Structure_Type&)baseType;
    //  const r_Attribute&      innerAttribute  = outerStruct[2];
    //  const r_Structure_Type& innerStruct     = (const r_Structure_Type&)innerAttribute.type_of();
    //  const r_Attribute&      doubleAttribute = innerStruct[1];
    // cout << doubleAttribute.get_double( gstCom[r_Point(4,6)] ) << endl;
    cout << outerStruct[2][1].get_double( gstCom[r_Point(4,6)] ) << endl;

    return 0;
}
