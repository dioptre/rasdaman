// -*-C++-*- (for Emacs)

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
/*************************************************************
 *
 *
 * PURPOSE:
 *   The Type class is the superclass for the classes
 *   CollectionType, BaseType, and MDDType.
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _TYPE_HH_
#define _TYPE_HH_

class Type;
class OId;

#include <iostream>
using std::ostream;
using std::cout;
using std::endl;

#include "catalogmgr/ops.hh"
#include "reladminif/dbnamedobject.hh"

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:
Type is the abstract base class for CollectionType, BaseType, and MDDType.

Common to each type is the ability to get its name.
This functionality is defined as a pure virtual function here.

{\bf Interdependencies}

Each \Ref{Tile} has a pointer to its BaseType. Pointers to BaseType
are also used in subclasses of \Ref{MDDObject}.
*/

/**
  * \ingroup Relcatalogifs
  */
class Type : public DBNamedObject
	{
	public:
		virtual void destroy();
		/*@Doc:
		does nothing.  is neccessary to stop types from being deleted by ~DBRef<Type>
		*/
	
		/// returns the name of the type as a C string.
		virtual const char* getTypeName() const;
		/*@Doc:
		  The name of the type is the class name without the Type suffix.
		  e.g. "Bool" for \Ref{BoolType}, or "ULong" for \Ref{ULongType},
		  or "Set" for \Ref{SetType}, or "Dimension" for \Ref{DimensionType}.
		*/

		/// returns the structure of the type as a C string.
		virtual char* getTypeStructure() const;
		/*@Doc:
		  Returns a copy of getTypeName() for non-structured base types. For
		  structured types a list of the elements in the form of #struct {
		  ulong elemName1, ushort elemName2 }# is returned. MDDTypes are
		  printed in the form #marray< RGBPixel, [10:20]# (less information,
		  if domain is not specified). Sets are printed in the form
		  #set<setName>#. The char* has to be freed by the caller!
		*/

		const TypeEnum getType() const;
		/*@Doc:
		returns the type as a TypeEnum.
		*/

		virtual int compatibleWith(const Type* aType) const;
		/*@Doc:
		checks, if two types are compatible (see also \Ref{MDDType}).
		*/

		Type();
		/*@Doc:
		default constructor, cannot be used.
		*/

		Type(const OId& id) throw (r_Error);
		/*@Doc:
		*/

		Type(const Type& old);
		/*@Doc:
		*/

		Type& operator=(const Type& old);
		/*@Doc:
		*/

		virtual ~Type();
		/*@Doc:
		virtual destructor.
		*/
		
	protected:
		TypeEnum myType;
		/*@Doc:
		enum for type.  this can be ULONG, USHORT, CHAR,
			BOOLTYPE, LONG, SHORT, OCTET, DOUBLE,
			FLOAT, STRUCT, CLASSTYPE, SETTYPE, MDDTYPE
		*/

		Type(const char* name);
		/*@Doc:
		*/
	};

#endif
