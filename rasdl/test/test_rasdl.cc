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
// #include <conio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "rasdl/symbtbl.hh"
#include "rasdl/yparse.hh"

extern int             yyparse();
extern FILE            *yyin;
extern YSymbolTable    *Symbols;

int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        cerr<<"USAGE: odl odl-script cpp-header";
        exit(-1);
    };

    cout<<"--BEGIN-------------------------------------------------------------------------";
    cout<<"opening \""<<argv[1]<<"\"...";
    yyin=fopen(argv[1],"r");

    if(yyin==NULL)
    {
        cout<<"fail!\n";
        cerr<<"\nfailed to open "<<argv[1]<<".\n";

        exit(-1);
    };

    cout<<"ok!\n";

    cout<<"parsing...";
    yyparse();
    cout<<"done!\n";

    cout<<"output...";

    FILE   *file=fopen(argv[2],"wt");

    /* header description */
    fprintf(file,"//------------------------------------------------------------\n");
    fprintf(file,"//  automatic created C++-header                               \n");
    fprintf(file,"//------------------------------------------------------------\n");
    fprintf(file,"\n");

    Symbols->global_scope->output(file);

    fclose(file);

    cout<<"done!\n";
    cout<<"--END---------------------------------------------------------------------------";

    return(0);
};

