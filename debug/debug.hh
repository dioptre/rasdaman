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
 * SOURCE: debug.hh
 *
 * MODULE: rasmgr/test (prelim)
 *
 * COMMENTS:
 * 		- needs RMInit
*/

/**
*	@file debug.hh
*
*	@ingroup Debug
*/

#ifndef DEBUG_HH
#define DEBUG_HH

/* activated through
#define DEBUG
in the target code; needs one main! */

#include <iostream>

#ifdef DEBUG
// enable trace macros

// allow output stream 'cout' to be overridden
#ifndef OSTREAM
	#define OSTREAM std::cout
#endif // OSTREAM

#define INDENT ". "

// no output by default
#define DEBUG_OUTPUT_DEFAULT false

/// for C++, we declare refs as "C" to exclude them from name mangling;
/// for C it's like always
/// --- well nice, but doesn't work for now, so we set it back
#ifdef __cplusplus
#	define EXTERN extern // "C"
#else
#	define EXTERN extern
#endif

/// variables are allocated in the main module (i.e., the source where main() resides),
///  and referened from all other places
#ifdef DEBUG_MAIN
	int indentLevel = 0;
	bool debugOutput = DEBUG_OUTPUT_DEFAULT;
#else
	EXTERN int indentLevel;
	EXTERN bool debugOutput;
#endif // DEBUG_MAIN

/// enable/disable debug output from program
///	b == true: switch on output
///	b == false: switch off output
#define SET_OUTPUT(b) { debugOutput = b; }

/// ENTER(a): write trace line for entering a function (increases indentation)
#define ENTER(a) 				\
  {						\
    if (debugOutput)				\
    {						\
      for (int i = 0; i < indentLevel; i++)	\
          OSTREAM << INDENT;			\
      OSTREAM << "ENTER " << a << std::endl << std::flush;	\
      indentLevel++;				\
    }						\
  }

/// LEAVE(a): write trace line for leaving a function (decreases indentation)
#define LEAVE(a) 				\
  {						\
    if (debugOutput)				\
    {						\
      if (indentLevel > 0 )			\
          indentLevel--;			\
      for (int i = 0; i < indentLevel; i++)	\
          OSTREAM << INDENT;			\
      OSTREAM << "LEAVE " << a << std::endl << std::flush;	\
    }						\
  }


/// TALK(a): write trace line from within a function (leaves indentation unchanged)
#define TALK(a)  				\
  {						\
    if (debugOutput)				\
    {						\
      for (int i = 0; i < indentLevel; i++)	\
          OSTREAM << INDENT;			\
      OSTREAM << a << std::endl << std::flush;		\
    }						\
  }

#else
// disable all trace macros
#undef SET_OUTPUT
#undef ENTER
#undef LEAVE
#undef TALK

#define SET_OUTPUT(b) { /* SET_OUTPUT(b) */ }
#define ENTER(a) { /* ENTER(a) */ }
#define LEAVE(a) { /* LEAVE(a) */ }
#define TALK(a)  { /* TALK(a) */ }

#endif // DEBUG

#endif // DEBUG_HH
