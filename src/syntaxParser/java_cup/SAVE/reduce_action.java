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

package java_cup;

/** This class represents a reduce action within the parse table. 
 *  The action simply stores the production that it reduces with and 
 *  responds to queries about its type.
 */
public class reduce_action extends parse_action {
 
  /*-----------------------------------------------------------*/
  /*--- Constructor(s) ----------------------------------------*/
  /*-----------------------------------------------------------*/

  /** Simple constructor. 
   * @param prod the production this action reduces with.
   */
  public reduce_action(production prod ) throws internal_error
    {
      /* sanity check */
      if (prod == null)
	throw new internal_error(
	  "Attempt to create a reduce_action with a null production");

      _reduce_with = prod;
    }

  /*-----------------------------------------------------------*/
  /*--- (Access to) Instance Variables ------------------------*/
  /*-----------------------------------------------------------*/
  
  /** The production we reduce with. */
  protected production _reduce_with;

  /** The production we reduce with. */
  public production reduce_with() {return _reduce_with;}

  /*-----------------------------------------------------------*/
  /*--- General Methods ---------------------------------------*/
  /*-----------------------------------------------------------*/

  /** Quick access to type of action. */
  public int kind() {return REDUCE;}

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Equality test. */
  public boolean equals(reduce_action other)
    {
      return other != null && other.reduce_with() == reduce_with();
    }

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Generic equality test. */
  public boolean equals(Object other)
    {
      if (other instanceof reduce_action)
	return equals((reduce_action)other);
      else
       return false;
    }

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Compute a hash code. */
  public int hashCode()
    {
      /* use the hash code of the production we are reducing with */
      return reduce_with().hashCode();
    }


  /** Convert to string. */
  public String toString() 
    {
      return "REDUCE(with prod " + reduce_with().index() + ")";
    }

  /*-----------------------------------------------------------*/

}
