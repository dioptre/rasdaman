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
#ifndef __PARSE_H
#define __PARSE_H

// Put it in front of any typedef bool ... because o2 is using bool as a variable.
// #include "o2template_CC.hxx"

#include "raslib/minterval.hh"
#include "raslib/error.hh"

#include "relcatalogif/basetype.hh"
#include "relcatalogif/type.hh"

#include <stdio.h>

class YSymbol;

typedef Type CType;

//@ManMemo: Module: {\bf rasdl}

/**
  C structure representing a position in source.
*/
struct YWhere
{
    ///
    long        line;
    ///
    int         column;
    ///
    const char* file;
};



//@ManMemo: Module: {\bf rasdl}

/**
  C++ structure representing a position in source.
*/
/**
  * \defgroup Rasdls Rasdl Classes
  */

/**
  * \ingroup Rasdls
  */
class Parse_info
{
public:
    ///
    Parse_info();

    ///
    Parse_info( long lineNo, int columnNo, const char* fileName, const char* tokenName );

    ///
    Parse_info( YWhere &where, const char* tokenName );

    /// copy constructor
    Parse_info( const Parse_info& obj );

    /// destructor
    ~Parse_info();

    /// assignment operator
    const Parse_info& operator=( const Parse_info& obj );

    ///
    long        line;
    ///
    int         column;
    ///
    char* file;
    ///
    char* token;
};



//@ManMemo: Module: {\bf rasdl}

/**
 Base class for all types which can easily be converted to numbers.
*/

/**
  * \ingroup Rasdls
  */
class Parse_number
{
};



//@ManMemo: Module: {\bf rasdl}

/**
 Virtual base class for all objects with kind and output.
*/

/**
  * \ingroup Rasdls
  */
class Parse_atom
{
public:
    ///
    Parse_atom();
    ///
    virtual ~Parse_atom();
    ///
    virtual   void   output(FILE*)const=0;
    ///
    virtual void insertData() const throw( r_Equery_execution_failed );

    ///
    void setParseInfo( const Parse_info &token );

    ///
    const Parse_info& getParseInfo();

    ///
    enum Kind
    {
        Atom,Type,
        Typedefinition,
        Typereference,

        Composite,
        Struct,Union,Interface,

        Function,
        Operation,

        Pointer,
        Array,
        Alias,

        Enum,

        Atomic,
        Any,Void,Boolean,Float,Integer,Char,Octet,Complex1,Complex2,String,

        Atomic_template,
        Domain,Set,MDD
    };

    ///
    Kind            kind;
    ///
    const char      *name;
    ///
    YSymbol         *symbol;

    /// definition of corresponding token
    Parse_info parseInfo;
};



//@ManMemo: Module: {\bf rasdl}

/**
  Virtual base class for all types.
*/

/**
  * \ingroup Rasdls
  */
class Parse_type : public Parse_atom
{
public:
    ///
    Parse_type();
    ///
    virtual ~Parse_type();
    ///
    Parse_type(char*);

    ///
    virtual const CType* getType( const char* typeName = NULL ) const;

    /// this type is defined as forward   {should be moved into YSymbol}
    bool   forward;
};




//@ManMemo: Module: {\bf rasdl}

/**
  Virtual base class for all types that create a new one.
*/

/**
  * \ingroup Rasdls
  */
class Parse_typedefinition : public Parse_type
{
};



//@ManMemo: Module: {\bf rasdl}

/**
  Represents a reference to a type.
*/

/**
  * \ingroup Rasdls
  */
class Parse_typereference : public Parse_type
{
public:
    ///
    Parse_typereference();
    ///
    virtual ~Parse_typereference();
    ///
    virtual void output(FILE*)const;

    ///
    virtual const CType* getType( const char* typeName = NULL ) const;

    ///
    const Parse_type   *type;
};



//@ManMemo: Module: {\bf rasdl}

/**
  Virtual base class for all types that are a kind of composite.
*/

/**
  * \ingroup Rasdls
  */
class Parse_composite : public Parse_typedefinition
{
public:
    ///
    Parse_composite();

    ///
    enum Access_mode {Private,Public,Protected};

    ///
    class Element : public Parse_atom
    {
    public:
        ///
        Element();
        ///
        virtual ~Element();
        ///
        virtual void output(FILE*)const;

        ///
        bool         readonly;
        ///
        const Parse_type   *type;
        ///
        Access_mode   access;

        ///
        Element      *next;
    };

    ///
    Element   *elements;
};



//@ManMemo: Module: {\bf rasdl}

/**
  Class that represents the STRUCT type.
*/

/**
  * \ingroup Rasdls
  */
class Parse_struct : public Parse_composite
{
public:
    ///
    Parse_struct();
    ///
    virtual ~Parse_struct();
    ///
    virtual void output(FILE*)const;
    ///
    virtual void insertData() const throw( r_Equery_execution_failed );
    ///
    virtual const CType* getType( const char* typeName = NULL ) const;
};



//@ManMemo: Module: {\bf rasdl}

/**
  Class that represents the UNION type.
*/

/**
  * \ingroup Rasdls
  */
class Parse_union : public Parse_composite
{
    ///
    Parse_union();
    ///
    virtual ~Parse_union();
    ///
    virtual void output(FILE*)const;
};


class Parse_operation;


//@ManMemo: Module: {\bf rasdl}

/**
  class that represents the INTERFACE type
*/

/**
  * \ingroup Rasdls
  */
class Parse_interface : public Parse_composite
{
public:
    ///
    Parse_interface();
    ///
    virtual void output(FILE*)const;

    ///
    enum Lifetime {persistend,transient,undefined};

    ///

    /**
      * \ingroup Rasdls
      */
    class Base_class : public Parse_atom
    {
    public:
        ///
        Base_class();
        ///
        virtual void output(FILE*)const;

        ///
        Parse_interface   *base_class;
        ///
        Access_mode           access;

        ///
        Base_class         *next;
    };

    ///

    /**
      * \ingroup Rasdls
      */
    class Method : public Parse_atom
    {
    public:
        ///
        Method();
        ///
        virtual   void   output(FILE*)const;

        ///
        Parse_operation   *function;

        ///
        Access_mode               access;

        ///
        Method                  *next;
    };

    ///
    Base_class         *base_classes;
    ///
    Lifetime            lifetime;
    ///
    Method            *methods;
    ///
    void                 *relationships;
};



//@ManMemo: Module: {\bf rasdl}

/**
  Represents a simple function with parameters and return value.
*/

/**
  * \ingroup Rasdls
  */
class Parse_function : public Parse_atom
{
public:
    ///
    Parse_function();
    ///
    virtual   void   output(FILE*)const;

    ///

    /**
      * \ingroup Rasdls
      */
    class Parameter : public Parse_atom
    {
    public:
        ///
        Parameter();
        ///
        virtual   void   output(FILE*)const;

        ///
        Parse_typereference      *type;
        ///
        enum   {In,Out,Unknown}   state;

        ///
        Parameter               *next;
    };

    ///
    Parameter            *parameters;
    ///
    Parse_typereference   *return_type;
};



//@ManMemo: Module: {\bf rasdl}

/**
  Is a operation of an Parse_interface.
*/

/**
  * \ingroup Rasdls
  */
class Parse_operation : public Parse_function
{
public:
    ///
    Parse_operation();

    ///
    Parse_interface   *scope_class;
};



//@ManMemo: Module: {\bf rasdl}

/**
  Class that represents pointer like types.
*/

/**
  * \ingroup Rasdls
  */
class Parse_pointer : public Parse_typedefinition
{
public:
    ///
    Parse_pointer();
    ///
    virtual void output(FILE*)const;

    ///
    const Parse_type   *type;
};



//@ManMemo: Module: {\bf rasdl}

/**
  Class that represents an ARRAY.
*/

/**
  * \ingroup Rasdls
  */
class Parse_array : public Parse_pointer
{
public:
    ///
    Parse_array();
    ///
    virtual void output(FILE*)const;
    ///
    int         size;
};



//@ManMemo: Module: {\bf rasdl}

/**
  Gives a type another name.
*/

/**
  * \ingroup Rasdls
  */
class Parse_alias : public Parse_typedefinition
{
public:
    ///
    Parse_alias();
    ///
    virtual void output(FILE*)const;
    ///
    virtual void insertData() const throw( r_Equery_execution_failed );
    ///
    const Parse_type   *type;
};



//@ManMemo: Module: {\bf rasdl}

/**
*/

/**
  * \ingroup Rasdls
  */
class Parse_enum : public Parse_typedefinition,public Parse_number
{
public:
    ///
    Parse_enum();
    ///
    virtual void output(FILE*)const;

    ///

    /**
      * \ingroup Rasdls
      */
    class Enumerator : public Parse_atom
    {
    public:
        ///
        virtual void output(FILE*)const;

        ///
        const char    *name;
        ///
        int         value;
        ///
        Enumerator   *next;
    };

    ///
    Enumerator   *enumerators;
};



//@ManMemo: Module: {\bf rasdl}

/**
*/

/**
  * \ingroup Rasdls
  */
class Parse_atomic : public Parse_typedefinition
{
public:
    ///
    virtual void output(FILE*)const;
};



//@ManMemo: Module: {\bf rasdl}

/**
*/

/**
  * \ingroup Rasdls
  */
class Parse_any : public Parse_atomic
{
public:
    ///
    Parse_any();
};



//@ManMemo: Module: {\bf rasdl}

/**
*/

/**
  * \ingroup Rasdls
  */
class Parse_void : public Parse_atomic
{
public:
    ///
    Parse_void();
};



//@ManMemo: Module: {\bf rasdl}

/**
*/

/**
  * \ingroup Rasdls
  */
class Parse_string : public Parse_atomic
{
public:
    ///
    Parse_string();

    ///
    int   length;
};



//@ManMemo: Module: {\bf rasdl}

/**
  A fractional number.
*/

/**
  * \ingroup Rasdls
  */
class Parse_float : public Parse_atomic,public Parse_number
{
public:
    ///
    Parse_float();
    ///
    virtual void output(FILE*)const;
    ///
    virtual const CType* getType( const char* typeName = NULL ) const;

    ///
    enum {Double,Single}   accurance;
};



//@ManMemo: Module: {\bf rasdl}

/**
 Represents a non-fractional number signed/unsigned or 16/32 number.
*/

/**
  * \ingroup Rasdls
  */
class Parse_int : public Parse_atomic,public Parse_number
{
public:
    ///
    Parse_int();
    ///
    virtual void output(FILE*)const;
    ///
    virtual const CType* getType( const char* typeName = NULL ) const;

    ///
    enum {Short,Long}       width;
    ///
    enum {Unsigned,Signed}   sign;
};



//@ManMemo: Module: {\bf rasdl}

/**
 An 8-bit unsigned number.
*/

/**
  * \ingroup Rasdls
  */
class Parse_octet : public Parse_atomic,public Parse_number
{
public:
    ///
    Parse_octet();
    ///
    virtual const CType* getType( const char* typeName = NULL ) const;
};

//@ManMemo: Module: {\bf rasdl}


/**
  * \ingroup Rasdls
  */
class Parse_complex1 : public Parse_atomic,public Parse_number
{
public:
    ///
    Parse_complex1();
    ///
    virtual const CType* getType( const char* typeName = NULL ) const;
};


/**
  * \ingroup Rasdls
  */
class Parse_complex2 : public Parse_atomic,public Parse_number
{
public:
    ///
    Parse_complex2();
    ///
    virtual const CType* getType( const char* typeName = NULL ) const;
};




//@ManMemo: Module: {\bf rasdl}

/**
  Represents an 8-bit unsigned number.
*/

/**
  * \ingroup Rasdls
  */
class Parse_char : public Parse_atomic,public Parse_number
{
public:
    ///
    Parse_char();
    ///
    virtual const CType* getType( const char* typeName = NULL ) const;
};



//@ManMemo: Module: {\bf rasdl}

/**
*/

/**
  * \ingroup Rasdls
  */
class Parse_boolean : public Parse_atomic,public Parse_number
{
public:
    ///
    Parse_boolean();
    ///
    virtual const CType* getType( const char* typeName = NULL ) const;
};



//@ManMemo: Module: {\bf rasdl}

/**
*/

/**
  * \ingroup Rasdls
  */
class Parse_atomic_templates : public Parse_atomic
{
public:
    ///
    Parse_atomic_templates();
    ///
    const Parse_type   *base_type;
};



//@ManMemo: Module: {\bf rasdl}

/**
*/

/**
  * \ingroup Rasdls
  */
class Parse_set : public Parse_atomic_templates
{
public:
    ///
    Parse_set();
    ///
    virtual void output(FILE*)const;
    ///
    virtual const CType* getType( const char* typeName = NULL ) const;
};



//@ManMemo: Module: {\bf rasdl}

/**
*/

/**
  * \ingroup Rasdls
  */
class Parse_MDD : public Parse_atomic_templates
{
public:
    ///
    Parse_MDD();

    ///
    virtual void output(FILE*)const;

    ///
    virtual const CType* getType( const char* typeName = NULL ) const;

    ///
    r_Minterval* domain;

    ///
    unsigned long dimensionality;
};
#endif
























