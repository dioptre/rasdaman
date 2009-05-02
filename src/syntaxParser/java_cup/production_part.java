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

/** This class represents one part (either a symbol or an action) of a 
 *  production.  In this base class it contains only an optional label 
 *  string that the user can use to refer to the part within actions.<p>
 *
 *  This is an abstract class.
 *
 * @see     java_cup.production
 */
public abstract class production_part {

  /*-----------------------------------------------------------*/
  /*--- Constructor(s) ----------------------------------------*/
  /*-----------------------------------------------------------*/
       
  /** Simple constructor. */
  public production_part(String lab)
    {
      _label = lab;
    }

  /*-----------------------------------------------------------*/
  /*--- (Access to) Instance Variables ------------------------*/
  /*-----------------------------------------------------------*/
       
  /** Optional label for referring to the part within an action (null for 
   *  no label). 
   */
  protected String _label;

  /** Optional label for referring to the part within an action (null for 
   *  no label). 
   */
  public String label() {return _label;}

  /*-----------------------------------------------------------*/
  /*--- General Methods ---------------------------------------*/
  /*-----------------------------------------------------------*/
       
  /** Indicate if this is an action (rather than a symbol).  Here in the 
   * base class, we don't this know yet, so its an abstract method.
   */
  public abstract boolean is_action();

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Equality comparison. */
  public boolean equals(production_part other)
    {
      if (other == null) return false;

      /* compare the labels */
      if (label() != null)
	return label().equals(other.label());
      else
	return other.label() == null;
    }

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Generic equality comparison. */
  public boolean equals(Object other)
    {
      if (!(other instanceof production_part))
        return false;
      else
	return equals((production_part)other);
    }

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Produce a hash code. */
  public int hashCode()
    {
      return label()==null ? 0 : label().hashCode();
    }

  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

  /** Convert to a string. */
  public String toString()
    {
      if (label() != null)
	return label() + ":";
      else
	return " ";
    }

  /*-----------------------------------------------------------*/

}
