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
 *  defconv.cc
 *
  *  Covert the DEF source file to another DEF destination file.
 *  Returns 0 for succes, otherwise 1.
 *
 *  Parameters:
 *
 *  --inputfilename <src-file>
 *     the filename of the src file, mandatory
 *  --outputfilename <dest-file>
 *     the filename of the dest file, mandatory
 *
 *  --inputformat name
 *     name of DEF fromat(see data_foramat from mddtypes.hh)
 *  --inputformatparams params
 *     params used by convertFrom methods(see the heder file for you DEF from conversion)
 *  --outputformat name
 *     name of DEF fromat(see data_foramat from mddtypes.hh)
 *  --outputformatparams params
 *     params used by convertFrom methods(see the heder file for you DEF from conversion)
 *
 *  --help
 *     print usage and exit
 */

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#endif
#endif

#include "defutil.hh"
#include "raslib/miter.hh"

//program data 
const char* paramPrgHelp="--help";
bool defPrgHelp=false;
char* prgHelp=NULL;

const char* paramSrcFileName="--srcfilename";
bool defSrcFileName=false;
char* srcFileName=NULL;

const char* paramSrcFormat="--srcformat";
bool defSrcFormat=false;
char* srcFormatc=NULL;
r_Data_Format srcFormat=r_Array;
char *srcIvc=NULL, *srcTypec=NULL;
r_Minterval srcIv;
r_Type* srcType=NULL;

const char* paramSrcFormatParams="--srcformatparams";
bool defSrcFormatParams=false;
char *srcFormatParams=NULL;

const char* paramDestFileName="--destfilename";
bool defDestFileName=false;
char *destFileName=NULL;

const char* paramDestFormat="--destformat";
bool defDestFormat=false;
char* destFormatc=NULL;
r_Data_Format destFormat=r_Array;
char *destIvc=NULL, *destTypec=NULL;
r_Minterval destIv;
r_Type* destType=NULL;

const char* paramDestFormatParams="--destformatparams";
bool defDestFormatParams=false;
char *destFormatParams=NULL;

//data used to parase formatparams for format Array
const char* fpDomain="domain";
const char* fpType="type";

//structures used for parseParam
const int paramsNo=7;

const char *paramsName[paramsNo]={ paramPrgHelp, paramSrcFileName, paramSrcFormat, paramSrcFormatParams, 
					paramDestFileName, paramDestFormat, paramDestFormatParams};
					
bool *paramsPresent[paramsNo]={ &defPrgHelp, &defSrcFileName, &defSrcFormat, &defSrcFormatParams,
							   &defDestFileName, &defDestFormat, &defDestFormatParams};
							   
char **paramsValue[paramsNo]={ &prgHelp, &srcFileName, &srcFormatc, &srcFormatParams,
							   &destFileName, &destFormatc, &destFormatParams};							   

void printStatus()
{
 cout << "defdiff parameters list:" << endl;
 for (int i=0; i<paramsNo; i++)
   cout << "--Name='" << paramsName[i] 
        << "' Present=" << ((*paramsPresent[i])? "true": "false") 
        << " Value='" << *paramsValue[i]  << "'" <<  endl;
}

void printUsage()
{
  char* fileNameSrc="test.bmp";
  char* fileNameDest="test.tiff";
  
  cout << "defconv v 0.1 - RasDaMan Data Exchange Format Convertor Utility" << endl;
  cout << "Description: Returns " << EXIT_SUCCESS << " for succes, otherwise " << EXIT_FAILURE << endl;
  cout << "For " << paramSrcFormat << "/" << paramDestFormat << " Array you have to specify the "  
       << paramSrcFormatParams << "/" << paramDestFormatParams << " as following:" << endl;
  cout << "\"" << fpDomain << "=<domain_str>," << fpType << "=<type_str>\" (e.g. \"" 
       << fpDomain << "=\"[0:477,0:464]\"," << fpType << "=char\")" << endl;
  cout << "Usage options:" << endl;
  cout << "        " << paramSrcFileName << " <src-file>        ... the filename used as src mandatory" << endl;
  cout << "        " << paramSrcFormat << " name                ... name of DEF fromat(see data_foramat from mddtypes.hh)," << endl;
  cout << "                                              default Array" << endl;
  cout << "        " << paramSrcFormatParams << " params        ... params used by convertFrom methods(see the header file" << endl;
  cout << "                                              for you DEF from conversion), default NULL" << endl;
  cout << "        " << paramDestFileName << " <dest-file>      ... the filename used as dest mandatory" << endl; 
  cout << "        " << paramDestFormat << " name               ... name of DEF fromat(see data_foramat from mddtypes.hh)," << endl;
  cout << "                                              default Array" << endl;
  cout << "        " << paramDestFormatParams << " params        ... params used by convertFrom methods(see the header file" << endl;
  cout << "                                              for you DEF from conversion), default NULL" << endl;
  cout << "        " << paramPrgHelp << "             ... this help" << endl;
  cout << "For example:" << endl;
  cout << "defconv " << paramSrcFileName << " " << fileNameSrc << " " << paramSrcFormat << " " << format_name_bmp <<" " 
  	   << paramDestFileName << " " << fileNameDest  << " " << paramDestFormat <<" " << format_name_tiff << endl;
  cout << "Report bugs to liviu.coman@active­knowledge.com" << endl;
  
}

int checkParam(int& paramIndex, char** param)
{
 int i=0;
 while(i<paramsNo)
 {
  if(!strcmp(param[paramIndex], paramsName[i]))
   break;
  i++;
 }
   
 if(i==paramsNo)
 {
   cout << "checkParam(...) error processing parameters: parameter " 
   		<< param[paramIndex] << " is unknown!" << endl;  
   return EXIT_FAILURE;
 }
 if(*paramsPresent[i])
 {
   cout << "checkParam(...) error processing parameters: parameter " 
   		<< param[paramIndex] << " is present more than once!" << endl;  
   return EXIT_FAILURE;
 }
 
 if(paramsName[i]!=paramPrgHelp)
 {
  *paramsPresent[i]=true;
  paramIndex++;
  *paramsValue[i]=param[paramIndex];
 }
 else
  *paramsPresent[i]=true;
 
 return EXIT_SUCCESS;
}

int parseParams(int argc, char** argv)
{
 int argIndex=0;
 if (argc==1)
 {
   cout << "parseParams(...) parameters " << paramSrcFileName << " <src-file> and " 
        << paramDestFileName << " <dest-file> are mandatatory!" << endl;   
   return EXIT_FAILURE;
 }
 else
 {
   argIndex=1;
   while(argIndex<argc)
   {
    if(checkParam(argIndex, argv)!=EXIT_SUCCESS)
      break;
    argIndex++;
   }
   if(argIndex<argc)
   {
    //error while parsing parameters
    return EXIT_FAILURE; 
   }

   //check rule	  
   if((defSrcFileName && defDestFileName) || (defPrgHelp))
     return EXIT_SUCCESS;
   else
   {
     cout << "parseParams(...) parameters " << paramSrcFileName << " <src-file> and " 
          << paramDestFileName << " <dest-file> are mandatatory!" << endl;
     return EXIT_FAILURE; 
   }
 }
}

int validateParams()
{
 
 //check srcformat
 if(defSrcFormat)
 {
  srcFormat = get_data_format_from_name(srcFormatc);
  if((srcFormat!=r_Array) &&
     (!r_Convertor_Factory::is_supported(srcFormat)))
    {
 	 cout << "convertDEFs(...) conversion " << srcFormatc << " not supported" << endl;
     return EXIT_FAILURE;
    }
 }
 
 //check destformat
 if(defDestFormat)
 {
    destFormat = get_data_format_from_name(destFormatc);
    if((destFormat!=r_Array) &&
       (!r_Convertor_Factory::is_supported(destFormat)))
    {
     cout << "convertDEFs(...) conversion " << destFormatc << " not supported" << endl;
     return EXIT_FAILURE;
    }
 }
 
 //check srcFormatParams if srcFormat=r_Array
 if(srcFormat == r_Array)
 {
  if(!defSrcFormatParams)
  {
   cout << "convertDEFs(...) for srcformat r_Array is necessary to provide the srcformatparams as described in help" << endl;
   return EXIT_FAILURE;
  }
  
  if(parseArrayParams((const char*)srcFormatParams, fpDomain, fpType, srcIvc, srcTypec) != EXIT_SUCCESS)
   return EXIT_FAILURE;   
  
  if(decodeMinterval(srcIvc, srcIv) != EXIT_SUCCESS)
   return EXIT_FAILURE; 
   
  if(decodeType(srcTypec, srcType) != EXIT_SUCCESS)
   return EXIT_FAILURE;    
 }
 
 //check destFormatParams if destFormat=r_Array 
 if(destFormat == r_Array)
 {
  if(!defDestFormatParams)
  {
   cout << "convertDEFs(...) for destformat r_Array is necessary to provide the srcformatparams as described in help" << endl; 
   return EXIT_FAILURE;
  }

  if(parseArrayParams((const char*)destFormatParams, fpDomain, fpType, destIvc, destTypec) != EXIT_SUCCESS)
   return EXIT_FAILURE;   
  
  if(decodeMinterval(destIvc, destIv) != EXIT_SUCCESS)
   return EXIT_FAILURE; 
   
  if(decodeType(destTypec, destType) != EXIT_SUCCESS)
   return EXIT_FAILURE;      
 }
}

int setCell(const char* srcCell, 
	    const r_Type* srcType,
	    const r_Type* destType, 
	    char* destCell,
	    bool rescale=false)
{
 r_Double srcTypeMin=0., srcTypeMax=0.;
 r_Double destTypeMin=0., destTypeMax=0.; 
 r_Double srcVal=0., destVal=0.;
 
 if(!srcCell)
 {
  cout << "setCell(...) srcCell is null" << endl;
  return EXIT_FAILURE;
 }
 if(!srcType)
 {
  cout << "setCell(...) srcType is null" << endl;
  return EXIT_FAILURE;
 } 
 if(!destType)
 {
  cout << "setCell(...) destType is null" << endl;
  return EXIT_FAILURE;
 } 
 if(!destCell)
 {
  cout << "setCell(...) destCell is null" << endl;
  return EXIT_FAILURE;
 } 
 
 try
 {
  srcVal=((r_Primitive_Type*)srcType)->get_value(srcCell);
  if(rescale)
  {
    ((r_Primitive_Type*)srcType)->get_limits(srcTypeMin, srcTypeMax);
    ((r_Primitive_Type*)destType)->get_limits(destTypeMin, destTypeMax);
    destVal=srcVal*destTypeMax/srcTypeMax;
  }
  else
    destVal=srcVal;
  ((r_Primitive_Type*)destType)->set_value(destCell,destVal);
 }
 catch(r_Error& err)
 {
  cout << "setCell(...) error while setting value to dest from src" << endl;
  cout << "Error " << err.get_errorno() << " : " << err.what() << endl;
  return EXIT_FAILURE;
 }
 
 return EXIT_SUCCESS;
}
int convertData(const r_convDesc& descSrc, r_convDesc& descDest)
{
 r_Minterval srcIv, destIv, commonIv;

 r_Bytes srcTypeSize=0, destTypeSize=0;
 char *srcBuffer=NULL, *destBuffer=NULL;

 if(!descSrc.dest)
 {
  cout << "convertData(...) descSrc.dest is null" << endl;
  return EXIT_FAILURE;
 }
 
 if(!descSrc.destType)
 {
  cout << "convertData(...) descSrc.destType is null" << endl;
  return EXIT_FAILURE;
 } 
 
 if(descDest.dest)
 {
  cout << "convertData(...) descDest.dest is not null" << endl;
  return EXIT_FAILURE;
 }
 
 if(!descDest.destType)
 {
  cout << "convertData(...) descDest.destType is null" << endl;
  return EXIT_FAILURE;
 }  
 
 //translate every interval to zero
 try
 {
  srcIv=descSrc.destInterv.create_reverse_translation(descSrc.destInterv.get_origin());
  destIv=descDest.destInterv.create_reverse_translation(descDest.destInterv.get_origin()); 
 }
 catch(r_Error& err)
 {
  cout << "convertData(...) error while translating to zero the following intervals:" << endl;
  cout << "descSrc.destInterv=" << descSrc.destInterv << endl;
  cout << "descDest.destInterv=" << descDest.destInterv << endl;
  cout << "Error " << err.get_errorno() << ":" << err.what() << endl;
  return EXIT_FAILURE;
 }
 
 //check intersection
 try
 {
  commonIv.intersection_of(srcIv, destIv);
 }
 catch(r_Error& err)
 {
  cout << "convertData(...) error while doing the intersection of the following intervals:" << endl;
  cout << "descSrc.destInterv=" << descSrc.destInterv << endl;
  cout << "descDest.destInterv=" << descDest.destInterv << endl;
  cout << "Error " << err.get_errorno() << ":" << err.what() << endl;
  return EXIT_FAILURE;
 }
 
 //get type size
 srcTypeSize=((r_Base_Type*)descSrc.destType)->size();
 destTypeSize=((r_Base_Type*)descDest.destType)->size(); 
 
 //allocate data
 srcBuffer=new char[srcTypeSize];
 if(!srcBuffer)
 {
  cout << "convertData(...) unable to claim memory for srcBuffer" << endl;
  return EXIT_FAILURE;
 } 
 destBuffer=new char[destTypeSize];
 if(!destBuffer)
 {
  cout << "convertData(...) unable to claim memory for destBuffer" << endl;
  delete[] srcBuffer;
  srcBuffer=NULL;
  return EXIT_FAILURE;
 }  
 descDest.dest=new char[descDest.destInterv.cell_count()*destTypeSize];
 if(!descDest.dest)
 {
  cout << "convertData(...) unable to claim memory for descDest.dest" << endl;
  delete[] srcBuffer;
  srcBuffer=NULL;
  delete[] destBuffer;
  destBuffer=NULL;  
  return EXIT_FAILURE;
 }
 memset(descDest.dest,0, descDest.destInterv.cell_count()*destTypeSize);
 
 r_Miter srcIter( &commonIv, &srcIv, srcTypeSize, descSrc.dest );
 r_Miter destIter( &commonIv, &destIv, destTypeSize, descDest.dest );
 while(!srcIter.isDone() || !destIter.isDone())
 {
  memcpy(srcBuffer, srcIter.nextCell(), srcTypeSize);
  if(setCell(srcBuffer, descSrc.destType, descDest.destType, destBuffer)!=EXIT_SUCCESS)
  {
   //abort algorithm
   break;
  }
  memcpy(destIter.nextCell(), destBuffer, destTypeSize);
 }
 
 //clean up
 delete[] srcBuffer;
 srcBuffer=NULL;
 delete[] destBuffer;
 destBuffer=NULL;  
 
 //check algorithm
 if(!srcIter.isDone() || !destIter.isDone())
 {
  cout << "convertData(...) error in algorithm" << endl;
  return EXIT_FAILURE;
 }
 
 return EXIT_SUCCESS;
}



int convertDEFs()
{
 char *dataSrc=NULL, *dataDest=NULL;
 r_ULong dataSrcSize=0, dataDestSize=0;
 r_convDesc descSrc, descDest;
 
 initConvDesc(descSrc);
 initConvDesc(descDest);
 
 //read the source file
 if(readFile(srcFileName, &dataSrc, dataSrcSize)!=EXIT_SUCCESS)
   return EXIT_FAILURE;
 
 if(!dataSrcSize)
 {
   cout << "convertDEFs() size of file " << srcFileName << " is zero" << endl;
   //clean up 
   delete [] dataSrc;
   dataSrc=NULL;
   return EXIT_FAILURE;  
 }
 
 //what is the input format?   
 if(srcFormat !=r_Array)
 {
   //it is safe to use srcType, srcIv because the srcFormat is not r_Array
   srcType=r_Convertor::get_external_type(r_Convertor::ctype_char);
   srcIv  = r_Minterval(1);
   srcIv << r_Sinterval((r_Long)0, (r_Long)(dataSrcSize - 1));
  
  //did conversion work?
  if(convertFrom(srcFormat, srcFormatParams, dataSrc, 
    	         srcIv, srcType, descSrc) != EXIT_SUCCESS)
  {
   //clean up 
   delete [] dataSrc;
   dataSrc=NULL;
   return EXIT_FAILURE;
  }
 }
 else
 {
  descSrc.dest=dataSrc;
  descSrc.destInterv=srcIv;
  descSrc.destType=srcType;
 }
 
 //what is the output format?
 if(destFormat !=r_Array)
 {
  if(convertTo(destFormat, destFormatParams, descSrc.dest, descSrc.destInterv, descSrc.destType, descDest) != EXIT_SUCCESS)
  {
   //clean up  
   cleanConvDesc(descSrc);
   return EXIT_FAILURE;
  }
   //prepare the data for writing
   dataDest=descDest.dest;
   dataDestSize=descDest.destInterv.cell_count() * ((r_Base_Type*)descDest.destType)->size();  
 }
 else
 {
  //prepare for conversion
  descDest.destType=destType;
  descDest.destInterv=destIv;
  descDest.dest=NULL;
  
  if(descSrc.destType->isPrimitiveType())
  {
   if(convertData(descSrc, descDest) != EXIT_SUCCESS)
   {
    //clean up
    cleanConvDesc(descSrc);
    cleanConvDesc(descDest);   
    return EXIT_FAILURE;   
   }
   
   //prepare the data for writing
   dataDest=descDest.dest;
   dataDestSize=descDest.destInterv.cell_count() * ((r_Base_Type*)descDest.destType)->size();
  }
  else
  {
   //we store the data as it is
   dataDest=descSrc.dest;
   dataDestSize=descSrc.destInterv.cell_count() * ((r_Base_Type*)descSrc.destType)->size();   
  }
   
 }
 

 
 //write data into destFileName
 if(writeFile(destFileName, (const char**)&dataDest, dataDestSize)!=EXIT_SUCCESS)
 {
   //clean up
   cleanConvDesc(descSrc);
   cleanConvDesc(descDest);   
   return EXIT_FAILURE;
 } 
 
 //clean up
 cleanConvDesc(descSrc);
 cleanConvDesc(descDest);
   
 return EXIT_SUCCESS;
}

int processRequest()
{
 if(validateParams() != EXIT_SUCCESS)
  return EXIT_FAILURE;
 return convertDEFs();
}

int main(int argc, char **argv)
{
 if(parseParams(argc, argv) != EXIT_SUCCESS)
 {
  printUsage();
  return EXIT_FAILURE;
 }
 else
 {
  if(!defPrgHelp)
  {
   printStatus();
   return processRequest();
  }
  else
  {
   printUsage();
   return EXIT_SUCCESS;
  }
 }
}
