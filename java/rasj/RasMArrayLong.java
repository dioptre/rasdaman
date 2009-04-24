package rasj;

import rasj.*;
import rasj.odmg.*;
import rasj.global.*;
import java.io.*;

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
 *  This class represents a MArray with base type long.
 *  @version $Revision: 1.7 $
 *  for ODMG type Long(4 bytes, due to ODMG standard) please use RasMArrayInteger,
 *  for ODMG type ULong(4 bytes, due to ODMG standard) you can use RasMArrayLong,
 *  but only values up to 2^32 (performance loss because of convertion 8 bytes-> 4 bytes!);
 *  method intersectionWith(RasMInterval) uses byte array and is not optimized for special data arrays
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

public class RasMArrayLong extends RasGMArray
{

  /** default constructor */
  public RasMArrayLong()
    {
	super();
	typeLength = SIZE_OF_LONG;
    }

    /**
     * constructor for uninitialized MDD objects
     *  @param initDomain The initial Domain of the MArray
     */
  public RasMArrayLong(final RasMInterval initDomain)
    {
	super(initDomain, SIZE_OF_LONG);
        objectData = new long[(int)(dataSize/SIZE_OF_LONG)];
        data = null;
    }

  /**
    * Constructor for uninitialized MDD objects with Storage Layout
    * @param initDomain The initial Domain of the MArray
    * @param RasStorageLayout The storage layout to be used
    */
  public RasMArrayLong(final RasMInterval initDomain, RasStorageLayout stl)
    {
        super(initDomain, SIZE_OF_LONG, stl);
        objectData = new long[(int)(dataSize/SIZE_OF_LONG)];
        data = null;
    }


  /**
   * copy constructor
   * @param obj a copy of this object will be created
   */
  public RasMArrayLong(final RasMArrayLong obj)
    {
	super(obj);
        if(obj.objectData!=null)
        {
          objectData = new long[(int)(obj.dataSize/SIZE_OF_LONG)];
          System.arraycopy(obj.getLongArray(), 0, objectData, 0, (int)(obj.dataSize/SIZE_OF_LONG));
          data = null;
        }
    }

  /**
   * subscript operator for read access of a cell. The cell value is returned
   * as a byte[SIZE_OF_LONG] array. This kind of access to a cell is significantly slower
   * than getLong(), because each cell value has to be converted from long to a
   * byte[SIZE_OF_LONG] array.
   * The user has to take care that each Cell value is stored, before getting the next Cell.
   */
  public byte[] getCell(RasPoint point)
         throws RasDimensionMismatchException, RasIndexOutOfBoundsException
    {
	long cellValue;
	long tmp;

	//first test dimensionality
	if(point.dimension() != domain.dimension())
  	  throw new RasDimensionMismatchException(point.dimension(), domain.dimension());

	cellValue = ((long[])objectData)[(int)domain.cellOffset(point)];
        for(int i=0; i<SIZE_OF_LONG; i++)
	{
	    tmp = cellValue;
	    tmp >>>= (((SIZE_OF_LONG -1)-i)*SIZE_OF_LONG);
	    currentCell[i] = (byte)tmp;
	}

	return currentCell;

    }

  /** subscript operator for read access of a cell. The cell value is returned as
   * an long. This access method is faster then getCell(), because no conversion
   * from long to Byte[SIZE_OF_LONG] has to be done.
   */
  public long getLong( final RasPoint point)
         throws RasDimensionMismatchException, RasIndexOutOfBoundsException
    {
      // first test dimensionality
      if(point.dimension() != domain.dimension())
        throw new RasDimensionMismatchException(point.dimension(), domain.dimension());

      return ((long[])objectData)[(int)domain.cellOffset(point)];
    }

  /**
   * get the internal representation of the array
   */
  public long[] getLongArray()
    {
      if(objectData==null)
      {
        ByteArrayInputStream bis = new ByteArrayInputStream(data);
        DataInputStream dis = new DataInputStream(bis);
        objectData = new long[data.length/SIZE_OF_LONG];
        try
        {
          for(int j=0; j<data.length/SIZE_OF_LONG; j++)
          {
            ((long[])objectData)[j] = dis.readLong();
          }
        }
        catch(IOException e)
        {
          throw new RasClientInternalException("RasMArrayLong", "getLongArray()",
          "IOException while converting data to objectData array " + e.getMessage());
        }
      }
      return (long[])objectData;
    }

  /** get the internal representation of the array in bytes,
   *  please use getLongArray()
   */
  public byte[] getArray()
    {
      if(data==null)
      {
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(bos);
        try
        {
          for(int j=0; j<((long[])objectData).length; j++)
            dos.writeLong(((long[])objectData)[j]);
        }
        catch(IOException e)
        {
          throw new RasClientInternalException("RasMArrayLong", "getArray()",
          "IOException while converting objectData to data array " + e.getMessage());
        }
        data = bos.toByteArray();
      }
      return data;
    }

  /** set the internal representation of the array */
  public void setArray(long[] newData)
    {
      objectData = newData;
      data = null;
      dataSize = newData.length * SIZE_OF_LONG;
    }
}

