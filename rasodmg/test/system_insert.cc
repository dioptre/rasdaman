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
 * SOURCE: system_insert.cc
 *
 * MODULE: rasodmg/test
 *
 * COMMENTS:
 *      None
*/

static const char rcsid[] = "@(#)rasodmg/test,SystemInsert: $Id: system_insert.cc,v 1.10 2002/03/13 13:48:17 coman Exp $";

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include "system_insert.hh"
#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"
#include "raslib/primitivetype.hh"
#include "rasodmg/tiling.hh"
#include "rasodmg/stattiling.hh"
#include "rasodmg/storagelayout.hh"

int main(int argc, const char** argv)
{
    return SystemInsert::doStuff(argc, argv);
}

int
SystemInsert::doStuff(int argc, const char** argv)
{
    RMInit::logOut.rdbuf(cout.rdbuf());
    RMInit::dbgOut.rdbuf(cout.rdbuf());
    RMDebug::level = 10;
    r_Base_Type* conversionType = 0;
    int retval = 0;

    if ((argc == 1) || checkArguments(argc, argv, "-h", optionValueIndex) || checkArguments(argc, argv, "--help", optionValueIndex))
    {
        printUsage();
    }
    else
    {
        retval = readArgs(argc, argv);
        /*
        what can we do:
            create mdd:
                    requires
                        mddType
                    optional
                        collName ok
                        collOId  ok
            create coll:
                    requires
                        settype  ok
                    optional
                        collName ok
            insert existing mdd into coll:
                    requires
                        mddOId
                        collName/collOId
        */
        if (retval == 0)
        {
            if (mddTypeDef)
            {
                //create mdd
                if (mddOIdDef)
                {
                    //cannot create mdd with a user specified oid
                    retval = CREATEMDDWITHOID;
                }
                else
                {
                    if (fileNameDef)
                    {
                        if (mddDomainDef)
                        {
                            if (collNameDef)
                            {
                                //create mdd, lookup coll by name, insert mdd into coll
                                //open stuff
                                try
                                {
                                    openTransaction();
                                    //get collection
                                    try
                                    {
                                        collection = db.lookup_object(collName);
                                    }
                                    catch   (r_Error& obj)
                                    {
                                        RMInit::logOut << "Error (" << obj.get_errorno() << ") accessing collection " << collName << ": " << obj.what() << endl;
                                        retval = COLLECTIONINACCESSIBLE;
                                    }
                                    //create mddObject
                                    if (retval == 0)
                                    {
                                        r_Storage_Layout* stl = new r_Storage_Layout(theTiling->clone());
                                        //stl->set_storage_format(storageFormat, storageFormatParams);
                                        mddObject = new (&db, mddTypeName) r_GMarray(mddDomain, 0, stl);
                                        retval = initGMarray(mddObject, filePointer, inputFormat, mddDomain, inputFormatParams);
                                        if (retval == 0)
                                        {
                                            collection->insert_element(mddObject);
                                            ta.commit();
                                        }
                                        else
                                            ta.abort();
                                    }
                                    else
                                    {
                                        ta.abort();
                                    }
                                    db.close();
                                }
                                catch (r_Error& obj)
                                {
                                    cout << "Exception (" << obj.get_errorno() << ") at administrative action: " << obj.what() << endl;
                                    retval = EXCEPTIONADMIN;
                                    ta.abort();
                                    db.close();
                                }
                            }
                            else     //no collname
                            {
                                if (collOIdDef)
                                {
                                    //create mdd, lookup coll by oid, insert mdd into coll
                                    try
                                    {
                                        openTransaction();
                                        //get collection
                                        try
                                        {
                                            collection = db.lookup_object(collOId);
                                        }
                                        catch   (r_Error& obj)
                                        {
                                            RMInit::logOut << "Error (" << obj.get_errorno() << ") accessing collection " << collOId << ": " << obj.what() << endl;
                                            retval = COLLECTIONINACCESSIBLE;
                                        }
                                        //create mddObject
                                        if (retval == 0)
                                        {
                                            r_Storage_Layout* stl = new r_Storage_Layout(theTiling->clone());
                                            //stl->set_storage_format(dataFormat, dataFormatParams);
                                            mddObject = new (&db, mddTypeName) r_GMarray(mddDomain, 0, stl);
                                            retval = initGMarray(mddObject, filePointer, inputFormat, mddDomain, inputFormatParams);
                                            if (retval == 0)
                                            {
                                                collection->insert_element(mddObject);
                                                ta.commit();
                                            }
                                            else
                                                ta.abort();
                                        }
                                        else
                                        {
                                            ta.abort();
                                        }
                                        db.close();
                                    }
                                    catch (r_Error& obj)
                                    {
                                        cout << "Exception (" << obj.get_errorno() << ") at administrative action: " << obj.what() << endl;
                                        retval = EXCEPTIONADMIN;
                                        ta.abort();
                                        db.close();
                                    }
                                }
                                else     //no collname, no collOId, create mdd
                                {
                                    try
                                    {
                                        openTransaction();
                                        r_Storage_Layout* stl = new r_Storage_Layout(theTiling->clone());
                                        //stl->set_storage_format(storageFormat, storageFormatParams);
                                        mddObject = new (&db, mddTypeName) r_GMarray(mddDomain, 0, stl);
                                        retval = initGMarray(mddObject, filePointer, inputFormat, mddDomain, inputFormatParams);
                                        if (retval == 0)
                                        {
                                            collection->insert_element(mddObject);
                                            ta.commit();
                                        }
                                        else
                                            ta.abort();
                                        db.close();
                                    }
                                    catch (r_Error& obj)
                                    {
                                        cout << "Exception (" << obj.get_errorno() << ") at administrative action: " << obj.what() << endl;
                                        retval = EXCEPTIONADMIN;
                                        ta.abort();
                                        db.close();
                                    }
                                }
                            }
                        }
                        else
                        {
                            retval = MDDDOMAINNOTSPECIFIED;
                        }
                    }
                    else
                    {
                        retval = FILENAMENOTSPECIFIED;
                    }
                }
            }
            else     //create coll or insert existing mdd into existing coll
            {
                if (mddOIdDef)
                {
                    //insert existing mdd into existing coll
                    if (collOIdDef)
                    {
                        //lookup mdd by oid, lookup coll by oid, insert mdd into coll
                        try
                        {
                            openTransaction();
                            //get collection
                            try
                            {
                                collection = db.lookup_object(collOId);
                            }
                            catch   (r_Error& obj)
                            {
                                RMInit::logOut << "Error (" << obj.get_errorno() << ") accessing collection " << collOId << ": " << obj.what() << endl;
                                retval = COLLECTIONINACCESSIBLE;
                            }
                            if (retval == 0)
                            {
                                //lookup mddObject
                                try
                                {
                                    mddObject = db.lookup_object(mddOId);
                                }
                                catch (r_Error& obj)
                                {
                                    RMInit::logOut << "Error (" << obj.get_errorno() << ") accessing mdd " << mddOId << ": " << obj.what() << endl;
                                    retval = MDDINACCESSIBLE;
                                }
                                if (retval == 0)
                                {
                                    collection->insert_element(mddObject);
                                    ta.commit();
                                }
                                else
                                {
                                    ta.abort();
                                }
                            }
                            else
                            {
                                ta.abort();
                            }
                            db.close();
                        }
                        catch (r_Error& obj)
                        {
                            cout << "Exception (" << obj.get_errorno() << ") at administrative action: " << obj.what() << endl;
                            retval = EXCEPTIONADMIN;
                            ta.abort();
                            db.close();
                        }
                    }
                    else
                    {
                        if (collNameDef)
                        {
                            //lookup mdd by oid, lookup coll by name
                            try
                            {
                                openTransaction();
                                //get collection
                                try
                                {
                                    collection = db.lookup_object(collName);
                                }
                                catch   (r_Error& obj)
                                {
                                    RMInit::logOut << "Error (" << obj.get_errorno() << ") accessing collection " << collName << ": " << obj.what() << endl;
                                    retval = COLLECTIONINACCESSIBLE;
                                }
                                if (retval == 0)
                                {
                                    //lookup mddObject
                                    try
                                    {
                                        mddObject = db.lookup_object(mddOId);
                                    }
                                    catch (r_Error& obj)
                                    {
                                        RMInit::logOut << "Error (" << obj.get_errorno() << ") accessing mdd " << mddOId << ": " << obj.what() << endl;
                                        retval = MDDINACCESSIBLE;
                                    }
                                    if (retval == 0)
                                    {
                                        collection->insert_element(mddObject);
                                        ta.commit();
                                    }
                                    else
                                    {
                                        ta.abort();
                                    }
                                }
                                else
                                {
                                    ta.abort();
                                }
                                db.close();
                            }
                            catch (r_Error& obj)
                            {
                                cout << "Exception (" << obj.get_errorno() << ") at administrative action: " << obj.what() << endl;
                                retval = EXCEPTIONADMIN;
                                ta.abort();
                                db.close();
                            }
                        }
                        else     //no coll name, no coll oid, error
                        {
                            retval = NOCOLLNAMENOCOLLOID;
                        }
                    }
                }
                else     //do not create mdd, do not insert mdd into coll
                {
                    if (setTypeDef)
                    {
                        //create coll
                        //open stuff
                        try
                        {
                            openTransaction();
                            try
                            {
                                collection = new (&db, setTypeName) r_Set< r_Ref< r_GMarray > >;
                                if (collNameDef)
                                {
                                    //object.cc insert in db will throw r_error without name
                                    db.set_object_name(*collection, collName);
                                }
                                ta.commit();
                            }
                            catch (r_Error& obj)
                            {
                                cout << "Exception (" << obj.get_errorno() << ") at creating collection: " << obj.what() << endl;
                                retval = EXCEPTIONCREATECOLL;
                                ta.abort();
                            }
                            //create mddObject
                            db.close();
                        }
                        catch (r_Error& obj)
                        {
                            cout << "Exception (" << obj.get_errorno() << ") at administrative action: " << obj.what() << endl;
                            retval = EXCEPTIONADMIN;
                            ta.abort();
                            db.close();
                        }
                    }
                    else
                    {
                        retval = NOCOLLTYPE;
                    }
                }
            }
        }
        else
        {
            //errors occured in check arguments
        }
    }
    explainRetval(retval);
    return retval;
}

void
SystemInsert::printUsage()
{
    cout << "Insert program for extended systemtest" << endl << endl;
    SystemBasic::printOptions();
    cout << "Actions: Create and insert MDD object" << endl;
    cout << "           required information: MDD type" << endl;
    cout << "                                 Collection name or oid" << endl;
    cout << "                                 MDD domain" << endl;
    cout << "                                 File name with data" << endl;
    cout << "         Create MDD object (is not supported by rasdaman)" << endl;
    cout << "           required information: MDD type" << endl;
    cout << "                                 MDD domain" << endl;
    cout << "                                 File name with data" << endl;
    cout << "         Create Collection object" << endl;
    cout << "           required information: Collection type" << endl;
    cout << "           optional information: Collection name" << endl;
    cout << "         Insert existing MDD in existing Collection (is not supported by rasdaman)" << endl;
    cout << "           required information: MDD oid" << endl;
    cout << "                                 Collection name or oid" << endl;
    cout << endl;
}
