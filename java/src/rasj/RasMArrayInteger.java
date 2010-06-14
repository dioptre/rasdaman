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
 *  This class represents a MArray with base type Integer.
 *  @version $Revision: 1.11 $
 *  for ODMG type UShort(2 bytes, due to ODMG standard) you can use RasMArrayInteger,
 *  but only values up to 2^16 (performance loss because of convertion 4 bytes-> 2 bytes!);
 *  method intersectionWith(RasMInterval) uses byte array and is not optimized for special data arrays
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

public class RasMArrayInteger extends RasGMArray
{

  /** default constructor */
  public RasMArrayInteger()
    {
	super();
	typeLength = SIZE_OF_INTEGER;
    }

    /**
     * constructor for uninitialized MDD objects
     * @param initDomain The initial Domain of the MArray
     * */
  public RasMArrayInteger(final RasMInterval initDomain)
    {
	super(initDomain, SIZE_OF_INTEGER);
        objectData = new int[(int)(dataSize/SIZE_OF_INTEGER)];
        data = null;
    }

    /**
     * Constructor for uninitialized MDD objects with Storage Layout
     * @param initDomain The initial Domain of the MArray
     * @param RasStorageLayout The storage layout to be used
     */
    public RasMArrayInteger(final RasMInterval initDomain, RasStorageLayout stl)
    {
        super(initDomain, SIZE_OF_INTEGER, stl);
        objectData = new int[(int)(dataSize/SIZE_OF_INTEGER)];
        data = null;
    }


  /**
   *  copy constructor
   *  @param obj a copy of this object will be created
   */
  public RasMArrayInteger(final RasMArrayInteger obj)
    {
	super(obj);
        if(obj.objectData!=null)
        {
          objectData = new int[(int)(obj.dataSize/SIZE_OF_INTEGER)];
          System.arraycopy(obj.getIntArray(), 0, objectData, 0, (int)(obj.dataSize/SIZE_OF_INTEGER));
          data = null;
        }
    }

  /**
   * subscript operator for read access of a cell. The cell value is returned
   * as a byte[SIZE_OF_INTEGER] array. This kind of access to a cell is significantly slower
   * than getInt(), because each cell value has to be converted from int to a
   * byte[SIZE_OF_INTEGER] array.
   * The user has to take care that each Cell value is stored, before getting the next Cell.
   */
  public byte[] getCell(RasPoint point)
         throws RasDimensionMismatchException, RasIndexOutOfBoundsException
    {
	int cellValue;
	int tmp;

	//first test dimensionality
	if(point.dimension() != domain.dimension())
  	  throw new RasDimensionMismatchException(point.dimension(), domain.dimension());

        cellValue = ((int[])objectData)[(int)domain.cellOffset(point)];
        for(int i=0; i<SIZE_OF_INTEGER; i++)
        {
          tmp = cellValue;
          tmp >>>= (((SIZE_OF_INTEGER -1)-i)*8);
          currentCell[i] = (byte)tmp;
        }

	return currentCell;

    }

  /** subscript operator for read access of a cell. The cell value is returned as
   * an Integer. This access method is faster then getCell(), because no conversion
   * from int to Byte[SIZE_OF_INTEGER] has to be done.
   */
  public int getInt( final RasPoint point)
         throws RasDimensionMismatchException, RasIndexOutOfBoundsException
    {
      // first test dimensionality
      if(point.dimension() != domain.dimension())
        throw new RasDimensionMismatchException(point.dimension(), domain.dimension());

      return ((int[])objectData)[(int)domain.cellOffset(point)];
    }

  /**
   * get the internal representation of the array
   */
  public int[] getIntArray()
    {
      if(objectData==null)
      {
        ByteArrayInputStream bis = new ByteArrayInputStream(data);
        DataInputStream dis = new DataInputStream(bis);
        objectData = new int[data.length/SIZE_OF_INTEGER];
        try
        {
          for(int j=0; j<data.length/SIZE_OF_INTEGER; j++)
          {
            ((int[])objectData)[j] = dis.readInt();
          }
        }
        catch(IOException e)
        {
          throw new RasClientInternalException("RasMArrayInteger", "getIntArray()",
          "IOException while converting data to objectData array " + e.getMessage());
        }
      }
      return (int[])objectData;
    }

  /** get the internal representation of the array in bytes,
   *  please use getIntArray()
   */
  public byte[] getArray()
    {
      if(data==null)
      {
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(bos);
        try
        {
          for(int j=0; j<((int[])objectData).length; j++)
            dos.writeInt(((int[])objectData)[j]);
        }
        catch(IOException e)
        {
          throw new RasClientInternalException("RasMArrayInteger", "getArray()",
          "IOException while converting objectData to data array " + e.getMessage());
        }
        data = bos.toByteArray();
      }
      return data;
    }

  /** set the internal representation of the array */
  public void setArray(int[] newData)
    {
      objectData = newData;
      data = null;
      dataSize = newData.length * SIZE_OF_INTEGER;
    }
}

