package rasj;

import rasj.*;
import rasj.odmg.*;
import rasj.global.*;

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
/** ***********************************************************
 * <pre>
 *
 * PURPOSE:
 *  This class represents a MArray with base type Byte.
 *  @version $Revision: 1.7 $
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

public class RasMArrayByte extends RasGMArray
{

  /** default constructor */
  public RasMArrayByte()
    {
	super();
	typeLength = SIZE_OF_BYTE;
    }

    /**
     * constructor for uninitialized MDD objects
     *  * @param initDomain The initial Domain of the MArray
     */
  public RasMArrayByte(final RasMInterval initDomain)
    {
	super(initDomain, SIZE_OF_BYTE);
    }

    /**
    * Constructor for uninitialized MDD objects with Storage Layout
    * @param initDomain The initial Domain of the MArray
    * @param RasStorageLayout The storage layout to be used
    */
  public RasMArrayByte(final RasMInterval initDomain, RasStorageLayout stl)
    {
        super(initDomain, SIZE_OF_BYTE, stl);
    }

  /**
   * copy constructor
   * @param obj a copy of this object will be created
   */
  public RasMArrayByte(final RasMArrayByte obj)
    {
	super(obj);
    }

  /**
   * subscript operator for read access of a cell. The cell value is returned
   * as a byte[1] array.
   */
  public byte[] getCell(RasPoint point)
         throws RasDimensionMismatchException, RasIndexOutOfBoundsException
    {
	//first test dimensionality
	if(point.dimension() != domain.dimension())
  	  throw new RasDimensionMismatchException(point.dimension(), domain.dimension());
        byte[] retValue = new byte[1];
        retValue[0] = data[(int)domain.cellOffset(point)];
	return retValue;
    }

  /** subscript operator for read access of a cell. The cell value is returned as
   * a Byte.
   */
  public byte getByte(final RasPoint point)
         throws RasDimensionMismatchException, RasIndexOutOfBoundsException
    {
      // first test dimensionality
      if(point.dimension() != domain.dimension())
        throw new RasDimensionMismatchException(point.dimension(), domain.dimension());

      return data[(int)domain.cellOffset(point)];
    }


}

