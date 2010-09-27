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
// -*-C++-*- (for Emacs)

/*************************************************************
 *
 *
 * PURPOSE:
 *
 *
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _COMPLEXTYPE_HH_
#define _COMPLEXTYPE_HH_

#include <iostream>
#ifdef __APPLE__
#include <float.h>
#else
#include <values.h>
#endif
#include "realtype.hh"
#include "catalogmgr/ops.hh"

class OId;



/**
  * \ingroup Relcatalogifs
  */
class GenericComplexType : public AtomicType {

public:
	GenericComplexType(const char *name, unsigned int size): AtomicType(name, size) {}
	GenericComplexType(const OId& id) throw (r_Error): AtomicType(id) {}
	virtual ~GenericComplexType() {}
	virtual unsigned int getReOffset() const = 0;
	virtual unsigned int getImOffset() const = 0;

protected:
	virtual void readFromDb() throw (r_Error) = 0;
};


/**
  * \ingroup Relcatalogifs
  */
class ComplexType1 : public GenericComplexType {

public:
	ComplexType1();
	ComplexType1(const OId& id) throw (r_Error);
	ComplexType1(const ComplexType1& old);
	ComplexType1& operator=(const ComplexType1& old);
	virtual ~ComplexType1();
	virtual void printCell(ostream& stream, const char* cell) const;
	unsigned int getReOffset() const;
	unsigned int getImOffset() const;
	virtual const char* getTypeName() const;
	static const char* Name;


protected:
	virtual void readFromDb() throw (r_Error);

private:			
	unsigned int reOffset, imOffset;

//	static const char* complexTypeName;

	r_ULong* convertToCULong(const char*, r_ULong*) const;	
	char* makeFromCULong(char*, const r_ULong*) const;
	r_Long* convertToCLong(const char*, r_Long*) const;
	char* makeFromCLong(char*, const r_Long*) const;
	double* convertToCDouble(const char* cell, double* value) const;
	char* makeFromCDouble(char* cell, const double* value) const;	
	
};

/**
  * \ingroup Relcatalogifs
  */
class ComplexType2 : public GenericComplexType {

public:
	ComplexType2();
	ComplexType2(const OId& id) throw (r_Error);
	ComplexType2(const ComplexType2& old);
	ComplexType2& operator=(const ComplexType2& old);
	virtual ~ComplexType2();
	virtual void printCell(ostream& stream, const char* cell) const;
	unsigned int getReOffset() const;
	unsigned int getImOffset() const;
	virtual const char* getTypeName() const;
	static const char* Name;

protected:
	virtual void readFromDb() throw (r_Error);

private:			
	unsigned int reOffset, imOffset;
	
//	static const char* complexTypeName;

	r_ULong* convertToCULong(const char*, r_ULong*) const;	
	char* makeFromCULong(char*, const r_ULong*) const;
	r_Long* convertToCLong(const char*, r_Long*) const;
	char* makeFromCLong(char*, const r_Long*) const;
	double* convertToCDouble(const char* cell, double* value) const;
	char* makeFromCDouble(char* cell, const double* value) const;	

};


#include "complextype.icc"



#endif
