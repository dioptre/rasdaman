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
 * SOURCE: test_ix.cc
 *
 * MODULE: test for RasDaMan tiles indexes
 *
 * PURPOSE:
 *   Compares RPTreeIx's to DirIx's
 *
 * COMMENTS:
 *   none
 *
*/

#include "mymalloc/mymalloc.h"

#include <stdlib.h>
#include <iostream.h>
#include <vector.h>


#include "o2lib_CC.hxx"       // declaration of O2-collection-classes
#include <o2.h>               //   O2 Engine
#include <o2_error.h>         //   O2 error from O2 Engine

#include "dbmddobjix.hh"
#include "basetype.hh"
#include "chartype.hh"
#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"
#include "cachetamgr/perstile.hh"
#include "tools/timer.hh"
#include "indexmgr/persdirix.hh"
#include "indexmgr/pershierix.hh"
#include "indexmgr/rptreeix.hh"
#include "indexmgr/dirix.hh"
#include "raslib/rmdebug.hh"


#include <math.h>

// This test program must use a different base because it
// doesn't use catalogif and adminif. It is not a complete RasDaBase.
static char O2BenchDBName[] = "TestIxBase";
static char O2BenchSchemaName[] = "TestSMSchema";

extern char* myExecArgv0 = "";

unsigned maximumFill = 10;

#include "raslib/rminit.hh"
RMINITGLOBALS('C')

static void ClearDB( d_Database &DB );

// indexType 1 : R+ tree, 2: DirIx
static void testPopulateIx( int indexType,
                            const vector< r_Minterval >& tilesDoms );

// static void createTilesArr( Tile** tiles, float alignFactor, int numberTiles );


void testCompareIxs(  );

void calculateGrid( const r_Minterval& baseTile,
                    const r_Minterval& gridDesc,
                    long& numberParts,
                    vector<r_Minterval>*& grid ); //r_Minterval*& grid );

static void testCompareIxs( char* index,             char* index1,
                            MultiDimIx<Tile>* ix,    MultiDimIx<Tile>* ix1,
                            r_Minterval* searchInts, int numInts);

float calculateAlignFactor( const vector<r_Minterval>& partition);

int isDisjunctive( const vector<r_Minterval>& parts );

void createTilesDomains( ofstream& outPopFile,
                         ofstream& outBMFile,
                         float alignFactor,
                         vector< r_Minterval >*& tgIntsVec,
                         int numberTiles,
                         unsigned dim );
int randomInsertion;

/*************************************************************
 * Function name.: int main( int argc, char** argv)
 *
 * Arguments.....:
 *   argc: number of arguments given to program
 *   argv: array of char* with arguments
 * Return value..: exit status
 * Description...: none
 ************************************************************/
int
main( int argc, char** argv)
{

    // variables representing O2 database, ta and session
    d_Session  session;
    d_Database database;
    d_Transaction ta;

    if( argc < 4 )
    {
        cout << "Usage: test_ix <dim> <numberTiles> <alignFactor> [-r]" << endl;
        return -1;
    }

    unsigned dim;
    unsigned numberTiles;
    float af;
    dim = atoi( argv[1]);
    numberTiles = atoi( argv[2]);
    af = atof( argv[3] );
    if ( dim < 2 || dim > 8  )
    {
        cout << " Error : dimensionality outside supported limits !" << endl;
        return -1;
    }
    if ( numberTiles > 100000 || numberTiles < 50 )
    {
        cout << " Error : number of tiles outside supported limits !" << endl;
        return -1;
    }
    if ( af < 0.05 || af  > 1 )
    {
        cout << " Error : alignment fator outside supported limits !" << endl;
        return -1;
    }

    if ( argc == 5 && strcmp(argv[4], "-r") == 0 )
        randomInsertion = 1;
    else
        randomInsertion = 0;


    cout << "Dimensionality " << dim << endl;
    cout << "Number of Tiles " << numberTiles << endl;
    cout << "Alignment factor " << af << endl;
    cout << "RandomInsertion ";
    if ( randomInsertion )
        cout << " yes " << endl;
    else
        cout << " no " << endl;

    /*
    // initialize the O2 session
    cout << "Initializing O2 session..." << endl;
    session.set_default_env();
    if (session.begin(argc, argv)){
      cerr << "Something wrong to start o2" << endl;
      exit(1);
    }

    // connect to the database
    cout << "Connecting to database " << O2BenchDBName
         << "..." << endl;
    try{
      database.open( O2BenchDBName ); // doesn't work with O2 V.5
    }
    catch( ...)
    {
      cout << "Could not open database. Exiting "<< endl;
      session.end();
      return -1;
    }


    // create root collection
    cout << "Checking root collection..." << endl;
    ta.begin();
    Handle collHandle = 0;
    collHandle = o2_get_root((char*)"HierIndexContainer");

    if (!collHandle)
    {
      cout << "Collection doesn't exist yet,  creating collection ... "<< endl;
      // if root name isn't used yet, create new root name for MDD collection
          // I don't know if this is needed. It works without it.
      o2_unref_handle( collHandle );
      database.create_persistent_root( "HierIndexContainer",
                                       "d_List<d_Ref<DBMDDObjIx>>",
                           OL_CONSTANT);
    }
    else
    {
      cout << "Collection exists already, does nothing. "<< endl;
      // if it exists already, doesn't do anything
          // I don't know if this is needed. It works without it.
      o2_unref_handle(collHandle);
     }
     */
    /*
    int exists = 1;
    try{
     d_List< DBMDDObjIxId > indexList("HierIndexContainer");
    }
    catch( ...)
    {
     cout << "Persistent root does't exist" << endl;
     exists =0;
    }
    if ( !exists )
     database.create_persistent_root( "HierIndexContainer",
                                      "d_List<d_Ref<DBMDDObjIx>>",
                              OL_CONSTANT);
     */
    // ta.commit();


    vector< r_Minterval >* tgIntsVec;


    char  namePopFile[100];
    char  nameBMFile[100];
    int afint = af*100;
    if ( randomInsertion )
    {
        sprintf( namePopFile, "ix%dd%dn%dafrndpop.txt", dim, numberTiles, afint);
        sprintf( nameBMFile, "ix%dd%dn%dafrnd.bm", dim, numberTiles, afint);
    }
    else
    {
        sprintf( namePopFile, "ix%dd%dn%dafpop.txt", dim, numberTiles, afint);
        sprintf( nameBMFile, "ix%dd%dn%daf.bm", dim, numberTiles, afint);
    }
    cout << "namePopFile " << namePopFile << endl;
    cout << "nameBMFile " << nameBMFile << endl;

    ofstream ixStreamPopResults(namePopFile);
    ofstream ixStreamBMResults(nameBMFile);

    // system("/usr/bin/date
    if ( !ixStreamPopResults || !ixStreamBMResults )
    {
        cout <<"Error: one file could not be openened" << endl;
        return -1;
    }

    cout << "Dim                " << dim << endl;
    cout  << "NTiles wanted      " << numberTiles << endl;
    cout << "AlignFactor wanted " << af << endl;


    // numberTiles = 80 * i;
    createTilesDomains( ixStreamPopResults, ixStreamBMResults, af, tgIntsVec, numberTiles, dim );
    numberTiles = tgIntsVec->size( ); // integer arithmetic error

    /*
    cout << "Populating Index 1..." << endl;
    ta.begin( );
    testPopulateIx( 1, *tgIntsVec );
    ta.commit( );

    cout << "Populating Index 2..." << endl;
    ta.begin( );
    testPopulateIx( 2, *tgIntsVec );
    ta.commit( );

    delete tgIntsVec;

    cout <<"Testing two indexes ..."<< endl;
    ta.begin( );
    testCompareIxs( );
    ta.commit( );

    cout << endl;
    cout << "Ending O2 session..." << endl;
    database.close();
    session.end();
    */

    ixStreamPopResults.close( );
    ixStreamBMResults.close( );

    return 0;
}

/*************************************************************************
 *
 *
 ************************************************************************/
void
testPopulateIx( int indexType, const vector< r_Minterval >& tilesDomains )
{
    CharType anyType;
    char* anyCells;
    const int numberTiles = tilesDomains.size( );
    const int dim = tilesDomains[1].dimension( );


    cout << "....testPopulateIx"<< endl;
    d_List< DBMDDObjIxId > indexList("HierIndexContainer");

    PersDirIx* ix = new PersDirIx( dim, &anyType );

    MultiDimIx< Tile >* rix;

    cout << "Ix of type ";
    switch( indexType )
    {
    case 1:
        cout << "RPlusTree ";
        rix = new RPlusTreeIx<Tile>( ix, maximumFill );
        break;
    case 2:
        cout << "DirIx ";
        rix = new DirIx< PersDirIx, Tile >( ix );
        break;
        // to be extended
    default:
        cout <<"Error "<<endl;
        exit( 2 );
    }

    cout << "just created: "<< endl;
    rix->printStatus( );

    Tile* tiles[100000];

    for(  int i = 0; i < tilesDomains.size( ); i++ )
    {
        anyCells = (char*)mymalloc(tilesDomains[i].cell_count() * anyType.getSize());
        tiles[i] =
            new PersTile( tilesDomains[i], (const BaseType*) &anyType, anyCells);
    }


    cout << endl;
    for( i = 0; i < numberTiles ; i++ )
    {
        cout << endl << "Insert new tile " << i << " " << tiles[i]->getDomain( )<< endl;

        rix->insertObject( tiles[i] );

        // rix->printStatus( );
        // cout << endl;
    }
    cout << endl << "Finished, index contents: " << endl;
    rix->printStatus( );
    cout << endl;
    indexList.insert_element_last( ( (PersIx*) (rix->getIxDS( )) )->getDBMDDObjIxId( ) );
    for( i = 0; i < numberTiles ; i++ )
        delete tiles[i];
    delete rix;
}

/*************************************************************************
 *
 *
 ************************************************************************/
void
calculateGrid( const r_Minterval& baseTile,
               const r_Minterval& gridDesc,
               long& numberParts,
               vector< r_Minterval>*& grid ) // grid r_Minterval*& grid)
{
    cout << "calculateGrid( " << baseTile << ", "<< gridDesc << ") "<< endl;
    numberParts = gridDesc.cell_count( );
    grid = new vector<r_Minterval>(numberParts); //new r_Minterval[numberParts];
    r_Point ix( baseTile.dimension( ));
    r_Point ext = baseTile.get_extent( );
    for ( int i= 0; i < numberParts ; i++ )
    {
        ix = gridDesc.cell_point( i )* ext;
        (*grid)[i] = baseTile.create_translation( ix );
        // grid[i] = r_Minterval( baseTile.create_translation( ix ) );
    }

}

/*************************************************************************
 *
 *
 ************************************************************************/
void
createTilesDomains( ofstream& outPopFile, // file for test_populate
                    ofstream& outBMFile,  // file for benchmark
                    float alignFactor,
                    vector< r_Minterval >*& tgIntsVec,
                    int numberTiles,
                    unsigned dim )
{

    if( alignFactor > 1 || alignFactor == 0 )
    {
        cout <<"Error: invalid alignment factor." << endl;
        exit(0);
    }

    cout << "Alignment factor wanted "  << alignFactor << endl;
    cout << "Number of tiles, dim " << numberTiles << ", "<< dim << endl;

    // number of partitions of the total grid
    double ntgdouble = numberTiles/alignFactor;
    // cout <<"Number of tiles in total grid (double)== " << ntgdouble << endl;
    long ntg =  ntgdouble;
    cout <<"Number of tiles in total grid (long)== " << ntg << endl;

    // r_Minterval* tgInts;

    float f = float (1/ float(dim) );
    long n = pow( ntg, f);
    cout <<"Number of tiles in d-1 first directions (n) == "  << n << endl;
    r_Minterval tile(dim);
    const unsigned tileLength = 5;
    r_Minterval totalGrid( dim );
    for (int i = 0; i <  dim -1; i++ )
    {
        tile[i].set_interval( r_Range( 0 ), tileLength-1 );
        totalGrid[i].set_interval( r_Range( 0 ), n-1 );
    }
    long n1 = pow(n,dim-1);
    n1 = ntg/n1;
    cout << "Number of tiles in direction d (n1)  == " << n1 << endl;
    tile[dim-1].set_interval( r_Range( 0 ), tileLength-1 );
    totalGrid[dim-1].set_interval( r_Range( 0 ), n1-1 );

    long ntg1;
    calculateGrid( tile, totalGrid, ntg1, tgIntsVec );

    if ( ntg1 < numberTiles )
    {
        cout <<"Error: number of partitions in total grid less than numberTiles";
        cout <<"Exiting the program (integer arithmetic)"<< endl;
        // exit(0);
    }

    if ( ntg1 != ntg )
    {
        cout << "Warning: number of partitions in total grid is not as expected " << endl;
        cout << "Alignment factor won't be as expected (integer arithmetic)"<< endl;
    }

    cout << endl;

    //tgIntsVec = new vector< r_Minterval >( tgInts, tgInts+ntg1 );

    // delete tgInts;  // ????

    cout << "Number of tiles in total grid " << ntg1 << endl;
    /*
    for( i =0; i < tgIntsVec->size( ); i++ )
       cout << "Interval "<< i << (*tgIntsVec)[i] <<endl;
    */
    int numberMerges = ntg1-numberTiles;
    int cont = 1;
    for ( i = 0; i < numberMerges && cont ; )
    {
        int found = 0;
        int r = rand( )  % (tgIntsVec->size( ) );
        // merge randomly
        int notFinished = 1;

        int contr = 1;
        for( int ri = 0; contr  && cont ; ri++)
        {
            notFinished = 1;
            int init = rand( ) % tgIntsVec->size( );
            for( int j = init; notFinished ; )
            {
                // cout <<"i,j"<< i <<","<< j<<endl;
                if( (*tgIntsVec)[j].is_mergeable( (*tgIntsVec)[r]) )
                {

                    if ( i % 100 == 0 )
                        cout << numberMerges-i << ". Merging j,r "<< j <<","<< r << " "
                             << (*tgIntsVec)[j] << ", " << (*tgIntsVec)[r] << " resulting ";
                    /*
                    cout << numberMerges-i << ". Merging j,r "<< j <<","<< r << " "
                       << (*tgIntsVec)[j] << ", " << (*tgIntsVec)[r] << " resulting ";
                    */

                    (*tgIntsVec)[j] = (*tgIntsVec)[j].closure_with( (*tgIntsVec)[r] );
                    tgIntsVec->erase( tgIntsVec->begin( ) + r );

                    if ( i % 100 == 0 )
                    {
                        if ( r < j )
                            cout << (*tgIntsVec)[j-1] <<endl;
                        else
                            cout << (*tgIntsVec)[j] <<endl;
                    }

                    i++;
                    notFinished = 0;
                    found = 1;
                }
                j = (j+1) %  tgIntsVec->size( );
                if ( j == init )
                    notFinished = 0;
            }
            // cout << " r == " << r << ", j == " << j << " , notFinished == " << notFinished << endl;
            //  cout << " ri == " << ri << " , tgIntsVec->size( ) " << tgIntsVec->size( ) << endl;
            r = (r+1)  % tgIntsVec->size( );
            if ( found )
                contr = 0;    // already found
            else
            {
                if ( ri >= tgIntsVec->size( ) )
                    cont = 0; // no more merges possible
                else
                    r = (r+1)  % tgIntsVec->size( );
            }
        }
    }

    cout << "======================================================="<< endl;
    cout << "Results : "<< endl;
    cout << "  Alignement Factor wanted :" << alignFactor << endl;
    cout << "  Number of Resulting Tiles " << tgIntsVec->size( ) << endl;

    cout << "  Resulting Tiles " << endl;

    unsigned ntilesObta = tgIntsVec->size( );
    cout << "NTiles obtained    " << ntilesObta << endl;

    float faObta = float ( tgIntsVec->size( ))/ntg1;
    cout <<"  Alignment factor obtained "<< faObta << endl;

    /*
     * Benchmark file format:
     *     Dim \t ntiles \t af \t random \t h \t occ
     */
    outBMFile << dim << "\t";
    outBMFile << ntilesObta << "\t";
    outBMFile << faObta << "\t";
    if ( randomInsertion )
        outBMFile <<"1\t";
    else
        outBMFile << "0\t";

    // Visual_Tiling_2D visTil( dom, "TilesImage");

    /*
     * Pop file format:
     *   Database: BmarkIxBase
     *
     *   MDDColl: Ix2D_1000N_5AF_Set; Char2DSet
     *
     *   MDDObj: [  0:*,  0:*] ; Char2D
     *
     *   HowToStore:
     *   IndexType: R+TreeIx
     */
    char  collName[100];
    char  collTypeName[100];
    char  mddTypeName[100];
    int faInt = faObta*100;
    if ( randomInsertion )
        sprintf( collName, "Ix%dD_%dN_%dAF_RND_Set", dim, ntilesObta, faInt);
    else
        sprintf( collName, "Ix%dD_%dN_%dAF_Set", dim, ntilesObta, faInt);
    r_Minterval dom( dim );
    for ( int d = 0; d < dim; d++ )
        dom[d].set_interval((r_Range)0,'*');

    sprintf( collTypeName, "Char%dDSet", dim);
    sprintf( mddTypeName, "Char%dD", dim );
    outPopFile << "Database: BmarkIxBase " << endl << endl;
    outPopFile << "MDDColl: " << collName << "; " << collTypeName << endl<< endl;
    outPopFile << "MDDObj: " << dom << " ; " << mddTypeName << endl;
    outPopFile << "HowToStore:" << endl;
    outPopFile << "IndexType: R+TreeIx " << endl << endl;
    if ( randomInsertion )
    {
        unsigned vecSz =  tgIntsVec->size( );
        vector< r_Minterval >* tgIntsVec2;
        tgIntsVec2 = new vector<r_Minterval>(vecSz);
        unsigned tix = i;
        for( i = 0; i < vecSz; i++ )
        {
            tix = rand( ) % tgIntsVec->size( );
            outPopFile <<"Tile :    "<< " "<< (*tgIntsVec)[tix]  << "; 0x0000" << endl;
            (*tgIntsVec2)[i] = (*tgIntsVec)[tix];
            tgIntsVec->erase( tgIntsVec->begin( ) + tix );
        }
        vector< r_Minterval >* tmpIntsVec;
        tmpIntsVec = tgIntsVec;
        tgIntsVec = tgIntsVec2;
        delete tmpIntsVec;
    }
    else
    {
        for( i = 0; i < tgIntsVec->size( ); i++ )
        {
            outPopFile <<"Tile :    "<< " "<< (*tgIntsVec)[i]  << "; 0x0000" << endl;
        }
    }

    /* For debugging purposes:
    if( !isDisjunctive( *tgIntsVec ))
    {
       cout <<"Error: Nondisjunctive partition, exiting program "<< endl;
       exit( 0);
    }
    else cout <<"  Disjunctive partition OK " << endl;
    */

    if( tgIntsVec->size( ) != numberTiles )
    {
        cout <<"Error: tgIntsVec->size( ) != numberTiles "<< endl;
        // exit( 0);
    }
    // calculateAlignFactor( tgIntsVec );

}

/*************************************************************************
 *
 *
 ************************************************************************/
/*
void
createTilesArr( Tile** tiles,
            float alignFactor,
        vector< r_Minterval >* tgIntsVec )
{

  ULongType anyType;
  char* anyCells;

  for(  i = 0; i < tgIntsVec->size( ); i++ )
  {
    anyCells = (char*)malloc(tilesDomains[i].cell_count() * anyType.getSize());
    tiles[i] =
      new PersTile( (*tgIntsVec)[i], (const BaseType*) &anyType, anyCells);
  }

}
*/

/*************************************************************************
 *
 *
 ************************************************************************/
float
calculateAlignFactor( const vector<r_Minterval>& part )
{
    float fa;
    // for ( int i = 0; i < part.size( ); i++ ) ;

    return fa;
}

/*************************************************************************
 *
 *
 ************************************************************************/
int isDisjunctive( const vector<r_Minterval>& parts )
{
    for( int i = 0; i < parts.size( ); i++ )
    {
        r_Minterval inter = parts[i];
        for( int j = i+1; j < parts.size( ); j++ )
        {
            if( inter.intersects_with( parts[j] ))
                return 0;
        }
    }
    return 1;
}

/*************************************************************
 * Function name.:
 *   void
 *   testCompareIxs( char* index, char* index1, )
 *
 * Arguments.....:
 *   index : name of first index
 *   index1: name of second index
 *
 ************************************************************/
void
testCompareIxs(  )
{
    DBMDDObjIxId accessedIndex;
    CharType anyType;
    // char anyCell[4];
    cout << "....testCompareIxs"<<endl;

    // read root object
    d_List< DBMDDObjIxId > indexList("HierIndexContainer");
    // used for iterating
    d_Iterator< DBMDDObjIxId > indexIt = indexList.create_iterator();

    MultiDimIx< Tile >* ix = 0;
    MultiDimIx< Tile >* ix1 = 0;
    PersIx* dix;
    PersIx* dix1;

    for( int i = 1 ; indexIt.not_done(); i++, indexIt.advance())
    {
        accessedIndex = indexIt.get_element();
        cout << "    --"<<i<<". index object in list:" << endl;
        accessedIndex->printStatus();
        if (accessedIndex->isRoot( ) && accessedIndex->isLeaf( )  )
        {
            // to avoid mem leaks in repeated executions of this test program
            if ( ix1 ) delete ix1;
            cout << endl << "Creating DirIx ... ";
            dix1 = new PersDirIx( accessedIndex, (const BaseType*) &anyType );
            ix1 = new DirIx< PersDirIx, Tile >( (PersDirIx*)dix1 );
        }
        else
        {
            // to avoid mem leaks in repeated executions of this test program
            if ( ix ) delete ix;
            cout << endl << "Creating R+-tree ... ";
            dix = new PersHierIx( accessedIndex, (const BaseType*) &anyType );
            ix = new RPlusTreeIx<Tile>( dix,maximumFill );
        }
        cout << endl;
    }
    cout <<"OK"<<endl;

    const int numInts = 30;
    r_Minterval searchInts[numInts];

    for( i = 0; i < numInts ; i++ )
    {
        r_Range l1= rand( ) % 25;
        r_Range l2 = rand( ) % 30;
        searchInts[i] = r_Minterval( 2 );
        searchInts[i] << r_Sinterval( l1, l1+rand() % 50 );
        searchInts[i] << r_Sinterval( l2, l2 +rand( ) % 50 );

        /*
          searchInts[0] = r_Minterval( "[0:100,0:100]");
          searchInts[1] = r_Minterval( "[150:200,100:250]");
          searchInts[2] = r_Minterval( "[100:300,250:400]");
          searchInts[1] = r_Minterval( "[50:100,100:250]");
          searchInts[1] = r_Minterval( "[50:73,90:94]");
        */
    }

    testCompareIxs( "R+-tree ", "DirIx   ", ix,  ix1, searchInts, numInts);

    delete ix;
    delete ix1;
}

/*************************************************************************
 *
 *
 ************************************************************************/
void
testCompareIxs( char* index,             char* index1,
                MultiDimIx<Tile>* ix,    MultiDimIx<Tile>* ix1,
                r_Minterval* searchInts, int numInts)
{
    cout << "....testCompareIxs"<< endl;

    //ULongType anyType;
    // char anyCell[4];


    cout << "Comparison of      Index  " << index << endl;
    cout << "with               Index1 " << index1 << endl;

    for( int i = 0; i < numInts; i++ )
    {
        Timer time;
        Timer time1;
        RMTimer* rtime = new RMTimer( index, "intersect" );
        RMTimer* rtime1 = new RMTimer( index1, "intersect" );

        cout << "Intersect with "<<  searchInts[i] << endl;

        // Index
        time.start( );
        rtime->start( );
        vector< Tile* >* rqResult = ix->intersect( searchInts[i] );
        int num, num1;
        rtime->stop( );
        time.stop( );
        if ( rqResult )
        {
            num = rqResult->size( );
            cout << index << endl << " No. of tiles, time , time/noTiles = "
                 << rqResult->size( ) << " , "<< time << " , ";
            if ( rqResult->size( ) )  cout << time.ellapsed_sec( )/rqResult->size( ) << endl;
        }
        else
        {
            cout << "No tiles intersected "<< endl;
            num = 0;
        }
        delete rtime;

        // Index 1
        time1.start( );
        rtime1->start( );
        vector< Tile* >* rqResult1 = ix1->intersect( searchInts[i] );
        rtime1->stop( );
        time1.stop( );
        if ( rqResult1 )
        {
            num1 = rqResult1->size( );
            cout << index1 << endl << " No. of tiles, time1, time1/noTiles = "
                 << rqResult1->size( ) << " , "<< time1 << " , ";
            if ( rqResult1->size( ) )  cout << time1.ellapsed_sec( )/rqResult1->size( ) << endl;
        }
        else
        {
            cout << "No tiles intersected "<< endl;
            num1 = 0;
        }
        delete rtime1;
        if ( num != num1 )
            cout << "Error !!!!" << endl;

        // Index
        cout << "Result " << index << endl;
        if ( rqResult )
        {
            for( int j = 0; j < rqResult->size( ); j++)
            {
                cout << ( *rqResult )[j]->getDomain( ) << endl;
                delete (*rqResult)[j];
            }
            delete rqResult;
        }

        // Index1
        cout << "Result " << index1 << endl;
        if ( rqResult1 )
        {
            for( int j = 0; j < rqResult1->size( ); j++)
            {
                cout << ( *rqResult1 )[j]->getDomain( ) << endl;
                delete (*rqResult1)[j];
            }
            delete rqResult1;
        }
        cout << endl;
    }

}
