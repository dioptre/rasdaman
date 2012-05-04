/*************************************************************
 *
 * Copyright (C) 2003 Dr. Peter Baumann
 *
 * SOURCE: rasgeo_globals.cc
 *
 * MODULE: rasgeo
 *
 * PURPOSE:
 * provide global basic services used by the rasgeo tools:
 * signal handling, db/ta open/close, ...
 *
 * CHANGE HISTORY (append further entries):
 * when         who         what
 * ----------------------------------------------------------
 * 2003-jul-02  PB          cloned from rasodmg/test/system_basic.cc
 * 2003-dec-05  PB          don't use xx_error, because  rasgeo_signal.cc is shared between import and export
 *
 * COMMENTS:
 *
 ************************************************************/

static const char rcsid[] = "@(#)rasodmg/test,ImportOrthoUtil: $Id: rasgeo_signal.cc,v 1.1 2003/12/27 21:53:50 rasdev Exp $";

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include <iostream>
#include <string>
#include <iostream>
#include <signal.h>
#ifdef SOLARIS
#include <strings.h>
#endif

#include "rasgeo_globals.hh"
#include "rasgeo_signal.hh"

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
	static bool handleSignal = true;	// sema to prevent nested signals

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
		case SIGSTKFLT:
			cout << "Stack fault.";
			break;
#endif
#endif
		case SIGCLD:
			cout << "SIGCHLD (System V) or child status has changed (POSIX).";
			break;
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
		case SIGPOLL:
			cout << "Pollable event occurred (System V) or I/O now possible (4.2 BSD).";
			break;
		case SIGPWR:
			cout << "Power failure restart (System V).";
			break;
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
		cout << " Exiting." << endl;
		exit(sig);
	}
}

void
installSignalHandlers()
{
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
	signal(SIGSTKFLT, signalHandler);
#endif
#endif
	signal(SIGCLD, signalHandler);
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
	signal(SIGPOLL, signalHandler);
	signal(SIGIO, signalHandler);
	signal(SIGPWR, signalHandler);
	signal(SIGSYS, signalHandler);
#if !defined SOLARIS
#if !defined DECALPHA
	signal(SIGUNUSED, signalHandler);
#endif
#endif
}

