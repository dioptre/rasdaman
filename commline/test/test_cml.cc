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
*/

static const char rcsid[] = "@(#)commline,test_cml.cc: $Id: test_cml.cc,v 1.3 2005/12/21 23:02:06 rasdev Exp $";

#include "cmlparser.hh"

using std::cout;
using std::endl;

//set to 1 for testing invalid options
const int TESTOPT=0;

int
main(int argc, char** argv)
{
    CommandLineParser& cmlInter = CommandLineParser::getInstance();

    if(TESTOPT)
        cout<<"------ Defining Test options -----"<<endl;

    //defines options
    try
    {
        CommandLineParameter& a = cmlInter.addFlagParameter('r',"resistent", "r desc");
        CommandLineParameter& b = cmlInter.addFlagParameter('g',"ab", "ab desc");
        CommandLineParameter& c = cmlInter.addStringParameter('f',"file", "<filename f desc");
        CommandLineParameter& d = cmlInter.addStringParameter('s',"server", "server> s desc");
        CommandLineParameter& e = cmlInter.addStringParameter(CommandLineParser::noShortName,"ts", "<ts> test -");
        CommandLineParameter& f = cmlInter.addStringParameter('t', CommandLineParser::noLongName, "<tl> test --", "test --");

        if(TESTOPT)
            CommandLineParameter& g = cmlInter.addStringParameter(CommandLineParser::noShortName, CommandLineParser::noLongName, "<tl> - --");

    }
    catch(CmlException& e)
    {
        cout << "Erorr:" << e.what() << endl;
        return 1;
    }

    cout<<"------ Test options -----"<<endl;
    cmlInter.printHelp();

    cout<<"------ Correct Start -----"<<endl;
    cmlInter.testProcessCommandLine("-r --ab --file testFile -s serverName");
    cmlInter.testProcessCommandLine("-r --ab --file testFile              ");
    cmlInter.testProcessCommandLine("-r --ab                 -s serverName");
    cmlInter.testProcessCommandLine("-rf testFile -s serverName");
    cmlInter.testProcessCommandLine("-rgf testFile -s serverName");
    cmlInter.testProcessCommandLine("   --ab --file testFile -s serverName");
    cmlInter.testProcessCommandLine("--resistent");
    cmlInter.testProcessCommandLine("--file testFile --server serverName");
    cmlInter.testProcessCommandLine("-r --ab --file testFile -s -1");
    cmlInter.testProcessCommandLine("-r --ab --file test -s \"serverName\"");
    cmlInter.testProcessCommandLine("");
    cmlInter.testProcessCommandLine(" ");
    cmlInter.testProcessCommandLine("	");

    cout<<"------ Multiple Options Start -----"<<endl;
    cmlInter.testProcessCommandLine("-f F1 -f F2 --file F3 --file F4");

    cout<<"------ Errors Start -----"<<endl;
    cmlInter.testProcessCommandLine("-r -r --resistent --resistent");   // multiple flags not allowed
    cmlInter.testProcessCommandLine("-r --ab --file testFile -s");
    cmlInter.testProcessCommandLine("-r --ab -file testFile");
    cmlInter.testProcessCommandLine("-r --ab --file -s serverName");
    cmlInter.testProcessCommandLine("r --ab --file testFile -s serverName");
    cmlInter.testProcessCommandLine("-r -ab --file testFile -s serverName");
    cmlInter.testProcessCommandLine("-r ab --file testFile -s serverName");
    cmlInter.testProcessCommandLine("-rfs testFile serverName");
    cmlInter.testProcessCommandLine("- r");
    cmlInter.testProcessCommandLine("-");
    cmlInter.testProcessCommandLine("--");
    cmlInter.testProcessCommandLine("-r --");
    cmlInter.testProcessCommandLine("-r -- ab --file testFile -s serverName");

    cout<<"------ Test End -----"<<endl;

    return 0;
}
