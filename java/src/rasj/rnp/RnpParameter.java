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
/*************************************************************
 * <pre>
 *
 * PURPOSE: 
 *
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */
package rasj.rnp;

import java.io.*;

public abstract class RnpParameter
        { 
        int paramType;
        int dataType;
	int dataLength;
        int totalLength;
	
        public static final int headerSize  = 16;

	protected RnpParameter(int pT, int dT, int dL,int tL)
	     {
             paramType   = pT;
             dataType    = dT;
	     dataLength  = dL;
             totalLength = tL;
	     }
	   
	void print()
	     {
	     System.out.print("  Parameter type=" + paramType);
    	     System.out.print("  data type=" + dataType);
	     System.out.print("  dataLength="+dataLength);
	     System.out.print("  totalLength=" + totalLength);
	     printSpecific();
	     }
	   
	void write(DataOutputStream dataStream) throws IOException
	     {
	     dataStream.writeInt(paramType);
	     dataStream.writeInt(dataType);
	     dataStream.writeInt(dataLength);
	     dataStream.writeInt(totalLength);	    
	     writeSpecific(dataStream);
   	     }
	   
	int getTotalLength()
	     {
	     return totalLength;
	     }   
	int getDataLength()
	     {
	     return dataLength;
	     }     
	int getPaddLength()
	     {
	     return totalLength - dataLength - headerSize;
	     }
	   
	void computeTotalAlignedLength()
	    {
	      totalLength = (dataLength + headerSize + 3) & 0xFFFFFFFC;
	    }       
	      
	static RnpParameter constructFromStream(DataInputStream dataStream) throws IOException, RnpException
	     {
	     int paramType   = dataStream.readInt();
	     int dataType    = dataStream.readInt();
	     int dataLength  = dataStream.readInt();
	     int totalLength = dataStream.readInt();
	     
	     RnpParameter result = null;
	     switch(dataType)
	         {
	         case Rnp.dtt_Int32:    result = new ParameterInt32(paramType, dataType, dataLength, totalLength);
		 		        break;
	         case Rnp.dtt_Float32:  result = new ParameterFloat32(paramType, dataType, dataLength, totalLength);
		 		        break;
	         case Rnp.dtt_Double64: result = new ParameterDouble64(paramType, dataType, dataLength, totalLength);
		 		        break;
	         case Rnp.dtt_Asciiz:   result = new ParameterString(paramType, dataType, dataLength, totalLength);
		 		        break;
	         case Rnp.dtt_Opaque:   result = new ParameterOpaque(paramType, dataType, dataLength, totalLength);
		 		        break;
		 default: throw new RnpException("Incorrect parameter data type: " + dataType);
	         }
	     result.readSpecific(dataStream);
             //System.out.println("constructFromStream: "); result.print();	     	     
	     return result;	
	    }
	    
        int getParameterType() { return paramType; }
        int getDataType()      { return dataType;  }
	
	abstract void writeSpecific(DataOutputStream dataStream) throws IOException;
	abstract void readSpecific(DataInputStream dataStream) throws IOException;
	abstract void printSpecific();
       }
       
       
     
    class ParameterInt32 extends RnpParameter
        {
        int data;	
	public static final int length = 4;
        
	ParameterInt32(int parameterType, int nData)
	    {
	    super(parameterType,Rnp.dtt_Int32, length, length + headerSize);
	    data        = nData;
	    }
	   
	ParameterInt32(int pT, int dT, int dL, int tL)
	   { 
	   super(pT, dT, dL,tL);
	   }
	   
	void printSpecific()
	   {	
	   System.out.println("  value=" + data);
	   }
	
	void writeSpecific(DataOutputStream dataStream) throws IOException
	   {
	   dataStream.writeInt(data);
	   }
	
	void readSpecific(DataInputStream dataStream) throws IOException
	   {
	   data = dataStream.readInt();
	   }
	
	int getData()
	   { 
	   return data;
	   }	   
       }
       
       
    class ParameterFloat32 extends RnpParameter
        {
        float data;	
	public static final int length = 4;
        
	ParameterFloat32(int parameterType, float nData)
	    {
	    super(parameterType,Rnp.dtt_Float32, length, length + headerSize);
	    data        = nData;
	    }
	
	ParameterFloat32(int pT, int dT, int dL, int tL)
	    { 
	    super(pT, dT, dL,tL);
	    }
	
	void printSpecific()
	    {	
	    System.out.println("  value=" + data);
	    }
	   
	void writeSpecific(DataOutputStream dataStream) throws IOException
	    {
	    dataStream.writeFloat(data);
	    }
	
	void readSpecific(DataInputStream dataStream) throws IOException
	    {
	    data = dataStream.readFloat();
	    }
	
	float getData()
	    { 
	    return data;
	    }
       }
       
       
    class ParameterDouble64 extends RnpParameter
           {
           double data;	
	   public static final int length = 8;
        
	   ParameterDouble64(int parameterType, double nData)
	     {
	     super(parameterType,Rnp.dtt_Double64, length, length + headerSize);
	     data        = nData;
	     }
	  ParameterDouble64(int pT, int dT, int dL, int tL)
	    { 
	    super(pT, dT, dL, tL);
	    }
	
	void printSpecific()
	    {	
	    System.out.println("  value=" + data);
	    }
	   
	void writeSpecific(DataOutputStream dataStream) throws IOException
	    {
	    dataStream.writeDouble(data);
	    }
	
	void readSpecific(DataInputStream dataStream) throws IOException
	    {
	    data = dataStream.readDouble();
	    }
	
	double getData()
	    { 
	    return data;
	    }
       }
       
    class ParameterString extends RnpParameter
           {
           byte[] data;
	        
	   ParameterString(int parameterType, String nData)
	     {
	     super(parameterType,Rnp.dtt_Asciiz,0,0);	    
	     data        = nData.getBytes();
	     // do not forget: Strings are ASCIIZ in RNP!!
	     dataLength  = data.length+1;
	     computeTotalAlignedLength();
	     }
	
	ParameterString(int pT, int dT, int dL,int tL)
	    { 
	    super(pT, dT, dL,tL);
	    }
	
	void printSpecific()
	   {	
	   System.out.println("  value=" + getData());
	   }
	
	void writeSpecific(DataOutputStream dataStream) throws IOException
	   {
	   dataStream.write(data,0,dataLength - 1);
	   dataStream.writeByte(0);
	   
	   for(int i=0;i<getPaddLength();i++) dataStream.writeByte(0);
	   }
	
	void readSpecific(DataInputStream dataStream) throws IOException
	   {
	   data = new byte[dataLength - 1];
	   dataStream.readFully(data,0,dataLength - 1);
	   dataStream.readByte(); 
	   for(int i=0;i<getPaddLength();i++) dataStream.readByte();
	   }
	   
	String getData()
	   {
	   return new String(data);
	   }
       }
    
    class ParameterOpaque extends RnpParameter
          {
          byte[] data;
	        
	  ParameterOpaque(int parameterType, byte[] nData)
	    {
	    super(parameterType,Rnp.dtt_Opaque,nData.length,0); 	    
	    data        = nData;
	    computeTotalAlignedLength();
	    }
	   
	ParameterOpaque(int pT, int dT, int dL,int tL)
	    { 
	    super(pT, dT, dL,tL);
	    }

	void printSpecific()
	    {	
	    System.out.println("  value=(opaque data, size=" + data.length +")");
	    }
	
	void writeSpecific(DataOutputStream dataStream) throws IOException
	    {
	    dataStream.write(data,0,dataLength);
	    for(int i=0;i<getPaddLength();i++) dataStream.writeByte(0);
	    }
	
	void readSpecific(DataInputStream dataStream) throws IOException
	    {
	    data = new byte[dataLength];
	    dataStream.readFully(data,0,dataLength);
	    
	    for(int i=0;i<getPaddLength();i++) dataStream.readByte();
	    }
	
	byte[] getData()
	    {
	    return data;
	    }   
       }

