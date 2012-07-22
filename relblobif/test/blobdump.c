#include "mymalloc/mymalloc.h"
/* Processed by ecpg (3.1.1) */
/* These include files are added by the preprocessor */
#include <ecpgtype.h>
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
#line 1 "blobdump.pgc"
/* End of automatic include section */
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
 *
 *
 * PURPOSE:
 * dump RAS_TILES blob table.
 * Depending on parameters, either a particular tuple is printed
 * or the whole table. Optionally also the blob is dumped in hex.
 * Parameters: see usage().
 *
 *
 * COMMENTS:
 * - assumes RAS_TILES
 *
 ************************************************************/

using namespace std;

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>             /* getopt */

#include <sqlca.h>

// SQL error codes:
#include "externs.h"

// PG stuff
#include "libpq-fe.h"   // C interface to PgSQL
#include "libpq/libpq-fs.h"     /* large object (lo) api */
// libq-style connection ptr taken from ECPG connect (needed for libq functions):
// (currently used by relblobif/blobtile.pgc)
PGconn *pgConn = NULL;
PGresult *pgResult = NULL;

#define RC_OK 0
#define RC_ERROR -1

#define QUERYSIZE 2000
static char pgQuery[QUERYSIZE];

void exitNicely()
{
    PQclear( pgResult );
    (void) PQexec( pgConn, "ROLLBACK WORK" );
    PQfinish( pgConn );
    exit( RC_ERROR );
}


void
disconnect()
{
    pgResult = PQexec( pgConn, "ROLLBACK WORK" );
    if (PQresultStatus(pgResult) != PGRES_COMMAND_OK)
    {
        cerr << "error during disconnect: " << PQerrorMessage(pgConn) << endl << flush;
        PQclear( pgResult );
        exit( RC_ERROR );
    }
    PQclear( pgResult );

    PQfinish( pgConn );
    pgConn = NULL;
}

void
connect( const char * db )
{
    char pgConnInfo[200];

    (void) snprintf( pgConnInfo, (size_t) sizeof(pgConnInfo), "dbname = %s", db );
    pgConn = PQconnectdb( pgConnInfo );
    if (PQstatus(pgConn) != CONNECTION_OK)
    {
        cerr << "error during connect: " << PQerrorMessage(pgConn) << endl << flush;
        exitNicely();
    }

    pgResult = PQexec( pgConn, "BEGIN TRANSACTION" );
    if (PQresultStatus(pgResult) != PGRES_COMMAND_OK)
    {
        cerr << "error during being ta: " << PQerrorMessage(pgConn) << endl << flush;
        PQclear( pgResult );
        exit( RC_ERROR );
    }
    PQclear( pgResult );

}

// read one tuple from ras_tiles, identified by myOId, and display it (with contents if withDump==true)
void
readFromDb( long myOId, bool withDump )
{
    unsigned int blobOid = 0;
    short dataFormat = 0;
    unsigned long size = 0;
    char *cells = NULL;

    // (1) --- access tuple
    int result = snprintf( pgQuery, (size_t) sizeof(pgQuery), "SELECT Tile, DataFormat FROM RAS_TILES WHERE BlobId = %d", myOId );

    pgResult = PQexec( pgConn, pgQuery );
    if (PQresultStatus(pgResult) != PGRES_TUPLES_OK)
    {
        cerr << "error during 'select': " << PQerrorMessage(pgConn) << endl << flush;
        exitNicely();
    }
    else if (PQntuples(pgResult) < 1)
    {
        cerr << "no tuple with id " << myOId << endl << flush;
        exitNicely();
    }

    blobOid    = atoi( PQgetvalue( pgResult, 0, 0 ) );
    dataFormat = atoi( PQgetvalue( pgResult, 0, 1 ) );
    PQclear( pgResult );

    // (2) --- open, read, close blob
    int fd = lo_open( pgConn, blobOid, INV_READ );      // open; manual tells no error indication
    size = lo_lseek( pgConn, fd, 0, SEEK_END );     // determine blob size; FIXME: more efficient method??
    (void) lo_lseek( pgConn, fd, 0, SEEK_SET );     // rewind for reading
    cells = (char*) mymalloc( size * sizeof(char) );    // allocate buffer for blob
    if (cells == NULL)
    {
        cerr << "error during malloc()" << endl << flush;
        exitNicely();
    }
    int loResult = lo_read( pgConn, fd, cells, size );
    if (loResult < 0)
    {
        cerr << "error during lo_read(): " << PQerrorMessage(pgConn) << endl << flush;
        exitNicely();
    }
    else if (loResult != size)              // did not get all
    {
        cerr << "error (did not get all bytes): " << PQerrorMessage(pgConn) << endl << flush;
        exitNicely();
    }
    int ignoredPgResult = lo_close( pgConn, fd );       // close blob

    if (withDump)
    {
        cout << "   id = " << myOId << ", dataFormat = " << dataFormat << ", bloboid = " << blobOid << ", blob length = " << size << ", contents = ";
        for (int a = 0; a < size; a++)
            cout << " " << hex << (0xff & (unsigned int)(cells[a])) << dec;
        cout << dec << endl;
    }
    else
    {
        cout << "   id = " << myOId << ", dataFormat = " << dataFormat << ", bloboid = " << blobOid << ", blob length = " << size << endl;
    }

    free( cells );
}

// read all tuples from ras_tiles and display them (with contents if withDump==true)
void
readAllFromDb( bool withDump )
{
    unsigned int blobOid;
    long myId;
    short dataFormat;
    unsigned long size = 0;
    char *cells = NULL;
    int rowNumber = 0;

    // (1) --- declare cursor
    pgResult = PQexec( pgConn, "SELECT BlobId, Tile, DataFormat FROM RAS_TILES" );
    if (PQresultStatus(pgResult) != PGRES_TUPLES_OK && PQresultStatus(pgResult) != PGRES_COMMAND_OK)
    {
        cerr << "error during 'declare cursor': " << PQerrorMessage(pgConn) << endl << flush;
        exitNicely();
    }

    do
    {
        myId       = atoi( PQgetvalue( pgResult, rowNumber, 0 ) );
        blobOid    = atoi( PQgetvalue( pgResult, rowNumber, 1 ) );
        dataFormat = atoi( PQgetvalue( pgResult, rowNumber, 2 ) );

        // (2) --- open, read, close blob
        int fd = lo_open( pgConn, blobOid, INV_READ );      // open; manual tells no error indication
        size = lo_lseek( pgConn, fd, 0, SEEK_END );     // determine blob size; FIXME: more efficient method??
        (void) lo_lseek( pgConn, fd, 0, SEEK_SET );     // rewind for reading
        cells = (char*) mymalloc( size * sizeof(char) );        // allocate buffer for blob
        if (cells == NULL)
        {
            cerr << "error: readFromDb() - no tuples found " << endl;
            return;
        }
        int loResult = lo_read( pgConn, fd, cells, size );
        if (loResult < 0)
        {
            cerr << "error: readFromDb() - no tuples found " << endl;
            return;
        }
        else if (loResult != size)              // did not get all
        {
            cerr << "error: readFromDb() - no tuples found " << endl;
            return;
        }
        int ignoredPgResult = lo_close( pgConn, fd );       // close blob

        if (withDump)
        {
            cout << "   id = " << myId << ", dataFormat = " << dataFormat << ", bloboid = " << blobOid << ", blob length = " << size << ", contents = ";
            for (int a = 0; a < size; a++)
                cout << " " << hex << (0xff & (unsigned int)(cells[a])) << dec;
            cout << endl;
        }
        else
        {
            cout << "   id = " << myId << ", dataFormat = " << dataFormat << ", bloboid = " << blobOid << ", blob length = " << size << endl;
        }

        rowNumber++;
    }
    while (PQresultStatus(pgResult) == PGRES_TUPLES_OK || PQresultStatus(pgResult) == PGRES_COMMAND_OK);

    (void) PQexec( pgConn, "CLOSE TileLoop" );
    PQclear( pgResult );
    free( cells );
}

void usage(const char *prog)
{
    cout << "Usage: " << prog << " -d db [-i id] [-c]" << endl;
    cout << "where:" << endl;
    cout << "   -d db    log into PG server using PG database name db (mandatory)" << endl;
    cout << "   -i id    dump blob with identifier id (default: dump whole table)" << endl;
    cout << "   -c       dump blob contents too (default: descriptor data only)" << endl;
    cout << "   -h       (help) print this overview" << endl;
}

int
main(int argc, char* argv[])
{
    char *prog = argv[0];   // prog name
    char db[100];       // DB connect string
    int blobOid = 0;    // tuple to be dumped
    bool withDump = false;  // by default, don't dump blob contents
    char c;         // getop var
    int result;     // for sscanf()

    cout << prog << ": dump rasdaman blob table." << endl;

    opterr = 0;
    while ((c = getopt(argc, argv, "d:i:c")) != -1)
    {
        switch (c)
        {
        case 'd':
            (void) strncpy( db, optarg, sizeof(db) );
            break;
        case 'i':
            result = sscanf( optarg, "%d", &blobOid );
            if (result != 1)
            {
                cerr << prog << ": positive integer expected: " << optarg << endl;
                exit( RC_ERROR );
            }
            break;
        case 'c':
            withDump = true;
            break;
        case 'h':
        case '?':
        case ':':
            usage(prog);
            exit( RC_OK );
            break;
        }
    }
    if (optind < argc || argc == 1)
    {
        usage( prog );
        exit( RC_ERROR );
    }

    connect( db );

    if (blobOid > 0)
        readFromDb( blobOid, withDump );
    else
        readAllFromDb( withDump );

    disconnect();

    cout << prog << ": done." << endl;
}

