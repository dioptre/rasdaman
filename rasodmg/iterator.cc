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
 * SOURCE: iterator.cc
 *
 * MODULE: rasodmg
 * CLASS:  r_Iterator
 *
 * COMMENTS:
 *		None
*/

static const char rcsiditerator[] = "@(#)rasodmg, r_Iterator: $Id: iterator.cc,v 1.18 2002/08/23 11:18:44 schatz Exp $";

#include <iostream>
using namespace std;

#include "rasodmg/iterator.hh"

#ifdef EARLY_TEMPLATE
#ifndef __EXECUTABLE__
#define __EXECUTABLE__
#define ITERATOR_NOT_SET
#endif
#endif

#include "rasodmg/collection.hh"

#ifdef ITERATOR_NOT_SET
#undef __EXECUTABLE__
#endif

template<class T>
r_Iterator<T>::r_Iterator()
{
   // use default constructor only with assignment operator!
   collection = 0;
   ptr = 0;
   ndone = 0;
}

template<class T>
r_Iterator<T>::r_Iterator( const r_Iterator<T>& iter )
{
   collection = iter.collection;
   ptr = iter.ptr;
   ndone = iter.ndone;
}

template<class T>
r_Iterator<T>::r_Iterator( r_Collection<T>& source, int removed_objects )
{
  collection = &source;
  // sorry for this awful cast but there is
  // no standard conversion of r_Collection<T>::CNode* to r_Collection::CNode*
  if( removed_objects )
    ptr = (typename r_Collection<T>::CNode*)source.removed_objects;    
  else
    ptr = (typename r_Collection<T>::CNode*)source.coll;

  ndone = (ptr->elem != 0);
}

template<class T>
r_Iterator<T>::~r_Iterator()
{
}

template<class T>
r_Iterator<T>&
r_Iterator<T>::operator=( const r_Iterator<T>& iter )
{
  if( this != &iter )
  {
    collection = iter.collection;
    ptr        = iter.ptr;
    ndone      = iter.ndone;
  }
  
  return *this;
}

template<class T>
int
r_Iterator<T>::is_equal(const r_Iterator<T>& iter) const
{
  if ( collection == iter.collection)
    if ( ptr == iter.ptr )
      return 1;
  return 0;
}


template<class T>
int 
operator==( const r_Iterator<T>& iter1, const r_Iterator<T>& iter2 )
{
  return iter1.is_equal(iter2);
}

template<class T>
int 
operator!=( const r_Iterator<T>& iter1, const r_Iterator<T>& iter2 )
{
  return !iter1.is_equal(iter2);
}

template<class T>
r_Iterator<T>& 
r_Iterator<T>::operator++()
{
  // ++prefix operator
  
  if ( !ndone )
    throw r_Error( r_Error::r_Error_IteratorExhausted );
  if ( ptr->next != 0 )
    ptr = ptr->next;
  else
    ndone = 0;
  
  return *this;
}

template<class T>
r_Iterator<T>
r_Iterator<T>::operator++( int )
{
  // postfix++ operator
  // create a copy of this, increment the original and return the copy
  r_Iterator<T> result( *this );
  
  operator++();

  return result;
}

template<class T>
T 
r_Iterator<T>::operator*()
  throw( r_Error )
{
  if ( !ndone || ptr->elem == 0 )
  {
    r_Error err = r_Error( r_Error::r_Error_IteratorExhausted );
	throw err;
  }

  // The following line was return *(ptr->elem) but the HP compiler had problems 
  // while instantiating the code. CNode::elem was of a different type than T.
  return *((T*)ptr->elem);
}

template<class T>
T
r_Iterator<T>::get_element() const
  throw( r_Error )
{
  if ( !ndone || ptr->elem == 0 )
    throw r_Error( r_Error::r_Error_IteratorExhausted );
  else
    return *(ptr->elem);
}

template<class T>
int
r_Iterator<T>::next( T& element )
{
  if ( !ndone || ptr->elem == 0 ) return 0;
  element = *(ptr->elem);
  advance();
  return 1;
}

template<class T>
void
r_Iterator<T>::reset( int removed_objects )
{
  if( removed_objects )
    ptr = (typename r_Collection<T>::CNode*)collection->removed_objects;
  else
    ptr = (typename r_Collection<T>::CNode*)collection->coll;

  ndone = (ptr->elem != 0);
}

template<class T>
void
r_Iterator<T>::advance()
{
  if ( !ndone )
    throw r_Error( r_Error::r_Error_IteratorExhausted );
  if ( ptr->next != 0 )
    ptr = ptr->next;
  else
    ndone = 0;
}
