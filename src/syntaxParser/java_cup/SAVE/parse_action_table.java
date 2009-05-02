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

/** This class represents the complete "action" table of the parser. 
 *  It has one row for each state in the parse machine, and a column for
 *  each terminal symbol.  Each entry in the table represents a shift,
 *  reduce, or an error.  
 *
 * @see     java_cup.parse_action
 * @see     java_cup.parse_action_row
 */
public class parse_action_table {

  /*-----------------------------------------------------------*/
  /*--- Constructor(s) ----------------------------------------*/
  /*-----------------------------------------------------------*/

  /** Simple constructor.  All terminals, non-terminals, and productions must 
   *  already have been entered, and the viable prefix recognizer should
   *  have been constructed before this is called.
   */
  public parse_action_table()
    {
      /* determine how many states we are working with */
      _num_states = lalr_state.number();

      /* allocate the array and fill it in with empty rows */
      under_state = new parse_action_row[_num_states];
      for (int i=0; i<_num_states; i++)
	under_state[i] = new parse_action_row();
    }

  /*-----------------------------------------------------------*/
  /*--- (Access to) Instance Variables ------------------------*/
  /*-----------------------------------------------------------*/

  /** How many rows/states are in the machine/table. */
  protected int _num_states;

  /** How many rows/states are in the machine/table. */
  public int num_states() {return _num_states;}

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Actual array of rows, one per state. */
  public  parse_action_row[] under_state;

  /*-----------------------------------------------------------*/
  /*--- General Methods ---------------------------------------*/
  /*-----------------------------------------------------------*/

  /** Check the table to ensure that all productions have been reduced. 
   *  Issue a warning message (to System.err) for each production that
   *  is never reduced.
   */
  public void check_reductions()
    throws internal_error
    {
      parse_action act;
      production   prod;

      /* tabulate reductions -- look at every table entry */
      for (int row = 0; row < num_states(); row++)
	{
	  for (int col = 0; col < under_state[row].size(); col++)
	    {
	      /* look at the action entry to see if its a reduce */
	      act = under_state[row].under_term[col];
	      if (act != null && act.kind() == parse_action.REDUCE)
		{
		  /* tell production that we used it */
		  ((reduce_action)act).reduce_with().note_reduction_use();
		}
	    }
	}

      /* now go across every production and make sure we hit it */
      for (Enumeration p = production.all(); p.hasMoreElements(); )
	{
	  prod = (production)p.nextElement();

	  /* if we didn't hit it give a warning */
	  if (prod.num_reductions() == 0)
	    {
	      /* count it *
	      emit.not_reduced++;

	      /* give a warning if they haven't been turned off */
	      if (!emit.nowarn)
		{
		  System.err.println("*** Production \"" + 
				  prod.to_simple_string() + "\" never reduced");
		  lexer.warning_count++;
		}
	    }
	}
    }

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*

  /** Convert to a string. */
  public String toString()
    {
      String result;
      int cnt;

      result = "-------- ACTION_TABLE --------\n";
      for (int row = 0; row < num_states(); row++)
	{
	  result += "From state #" + row + "\n";
	  cnt = 0;
	  for (int col = 0; col < under_state[row].size(); col++)
	    {
	      /* if the action is not an error print it */ 
	      if (under_state[row].under_term[col].kind() != parse_action.ERROR)
		{
		  result += " [term " + col + ":" + under_state[row].under_term[col] + "]";

		  /* end the line after the 2nd one */
		  cnt++;
		  if (cnt == 2)
		    {
		      result += "\n";
		      cnt = 0;
		    }
		}
	    }
          /* finish the line if we haven't just done that */
	  if (cnt != 0) result += "\n";
	}
      result += "------------------------------";

      return result;
    }

  /*-----------------------------------------------------------*/

}

