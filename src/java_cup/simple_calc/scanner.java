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

// Simple Example Scanner Class

package java_cup.simple_calc;

import java_cup.runtime.Symbol;

public class scanner implements java_cup.runtime.Scanner {
  final java.io.InputStream instream;

  public scanner(java.io.InputStream is) throws java.io.IOException {
    instream = is;
  }
  public scanner() throws java.io.IOException { this(System.in); }

  /* single lookahead character */
  protected int next_char = -2;

  /* advance input by one character */
  protected void advance()
    throws java.io.IOException
    { next_char = instream.read(); }

  /* initialize the scanner */
  private void init()
    throws java.io.IOException
    { advance(); }

  /* recognize and return the next complete token */
  public Symbol next_token()
    throws java.io.IOException
    {
      if (next_char==-2) init(); // set stuff up first time we are called.
      for (;;)
        switch (next_char)
	  {
	    case '0': case '1': case '2': case '3': case '4': 
	    case '5': case '6': case '7': case '8': case '9': 
	      /* parse a decimal integer */
	      int i_val = 0;
	      do {
	        i_val = i_val * 10 + (next_char - '0');
	        advance();
	      } while (next_char >= '0' && next_char <= '9');
	    return new Symbol(sym.NUMBER, new Integer(i_val));

	    case ';': advance(); return new Symbol(sym.SEMI);
	    case '+': advance(); return new Symbol(sym.PLUS);
	    case '-': advance(); return new Symbol(sym.MINUS);
	    case '*': advance(); return new Symbol(sym.TIMES);
	    case '/': advance(); return new Symbol(sym.DIVIDE);
	    case '%': advance(); return new Symbol(sym.MOD);
	    case '(': advance(); return new Symbol(sym.LPAREN);
	    case ')': advance(); return new Symbol(sym.RPAREN);

	    case -1: return new Symbol(sym.EOF);

	    default: 
	      /* in this simple scanner we just ignore everything else */
	      advance();
	    break;
	  }
    }
};
