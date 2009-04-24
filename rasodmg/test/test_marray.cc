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
 * SOURCE: test_marray.cc
 *
 * MODULE: raslib
 *
 * COMMENTS:
 *		None
*/

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include <iostream>

#include "rasodmg/marray.hh"

#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"
#include "raslib/point.hh"
#include "raslib/error.hh"

static int INIT = 0;

int initFunction( const r_Point& /*pt*/ )
{
   /*
  int value=0;
  
  for( int i=0; i< pt.dimension(); i++ )
    value += pt[i];
  
  return value;
  */
   return INIT++;
}


int main()
{   
  cout << endl << endl;
  cout << "Marray Examples" << endl;
  cout << "===============" << endl << endl;
        
  cout << "Initialization of Marray<int, [0:2,3:5,6:8]> with init function:" << endl;
  r_Minterval domain(3);
  domain << r_Sinterval(0,2) << r_Sinterval(3,5) << r_Sinterval(6,8);
  r_Marray<int> a( domain, &initFunction );
  cout << "OK" << endl;
  a.print_status( cout );

  cout << "Value [0,3,6] = " << a[r_Point(0,3,6)] << endl;
  cout << "Changing to 99" << endl;
  a[r_Point(0,3,6)] = 99;
  cout << "New value [0,3,6] = " << a[r_Point(0,3,6)] << endl;
  
  cout << endl << "Projection operator:" << endl;
  cout << "project a[1]" << endl;
  r_Marray<int> b = a[1];
  b.print_status( cout );

  cout << endl << "Projection and cast operator a[r_Point(1,4,7)] :" << endl;
  cout << "a[1][4][7] = " << (int)(a[ r_Point(1,4,7) ]) << endl;
  
  cout << endl << "Projection and cast operator a[1][4][7] :" << endl;
  cout << "a[1][4][7] = " << (int)(a[1][4][7]) << endl;
  
  cout << endl << "Triming operator:" << endl;
  cout << "trim a[1:3, 4:6, 7:9]" << endl;
  r_Marray<int> c = a[ r_Minterval(3) << r_Sinterval(1,3) << r_Sinterval(4,6) << r_Sinterval(7,9) ];
  c.print_status( cout );

  cout << "Initialization of Marray<char, [0:2,0:2,0:2]> with 'A'" << endl;
  r_Minterval domain2(3);
  domain2 << r_Sinterval(0,2) << r_Sinterval(0,2) << r_Sinterval(0,2);
  r_Marray<char> a_char( domain2, 'A' );
  cout << "OK" << endl;
  a_char.print_status( cout );

  cout << "Transforming the object GMarray( [0:2,0:2,0:2], 4 ) to Marray<int>( [...] )" << endl;
  r_GMarray     genMDD( domain2, 4 );
  r_Marray<int> specMDD( genMDD );
  
  cout << specMDD.spatial_domain() << endl;  
  
  return 0;
}
