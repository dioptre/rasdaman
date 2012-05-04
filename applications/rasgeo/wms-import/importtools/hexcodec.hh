/*************************************************************
 *
 * Copyright (C) 2003 Dr. Peter Baumann
 *
 * SOURCE: hexcodec.hh
 *
 * MODULE: rasgeo
 *
 * PURPOSE:
 *
 * CHANGE HISTORY (append further entries):
 * when         who    what
 * ----------------------------------------------------------
 * 2003-jul-10  PB     cloned from exportutils
 * 2003-dec-05  PB     file renamed from rasgeo_error.?? to export_error.??
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef __HEXCODEC_HH__
#define __HEXCODEC_HH__

#include <string>
#include <iosfwd>

using std::string;
using std::ostream;

#include "mymalloc/mymalloc.h"
#include "import_error.hh"

class HexCodec
{
 public:
   static string convertTo(const string& figureStr) throw(ImportError);
   static string convertFrom(const string& hexaStr) throw(ImportError);
   void printStatus(ostream& s);
 
 private:
   //class members
   static char hexVal[16];
   static const string hexFig;
   static const string hexId;
   static const string emptyStr;
   static const char   hexBase;
   static const char   hexPerByte;   
   static string hexStr;
   static string figStr;
   
};

#endif
