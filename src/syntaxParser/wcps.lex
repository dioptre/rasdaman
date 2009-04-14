/*
 * This file is part of %PACKAGENAME%.
 *
 * %PACKAGENAME% is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * %PACKAGENAME% is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with %PACKAGENAME%. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.%PACKAGENAME%.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */


/*

	This is the JLex specification file for the WCPS abstract grammar. The JLex executable produces a Scanner.java source file which encapsulated
	all symbol and token definition and which is needed by the Java CUP parser
	
	This file specifies all possible tokens / symbols that may appear in a WCPS request
	
	New symbols may be added following the same syntax standard
	
*/


/*
	Required declarations for source generator.
	Make sure you know what you are doing if you wish to modify these declarations
*/

package syntaxParser;
import java_cup.runtime.*;
import java.io.*;
%%
%notunix
%implements java_cup.runtime.Scanner
%type Symbol
%function next_token
%public
%class WCPSScanner
%eofval{ return null;
%eofval}
%line
%char

IDENTIFIER = [a-zA-Z][a-zA-Z0-9_]*
FLOATNUMBER = [-+]?[0-9]+[.]?[0-9]+
NUMBER = [0-9]+
STRING = [a-zA-Z]* 


%%
"+" { return new Symbol(WCPSSymbol.PLUS, yyline, yychar, yytext()); }
"*" { return new Symbol(WCPSSymbol.MULTI, yyline, yychar, yytext()); }
"/" { return new Symbol(WCPSSymbol.DIVIDE, yyline, yychar, yytext()); }
"-" { return new Symbol(WCPSSymbol.MINUS, yyline, yychar, yytext()); }
"(" { return new Symbol(WCPSSymbol.LPAREN, yyline, yychar, yytext()); }
")" { return new Symbol(WCPSSymbol.RPAREN, yyline, yychar, yytext()); }
"{" { return new Symbol(WCPSSymbol.LBRACE, yyline, yychar, yytext()); }
"}" { return new Symbol(WCPSSymbol.RBRACE, yyline, yychar, yytext()); }
"struct" { return new Symbol(WCPSSymbol.STRUCT, yyline, yychar, yytext()); }
"sin" { return new Symbol(WCPSSymbol.SIN, yyline, yychar, yytext()); }
"cos" { return new Symbol(WCPSSymbol.COS, yyline, yychar, yytext()); }
"tan" { return new Symbol(WCPSSymbol.TAN, yyline, yychar, yytext()); }
"sinh" { return new Symbol(WCPSSymbol.SINH, yyline, yychar, yytext()); }
"cosh" { return new Symbol(WCPSSymbol.COSH, yyline, yychar, yytext()); }
"tanh" { return new Symbol(WCPSSymbol.TANH, yyline, yychar, yytext()); }
"arcsin" { return new Symbol(WCPSSymbol.ARCSIN, yyline, yychar, yytext()); }
"arccos" { return new Symbol(WCPSSymbol.ARCCOS, yyline, yychar, yytext()); }
"arctan" { return new Symbol(WCPSSymbol.ARCTAN, yyline, yychar, yytext()); }
"exp" { return new Symbol(WCPSSymbol.EXP, yyline, yychar, yytext()); }
"log" { return new Symbol(WCPSSymbol.LOG, yyline, yychar, yytext()); }
"ln" { return new Symbol(WCPSSymbol.LN, yyline, yychar, yytext()); }
"abs" { return new Symbol(WCPSSymbol.ABS, yyline, yychar, yytext()); }
"sqrt" { return new Symbol(WCPSSymbol.SQRT, yyline, yychar, yytext()); }
":" { return new Symbol(WCPSSymbol.COLON, yyline, yychar, yytext()); }
"." { return new Symbol(WCPSSymbol.DOT, yyline, yychar, yytext()); }
"return" { return new Symbol(WCPSSymbol.RETURN, yyline, yychar, yytext()) ;}
"for" { return new Symbol(WCPSSymbol.FOR, yyline, yychar, yytext()) ;}
"in" { return new Symbol(WCPSSymbol.IN, yyline, yychar, yytext()) ; }
"," { return new Symbol(WCPSSymbol.COMMA, yyline, yychar, yytext()); }
"encode" { return new Symbol(WCPSSymbol.ENCODE, yyline, yychar, yytext()) ; }
"condense" { return new Symbol(WCPSSymbol.CONDENSE, yyline, yychar, yytext()) ; }
"over" { return new Symbol(WCPSSymbol.OVER, yyline, yychar, yytext()) ; }
"using" { return new Symbol(WCPSSymbol.USING, yyline, yychar, yytext()) ; }
"max" { return new Symbol(WCPSSymbol.MAX, yyline, yychar, yytext()) ; }
"min" { return new Symbol(WCPSSymbol.MIN, yyline, yychar, yytext()) ; }
"and" { return new Symbol(WCPSSymbol.AND, yyline, yychar, yytext()) ; }
"or" { return new Symbol(WCPSSymbol.OR, yyline, yychar, yytext()) ; }
"setCrsSet" { return new Symbol(WCPSSymbol.SETCRSSET, yyline, yychar, yytext()); }
"nearest" { return new Symbol(WCPSSymbol.NEAREST, yyline, yychar, yytext()) ; }
"cTransform" { return new Symbol(WCPSSymbol.CTRANSFORM, yyline, yychar, yytext()) ; }
"scale" { return new Symbol(WCPSSymbol.SCALE, yyline, yychar, yytext()) ; }
"count" { return new Symbol(WCPSSymbol.COUNT, yyline, yychar, yytext()) ; }
"all" { return new Symbol(WCPSSymbol.ALL, yyline, yychar, yytext()) ; }
"add" { return new Symbol(WCPSSymbol.ADD, yyline, yychar, yytext()) ; }
"some" { return new Symbol(WCPSSymbol.SOME, yyline, yychar, yytext()) ; }
"avg" { return new Symbol(WCPSSymbol.AVG, yyline, yychar, yytext()) ; }
"bool" { return new Symbol(WCPSSymbol.BOOL, yyline, yychar, yytext()) ; }
"char" { return new Symbol(WCPSSymbol.CHAR, yyline, yychar, yytext()) ; }
"unsigned char" { return new Symbol(WCPSSymbol.UNSIGNEDCHAR, yyline, yychar, yytext()) ; }
"unsigned short" { return new Symbol(WCPSSymbol.UNSIGNEDSHORT, yyline, yychar, yytext()) ; }
"short" { return new Symbol(WCPSSymbol.SHORT, yyline, yychar, yytext()) ; }
"long" { return new Symbol(WCPSSymbol.LONG, yyline, yychar, yytext()) ; }
"unsigned long" { return new Symbol(WCPSSymbol.UNSIGNEDLONG, yyline, yychar, yytext()) ; }
"float" { return new Symbol(WCPSSymbol.FLOAT, yyline, yychar, yytext()) ; }
"double" { return new Symbol(WCPSSymbol.DOUBLE, yyline, yychar, yytext()) ; }
"not" { return new Symbol(WCPSSymbol.NOT, yyline, yychar, yytext()) ; }
"bit" { return new Symbol(WCPSSymbol.BIT, yyline, yychar, yytext()) ; }
"xor" { return new Symbol(WCPSSymbol.XOR, yyline, yychar, yytext()) ; }
"=" { return new Symbol(WCPSSymbol.EQUALS, yyline, yychar, yytext()) ; }
"!=" { return new Symbol(WCPSSymbol.NOTEQUALS, yyline, yychar, yytext()) ; }
"overlay" { return new Symbol(WCPSSymbol.OVERLAY, yyline, yychar, yytext()) ; }
">" { return new Symbol(WCPSSymbol.GT, yyline, yychar, yytext()) ; }
"<" { return new Symbol(WCPSSymbol.LT, yyline, yychar, yytext()) ; }
"<=" { return new Symbol(WCPSSymbol.LTE, yyline, yychar, yytext()) ; }
">=" { return new Symbol(WCPSSymbol.GTE, yyline, yychar, yytext()) ; }
"[" { return new Symbol(WCPSSymbol.LBRACKET, yyline, yychar, yytext()) ; }
"]" { return new Symbol(WCPSSymbol.RBRACKET, yyline, yychar, yytext()) ; }
"trim" { return new Symbol(WCPSSymbol.TRIM, yyline, yychar, yytext()) ; }
"true" { return new Symbol(WCPSSymbol.TRUE, yyline, yychar, yytext()) ; }
"false" { return new Symbol(WCPSSymbol.FALSE, yyline, yychar, yytext()) ; }
"where" { return new Symbol(WCPSSymbol.WHERE, yyline, yychar, yytext()); }
"store" { return new Symbol(WCPSSymbol.STORE, yyline, yychar, yytext()); }
"identifier" { return new Symbol(WCPSSymbol.IDENTIFIER, yyline, yychar, yytext()); }
"imageCrs" { return new Symbol(WCPSSymbol.IMAGECRS, yyline, yychar, yytext()); }
"imageCrsDomain" { return new Symbol(WCPSSymbol.IMAGECRSDOMAIN, yyline, yychar, yytext()); }
"crsSet" { return new Symbol(WCPSSymbol.CRSSET, yyline, yychar, yytext()); }
"generalDomain" { return new Symbol(WCPSSymbol.GENERALDOMAIN, yyline, yychar, yytext()); }
"nullDefault" { return new Symbol(WCPSSymbol.NULLDEFAULT, yyline, yychar, yytext()); }
"nullSet" { return new Symbol(WCPSSymbol.NULLSET, yyline, yychar, yytext()); }
"interpolationDefault" { return new Symbol(WCPSSymbol.INTERPOLATIONDEFAULT, yyline, yychar, yytext()); }
"interpolationSet" { return new Symbol(WCPSSymbol.INTERPOLATIONSET, yyline, yychar, yytext()); }
"setNullDefault" { return new Symbol(WCPSSymbol.SETNULLDEFAULT, yyline, yychar, yytext()); }
"setNullSet" { return new Symbol(WCPSSymbol.SETNULLSET, yyline, yychar, yytext()); }
"setInterpolationDefault" { return new Symbol(WCPSSymbol.SETINTERPOLATIONDEFAULT, yyline, yychar, yytext()); }
"setInterpolationSet" { return new Symbol(WCPSSymbol.SETINTERPOLATIONSET, yyline, yychar, yytext()); }
"complex" { return new Symbol(WCPSSymbol.COMPLEX, yyline, yychar, yytext()); }
"complex2" { return new Symbol(WCPSSymbol.COMPLEX2, yyline, yychar, yytext()); }
"slice" { return new Symbol(WCPSSymbol.SLICE, yyline, yychar, yytext()); }
"extend" { return new Symbol(WCPSSymbol.EXTEND, yyline, yychar, yytext()); }
"linear" { return new Symbol(WCPSSymbol.LINEAR, yyline, yychar, yytext()) ; }
"quadratic" { return new Symbol(WCPSSymbol.QUADRATIC, yyline, yychar, yytext()); }
"cubic" { return new Symbol(WCPSSymbol.CUBIC, yyline, yychar, yytext()); }
"full" { return new Symbol(WCPSSymbol.FULL, yyline, yychar, yytext()); }
"none" { return new Symbol(WCPSSymbol.NONE, yyline, yychar, yytext()); }
"half" { return new Symbol(WCPSSymbol.HALF, yyline, yychar, yytext()); }
"other" { return new Symbol(WCPSSymbol.OTHER, yyline, yychar, yytext()); }
"coverage" { return new Symbol(WCPSSymbol.COVERAGE, yyline, yychar, yytext()); }
"values" { return new Symbol(WCPSSymbol.VALUES, yyline, yychar, yytext()); }
"x" { return new Symbol(WCPSSymbol.X, yyline, yychar, yytext()); }
"y" { return new Symbol(WCPSSymbol.Y, yyline, yychar, yytext()); }
"z" { return new Symbol(WCPSSymbol.Z, yyline, yychar, yytext()); }
"time" { return new Symbol(WCPSSymbol.TIME, yyline, yychar, yytext()); }
"abstract" { return new Symbol(WCPSSymbol.ABSTRACT, yyline, yychar, yytext()); }

{IDENTIFIER} { System.out.println("Got an Identifier: '" + yytext() + "'"); return new Symbol(WCPSSymbol.ID, yyline, yychar, yytext()); }
{NUMBER} { System.out.println("Got an int: '" + yytext() + "'"); return new Symbol(WCPSSymbol.NUMBER, yyline, yychar, new Integer(yytext()));}  
{FLOATNUMBER} { System.out.println("Got a float: '" + yytext() + "'"); return new Symbol(WCPSSymbol.FLOATNUMBER, yyline, yychar, new Float(yytext()));}
{STRING} { System.out.println("Got a string: '" + yytext() + "'"); return new Symbol(WCPSSymbol.STRING, yyline, yychar, yytext()); }




\n {System.out.println("newline" + yytext()); return null; }
. {System.out.println("illegal input: '" + yytext() +"'"); }
