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
 * SOURCE: test_interestrtiling.cc
 *
 * MODULE: rasodmg
 *
 * PURPOSE: test class r_StatTiling.
 *
 * COMMENTS:
 * 		None
*/

/* 
   ATENTION: The format of the input file for using with this program is:

     border_threshold accesses_threshold tilesize domain
     access1
     access2
     accessXXX
     ...

   Example:
   
     50 0.20 1000 [0:799, 0:599]
     [10:20, 30:40]
     [12:20, 35:39]
     [100:300, 300:400]
     [120:300, 310:410]
     [200:500, 350:500]
     [200:510, 350:500]
*/


#include <iostream>
#include <stdio.h>
#include "rasodmg/marray.hh"
#include "rasodmg/tiling.hh"
#include "raslib/dlist.hh"
#include "rasodmg/stattiling.hh"
#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"

char* filename;
DList<r_Access> stat_info;
unsigned int border_threshold;
double interesting_threshold;
unsigned long tile_size;
r_Minterval* domain;

void parse(int argc, char* argv[])
{
  if (argc != 2)
  {
    cout << "Usage: test_stattiling <filename>" << endl << endl;
    exit(1);
  }

  filename = argv[1];
}

void read_data()
{
  const unsigned int BUF_SIZE = 256;
  char buf[BUF_SIZE], buf2[BUF_SIZE];

  cout << "Opening " << filename << " for reading... ";

  ifstream is(filename, ios::in);
  if (!is)
  {
    cout << "Couldn't open!!!" << endl;
    exit(1);
  }
  else
    cout << "done." << endl;

  cout << "Reading parameters... ";

  is >> border_threshold;
  is >> interesting_threshold;
  is >> tile_size;

  is.getline(buf, BUF_SIZE);
  domain = new r_Minterval(buf);

  cout << "done." << endl;
  cout << "Geting the accesses... ";

  unsigned long count = 0;

  while (!is.eof())
  {
    is.getline(buf, BUF_SIZE);
    if (sscanf(buf, "%s", buf2) == 1)
    {
      r_Minterval inter(buf);    
      stat_info += inter;
      ++count;
    
      cout << "*";
    }
  }

  is.close();

  cout << endl;
  cout << "Geting the accesses... done." << endl << endl;

  cout << "Border threshold      = " << border_threshold << endl;
  cout << "Interesting threshold = " << interesting_threshold << endl;
  cout << "Tile size             = " << tile_size << endl;
  cout << "Domain                = " << *domain << endl;
  cout << "Number of accesses    = " << count << endl << endl;
}

void test_tiling()
{
  r_Stat_Tiling tiling(border_threshold, interesting_threshold, tile_size);
  tiling.update_stat_information(stat_info);

  DList<r_Minterval>* tiles = tiling.compute_tiles(*domain, 1);

  cout << endl << "Tiles: " << endl;
  while (!tiles->is_empty())
  {
    r_Minterval inter = tiles->get_first(TRUE);
    cout << "  " << inter << endl;
  }

  delete tiles;
  delete domain;
}

int main(int argc, char* argv[])
{
  parse(argc, argv);
  read_data();
  test_tiling();

  return 0;
}











