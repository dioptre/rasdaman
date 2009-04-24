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
 *  This class represents a MArray with base type float.
 *  @version $Revision: 1.5 $
 *  method intersectionWith(RasMInterval) uses byte array and is not optimized for special data arrays
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

public class RasMArrayFloat extends RasGMArray
{

  /** default constructor */
  public RasMArrayFloat()
    {
	super();
	typeLength = SIZE_OF_FLOAT;
    }

    /**
     * constructor for uninitialized MDD objects
     * @param initDomain The initial Domain of the MArray
     * */
  public RasMArrayFloat(final RasMInterval initDomain)
    {
	super(initDomain, SIZE_OF_FLOAT);
        objectData = new float[(int)(dataSize/SIZE_OF_FLOAT)];
        data = null;
    }

    /**
     * Constructor for uninitialized MDD objects with Storage Layout
     * @param initDomain The initial Domain of the MArray
     * @param RasStorageLayout The storage layout to be used
     */
    public RasMArrayFloat(final RasMInterval initDomain, RasStorageLayout stl)
    {
        super(initDomain, SIZE_OF_FLOAT, stl);
        objectData = new float[(int)(dataSize/SIZE_OF_FLOAT)];
        data = null;
    }

  /**
   * copy constructor
   * @param obj a copy of this object will be created
   * */
  public RasMArrayFloat(final RasMArrayFloat obj)
    {
	super(obj);
        if(obj.objectData!=null)
        {
          objectData = new float[(int)(obj.dataSize/SIZE_OF_FLOAT)];
          System.arraycopy(obj.getFloatArray(), 0, objectData, 0, (int)(obj.dataSize/SIZE_OF_FLOAT));
          data = null;
        }
    }

  /**
   * subscript operator for read access of a cell. The cell value is returned
   * as a byte[SIZE_OF_FLOAT] array. This kind of access to a cell is significantly slower
   * than getFloat(), because each cell value has to be converted from float to a
   * byte[SIZE_OF_FLOAT] array.
   * The user has to take care that each Cell value is stored, before getting the next Cell.
   */
  public byte[] getCell(RasPoint point)
         throws RasDimensionMismatchException, RasIndexOutOfBoundsException
    {
	float cellValue;
	int tmp;

	//first test dimensionality
	if(point.dimension() != domain.dimension())
  	  throw new RasDimensionMismatchException(point.dimension(), domain.dimension());

	cellValue = ((float[])objectData)[(int)domain.cellOffset(point)];
        for(int i=0; i<SIZE_OF_FLOAT; i++)
	{
	    tmp = Float.floatToRawIntBits(cellValue);
	    tmp >>>= (((SIZE_OF_FLOAT -1)-i)*8);
	    currentCell[i] = (byte)tmp;
	}

	return currentCell;

    }

  /** subscript operator for read access of a cell. The cell value is returned as
   * an float. This access method is faster then getCell(), because no conversion
   * from float to Byte[SIZE_OF_FLOAT] has to be done.
   */
  public float getFloat( final RasPoint point)
         throws RasDimensionMismatchException, RasIndexOutOfBoundsException
    {
      // first test dimensionality
      if(point.dimension() != domain.dimension())
        throw new RasDimensionMismatchException(point.dimension(), domain.dimension());

      return ((float[])objectData)[(int)domain.cellOffset(point)];
    }

  /**
   *  get the internal representation of the array
   */
  public float[] getFloatArray()
    {
      if(objectData==null)
      {
        ByteArrayInputStream bis = new ByteArrayInputStream(data);
        DataInputStream dis = new DataInputStream(bis);
        objectData = new float[data.length/SIZE_OF_FLOAT];
        try
        {
          for(int j=0; j<data.length/SIZE_OF_FLOAT; j++)
          {
            ((float[])objectData)[j] = dis.readFloat();
          }
        }
        catch(IOException e)
        {
          throw new RasClientInternalException("RasMArrayFloat", "getFloatArray()",
          "IOException while converting data to objectData array " + e.getMessage());
        }
      }
      return (float[])objectData;
    }

  /** get the internal representation of the array in bytes,
   *  please use getFloatArray()
   */
  public byte[] getArray()
    {
      if(data==null)
      {
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(bos);
        try
        {
          for(int j=0; j<((float[])objectData).length; j++)
            dos.writeFloat(((float[])objectData)[j]);
        }
        catch(IOException e)
        {
          throw new RasClientInternalException("RasMArrayFloat", "getArray()",
          "IOException while converting objectData to data array " + e.getMessage());
        }
        data = bos.toByteArray();
      }
      return data;
    }

  /** set the internal representation of the array */
  public void setArray(float[] newData)
    {
      objectData = newData;
      data = null;
      dataSize = newData.length * SIZE_OF_FLOAT;
    }

}

