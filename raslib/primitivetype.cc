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

static const char rcsid[] = "@(#)raslib, r_Primitive_Type: $Header: /home/rasdev/CVS-repository/rasdaman/raslib/primitivetype.cc,v 1.26 2003/12/27 23:01:21 rasdev Exp $";

#include "raslib/primitivetype.hh"
#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"
#include "raslib/odmgtypes.hh"
#include "raslib/endian.hh"
#include "raslib/error.hh"

#include <iomanip>
#include <string>
#include <cstring>

r_Primitive_Type::r_Primitive_Type()
  : r_Base_Type(),
	typeId(UNKNOWNTYPE)
{
}

r_Primitive_Type::r_Primitive_Type( const char* newTypeName, 
				    const r_Type::r_Type_Id newTypeId ) 
  : r_Base_Type(newTypeName, 0),
	typeId(newTypeId)
{

  switch( typeId )
  {
    case ULONG:  typeSize = 4; break;
    case USHORT: typeSize = 2; break;
    case BOOL:   typeSize = 1; break;
    case LONG:   typeSize = 4; break;
    case SHORT:  typeSize = 2; break;
    case OCTET:  typeSize = 1; break;
    case DOUBLE: typeSize = 8; break;
    case FLOAT:  typeSize = 4; break;
    case CHAR:   typeSize = 1; break;
    case COMPLEXTYPE1:
		typeSize = 2 * sizeof(float);
		break;
    case COMPLEXTYPE2:
		typeSize = 2 * sizeof(double);
		break;
    default:
		RMDBGONCE(3,	RMDebug::module_raslib, "r_Primitive_Type", "r_Primitive_Type(....) bad typeId " << typeId);
    		break;
  }
}

r_Primitive_Type::r_Primitive_Type( const r_Primitive_Type& oldObj ) 
  : r_Base_Type(oldObj),
	typeId(oldObj.typeId)
{
}

const r_Primitive_Type& 
r_Primitive_Type::operator=( const r_Primitive_Type& oldObj )
{
  // Gracefully handle self assignment
  if (this == &oldObj) return *this;

  r_Base_Type::operator=( oldObj );
  typeId = oldObj.typeId; 

  return *this;
}

r_Primitive_Type::~r_Primitive_Type()
{
}

r_Type*
r_Primitive_Type::clone() const
{
  return new r_Primitive_Type( *this );
}


r_Type::r_Type_Id 
r_Primitive_Type::type_id() const
{
  return typeId;  
}

bool
r_Primitive_Type::isPrimitiveType() const
{
 return true;
}

void
r_Primitive_Type::convertToLittleEndian(char* cells, r_Bytes noCells) const
	{
	char c0 = 0;
	char c1 = 0;
	char c2 = 0;
	char c3 = 0;
	r_Bytes i = 0;

	switch( typeId )
		{
		case USHORT:
	  	case SHORT:
   	 		for(i=0; i<noCells; i++ )
				{
				c1 = cells[i*typeSize];
      				c0 = cells[i*typeSize + 1];
      				cells[i*typeSize] = c0;
      				cells[i*typeSize + 1] = c1;
    				}
    			break;
    	
  		case ULONG:
  		case LONG:
			for(i=0; i<noCells; i++ )
				{
				c3 = cells[i*typeSize];
				c2 = cells[i*typeSize + 1];
				c1 = cells[i*typeSize + 2];
				c0 = cells[i*typeSize + 3];
				cells[i*typeSize] = c0;
				cells[i*typeSize + 1] = c1;
				cells[i*typeSize + 2] = c2;
				cells[i*typeSize + 3] = c3;
				}
    			break;
    
		case FLOAT:
			for(i = 0; i < noCells; ++i) 
				((r_Float *)cells)[i] = r_Endian::swap( ((r_Float *)cells)[i] );
			break;

		case DOUBLE:
			for(i = 0; i < noCells; ++i) 
				((r_Double *)cells)[i] = r_Endian::swap( ((r_Double *)cells)[i] );
			break;
	       default:
			RMDBGONCE(3,	RMDebug::module_raslib, "r_Primitive_Type", "convertToLittleEndian(....) bad typeId " << typeId);
    			break;    		
		}
	}

void
r_Primitive_Type::convertToBigEndian(char* cells, r_Bytes noCells) const
	{
	char c0 = 0;
	char c1 = 0;
	char c2 = 0;
	char c3 = 0;
  	r_Bytes i = 0;

  	switch( typeId )
		{
  		case USHORT:
  		case SHORT:
			for(i=0; i<noCells; i++)
				{
				c1 = cells[i*typeSize];
				c0 = cells[i*typeSize + 1];
				cells[i*typeSize] = c0;
				cells[i*typeSize + 1] = c1;
				}
			break;
    	
  		case ULONG:
  		case LONG:
			for(i=0; i<noCells; i++)
				{
				c3 = cells[i*typeSize];
				c2 = cells[i*typeSize + 1];
				c1 = cells[i*typeSize + 2];
				c0 = cells[i*typeSize + 3];
				cells[i*typeSize] = c0;
				cells[i*typeSize + 1] = c1;
				cells[i*typeSize + 2] = c2;
				cells[i*typeSize + 3] = c3;
				}
			break;
    	
		case FLOAT:
			for(i = 0; i < noCells; ++i) 
				((r_Float *)cells)[i] = r_Endian::swap( ((r_Float *)cells)[i] );
			break;

		case DOUBLE:
			for(i = 0; i < noCells; ++i) 
				((r_Double *)cells)[i] = r_Endian::swap( ((r_Double *)cells)[i] );
			break;
	       default:
			RMDBGONCE(3,	RMDebug::module_raslib, "r_Primitive_Type", "convertToBigEndian(....) bad typeId " << typeId);
    			break;			
		}
	}

void 
r_Primitive_Type::print_status( std::ostream& s ) const
{
  s << typeId;
}

void 
r_Primitive_Type::print_value( const char* storage,  std::ostream& s  ) const
{  
    switch( typeId )
    {
        case r_Type::ULONG:  s << std::setw(5) <<       get_ulong( storage );    break;
        case r_Type::USHORT: s << std::setw(5) <<       get_ushort( storage );   break;
        case r_Type::BOOL:   s << std::setw(5) << ( get_boolean( storage ) ? "T" : "F" ); break;
        case r_Type::LONG:   s << std::setw(5) <<       get_long( storage );   break;
        case r_Type::SHORT:  s << std::setw(5) <<       get_short( storage );   break;
        case r_Type::OCTET:  s << std::setw(5) << (int)( get_octet( storage ) ); break; 
        case r_Type::DOUBLE: s << std::setw(5) <<       get_double( storage ) ;  break;
        case r_Type::FLOAT:  s << std::setw(5) <<       get_float( storage );   break;
        case r_Type::CHAR:   s << std::setw(5) << (int)( get_char( storage ) ); break;
        default: 
	  RMInit::logOut << "r_Primitive_Type::print_value(...) type unknown" << endl;
    	break;
    }        
}

//FIXME
// We have to return the value in the most powerfull type(e.g. now r_Double) without loss
// This may change in future

r_Double 
r_Primitive_Type::get_value( const char* storage ) const throw(r_Error)
{  
 r_Double retVal=0.;

    switch( typeId )
    {
        case r_Type::ULONG:  retVal=get_ulong( storage ); break;
        case r_Type::USHORT: retVal=get_ushort( storage ); break;
        case r_Type::BOOL:   retVal=get_boolean( storage ); break;
        case r_Type::LONG:   retVal=get_long( storage ); break;
        case r_Type::SHORT:  retVal=get_short( storage ); break;
        case r_Type::OCTET:  retVal=get_octet( storage ); break; 
        case r_Type::DOUBLE: retVal=get_double( storage );  break;
        case r_Type::FLOAT:  retVal=get_float( storage );   break;
        case r_Type::CHAR:   retVal=get_char( storage ); break;
        default:
	{
	   RMInit::logOut << "r_Primitive_Type::get_value(...) type unknown" << endl;
	   r_Error err( r_Error::r_Error_TypeInvalid );     
	   throw( err );     
	};
        break;
    }        
 return retVal;
}

//FIXME
// We have to set the value from the most powerfull type(e.g. now r_Double) without loss
// This may change in future

void
r_Primitive_Type::set_value( char* storage, r_Double val ) throw(r_Error)
{  
    switch( typeId )
    {
        case r_Type::ULONG:  set_ulong( storage, (r_ULong)val );    break;
        case r_Type::USHORT: set_ushort( storage, (r_UShort)val );   break;
        case r_Type::BOOL:   set_boolean( storage, (r_Boolean)val ); break;
        case r_Type::LONG:   set_long( storage, (r_Long)val );   break;
        case r_Type::SHORT:  set_short( storage, (r_Short)val );   break;
        case r_Type::OCTET:  set_octet( storage, (r_Octet)val ); break; 
        case r_Type::DOUBLE: set_double( storage, (r_Double)val);  break;
        case r_Type::FLOAT:  set_float( storage, (r_Float)val );   break;
        case r_Type::CHAR:   set_char( storage, (r_Char)val ); break;
	default:
	{
	 RMInit::logOut << "r_Primitive_Type::set_value(...) type unknown" << endl;
	 r_Error err( r_Error::r_Error_TypeInvalid );     
	 throw( err );     
	};	
	break;        
    }        
}

//FIXME
// We have to set the value from the most powerfull type(e.g. now r_Double) without loss
// This may change in future

void
r_Primitive_Type::get_limits( r_Double& min, r_Double& max ) throw(r_Error)
{  
  r_Double *type=NULL;
    switch( typeId )
    {
        case r_Type::ULONG:  ::get_limits( (r_ULong*)type, min, max );    break;
        case r_Type::USHORT: ::get_limits( (r_UShort*)type, min, max );   break;
        case r_Type::BOOL:   ::get_limits( (r_Boolean*)type, min, max ); break;
        case r_Type::LONG:   ::get_limits( (r_Long*)type, min, max );   break;
        case r_Type::SHORT:  ::get_limits( (r_Short*)type, min, max );   break;
        case r_Type::OCTET:  ::get_limits( (r_Octet*)type, min, max ); break; 
        case r_Type::DOUBLE: ::get_limits( (r_Double*)type, min, max);  break;
        case r_Type::FLOAT:  ::get_limits( (r_Float*)type, min, max );   break;
        case r_Type::CHAR:   ::get_limits( (r_Char*)type, min, max ); break;
	default:
	{
	 RMInit::logOut << "r_Primitive_Type::get_limits(...) type unknown" << endl;
	 r_Error err( r_Error::r_Error_TypeInvalid );     
	 throw( err );     
	};	
	break;        
    }        
}

r_Boolean 
r_Primitive_Type::get_boolean( const char* cell ) const throw( r_Error )
{
  if( typeId != r_Type::BOOL )
  {
    RMInit::logOut << "r_Primitive_Type::get_boolean(cell) type not a boolean" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  return *((r_Boolean*)cell);
}



r_Char   
r_Primitive_Type::get_char( const char* cell )    const throw( r_Error )
{
  if( typeId != r_Type::CHAR )
  {
	RMInit::logOut << "r_Primitive_Type::get_char(cell) type not a char" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  return *((r_Char*)cell);
}



r_Octet   
r_Primitive_Type::get_octet( const char* cell )   const throw( r_Error )
{
  if( typeId != r_Type::OCTET )
  {
	RMInit::logOut << "r_Primitive_Type::get_octet(cell) type not a octet" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  return *((r_Octet*)cell);
}



r_Short   
r_Primitive_Type::get_short( const char* cell )   const throw( r_Error )
{
  if( typeId != r_Type::SHORT )
  {
	RMInit::logOut << "r_Primitive_Type::get_short(cell) type not a short" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  return *((r_Short*)cell);
}



r_UShort  
r_Primitive_Type::get_ushort( const char* cell )  const throw( r_Error )
{
  if( typeId != r_Type::USHORT )
  {
	RMInit::logOut << "r_Primitive_Type::get_ushort(cell) type not a ushort" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  return *((r_UShort*)cell);
}



r_Long    
r_Primitive_Type::get_long( const char* cell )    const throw( r_Error )
{
  if( typeId != r_Type::LONG )
  {
	RMInit::logOut << "r_Primitive_Type::get_long(cell) type not a long" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  return *((r_Long*)cell);
}



r_ULong   
r_Primitive_Type::get_ulong( const char* cell )   const throw( r_Error )
{
  if( typeId != r_Type::ULONG )
  {
	RMInit::logOut << "r_Primitive_Type::get_ulong(cell) type not a ulong" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  return *((r_ULong*)cell);
}



r_Float   
r_Primitive_Type::get_float( const char* cell )   const throw( r_Error )
{
  if( typeId != r_Type::FLOAT )
  {
	RMInit::logOut << "r_Primitive_Type::get_float(cell) type not a float" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  return *((r_Float*)cell);
}



r_Double  
r_Primitive_Type::get_double( const char* cell )  const throw( r_Error )
{
  if( typeId != r_Type::DOUBLE )
  {
	RMInit::logOut << "r_Primitive_Type::get_double(cell) type not a double" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  return *((r_Double*)cell);
}

void
r_Primitive_Type::set_boolean( char* cell, r_Boolean val ) throw( r_Error )
{
  if( typeId != r_Type::BOOL )
  {
	RMInit::logOut << "r_Primitive_Type::set_boolean(cell, val) type not a boolean" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  memmove(cell, &val, typeSize);
}



void 
r_Primitive_Type::set_char( char* cell, r_Char val )    throw( r_Error )
{
  if( typeId != r_Type::CHAR )
  {
	RMInit::logOut << "r_Primitive_Type::set_char(cell, val) type not a char" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  memmove(cell, &val, typeSize); 
}



void
r_Primitive_Type::set_octet( char* cell, r_Octet val )   throw( r_Error )
{
  if( typeId != r_Type::OCTET )
  {
	RMInit::logOut << "r_Primitive_Type::set_octet(cell, val) type not a octet" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  memmove(cell, &val, typeSize);
}



void
r_Primitive_Type::set_short( char* cell, r_Short val )   throw( r_Error )
{
  if( typeId != r_Type::SHORT )
  {
	RMInit::logOut << "r_Primitive_Type::set_short(cell, val) type not a short" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  memmove(cell, &val, typeSize);
}



void
r_Primitive_Type::set_ushort( char* cell, r_UShort val )  throw( r_Error )
{
  if( typeId != r_Type::USHORT )
  {
	RMInit::logOut << "r_Primitive_Type::set_ushort(cell, val) type not a ushort" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  memmove(cell, &val, typeSize);
}



void
r_Primitive_Type::set_long( char* cell, r_Long val )    throw( r_Error )
{
  if( typeId != r_Type::LONG )
  {
	RMInit::logOut << "r_Primitive_Type::set_long(cell, val) type not a long" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  memmove(cell, &val, typeSize);
}



void
r_Primitive_Type::set_ulong( char* cell, r_ULong val )   throw( r_Error )
{
  if( typeId != r_Type::ULONG )
  {
	RMInit::logOut << "r_Primitive_Type::set_ulong(cell, val) type not a ulong" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  memmove(cell, &val, typeSize);
}



void
r_Primitive_Type::set_float( char* cell, r_Float val )   throw( r_Error )
{
  if( typeId != r_Type::FLOAT )
  {
	RMInit::logOut << "r_Primitive_Type::set_float(cell, val) type not a float" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  memmove(cell, &val, typeSize);
}



void  
r_Primitive_Type::set_double( char* cell, r_Double val )  throw( r_Error )
{
  if( typeId != r_Type::DOUBLE )
  {
	RMInit::logOut << "r_Primitive_Type::set_double(cell, val) type not a double" << endl;
    r_Error err( r_Error::r_Error_TypeInvalid );     
    throw( err );     
  }

  memmove(cell, &val, typeSize);
}

std::ostream &operator<<( std::ostream &str, const r_Primitive_Type &type )
{
  type.print_status(str);
  return str;
}
