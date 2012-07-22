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
 * SOURCE: test_iterator.cc
 *
 * MODULE: rasodmg
 *
 * COMMENTS:
 *      None
*/

#include <iostream>
#include "rasodmg/collection.hh"
#include "rasodmg/iterator.hh"

int main()
{
    int i,v,x,y,z,status;

    i = 0;
    v = 100;
    x = 200;
    y = 300;
    z = 400;
    status = 5;

    cout << endl << endl;
    cout << "Iterator Examples" << endl;
    cout << "===================" << endl << endl;

    cout << "Creating r_Collection of type int." <<endl;
    r_Collection<int> a;

    cout << "Creating an iterator of an empty collection." <<endl;
    r_Iterator<int> iterEmpty = a.create_iterator();
    if( iterEmpty.not_done() )
        cout << "Iterator says that iteration is not done." << endl << endl;
    else
        cout << "Iterator says that iteration is done." << endl << endl;

    cout << "Now inserting four elements:" << endl << "v = 100" << endl;
    a.insert_element(v);
    r_Iterator<int> iterTest = a.create_iterator();
    if( iterTest.not_done() )
        cout << "Iterator says that iteration is not done." << endl << endl;
    else
        cout << "Iterator says that iteration is done." << endl << endl;

    cout << "x = 200" << endl;
    a.insert_element(x);

    cout << "y = 300" << endl;
    a.insert_element(y);

    cout << "z = 400" << endl;
    a.insert_element(z);

    cout << "Cardinality of collection 'a' after four inserts: " << a.cardinality() << endl << endl;

    cout << "Now creating an iterator for 'a'." << endl;

    // ODMG wants an iterator to be created this way...
    r_Iterator<int> iter = a.create_iterator();

    // ...but this is an equally valid version that doesn't require
    // r_Collection to have a member function create_iterator:
    //r_Iterator<int> iter( a );

    cout << "Iterator points to element: " << iter.get_element() << endl << endl
         << "Advancing iterator two times." << endl;
    iter.advance();
    iter.advance();
    cout << "Iterator points to element: " << iter.get_element() << endl << endl
         << "Regetting this element and advancing iterator (next function)." << endl;
    status = iter.next( i );
    cout << "Element is " << i << "." << endl;
    cout << "Iterator points to element: " << iter.get_element() << endl << endl
         << "Resetting iterator." << endl;
    iter.reset();
    cout << "Iterator points to element: " << iter.get_element() << endl << endl;

    cout << "Testing prefix and postfix incrementors." << endl;
    r_Iterator<int> iter2 = a.create_iterator();
    //r_Iterator<int> iter2( a );
    iter.reset();
    cout << "Postfix incrementor returns: " << iter++.get_element() << endl;
    iter.reset();
    iter2 = ++iter;
    cout << "Prefix incrementor returns: " << iter2.get_element() << endl << endl;

    cout << "Resetting both iterators." << endl;
    iter.reset();
    iter2.reset();
    cout << "iter1 == iter2 ? (1=TRUE/0=FALSE) " << iter.is_equal(iter2) << endl;
    cout << "iter1 != iter2 ? (1=TRUE/0=FALSE) " << (!iter.is_equal(iter2)) << endl << endl;

    cout << "Computing all permutatios of the four numbers:" << endl;

    r_Iterator<int> iter3 = a.create_iterator();
    //r_Iterator<int> iter3( a );
    r_Iterator<int> iter4 = a.create_iterator();
    //r_Iterator<int> iter4( a );

    for ( ; iter.not_done(); iter++ )
        for ( iter2.reset(); iter2.not_done(); iter2++ )
            for ( iter3.reset(); iter3.not_done(); iter3++ )
                for ( iter4.reset(); iter4.not_done(); iter4++ )
                    if ( !(iter4.is_equal(iter3) ||
                            iter4.is_equal(iter2) ||
                            iter4.is_equal(iter)  ||
                            iter3.is_equal(iter2) ||
                            iter3.is_equal(iter)  ||
                            iter2.is_equal(iter) ) )
                        cout << *iter << " " << *iter2 << " " << *iter3 << " " << *iter4 << endl;
}
