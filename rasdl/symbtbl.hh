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
#ifndef __SYMBTABLE_H
#define __SYMBTABLE_H

#include "parse.hh"

#include <string.h>

class YSymbol;

//@ManMemo: Module: {\bf rasdl}

/**
  Contains the all the symbols and the scopetree.
*/

/**
  * \ingroup Rasdls
  */
class YSymbolTable
{

    friend class YSymbol;

public:
    ///
    YSymbolTable();

    /// get the corresponding symbol to name
    const YSymbol  *get_symbol(const char*name)const;
    /// creates a symbol in the current scoped
    bool scoped_symbol(YSymbol**result,const char*name,const YWhere&where);
    /// get the symbol that defines this scope
    const YSymbol  *get_defining_symbol   ()const;

    /// search only current scope
    bool     search_scope         (const char*,YSymbol*&)const;
    /// search current scope and all abov
    bool     search_scopes        (const char*,YSymbol*&)const;
    /// search me all scopes above me, and not myself
    bool     search_scopes_above  (const YSymbol*,YSymbol*&)const;
    /// search a specified scope of a symbol
    bool     search_my_scope      (const char*,const YSymbol*,YSymbol*&)const;
    /// search the global_scope
    bool     search_global_scope  (const char*,YSymbol*&)const;

    ///
    void     insert_symbol        (YSymbol*)const;

    ///
    void           push_scope(YSymbol*);

    ///
    const YSymbol  *pop_scope();

    ///
    struct Scope
    {
        void output(FILE*out) const;
        void insertData() const;

        struct Scope      *up;
        struct Scope      *next,*son;

        YSymbol            *symbols;
        YSymbol            *last_symbol;      // last symbol defined in list {to assure correct order of symbols}

        const YSymbol      *owner;            // which symbol is the owner of this scope
    };

    ///
    Scope      *scope;

    ///
    Scope      *global_scope;

    ///
    inline bool  search_this_scope (const char*,const Scope*,YSymbol*&)const;

};



//@ManMemo: Module: {\bf rasdl}

/**
  Literal used during parse process for result of an expression.
*/
struct YLiteral
{
    ///
    enum Literal_type {dLfloat,dLinteger,dLchar,dLbool,dLstring} type;

    ///
    union
    {
        ///
        double      Real;
        ///
        long        Integer;
        ///
        const char   *String;
        ///
        char        Character;
        ///
        bool        Boolean;
    };
};



//@ManMemo: Module: {\bf rasdl}

/**
  To store constants durng parse process.
*/
struct YConstant
{
    ///
    YLiteral      value;
    ///
    Parse_type   *type;
};



//@ManMemo: Module: {\bf rasdl}

/**
  Defines a symbol during parsing. It can represent various types of data.
*/

/**
  * \ingroup Rasdls
  */
class YSymbol
{
    friend class YSymbolTable;

public:
    ///
    YSymbol();
    ///
    YSymbol(const char*);

    ///
    const char                *get_name()const
    {
        return(name);
    };

    /// defined where
    YWhere                   where;
    /// defines wether this symbol is owned by another symbol or by a scope
    bool                     owned_by_symbol;

private:
    ///
    const char                 *name;

public:
    ///
    YSymbol                         *next;
    ///
    const YSymbolTable::Scope      *scope;
    ///
    const YSymbolTable::Scope      *defines;

    ///
    enum YSymbol_type {   dParse_Type,
                          dParse_Attribute,
                          dParse_Const,
                          dParse_Function,
                          dParse_Enumerator
                      } type;
    ///
    union
    {
        ///
        Parse_type                   *Type;
        ///
        Parse_composite::Element   *Attribute;
        ///
        YConstant                  constant;

        ///
        Parse_enum::Enumerator      *enumerator;
    };
};
#endif

