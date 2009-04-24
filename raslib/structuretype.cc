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
*/

static const char rcsid[] = "@(#)raslib, r_Structure_Type: $Header: /home/rasdev/CVS-repository/rasdaman/raslib/structuretype.cc,v 1.23 2003/12/27 23:01:21 rasdev Exp $";

#include <malloc.h>
#include <string.h>

#include "raslib/structuretype.hh"
#include "raslib/attribute.hh"
#include "raslib/rminit.hh"
#include "raslib/error.hh"

r_Structure_Type::r_Structure_Type()
  : r_Base_Type(),
	myAttributes(NULL),
	numAttrs(0)
{
}

r_Structure_Type::r_Structure_Type( char* newTypeName, 
				    unsigned int newNumAttrs,
				    r_Attribute* newAttrs, int offset ) 
  : r_Base_Type(newTypeName, 0),
	numAttrs(newNumAttrs),
	myAttributes(new r_Attribute[newNumAttrs])
{
  for(unsigned int i = 0; i < numAttrs; i++)
  {
    myAttributes[i] = newAttrs[i];
    myAttributes[i].set_offset( typeSize );
    myAttributes[i].set_global_offset( typeSize + offset );
    typeSize += myAttributes[i].type_of().size();
  }
}

r_Structure_Type::r_Structure_Type( const r_Structure_Type& oldObj ) 
  : r_Base_Type(oldObj), myAttributes(NULL), numAttrs(oldObj.numAttrs)
{

  if( oldObj.myAttributes )
  {
    myAttributes = new r_Attribute[numAttrs];
    for( unsigned int i = 0; i < numAttrs; i++)
      myAttributes[i] = oldObj.myAttributes[i];
  }
}

const r_Structure_Type& 
r_Structure_Type::operator=( const r_Structure_Type& oldObj )
{
  // Gracefully handle self assignment
  if (this == &oldObj) return *this;

  r_Base_Type::operator=( oldObj );
  numAttrs = oldObj.numAttrs;
  if( myAttributes )
  {
    delete[] myAttributes;
    myAttributes=NULL;
  }

  if( oldObj.myAttributes )
  {
    myAttributes = new r_Attribute[numAttrs];
    for(unsigned int i = 0; i < numAttrs; i++)
      myAttributes[i] = oldObj.myAttributes[i];
  }

  return *this;
}

r_Structure_Type::~r_Structure_Type()
{
  if( myAttributes )
    delete[] myAttributes;
}

r_Type*
r_Structure_Type::clone() const
{
  return new r_Structure_Type( *this );
}


r_Type::r_Type_Id 
r_Structure_Type::type_id() const
{
  return STRUCTURETYPE;
}

bool
r_Structure_Type::isStructType() const
{
  return true;
}

bool
r_Structure_Type::compatibleWith(const r_Structure_Type* myType) const
{
  if(myType == NULL)
    return false;
  if( count_elements() != myType->count_elements())
    return false;
    
  r_Structure_Type::attribute_iterator myIter(defines_attribute_begin());
  r_Structure_Type::attribute_iterator myTypeIter(myType->defines_attribute_begin());
  r_Structure_Type::attribute_iterator myIterEnd(defines_attribute_end());
  // FIXME not used in curr implementation
  // r_Structure_Type::attribute_iterator myTypeIterEnd(myType->defines_attribute_end());  
  while(myIter != myIterEnd) {
   if((*myIter).type_of().type_id() != (*myTypeIter).type_of().type_id())
    return false;
   myIter++; 
   myTypeIter++;
  }
  
  return true;
}

r_Structure_Type::attribute_iterator 
r_Structure_Type::defines_attribute_begin() const
{
  return attribute_iterator(myAttributes + numAttrs, myAttributes);
}

r_Structure_Type::attribute_iterator 
r_Structure_Type::defines_attribute_end() const
{
  return attribute_iterator(myAttributes + numAttrs - 1, myAttributes,
			    myAttributes + numAttrs);
}

r_Attribute
r_Structure_Type::resolve_attribute(const char* name) const throw( r_Error )
{
  r_Structure_Type::attribute_iterator iter(defines_attribute_begin());

  while( iter != defines_attribute_end() && strcmp((*iter).name(), name) != 0 ) 
    iter++;

  if( iter == defines_attribute_end() )
  {
	RMInit::logOut << "r_Structure_Type::resolve_attribute(" << name << ") not a valid atribute name" << endl;
    r_Error err(r_Error::r_Error_NameInvalid);
    throw err;
  }
      
  return (*iter);
}

r_Attribute
r_Structure_Type::resolve_attribute(unsigned int number) const throw( r_Error )
{
  r_Structure_Type::attribute_iterator iter(defines_attribute_begin());
  unsigned int i = 0;
  while( iter != defines_attribute_end() && i < number ) 
  {
    i++;
    iter++;
  }

  if( iter == defines_attribute_end() || i < number )
	{
	RMInit::logOut << "r_Structure_Type::resolve_attribute(" << number << ") index out of bounds (" << i << ")" << endl;
    throw r_Eindex_violation( 0, numAttrs-1, number );
	}

  return (*iter);
}

r_Attribute 
r_Structure_Type::operator[]( unsigned int number ) const throw( r_Error )
{
  return resolve_attribute( number );
}

unsigned int
r_Structure_Type::count_elements() const
{
  return numAttrs;
}

void
r_Structure_Type::convertToLittleEndian(char* cells, r_Area noCells) const
{
  r_Area i = 0;
  unsigned int j = 0;

  for(i=0; i<noCells; i++) {
    for(j=0; j<numAttrs; j++) {
      myAttributes[j].type_of().convertToLittleEndian(
	&cells[i*typeSize + myAttributes[j].offset()], 1);
    }
  }
}

void
r_Structure_Type::convertToBigEndian(char* cells, r_Area noCells) const
{
  r_Area i = 0;
  unsigned int j = 0;

  for(i=0; i<noCells; i++) {
    for(j=0; j<numAttrs; j++) {
      myAttributes[j].type_of().convertToBigEndian(
	&cells[i*typeSize + myAttributes[j].offset()], 1);
    }
  }
}

void 
r_Structure_Type::print_status( std::ostream& s ) const
{
  r_Structure_Type::attribute_iterator iter(defines_attribute_begin());

  s << "struct{ ";

  while(iter != defines_attribute_end())
  {
    (*iter).print_status( s );

    iter++;

    if( iter != defines_attribute_end() )
      s << ", ";
  }
 
  s << " }";
}

void 
r_Structure_Type::print_value( const char* storage,  std::ostream& s ) const
{
    s << "{ ";

    r_Structure_Type::attribute_iterator  iter( defines_attribute_begin() );

    while( iter !=  defines_attribute_end() )
    {
      (*iter).type_of().print_value( storage + (*iter).offset(),  s  );

       iter++;

       if( iter != defines_attribute_end() )
          s << ", ";
    }
    s << "}  ";  
}


std::ostream &operator<<( std::ostream &str, const r_Structure_Type &type )
{
  type.print_status(str);
  return str;
}