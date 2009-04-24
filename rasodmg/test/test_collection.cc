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
 * SOURCE: test_collection.cc
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

#include <iostream>
#include "rasodmg/collection.hh"
#include "rasodmg/iterator.hh"
#include "rasodmg/ref.hh"

int main()
{   
  int v,x,y,z,d;
  
  v = 100;
  x = 200;
  y = 100;
  z = 300;
  d = 500;

  cout << endl << endl;
  cout << "Collection Examples" << endl;
  cout << "====================" << endl << endl;

  cout << "Creating r_Collection of type int." <<endl;
  r_Collection< r_Ref_Any > a;
  r_Iterator< r_Ref_Any >   iter;
    
  cout << "Cardinality of empty collection 'a': " << a.cardinality() << endl << endl;
  
  cout << "Now inserting four elements:" << endl << "v = 100" << endl;
  a.insert_element( &v );
  
  cout << "x = 200" << endl;
  a.insert_element( &x );
  
  cout << "y = 100 (should work in collections)" << endl;
  a.insert_element( &y );
  
  cout << "z = 300" << endl;
  a.insert_element( &z );

  cout << "Cardinality of collection 'a' after four inserts: " << a.cardinality() << endl << endl;

  cout << "Elements: " << flush;
  for( iter = a.create_iterator(); iter.not_done(); iter++ )
    cout << (*iter) << ", " << flush;
  cout << endl;

  cout << "Does 'a' contain element '100' (1=TRUE/0=FALSE)? " << a.contains_element(&y) << endl;
  
  cout << "Does 'a' contain element '500' (1=TRUE/0=FALSE)? " << a.contains_element(&d) << endl << endl;
  
  cout << "Now removing element 'x=200' from 'a'." << endl;
  a.remove_element(&x);
  cout << "Cardinality of 'a' now: " << a.cardinality() << endl << endl;

  cout << "Now removing element '100' from 'a'." << endl;
  a.remove_element(&y);
  cout << "Cardinality of 'a' now: " << a.cardinality() << endl << endl;
  
  cout << "Now removing (non-existing) element '500' from 'a'." << endl;
  a.remove_element(&d);
  cout << "Cardinality of 'a' now: " << a.cardinality() << endl << endl;
  
  cout << "Elements: " << flush;
  for( iter = a.create_iterator(); iter.not_done(); iter++ )
    cout << (*iter) << ", " << flush;
  cout << endl;

  cout << "Testing assignment operator on r_Collections." << endl << "(creating r_Collection 'b' that is equal to 'a'.)" <<endl;
  r_Collection< r_Ref_Any > b;
  b = a;
  cout << "Cardinality of 'b': " << b.cardinality() << endl << endl;

  cout << "Testing copy constructor of r_Collection." << endl << "(creating r_Collection 'c' that is equal to 'a'.)" <<endl;
  r_Collection< r_Ref_Any > c(a);
  cout << "Cardinality of 'c': " << c.cardinality() << endl << endl;

  cout << "Now removing all elements from 'a'." << endl;
  a.remove_all();
  cout << "Cardinality of 'a' now: " << a.cardinality() << endl;
  cout << "Cardinality of 'b' is still: " << b.cardinality() << endl;
  cout << "Cardinality of 'c' is still: " << c.cardinality() << endl << endl;
}
