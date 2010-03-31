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
 * SOURCE: cmlparser.cc
 *
 * MODULE: akinside/commline
 *
 * PURPOSE:
 *
 * COMMENTS:
 *		None
*/


#include <cmlparser.hh>

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include <ctype.h>
#include <vector>
#include <iomanip>
#include <cstdlib>

using namespace std;

static const char rcsid[] = "@(#)commline,cmlparser: $Id: cmlparser.cc,v 1.5 2006/01/17 07:50:07 rasdev Exp $";

const char* CommandLineParserVersion = "1.0 (c) 2003 Dr. Peter Baumann";

//#############################################################

char*
dupString(const char* cc)
{
	if(cc)
	{
		// strdup uses malloc, we would like to avoid it
		char *dup = new char[strlen(cc)+1];
		// new throws? we hope
		strcpy(dup,cc);
		return dup;
	}

	return NULL;
}
   
CmlException::CmlException(const string& whatString)
:problem(whatString)
{
}
   
CmlException::~CmlException() throw()
{
}
   
const char* CmlException::what() const throw()
{
	return problem.c_str();
}   
   
   
//############################################################

const char* CommandLineParameter::defaultTitle     = "default: ";
const char* CommandLineParameter::descSep          = " ";
const char* CommandLineParameter::descTab          = "\t";
const char* CommandLineParameter::descIndent       = "       ";
const char* CommandLineParameter::descLineSep      = "\n";
const char  CommandLineParameter::descOpen         = '<';
const char  CommandLineParameter::descClose        = '>';
const char* CommandLineParameter::descLeftDefault  = "(";
const char* CommandLineParameter::descRightDefault = ")";



CommandLineParameter::CommandLineParameter(char newShortName, const char* newLongName, const char* newDefaultValue) throw(CmlException)
  : longName(NULL), defaultValue(NULL), descriptionText(NULL), paramDescription(NULL)
{
  
	if((!isalnum(newShortName) || (newShortName == CommandLineParser::noShortName))
	&& (!newLongName || !strcmp(newLongName, CommandLineParser::noLongName)))
		throw CmlException(string("") + "Invalid option: shortName='" + newShortName + "' longName='" + (newLongName? newLongName: "NULL") + "'");
  
	shortName    = newShortName;
	longName     = dupString(newLongName);
	present      = false;
	wasLongName  = false;
	defaultValue = dupString(newDefaultValue);
	
	shNameString[0] = newShortName;
	shNameString[1] = 0;
	
	paramDescription = NULL;
	descriptionText  = NULL;
}

CommandLineParameter::CommandLineParameter(char newShortName, const char* newLongName, long newDefaultValue) throw(CmlException)
  : longName(NULL), defaultValue(NULL), descriptionText(NULL), paramDescription(NULL)
{
  
	if((!isalnum(newShortName) || (newShortName == CommandLineParser::noShortName))
	&& (!newLongName || !strcmp(newLongName, CommandLineParser::noLongName)))
		throw CmlException(string("") + "Invalid option: shortName='" + newShortName + "' longName='" + (newLongName? newLongName: "NULL") + "'");
  
	shortName    = newShortName;
	longName     = dupString(newLongName);
	present      = false;
	wasLongName  = false;

	// convert default to ASCII, for uniform handling
        stringstream s;
        s << newDefaultValue;
	defaultValue = dupString( s.str().c_str() );

	shNameString[0] = newShortName;
	shNameString[1] = 0;
	
	paramDescription = NULL;
	descriptionText  = NULL;
}

const char* CommandLineParameter::calledName()
{
	const char* retval=NULL;
	
	if(shortName == CommandLineParser::noShortName)
		retval = longName;
	else
		if(!strcmp(longName, CommandLineParser::noLongName))
			retval = shNameString;
		else
			retval = (wasLongName && longName) ? longName : shNameString;
	return retval;
}

void CommandLineParameter::reset()
{
	present=false;
	wasLongName=false;
}


CommandLineParameter::~CommandLineParameter()
{
	if(longName)
		delete[] longName;
	
	if(defaultValue)
		delete[] defaultValue;
	
	// paramDescription and descriptionText point both to the same string
	// but depends on the context which has to be deleted
	if(paramDescription) 
	{
		delete[] paramDescription;
		paramDescription = NULL;
		descriptionText = NULL;
	}
	if(descriptionText)
		delete[] descriptionText;
}

void CommandLineParameter::setDescription(const char *desc)
{
	// cleaning the previous paramDescription
	// paramDescription and descriptionText point both to the same string
	// but depends on the context which has to be deleted
	if(paramDescription) 
	{
		delete[] paramDescription;
		paramDescription = NULL;
		descriptionText = NULL;
	}
	if(descriptionText) delete[] descriptionText;

	if(desc==NULL) 
	{
		paramDescription=NULL;
		descriptionText =NULL;
	}	
	else
	{	
		paramDescription = dupString(desc);
	
		// if the string is "<text> line1...", for options with parameters
		if(paramDescription[0]==descOpen)
		{
			int len = strlen(paramDescription);
			int i=0;
			for(i=0;i<len;i++)
			{
				if((paramDescription[i]==descClose) && (i<len-1))
				{
					paramDescription[i+1]=0;
					descriptionText = paramDescription+i+2;
					break;
				}
		  	}
		  
			if(i>=len-1) // I didn't find descClose
			{
				descriptionText=paramDescription;
				paramDescription=NULL;
			}
		}  
	 	else
		{
			// the string is just "line1..." for options without parameters
			descriptionText = paramDescription;
			paramDescription = NULL;
		}
	}
}
   
bool CommandLineParameter::doesMatch(char c)
{
	if(shortName==CommandLineParser::noShortName)
		return false;
	return shortName==c ? true:false;
}

bool CommandLineParameter::doesMatch(const char* s)
{
	if(longName==NULL)
		return false;
	if(s == NULL)
		return false;
	if(!strcmp(longName, CommandLineParser::noLongName))
		return false;
	return strcmp(longName,s)==0 ? true:false;
}

char CommandLineParameter::getShortName() const
{
	return shortName;
}

const char* CommandLineParameter::getLongName() const 
{
	return longName;    
}

/// print help text, composed from the option descriptions passwd to cml 
ostream& CommandLineParameter::printHelp(ostream &os)
{
	if(descriptionText == NULL)
		return os;
	
	static const unsigned int longNameLen=30;
	static const unsigned int longParamLen=30;   
	
	os << CommandLineParameter::descIndent;

	if(isalnum(shortName) && (shortName != CommandLineParser::noShortName))
		os<< CommandLineParser::ShortSign <<shortName;
	// we want a flush left display, don't adjust short/long params each -- PB 2003-jul-05
	// else
	//    os << "  ";
 
	if((isalnum(shortName) && (shortName != CommandLineParser::noShortName)) && 
	  (longName && strcmp(longName, CommandLineParser::noLongName)))
		os << ", ";
	// we want a flush left display, don't adjust short/long params each -- PB 2003-jul-05
	// else
	//    os << "  ";

	// we want the parameter to appear right after the option, no adjustment -- PB 2003-jul-
	// os.setf(ios::left);
	// os << setw(longNameLen);
	
	if(longName && strcmp(longName, CommandLineParser::noLongName))
	{
		string s;
		s += CommandLineParser::LongSign;
		s += longName;
		os << s.c_str();
	}
	else
		os << "";

	// separator between param name and value
	os << CommandLineParameter::descSep;
	
	// no adjustment, again
	// os.setf(ios::left);
	// os << setw(longParamLen);
	
	// param value, such as "<database>"
	if(paramDescription)
		os << paramDescription;
	
	// separator between param value and default
	if(defaultValue)
		os << CommandLineParameter::descTab;

	// print default, if any: "(default: 42)"
	if(defaultValue)
		os << CommandLineParameter::descLeftDefault << defaultTitle << defaultValue << CommandLineParameter::descRightDefault;

	// in a second line, print description text (2*indent)
	if(descriptionText)
		os << CommandLineParameter::descLineSep << CommandLineParameter::descIndent << CommandLineParameter::descIndent << descriptionText;
	
	os<<flush;

	return os;
}
  
//#####################################################################

FlagParameter::FlagParameter(char nShortName, const char* nLongName) throw(CmlException)
:CommandLineParameter(nShortName,nLongName,(const char*)NULL)
{
}

bool FlagParameter::needsValue()
{ 
	return false;
}

bool FlagParameter::takeValue(const char*)
{ 
	return false;
}

void FlagParameter::popValue()
{ 
}

bool FlagParameter::isPresent()
{
	return present;
}

bool FlagParameter::setPresent(char c)  throw(CmlException)
{
	if(c && c==shortName)
	{
		if(present==true)
		{
			if(wasLongName==true)
				throw CmlException(string("") + "Parameter '" + longName + "' clashes with parameter '" + shortName + "'");
			else
				throw CmlException(string("") + "Parameter '" + shortName + "' was already present");
		}
	 	present=true;
	 	return true;
	}
	return false;
}

bool FlagParameter::setPresent(const char* s) throw(CmlException)
{
	if(longName != NULL && !strcmp(longName,s))
	{
		if(present==true)
		{
			if(wasLongName==true)
				throw CmlException(string("") + "Parameter '" + longName + "' was already present");
			else
				throw CmlException(string("") + "Parameter '" + longName + "' clashes with parameter '" + shortName + "'");
		}
		wasLongName=true;
		present=true;
		return true;
	}
	return false;
}

const char* FlagParameter::getValueAsString() throw(CmlException)
{
	throw CmlException("Flag parameter can't return a string value");
}

long FlagParameter::getValueAsLong() throw(CmlException)
{
	throw CmlException("Flag parameter can't return a 'long' value");
}

double FlagParameter::getValueAsDouble() throw(CmlException)
{
	throw CmlException("Flag parameter can't return a 'double' value");
}

ostream& FlagParameter::printStatus(ostream &os)
{
	if(present)
		os<<"option '"<<calledName()<<"' present";
	else
		os<<"option '"<<calledName()<<"' missing";    
	
	return os;  
}
//#####################################################################
StringParameter::StringParameter(char nShortName, const char* nLongName, const char* newDefaultValue) throw(CmlException)
:CommandLineParameter(nShortName,nLongName,newDefaultValue)
{
	value.clear();
}

StringParameter::StringParameter(char nShortName, const char* nLongName, long newDefaultValue) throw(CmlException)
:CommandLineParameter(nShortName,nLongName,newDefaultValue)
{
	value.clear();
}

StringParameter::~StringParameter()
{
	reset();
}    
	
bool StringParameter::setPresent(char c)  throw(CmlException)
{
	if(c && c==shortName)
	{
	 	present=true;
	 	return true;
	}
	return false;
}

bool StringParameter::setPresent(const char* s) throw(CmlException)
{
	if(longName != NULL && !strcmp(longName,s))
	{
		wasLongName=true;
		present=true;
		return true;
	}
	return false;
}

bool StringParameter::isPresent()
{
	return ( ! value.empty() );
}

void StringParameter::reset()
{
	list<char*>::iterator iter = value.begin();
/* FIXME: free mem to avoid mem leaks
	while ( iter != value.end() )
	{
		if (*iter != NULL)
			delete[] *iter;
	}
*/
	value.clear();

	CommandLineParameter::reset();
}    
	
	
bool StringParameter::needsValue()
{ 
	return true;
}
		
bool StringParameter::takeValue(const char* s)
{
	char *aux = dupString(s);
	value.push_back( aux );
	return true;
}

void StringParameter::popValue()
{
	if ( ! value.empty() )
		value.pop_front();
}

const char* StringParameter::getValueAsString() throw(CmlException)
{
	return ( !value.empty() ? value.front() : defaultValue );
}

long   StringParameter::getValueAsLong()   throw(CmlException)
{
	const char *r = ( !value.empty() ? value.front() : defaultValue );
	
	if(r == NULL)
		throw CmlException(string("") + "No value for parameter '" + calledName() + "'");
	
	char *endptr;
	
	long result = strtol( r, &endptr, 0);
	
	if( *endptr != 0 )
		throw CmlException(string("") + "Invalid integer value for parameter '" + calledName() + "'");
	
	return result;
}
	
double StringParameter::getValueAsDouble() throw(CmlException)
{
	const char *r = ( !value.empty() ? value.front() : defaultValue );
	
	if(r == NULL)
		throw CmlException(string("") + "No value for parameter '" + calledName() + "'");
	
	char *endptr;
	
	double result = strtod( r, &endptr);
	
	if( *endptr != 0 )
		throw CmlException(string("") + "Invalid double value for parameter '" + calledName() + "'");
	
	return result;
}
		
ostream& StringParameter::printStatus(ostream &os)
{
	if(value.empty())
		os<<"option '"<<calledName()<<"' empty.";
	else
	{
		os<<"option '"<<calledName()<<"' present: ";
		list<char*>::iterator iter = value.begin();
		while ( iter != value.end() )
		{
			os<< "'" << (*iter ? *iter : "(null)") << "' ";
			iter++;
		}
	}
	return os;  
}

//##########################################################
const char CommandLineParser::noShortName = '-';
const char* CommandLineParser::noLongName = "--";
const char* CommandLineParser::ShortSign = "-";
const char* CommandLineParser::LongSign = "--";
	
CommandLineParser* CommandLineParser::myself = NULL;

CommandLineParser& CommandLineParser::getInstance()
{
	if(myself == NULL)
		myself = new CommandLineParser;
	return *myself;
}

CommandLineParser::CommandLineParser()
{
	lastParameter = NULL;
	nextTokenIsValue = false;
}

CommandLineParser::~CommandLineParser()
{
	delete myself;
	myself=NULL;
	
	lastParameter=NULL;
	
	list<CommandLineParameter*>::iterator iter = cmlParameter.begin();
	for(unsigned int i=0;i<cmlParameter.size();i++,iter++)
		delete *iter;
}


CommandLineParameter& CommandLineParser::addFlagParameter(char shortName, const char *longName, const char *description) throw(CmlException)
{
	CommandLineParameter *cp = new FlagParameter(shortName,longName);
	cp->setDescription(description);
	cmlParameter.push_back(cp);
	return *cp;
}

CommandLineParameter& CommandLineParser::addStringParameter(char shortName, const char* longName, const char *description, const char* newDefaultValue) throw(CmlException)
{
	CommandLineParameter *cp = new StringParameter(shortName,longName,newDefaultValue);
	cp->setDescription(description);
	cmlParameter.push_back(cp);
	return *cp;
}

CommandLineParameter& CommandLineParser::addLongParameter(char shortName, const char* longName, const char *description, long newDefaultValue) throw(CmlException)
{
	CommandLineParameter *cp = new StringParameter(shortName,longName,newDefaultValue);
	cp->setDescription(description);
	cmlParameter.push_back(cp);
	return *cp;
}

bool CommandLineParser::isPresent(char shortName) throw(CmlException)
{
	StringParameter *sp;
	FlagParameter *fp;
	bool result;

	CommandLineParameter &cml = getParameter(shortName);
	if ((sp = dynamic_cast<StringParameter*>(&cml)))
		result = sp->isPresent();
        else if ((fp = dynamic_cast<FlagParameter*>(&cml)))
		result = fp->isPresent();
	return result;
}

bool CommandLineParser::isPresent(const char* longName) throw(CmlException)
{
	StringParameter *sp;
	FlagParameter *fp;
	bool result;

	CommandLineParameter &cml = getParameter(longName);
	if ((sp = dynamic_cast<StringParameter*>(&cml)))
		result = sp->isPresent();
        else if ((fp = dynamic_cast<FlagParameter*>(&cml)))
		result = fp->isPresent();
	return result;
}

const char* CommandLineParser::getValueAsString(char shortName) throw(CmlException)
{
	CommandLineParameter &cml = getParameter(shortName);
	return cml.getValueAsString();
}
	
long CommandLineParser::getValueAsLong(char shortName) throw(CmlException)
{ 
	CommandLineParameter &cml = getParameter(shortName);
	return cml.getValueAsLong();
}
	
double CommandLineParser::getValueAsDouble(char shortName) throw(CmlException)
{
	CommandLineParameter &cml = getParameter(shortName);
	return cml.getValueAsDouble();
}
	 
const char* CommandLineParser::getValueAsString(const char* longName) throw(CmlException)
{
	CommandLineParameter &cml = getParameter(longName);
	return cml.getValueAsString();
}
	
long CommandLineParser::getValueAsLong(const char* longName)   throw(CmlException)
{
	CommandLineParameter &cml = getParameter(longName); 
	return cml.getValueAsLong();
}
	
double CommandLineParser::getValueAsDouble(const char* longName) throw(CmlException)
{
	CommandLineParameter &cml = getParameter(longName);
	return cml.getValueAsDouble();
}


void CommandLineParser::processCommandLine(int argc, char** argv) throw(CmlException)
{
	for(int i=1;i<argc;i++)
	{
		char *nextToken=argv[i];
		
//	bool sw1 = (nextToken[0]=='-') ? true:false;        // token starts with "-"
//	bool sw2 = (sw1 && nextToken[1]=='-') ? true:false; // token starts with "--"

		bool sw1 = (!strncmp(nextToken, ShortSign, strlen(ShortSign))) ? true:false; // token starts with ShortSign
		bool sw2 = (!strncmp(nextToken, LongSign, strlen(LongSign))) ? true:false;   // token starts with LongSign

		//check only for ShortSign/LongSign
		if(sw1 && !strcmp(nextToken, ShortSign))
			throw CmlException(string("") + "Syntax error: '" + ShortSign + "' with no parameter in command line");   	
		if(sw2 && !strcmp(nextToken, LongSign))
			throw CmlException(string("") + "Syntax error: '" + LongSign  + "' with no parameter in command line");   	
		
		if(nextTokenIsValue)	
			setValue(nextToken);
		else if(sw2) 
			longNameParameter(nextToken);
		else if(sw1) 
			shortNameParameter(nextToken);
		else
		{ 
			throw CmlException(string("") + "Syntax error: unexpected token '" + nextToken + "' in command line");
		}
	}
	if(nextTokenIsValue)
		throw CmlException(string("") + "Syntax error: missing value for parameter '" + lastParameter->calledName() + "' in command line");   
}


CommandLineParameter& CommandLineParser::getParameter(char shortName) throw(CmlException)
{
	list<CommandLineParameter*>::iterator iter = cmlParameter.begin();
	
	for(unsigned int i=0;i<cmlParameter.size();i++,iter++)
	{ 
		if( (*iter)->doesMatch(shortName) )   
		{
			return *(*iter);
		}
	}
	throw CmlException(string("") + "Syntax error: unknown parameter '" + shortName + "' in command line");   
}	
	
CommandLineParameter& CommandLineParser::getParameter(const char* longName) throw(CmlException)
{
	list<CommandLineParameter*>::iterator iter = cmlParameter.begin();
	
	for(unsigned int i=0;i<cmlParameter.size();i++,iter++)
	{ 
		if( (*iter)->doesMatch(longName) )   
		{
			return *(*iter);
		}
	}
	throw CmlException(string("") + "Syntax error: unknown parameter '" + longName + "' in command line");   
}
	

void CommandLineParser::setValue(const char* value) throw(CmlException)
{
	if(lastParameter == NULL)
	{
		throw CmlException("internal error: setValue - lastParameter==null");
	}
	lastParameter->takeValue(value);
	nextTokenIsValue=false;
}

void CommandLineParser::longNameParameter(const char *nextToken) throw(CmlException)
{
	const char* longName = nextToken + strlen(LongSign); // sari peste LongSign (former "--")

	CommandLineParameter &cml = getParameter(longName);
	cml.setPresent(longName);
	if(cml.needsValue())
	{
		lastParameter = &cml;
		nextTokenIsValue=true;
	}
}

void CommandLineParser::shortNameParameter(const char *nextToken) throw(CmlException)
{
	int tokenLength = strlen(nextToken);
	 
	for(int i=strlen(ShortSign);i<tokenLength;i++) //former i=1 for '-'
	{
		char shortName = nextToken[i];
		CommandLineParameter &cml = getParameter(shortName);
		cml.setPresent(shortName);
	 
		if(cml.needsValue())
		{
			if(i==tokenLength-1)
			{
				lastParameter = &cml;
				nextTokenIsValue=true;
			}
			else
			{
				const char *value = nextToken + i+1;
				cml.takeValue(value);
			}
			break;	    
		}
	}
	
}
	
void CommandLineParser::printHelp()
{
	std::list<CommandLineParameter*>::const_iterator iter=cmlParameter.begin();
	std::list<CommandLineParameter*>::const_iterator iterEnd=cmlParameter.end();
	for(; iter!=iterEnd; ++iter)
	{
		CommandLineParameter* ptr=*iter;
		ptr->printHelp( std::cout );
		std::cout << std::endl;
	}
	return;
}

void CommandLineParser::printStatus()
{
	std::list<CommandLineParameter*>::const_iterator iter=cmlParameter.begin();
	std::list<CommandLineParameter*>::const_iterator iterEnd=cmlParameter.end();
	
	for(; iter!=iterEnd; ++iter)
	{
		CommandLineParameter* ptr=*iter;
		ptr->printStatus( std::cout ) << std::endl;
	}
		
	std::cout <<std::endl;

}

bool CommandLineParser::testProcessCommandLine(const char *testCml)
{
	std::list<CommandLineParameter*>::const_iterator iter=cmlParameter.begin();
	std::list<CommandLineParameter*>::const_iterator iterEnd=cmlParameter.end();
	string test;
	string::size_type posStart, posEnd;
	const char* spaceSep = " \t\v\f\n\r";
	const char* prgName = "program";
	std::vector<std::string> argList;
	char  **argv=NULL;
	unsigned int argc=0, i=0, n=0;
	static unsigned int counter=1;
	 
	if(!testCml)
	{
		std::cout << "Error: test_cml is null" << endl;
		printStatus();
		return false;
	}
	 
	std::cout << "Test " << counter << " commandline='" << testCml << "'" << std::endl;
	 
	//converting to argc, argv
	test=testCml; 
	posEnd=0;
	while(true)
	{
		posStart=test.find_first_not_of(spaceSep, posEnd);
		if(posStart != string::npos)
		{
			posEnd=test.find_first_of(spaceSep, posStart);
			if(posEnd != string::npos)
			{
				std::cout << "arg " << argList.size() + 1 << "='" << test.substr(posStart, posEnd-posStart) << "'" << std::endl;	       
				argList.push_back(test.substr(posStart, posEnd-posStart));
			}
		 	else //last element
			{
				std::cout << "arg " << argList.size() + 1 << "='" << test.substr(posStart, test.size()-posStart) << "'" << std::endl;
				argList.push_back(test.substr(posStart, test.size()-posStart));
				break;
			}
		}
		else
  			break;
	}

	//build argv in C style       
	n=argList.size();
	argc = n + 1;
	argv=new char*[ argc ];
	argv[0] = dupString(prgName);
	for(i=0; i <  n; i++)
		argv[i + 1] = dupString(argList[i].c_str());

	std::cout << "Test " << counter++ << " output:" << std::endl;         
	
	try
	{
		//reset all options
		for(; iter!=iterEnd; ++iter)
		{
			(*iter)->reset();
		}
		//process commandline	
		processCommandLine(argc, argv);
	}
	catch(CmlException &e)
	{
		std::cout<<"Error: "<<e.what()<<std::endl;
		for(i=0; i < argc; i++)
			delete [] argv[i];
		delete[] argv;

		printStatus();
		return false;
	}
	
	for(i=0; i < argc; i++)
		delete [] argv[i];
	delete[] argv;
	
	printStatus();
	return true;
}

