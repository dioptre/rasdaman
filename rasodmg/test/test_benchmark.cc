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
 * SOURCE: test_benchmark.cc
 *
 * MODULE: rasodmg
 *
 * COMMENTS:
 *			None
*/

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include <iostream>
#include <fstream.h>
#include <string.h>

#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/set.hh"
#include "rasodmg/marray.hh"
#include "rasodmg/iterator.hh"
#include "rasodmg/oqlquery.hh"

#include "raslib/type.hh"
#include "raslib/shhopt.h"
#include "raslib/rmdebug.hh"

#include <sys/time.h>

static int numExec = 1;  // number of repetitions for each query
static int readEach = 0; // number of repeated queries in query file  
static int qNum = 0;     // counter for current query

RMTimer execTimer("test_benchmark", "r_oql_execute()");
RMTimer wholeTimer("test_benchmark", "execQuery()");

int checkArguments( int argc, char** argv, const char* searchText, 
		    int& optionValueIndex )
{
  int found = 0;
  int i=1;

  while( !found && i<argc )
    found = !strcmp( searchText, argv[i++] );   

  if( found && i<argc && !strchr(argv[i],'-') )
    optionValueIndex = i;
  else
    optionValueIndex = 0;
  
  return found;
}

void execQuery( char* serverName, char* baseName, char* queryBuffer )
{
}

static void 
printUsage(void)
{
  cout << "Usage: test_benchmark [options] server_name base_name file_name" 
       << endl;
  cout << "  -h, --help         Print this message and exit." << endl;
  cout << "  -n, --nrepeat      Followed by number of repeated" << endl
       << "                     executions for each query in file_name."
       << endl;
  cout << "  -e, --readeach     Followed by number of queries read from" 
       << endl
       << "                     the query file treated as variations of" 
       << endl
       << "                     the same query." << endl;
  exit(0);
}

void execQuery(char* serverName, char* baseName, char* comment, char* query)
{
  int i;
  r_Database db;
  r_Transaction ta;

  RMInit::bmOut << comment;
  RMInit::bmOut << "Query " << (qNum/numExec)+1 << "." << (qNum%numExec)+1
		<< ": " << query;
  qNum++;

  wholeTimer.start();

  r_Set< r_Ref_Any > image_set;
  db.set_servername( serverName );

  try
    {
      r_OQL_Query q1( query );

      db.open( baseName );

      if( q1.is_update_query() )
        ta.begin();
      else
        ta.begin( r_Transaction::read_only );
  
      // here the query is executed and times are taken

      try
	{
	  execTimer.start();
	  if( q1.is_update_query() )
	    r_oql_execute( q1 );
	  else
	    r_oql_execute( q1, image_set );
	  execTimer.stop();
	}

      catch( r_Error& errorObj )
	{
	  execTimer.stop();
	  cerr << endl << "QUERY FAILED" << endl << errorObj.what() << endl;
	  ta.commit();
	  db.close();
	}

      ta.commit();
      db.close();

      wholeTimer.stop();
    }
  catch( r_Error& errorObj )
    {
      wholeTimer.stop();
      cerr << endl << "FAILED" << endl << errorObj.what() << endl;
    }
}

void execQueries(char* serverName, char* baseName, char* comment, char* query)
{
  if(!readEach) {
    for(int i = 0; i < numExec; i++)
      execQuery(serverName, baseName, comment, query);
  }
  else {
    execQuery(serverName, baseName, comment, query);
  }
}

void parseFile(ifstream& fileStream, char* serverName, char* baseName)
{
  enum legalStates { FIRSTCOMMENT, COMMENT, QUERY };
  legalStates parseState = FIRSTCOMMENT;
  ostrstream commentStream; 
  ostrstream queryStream;
  char buf[256];
  char dummy;

  while( fileStream.get( buf, 255, '\n' ) ) {
    // read end of line
    fileStream.get(dummy);
    // checking for empty line
    if(buf[0] == 0) {
      if(parseState == FIRSTCOMMENT) {
	// end of first comment, print it and switch to normal comment
	commentStream << ends;
	RMInit::bmOut << commentStream.str() << endl;
	commentStream.rdbuf()->freeze(0);
	commentStream.seekp(0, ios::beg);
	parseState = COMMENT;
      }
    }
    // checking for comment
    else if(buf[0] == '/' && buf[1] == '/') {
      if(parseState == QUERY) {
	// execute the query, reset buffers
	queryStream << endl << ends;
	commentStream << ends;
	execQueries(serverName, baseName, commentStream.str(), 
		    queryStream.str());
	queryStream.rdbuf()->freeze(0);
	queryStream.seekp(0, ios::beg);
	commentStream.rdbuf()->freeze(0);
	commentStream.seekp(0, ios::beg);
	parseState = COMMENT;
      }
      commentStream << buf << endl; 
    } 
    // must be a query
    else {
      parseState = QUERY;
      queryStream << buf;
      queryStream << " ";
    }
  }
  // last query
  if(parseState == QUERY) {
    // execute the query, reset buffers
    queryStream << endl << ends;
    commentStream << ends;
    execQueries(serverName, baseName, commentStream.str(), 
		queryStream.str());
    queryStream.rdbuf()->freeze(0);
    queryStream.seekp(0, ios::beg);
    commentStream.rdbuf()->freeze(0);
    commentStream.seekp(0, ios::beg);
  }
}

int main( int argc, char** argv )
{   
  int  optionValueIndex; 
  char serverName[255];
  char baseName[255];
  char fileName[255];

  optStruct testBenchmarkOpt[] = {
    /* short long           type        var/func        special    */
    { 'h',   "help",        OPT_FLAG,   printUsage,     OPT_CALLFUNC },
    { 'n',   "nrepeat",     OPT_INT,    &numExec,       0 },
    { 'e',   "readeach",    OPT_INT,    &readEach,      0 },
    { 0, 0, OPT_END, 0, 0 }  /* no more options */
  };

  /* parse all options */
  optParseOptions(&argc, argv, testBenchmarkOpt, 0);

  if( argc < 4 )
    printUsage();

  if(readEach)
    numExec = readEach;

  strcpy( serverName, argv[argc-3] );
  strcpy( baseName, argv[argc-2] );
  strcpy( fileName, argv[argc-1] );

  ifstream fileStream( fileName );
  if( !fileStream )
  {
    cout << "Error: File not found." << endl;
    return -1;
  }

  parseFile(fileStream, serverName, baseName);
  return 0;
}
