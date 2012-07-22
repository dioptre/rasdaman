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
 * SOURCE: testabc.cc
 *
 * MODULE: test for A,B,C
 *
 * PURPOSE:
 *   Tests a hierarchy of classes where each one, after insertElement( )
 *   transforms itself into the superclass. Preparation for Hierarchy
 *   of indexes DirIx, RegDirIx, etc.
 *
 * COMMENTS:
 *   none
 *
*/

#include <stdlib.h>
#include <iostream.h>
// #include <vector.h>


// extern char* myExecArgv0 = "";

// #include "raslib/rminit.hh"
//RMINITGLOBALS('C')

class A
{
public:
    A( );
    A( A* a );
    virtual void insertElement( A*& a );
    virtual A* insertElementTransform( );
    static void insertElementTransformStat( A*& );
    virtual void printStatus( );
    virtual ~A( );
    int* DynAtt_A;
    int  StatAtt_A;
protected:
    void testProtectedA( );
};

A::A( )
    :StatAtt_A(1)
{
    DynAtt_A = new int;
    *DynAtt_A = 2;
    cout <<"A Constructor"<< endl;
}

A::A( A* a)
    :StatAtt_A( a->StatAtt_A ), DynAtt_A( a->DynAtt_A )
{
    a->DynAtt_A = 0;
}

void A::insertElement( A*& a )
{
    cout <<"A::insertElement( ) ";
    printStatus( );
    cout << endl;
}

A* A::insertElementTransform( )
{
    cout <<"A::insertElementTransform( ) ";
    printStatus( );
    cout << endl;
    return this;
}

void A::insertElementTransformStat( A*& ix )
{
    cout <<"A::insertElementTransformStat( ) ";
    ix->printStatus( );
    cout << endl;
}

void A::printStatus( )
{
    cout <<"A: Dyn " << *DynAtt_A <<" Stat "<< StatAtt_A << endl;
}

A::~A()
{
    cout <<"A Destructor"<< endl;
    if (DynAtt_A) delete DynAtt_A;
}

void A::testProtectedA( )
{
    cout <<"A::testProtectedA( )"<<endl;
}

class B: public A
{
public:
    B( );
    B( A* b );
    virtual void insertElement( A*& a );
    virtual A* insertElementTransform( );
    static void insertElementTransformStat( A*& ix );
    virtual void printStatus( );
    virtual ~B( );
    int* DynAtt_B;
    int  StatAtt_B;
    void testProtectedB( );
};

B::B( )
    :A( ),StatAtt_B(3)
{
    DynAtt_B = new int;
    *DynAtt_B = 4;
    cout <<"B Constructor"<< endl;
}

B::B( A* b )
    :A(b),StatAtt_B(((B*)b)->StatAtt_B),DynAtt_B(((B*)b)->DynAtt_B)
{
    // use dynamic attributes of b, so that has to delete them
    // from origin
    ((B*)b)->DynAtt_B = 0;
}

void B::insertElement( A*& a )
{
    cout <<"B::insertElement( ) ";
    printStatus( );
    cout << endl;
    A* c = new A( a );
    delete a;
    a = c;
}

A* B::insertElementTransform( )
{
    A* thisA = this;
    cout <<"B::insertElementTransform( ) ";
    printStatus( );

    A* a = new A(thisA);
    delete thisA;
    return a;
}

void B::insertElementTransformStat( A*& ix )
{
    A* thisA = ix;
    cout <<"B::insertElementTransformStat( ) ";
    ix->printStatus( );

    A* a = new A(thisA);
    delete thisA;
    ix = thisA;
}

void B::printStatus( )
{
    cout <<"B: Dyn " << *DynAtt_B <<" Stat "<< StatAtt_B << endl;
}
B::~B()
{
    cout <<"B Destructor"<< endl;
    if (DynAtt_B) delete DynAtt_B;
}

void B::testProtectedB( )
{
    cout << "B::testProtectedB( ) calling A::testProtectedA( ) on itself: "<< endl;
    A::testProtectedA( );
    B objB;
    cout << "B::testProtectedB( ) calling A::testProtectedA( ) on another obj B: "<< endl;
    objB.testProtectedA( );
    // A objA;
    // cout << "B::testProtectedB( ) calling A::testProtectedA( ) on another obj A: "<< endl;
    // objA.testProtectedA();
}

class C: public B
{
public:
    C( int );
    virtual void insertElement( A*& a );
    virtual A* insertElementTransform( );
    static void insertElementTransformStat( A*& ix );
    virtual void printStatus( );
    virtual ~C( );
    int* DynAtt_C;
    int  StatAtt_C;
};

C::C(int)
    :B(), StatAtt_C(5)
{
    DynAtt_C = new int;
    *DynAtt_C = 6;
    cout <<"C Constructor "<< endl;
}
void C::insertElement( A*& a )
{
    if( a != this )
        cout << "Unexpected Usage of C::insertElement( ) "<< endl;
    cout <<"C::insertElement( ) ";
    printStatus( );

    B* c = new B(a);
    delete a;
    a = c;
}

A* C::insertElementTransform( )
{
    A* thisA = this;
    cout <<"C::insertElementTransform( ) ";
    printStatus( );

    B* b = new B(thisA);
    delete thisA;
    return b;

}

void C::insertElementTransformStat( A*& ix )
{
    A* thisA = ix;
    cout <<"C::insertElementTransformStat( ) ";
    ix->printStatus( );


    B* b = new B(thisA);
    ix = b;
    delete thisA;

}

void C::printStatus( )
{
    cout <<"C: Dyn " << *DynAtt_C <<" Stat "<< StatAtt_C << endl;
}

C::~C( )
{
    cout <<"C Destructor"<< endl;
    if (DynAtt_C) delete DynAtt_C;
}

/*************************************************************
 * Function name.: int main( )
 *
 * Return value..: exit status
 ************************************************************/
int
main( )
{

    cout << endl << " ---------------------------------------------- " << endl;
    cout << endl << " Testing insertElementTransformStat( ) -------- " <<endl;

    int i1t = 3;
    cout << "Creating new C" << endl;
    A *at = new C( i1t );
    cout << endl << "Inserting 1. element in C and getting B"<< endl;
    at->insertElementTransformStat( at );
    cout << endl << "Inserting 2. element in B and getting A"<< endl;
    at->insertElementTransformStat( at );
    cout << endl << "Inserting 3. element in A and getting A"<< endl;
    at->insertElementTransformStat( at );
    cout << endl << "Inserting 4. element in A and getting A"<< endl;
    at->insertElementTransformStat( at );
    cout << endl << "Destroying object"<< endl;
    delete at;

    cout << endl << " ---------------------------------------------- "<< endl;
    at = new C( i1t );
    cout << endl << "Inserting 1. element in C and getting B"<< endl;
    at->insertElementTransformStat( at );
    cout << endl << "Destroying object"<< endl;
    delete at;

    cout << endl << " ---------------------------------------------- " << endl;

    exit( 0);
    /*
      cout << endl << " ---------------------------------------------- " << endl;
      cout << endl << " Testing insertElementTransform( ) ------------ " <<endl;

      int i1t = 3;
      cout << "Creating new C" << endl;
      A *at = new C( i1t );
      cout << endl << "Inserting 1. element in C and getting B"<< endl;
      at = at->insertElementTransform( );
      cout << endl << "Inserting 2. element in B and getting A"<< endl;
      at = at->insertElementTransform( );
      cout << endl << "Inserting 3. element in A and getting A"<< endl;
      at = at->insertElementTransform( );
      cout << endl << "Inserting 4. element in A and getting A"<< endl;
      at = at->insertElementTransform( );
      cout << endl << "Destroying object"<< endl;
      delete at;

      cout << endl << " ---------------------------------------------- "<< endl;
      at = new C( i1t );
      cout << endl << "Inserting 1. element in C and getting B"<< endl;
      at = at->insertElementTransform( );
      cout << endl << "Destroying object"<< endl;
      delete at;

      cout << endl << " ---------------------------------------------- " << endl;
    */



    /*
      B objB;
      objB.testProtectedB( );
      exit( 0 );

      int i1 = 3;
      cout << "Creating new C" << endl;
      A *a = new C( i1 );
      cout << endl << "Inserting 1. element in C and getting B"<< endl;
      a->insertElement( a );
      cout << endl << "Inserting 2. element in B and getting A"<< endl;
      a->insertElement( a );
      cout << endl << "Inserting 3. element in A and getting A"<< endl;
      a->insertElement( a );
      cout << endl << "Inserting 4. element in A and getting A"<< endl;
      a->insertElement( a );
      cout << endl << "Destroying object"<< endl;
      delete a;

      cout << endl << " ---------------------------------------------- "<< endl;
      a = new C( i1 );
      cout << endl << "Inserting 1. element in C and getting B"<< endl;
      a->insertElement( a );
      cout << endl << "Destroying object"<< endl;
      delete a;

      cout << endl << " ---------------------------------------------- "<< endl;
      C a1( i1 );
      a = new C( i1 );
      cout << endl << "Inserting 1. element in C and getting B"<< endl;
      a1.insertElement( a );
      cout << endl << "Inserting 2. element in C and getting B"<< endl;
      a->insertElement( a );
      cout << endl << "Destroying object"<< endl;
      delete a;

      exit( 0 );
    */
}
