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
 * SOURCE: system_update.cc
 *
 * MODULE: rasodmg/test
 *
 * COMMENTS:
 *			None
*/

static const char rcsid[] = "@(#)rasodmg/test,SystemUpdate: $Id: system_update.cc,v 1.21 2003/12/27 23:02:58 rasdev Exp $";

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include <unistd.h>
#include <math.h>

#include "system_update.hh"
#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"
#include "raslib/primitivetype.hh"
#include "raslib/marraytype.hh"
#include "raslib/mitera.hh"
#include "rasodmg/tiling.hh"
#include "rasodmg/stattiling.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/oqlquery.hh"
#include "clientcomm/clientcomm.hh"



int main(int argc, char** argv)
	{
	SystemBasic::usageHeader = "system_update version 0.9.4\n\t\tProgram for writing data into RasDaMan\n";
	SystemBasic::usageFooter = "Required information:\n\t\t MDD type\n\t\tCollection name\n\t\tMDD domain\n\t\tFile name with data\n";
	int retval = 0;
	try	{
		retval = SystemUpdate::doStuff(argc, argv);
		}
	catch (const r_Error& e)
		{
		cout << "Caught Exception at top level: " << e.get_errorno() << " " << e.what() << endl;
		retval = -1;
		}
	catch (const std::exception& e2)
		{
		cout << "Caught Exception at top level: " << e2.what() << endl;
		retval = -1;
		}
	catch (...)
		{
		cout << "Caught Unknown Exception at top level!" << endl;
		retval = -1;
		}
	return retval;
	}

int
SystemUpdate::doStuff(int argc, char** argv)
	{
#ifndef RMANDEBUG	
	installSignalHandlers();
#endif	
	int retval = 0;
	retval = parseParams(argc, argv);
	if (retval == 0)
		{
		if (mddTypeName)
			{
			if (mddDomainDef)
				{
				if (fileName && (conversionTypeName == NULL))
					{
					retval = CONVERSIONTYPEMISSING;
					}
				else	{
					if (fileName || (!fileName && transparent))
						{
						if (collName)
							{
							r_MiterArea* iter = NULL;
							r_Minterval tempDom;
							r_Minterval tileDom;
							if (tiledUpdate)
								{
								tempDom = overlayDomain;
								//taken from tiling.cc r_Size_Tiling
								r_Dimension dim = tempDom.dimension();
								r_Range edgeLength = (r_Range)std::max((r_Range)floor(pow(updateBufferSize, 1/(double)dim)), 1);
								if (align)
									{
									edgeLength = edgeLength - edgeLength%align;
									if (edgeLength < align)
										edgeLength = align;
									}
								tileDom = r_Minterval(dim);
								for (r_Dimension dimcnt = 0; dimcnt < dim; dimcnt++)
									tileDom << r_Sinterval(0, edgeLength - 1);
								iter = new r_MiterArea(&tileDom, &tempDom);
								RMInit::logOut << "Tiling domain " << tileDom << " complete domain " << tempDom << endl;
								//nextArea()
								//isDone()
								}
							do	{
								std::ostringstream selstream;
								std::ostringstream updstream;
								if (tiledUpdate)
									{
									mddDomain = iter->nextArea();
									overlayDomain = mddDomain;
									tiledUpdate = !iter->isDone();
									RMInit::logOut << "Doing now " << mddDomain << " " << overlayDomain << " done " << tiledUpdate << endl;
									}
								if (mddOIdDef)
									{
									updstream << "UPDATE " << collName << " AS a SET a" << overlayDomain << " ASSIGN $1 WHERE oid(a)=" << mddOId;
									}
								else	{
									updstream << "UPDATE " << collName << " AS a SET a" << overlayDomain << " ASSIGN $1";
									}
								if (transparent || !fileName)
									{
									if (overlayDomainDef)
										{
										if (overlayDomain.covers(mddDomain))
											{
											selstream << "SELECT A" << overlayDomain << " FROM " << collName << " AS A";
											}
										else	{
											RMInit::logOut << "Overlay domain " << overlayDomain << " does not cover MDD domain " << mddDomain << endl;
											retval = OVERLAYDOMAINDOESNOTMATCH;
											}
										}
									else	{
										selstream << "SELECT A" << mddDomain << " FROM " << collName << " AS A";
										}
									}
								if (retval == 0)
									{
									retval = doUpdate(updstream.str().c_str(), selstream.str().c_str());
									}
								}
							while (tiledUpdate && (retval == 0));
							delete iter;
							iter = NULL;
							}
						else	{
							retval = NOCOLLNAMEDEFINED;
							}
						}
					else	{
						retval = FILENAMENOTSPECIFIED;
						}
					}
				}
			else	{
				retval = MDDDOMAINNOTSPECIFIED;
				}
			}
		else	{
			retval = NOMDDTYPEDEFINED;
			}
		}
	explainRetval(retval);
	return retval;
	}

//SystemUpdate::doUpdate(const char* queryString, const char* queryStringS, const char* mddTypeName, const r_Marray_Type mddType, const r_Minterval& mddDomain, const char* inputFormatParams, )
int
SystemUpdate::doUpdate(const char* queryString, const char* queryStringS)
	{
	int retval = 0;
	char* typeStructure = NULL;
	r_Ref<r_GMarray> selectedMDD;
	r_Set< r_Ref_Any > result;
	r_Marray_Type* mddType = NULL;
	try	{
		openTransaction(false);
		typeStructure = db.getComm()->getTypeStructure(mddTypeName, ClientComm::r_MDDType_Type);
		ta.abort();
		db.close();
		}
	catch (r_Error& err)
		{
		if (err.get_kind() == r_Error::r_Error_DatabaseClassUndefined)
			{
			RMInit::logOut << "Type is not a well known type" << endl;
			typeStructure = new char[strlen(mddTypeName) + 1];
			strcpy(typeStructure, mddTypeName);
			}
		else	{
			RMInit::logOut << "Error during type retrieval from database: " << err.get_errorno() << " " << err.what() << endl;
			throw;
			}
		}
	try	{
		r_Type* tempType = r_Type::get_any_type(typeStructure);
		if (tempType->isMarrayType())
			{
			mddType = (r_Marray_Type*)tempType;
			tempType = NULL;
			}
		else	{
			RMInit::logOut << "The type (" << typeStructure << ") is not an marray type." << endl;
			delete tempType;
			tempType = NULL;
			retval = MDDTYPEINVALID;
			}
		}
	catch (r_Error& err)
		{
		RMInit::logOut << "Error during type retrieval from type structure (" << typeStructure << "): " << err.get_errorno() << " " << err.what() << endl;
		retval = MDDTYPEINVALID;
		}
	delete [] typeStructure;
	typeStructure = NULL;
	if (retval == 0)
		{
		size_t baseTypeLength = mddType->base_type().size();
		r_Storage_Layout* stl = new r_Storage_Layout(theTiling->clone());
		r_Ref<r_GMarray> fileMDD = new (mddTypeName)r_GMarray(mddDomain, baseTypeLength, stl);
		fileMDD->set_type_schema(mddType);
		stl = new r_Storage_Layout(theTiling->clone());
		r_Ref<r_GMarray> targetMDD = new (mddTypeName)r_GMarray(overlayDomain, baseTypeLength, stl);
		targetMDD->set_type_schema(mddType);
		if (fileName)
			{
			FILE* filePointer = checkFile(fileName, retval);
			if (retval != 0)
				{
				fileMDD.destroy();
				targetMDD.destroy();
				delete mddType;
				mddType = NULL;
				return retval;
				}
			retval = initGMarray(fileMDD, filePointer, inputFormat, mddDomain, inputFormatParams);
			}
		else	{
			RMInit::logOut << "Will not read from file" << endl;
			}
		if (retval == 0)
			{
			try	{
				openTransaction();
				if (strlen(queryStringS))
					{//do transparent update
					r_OQL_Query query(queryStringS);
					RMDBGIF(20, RMDebug::module_tools, "WAITBEFOREQL", \
						RMInit::dbgOut << "Waiting 100 sec before execute\n" << std::endl; \
						sleep(100); \
						RMInit::dbgOut << "Continue now\n" << std::endl; );
					bool dataFound = true;
					try	{
						r_oql_execute(query, result);
						}
					catch (r_Error& err)
						{
						dataFound = false;
						if (err.get_kind() != r_Error::r_Error_TransferFailed)
							{
							throw;
							}
						}
					RMDBGIF(20, RMDebug::module_tools, "WAITAFTERQL", \
						RMInit::dbgOut << "Waiting 100 sec after execute\n" << std::endl; \
						sleep(100); \
						RMInit::dbgOut << "Continue now\n" << std::endl; );
					if (dataFound)
						{
						r_Iterator< r_Ref_Any > iter = result.create_iterator();
						selectedMDD = r_Ref<r_GMarray>(*iter);
						if (polygonDefined)
							{
							polygon.setMArray(*selectedMDD);
							if (insidePatternSelDef)
								polygon.fillMArrayInside(insidePatternSel);
							if (outsidePatternSelDef)
								polygon.fillMArrayOutside(outsidePatternSel);
							}
						if (fileName)
							{
							retval = overlayGMarrays(targetMDD, fileMDD, selectedMDD);
							}
						else	{//just copy selected over target
							retval = overlayGMarrays(targetMDD, selectedMDD, targetMDD);
							}
						}
					else	{
						retval = overlayGMarrays(targetMDD, fileMDD, targetMDD);
						}
					}
				else	{
					retval = overlayGMarrays(targetMDD, fileMDD, targetMDD);
					}
				if (retval == 0)
					{
					if (scaleLevels == NULL)
						{
						r_OQL_Query query(queryString);
						query << *targetMDD;
						RMDBGIF(20, RMDebug::module_tools, "WAITBEFOREQL", \
							RMInit::dbgOut << "Waiting 100 sec before execute\n" << std::endl; \
							sleep(100); \
							RMInit::dbgOut << "Continue now\n" << std::endl; );
						r_oql_execute(query);
						RMDBGIF(20, RMDebug::module_tools, "WAITAFTERQL", \
							RMInit::dbgOut << "Waiting 100 sec after execute\n" << std::endl; \
							sleep(100); \
							RMInit::dbgOut << "Continue now\n" << std::endl; );
						}
					else	{
						std::list<std::pair<double, char*> >::iterator iter = scaleLevels->begin();
						std::list<std::pair<double, char*> >::iterator end = scaleLevels->end();
						r_Minterval scaledDomain;
						r_Minterval clipDomain;
						unsigned int length = 0;
						r_Dimension maxDim = mddDomain.dimension();
						r_Point origin(maxDim);
						double factor = 0;
						for (r_Dimension i = 0; i < maxDim; i++)
							origin[i] = 0;
						while ((iter != end) && (retval == 0))
							{
							factor = iter->first;
							if (factor == 1)
								{
								r_OQL_Query query(queryString);
								query << *targetMDD;
								RMDBGIF(20, RMDebug::module_tools, "WAITBEFOREQL", \
									RMInit::dbgOut << "Waiting 100 sec before execute\n" << std::endl; \
									sleep(100); \
									RMInit::dbgOut << "Continue now\n" << std::endl; );
								r_oql_execute(query);
								RMDBGIF(20, RMDebug::module_tools, "WAITAFTERQL", \
									RMInit::dbgOut << "Waiting 100 sec after execute\n" << std::endl; \
									sleep(100); \
									RMInit::dbgOut << "Continue now\n" << std::endl; );
								}
							else	{
								retval = scaleDomain(overlayDomain, origin, factor, scaledDomain, clipDomain, length);
								RMInit::logOut << "scaled: " << iter->second << " scaled domain " << scaledDomain << " clip domain " << clipDomain << " result=" << retval << endl;
								if (retval == 0)
									retval = updateScaledMDD(targetMDD, clipDomain, scaledDomain, length, iter->second);
								}
							iter++;
							}
						}
					RMDBGIF(20, RMDebug::module_tools, "WAITCOMMIT", \
						RMInit::dbgOut << "Waiting 100 sec before commit\n" << std::endl; \
						sleep(100); \
						RMInit::dbgOut << "Continue now\n" << std::endl; );
					}
				if (retval == 0)
					ta.commit();
				else
					ta.abort();
				}
			catch (r_Error& err)
				{
				RMInit::logOut << "Error during administrative action: " << err.get_errorno() << " " << err.what() << endl;
				retval = EXCEPTIONADMIN;
				ta.abort();
				}
			if (db.get_status() != r_Database::not_open)
				db.close();
			}
		fileMDD.destroy();
		targetMDD.destroy();
		}
	delete mddType;
	mddType = NULL;
	return retval;
	}

