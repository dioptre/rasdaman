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
 * SOURCE: system_basic.cc
 *
 * MODULE: rasodmg/test
 *
 * COMMENTS:
 *      None
*/

static const char rcsid[] = "@(#)rasodmg/test,rasql: $Id: rasql.cc,v 1.5 2002/05/25 14:29:15 coman Exp $";

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "readln.hh"
#include "rasql.hh"

static bool noMore = false;


void logo()
{
    std::cout << "RasQL v0.1, (c) 2001 active knowledge GmbH." << std::endl;
    if(noMore)
        std::cout << "Option: no more" << std::endl;
    std::cout << std::endl;
}

bool toFile(char *s)
{
    std::ofstream os(file_name);
    if(!os)
        return false;
    os << s;
    return true;
}

void filter(void)
{
    execlp(filter_prog, filter_prog, filter_params, 0);
    perror("exec");
    exit(1);
}


void query(void)
{
    execlp(query_prog, query_prog,
           server_param, server_name,
           base_param, base_name,
           file_param, file_name,
           0);
    perror("exec");
    exit(1);
}

bool process(char* s)
{
    int p[2];
    if(strcmp(s, "exit") == 0)
        return false;
    if(strcmp(s, "quit") == 0)
        return false;

    if(!toFile(s))
    {
        std::cerr << "toFile: error opening file " << file_name << std::endl;
        return true;
    }

    if(noMore)
    {
        if(!fork())
            query();
    }
    else
    {
        pipe(p);
        if(!fork())
        {
            close(p[1]);
            dup2(p[0], 0);
            filter();
        }
        else if(!fork())
        {
            close(p[0]);
            dup2(p[1], 1);
            query();
        }
        else
        {
            close(p[0]);
            close(p[1]);
        }
    }

    while(wait(0) > 0);
    return true;
}

int main(int argc, char **argv)
{
    if(--argc && strcmp(*(++argv), "-nomore") == 0)
        noMore = true;

    logo();

    ReadLn l(process, "RasQL> ");
    l.loop();
    std::cout << std::endl;
    return 0;
}