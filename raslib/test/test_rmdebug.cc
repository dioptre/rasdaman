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
 * SOURCE: test_rmdebug.cc
 *
 * MODULE: raslib
 *
 * PURPOSE:
 *
 * COMMENTS:
 *
 ************************************************************/

#include <iostream>
#include "raslib/rmdebug.hh"

#include "raslib/rminit.hh"

using namespace std;

// number of repetitions for performance tests
static const int repeat = 100000;

RMINITGLOBALS('C')

void testFunc1()
{
    RMDebug localRMDebug = RMDebug("Class1", "testFunc1", "Module1",
                                   __FILE__, __LINE__);
}

void testFunc2(int reclevel)
{
    RMDebug localRMDebug = RMDebug("Class1", "testFunc2", "Module1",
                                   __FILE__, __LINE__);

    if(reclevel > 1)
        testFunc2(reclevel - 1);
}

void testFunc3(void)
{
    RMDebug localRMDebug("Class1", "testFunc3", "server",
                         __FILE__, __LINE__);

//  RMDBGMOUT( 2, RMDebug::module_server, NULL, "D: testing 1" );
//  RMDBGMOUT( 3, RMDebug::module_server, "Class1", "D: testing 2" );
//  RMDBGMINOUT( 4, RMDebug::module_server, "Class1", "D: testing 3" );
}

void testFunc4(int reclevel)
{
    RMDebug localRMDebug(1, "Class2", "testFunc4", RMDebug::module_raslib,
                         __FILE__, __LINE__);

    if (reclevel > 1)
        testFunc4(reclevel - 1);
}

// this was used to test correctness

void oldMain()
{
    int i, busy;

    for(i = 1; i<=5; i++)
        testFunc1();

    testFunc2(5);

    testFunc3();

    testFunc4(4);

    cout << "Test of RMTimer" << endl;

    cout << "The following should hold approximately: timer2 + timer3 = timer1" << endl << endl;

    RMTimer* timer1 = new RMTimer("main","timer1");
    RMTimer* timer2 = new RMTimer("main","timer2");

    timer2->pause();

    RMTimer* timer3 = new RMTimer("main","timer3");
    for( long busy=0; busy <= 50000000; busy++ );
    delete timer3;

    timer2->resume();

    for( busy=0; busy <= 30000000; busy++ );

    delete timer2;
    delete timer1;

    cout << "Benchmark level set to " << RManBenchmark << endl;

    RMInit::bmOut << "test output in benchmark stream" << endl;
}

double testStatic(double dummy)
{
    RMTIMER("test_rmdebug", "testStatic");

    for(int i=0; i<repeat; i++)
        dummy = dummy*4711.4712;
    return dummy;
}

double testDynamic(double dummy)
{
#ifdef RMANBENCHMARK
    RMTimer* localRMTimer = 0;

    if( RManBenchmark >= 3 )
        localRMTimer = new RMTimer("test_rmdebug", "testDynamic");
#endif

    for(int i=0; i<repeat; i++)
        dummy = dummy*4711.4712;

#ifdef RMANBENCHMARK
    if( localRMTimer ) delete localRMTimer;
#endif

    return dummy;
}

// Evaluating time needed for measurement with different uses
// of RMTimer.

void testPerf()
{
    double dummy = 3.14;

    cout << "Testing dynamic RMTimer vs. static RMTimer." << endl;

    // to put in cache
    dummy = testStatic(dummy);
    cout << "Static:" << endl;
    dummy = testStatic(dummy);

    // to put in cache
    dummy = testDynamic(dummy);
    cout << "Dynamic:" << endl;
    dummy = testDynamic(dummy);
}

int main()
{
    RMDebug::initRMDebug();
    RManBenchmark = 4;
//  RMInit::bmOut = cout.rdbuf();

    oldMain();
    // testPerf();

    return 0;
}


