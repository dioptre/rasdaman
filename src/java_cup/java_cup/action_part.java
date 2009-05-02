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

/** 
 * This class represents a part of a production which contains an
 * action.  These are eventually eliminated from productions and converted
 * to trailing actions by factoring out with a production that derives the
 * empty string (and ends with this action).
 *
 * @see java_cup.production
 */

public class action_part extends production_part {

  /*-----------------------------------------------------------*/
  /*--- Constructors ------------------------------------------*/
  /*-----------------------------------------------------------*/

  /** Simple constructor. 
   * @param code_str string containing the actual user code.
   */
  public action_part(String code_str)
    {
      super(/* never have a label on code */null);
      _code_string = code_str;
    }

  /*-----------------------------------------------------------*/
  /*--- (Access to) Instance Variables ------------------------*/
  /*-----------------------------------------------------------*/

  /** String containing code for the action in question. */
  protected String _code_string;

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** String containing code for the action in question. */
  public String code_string() {return _code_string;}

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Set the code string. */
  public void set_code_string(String new_str) {_code_string = new_str;}

  /*-----------------------------------------------------------*/
  /*--- General Methods ---------------------------------------*/
  /*-----------------------------------------------------------*/

  /** Override to report this object as an action. */
  public boolean is_action() { return true; }

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Equality comparison for properly typed object. */
  public boolean equals(action_part other)
    {
      /* compare the strings */
      return other != null && super.equals(other) && 
	     other.code_string().equals(code_string());
    }

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Generic equality comparison. */
  public boolean equals(Object other)
    {
      if (!(other instanceof action_part)) 
	return false;
      else
	return equals((action_part)other);
    }

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Produce a hash code. */
  public int hashCode()
    {
      return super.hashCode() ^ 
	     (code_string()==null ? 0 : code_string().hashCode());
    }

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Convert to a string.  */
  public String toString()
    {
      return super.toString() + "{" + code_string() + "}";
    }

  /*-----------------------------------------------------------*/
}
