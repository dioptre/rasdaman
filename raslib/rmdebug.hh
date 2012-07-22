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

/**
 * INCLUDE: rmdebug.hh
 *
 * PURPOSE:
 *      Contains debug stuff.
 *
 * COMMENTS:
 *
*/

#ifndef _RMDEBUG_
#define _RMDEBUG_

#ifdef __VISUALC__
#include <time.h>
#else
#include <sys/time.h>
#endif

#include "raslib/rminit.hh"
#include "raslib/rm.hh"

extern int RManDebug;
extern int RManBenchmark;

#ifdef RMANDEBUG

#define RMDBGIF( levell, module, cls, text ) \
  if (RMDebug::debugOutput( levell, module, cls )) { text }

#define RMDBGENTER( levell, module, cls, text ) \
  RMCounter rmCounter(levell, module, cls); \
  if (RMDebug::debugOutput( levell, module, cls )) { \
    RMInit::dbgOut << "ENTER  "; RMInit::dbgOut.width(18); RMInit::dbgOut.setf(ios::left, ios::adjustfield); RMInit::dbgOut << cls << " "; RMDebug::indentLine(); RMInit::dbgOut << text << endl << std::flush; \
  }

#define RMDBGMIDDLE( levell, module, cls, text ) \
  if (RMDebug::debugOutput( levell, module, cls )) { \
    RMInit::dbgOut << "MIDDLE "; RMInit::dbgOut.width(18); RMInit::dbgOut.setf(ios::left, ios::adjustfield); RMInit::dbgOut << cls << " "; RMDebug::indentLine(); RMInit::dbgOut << text << endl << std::flush; \
  }

#define RMDBGONCE( levell, module, cls, text ) \
  RMCounter rmCounter(levell, module, cls); \
  if (RMDebug::debugOutput(levell, module, cls)) \
  { \
    RMInit::dbgOut << "ONCE   "; RMInit::dbgOut.width(18); RMInit::dbgOut.setf(ios::left, ios::adjustfield); RMInit::dbgOut << cls << " "; \
    RMDebug::indentLine(); \
    RMInit::dbgOut << text << endl << std::flush; \
  }

#define RMDBGEXIT( levell, module, cls, text ) \
  if (RMDebug::debugOutput( levell, module, cls )) { \
    RMInit::dbgOut << "EXIT   "; RMInit::dbgOut.width(18); RMInit::dbgOut.setf(ios::left, ios::adjustfield); RMInit::dbgOut << cls << " "; RMDebug::indentLine(); RMInit::dbgOut << text << endl << std::flush; \
  }

#define RMDBCLASS( t1, t2, t3, t4, t5 ) RMDebug localRMDebug = RMDebug( t1, t2, t3, t4, t5 );

#else

// Note: some parts of the code rely on these to be terminated by a ';'!
#define RMDBGENTER( level, module, cls, text ) ;
#define RMDBGMIDDLE( level, module, cls, text ) ;
#define RMDBGONCE( level, module, cls, text ) ;
#define RMDBGEXIT( level, module, cls, text ) ;
#define RMDBGIF( level, module, cls, text) ;

#define RMDBCLASS( t1, t2, t3, t4, t5 ) ;
#endif

// generate benchmark code only when RMANBENCHMARK is set
#ifdef RMANBENCHMARK
#define RMTIMER(class, func)  RMTimer localRMTimer = RMTimer(class, func);
#else
#define RMTIMER(class, func)
#endif

//@ManMemo: Module: {\bf raslib}.

/*@Doc:
RMDebug is not strictly part of RasLib. It is a class used for
generating debug output if compiling with RMANDEBUG defined. One way
of using it is to put the following at the beginning of a function:

{\tt RMDebug localRMDebug = RMDebug("className", "functionName",
"moduleName", __FILE__, __LINE__");}

This can be patched in automatically by a modified funchead.pl script.

{\bf Functionality}

Debug output printing class name, function name, module name, file
name and line number given as parameters to the constructor is
created, whenever the constructor is called. The destructor
outputs class name and function name. If the static members {\tt
debugModules} or {\tt debugClasses} are set, then only modules
which are mentioned in the array of strings {\tt debugModules} or
classes which are mentioned {\tt debugClasses} give debug output.
{\tt debugModules} and {\tt debugClasses} can either be read from
files named "rmdbmodules" and "rmdbclasses" or from the environment
variables RMDBGMODULES and RMDBGCLASSES. The environment variables
override the files. The contents of the files / variables are the
names of the modules / classes separated by whitespace (space,
newlines, ...). In the case of the modules each modulename may
be followed by ",<dbgLevel>" to set the debug level for that
module explizitly, otherwise the default is used.

{\bf Interdependencies}

If only certain modules or classes are to be debugged, RMDebug
has to be initialized in {\Ref RMInit}. This is done by reading
the files {\tt rmdbmodules} and {\tt rmdbclasses}. The files
should contain names of modules resp. classes to be debugged, each
(including the last one!) followed by end of line. */

/**
  * \defgroup RMs RM Classes
  */

/**
  * \ingroup RMs
  */

class RMDebug : public RM_Class
{
public:
    /// constructor, initializes members and prints message.
    RMDebug(const char* newClass, const char* newFunc, const char* newModule,
            const char* newFile, int newLine);
    /// constructor taking an identifier to the module for more efficiency
    RMDebug(int newLevel, const char* newClass, const char* newFun, int newModuleNum,
            const char* newFile, int newLine);
    /// destructor, prints message.
    ~RMDebug(void);

    /// for initializing modules and classes to debug.
    static int initRMDebug(void);

    /// get the debug level of a module by its number
    static inline int getModuleDebugLevel(int modNum)
    {
        return allModuleLevels[modNum];
    }
    /// get the name of a module by its number
    static inline const char* getModuleName(int modNum)
    {
        return allModuleNames[modNum];
    }
    /// indent by the amount specified by level
    static inline void indentLine(void)
    {
        for (int i=0; i<level; i++) RMInit::dbgOut << "  ";
    }

    /// return whether debug output should happen for the given module, class
    /// and debugging level
    static int debugOutput(int dbgLevel, int modNum, const char* className);

    /// all modules for debugging
    enum
    {
        module_adminif = 0,
        module_applications,
        module_blobif,
        module_cachetamgr,
        module_catalogif,
        module_catalogmgr,
        module_clientcomm,
        module_conversion,
        module_httpserver,
        module_indexif,
        module_indexmgr,
        module_insertutils,
        module_mddif,
        module_mddmgr,
        module_qlparser,
        module_rasdl,
        module_raslib,
        module_rasodmg,
        module_rasql,
        module_server,
        module_servercomm,
        module_storageif,
        module_storagemgr,
        module_tilemgr,
        module_tools,
        module_utilities,
        module_number
    } RMDebugModules;
    /*@ManMemo level of function calls (incremented by constructor,
               decremented by destrctor. */
    static int level;

private:
    /// checks, if messages should be printed.
    int checkDebug(void);
    /// loads a file containing text and returns a 0-terminated string
    static char* loadTextFile(const char* name);
    /*@Doc:
       If {\tt debugModules} or {\tt debugClasses} is set, checks
       if myModule or myClass is in the corresponding array.
    */

    /// name of class.
    const char* myClass;
    /// name of function (no parameters).
    const char* myFunc;
    /// name of module.
    const char* myModule;
    /// name of source file.
    const char* myFile;
    /// line of code where destructor call is.
    int myLine;
    /// number of module
    int myModuleNum;
    /// debugging level for this instance
    int myDebugLevel;
    /// debugging on for this class?
    int myDebugOn;
    /// number of strings in {\tt debugModules}.
    static int numDebugModules;
    /// array with pointers into names of modules to be debugged.
    static char** debugModules;
    /// names of modules to be debugged.
    static char* debugModulesText;
    /// number of strings in {\tt debugClasses}.
    static int numDebugClasses;
    /// array with pointers into names of classes to be debugged.
    static char** debugClasses;
    /// names of class es to be debugged.
    static char* debugClassesText;
    /// translate index of debug module into index of all modules
    static int* transDebugModules;
    /// names of all modules
    static const char* allModuleNames[];
    /// the debug levels for all modules
    static int allModuleLevels[];
};

///Module: {\bf raslib}.

/**
RMTimer is not strictly part of RasLib. It is a class used for taking
timing measurements if configuring with --benchmark-enabled. One way
of using it is to put the following at the beginning of a function:

{\tt RMTIMER("className", "functionName");}

If RMANBENCHMARK is defined this is expanded to:

{\tt RMTimer localRMTimer = RMTimer("className", "functionName");}

Time is taken between this line and exiting the block where this line
was. For more elaborate timing measurements an RMTimer object can be
used directly. All timing information is stored in the object, so
multiple RMTimer objects can be used at the same time.

If output is generated on RMInit::bmOut depends on the flag {\tt
output} and the benchmark level. Output is generated if {\tt output}
is TRUE and {\tt bmLevel} is lower than the global benchmark level
stored in RManBenchmark. The flag {\tt output} can be changed with
setOutput(). The function start() sets {\tt output} to TRUE, stop()
sets {\tt output} to FALSE.

{\bf Important}: If a RMTimer is used as a static variable, it must be
ensured that no output is generated in the destructor either by
calling stop() or by manually setting {\tt output} to FALSE using
setOutput() before termination of the program. The reason is that
potentially RMInit::bmOut may be destructed before the RMTimer
destructor is called possibly causing a crash.
*/

/**
  * \ingroup RMs
  */

class RMTimer : public RM_Class
{
public:
    /// constructor, initializes members and starts timer.
    inline RMTimer(const char* newClass, const char* newFunc,
                   int newBmLevel = 0);
    /**
      The parameters newClass and newFunc have to be string literals. Just
      a pointer to them is stored. No output is generated if RManBenchmark
      < newBmLevel.
    */
    /// destructor, calls stop().
    inline ~RMTimer();
    /// switch output on RMInit::bmOut on and off.
    inline void setOutput(int newOutput);
    /**
      If newOutoutput is FALSE no output is created on RMInit::bmOut on
      the following calls to stop() and ~RMTimer() until the next start().
    */
    /// pauses timer.
    inline void pause();
    /// resumes timer.
    inline void resume();
    /// resets timer.
    inline void start();
    /**
      Also switches output to RMInit::bmOut on again.
    */
    /// prints time spent if output is TRUE.
    inline void stop();
    /**
      Time spent is the time since construction or last start() excluding
      the times between pause() and resume().
    */
    /// delivers current time count.
    inline int getTime();

private:
    /// name of class.
    const char* myClass;
    /// name of function (no parameters).
    const char* myFunc;
    /// flag, if stop() should print timing information
    int output;
    /// stores benchmark level, checked before output.
    int bmLevel;
    // reference parameter for gettimeofday().
#ifdef __VISUALC__
    time_t acttime;
#else
    timeval acttime;
#endif
    /// accu for saving time in us
    long accuTime;
    /// flag indicating if the timer is currently running.
    unsigned short running;
    // reference parameter for gettimeofday, not used.
    static struct timezone dummy;
    /// used to calculate time spent in function.
    long oldsec;
    /// used to calculate time spent in function.
    long oldusec;
    /// aux function to determine clock time elapsed so far.
    inline void fetchTime();
};
///Module: {\bf raslib}.

/**
Objects of this class increment the indent level of RMDebug at construction time and decrease this level at destruction time.
*/

/**
  * \ingroup RMs
  */

class RMCounter : public RM_Class
{
public:
    /// constructor, increments indent level if the class should be debugged.
    RMCounter(int levell, int module, const char* cls);
    /// destructor, decrements indent level.
    ~RMCounter();
private:
    bool doStuff;
};

#include "raslib/rmdebug.icc"


#endif
