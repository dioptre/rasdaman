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
 * SOURCE: set.cc
 *
 * MODULE: rasodmg
 * CLASS:  r_Set
 *
 * COMMENTS:
 *			None
*/


static const char rcsidset[] = "@(#)rasodmg, r_Set: $Id: set.cc,v 1.17 2002/08/23 11:18:44 schatz Exp $";

#include "rasodmg/set.hh"

class r_GMarray;

#ifdef __VISUALC__
  template class r_Set< r_GMarray *>;
#endif

#ifndef __GNUG__
#define NULL 0
#endif

template<class T>
r_Set<T>::r_Set() throw(r_Error)
  : r_Collection<T>()
{
  this->allowsDuplicates = 0;
  this->isOrdered = 0;
  this->card = 0;
}

/* OBSOLETE
template<class T>
r_Set<T>::r_Set( const char* name )
  : r_Collection<T>( name )
{
  allowsDuplicates = 0;
  isOrdered = 0;
  card = 0;
}
*/

template<class T>
r_Set<T>::r_Set( const r_Set<T>& set ) throw(r_Error)
  : r_Collection<T>( set )
{
  this->allowsDuplicates = 0;
  this->isOrdered = 0;
}

template<class T>
r_Set<T>::~r_Set()
{
}

template<class T>
void
r_Set<T>::insert_element( const T& element, int no_modification )
{
  typename r_Collection<T>::CNode* ptr = (typename r_Collection<T>::CNode*)this->coll;
  
  while ( ptr->next != NULL && *((T*)(ptr->elem)) != element )
    ptr = ptr->next;
  
  if ( ptr->elem == NULL || *((T*)(ptr->elem)) != element )
     r_Collection<T>::insert_element( element, no_modification );
}
