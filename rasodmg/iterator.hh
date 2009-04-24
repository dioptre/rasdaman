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
 * INCLUDE: iterator.hh
 *
 * MODULE:  rasodmg
 * CLASS:   r_Iterator
 *
 * COMMENTS:
 *		None
*/

#ifndef _D_ITERATOR_
#define _D_ITERATOR_

#include "raslib/error.hh"

#ifdef __VISUALC__
template <class T> class r_Collection;
#else
#include "rasodmg/collection.hh"
#endif

//@ManMemo: Module: {\bf rasodmg}



/*@Doc:

  The template class \Ref{r_Iterator} defines the generic
  behavior for iteration. All iterators use a consistent protocol
  for sequentially returning each element from the collection over
  which the iteration is defined.
  When an iterator is constructed, it is either initialized with
  another iterator or is set to null. When an iterator is constructed
  via the {\tt create_iterator()} method defined in \Ref{r_Collection},
  the iterator is initailized to point to the first element, if there 
  is one. 
*/

template <class T>
class r_Iterator
{
  public:
    /// default constructor
    r_Iterator();
    /// copy constructor
    r_Iterator( const r_Iterator<T>& iter );
    /// constructor getting the collection on which to iterate (used for {\tt r_Collection::create_iterator()})
    r_Iterator( r_Collection<T>& source, int removed_objects=0 );
    /**
      Creates an iterator which points to the first element of the element collection. If
      {\tt removed_objects} ist set to 1, the iterator points to the first element of
      the list containing the removed objects (for internal use).
    */

    /// destructor
    ~r_Iterator();

    /// assignment operator
    r_Iterator<T>& operator=( const r_Iterator<T>& iter );
    /// comparisons: equal if they point to the same element in the same collection,
    /// not equal if they point to different collections or elements
    int is_equal( const r_Iterator<T>& iter ) const;

    /// prefix incrementor
    r_Iterator<T>& operator++();
    /// postfix incrementor
    r_Iterator<T>  operator++( int );

    /// re-initialize the iterator to the start of iteration for the same collection
    void reset( int removed_objects=0 );
    /**
      Resets the iterator to point to the first element of the element collection. If
      {\tt removed_objects} ist set to 1, the iterator points to the first element of
      the list containing the removed objects (for internal use).
    */

    /// returns 1 if there are more elements to be visited in the iteration and 0 if iteration is complete
    inline int  not_done() const;
    /// advances one element
    void advance();

    /// the dereference operator gets the actual element
    T    operator*() throw( r_Error );
    /// gets the actual element
    T    get_element() const throw( r_Error );
    
    /// gets the actual element, advances one element, and returns whether iteration is complete or not
    int  next( T& element );
    
    /// replaces the actual element (can only be used with r_List)
    void replace_element( const T& element );

  private:
    /// flag for end of iteration
    int ndone;
    /// pointer to the collection on which is iterated
    r_Collection<T>* collection;
    /// pointer to the actual element
    typename r_Collection<T>::CNode* ptr;

};

#include "iterator.icc"

#ifdef EARLY_TEMPLATE
#ifdef __EXECUTABLE__
#ifdef __VISUALC__
#include "iterator.cpp"
#else
#include "iterator.cc"
#endif
#endif 
#endif

#endif
