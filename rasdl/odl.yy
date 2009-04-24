%{
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
//==============================================================================
//
//
//
// DESCRIPTION : Grammar description used to parse ODL files.
//
//==============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "parse.hh"
#include "yparse.hh"
#include "symbtbl.hh"

#include "raslib/minterval.hh"

int yyparse();

extern int    yylex();
extern long   lineNumber;
extern int    columnNumber;

YSymbolTable  *Symbols=new YSymbolTable();
YSymbol       *unique_symbol(const char*name,const YWhere&);

// error handling
void     yyerror(const char* s);
extern   const YWhere *error_where;  // associatted place in source code with error is NULL if no available
extern   const YWhere *error_source; // place where source of error
extern   const char   *error_token;

// outputs an error with an argument like "...%s.." or "...%i.."
#define error_arg(OUTPUT,ARGUMENT,WHERE) \
{                                        \
   char   buffer[1024];                  \
   sprintf(buffer,OUTPUT,ARGUMENT);      \
                                         \
   error_where   =WHERE;                 \
   yyerror(buffer);                      \
};

%}

%union{
            // types for LEX
            struct
            {
               YWhere       where;
               const char   *name;
            }                     Entry;

            struct
            {
               YWhere        where;
               int         command;
            }                     Command;

            struct
            {
               YWhere       where;
               long         value;
            }                     LEX_integer;

            struct
            {
               YWhere       where;
               double      value;
            }                     Real;

            struct
            {
               YWhere      where;
               char         character;
            }                     Character;

            struct
            {
               YWhere      where;
               const char   *string;
            }                     String;

            // types for YACC
            YSymbol*                     Symbol;

            Parse_type*                  Type;
            Parse_interface*             Interface;
            Parse_composite::Element*    element;
            Parse_interface::Base_class* Base_Classes;

            Parse_interface::Lifetime    Persistence;
            Parse_enum::Enumerator*      enumerator;
            bool                         Boolean;
            rINT_list*                   INT_list;
            YLiteral                     Literal;
            YDeclarator*                 Declarator;

            // spatial domain
            r_Sinterval*                 dimension;

            struct
            { 
              r_Minterval*               domain;
              unsigned long              dimensionality;
            } domainSpec;

            r_Minterval*                 domain;

            int                          dummyValue;
        }

%token <Entry>       Identifier
%token <LEX_integer> IntegerLiteral

%token <Command>     SEMI
                     LPAR
                     RPAR
                     COLON
                     COMMA
                     TIMES
                     TYPEDEF
                     PFLOAT
                     PDOUBLE
                     PLONG
                     PSHORT
                     PUNSIGNED
                     PCHAR
                     PBOOLEAN
                     POCTET
                     PCOMPLEX1
                     PCOMPLEX2
                     PSTRUCT
                     LEFT
                     RIGHT
                     LEPAR
                     REPAR
                     SET
                     MARRAY

/* types of nonterminals */
%type<dummyValue>    model,specification,definition;
%type <Literal>      literal,primary_expr
%type <element>      member,member_list
%type <Symbol>       scoped_name
// enum_name
%type <Type>         type_spec,simple_type_spec,type_declarator,
                     floating_pt_type,integer_type,signed_int,signed_long_int,
                     signed_short_int,unsigned_int,unsigned_long_int,
                     unsigned_short_int,char_type,boolean_type,octet_type,complex1_type,complex2_type,
                     struct_type,marray_type, type_dcl,constr_type_spec,
                     opt_marray_base,collection_type,base_type_spec,
                     template_type_spec
%type <Declarator>   declarator,declarators,simple_declarator

%type   <dimension>      dimension_spec
%type   <domainSpec>     opt_spatial_domain
%type   <domain>         spatial_domain,dimension_spec_list

%%

/*(0)*/
model                 : { Symbols->push_scope(NULL); }
                        specification
                        { Symbols->pop_scope(); }

/*(1)*/
specification         : definition
                      | definition specification

/*(2)*/
definition            : type_dcl   SEMI { $$ = NULL; }

/*(11)*/
/*<Symbol>*/
scoped_name           : Identifier
               {
                  if(!Symbols->search_scopes($1.name,$$))
                     error_arg("undefined symbol[%s]",$1.name,&$1.where);
               }

/*(23)*/
/*<Literal>*/
primary_expr          : literal { $$ = $1; }

/*(24)*/
/*<Literal>*/
literal               : IntegerLiteral
            {
               $$.Integer  =$1.value;
               $$.type     =YLiteral::dLinteger;
            }

/*(27)*/
type_dcl              : TYPEDEF type_declarator { $$ = $2; }
                      | struct_type

/*(28)*/
type_declarator       : type_spec declarators
                  {
                     // for each declarator there exists a symbol
                     // therefore generate only aliases
                     for(YDeclarator*scan=$2;scan!=NULL;scan=scan->next)
                     {
                        Parse_alias *alias=new Parse_alias;

                        alias->type=$1;
                        alias->name=scan->symbol->get_name();
                        alias->symbol=scan->symbol;

                        scan->symbol->Type            =alias;
                        scan->symbol->type            =YSymbol::dParse_Type;
                        scan->symbol->owned_by_symbol   =false;   // never owned by symbol
                     };
                  }

/*(29)*/
/*<Type>*/
type_spec             : simple_type_spec
                      | constr_type_spec
               {
                  Parse_typereference *reference=new Parse_typereference;
                  reference->type=$1;
                  reference->setParseInfo( $1->getParseInfo() );

                  $$=reference;
               }

/*(30)*/
/*<Type>*/
simple_type_spec      : base_type_spec
                      | template_type_spec
                      | scoped_name
                  {
                     Parse_typereference *reference=new Parse_typereference;
                     reference->type=$1->Type;
                     reference->setParseInfo( Parse_info( $1->where, $1->get_name() ) );

                     $$=reference;
                  }

/*(31)*/
/*<Type>*/
base_type_spec        : floating_pt_type
                      | integer_type
                      | char_type
                      | boolean_type
                      | octet_type
                      | complex1_type
                      | complex2_type

/*(32)*/
/*<Type>*/
template_type_spec    : collection_type
                      | marray_type         

/*(32a)*/
/*<Type>*/
collection_type       : SET LEFT simple_type_spec RIGHT
                        {
                          $$=new Parse_set;
                          $$->setParseInfo( Parse_info( $1.where, "Set" ) );
                          ((Parse_set*)$$)->base_type=$3;
                        };

/*(33)*/
/*<Type>*/
constr_type_spec      : struct_type

/*(34)*/
/*<Declarator>*/
declarators           : declarator
                      | declarator COMMA declarators
               {
                  $1->next   =$3;
                  $$         =$1;
               };

/*(35)*/
/*<Declarator>*/
declarator            : simple_declarator

/*(36)*/
/*<Declarator>*/
simple_declarator     : Identifier
                     {
                        YSymbol  *symbol=unique_symbol($1.name,$1.where);

                        $$=new YDeclarator;
                        $$->next       =NULL;
                        $$->symbol     =symbol;
                        $$->array_size =NULL;
                     }

/*(38)*/
/*<Type>*/
floating_pt_type      : PFLOAT
                     {
                        Parse_float *real=new Parse_float;
                        real->accurance=Parse_float::Single;
                        real->setParseInfo( Parse_info( $1.where, "float") );

                        $$=real;
                     }
                      | PDOUBLE
                     {
                        Parse_float *real=new Parse_float;
                        real->accurance=Parse_float::Double;
                        real->setParseInfo( Parse_info( $1.where, "double") );

                        $$=real;
                     }

/*(39)*/
/*<Type>*/
integer_type          : signed_int
                      | unsigned_int

/*(40)*/
/*<Type>*/
signed_int            : signed_long_int
                      | signed_short_int

/*(41)*/
/*<Type>*/
signed_long_int       : PLONG
                  {
                     Parse_int *integer=new Parse_int;

                     integer->width  =Parse_int::Long;
                     integer->sign   =Parse_int::Signed;
                     integer->setParseInfo( Parse_info( $1.where, "long") );

                     $$=integer;
                  }

/*(42)*/
/*<Type>*/
signed_short_int      : PSHORT
                  {
                     Parse_int *integer=new Parse_int;

                     integer->width  =Parse_int::Short;
                     integer->sign   =Parse_int::Signed;
                     integer->setParseInfo( Parse_info( $1.where, "short") );

                     $$=integer;
                  }

/*(43)*/
/*<Type>*/
unsigned_int          : unsigned_long_int
                      | unsigned_short_int

/*(44)*/
/*<Type>*/
unsigned_long_int     : PUNSIGNED PLONG
                      {
                        Parse_int   *integer=new Parse_int;

                        integer->width   =Parse_int::Long;
                        integer->sign   =Parse_int::Unsigned;
                        integer->setParseInfo( Parse_info( $1.where, "unsigned long") );

                        $$=integer;
                      }

/*(45)*/
/*<Type>*/
unsigned_short_int    : PUNSIGNED PSHORT
                      {
                        Parse_int   *integer=new Parse_int;

                        integer->width   =Parse_int::Short;
                        integer->sign   =Parse_int::Unsigned;
                        integer->setParseInfo( Parse_info( $1.where, "unsigned short") );

                        $$=integer;
                      }

/*(46)*/
/*<Type>*/
char_type             : PCHAR
            {
               $$=new Parse_char;
               $$->setParseInfo( Parse_info( $1.where, "char") );
            }

/*(47)*/
/*<Type>*/
boolean_type          : PBOOLEAN
               {
                  $$=new Parse_boolean;
                  $$->setParseInfo( Parse_info( $1.where, "boolean") );
               }

/*(48)*/
/*<Type>*/
octet_type            : POCTET
            {
               $$=new Parse_octet;
               $$->setParseInfo( Parse_info( $1.where, "octet") );
            }

/*(48a)*/
/*<Type>*/
complex1_type            : PCOMPLEX1
            {
               $$=new Parse_complex1;
               $$->setParseInfo( Parse_info( $1.where, "complex") );
            }


/*(48b)*/
/*<Type>*/
complex2_type            : PCOMPLEX2
            {
               $$=new Parse_complex2;
               $$->setParseInfo( Parse_info( $1.where, "complexd") );
            }

/*(50)*/
/*<Type>*/
struct_type           : PSTRUCT Identifier
               {
                  YSymbol *symbol=unique_symbol($2.name,$1.where);

                  symbol->type            =YSymbol::dParse_Type;
                  symbol->Type            =new Parse_struct;
                  symbol->Type->symbol    =symbol;
                  symbol->owned_by_symbol   =false;   // never owned by a symbol

                  symbol->Type->setParseInfo( Parse_info( $1.where, "struct") );

                  Symbols->push_scope(symbol);
               }
                        LPAR member_list RPAR
               {
                  const YSymbol *symbol=Symbols->pop_scope();

                  $$=symbol->Type;

                  $$->name       =symbol->get_name();
                  ((Parse_struct*)$$)->elements =$5;
               }

/*(51)*/
/*<Element>*/
member_list           : member
                      | member member_list
               {
                  Parse_composite::Element *scan=$1;
                  while(scan->next!=NULL)
                     scan=scan->next;

                  scan->next=$2;
                  $$=$1;
               }

/*(52)*/
/*<Element>*/
member                : type_spec declarators SEMI
         {
            YDeclarator                *scan=$2;
            Parse_composite::Element   *current=NULL;

            for(;scan!=NULL;scan=scan->next)
            {
               if(current==NULL)
               {
                  current=new Parse_composite::Element;
                  $$=current;
               }
               else
               {
                  current->next=new Parse_composite::Element;
                  current=current->next;
               };

               current->type=$1;
               current->name=scan->symbol->get_name();
            };
         };

/*(75)*/
/*<Type>*/
marray_type           : MARRAY LEFT
                        opt_marray_base
                        opt_spatial_domain
                        RIGHT
                        {
                          $$=new Parse_MDD;

                          ((Parse_MDD*)$$)->base_type      =$3;
                          ((Parse_MDD*)$$)->domain         =$4.domain;
                          ((Parse_MDD*)$$)->dimensionality =$4.dimensionality;
                        }

/*<Type>*/
opt_marray_base       : { $$=NULL; }
                      | simple_type_spec

/*<domain>*/
opt_spatial_domain    :                       { $$.domain=NULL; $$.dimensionality = 0; }
                      | COMMA primary_expr    { $$.domain=NULL; $$.dimensionality = $2.Integer; }
                      | COMMA spatial_domain  { $$.domain=$2;   $$.dimensionality = 0;  };

/*(76)*/
/*<domain>*/
spatial_domain        : LEPAR dimension_spec_list REPAR { $$ = $2; }


/*(77)*/
/*<dimension>*/
dimension_spec_list   : dimension_spec 
                        {
                          $$ = new r_Minterval(1);
                          (*$$)[0] = (*$1);
                          delete $1; $1=NULL;
                        } 
                      | dimension_spec COMMA dimension_spec_list
                        {
                          $$ = new r_Minterval( $3->dimension() + 1 );

                          (*$$)[0] = (*$1);
                          delete $1; $1=NULL;

                          for( int i=0; i<$3->dimension(); i++ )
                            (*$$)[i+1] = (*$3)[i];

                          delete $3; $3=NULL;
                        }


/*(78)*/
/*<dimension>*/
dimension_spec        : primary_expr COLON primary_expr { $$=new r_Sinterval( (r_Range)$1.Integer, (r_Range)$3.Integer ); }
                      | TIMES        COLON primary_expr { $$=new r_Sinterval( '*'       , (r_Range)$3.Integer ); }
                      | primary_expr COLON TIMES        { $$=new r_Sinterval( (r_Range)$1.Integer, '*'        ); }
                      | TIMES        COLON TIMES        { $$=new r_Sinterval( '*'       , '*'        ); }


%%
//****************************************************************************
//
//   name      : unique_symbol
// purpose    : tries to create an unique symbol within scope
//   remarks    : calls yyerror if name already exists in scope
//
//****************************************************************************
YSymbol  *unique_symbol(const char*name,const YWhere&where)
{
   YSymbol  *symbol;

   if(!Symbols->scoped_symbol(&symbol,name,where))
   {
      error_source=&symbol->where;
      error_token   =name;

      error_arg("[%s] symbol already defined within scope",name,&where);
   };

   return(symbol);
};

const YWhere   *error_source  =NULL;
const YWhere   *error_where   =NULL;
const   char   *error_token   =NULL;
// associatted place in source code with error
// is NULL if no available

void yyerror(const char* s)
{
   std::cout<<"error!\n";

   if(error_where!=NULL)
   {
       std::cerr<<"\n  pos : line "<<error_where->line<<" colmun "<<error_where->column;

      if(error_where->file!=NULL)
         std::cerr<<" in \""<<error_where->file<<"\"";

      std::cerr<<".\n";
   }
   else
      std::cerr<<"\n near : line "<<lineNumber<<" column "<<columnNumber<<".\n";

   if(error_token!=NULL)
      std::cerr<<"at token <"<<error_token<<"> ";

   std::cerr<<"error : "<<s<<".\n";

   if(error_source!=NULL)
   {
      std::cerr<<"source: line "<<error_source->line<<" column "<<error_source->column;

      if(error_source->file!=NULL)
         std::cerr<<" in \""<<error_source->file<<"\"";

      std::cerr<<".\n";
   };

   error_where =NULL;
   error_source=NULL;

   exit(-2);
};

