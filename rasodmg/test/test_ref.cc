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
 * SOURCE: test_ref.cc
 *
 * MODULE: rasodmg
 *
 * COMMENTS:
 * 			None
*/

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include <iostream>
#include "rasodmg/ref.hh"

class X
{
  public:
    int a;
};

int main()
{   
  cout << endl << endl;
  cout << "r_Ref tests" << endl;
  cout << "===========" << endl << endl;
    
  r_Ref<X>   ptr;
  r_Ref<int> ptr2;
  
  cout << "r_Ref created. is_null() ? " << ptr.is_null() << endl;
  
  ptr = new X;
  
  cout << "r_Ref assigned to pointer of base type. is_null() ? " << ptr.is_null() << endl;
  
  (*ptr).a = 1;
  cout << "assignment *ptr.a = 1. Current value ? " << (*ptr).a << endl;
  ptr->a = 2;
  cout << "assignment ptr->a = 2. Current value ? " << ptr->a << endl;

  ptr.destroy(); // delete from memory
  
  return 0;
}
