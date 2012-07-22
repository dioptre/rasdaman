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
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann / rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
/

/**
* rasql_signal
*
* PURPOSE:  Provides signal handling
*
* COMMENTS:
*
*   No comments
*/


static const char rcsid[] = "@(#)rasodmg/test,ImportOrthoUtil: $Id: rasql_signal.cc,v 1.1 2003/12/27 19:30:23 rasdev Exp $";

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include <iostream>
#include <string>
#include <cstdlib>
#include <signal.h>
#ifdef SOLARIS
#include <strings.h>
#endif

using namespace std;

#include "rasql_error.hh"
#include "rasql_signal.hh"

// debug facility; relies on -DDEBUG at compile time
#include "debug-clt.hh"

using namespace std;

//signalCleanup function is called when a signal is received by the program.
//You should write your function in order to have signal management
void signalCleanup();

//signalHandler function is called when a signal occurs
void
signalHandler(int sig);

//installSignalHandlers function should be called first in main function
//in order to receive a signal in your program
void
signalHandler(int sig)
{
    static bool handleSignal = true;    // sema to prevent nested signals

    cout << "Caught signal " << sig << ": ";
    switch (sig)
    {
    case SIGHUP:
        cout << "Hangup (POSIX).  ";
        break;
    case SIGINT:
        cout << "Interrupt (ANSI).";
        break;
    case SIGQUIT:
        cout << "Quit (POSIX).";
        break;
    case SIGILL:
        cout << "Illegal instruction (ANSI).";
        break;
    case SIGTRAP:
        cout << "Trace trap (POSIX).";
        break;
    case SIGABRT:
        cout << "Abort (ANSI) or IOT trap (4.2 BSD).";
        break;
    case SIGBUS:
        cout << "BUS error (4.2 BSD).";
        break;
    case SIGFPE:
        cout << "Floating-point exception (ANSI).";
        break;
    case SIGKILL:
        cout << "Kill, unblockable (POSIX).";
        break;
    case SIGUSR1:
        cout << "User-defined signal 1 (POSIX).";
        break;
    case SIGSEGV:
        cout << "Segmentation violation (ANSI).";
        break;
    case SIGUSR2:
        cout << "User-defined signal 2 (POSIX).";
        break;
    case SIGPIPE:
        cout << "Broken pipe (POSIX).";
        break;
    case SIGALRM:
        cout << "Alarm clock (POSIX).";
        break;
    case SIGTERM:
        cout << "Termination (ANSI).";
        break;
#ifndef SOLARIS
#ifndef DECALPHA
#ifndef __APPLE__
    case SIGSTKFLT:
        cout << "Stack fault.";
        break;
#endif
#endif
#endif
    case SIGCONT:
        cout << "Continue (POSIX).";
        break;
    case SIGSTOP:
        cout << "Stop, unblockable (POSIX).";
        break;
    case SIGTSTP:
        cout << "Keyboard stop (POSIX). Continuing operation.";
        break;
    case SIGTTIN:
        cout << "Background read from tty (POSIX).";
        break;
    case SIGTTOU:
        cout << "Background write to tty (POSIX). Continuing operation";
        break;
    case SIGURG:
        cout << "Urgent condition on socket (4.2 BSD).";
        break;
    case SIGXCPU:
        cout << "CPU limit exceeded (4.2 BSD).";
        break;
    case SIGXFSZ:
        cout << "File size limit exceeded (4.2 BSD).";
        break;
    case SIGVTALRM:
        cout << "Virtual alarm clock (4.2 BSD).";
        break;
    case SIGPROF:
        cout << "Profiling alarm clock (4.2 BSD).";
        break;
    case SIGWINCH:
        cout << "Window size change (4.3 BSD, Sun). Continuing operation.";
        break;
#ifndef __APPLE__
    case SIGCLD:
        cout << "SIGCHLD (System V) or child status has changed (POSIX).";
        break;
    case SIGPOLL:
        cout << "Pollable event occurred (System V) or I/O now possible (4.2 BSD).";
        break;
    case SIGPWR:
        cout << "Power failure restart (System V).";
        break;
#endif
    case SIGSYS:
        cout << "Bad system call.";
        break;
    default:
        cout << "Unknown signal.";
        break;
    }
    cout << endl << flush;

    // no repeated signals
    if (handleSignal)
        handleSignal = false;

    if (sig == SIGCONT || sig == SIGTSTP || sig == SIGTTIN || sig == SIGTTOU || sig == SIGWINCH)
        return;
    else
    {
        TALK( "fatal signal, exiting." << flush );
        exit(sig);
    }
}

void
installSignalHandlers()
{
    ENTER( "installSignalHandlers" );

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGHUP, signalHandler);
    signal(SIGPIPE, signalHandler);
    signal(SIGHUP, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGTRAP, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGIOT, signalHandler);
    signal(SIGBUS, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGKILL, signalHandler);
    signal(SIGUSR1, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGUSR2, signalHandler);
    signal(SIGPIPE, signalHandler);
    signal(SIGALRM, signalHandler);
    signal(SIGTERM, signalHandler);
#ifndef SOLARIS
#ifndef DECALPHA
#ifndef __APPLE__
    signal(SIGSTKFLT, signalHandler);
#endif
#endif
#endif
    signal(SIGCHLD, signalHandler);
    signal(SIGCONT, signalHandler);
    signal(SIGSTOP, signalHandler);
    signal(SIGTSTP, signalHandler);
    signal(SIGTTIN, signalHandler);
    signal(SIGTTOU, signalHandler);
    signal(SIGURG, signalHandler);
    signal(SIGXCPU, signalHandler);
    signal(SIGXFSZ, signalHandler);
    signal(SIGVTALRM, signalHandler);
    signal(SIGPROF, signalHandler);
    signal(SIGWINCH, signalHandler);
#ifndef __APPLE__
    signal(SIGCLD, signalHandler);
    signal(SIGPOLL, signalHandler);
    signal(SIGPWR, signalHandler);
#endif
    signal(SIGIO, signalHandler);
    signal(SIGSYS, signalHandler);
#if !defined SOLARIS
#if !defined DECALPHA
#ifndef __APPLE__
    signal(SIGUNUSED, signalHandler);
#endif
#endif
#endif
    LEAVE( "installSignalHandlers" );
}

