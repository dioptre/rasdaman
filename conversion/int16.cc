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
/
/**
 * SOURCE: int16.hh
 *
 * MODULE:  conversion
 *
 * CLASSES: r_Conv_int16
 *
 * COMMENTS:
 * Provides interface to convert data from/to Array format to/from int16 format.
 *
*/

static const char rcsid[] = "@(#)conversion,r_Conv_int16: $Id: int16.cc,v 1.8 2002/10/01 09:16:38 coman Exp $"; 
 
#include "conversion/int16.hh"

#include <float.h>
#include <string>
#include <cstring>
#include <strstream>
#include <sstream>
#include <algorithm>

using std::istringstream;
using std::istrstream;
using std::string;

#include "raslib/rminit.hh"
#include "raslib/parseparams.hh"
#include "raslib/primitivetype.hh"


const r_Dimension r_Conv_int16::srcIntervDim=1;
const r_Dimension r_Conv_int16::destIntervDim=2;
const r_ULong     r_Conv_int16::paramMin=6;
const char*       r_Conv_int16::paramSep=",";
const char*       r_Conv_int16::paramEq="=";
const char*       r_Conv_int16::paramFlipX="flipx";
const char*       r_Conv_int16::paramFlipY="flipy";
const char*       r_Conv_int16::paramStartX="startx";
const char*       r_Conv_int16::paramEndX="endx";
const char*       r_Conv_int16::paramResX="resx";
const char*       r_Conv_int16::paramStartY="starty";
const char*       r_Conv_int16::paramEndY="endy";
const char*       r_Conv_int16::paramResY="resy";

const r_Double r_Conv_int16::NULL_DB = 0.;
const r_Double r_Conv_int16::ZERO_DB = FLT_MIN;
const r_Double r_Conv_int16::ZERO_int16 = 0.;

r_Conv_int16::~r_Conv_int16( void )
{
	//nothing to care for
}

void r_Conv_int16::initGeoBBox( r_GeoBBox& cBBox )
{
	//flipy is selected by default
	cBBox.flipy = 1;
  
	//flipx is not selected by default
	cBBox.flipx = 0;

	//geo information are initialized by default to DBL_MAX
	// FIXME: better defaults res=1, min=-MAX?
	cBBox.startx = DBL_MAX;  
	cBBox.endx   = DBL_MAX;
	cBBox.resx   = DBL_MAX;
	cBBox.starty = DBL_MAX;  
	cBBox.endy   = DBL_MAX;
	cBBox.resy   = DBL_MAX;  
}

r_Conv_int16::r_Conv_int16(const char* source, const r_Minterval& lengthordomain, const r_Type* tp) throw(r_Error)
	:	r_Convertor(source, lengthordomain, tp, true)
{
	initGeoBBox(collBBox);
}

r_Conv_int16::r_Conv_int16(const char* source, const r_Minterval& lengthordomain, int tp) throw(r_Error)
	:	r_Convertor(source, lengthordomain, tp)
{
	initGeoBBox(collBBox);
}
	
bool 
r_Conv_int16::decodeOptions(const char* options,
			 r_GeoBBox& cBBox) throw()
{
	RMInit::logOut << "r_Conv_int16::decodeOptions(" << (options?options:"NULL") << ")" << endl;
  
	r_Parse_Params parseParams;
  
	initGeoBBox(cBBox);

	parseParams.add(paramFlipX,  &cBBox.flipx,  r_Parse_Params::param_type_int);
	parseParams.add(paramFlipY,  &cBBox.flipy,  r_Parse_Params::param_type_int);
	parseParams.add(paramStartX, &cBBox.startx, r_Parse_Params::param_type_double);
	parseParams.add(paramEndX,   &cBBox.endx,   r_Parse_Params::param_type_double);
	parseParams.add(paramResX,   &cBBox.resx,   r_Parse_Params::param_type_double);  
	parseParams.add(paramStartY, &cBBox.starty, r_Parse_Params::param_type_double);
	parseParams.add(paramEndY,   &cBBox.endy,   r_Parse_Params::param_type_double);  
	parseParams.add(paramResY,   &cBBox.resy,   r_Parse_Params::param_type_double);
   
	//process options
	r_Long processRet=parseParams.process(options);
	if(processRet < paramMin)
	{
		RMInit::logOut << "r_Conv_int16::decodeOptions(...) Error: Some required options are missing!" << endl;
		return false;	   
	}

	//check if start,res,end are present
	if(cBBox.startx == DBL_MAX)
	{
		RMInit::logOut << "r_Conv_int16::decodeOptions(...) Error: startx is not present!" << endl;
		return false;	   
	}	
	
	if(cBBox.starty == DBL_MAX)
	{
		RMInit::logOut << "r_Conv_int16::decodeOptions(...) Error: starty is not present!" << endl;
		return false;	   
	}	  
  
	if(cBBox.endx == DBL_MAX)
	{
		RMInit::logOut << "r_Conv_int16::decodeOptions(...) Error: endx is not present!" << endl;
		return false;	   
	}	
  
	if(cBBox.endy == DBL_MAX)
	{
		RMInit::logOut << "r_Conv_int16::decodeOptions(...) Error: endy is not present!" << endl;
		return false;
	}	  
  
	if(cBBox.resx == DBL_MAX)
	{
		RMInit::logOut << "r_Conv_int16::decodeOptions(...) Error: resx is not present!" << endl;
		return false;
	}	
  
	if(cBBox.resy == DBL_MAX)
	{
		RMInit::logOut << "r_Conv_int16::decodeOptions(...) Error: resy is not present!" << endl;
		return false;
	}	  
  

	//check res 
	if(!cBBox.resx)
	{
		RMInit::logOut << "r_Conv_int16::decodeOptions(...) Error: resx is zero!" << endl;
		return false;
	}	
  
	if(!cBBox.resy)
	{
		RMInit::logOut << "r_Conv_int16::decodeOptions(...) Error: resy is zero!" << endl;
		return false;
	}
	
	//check start >= end
	if(cBBox.startx >= cBBox.endx )
	{
		RMInit::logOut << "r_Conv_int16::decodeOptions(...) Error: startx >= endx!" << endl;
		return false;
	}	
  
	if(cBBox.starty >= cBBox.endy)
	{
		RMInit::logOut << "r_Conv_int16::decodeOptions(...) Erorr: starty >= endy!" << endl;
		return false;
	}   
  
	//show parsed options
	RMInit::logOut.setf(std::ios::fixed);
	RMInit::logOut  << "r_Conv_int16::decodeOptions(...) parsed options:" << endl
		<< " " << paramFlipX  << paramEq << cBBox.flipx 
		<< " " << paramFlipY  << paramEq << cBBox.flipy  << endl  
	  	<< " " << paramStartX << paramEq << cBBox.startx  
	  	<< " " << paramEndX   << paramEq << cBBox.endx 
	  	<< " " << paramResX   << paramEq << cBBox.resx   << endl
		<< " " << paramStartY << paramEq << cBBox.starty  
		<< " " << paramEndY   << paramEq << cBBox.endy 
		<< " " << paramResY   << paramEq << cBBox.resy   <<  endl;	
	return true;
}

string 
r_Conv_int16::encodeOptions(const r_GeoBBox& cBBox) throw()
{
	std::ostringstream os;
   
	os.str("");
	os.setf(std::ios::fixed);
	os	<< paramFlipX  << paramEq << cBBox.flipx 
		<< paramSep << paramFlipY  << paramEq << cBBox.flipy
		<< paramSep << paramStartX << paramEq << cBBox.startx  
		<< paramSep << paramEndX   << paramEq << cBBox.endx 
		<< paramSep << paramResX   << paramEq << cBBox.resx
		<< paramSep << paramStartY << paramEq << cBBox.starty  
		<< paramSep << paramEndY   << paramEq << cBBox.endy 
		<< paramSep << paramResY   << paramEq << cBBox.resy;
   
	RMInit::logOut << "r_Conv_int16::encodeOptions(" << os.str() << ")" << endl;   
   
	return os.str();
}	

void 
r_Conv_int16::checkLimits() throw(r_Error)
{
	//show processed data
	RMInit::logOut  << "r_Conv_int16::checkLimits() processed data:" << endl
		<< " minx=" << min.x  << " miny=" << min.y << " minh=" << min.h << endl
		<< " maxx=" << max.x  << " maxy=" << max.y << " maxh=" << max.h <<  endl;	
	// printf( "r_Conv_int16::checkLimits() processed data: minx=%8G, miny=%8G, minh=%8G, maxx=%8G, maxy=%8G, maxh=%8G\n", min.x, min.y, min.h, max.x, max.y, max.h );	

	if(collBBox.startx > min.x)
	{
		RMInit::logOut << "r_Conv_int16::checkLimits() startx( " << collBBox.startx << ") > min.x (" << min.x << " )!" << endl;
		throw r_Error();	     
	}
	if(collBBox.endx < max.x)
	{
		RMInit::logOut << "r_Conv_int16::checkLimits() endx( " << collBBox.endx << ") < max.x (" << max.x << " )!" << endl;
		throw r_Error();	     
	}  

	if(collBBox.starty > min.y)
	{
		RMInit::logOut << "r_Conv_int16::checkLimits() starty( " << collBBox.starty << ")  > min.y (" << min.y << " )!" << endl;
		throw r_Error();	     
	}  

	if(collBBox.endy < max.y)
	{
		RMInit::logOut << "r_Conv_int16::checkLimits() endy( " << collBBox.endy << ") < max.y (" << max.y << " )!" << endl;
		throw r_Error();	   
	}
}

void 
r_Conv_int16::readFromSrcStream() throw(r_Error)
{
	istrstream iFile(desc.src, desc.srcInterv[0].get_extent());
	string currStrRow;
	istringstream icurrRow;
	r_Long rowNo=0;
	r_Double noResx, noResy;
	int16Row currRow, prevRow;
 
	min.x=min.y=min.h=DBL_MAX;
	max.x=max.y=max.h=-DBL_MAX;
	demRows.clear();
 
	//process the lines
	while(!iFile.eof())
	{
		getline(iFile, currStrRow);
		rowNo++;
		if(currStrRow.empty())
		{
			RMInit::logOut << "r_Conv_int16::readFromSrcStream() skipping empty line " << rowNo << endl;  
			continue;
		}
		else
		{
			icurrRow.str(currStrRow);  

			//decode x
			icurrRow >> currRow.x;
			if(!icurrRow)
			{
				RMInit::logOut << "r_Conv_int16::readFromSrcStream() skiping line " << rowNo 
					<< "(unable to decode x) !" << endl;
				continue;
			}   
   
			//decode y   
			icurrRow >> currRow.y;
			if(!icurrRow)
			{
				RMInit::logOut << "r_Conv_int16::readFromSrcStream() skiping line " << rowNo 
					<< "(unable to decode y) !" << endl;
				continue;
			}      

			//decode h
			icurrRow >> currRow.h;
			if(!icurrRow)
			{
				RMInit::logOut << "r_Conv_int16::readFromSrcStream() skiping line " << rowNo 
					<< "(unable to decode h) !" << endl;
				continue;
			}

			//update to support NULL value: 0. (real value) goes in FLT_MIN(db value)
			//because 0.(db value) represent NULL(real value). When we do export we skip NULL values.    
			if(currRow.h == ZERO_int16) currRow.h=ZERO_DB;
    
			//FIXME we ignore this check, because it may happen to have a incomplet dem
			/*
			//check if we have resx, resy
			noResx=currRow.x/collBBox.resx;
			if((currRow.x - noResx*collBBox.resx) > 0.)
			{
				RMInit::logOut << "r_Conv_int16::readFromSrcStream() resolution for x on line " << 
					rowNo << " is not " << collBBox.resx << " !" << endl;   
				throw r_Error();
			}
			noResy=currRow.y/collBBox.resy;   
			if((currRow.y - noResy*collBBox.resy) > 0.)
			{
				RMInit::logOut << "r_Conv_int16::readFromSrcStream() resolution for y on line " << 
					rowNo << " is not " << collBBox.resy << " !" << endl;       
				throw r_Error();
			} 
			*/   
	
			//compute min, max for x,y,z
			min.x=std::min<r_Double>(min.x, currRow.x);
			min.y=std::min<r_Double>(min.y, currRow.y);   
			min.h=std::min<r_Double>(min.h, currRow.h);      
			max.x=std::max<r_Double>(max.x, currRow.x);
			max.y=std::max<r_Double>(max.y, currRow.y);   
			max.h=std::max<r_Double>(max.h, currRow.h);         
	
			//store currRow
			demRows.push_back(currRow);
		}//end if(currStrRow.empty())
	}//end reading src stream
	
	if(demRows.empty())
	{
		RMInit::logOut << "r_Conv_int16::readFromSrcStream() desc.src stream is empty !" << endl; 
		throw r_Error(); 
	} 
	
	//check limits
	checkLimits();
}


void 
r_Conv_int16::readToSrcStream() throw(r_Error)
{
	r_Long x=0, y=0;
	r_Long xlow=0, ylow=0;
	r_Long xhigh=0, yhigh=0;
	int16Row currRow;
	r_Bytes typeSize=0;
	r_Long offset=0;
	char* buffer=NULL;
 
	//initialize
	xlow=desc.srcInterv[0].low();
	ylow=desc.srcInterv[1].low();
 
	xhigh=desc.srcInterv[0].high();
	yhigh=desc.srcInterv[1].high();

	//compute min & max
	if (collBBox.flipx)
	{
		min.x=collBBox.endx - xhigh*collBBox.resx;
		max.x=collBBox.endx - xlow*collBBox.resx;   
	}
	else
	{
		min.x=collBBox.startx + xlow*collBBox.resx;
		max.x=collBBox.startx + xhigh*collBBox.resx; 
	}

	if(collBBox.flipy)
	{
		min.y=collBBox.endy - yhigh*collBBox.resy;
		max.y=collBBox.endy - ylow*collBBox.resy; 
	} 
	else
	{
		min.y=collBBox.starty + ylow*collBBox.resy;
		max.y=collBBox.starty + yhigh*collBBox.resy; 
	}
	
	min.h=DBL_MAX;
	max.h=-DBL_MAX;
	
	//check limits
	checkLimits(); 
	
	//prepare container
	demRows.clear();
	typeSize=((r_Primitive_Type*)desc.srcType)->size();
	buffer=new char[typeSize];
	if(!buffer)
	{
		RMInit::logOut << "r_Conv_int16::readToSrcStream() unable to claim memory !" << endl; 
		throw  r_Ememory_allocation();  
	} 
	
	for(y=ylow; y<=yhigh; y++)
	{
		if(collBBox.flipy)
			currRow.y=collBBox.endy - y*collBBox.resy;
		else
			currRow.y=collBBox.starty + y*collBBox.resy; 
	
		for(x=xlow; x<=xhigh; x++)
		{
			if(collBBox.flipx)
				currRow.x=collBBox.endx - x*collBBox.resx;
			else
				currRow.x=collBBox.startx + x*collBBox.resx;
			offset=desc.srcInterv.cell_offset(r_Point(x,y))*typeSize;
			memcpy(buffer, &desc.src[offset], typeSize);
			switch(desc.srcType->type_id())
			{
			case r_Type::BOOL:
				currRow.h=((r_Primitive_Type*)desc.srcType)->get_boolean(buffer);    
				break;    
			case r_Type::CHAR:
				currRow.h=((r_Primitive_Type*)desc.srcType)->get_char(buffer);
				break;
			case r_Type::OCTET:
				currRow.h=((r_Primitive_Type*)desc.srcType)->get_octet(buffer);
				break;
			case r_Type::SHORT:
				currRow.h=((r_Primitive_Type*)desc.srcType)->get_short(buffer);
				break;    
			case r_Type::USHORT: 
				currRow.h=((r_Primitive_Type*)desc.srcType)->get_ushort(buffer);
				break;
			case r_Type::LONG:
				currRow.h=((r_Primitive_Type*)desc.srcType)->get_long(buffer);
				break;
			case r_Type::ULONG:
				currRow.h=((r_Primitive_Type*)desc.srcType)->get_ulong(buffer);
				break;            
			case r_Type::FLOAT:
				currRow.h=((r_Primitive_Type*)desc.srcType)->get_float(buffer);
				break;
			case r_Type::DOUBLE: 
				currRow.h=((r_Primitive_Type*)desc.srcType)->get_double(buffer);
				break;
			default:
				//write message to log
				RMInit::logOut << "r_Conv_int16::readToSrcStream() srcType (" << desc.srcType->type_id() <<  ") unsupported !" << endl; 
				//clean up
				if(buffer)
				{
					delete[] buffer;
					buffer=NULL;
				}
				//report error
				throw r_Error();     
				break;
			}
			min.h=std::min<r_Double>(min.h, currRow.h);
			max.h=std::max<r_Double>(max.h, currRow.h);   
			demRows.push_back(currRow);
		}
	} 
	
	//clean up
	if(buffer)
	{
		delete[] buffer;
		buffer=NULL;
	} 
	
	if(demRows.empty())
	{
		RMInit::logOut << "r_Conv_int16::readToSrcStream() src stream is empty !" << endl; 
		throw r_Error(); 
	} 
}


void
r_Conv_int16::writeFromDestStream() throw(r_Error)
{
	int16RowVec::const_iterator iter, iterEnd;
	r_Long xdim, ydim, offset;
	r_Point currPt(destIntervDim);
	r_Bytes typeSize=0;


	//FIXME here we should modify for other type support
	if(desc.destType->type_id() != r_Type::DOUBLE)
	{
		RMInit::logOut << "r_Conv_int16::writeFromDestStream() destType (" << desc.destType->type_id() 
			<< ") is not " << r_Type::DOUBLE << " !" << endl; 
		throw r_Error();  
	}
	
	xdim=desc.destInterv[0].get_extent();
	ydim=desc.destInterv[1].get_extent();
	iter=demRows.begin();
	iterEnd=demRows.end();
	typeSize=((r_Primitive_Type*)desc.destType)->size();

	//FIXME correction for strange efect of r_Long cast with 1e-6
	while(iter != iterEnd)
	{
		if(collBBox.flipx)
			currPt[0]=(collBBox.endx - iter->x)/collBBox.resx + 1e-6;
		else
			currPt[0]=(iter->x - collBBox.startx)/collBBox.resx + 1e-6;   
		if(collBBox.flipy)
			currPt[1]=(collBBox.endy - iter->y)/collBBox.resy + 1e-6;
		else
			currPt[1]=(iter->y - collBBox.starty)/collBBox.resy + 1e-6;
		((r_Primitive_Type*)desc.destType)->set_double(&desc.dest[desc.destInterv.cell_offset(currPt)*typeSize], iter->h);
		++iter;
	}
}

void
r_Conv_int16::writeToDestStream(ofstream& oFile) throw(r_Error)
{
	int16RowVec::const_iterator iter, iterEnd;
	r_Double currH;

	if(!oFile.is_open())
	{
		RMInit::logOut << "r_Conv_int16::writeToDestStream() oFile is not opened !" << endl; 
		throw r_Error();  
	}
	oFile.setf(std::ios::fixed);
	
	iter=demRows.begin();
	iterEnd=demRows.end();
	while(iter != iterEnd)
	{
		//update to support NULL value: 0. (real value) goes in FLT_MIN(db value)
		//because 0.(db value) represent NULL(real value). When we do export we skip NULL values. 
		currH = iter->h;
		if(currH != NULL_DB)
		{
			//FIXME we have to implement different here when we change server scale algorithm
			if(currH == ZERO_DB)
				currH = ZERO_int16;
			oFile << iter->x << "\t" << iter->y << "\t" << currH << endl;
		}
		++iter;
	}
}

r_convDesc&
r_Conv_int16::convertFrom(const char* options) throw (r_Error)
{
	bool hasSrcType=true;
	
        RMInit::logOut << "r_Conv_int16::convertFrom(" << (options?options:"NULL") << ")" << endl;	
        
      	if(!desc.srcType)
      	{
		desc.srcType=get_external_type(desc.baseType);	
		hasSrcType=false;
	}   
	
	try
	{
		RMInit::logOut  << "r_Conv_int16::convertFrom(...) src interval=" << desc.srcInterv << endl;
		RMInit::logOut  << "r_Conv_int16::convertFrom(...) src type=" << desc.srcType->type_id() << endl;		

		//check options
		if(!decodeOptions(options, collBBox))
			throw r_Error();
	
		//check desc.srcInterv.dimension
		if(desc.srcInterv.dimension() != srcIntervDim)
		{
			RMInit::logOut << "r_Conv_int16::convertFrom(" << (options?options:"NULL") 
		 		 << ") desc.srcInterv dimension (" << desc.srcInterv.dimension() 
		  		 << " != " << srcIntervDim << " !" << endl;
		 	throw r_Error();		  
		}
      	
      		//check srcType
		if(!desc.srcType->isPrimitiveType())
		{
			RMInit::logOut << "r_Conv_int16::convertFrom(" << (options?options:"NULL") 
				 << ") desc.srcType (" << desc.srcType->type_id() 
		  		 << ") not supported, only primitive types !" << endl;
			throw r_Error();		  	
		}
	
		if(desc.srcType->isComplexType())
		{
			RMInit::logOut << "r_Conv_int16::convertFrom(" << (options?options:"NULL") 
		  		 << ") desc.srcType (" << desc.srcType->type_id() 
		  		 << ") not supported !" << endl;
			throw r_Error();		  	
		}

		//read src stream
 		readFromSrcStream();
	
		//convert from int16 to marray 	
		//--computing the marray domain
		desc.destInterv = r_Minterval(destIntervDim);
		 
		//FIXME correction for strange efect of r_Long cast with 1e-6
		if(collBBox.flipx)
  			desc.destInterv << r_Sinterval((r_Long)((collBBox.endx - max.x)/collBBox.resx + 1e-6), 
  			                               (r_Long)((collBBox.endx - min.x)/collBBox.resx + 1e-6));
		else
  		  	desc.destInterv << r_Sinterval((r_Long)((min.x - collBBox.startx)/collBBox.resx + 1e-6), 
  			                                 (r_Long)((max.x - collBBox.startx)/collBBox.resx + 1e-6));
		if(collBBox.flipy)
  			desc.destInterv << r_Sinterval((r_Long)((collBBox.endy - max.y)/collBBox.resy + 1e-6), 
  			                               (r_Long)((collBBox.endy - min.y)/collBBox.resy + 1e-6));	
		else
  			desc.destInterv << r_Sinterval((r_Long)((min.y - collBBox.starty)/collBBox.resy + 1e-6), 
  			  		               (r_Long)((max.y - collBBox.starty)/collBBox.resy + 1e-6));
  		 
        	RMInit::logOut  << "r_Conv_int16::convertFrom(...) dest interval=" << desc.destInterv << endl;

		//--creating the resulting type			
		desc.destType = new r_Primitive_Type("Double", r_Type::DOUBLE); 
		RMInit::logOut  << "r_Conv_int16::convertFrom(...) dest type=" << desc.destType->type_id() << endl;	

		//--claim memory for result
		desc.dest = (char*)mystore.storage_alloc(desc.destInterv.cell_count() * ((r_Primitive_Type*)desc.destType)->size());
		if(desc.dest==NULL)
		{
			RMInit::logOut << "r_Conv_int16::convertFrom(" << (options?options:"NULL") 
	  			 << ") unable to claim memory !" << endl;
			throw  r_Ememory_allocation();		  	
		}
		memset(desc.dest, 0, desc.destInterv.cell_count() * ((r_Primitive_Type*)desc.destType)->size());		

		//--write parsed data in desc.dest
		writeFromDestStream();
	}
	catch(r_Error& err)
	{
		//cleanup
		if(!hasSrcType)
		{
			delete desc.srcType;
			desc.srcType=NULL;
		}
 	
		//desc.destType
		if(desc.destType)
        	{
			delete desc.destType;
			desc.destType=NULL;
		}

		//desc.dest
		if(desc.dest)
		{
			mystore.storage_free(desc.dest);
			desc.dest=NULL;
		}

		//report error
		throw;
	}
	
	//cleanup
	if(!hasSrcType)
	{
		delete desc.srcType;
		desc.srcType=NULL;
	}
	
	//return result	
	return desc;
}
	

r_convDesc&
r_Conv_int16::convertTo(const char* options) throw (r_Error)
{
	bool hasSrcType=true;

        RMInit::logOut << "r_Conv_int16::convertTo(" << (options?options:"NULL") << ")" << endl;

	try
	{
 		if(!desc.srcType)
		{
			desc.srcType=get_external_type(desc.baseType);	
			hasSrcType=false;
		}	   
	 
        	RMInit::logOut  << "r_Conv_int16::convertTo(...) src interval=" << desc.srcInterv << endl;
		RMInit::logOut  << "r_Conv_int16::convertTo(...) src type=" << desc.srcType->type_id() << endl;

	 	//check options
	 	if(!decodeOptions(options, collBBox))
			throw r_Error();

		if(!desc.srcType->isPrimitiveType())
		{
			RMInit::logOut << "r_Conv_int16::convertTo(" << (options?options:"NULL") 
				<< ") desc.srcType (" << desc.srcType->type_id() 
				<< ") not supported, only primitive types !" << endl;
			throw r_Error();	   	
		}
		if(desc.srcType->isComplexType())
		{
			RMInit::logOut << "r_Conv_int16::convertTo(" << (options?options:"NULL") 
		  		<< ") desc.srcType (" << desc.srcType->type_id() 
		  		<< ") not supported !" << endl;
			throw r_Error();	   	
		}
	
		//read src data
		readToSrcStream();
	
		//convert from marray to int16;
		char* pTempFileName=NULL;
		string tempFileName;
		ofstream oFile;		
		FILE* pFile=NULL;
		size_t lenFile=0;
	
		//--create the temp file
		//FIXME for multithread application
		pTempFileName=tmpnam(NULL);
		if(pTempFileName==NULL)
		{
			RMInit::logOut << "r_Conv_int16::convertTo(" << (options?options:"NULL") 
		  		<< ") desc.srcType (" << desc.srcType->type_id() 
		  		<< ") unable to generate a tempory file !" << endl;
			throw r_Error();	   	
		}

		tempFileName=pTempFileName;
		oFile.open(tempFileName.c_str());
		if(!oFile.is_open())
		{
			RMInit::logOut << "r_Conv_int16::convertTo(" << (options?options:"NULL") 
		  		  << ") desc.srcType (" << desc.srcType->type_id() 
		  		  << ") unable to open the tempory file !" << endl;
			throw r_Error();	   	
		}	
	
        	RMInit::logOut  << "r_Conv_int16::convertTo(...) temp file=" << tempFileName << endl;

		//--get int16 format
		writeToDestStream(oFile);
		oFile.close();
	
		//--accessing the temp file
		if ((pFile = fopen(tempFileName.c_str(), "rb")) == NULL)
         	{
			RMInit::logOut << "r_Conv_int16::convertTo(): unable to read back file." << endl;
			throw r_Error(r_Error::r_Error_General);
		}
		fseek(pFile, 0, SEEK_END);
		lenFile = ftell(pFile);
        	RMInit::logOut  << "r_Conv_int16::convertTo(...) dest len=" << lenFile << endl;

		if (!lenFile)
		{
			RMInit::logOut << "r_Conv_int16::convertTo(): source contains only NULL values." << endl;
			throw r_Error();
		}

		//--creating the resulting type			
		desc.destType = new r_Primitive_Type("Char", r_Type::CHAR); 
	
		//--computing the marray domain
		desc.destInterv = r_Minterval(srcIntervDim);
		desc.destInterv << r_Sinterval((r_Long)0, (r_Long)lenFile - 1);
	
		RMInit::logOut  << "r_Conv_int16::convertTo(...) dest interval=" << desc.destInterv << endl;
		RMInit::logOut  << "r_Conv_int16::convertTo(...) dest type=" << desc.destType->type_id() << endl;        

		//--claim memory for desc.dest
		desc.dest = (char*)mystore.storage_alloc(lenFile);
		if(desc.dest==NULL)
		{
			RMInit::logOut << "r_Conv_int16::convertTo(" << (options?options:"NULL") 
		  		  << ") unable to claim memory !" << endl;
			throw r_Ememory_allocation();	   	
		}
		memset(desc.dest, 0, lenFile);
	
		//--story the data in desc.dest
		fseek(pFile, 0, SEEK_SET);
		fread(desc.dest, 1, lenFile, pFile);

		//clean up
		fclose(pFile);
		pFile=NULL;
		remove(pTempFileName);		
	}
	catch(r_Error& err)
	{
		//cleanup
		if(!hasSrcType)
		{
			delete desc.srcType;
			desc.srcType=NULL; 
		}

		//desc.destType
		if(desc.destType)
	  	{
         		delete desc.destType;
			desc.destType=NULL;
		}

		//desc.dest
		if(desc.dest)
		{
			mystore.storage_free(desc.dest);
			desc.dest=NULL;
		}
	 
		//rethrow error
		throw;
	}
	
	//clean up
 	if(!hasSrcType)
	{
		delete desc.srcType;
		desc.srcType=NULL; 
	}	
	
	//return result
	return desc;
}

const char*
r_Conv_int16::get_name() const throw()
{
	return get_name_from_data_format(r_int16);
}

r_Data_Format
r_Conv_int16::get_data_format() const throw()
{
	return r_int16;
}

r_Convertor* 
r_Conv_int16::clone() const throw(r_Error)
{
	return new r_Conv_int16(desc.src, desc.srcInterv, desc.srcType);
}
