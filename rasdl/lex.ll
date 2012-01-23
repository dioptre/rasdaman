%a 3000
%p 2700
%o 5000

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
// NAME: ODL.lex
//
//
// DESCRIPTION : Pattern description for lexical analyzer generators.
//
//==============================================================================

#include <string.h>
#include <stdlib.h>

#include "parse.hh"
#include "yparse.hh"

#include "odl.h"    

/*   for error output */
long   lineNumber   =   1;   /* number of line in current file   */
int   columnNumber=   1;   /* number of column in current line */
int   commentLevel=   0;   /* Level of nested Comments         */

int    TAB_SIZE      =   3;   /* TAB size is always set to 3 */

#define ID(token,length)\
{\
   columnNumber+=length;\
   return(token);\
}\

#define SET_WHERE(WHERE)         \
{                                \
   WHERE.line   =lineNumber;     \
   WHERE.column=columnNumber;    \
   WHERE.file   ="main file";    \
}                                

#define COMMAND(token,length)       \
{                                   \
   SET_WHERE(yylval.Command.where); \
                                    \
   yylval.Command.command=token;    \
                                    \
   columnNumber+=length;            \
   return(token);                   \
}                                   

%}
%option noyywrap
%x Comment

%%

<INITIAL,Comment>"/*"                  {
                                          if(commentLevel++<1)
                                             BEGIN(Comment);

                                          columnNumber+=2;
                                       }
<Comment>"*/"                          {
                                          if(--commentLevel==0)
                                               BEGIN(INITIAL);

                                          columnNumber+=2;
                                       }
<Comment>\n                              {
                                          lineNumber++;
                                          columnNumber=1;
                                       }
<Comment>.                                {
                                          columnNumber++;
                                       }   // skip any character and count

"//".*                                   ;

";"                                    COMMAND(SEMI,1)
"{"                                    COMMAND(LPAR,1)
"}"                                    COMMAND(RPAR,1)
":"                                    COMMAND(COLON,1)
","                                    COMMAND(COMMA,1)
"*"                                    COMMAND(TIMES,1)
"typedef"                              COMMAND(TYPEDEF,7)
"float"                                COMMAND(PFLOAT,5)
"double"                               COMMAND(PDOUBLE,6)
"long"                                 COMMAND(PLONG,4)
"short"                                COMMAND(PSHORT,5)
"unsigned"                             COMMAND(PUNSIGNED,8)
"char"                                 COMMAND(PCHAR,4)
"boolean"                              COMMAND(PBOOLEAN,7)
"octet"                                COMMAND(POCTET,5)
"complexd"                             COMMAND(PCOMPLEX2,8)
"complex"                              COMMAND(PCOMPLEX1,7)
"struct"                               COMMAND(PSTRUCT,6)
"<"                                    COMMAND(LEFT,1)
">"                                    COMMAND(RIGHT,1)
"["                                    COMMAND(LEPAR,1)
"]"                                    COMMAND(REPAR,1)
"set"                                  COMMAND(SET,3)
"marray"                               COMMAND(MARRAY,6)/* FORWISS */

%{
/*
"module"                               COMMAND(MODULE,6)
"::"                                   COMMAND(DOUBLE_COLON,2)
"persistent"                           COMMAND(PERSISTENT,10)
"transient"                            COMMAND(TRANSIENT,9)
"interface"                            COMMAND(INTERFACE,9)
"("                                    COMMAND(LRPAR,1)
")"                                    COMMAND(RRPAR,1)
"extent"                               COMMAND(EXTENT,6)
"key"                                  COMMAND(KEY,3)
"keys"                                 COMMAND(KEYS,4)
"const"                                COMMAND(CONST,5)
"="                                    COMMAND(EQUAL,1)
"|"                                    COMMAND(VERT,1)
"^"                                    COMMAND(HAT,1)
"&"                                    COMMAND(AMPER,1)
">>"                                   COMMAND(DOUBLE_RIGHT,1)
"<<"                                   COMMAND(DOUBLE_LEFT,1)
"+"                                    COMMAND(PLUS,1)
"-"                                    COMMAND(MINUS,1)
"/"                                    COMMAND(SLASH,1)
"%"                                    COMMAND(PERCENT,1)
"~"                                    COMMAND(TILDE,1)
"TRUE"                                 COMMAND(TRUE,4)
"FALSE"                                COMMAND(FALSE,5)
"any"                                  COMMAND(ANY,3)
"union"                                COMMAND(UNION,5)
"switch"                               COMMAND(SWITCH,6)
"case"                                 COMMAND(CASE,4)
"default"                              COMMAND(DEFAULT,7)
"enum"                                 COMMAND(ENUM,4)
"Array"                                COMMAND(ARRAY,5)
"Sequence"                             COMMAND(SEQUENCE,8)
"String"                               COMMAND(STRING,6)
"readonly"                             COMMAND(READONLY,8)
"attribute"                            COMMAND(ATTRIBUTE,9)
"List"                                 COMMAND(LIST,4)
"Bag"                                  COMMAND(BAG,3)
"inverse"                              COMMAND(INVERSE,7)
"relationship"                         COMMAND(RELATIONSHIP,12)
"order_by"                             COMMAND(ORDER_BY,8)
"exception"                            COMMAND(EXCEPTION,9)
"oneway"                               COMMAND(ONEWAY,6)
"void"                                 COMMAND(VOID,4)
"in"                                   COMMAND(IN,2)
"out"                                  COMMAND(OUT,3)
"inout"                                COMMAND(INOUT,5)
"raises"                               COMMAND(RAISES,6)
"context"                              COMMAND(CONTEXT,7)
*/
%}

-?[0-9]+                                {
                                          yylval.LEX_integer.value=atol(yytext);

                                          SET_WHERE(yylval.LEX_integer.where);

                                          ID(IntegerLiteral,yyleng);
                                       }

%{
/*
-?[0-9]+"."[0-9]+                      {
                                          yylval.Real.value=atof(yytext);

                                          SET_WHERE(yylval.Real.where);

                                          ID(FloatingPtLiteral,yyleng);
                                       }

\"([^"]|\\["\n])*\"                    {
                                            yylval.String.string=(const char*)malloc(yyleng+1-2);
                                            strcpy((char*)yylval.String.string,yytext);

                                          SET_WHERE(yylval.String.where);

                                          ID(StringLiteral,yyleng);
                                       }

"'"[^']"'"                             {
                                          yylval.Character.character=yytext[1];

                                          SET_WHERE(yylval.Character.where);

                                          ID(CharacterLiteral,3);
                                       }
*/
%}

[a-zA-Z_][a-zA-Z0-9_]*                 {
                                            yylval.Entry.name            =(const char*)malloc(yyleng+1);

                                          SET_WHERE(yylval.Entry.where);

                                            strcpy((char*)yylval.Entry.name,yytext);

                                          ID(Identifier,yyleng);
                                        }

\n                                     {
                                          lineNumber++;
                                          columnNumber=1;
                                       }

\t                                     {
                                          columnNumber=((columnNumber-1)/TAB_SIZE+1)*TAB_SIZE+1;
                                       }

" "                                    {
                                          columnNumber++;
                                       }
