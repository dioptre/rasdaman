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
/*************************************************************
 *
 *
 * PURPOSE:
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _TYPEFACTORY_HH_
#define _TYPEFACTORY_HH_

class TypeFactory;

#include <vector>
#include <map>

#include "reladminif/oidif.hh"
#include "relcatalogif/typeiterator.hh"

class ULongType;
class LongType;
class CharType;
class BoolType;
class UShortType;
class ShortType;
class OctetType;
class DoubleType;
class FloatType;
class ComplexType1;
class ComplexType2;
class StructType;
class BaseType;
class SetType;
class MDDType;
class Type;
class DBMinterval;

//@ManMemo: Module: {\bf relcatalogif}.

/*@Doc:

The user specifies types as strings. This is passed down to the
TypeFactory where according to the string the object representing
the type is read from the database. The base DBMS manages
persistent objects for all base types. Pointers to these
persistent objects are returned if #mapType# is called. In the
current implementation, these pointers get invalid after a
transaction is committed. So the user has to call #mapType# again
after a commit(most programs do this anyway).

The name of the type used for accessing it is the class name without
the Type suffix. e.g. "Bool" for \Ref{BoolType} or "ULong" for
\Ref{ULongType}.

Only one instance of TypeFactory exists at any time. Also only one
instance of the types exits in memory. The pointers returned
should not be freed, the memory is freed, when the TypeFactory is
destroyed. The static member function can also be called directly
with the scope operator. The class follows the singleton design
pattern(Gamma et. al. S. 127ff.) and the builder design pattern
(Gamma et. al.  S. 97ff.).

{\bf Functionality}

Returns a pointer to \Ref{BaseType} to the type which is identified by
a string.

{\bf Interdependencies}

Will be used from \Ref{MDDObject} and subclasses when created.
Because all types are stored persistently, it is necessary to open
a session using AdminIf::instance()(see \Ref{AdminIf}), a
database using DatabaseIf::open()(see \Ref{DatabaseIf) and a
transaction using TransactionIf::begin(see \Ref{TransactionIf)
before creating instances of types with mapType().
*/

class TypeFactory
	{
	public:
		static TypeFactory* instance();
		/*@Doc:
		used to access instance of TileFactory.
		*/

		static const BaseType* mapType(const char* typeName);
		/*@Doc:
		maps a string to a base type
			e.g. #aTypeFactory->mapType("ULong")# returns a pointer to an
			instance of ULongType. This also works for user defined type
			like #aTypeFactory->mapType("myStruct")# returning a pointer
			to a user defined StructType.
		*/
		
		//static BaseType* mapType(char* typeName);
		/*@Doc: 
		maps a string to a base type
		use the const char* version!!
		*/
		
		static const SetType* mapSetType(const char* typeName);
		/*@Doc:
		maps a string to a set type
		*/

		//static const SetType* mapSetType(char* typeName);
		/*@Doc:
		maps a string to a set type
		use the const char* version!!
		*/
		
		static const MDDType* mapMDDType(const char* typeName);
		/*@Doc:
		maps a string to a mdd type
		*/

		//static const MDDType* mapMDDType(char* typeName);
		/*@Doc:
		maps a string to a mdd type.
		use the const char* version!!
		*/

		static const StructType* addStructType(const StructType* type);
		/*@Doc:
		add a new struct type to the current DBMS.
			After calling this function, a user defined type can be
			retrieved with function #mapType()#. The type only becomes
			persistent after commit.
		*/
		
		static const SetType* addSetType(const SetType* type);
		/*@Doc:
		add a new set type to the current DBMS.
			After calling this function, a user defined set type can be
			retrieved with function #mapSetType()#. The type only becomes
			persistent after commit.
		*/
		
		static const MDDType* addMDDType(const MDDType* type);
		/*@Doc:
		add a new set type to the current DBMS.
			After calling this function, a user defined mdd type can be
			retrieved with function #mapMDDType()#. The type only becomes
			persistent after commit.
		*/
		
		static void deleteStructType(const char* typeName);
		/*@Doc:
		delete a struct type in the current DBMS.
		*/

		static void deleteSetType(const char* typeName);
		/*@Doc:
		delete a set type in the current DBMS.
		*/
		
		static void deleteMDDType(const char* typeName);
		/*@Doc:
		delete a mdd type in the current DBMS.
		*/
		
		static TypeIterator<StructType> createStructIter();
		/*@Doc:
			Note that get_element returns a pointer to a StructType!
		returns an iterator for StructTypes.
		*/

		static TypeIterator<SetType> createSetIter();
		/*@Doc:
			Note that get_element returns a pointer to a SetType!
		returns an iterator for SetTypes.
		*/
		
		static TypeIterator<MDDType> createMDDIter();
		/*@Doc:
			Note that get_element returns a pointer to a MDDType!
		returns an iterator for MDDTypes.
		*/


		static Type* addTempType(Type* type);
		/*@Doc:
			Memory is freed at commit time of transaction in
			TransactionIf::commit(), TransactionIf::validate() and
			TransactionIf::abort().

			{\em Note:} You have to use addTempType() on a composite type
			(e.g. \Ref{SetType}) first before calling addTempType() on a
			component(e.g. \Ref{MDDType}). Otherwise the component may be
			freed first sometimes leading to a crash.
		registers a temporary type with the type factory.
		*/

		static void initialize();
		/*@Doc:	
			Should only be called by \Ref{TransactionIf}).
		initializes after a begin transaction.  theTempTypes are created.
		 */

		static void freeTempTypes();
		/*@Doc:	
			Should only be called by \Ref{TransactionIf}).
		frees temporary types.
		 */

		static const Type* ensurePersistence(Type* type);
		/*@Doc:
			This function has to be called on all types registered as
			temporary types but getting persistent during query execution. The
			type is deleted from the tempTypes list, i.e. not freed on end of
			transaction. If a structurally equivalent type is found in the
			database it will be used and a pointer to it returned. Note that
			this currently works only for StructTypes!
		has to be called on temporary types getting persistent.
		*/

		static const short MaxBuiltInId;

	protected:
		TypeFactory();
		/*@Doc:
		constructor, can not be used from outside.
		*/
		
	private:
		static TypeFactory* myInstance;
		/*@Doc:
		pointer to instance for Singleton pattern.
		*/

		static std::vector<Type*> *theTempTypes;
		/*@Doc:
		a vector containing pointers to temporary allocated types.
		*/
		
	};

#endif
