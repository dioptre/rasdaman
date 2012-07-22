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
 * SOURCE: test_timer.cc
 *
 * MODULE: raslib
 *
 * PURPOSE:
 *
 * Test program to test class RMTimer.
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#include <iostream>
#include "raslib/rmdebug.hh"


RMINITGLOBALS('C')

using namespace std;

class Exp
{

public:
    Exp( char* name );

    void useTimer( );

    ~Exp( );

    RMTimer* t1;
    RMTimer t2;
    char* name;
};

Exp::Exp( char* n)
    :t2("Test Timer ", "t2" ), name( n )
{
    t1 = new RMTimer( "Test Timer ", "t1" );
}

void Exp::useTimer( )
{
    cout << "useTimer "<< name << " t1 ... "<<endl;
    t1->start( );
    t1->stop( );
    cout << "and  t2"<<endl;
    t2.start( );
    t2.stop( );
}

Exp::~Exp( )
{
    cout << "Exp::~Exp( " << name << " ) "<< endl;
    delete t1;
}

static const Exp exp1("Static Exp Object");

int main()
{

    /*
    RMTimer tt(  "Test Timer ", "tt" );

    cout << "RMTimer start( )" <<endl;
    tt.start( );
    tt.stop( );
    */

    Exp e( "Exp Object" );
    e.useTimer( );

    Exp* ep = new Exp("Pointer to Exp Object");
    ep->useTimer( );
    delete ep;

    return 0;
}
