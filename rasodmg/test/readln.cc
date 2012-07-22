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

#include "readln.hh"

#include <stdlib.h>
#include <stdio.h>

#include <readline/readline.h>
#include <readline/history.h>

extern "C" int beep(int, int);

ReadLn::ReadLn(bool (*f)(char*), char* p, bool useTab): worker(f), prompt(p), line(0)
{
    if(!useTab)
        rl_bind_key((int)'\t', beep);
}

ReadLn::~ReadLn() {}

void ReadLn::loop()
{
    do
    {
        initLn();

        if((line = readline(prompt)) != 0  && *line)
        {
            add_history(line);
            if(!worker(line))
                break;
        }

    }
    while(line);
}

void ReadLn::initLn()
{
    if(line)
    {
        free(line);
        line = 0;
    }
}

int beep(int, int)
{
    putchar( '\a');
    return 1;   // meaningless, just for prototype
}