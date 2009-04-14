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

// Driver for parser

package java_cup.simple_calc;

import java_cup.simple_calc.parser;
import java_cup.runtime.Symbol;

class Main {

  static boolean do_debug_parse = false;

  static public void main(String[] args) throws java.io.IOException {

      /* create a parsing object */
      parser parser_obj = new parser(new scanner());

      /* open input files, etc. here */
      Symbol parse_tree = null;

      try {
        if (do_debug_parse)
          parse_tree = parser_obj.debug_parse();
        else
          parse_tree = parser_obj.parse();
      } catch (Exception e) {
        /* do cleanup here -- possibly rethrow e */
      } finally {
	/* do close out here */
      }
  }
}

