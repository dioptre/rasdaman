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
 * PURPOSE: test class r_Interest_Tiling.
 *
 * COMMENTS:
 *			None
*/


#include <iostream>
#include <stdio.h>
#include "rasodmg/marray.hh"
#include "rasodmg/tiling.hh"
#include "rasodmg/dirtiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "rasodmg/interesttiling.hh"
#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"
#include "raslib/dlist.hh"

int main()
{
  r_Minterval domain(2);
  domain << r_Sinterval(0L, 500L) << r_Sinterval(0L, 600L);

  r_Minterval int1(2);
  int1 << r_Sinterval(0L, 100L) << r_Sinterval(0L, 500L);

  r_Minterval int2(2);
  int2 << r_Sinterval(200L, 400L) << r_Sinterval(100L, 200L);

  r_Minterval int3(2);
  int3 << r_Sinterval(250L, 450L) << r_Sinterval(150L, 250L);

  r_Minterval int4(2);
  int4 << r_Sinterval(300L, 500L) << r_Sinterval(400L, 550L);

  DList<r_Minterval> iareas;
  iareas += int1;
  iareas += int2;
  iareas += int3;
  iareas += int4;

  r_Interest_Tiling tiling(iareas, r_Interest_Tiling::REGROUP_AND_SUBTILING, 
			   50000);

  DList<r_Minterval>* tiles = tiling.compute_tiles(domain, 1);

  cout << "Domain: " << domain << endl << endl;
  cout << "Interest Areas: " << endl;
  DListIterator<r_Minterval> it_areas = iareas.create_iterator();
  for (; it_areas.not_done(); it_areas++)
  {
    r_Minterval inter = *it_areas;
    cout << "   " << inter << endl;
  }

  cout << "Tiles:  " << endl;

  DListIterator<r_Minterval> it = tiles->create_iterator();
  for (; it.not_done(); it++)
  {
    r_Minterval inter = *it;
    cout << "   " << inter << endl;
  }

  delete tiles;

  return 0;
}


