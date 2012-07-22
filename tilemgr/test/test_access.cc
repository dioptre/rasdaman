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

/****************************************************************************
 *
 *
 * PURPOSE:
 *   accesses data from a RasDaMan database according to the options
 *   specified in an input file. Optionally outputs timing and size
 *   information.
 *
 * DESCRIPTION:
 *  test_access reads from a text file a database name, and mddcolls
 *  names and for each one of them, it either reads a PersMDDColl and
 *  prints the status of all the mddobjs it contains (or part of them)
 *  or it deletes the MDDObjs.
 *  The program is able to print the tiles contents, show the accessing
 *  time and the sizes of each MDDObject.
 *  The input file has to start with the line:
 *         'Database:  <database name> '
 *      and after it, for every MDDCollection, the line:
 *         'MDDColl:  <mddcoll name> '
 *      which can be followed by the line:
 *         ' intersect:  <domain> '
 *      or by the line:
 *         ' pointQuery: <domain> '
 *      in this case, the cell with address equal to the origin of the
 *      tile is accessed.
 *      which causes printing only the status of the mddobjs which
 *      intersect with <domain>.
 *  Options and arguments are described in the printUsage( ) function.
 *
 *  Example input file :
 *    Database:  SMRasDaBase
 *    MDDColl: Images2DA
 *    intersect: [0:29,9:19]
 *    MDDColl: Toms3DA
 *
 *
 * COMMENTS:
 *  In this version, the -r option deletes whole collections only.
 *  To be extended in the future to delete selected objects and/or
 *  selected parts of objects.
 *
 ***************************************************************************/

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "server/template_inst.hh"
#endif
#endif

#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <vector.h>

#ifdef BASEDB_O2
#include "o2template_CC.hxx"  // declaration of O2 ref and coll classes
#endif

#include "raslib/rminit.hh"
#include "raslib/rmdebug.hh"
#include "typefactory.hh"
#include "ulongtype.hh"       // from the catalogif base DBMS interface module    

#include "mddmgr/persmddcoll.hh"
#include "mddmgr/persmddobj.hh"

#include "mddmgr/persmddcolliter.hh"

#include "adminif.hh"
#include "databaseif.hh"
#include "transactionif.hh"

extern char* myExecArgv0 = "";

RMINITGLOBALS('C')

#define    MAX 256
ifstream  testfile;

void   testAccessing( char* persMDDCollName, char* search,
                      int printTilesContents, int timeAccess,
                      int printSizes, int printCompression );
void intersectOption( char* search,
                      PersMDDColl*  accessedColl,
                      int printTilesContents,
                      int timeAccess,
                      int printSizes );
void pointQueryOption( char* search,
                       PersMDDColl*  accessedColl,
                       int timeAccess,
                       int printSizes );
void testDeleteColls( DatabaseIf* db, char* persMDDCollName,
                      char* search, int timeAccess, int printSizes );

int   checkArguments( int argc, char** argv, const char* searchText,
                      int& optionValueIndex );
void      printUsage( );

/**********************************************************************
 * Function name.: int main (int argc, char** argv)
 *
 * Arguments.....:
 *   argc: number of arguments given to program
 *   argv: array of char* with arguments
 * Return value..: exit status
 * Description...: none
 **********************************************************************/

int
main( int argc, char** argv)
{
    // variables representing O2 database, test file, ta and session
    DatabaseIf                               database;
    TransactionIf                                  ta;
    // static char O2BenchSchemaName[] = "SMRasDaSchema";
    static char                    O2BenchDBName[MAX];
    char                     mc_name[MAX],search[MAX];
    int                        printTilesContents = 0;
    int                                timeAccess = 0;
    int                                printSizes = 0;
    int                          printCompression = 0;
    int                               deleteColls = 0;
    int                           i, optionValueIndex;

    // don't forget to initialize before using AdminIf!
    myExecArgv0 = argv[0];
    AdminIf* myAdmin = AdminIf::instance();

    // is help is needed
    if (checkArguments (argc, argv, "-h", optionValueIndex))
    {
        printUsage( );
        delete myAdmin;
        exit( 0 );
    }

    // openning the test file
    if (checkArguments (argc, argv, "-f", optionValueIndex))
    {
        for (i = 0; argv[optionValueIndex][i+1] != '\0'; i++)
            search[i] = argv[optionValueIndex][i+2];
        testfile.open (search);
        if (!testfile)
        {
            cout << "cannot open this file" << endl;
            return 1;
        }
    }
    else
        printUsage( );
    /*
    {
      cout << "Please write:  test_access  -f<testfile-name>" << endl;
      cout << "Or maybe:      test_access  -h   for help...." << endl;
      return 1;
    }
    */

    // updating the flag of printing the tiles
    if (checkArguments (argc, argv, "-p", optionValueIndex))
        printTilesContents = 1;

    // updating the flag of timming the access
    if (checkArguments (argc, argv, "-t", optionValueIndex))
        timeAccess = 1;

    // updating the flag of printing the sizes of mddobjs
    if (checkArguments (argc, argv, "-s", optionValueIndex))
        printSizes = 1;

    // updating the flag of deletion
    if (checkArguments (argc, argv, "-r", optionValueIndex))
        deleteColls = 1;

    // updating the flag of compression
    if (checkArguments (argc, argv, "-c", optionValueIndex))
        printCompression = 1;

    // reading data-base name
    testfile.getline (search, MAX, ':');
    while ((!strstr (search, "Database")) && (!testfile.eof ( )))
        testfile.getline (search, MAX, ':');
    testfile >> O2BenchDBName;

    // connect to the database
    cout << "Connecting to database " << O2BenchDBName
         << "..." << endl;
    database.open (O2BenchDBName);

    // searching for mddcalls
    testfile.getline (search, MAX, ':');
    while (!testfile.eof ( ))
    {
        ta.begin ( &database );

        // searching for mddcall
        while ((!strstr (search, "MDDColl")) && (!testfile.eof ( )))
            testfile.getline (search, MAX, ':');
        if (strstr (search, "MDDColl"))
        {
            testfile >> mc_name;
            testfile.getline (search, MAX, ':');
            if (!deleteColls)
                testAccessing (mc_name, search, printTilesContents, timeAccess,
                               printSizes, printCompression);
            else
                testDeleteColls(&database, mc_name, search, timeAccess, printSizes);
        }

        ta.commit( );
    }

    cout << endl<< "Ending O2 session..." << endl;
    database.close( );
    testfile.close( );
    delete myAdmin;
}

/**********************************************************************
 * Function......:
 *   testAccessing( char* persMDDCollName, char* search,
 *                  int printTilesContents, int timeAccess,
 *                  int printSizes)
 * Arguments.....:
 *       persMDDCollName: the MDDColl name
 *                search: for searching the words from the file
 *    printTilesContents: the flag of printing the tiles
 *            timeAccess: the flag of timming the access
 * Return value..: none
 * Description...: reads DirTilesIx's and shows contents
 **********************************************************************/

void testAccessing( char* persMDDCollName, char* search,
                    int printTilesContents, int timeAccess,
                    int printSizes, int printCompression)
{
    PersMDDObj*     accessedObj;
    PersMDDColl*   accessedColl;

    cout << endl << endl << "....testAccessing " << persMDDCollName << endl;
    try
    {
        accessedColl = new PersMDDColl(persMDDCollName);
    }
    catch(...)
    {
        cout << " Error: exception caught when trying to access collection ";
        cout << endl;
        return;
    }

    // To test PersMDDColl::printStatus( )
    // accessedColl->printStatus( );

    // To test PersMDDObj::printStatus( ), MDDCollIter::createIterator( ) and
    // MDDCollIter methods :

    // cout << " Iterating through the collection with PersMDDCollIter " << endl;
    // MDDCollIter* objsIt = accessedColl->createIterator( );


    // the regular option of showing the contents
    if ((strstr (search, "MDDColl")) || (testfile.eof ( )))
    {
        cout << " Iterating through the collection with PersMDDCollIter " << endl;
        MDDCollIter* objsIt = accessedColl->createIterator( );

        for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
        {
            accessedObj = (PersMDDObj*) objsIt->getElement();
            cout << endl << endl << "    --" << i << ". MDD object in set:" << endl;

            if (printTilesContents && timeAccess)
            {
                vector<Tile*>*  TileVector;

                // start counter
                RMInit::dbgOut.rdbuf(cout.rdbuf());
                RMTimer* Counter = new RMTimer("PersMDDObj", "getTiles");
                TileVector = accessedObj->getTiles ( );

                // stop counter
                cout << "The time of accessing by ";
                delete Counter;

                vector<Tile*> ::iterator   t;
                for (t = TileVector->begin ( ); t != TileVector->end ( ); t++)
                {
                    (*t)->printStatus( );
                    cout << endl;
                }

                TileVector->erase( TileVector->begin(), TileVector->end() );
                delete TileVector;
            }

            else if (printTilesContents && !timeAccess)
            {
                vector<Tile*>*  TileVector;
                TileVector = accessedObj->getTiles ( );

                vector<Tile*> ::iterator   t;
                for (t = TileVector->begin ( ); t != TileVector->end ( ); t++)
                {
                    (*t)->printStatus( );
                    cout << endl;
                }

                TileVector->erase( TileVector->begin(), TileVector->end() );
                delete TileVector;
            }

            else if (!printTilesContents && timeAccess)
            {
                // start counter
                RMInit::dbgOut.rdbuf(cout.rdbuf());
                RMTimer* Counter = new RMTimer("PersMDDObj", "printStatus");
                accessedObj->printStatus ( );

                // stop counter
                cout << endl << "The time of accessing by ";
                delete Counter;
            }

            else if (!printTilesContents && !timeAccess)
            {
                accessedObj->printStatus ( );
            }

            if (printSizes)
            {
                // printing the sizes of the mddobj
                cout << endl << "The logical-size of the mddobj: " << accessedObj->getLogicalSize ( ) << endl;
                cout << "The physical-size of the mddobj: " << accessedObj->getPhysicalSize ( ) << endl;
                cout << "The physical-cells-size of the mddobj: " << accessedObj->getPhysicalCellsSize ( ) << endl;
                cout << "The index-size of the mddobj: " << accessedObj->getIxSize ( ) << endl;
            }

            if (printCompression)
            {
                int i = 0;
                unsigned long totalSize = 0;

                vector<Tile*>*  TileVector;
                TileVector = accessedObj->getTiles ( );

                vector<Tile*> ::iterator   t;
                for (t = TileVector->begin ( ); t != TileVector->end ( ); t++)
                {
                    cout << ++i << ". ";
                    if( (*t)->getDataFormat() == r_ZLib )
                    {
                        totalSize += (*t)->getPhysicalSize( );
                        cout << "Size of compressed tile: "
                             << (*t)->getPhysicalSize( ) << endl;
                    }
                    else
                    {
                        cout << "Tile not compressed! Size: " << (*t)->getSize() << endl;

                    }
                }
                TileVector->erase( TileVector->begin(), TileVector->end() );
                delete TileVector;

                if(totalSize != 0)
                    cout << "Total size of compressed tiles: " << totalSize << endl;
            }
        } // for objsIt loop

        delete objsIt;

    }  // if MDDColl

    // the intersect option
    else if (strstr (search, "intersect"))
    {
        intersectOption( search, accessedColl, printTilesContents, timeAccess, printSizes);
    }
    else if (strstr (search, "pointQuery"))
    {
        pointQueryOption( search, accessedColl, timeAccess, printSizes);
    }
    else
    {
        cout << "don't know what is this..." << endl;
        testfile.getline (search, MAX, ':');
    }



    accessedColl->releaseAll( );
    delete accessedColl;
}



void
intersectOption( char* search,
                 PersMDDColl*  accessedColl,
                 int printTilesContents,
                 int timeAccess,
                 int printSizes )
{
    PersMDDObj* accessedObj;

    int cont = 1;
    for ( ; cont == 1  ; ) // intersect loop
    {
        testfile.get (search, MAX, '[');
        testfile.getline (search, MAX, ']');
        strcat (search, "]");

        cout << " Iterating through the collection with PersMDDCollIter " << endl;
        MDDCollIter* objsIt = accessedColl->createIterator( );

        for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
        {
            accessedObj = (PersMDDObj*) objsIt->getElement();
            cout << endl << endl << "    --" << i << ". MDD object in set:" << endl;

            r_Minterval  interval (search);
            cout << "Intersecting to "<< interval << endl;
            vector<Tile*>*      TileVector;
            vector<Tile*> ::iterator     t;
            RMTimer* Counter;

            if ( timeAccess )
            {
                // start counter
                //  RMInit::dbgOut = cout.rdbuf();
                RMInit::bmOut << "Intersect Region " << interval << " " << "\t";
                // Counter = new RMTimer(" PersMDDObj", "intersect");
            }

            TileVector = accessedObj->intersect (interval);

            if ( printTilesContents )
            {
                int j  = 1;
                for (t = TileVector->begin ( ); t != TileVector->end ( ); t++, j++)
                {
                    cout << "Tile "<< j <<": "<<endl;
                    (*t)->printStatus( );
                    cout << endl;
                }
            }
            else
            {
                int j  = 1;
                for (t = TileVector->begin ( ); t != TileVector->end ( ); t++, j++)
                    cout << "Tile " << j << ": " << (*t)->getDomain ( ) << endl;
            }

            // ?? TileVector->erase( );
            delete TileVector;

            if (printSizes)
            {
                // printing the sizes of the mddobj
                cout << endl << "The logical-size of the mddobj: " << accessedObj->getLogicalSize ( ) << endl;
                cout << "The physical-size of the mddobj: " << accessedObj->getPhysicalSize ( ) << endl;
                cout << "The physical-cells-size of the mddobj: " << accessedObj->getPhysicalCellsSize ( ) << endl;
                cout << "The index-size of the mddobj: " << accessedObj->getIxSize ( ) << endl;
            }
        } // for objsIt loop

        delete objsIt;
        testfile.getline (search, MAX, ':');
        cout << search << endl;
        if (  !strstr( search, "intersect")  )
            cont = 0;
        // cout << " cont "<< cont << endl;
    }   // for cont loop
}

void
pointQueryOption( char* search,
                  PersMDDColl*  accessedColl,
                  int timeAccess,
                  int printSizes )
{
    PersMDDObj* accessedObj;

    int cont = 1;
    for ( ; cont == 1  ; ) // intersect loop
    {
        testfile.get (search, MAX, '[');
        testfile.getline (search, MAX, ']');
        strcat (search, "]");

        cout << " Iterating through the collection with PersMDDCollIter " << endl;
        MDDCollIter* objsIt = accessedColl->createIterator( );

        for( int i = 1 ; objsIt->notDone( ); i++, objsIt->advance( ))
        {
            accessedObj = (PersMDDObj*) objsIt->getElement();
            cout << endl << endl << "    --" << i << ". MDD object in set:" << endl;

            r_Minterval  interval (search);
            r_Point pnt = interval.get_origin( );
            cout << "Point query to "<< pnt << endl;

            RMTimer* Counter;
            if ( timeAccess )
            {
                // start counter
                RMInit::bmOut << "Intersect Region " << interval << " " << "\t";
                // Counter = new RMTimer(" PersMDDObj", "intersect");
            }

            char* result = accessedObj->pointQuery(pnt);
            if ( result )
                cout << "Result of point query : "<< long(*result) <<endl;
            else
                cout << "Null pointer returned from pointQuery!!!! "<< endl;

            if (printSizes)
            {
                // printing the sizes of the mddobj
                cout << endl << "The logical-size of the mddobj: " << accessedObj->getLogicalSize ( ) << endl;
                cout << "The physical-size of the mddobj: " << accessedObj->getPhysicalSize ( ) << endl;
                cout << "The physical-cells-size of the mddobj: " << accessedObj->getPhysicalCellsSize ( ) << endl;
                cout << "The index-size of the mddobj: " << accessedObj->getIxSize ( ) << endl;
            }
        } // for objsIt loop

        delete objsIt;
        testfile.getline (search, MAX, ':');
        cout << search << endl;
        if (  !strstr( search, "pointQuery")  )
            cont = 0;
        // cout << " cont "<< cont << endl;
    }   // for cont loop
}



/**********************************************************************
 * Function......:
 *   testDeleteColls( DatabaseIf* db, char* persMDDCollName,
 *                    char* search, int timeAccess,int printSizes )
 *
 * Arguments.....:
 *       persMDDCollName: the MDDColl name
 *                search: for searching the words from the file
 *    printTilesContents: the flag of printing the tiles
 *            timeAccess: the flag of timming the access
 * Return value..: none
 * Description...: reads DirTilesIx's and shows contents
 **********************************************************************/

void testDeleteColls( DatabaseIf* db, char* persMDDCollName,
                      char* search, int timeAccess, int printSizes)
{
    // PersMDDObj*     accessedObj;
    PersMDDColl*     accessedColl;

    /*
    cout << endl << endl << "... Destroying root collection "
         << persMDDCollName << endl;
    PersMDDColl::destroyRoot( persMDDCollName, db );
    */

    cout << endl << endl << "....testAccessing " << persMDDCollName << endl;
    try
    {
        accessedColl = new PersMDDColl( persMDDCollName );
    }
    catch(...)
    {
        cout << " Error: exception caught when trying to access collection ";
        cout << endl;
        return;
    }

    accessedColl->removeAll( );
    accessedColl->releaseAll( );

    cout << endl << endl << "... Destroying root collection "
         << persMDDCollName << endl;
    PersMDDColl::destroyRoot( persMDDCollName, db );

}

/**********************************************************************
 * Function......: checkArguments (int argc, char** argv,
 *                                 const char* searchText,
 *                                 int& optionValueIndex)
 *
 * Arguments.....:
 *                argc: number of arguments given to program
 *                argv: array of char* with arguments
 *          searchText: the desire option
 *    optionValueIndex: the index of the found option
 * Return value..: int
 * Description...: searching an option in the accexuting line
 **********************************************************************/
int checkArguments (int argc, char** argv, const char* searchText,
                    int& optionValueIndex)
{
    int found = 0;
    int     i = 1;

    while (!found && i < argc)
    {
        found = !strncmp (searchText, argv[i], 2);
        if (!found)
            i++;
    }
    if (found)
        optionValueIndex = i;
    else
        optionValueIndex = 0;

    return found;
}


void printUsage( )
{
    cout << endl << "Usage: " << endl;
    cout << "test_access -h" << endl;
    cout << "test_access -f<file> [-t] [-p] [-s]" << endl;
    cout << "test_access -f<file> -r " << endl;
    cout << "      -h          ... this help" << endl;
    cout << "      -p          ... for printing tiles contents" << endl;
    cout << "      -t          ... for timming the access" << endl;
    cout << "      -s          ... for printing the sizes of mddobjs" << endl;
    cout << "      -f<file>    ... for the name of the text file"  << endl;
    cout << "      -r          ... for deleting the collections listed in <file>" << endl;
    cout << "      -c          ... for printing info on compressed tiles";
    cout << endl << endl;

}











