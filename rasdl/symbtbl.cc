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
 *
 *
 * COMMENTS:
 *   The internal functions return 0 on error, 1 on success. Not really common,
 *   but it was used in some routines and so I stuck with it. Norbert 25-05-2001.
 *
 ************************************************************/

#include "symbtbl.hh"

#include "raslib/rmdebug.hh"

#include "debug/debug.hh"

YSymbol::YSymbol()
{
	TALK( "YSymbol::YSymbol default constructor" );

	name            =NULL;
	next            =NULL;
	owned_by_symbol=true;   // by default symbols owned by symbols

	scope            =NULL;
	defines         =NULL;
};

YSymbol::YSymbol(const char*_name)
{
	TALK( "YSymbol::YSymbol constructor, name=" << name );

	name            =_name;
	next            =NULL;
	owned_by_symbol=true;   // by default symbols owned by symbols

	scope            =NULL;
	defines         =NULL;
};

YSymbolTable::YSymbolTable()
{
	TALK( "YSymbolTable::YSymbolTable default constructor" );

	scope            =NULL;
	global_scope   =NULL;
};

void YSymbolTable::push_scope(YSymbol*owner)
{
	Scope   *new_scope=new Scope;

	new_scope->up            =scope;
	new_scope->son            =NULL;
	new_scope->symbols      =NULL;
	new_scope->last_symbol   =NULL;
	new_scope->owner         =owner;

	if(scope!=NULL)
	{
		new_scope->next    =scope->son;
		scope->son         =new_scope;

		owner->defines      =new_scope;
	}
	else
	{
		new_scope->next   =NULL;
	};

	if(owner==NULL)
		global_scope=new_scope;

	scope=new_scope;
};

const YSymbol *YSymbolTable::pop_scope()
{
	const YSymbol  *old_symbol   =scope->owner;
	scope                        =scope->up;

	return(old_symbol);
};

void   YSymbolTable::insert_symbol(YSymbol*symbol)const
{
	ENTER( "YSymbolTable::insert_symbol" );

	if(scope!=NULL)
	{
		TALK( "adding symbol " << symbol->get_name() );
		if(scope->last_symbol==NULL)
			scope->symbols=symbol;
		else
			scope->last_symbol->next=symbol;

		scope->last_symbol = symbol;

		symbol->next = NULL;
		symbol->scope = scope;
	}

	LEAVE( "YSymbolTable::insert_symbol" );
};

bool  YSymbolTable::search_this_scope(const char*name,const Scope*this_scope,YSymbol*&result)const
{
	ENTER( "YSymbolTable::search_this_scope" );

	bool found = false;

	if((name==NULL)||(this_scope==NULL))
		found = false;		/* no name or no scope */
	else
	{
		for(YSymbol*scan=this_scope->symbols;scan!=NULL && found!=true;scan=scan->next)
		{
			if(!strcmp(name,scan->name))
			{
				result=scan;
				found = true;
			}
		}
	}

	LEAVE( "YSymbolTable::search_this_scope -> " << found );
	return( found );
};

bool  YSymbolTable::search_scope(const char*name,YSymbol*&result)const
{
	return(search_this_scope(name,scope,result));
};

bool  YSymbolTable::search_scopes(const char*name,YSymbol*&result)const
{
	for(Scope*scan=scope;scan!=NULL;scan=scan->up)
	{
		if(search_this_scope(name,scan,result))
		return(true);
	};
	return(false);
};

bool  YSymbolTable::search_scopes_above(const YSymbol*symbol,YSymbol*&result)const
{
	const char*name=symbol->get_name();
	if(symbol->scope==NULL)
		return(false);

	for(Scope*scan=scope->up;scan!=NULL;scan=scan->up)
	{
		if(search_this_scope(name,scan,result))
			return(true);
	};
	return(false);
};

bool  YSymbolTable::search_my_scope(const char*name,const YSymbol*symbol,YSymbol*&result)const
{
	return(search_this_scope(name,symbol->defines,result));
};

bool  YSymbolTable::search_global_scope(const char*name,YSymbol*&result)const
{
	if(scope==NULL)
		return(false);                                 /* no scope at all */

	Scope*global;
	for(global=scope;global->up!=NULL;global=global->up)
		;   /* get up to the global scope */

	return(search_this_scope(name,global,result));
};

//****************************************************************************
//
//   name      : get_symbol
// purpose    : returns symbol of name in current scope
//   remarks    : returns NULL if no such symbol defined
//
//****************************************************************************
const YSymbol  *YSymbolTable::get_symbol(const char*name)const
{
	YSymbol*result;
	if(search_scope(name,result))
		return(result);
	else
		return(NULL);
};

//****************************************************************************
//
//   name      : scoped_symbol
// purpose    : creates symbol name within current scope
//   remarks    : returns true if success full and false if name already exists
//               *result is always been changed to new or defined symbol
//
//****************************************************************************
bool YSymbolTable::scoped_symbol(YSymbol**result,const char*name,const YWhere&where)
{
	if(search_scope(name,*result))
		return(false);   // duplicate identifier

	*result=new YSymbol(name);
	(*result)->where   =where;

	insert_symbol(*result);
	return(true);
};

const YSymbol   *YSymbolTable::get_defining_symbol()const
{
	if(scope==NULL)
		return(NULL);
	else
		return(scope->owner);
};



void YSymbolTable::Scope::output(FILE*out)const
{
	ENTER( "YSymbolTable::Scope::output" );

	YSymbol   *scan=symbols;

	while (scan!=NULL)
	{
		if(!scan->owned_by_symbol)
		{
			fprintf(out,"/*[%li,%i]*/",scan->where.line,scan->where.column);

			switch(scan->type)
			{
				case YSymbol::dParse_Type:
					scan->Type->output(out);
					break;            
				case YSymbol::dParse_Enumerator:
					scan->enumerator->output(out);
					break;            
				default:
					RMDBGONCE(0, RMDebug::module_rasdl, "YSymbolTable::Scope", "output()  bad YSymbol_type " << scan->type);            
					break;
			}
		}
		scan=(YSymbol*)scan->next;
	}

	LEAVE( "YSymbolTable::Scope::output" );
};


void YSymbolTable::Scope::insertData() const
{
	ENTER( "YSymbolTable::Scope::insertData" );

	YSymbol   *scan=symbols;

	while(scan!=NULL)
	{
		if(!scan->owned_by_symbol)
		{
			switch(scan->type)
			{
				case YSymbol::dParse_Type:
					scan->Type->insertData();
					break;            
				case YSymbol::dParse_Enumerator:
					// scan->enumerator->output(out);
					break;            
				default:
					RMDBGONCE(0, RMDebug::module_rasdl, "YSymbolTable::Scope", "output()  bad YSymbol_type " << scan->type);            
					break;
			}
		}
		scan=(YSymbol*)scan->next;
	}

	LEAVE( "YSymbolTable::Scope::insertData" );
};
