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
 * SOURCE: system_compare.cc
 *
 * MODULE: rasodmg/test
 *
 * COMMENTS:
 *      None
*/

static const char rcsid[] = "@(#)rasodmg/test,SystemCompare: $Id: system_compare.cc,v 1.3 2002/07/16 07:42:36 hoefner Exp $";

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include "system_compare.hh"
#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"
#include "raslib/primitivetype.hh"
#include "raslib/marraytype.hh"
#include "rasodmg/tiling.hh"
#include "rasodmg/stattiling.hh"
#include "rasodmg/storagelayout.hh"
#include "rasodmg/oqlquery.hh"
#include "clientcomm/clientcomm.hh"

int main(int argc, const char** argv)
{
    SystemBasic::usageHeader = "system_compare version 0.9\n\t\tProgram for checking data in RasDaMan\n";
    SystemBasic::usageFooter = "Required information:\n\t\tMDD type\n\t\tCollection name\n\t\tMDD domain\n\t\tScale levels or file name\n";
    int retval = 0;
    try
    {
        retval = SystemCompare::doStuff(argc, argv);
    }
    catch (r_Error& e)
    {
        cout << "Caught Exception at top level: " << e.get_errorno() << " " << e.what() << endl;
    }
}

int
SystemCompare::doStuff(int argc, const char** argv)
{
    installSignalHandlers();
    int retval = 0;
    retval = parseParams(argc, (char**)argv);
    if (retval == 0)
    {
        if (mddTypeName)
        {
            if (mddDomainDef)
            {
                if (collName)
                {
                    if (fileName)
                    {
                        //compare given file with database
                        retval = doCompare();
                    }
                    else
                    {
                        if (scaleLevels != NULL)
                        {
                            //compare scalelevels
                            retval = doCompare();
                        }
                        else
                        {
                            retval = NOCOMPAREDESTINATION;
                        }
                    }
                }
                else
                {
                    retval = NOCOLLNAMEDEFINED;
                }
            }
            else
            {
                retval = MDDDOMAINNOTSPECIFIED;
            }
        }
        else
        {
            retval = NOMDDTYPEDEFINED;
        }
    }
    explainRetval(retval);
    return retval;
}

//SystemUpdate::doUpdate(const char* queryString, const char* queryStringS, const char* mddTypeName, const r_Marray_Type mddType, const r_Minterval& mddDomain, const char* inputFormatParams, )
int
SystemCompare::doCompare()
{
    int retval = 0;
    char queryBufferS[QUERYBUFFERLENGTH];
    memset(queryBufferS, 0, QUERYBUFFERLENGTH * sizeof(char));
    if (mddOIdDef)
    {
        std::ostrstream stream(queryBufferS, QUERYBUFFERLENGTH);
        stream << "SELECT A" << mddDomain << " FROM " << collName << " AS A WHERE oid(A) = " << mddOId;
    }
    else
    {
        std::ostrstream stream(queryBufferS, QUERYBUFFERLENGTH);
        stream << "SELECT A" << mddDomain << " FROM " << collName << " AS A";
    }
    r_Storage_Layout* stl = new r_Storage_Layout(theTiling->clone());
    char* typeStructure = NULL;
    r_Ref<r_GMarray> selectedMDD;
    r_Set< r_Ref_Any > result;
    r_Marray_Type* mddType = NULL;
    try
    {
        openTransaction(false);
        typeStructure = db.communication->getTypeStructure(mddTypeName, ClientComm::r_MDDType_Type);
        ta.abort();
        db.close();
    }
    catch (r_Error& err)
    {
        RMInit::logOut << "Error during type retrieval from database: " << err.get_errorno() << " " << err.what() << endl;
        typeStructure = new char[strlen(mddTypeName) + 1];
        strcpy(typeStructure, mddTypeName);
    }
    try
    {
        r_Type* tempType = r_Type::get_any_type(typeStructure);
        if (tempType->isMarrayType())
        {
            mddType = (r_Marray_Type*)tempType;
        }
        else
        {
            RMInit::logOut << "The type (" << typeStructure << ") is not an marray type." << endl;
            retval = MDDTYPEINVALID;
        }
    }
    catch (r_Error& err)
    {
        RMInit::logOut << "Error during type retrieval from type structure (" << typeStructure << "): " << err.get_errorno() << " " << err.what() << endl;
        retval = MDDTYPEINVALID;
    }
    if (retval == 0)
    {
        size_t baseTypeLength = mddType->base_type().size();
        if (fileName)
        {
            //check if file corresponds to collection
            r_Storage_Layout* stl = new r_Storage_Layout(theTiling->clone());
            r_Ref<r_GMarray> tempMDD = new (mddTypeName)r_GMarray(mddDomain, baseTypeLength, stl);
            tempMDD->set_type_schema(mddType);
            FILE* filePointer = checkFile(fileName, retval);
            if (retval != 0)
            {
                tempMDD.destroy();
                return retval;
            }
            retval = initGMarray(tempMDD, filePointer, inputFormat, mddDomain, inputFormatParams);
            fclose(filePointer);
            if (retval == 0)
            {
                try
                {
                    r_OQL_Query query(queryBufferS);
                    openTransaction(false);
                    r_oql_execute(query, result);
                    r_Iterator< r_Ref_Any > iter = result.create_iterator();
                    selectedMDD = r_Ref<r_GMarray>(*iter);
                    if (polygonDefined)
                    {
                        polygon.setMArray(*selectedMDD);
                        if (foreGroundDef)
                            polygon.fillMArrayInside(foreGround);
                        if (backGroundDef)
                            polygon.fillMArrayOutside(backGround);
                    }
                    if (retval == 0)
                    {
                        retval = compareGMarrays(selectedMDD, tempMDD);
                        if (((retval != 0) || (force)) && (outputFileName != NULL))
                        {
                            char* tempName = new char[strlen(outputFileName) + strlen("image.png") + 1];
                            strcpy(tempName, outputFileName);
                            strcat(tempName, ".db.png");
                            saveData(tempName, selectedMDD->get_array(), selectedMDD->get_array_size(), selectedMDD->spatial_domain());
                            strcpy(tempName, outputFileName);
                            strcat(tempName, ".image.png");
                            saveData(tempName, tempMDD->get_array(), tempMDD->get_array_size(), tempMDD->spatial_domain());
                            delete [] tempName;
                            tempName = NULL;
                        }
                    }
                    ta.abort();
                    db.close();
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
        }
        if (scaleLevels != NULL)
        {
            try
            {
                //initialise selectedMDD
                r_OQL_Query query(queryBufferS);
                openTransaction(false);
                r_oql_execute(query, result);
                r_Iterator< r_Ref_Any > iter = result.create_iterator();
                selectedMDD = r_Ref<r_GMarray>(*iter);
                if (polygonDefined)
                {
                    polygon.setMArray(*selectedMDD);
                    if (foreGroundDef)
                        polygon.fillMArrayInside(foreGround);
                    if (backGroundDef)
                        polygon.fillMArrayOutside(backGround);
                }
                if (retval == 0)
                {
                    //read the scaled mdds from db and scale down the selected MDD and compare
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
                        retval = scaleDomain(overlayDomain, origin, factor, scaledDomain, clipDomain, length);
                        RMInit::logOut << "scaled: " << iter->second << " scaled domain " << scaledDomain << " clip domain " << clipDomain << " result=" << retval << endl;
                        const r_Type* type = selectedMDD->get_base_type_schema();
                        size_t tlen = selectedMDD->get_type_length();
                        retval = compareScaledMDD(selectedMDD, clipDomain, scaledDomain, length, iter->second);
                        iter++;
                    }
                }
                ta.abort();
                db.close();
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
    }
    return retval;
}

