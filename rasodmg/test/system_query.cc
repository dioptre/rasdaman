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
 * SOURCE: system_query.cc
 *
 * MODULE: rasodmg/test
 *
 * COMMENTS:
 *		None
*/

static const char rcsid[] = "@(#)rasodmg/test,SystemQuery: $Id: system_query.cc,v 1.11 2002/07/30 07:16:23 hoefner Exp $";

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include <unistd.h>
#include "system_query.hh"
#include "raslib/complex.hh"
#include "raslib/structure.hh"
#include "raslib/primitive.hh"
#include "raslib/primitivetype.hh"
#include "rasodmg/oqlquery.hh"
#include "rasodmg/storagelayout.hh"

int
main(int argc, char** argv)
	{
	SystemBasic::usageHeader = "system_compare version 0.9\n\t\tProgram for executing queries against RasDaMan\n";
	SystemBasic::usageFooter = "Required information:\n\t\tQuery file\n";
	int retval = 0;
	try	{
		retval = SystemQuery::doStuff(argc, argv);
		}
	catch (r_Error& e)
		{
		cout << "Caught Exception at top level: " << e.get_errorno() << " " << e.what() << endl;
		}
	}

int
SystemQuery::doStuff(int argc, char** argv)
	{
	installSignalHandlers();
	int retval = 0;
	retval = parseParams(argc, argv);
	if (retval == 0)
		{
		if (fileName)
			{
			r_Storage_Layout* stl = 0;
			db.set_servername(serverName);
			db.set_useridentification(userName, passwd);
			try	{
				db.open(baseName);
				FILE* filePointer = checkFile(fileName, retval);
				if (retval != 0)
					return retval;
				size_t dataSize = 0;
				char* data = getData(filePointer, dataSize, retval);
				if(retval != 0)
					return retval;
				char* myQuery = new char[dataSize + 1];
				memcpy(myQuery, data, dataSize);
				delete data;
				data = 0;
				myQuery[dataSize - 1] = '\0';
				RMInit::logOut << "Query " << myQuery << endl;
				r_OQL_Query q1(myQuery);
				RMInit::logOut << "OQL   " << q1.get_query() << endl;
				if (q1.is_update_query())
					{
					cout << "Starting Update Transaction" << endl;
					ta.begin();
					db.set_storage_format(storageFormat, storageFormatParams);
					db.set_transfer_format(transferFormat, transferFormatParams);
					if (retval == 0)
						{
						try	{
							RMDBGIF(20, RMDebug::module_tools, "WAITBEFOREQL", \
								RMInit::dbgOut << "Waiting 100 sec before query\n" << std::endl; \
								sleep(100); \
								RMInit::dbgOut << "Continue now\n" << std::endl; );
							r_oql_execute(q1);
							RMDBGIF(20, RMDebug::module_tools, "WAITAFTERQL", \
								RMInit::dbgOut << "Waiting 100 sec before query\n" << std::endl; \
								sleep(100); \
								RMInit::dbgOut << "Continue now\n" << std::endl; );
							}
						catch (r_Error& errorObj)
							{
							cout << "FAILED" << endl << errorObj.what() << endl;
							ta.abort();
							retval = EXCEPTIONEXECUTEQUERY;
							}
						}
					if (retval == 0)
						ta.commit();
					}
				else	{
					cout << "Starting Read Only Transaction ... "; cout.flush();
					ta.begin(r_Transaction::read_only);
					db.set_storage_format(storageFormat, storageFormatParams);
					db.set_transfer_format(transferFormat, transferFormatParams);
					cout << "OK" << endl;
					r_Set< r_Ref_Any > result_set;
					cout << "Executing query ... "; cout.flush();
					try	{
						RMDBGIF(20, RMDebug::module_tools, "WAITBEFOREQL", \
							RMInit::dbgOut << "Waiting 100 sec before query\n" << std::endl; \
							sleep(100); \
							RMInit::dbgOut << "Continue now\n" << std::endl; );
						r_oql_execute(q1, result_set);
						RMDBGIF(20, RMDebug::module_tools, "WAITAFTERQL", \
							RMInit::dbgOut << "Waiting 100 sec before query\n" << std::endl; \
							sleep(100); \
							RMInit::dbgOut << "Continue now\n" << std::endl; );
						}
					catch(r_Error& errorObj)
						{
						cout << "FAILED" << endl << errorObj.what() << endl;
						ta.abort();
						retval = EXCEPTIONEXECUTEQUERY;
						}
					if (retval == 0)
						{
						cout << "OK" << endl;
						cout << "Collection" << endl;
						cout << "	Oid...................: " << result_set.get_oid() << endl;
						cout << "	Type Structure........: " << (result_set.get_type_structure() ? result_set.get_type_structure() : "<nn>") << endl;
						cout << "	Type Schema...........: " << flush;
						if (result_set.get_type_schema())
							result_set.get_type_schema()->print_status(cout);
						else
							cout << "<nn>" << flush;
						cout << endl;
						cout << "	Number of entries.....: " << result_set.cardinality() << endl;
						cout << "	Element Type Schema...: " << flush;
						if (result_set.get_element_type_schema())
							result_set.get_element_type_schema()->print_status(cout);
						else
							cout << "<nn>" << flush;
						cout << endl;
						if (testBed)
							{
							cout << "-- Testbed line: result_type=" << result_set.get_type_structure() << endl;
							cout << "-- Testbed line: result_elements=" << result_set.cardinality() << endl;
							cout << endl;
							}

						r_Iterator< r_Ref_Any > iter = result_set.create_iterator();
						cout << endl;
						if (testBed)
							cout << "-- Testbed start block:" << endl;
						for (int i=1 ; iter.not_done(); iter++, i++)
							{
							switch (result_set.get_element_type_schema()->type_id())
								{
								case r_Type::MARRAYTYPE:
									{
									const char *defExt=NULL;
									r_Data_Format mafmt = r_Ref<r_GMarray>(*iter)->get_current_format();
									r_Data_Format tmpfmt = r_Data_Format_NUMBER;
									if(outputFormat)
										tmpfmt = outputFormat;
									else
										tmpfmt = mafmt;
									// special treatment only for DEFs
									switch (tmpfmt)
										{
										case r_TOR:
											defExt = "tor"; break;
										case r_DEM:
											defExt = "dem"; break;
										case r_VFF:
											defExt = "vff"; break;
										case r_TIFF:
											defExt = "tif"; break;
										case r_JPEG:
											defExt = "jpg"; break;
										case r_HDF:
											defExt = "hdf"; break;
										case r_PNG:
											defExt = "png"; break;
										case r_BMP:
											defExt = "bmp"; break;
										default:
											defExt = NULL;
										}
									if (outputFormat && (defExt == NULL))
										defExt = "raw";
									if (defExt == NULL)
										{
										if (printText)
											{
											int numCells = r_Ref<r_GMarray>(*iter)->get_array_size();
											const char* theStuff = r_Ref<r_GMarray>(*iter)->get_array();
											for (int cnt = 0; cnt < numCells; cnt++)
												cout << theStuff[cnt];
											}
										else	{
											cout << "Image " << i << endl;
											r_Ref<r_GMarray>(*iter)->print_status(cout, 0);
											}
										cout << endl;
										}
									else{
									
										if(outputFormat && mafmt!=outputFormat)
										{
										  r_Base_Type* conversionType=NULL;
										  r_Minterval* mddDomain=NULL; 
										  if(mafmt!=r_Array)
										  {
										   data=r_Ref<r_GMarray>(*iter)->get_array();
										   dataSize=r_Ref<r_GMarray>(*iter)->get_array_size();
										   mddDomain=(r_Minterval*)&(r_Ref<r_GMarray>(*iter)->spatial_domain());
										   conversionType=(r_Base_Type*)r_Ref<r_GMarray>(*iter)->get_base_type_schema();
										   //convert this from currentformat(DEF) to r_Array
										   if (convertFrom(mafmt, data, dataSize, *mddDomain, conversionType, NULL) == 0)
										   {
										    r_Ref<r_GMarray>(*iter)->set_array_size(dataSize);
										    r_Ref<r_GMarray>(*iter)->set_type_length(conversionType->size());
										    r_Ref<r_GMarray>(*iter)->set_array(data);
										    r_Ref<r_GMarray>(*iter)->set_current_format(r_Array);
										    r_Ref<r_GMarray>(*iter)->set_spatial_domain(*mddDomain);
										    data = 0; 
										   }
										   else
										   {
										     cout << "Error while converting to " << r_Array << " from " << mafmt << endl;
										     retval=CONVERSIONEXCEPTION;
										   }
										  }
										  //convert this from r_Array to outputFormat
										  data=r_Ref<r_GMarray>(*iter)->get_array();
										  dataSize=r_Ref<r_GMarray>(*iter)->get_array_size();
										  mddDomain=(r_Minterval*)&(r_Ref<r_GMarray>(*iter)->spatial_domain());
										  conversionType=(r_Base_Type*)r_Ref<r_GMarray>(*iter)->get_base_type_schema();
										  if (convertTo(outputFormat, data, dataSize, *mddDomain, conversionType, outputFormatParams) == 0)
										  {
										    r_Ref<r_GMarray>(*iter)->set_array_size(dataSize);
										    r_Ref<r_GMarray>(*iter)->set_type_length(conversionType->size());
										    r_Ref<r_GMarray>(*iter)->set_array(data);
										    r_Ref<r_GMarray>(*iter)->set_current_format(outputFormat);
										    r_Ref<r_GMarray>(*iter)->set_spatial_domain(*mddDomain);
										    data = 0;
										  }
										  else
										  {
										    cout << "Error while converting to " << outputFormat << " from " << r_Array << endl;
										    retval=CONVERSIONEXCEPTION;
										  }
										 }
									
										char defFileName[256];
										sprintf(defFileName, "%s%d.%s", outputFileName, i, defExt);
										cout << "Marray " << i << " will write " << r_Ref<r_GMarray>(*iter)->get_array_size() << " bytes to " << defFileName << endl;

										FILE *tfile = fopen(defFileName, "wb");
										fwrite((void*)r_Ref<r_GMarray>(*iter)->get_array(), 1, r_Ref<r_GMarray>(*iter)->get_array_size(), tfile);
										fclose(tfile);
										}
									}
									break;

								case r_Type::POINTTYPE:
									cout << "Element " << i << ": " << *(r_Ref<r_Point>(*iter)) << endl;						
									break;

								case r_Type::SINTERVALTYPE:
									cout << "Element " << i << ": " << *(r_Ref<r_Sinterval>(*iter)) << endl;						
									break;

								case r_Type::MINTERVALTYPE:
									cout << "Element " << i << ": " << *(r_Ref<r_Minterval>(*iter)) << endl;						
									break;

								case r_Type::OIDTYPE:
									cout << "Element " << i << ": " << *(r_Ref<r_OId>(*iter)) << endl;						
									break;

								default:
									cout << "Element " << i << ": " << flush;
									printScalar( *(r_Ref<r_Scalar>(*iter)) );
									cout << endl;
								}
							}
						}
					if (testBed)
						cout << "-- Testbed end block:" << endl;
					if (retval == 0)
						ta.commit();
					else
						ta.abort();
					}
				delete myQuery;
				myQuery = 0;
				db.close();
				}
			catch (r_Error& obj)
				{
				cout << "Exception (" << obj.get_errorno() << ") : " << obj.what() << endl;
				ta.abort();
				db.close();
				retval = EXCEPTIONADMIN;
				}
			}
		else	{
			retval = FILENAMENOTSPECIFIED;
			}
		}
	explainRetval(retval);
	return retval;
	}

void SystemQuery::printScalar( const r_Scalar& scalar )
{
  switch( scalar.get_type()->type_id() )
  {
    case r_Type::BOOL:
      cout << ( ((r_Primitive*)&scalar)->get_boolean() ? "T" : "F" ) << flush;            
      break;

    case r_Type::CHAR:
      cout << (int)((r_Primitive*)&scalar)->get_char() << flush;            
      break;

    case r_Type::OCTET:
      cout << (int)((r_Primitive*)&scalar)->get_octet() << flush;            
      break;

    case r_Type::SHORT:
      cout << ((r_Primitive*)&scalar)->get_short() << flush;            
      break;

    case r_Type::USHORT:
      cout << ((r_Primitive*)&scalar)->get_ushort() << flush;            
      break;

    case r_Type::LONG:
      cout << ((r_Primitive*)&scalar)->get_long() << flush;            
      break;

    case r_Type::ULONG:
      cout << ((r_Primitive*)&scalar)->get_ulong() << flush;            
      break;

    case r_Type::FLOAT:
      cout << ((r_Primitive*)&scalar)->get_float() << flush;            
      break;

    case r_Type::DOUBLE:
      cout << ((r_Primitive*)&scalar)->get_double() << flush;            
      break;

    case r_Type::COMPLEXTYPE1:
    case r_Type::COMPLEXTYPE2:
      cout << "(" << ((r_Complex*)&scalar)->get_re() << ", " << ((r_Complex*)&scalar)->get_im() << ")" << flush;            
      break;

    case r_Type::STRUCTURETYPE:
      {
        r_Structure* structValue = (r_Structure*)&scalar;

        cout << "{ " << flush;
 
        for( int i=0; i<structValue->count_elements(); i++ )
				{
          printScalar( (*structValue)[i] );

          if( i < structValue->count_elements()-1 ) cout << ", " << flush;
				}
        cout << " }" << endl;
      } 
      break;
  }
}
