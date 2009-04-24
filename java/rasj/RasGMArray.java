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
 * This class represents a generic MDD in the sense that it
 * is independent of the cell base type. The only information
 * available is the length in bytes of the base type.
 * <P>More specific MDDs including base type information for more
 * type safety are represented by a set of typed subclasses.
 * @version $Revision: 1.26 $
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

public class RasGMArray extends RasObject implements RasGlobalDefs
{
    static final String rcsid = "@(#)Package rasj, class RasGMArray: $Header: /home/rasdev/CVS-repository/rasdaman/java/rasj/RasGMArray.java,v 1.26 2003/12/10 21:04:23 rasdev Exp $";

    /** spatial domain */
    protected RasMInterval domain;

    /** internal array representation in bytes*/
    protected byte[] data;

    /** internal array representation as Object for use in special marrays*/
    protected Object objectData;

    /** internal object for accessing one cell of the array */
    protected byte[] currentCell;

    /** size of internal array representation in bytes */
    protected long dataSize;

    /** length of the cell base type in bytes */
    protected long typeLength;

    /** store current data format */
    protected int currentFormat; //RasDataFormat -> RasGlobalDefs

    /** storage layout object */
    protected RasStorageLayout storageLayout;

    /**
     * Default constructor.
     **/
    public RasGMArray()
    {
	super(RAS_MARRAY);
	data = null;
        objectData = null;
	currentCell = null;
	dataSize = 0;
        domain = null;
	typeLength = 0;
	currentFormat = RAS_ARRAY;
	storageLayout = new RasStorageLayout();
    }

    /**
     * Constructor for uninitialized MDD objects
     * @param initDomain The initial Domain of the GMArray
     * @param cellTypeLength The length of the cell type used
     */
    public RasGMArray(final RasMInterval initDomain, long cellTypeLength)
    {
	super(RAS_MARRAY);
	data = null;
        objectData = null;
	dataSize = 0;
	domain = initDomain;
	typeLength = cellTypeLength;
	currentFormat = RAS_ARRAY;
	storageLayout = new RasStorageLayout();

	// If dimensionality is zero, just one scalar value is stored.
	dataSize = ((domain.dimension() != 0) ? domain.cellCount() : 1) * typeLength;
	data = new byte[(int)dataSize];
	currentCell = new byte[(int)cellTypeLength];
    }

    /**
     * Constructor for uninitialized MDD objects with Storage Layout
     * @param initDomain The initial Domain of the GMArray
     * @param cellTypeLength The length of the cell type used
     * @param RasStorageLayout The storage layout to be used
     */
    public RasGMArray(final RasMInterval initDomain,  long cellTypeLength, RasStorageLayout stl)
    {
	super(RAS_MARRAY);
	data = null;
        objectData = null;
	dataSize = 0;
	domain = initDomain;
	typeLength = cellTypeLength;
	currentFormat = RAS_ARRAY;
	storageLayout = stl;

	// If dimensionality is zero, just one scalar value is stored.
	dataSize = ((domain.dimension() != 0) ? domain.cellCount() : 1) * typeLength;
	data = new byte[(int)dataSize];
	currentCell = new byte[(int)cellTypeLength];
    }


    /**
     * Copy constructor.
     * @param obj a copy of this object will be created
     **/
    public RasGMArray(final RasGMArray obj)
    {
	super(obj, RAS_MARRAY);
        if(data!=null)
        {
          System.arraycopy(obj.getArray(), 0, data, 0, (int)obj.dataSize);
          objectData = null;
        }
	dataSize = obj.getArraySize();
	domain = obj.spatialDomain();
	typeLength = obj.typeLength;
	currentFormat = obj.currentFormat;
	storageLayout = new RasStorageLayout(obj.storageLayout);
	if(obj.typeLength != 0)
          currentCell = new byte[(int)obj.typeLength];
    }

    /**
     * This method copies the values of mArray to itself.
     * @param mArray the values of this MArray will be copied
     * @return itself (after having copied the values from mArray)
     **/
    public RasGMArray setTo(final RasGMArray mArray)
    {
	if(this != mArray)
	    {
		if(data != null)
		    {
			data = null;
		    }

		if (mArray.data != null)
		    {
			dataSize = mArray.dataSize;
			data = mArray.data;
		    }
                if (mArray.objectData != null)
		    {
			dataSize = mArray.dataSize;
			objectData = mArray.objectData;
		    }

		if(storageLayout != null)
		    {
			storageLayout = null;
		    }


		// this has to be changed to a clone() function in future
		if(mArray.storageLayout != null)
		    storageLayout = new RasStorageLayout(mArray.storageLayout);

		domain        = mArray.domain;
		typeLength    = mArray.typeLength;
		currentFormat = mArray.currentFormat;
	    }
	return this;
    }

    /**
     * Reads a cell of the MDD. The cell's value is returned
     * as a byte array, the length of which depends on the size of the base type.
     * If you want to get the cell value as a primitive type (like integer or byte),
     * you either have to do a cast or use the type-specific MArrays
     * that are derived from this class.
     * @param point the coordinates of the requested cell
     * @return a byte array representing the value of the requested cell.
     **/
    public byte[] getCell(RasPoint point) throws RasDimensionMismatchException, RasIndexOutOfBoundsException
    {
	// first test dimensionality
	if(point.dimension() != domain.dimension())
  	  throw new RasDimensionMismatchException(point.dimension(), domain.dimension());
	if(typeLength == 0)
	  return null;

	System.arraycopy(data, (int)(domain.cellOffset(point)*typeLength), currentCell, 0, (int)typeLength);
	return currentCell;

    }

    /**
     * Returns the storage layout object of this MDD.
     * @return the storage layout object
     **/
    public final RasStorageLayout getStorageLayout()
    {
	return storageLayout;
    }

    /**
     * Sets the storage layout object and checks compatibility with the domain.
     * @param stl the new storage layout
     */
    public void setStorageLayout(RasStorageLayout stl) throws RasDimensionMismatchException
    {
	if (!stl.isCompatible(domain))
	    throw new RasDimensionMismatchException(domain.dimension(),stl.getSpatialDomain().dimension());
        else
	    storageLayout = stl;
    }

    /**
     * Returns a RasGMArray that is the intersection of the current domain
     * with the specified interval.
     * @param where the interval that is used for the intersection
     * @return the result of the intersection of this GMArray with the parameter interval
     */
    public RasGMArray intersectionWith(RasMInterval where)
    {
	RasGMArray tile = new RasGMArray();

	try {
	    RasMInterval objDomain = spatialDomain();
	    int numDims = objDomain.dimension();
	    long tlength = getTypeLength();

	    byte[] objData = new byte[(int)(where.cellCount() * tlength)];
	    tile.setSpatialDomain(where);
	    tile.setTypeLength(tlength);
	    tile.setArray(objData);
	    tile.setArraySize(where.cellCount() * tlength);

	    long blockLength =
		where.item(numDims-1).high() - where.item(numDims-1).low() + 1;
	    long total = where.cellCount()/blockLength;

	    byte[] dest = objData;
	    byte[] source = getArray();
	    int blength = (int)(blockLength * tlength);

	    for (long cell=0; cell<total; cell++)
		{
		    RasPoint p = where.cellPoint(cell*blockLength);

		    System.arraycopy(source, (int)(objDomain.cellOffset(p) * tlength), dest,
				     (int)(where.cellOffset(p) * tlength), blength);
		}

	    return tile;
	}

	catch(RasIndexOutOfBoundsException e1) {
	    // this canno toccur (theoretically)
	    throw new RasClientInternalException("RasGMArray","intersectionWith()",e1.getMessage());
	}
	catch(RasResultIsNoCellException e2) {
	    // this canno toccur (theoretically)
            throw new RasClientInternalException("RasGMArray","intersectionWith()",e2.getMessage());
	}

    }


    // Read methods
    /**
     * Gets the spatial domain.
     * @return the spatial domain of this GMArray
     **/
    public final RasMInterval spatialDomain()
    {
	return domain;
    }

    /**
     * Gets the internal representation of this GMAarray (the byte array).
     * @return the byte array representing this GMArray
     **/
    public byte[] getArray()
    {
	return data;
    }

    /**
     * Gets the size of the internal representation of this GMArray (in bytes).
     * @return the size of this GMArray
     **/
    public final long getArraySize()
    {
	return dataSize;
    }

    /**
     * Gets the length of cell type (in bytes).
     * @return the cell type length
     **/
    public final long getTypeLength()
    {
	return typeLength;
    }

    /**
     * Gets the current data format (RAS_ARRAY).
     * @return the current data format
     **/
    public final int getCurrentFormat()
    {
	return currentFormat;
    }


    // Write methods
    /**
     * Sets the spatial domain.
     * @param dom the new spatial domain of this GMArray
     **/
    public void setSpatialDomain(final RasMInterval dom)
    {
	domain = dom;
    }

    /**
     * Sets the internal representation of the GMArray.
     * @param newData the new byte array representing the contents of this GMArray
     **/
    public void setArray(byte[] newData)
    {
        data = newData;
        dataSize = newData.length;
        objectData = null;
    }

    /**
     * Sets the size of the internal representation (in bytes).
     * @param newValue the size of the internal representation (the byte array)
     **/
    public void setArraySize(long newValue)
    {
	dataSize = newValue;
    }

    /**
     * Sets the length of the cell type (in bytes).
     * @param newValue the cell type length
     **/
    public void setTypeLength(long newValue)
    {
	typeLength = newValue;
	/* Now create the byte array object used for returning cell values */
	currentCell = new byte[(int)newValue];
    }

    /**
     * Sets the current data format.
     * @param newFormat the new data format
     **/
    public void setCurrentFormat(int newFormat)
    {
	currentFormat = newFormat;
    }

    // Methods for database communication (internal use only)

    /* inserts an object into the database
    public void insertObjIntoDB()
    {
	// Nothing is done in that case. RasMArray objects can just be inserted as elements
	// of a collection which invokes RasGMArray.insertObjIntoDB(String collName)
	// of the RasMArray objects.

	//RasDatabase.currentDatabase.communication.insertSingleMDDObj(this);
	System.out.println(" do nothing ");
    }
    */

    /* inserts myself into a specific collection in the database
    public void insertObjIntoDB(String collName) throws RasException
    {
	// Insert myself in database only if I have a type name, otherwise
	// an exception is thrown.
	if(!typeName)
	  {
	  throw new RasErrorDatabaseClassUndefined();
	  }
	//RasDatabase.currentDatabase.communication.insertMDD(collName, this);
    }
    */

    /** gets the String representation */
    public String toString()
    {
	String tileDomain = "";

	RasType typeSchema = (RasType)(((RasGMArray)this).getTypeSchema());
	RasBaseType baseTypeSchema = (RasBaseType)(((RasGMArray)this).getBaseTypeSchema());

	if(storageLayout.getSpatialDomain() == null)
	    tileDomain = getTilingDomain(domain,typeLength);
        else
	    tileDomain = storageLayout.getSpatialDomain().toString();

	String s = "\n" + getClass().getName();
	s = s + "\n"
	    + "  Oid...................: " + getOID() + "\n"
            + "  Object Name...........: " + getObjectName() + "\n"
            + "  Object Type...........: " + getObjectType() + "\n"
  	    + "  Object Type Name......: " + getObjectTypeName() + "\n"
	    + "  Type Structure........: " + ((getTypeStructure() != null) ? getTypeStructure() : "<nn>") + "\n"
	    + "  Type Schema...........: " + ((typeSchema != null) ? typeSchema.toString() : "<nn>") + "\n"
	    + "  Domain................: " + domain + "\n"
            + "  TilingDomain..........: " + tileDomain + "\n"
            + "  TileSize..............: " + storageLayout.getTileSize() + " bytes\n"
	    + "  Base Type Schema......: " + ((baseTypeSchema != null) ? baseTypeSchema.toString() : "<nn>") + "\n"
	    + "  Base Type Length......: " + typeLength + "\n"
	    + "  Data format...........: " + currentFormat + "\n"
	    + "  Data size (bytes).....: " + dataSize + "\n";

      return s;
    }

     /** gets the String for testing */
    public String toTestString()
    {
	String tileDomain = "";

	RasType typeSchema = (RasType)(((RasGMArray)this).getTypeSchema());
	RasBaseType baseTypeSchema = (RasBaseType)(((RasGMArray)this).getBaseTypeSchema());

	if(storageLayout.getSpatialDomain() == null)
	    tileDomain = getTilingDomain(domain,typeLength);
        else
	    tileDomain = storageLayout.getSpatialDomain().toString();

	StringBuffer buffer = new StringBuffer(getArray().length * 4); // for most elements 3 characters and a space
	buffer.append("\n" + getClass().getName());
	buffer.append("\n"
	    //+ "  Oid...................: " + getOID() + "\n"
            + "  Object Name...........: " + getObjectName() + "\n"
            + "  Object Type...........: " + getObjectType() + "\n"
  	    + "  Object Type Name......: " + getObjectTypeName() + "\n"
	    + "  Type Structure........: " + ((getTypeStructure() != null) ? getTypeStructure() : "<nn>") + "\n"
	    + "  Type Schema...........: " + ((typeSchema != null) ? typeSchema.toString() : "<nn>") + "\n"
	    + "  Domain................: " + domain + "\n"
            + "  TilingDomain..........: " + tileDomain + "\n"
            + "  TileSize..............: " + storageLayout.getTileSize() + " bytes\n"
	    + "  Base Type Schema......: " + ((baseTypeSchema != null) ? baseTypeSchema.toString() : "<nn>") + "\n"
	    + "  Base Type Length......: " + typeLength + "\n"
	    + "  Data format...........: " + currentFormat + "\n"
	    + "  Data size (bytes).....: " + dataSize + "\n"
	    );

            //print bytes, just for testing!!!
            for(int j=0; j < getArray().length; j++)
            {
              buffer.append(" " + getArray()[j]);
            }

      return buffer.toString();
    }

    /**
     * Returns the base type schema of this GMArray.
     * @return the base type schema
     **/
    public RasBaseType getBaseTypeSchema()
    {
	RasType type = getTypeSchema();
	RasBaseType baseType = null;

	if (type != null)
	    {
		if (type.getTypeID() == RasGlobalDefs.RAS_MARRAY)
		    {
			RasMArrayType mArrayType = (RasMArrayType)type;
			baseType = (mArrayType.getBaseType());
		    }
	    }

	return baseType;
    }


    //######### for internal use

    // calculates the tiling domain based on the original MDD, the type length and the tileSize
    // of the MDD's storageLayout.
    private String getTilingDomain(final RasMInterval originalDomain, final long typeLength)
    {
	long tileSize = storageLayout.getTileSize();
	int dim = originalDomain.dimension();
	double tmp = 1.0/dim;
	int size=(int)(Math.pow((double)(tileSize / typeLength),tmp))-1;
	String retVal = "0:"+String.valueOf(size);
	for(int x=1; x<dim; x++)
	    retVal = retVal + ",0:" + size;
	return "["+retVal+"]";
    }

}


