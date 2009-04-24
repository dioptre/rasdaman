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
 * INCLUDE: cmlparser.hh
 *
 * MODULE:  command line interpreter
 * 
 * PURPOSE:
 *   	Tool for interpreting command line arguments
 *
 * COMMENTS:
 *			None
*/


#ifndef AK_CMLPARSER_HH
#define AK_CMLPARSER_HH



#include <list>
#include <string>
#include <exception>
#include <iostream>

using std::string;
using std::list;
using std::ostream;
using std::cout;

// Command Line Parser version
extern const char* CommandLineParserVersion;

// specific errors thrown by the parser
class CmlException : public std::exception
  {
    public:
      explicit CmlException(const string& whatString);
      virtual ~CmlException() throw();
      virtual  const char* what() const throw();
      
    protected:  
      string  problem;
   };


class CommandLineParameter
  {
    public:

     static const char* defaultTitle;
     static const char* descSep;     
     static const char* descTab;
     static const char* descIndent;
     static const char* descLineSep;
     static const char  descOpen;
     static const char  descClose;
     static const char* descLeftDefault;
     static const char* descRightDefault;

     virtual ~CommandLineParameter(); 
     
     //interface for Parser
     void setDescription(const char*);
     
     bool doesMatch(char c);
     bool doesMatch(const char* s);
     
     char         getShortName() const;
     const char*  getLongName() const;
     
     virtual bool setPresent(char c) throw(CmlException) = 0;
     virtual bool setPresent(const char* s) throw(CmlException) = 0;
    
     virtual bool needsValue() = 0;
     virtual bool takeValue(const char* s) = 0;
     virtual void popValue() = 0;
     
     void virtual reset();
     const char *calledName();     
     
     // has a (at least one) value been assigned?
     virtual bool isPresent() = 0;

     virtual const char* getValueAsString() throw(CmlException) = 0;
     virtual long        getValueAsLong()   throw(CmlException) = 0;
     virtual double      getValueAsDouble() throw(CmlException) = 0;

     virtual ostream& printStatus(ostream& = cout) = 0;
     ostream& printHelp(ostream& = cout);

    protected:
      CommandLineParameter(char newShortName, const char* newLongName, const char* newDefaultValue) throw(CmlException);
      CommandLineParameter(char newShortName, const char* newLongName, long newDefaultValue) throw(CmlException);
      
    protected:
    
      char  shortName;
      char* longName;
      bool  present;
      bool  wasLongName;
      
      char* defaultValue;
      char  shNameString[2];
      
      char *descriptionText; 
      char *paramDescription;
   };


class FlagParameter: public CommandLineParameter
  {
   public:
     FlagParameter(char nShortName, const char*nLongName) throw(CmlException);

     bool setPresent(char c) throw(CmlException);
     bool setPresent(const char* s) throw(CmlException);
     bool isPresent();
    
     bool needsValue();
     bool takeValue(const char* s);
     void popValue();
     
     const char* getValueAsString() throw(CmlException);
     long        getValueAsLong()   throw(CmlException);
     double      getValueAsDouble() throw(CmlException);
     
     ostream& printStatus(ostream& = cout);
   };

class StringParameter: public CommandLineParameter
  {
   private:
      list<char*> value;
   
   public:
     StringParameter(char nShortName, const char* nLongName, const char *newDefaultValue = NULL) throw(CmlException);
     StringParameter(char nShortName, const char* nLongName, long newDefaultValue = 0L) throw(CmlException);
     ~StringParameter();
      
     bool setPresent(char c) throw(CmlException);
     bool setPresent(const char* s) throw(CmlException);
     bool isPresent();
    
     bool needsValue();
     bool takeValue(const char* s);
     void popValue();
     
     const char* getValueAsString() throw(CmlException);
     long        getValueAsLong()   throw(CmlException);
     double      getValueAsDouble() throw(CmlException);
     
     void reset();
     
     ostream& printStatus(ostream& = cout);
    
   };


class CommandLineParser
  {
   public:
     static const char noShortName;
     static const char* noLongName;     
     static const char* ShortSign;
     static const char* LongSign;
     
    
     static CommandLineParser& getInstance();
     
     ~CommandLineParser();

	/*
	These functions take a parameter called description. This is a string used in printHelp
	The format of this string has to be:
		<name of parameter> line1\n\t\tline2...\n\t\tlineN
		brackets<> and space after are mandatory if there is a parameter!
		Otherwise no <>!
	*/
     CommandLineParameter& addFlagParameter(char shortName, const char* longName, const char* description) throw(CmlException);
     CommandLineParameter& addStringParameter(char shortName, const char* longName,  const char* description, const char *newDefaultValue = NULL) throw(CmlException);
     CommandLineParameter& addLongParameter(char shortName, const char* longName,  const char* description, long newDefaultValue = 0L ) throw(CmlException);

     bool isPresent(char shortName) throw(CmlException);
     bool isPresent(const char* longName) throw(CmlException);

     const char* getValueAsString(char shortName) throw(CmlException);
     long        getValueAsLong(char shortName)   throw(CmlException);
     double      getValueAsDouble(char shortName) throw(CmlException);
     
     const char* getValueAsString(const char* longName) throw(CmlException);
     long        getValueAsLong(const char* longName)   throw(CmlException);
     double      getValueAsDouble(const char* longName) throw(CmlException);

     void processCommandLine(int argc, char** argv) throw(CmlException);

     bool testProcessCommandLine(const char* test_cml);
     
     void printHelp();
     void printStatus();

   private:
     static CommandLineParser* myself;
      
     list<CommandLineParameter*> cmlParameter;

     CommandLineParameter *lastParameter;
     bool nextTokenIsValue;

     CommandLineParser();

     CommandLineParameter& getParameter(char shortName) throw(CmlException);
     CommandLineParameter& getParameter(const char* longName) throw(CmlException);

     void setValue(const char* value) throw(CmlException);

     void longNameParameter(const char* nextToken) throw(CmlException);

     void shortNameParameter(const char* nextToken) throw(CmlException);
   };
   
#endif   
