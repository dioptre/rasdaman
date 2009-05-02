/*
 * This file is part of Petascope.
 *
 * Petascope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Petascope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Petascope. If not, see <http://www.gnu.org/licenses/>.
 *
 * For more information please see <http://www.Petascope.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 *
 * Copyright 2009 Jacobs University Bremen, Peter Baumann.
 */

package java_cup;

import java.util.Enumeration;

/** This class represents the complete "reduce-goto" table of the parser.
 *  It has one row for each state in the parse machines, and a column for
 *  each terminal symbol.  Each entry contains a state number to shift to
 *  as the last step of a reduce. 
 *
 * @see     java_cup.parse_reduce_row
 */
public class parse_reduce_table {
 
  /*-----------------------------------------------------------*/
  /*--- Constructor(s) ----------------------------------------*/
  /*-----------------------------------------------------------*/

  /** Simple constructor.  Note: all terminals, non-terminals, and productions 
   *  must already have been entered, and the viable prefix recognizer should
   *  have been constructed before this is called.
   */
  public parse_reduce_table()
    {
      /* determine how many states we are working with */
      _num_states = lalr_state.number();

      /* allocate the array and fill it in with empty rows */
      under_state = new parse_reduce_row[_num_states];
      for (int i=0; i<_num_states; i++)
	under_state[i] = new parse_reduce_row();
    }

   
  /*-----------------------------------------------------------*/
  /*--- (Access to) Instance Variables ------------------------*/
  /*-----------------------------------------------------------*/

  /** How many rows/states in the machine/table. */
  protected int _num_states;

  /** How many rows/states in the machine/table. */
  public int num_states() {return _num_states;}

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Actual array of rows, one per state */
  public  parse_reduce_row[] under_state;
 
  /*-----------------------------------------------------------*/
  /*--- General Methods ---------------------------------------*/
  /*-----------------------------------------------------------*/

  /** Convert to a string. */
  public String toString()
    {
      String result;
      lalr_state goto_st;
      int cnt;

      result = "-------- REDUCE_TABLE --------\n";
      for (int row = 0; row < num_states(); row++)
	{
	  result += "From state #" + row + "\n";
	  cnt = 0;
	  for (int col = 0; col < under_state[row].size(); col++)
	    {
	      /* pull out the table entry */
	      goto_st = under_state[row].under_non_term[col];

	      /* if it has action in it, print it */
	      if (goto_st != null)
		{
		  result += " [non term " + col + "->"; 
		  result += "state " + goto_st.index() + "]";

		  /* end the line after the 3rd one */
		  cnt++;
		  if (cnt == 3)
		    {
		      result += "\n";
		      cnt = 0;
		    }
		}
	    }
          /* finish the line if we haven't just done that */
	  if (cnt != 0) result += "\n";
	}
      result += "-----------------------------";

      return result;
    }

  /*-----------------------------------------------------------*/

}

