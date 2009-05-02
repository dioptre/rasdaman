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

/** This class serves as the base class for entries in a parse action table.  
 *  Full entries will either be SHIFT(state_num), REDUCE(production), NONASSOC,
 *  or ERROR. Objects of this base class will default to ERROR, while
 *  the other three types will be represented by subclasses. 
 * 
 * @see     java_cup.reduce_action
 * @see     java_cup.shift_action
 */

public class parse_action {

  /*-----------------------------------------------------------*/
  /*--- Constructor(s) ----------------------------------------*/
  /*-----------------------------------------------------------*/

  /** Simple constructor. */
  public parse_action()
    {
      /* nothing to do in the base class */
    }

 
  /*-----------------------------------------------------------*/
  /*--- (Access to) Static (Class) Variables ------------------*/
  /*-----------------------------------------------------------*/

  /** Constant for action type -- error action. */
  public static final int ERROR = 0;

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Constant for action type -- shift action. */
  public static final int SHIFT = 1;

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Constants for action type -- reduce action. */
  public static final int REDUCE = 2;

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Constants for action type -- reduce action. */
  public static final int NONASSOC = 3;

  /*-----------------------------------------------------------*/
  /*--- General Methods ---------------------------------------*/
  /*-----------------------------------------------------------*/
	 
  /** Quick access to the type -- base class defaults to error. */
  public int kind() {return ERROR;}

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Equality test. */
  public boolean equals(parse_action other)
    {
      /* we match all error actions */
      return other != null && other.kind() == ERROR;
    }

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Generic equality test. */
  public boolean equals(Object other)
    {
      if (other instanceof parse_action)
	return equals((parse_action)other);
      else
	return false;
    }
  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Compute a hash code. */
  public int hashCode()
    {
      /* all objects of this class hash together */
      return 0xCafe123;
    }

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Convert to string. */
  public String toString() {return "ERROR";}

  /*-----------------------------------------------------------*/
}
    
