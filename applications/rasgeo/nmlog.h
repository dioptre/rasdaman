/*
 * name: nmlog.h
 * author: Alexander Herzig
 *
 * last updated: 08/01/11
 *
 * version 0.3.0
 *
 */

#ifndef NMLOG_H_
#define NMLOG_H_

#include <string>
#include <sstream>
#include <iostream>

namespace nmlog
{
extern int nmindent;
}

// DEBUG MACROs
#ifdef DEBUG
#define NMDebug(arg)  \
        { \
            std::ostringstream str; \
            str arg; \
            std::cout << str.str(); \
        }

#define NMDebugAI(arg) \
        { \
            std::ostringstream str; \
            for (int q=1; q <= nmlog::nmindent+1; q++) \
            {\
                str << "  "; \
            }\
            str arg; \
            std::cout << str.str(); \
        }

#define NMDebugInd(level, arg) \
        { \
            std::ostringstream str; \
            for (int q=1; q <= level; q++) \
            {\
                str << "  "; \
            }\
            str arg; \
            std::cout << str.str(); \
        }

#define NMDebugTimeInd(level, arg) \
        { \
            std::ostringstream str; \
            for (int q=1; q <= level; q++) \
            {\
                str << "  "; \
            }\
            str << __TIME__ << " - " arg; \
            std::cout << str.str(); \
        }

#define NMDebugTime(arg) \
        { \
            std::ostringstream str; \
            str << __TIME__ << " - " arg; \
            std::cout << str.str(); \
        }

#define NMDebugCtx(context, arg)  \
        { \
            std::string tmp;\
            std::ostringstream str; \
            str arg;\
            tmp = str.str();\
            str.str(""); \
            if (tmp == "...") \
                nmlog::nmindent++; \
            for (int q=1; q <= nmlog::nmindent; q++) \
            {\
                str << "--"; \
            }\
            str << context << "::" << \
                   __FUNCTION__ << ": " arg; \
            std::cout << str.str() << std::endl; \
            if (tmp == "done!") \
                nmlog::nmindent--; \
        }

#define NMDebugTimeCtx(context, arg)  \
        { \
            std::ostringstream str; \
            str << __TIME__ << ": " << context << "::" << \
                   __FUNCTION__ << ": " arg; \
            std::cout << str.str() << std::endl; \
        }
#else
#define NMDebug(arg)
#define NMDebugAI(arg)
#define NMDebugInd(level, arg)
#define NMDebugTime(arg)
#define NMDebugTimeInd(level, arg)
#define NMDebugCtx(ctx, arg)
#define NMDebugTimeCtx(ctx, arg)
#endif

// ERROR MACRO
#define NMErr(context, arg)  \
        { \
            std::ostringstream str; \
            str << "ERROR - " << context << "::" << \
                   __FUNCTION__ << ", l. " << \
                   __LINE__ << ": " arg; \
            std::cerr << str.str() << std::endl; \
        }

// MESSAGE MACRO
#define NMMsg(arg) \
        { \
            std::ostringstream str; \
            str arg; \
            std::cout << str.str() << std::endl; \
        }

#endif /* NMLOG_H_ */
