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

#include <stdlib.h>
#include <iostream.h>
#include <vector.h>

#include "raslib/minterval.hh"
#include "raslib/sinterval.hh"
#include "raslib/rmdebug.hh"


#include <math.h>

extern char* myExecArgv0 = "";

unsigned maximumFill = 10;

#include "raslib/rminit.hh"
RMINITGLOBALS('C')


void calculateGrid( const r_Minterval& baseTile,
                    const r_Minterval& gridDesc,
                    long& numberParts,
                    vector<r_Minterval>*& grid ); //r_Minterval*& grid );


float calculateAlignFactor( const vector<r_Minterval>& partition);

int isDisjunctive( const vector<r_Minterval>& parts );

void createTilesDomains( ofstream& outRndPopFile,
                         ofstream& outRndBMFile,
                         ofstream& outPopFile,
                         ofstream& outBMFile,
                         float alignFactor,
                         vector< r_Minterval >*& tgIntsVec,
                         int numberTiles,
                         unsigned dim );
int randomInsertion = 1;
int seqInsertion = 1;

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

    if( argc < 4 )
    {
        cout << "Usage: test_ix <dim> <numberTiles> <alignFactor> [-r|-s]" << endl;
        cout << "       -s only sequential insertion " << endl;
        cout << "       -r only random insertion " << endl;
        cout << "       default - both sequential and random " << endl;
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
    {
        seqInsertion = 0;
    }
    else if ( argc == 5 && strcmp(argv[4], "-s") == 0 )
    {
        randomInsertion = 0;
    }


    cout << "Dimensionality " << dim << endl;
    cout << "Number of Tiles " << numberTiles << endl;
    cout << "Alignment factor " << af << endl;
    cout << "RandomInsertion ";

    if ( randomInsertion )
        cout << "RandomInsertion " << endl;
    if ( seqInsertion )
        cout << "SequentialInsertion " << endl;

    vector< r_Minterval >* tgIntsVec;


    char  namePopFile[100];
    char  nameRndPopFile[100];
    char  nameBMFile[100];
    char  nameRndBMFile[100];
    int afint = af*100;

    sprintf( nameRndPopFile, "ix%dd%dn%dafrndpop.txt", dim, numberTiles, afint);
    sprintf( nameRndBMFile, "ix%dd%dn%dafrnd.bm", dim, numberTiles, afint);
    sprintf( namePopFile, "ix%dd%dn%dafpop.txt", dim, numberTiles, afint);
    sprintf( nameBMFile, "ix%dd%dn%daf.bm", dim, numberTiles, afint);

    ofstream ixStreamPopResults( namePopFile );
    ofstream ixStreamRndPopResults( nameRndPopFile );
    ofstream ixStreamBMResults( nameBMFile );
    ofstream ixStreamRndBMResults( nameRndBMFile );
    if ( randomInsertion )
    {
        cout << "nameRndPopFile " << nameRndPopFile << endl;
        cout << "nameRndBMFile " << nameRndBMFile << endl;
        // system("/usr/bin/date
        if ( !ixStreamRndPopResults || !ixStreamRndBMResults )
        {
            cout <<"Error: one file could not be openened" << endl;
            return -1;
        }
    }
    if ( seqInsertion )
    {
        cout << "namePopFile " << namePopFile << endl;
        cout << "nameBMFile " << nameBMFile << endl;
        // system("/usr/bin/date
        if ( !ixStreamPopResults || !ixStreamBMResults )
        {
            cout <<"Error: one file could not be openened" << endl;
            return -1;
        }
    }


    cout << "Dim                " << dim << endl;
    cout  << "NTiles wanted      " << numberTiles << endl;
    cout << "AlignFactor wanted " << af << endl;


    createTilesDomains( ixStreamRndPopResults, ixStreamRndBMResults,
                        ixStreamPopResults, ixStreamBMResults, af, tgIntsVec,
                        numberTiles, dim );
    numberTiles = tgIntsVec->size( ); // integer arithmetic error


    ixStreamPopResults.close( );
    ixStreamBMResults.close( );
    ixStreamRndPopResults.close( );
    ixStreamRndBMResults.close( );

    return 0;
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
createTilesDomains( ofstream& outRndPopFile, // file for test_populate
                    ofstream& outRndBMFile,  // file for benchmark
                    ofstream& outPopFile, // file for test_populate
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
    const unsigned tileLength = 3;
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
    if ( seqInsertion )
    {
        outBMFile << dim << "\t";
        outBMFile << ntilesObta << "\t";
        outBMFile << faObta << "\t";
        outBMFile << "0\t";
    }
    if ( randomInsertion )
    {
        outRndBMFile << dim << "\t";
        outRndBMFile << ntilesObta << "\t";
        outRndBMFile << faObta << "\t";
        outRndBMFile <<"1\t";
    }

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
    char  collRndName[100];
    char  collName[100];
    char  collTypeName[100];
    char  mddTypeName[100];
    int faInt = faObta*100;
    if ( randomInsertion )
        sprintf( collRndName, "Ix%dD_%dN_%dAF_RND_Set", dim, ntilesObta, faInt);
    if ( seqInsertion )
        sprintf( collName, "Ix%dD_%dN_%dAF_Set", dim, ntilesObta, faInt);
    r_Minterval dom( dim );
    for ( int d = 0; d < dim; d++ )
        dom[d].set_interval((r_Range)0,'*');

    sprintf( collTypeName, "Char%dDSet", dim);
    sprintf( mddTypeName, "Char%dD", dim );

    if ( seqInsertion )
    {
        outPopFile << "Database: BmarkIxBase " << endl << endl;
        outPopFile << "MDDColl: " << collName << "; " << collTypeName << endl<< endl;
        outPopFile << "MDDObj: " << dom << " ; " << mddTypeName << endl;
        outPopFile << "HowToStore:" << endl;
        outPopFile << "IndexType: R+TreeIx " << endl << endl;
    }
    if ( randomInsertion )
    {
        outRndPopFile << "Database: BmarkIxBase " << endl << endl;
        outRndPopFile << "MDDColl: " << collRndName << "; " << collTypeName << endl<< endl;
        outRndPopFile << "MDDObj: " << dom << " ; " << mddTypeName << endl;
        outRndPopFile << "HowToStore:" << endl;
        outRndPopFile << "IndexType: R+TreeIx " << endl << endl;
    }
    if ( seqInsertion )
    {
        for( i = 0; i < tgIntsVec->size( ); i++ )
        {
            outPopFile <<"Tile :    "<< " "<< (*tgIntsVec)[i]  << "; 0x0000" << endl;
        }
    }
    if ( randomInsertion )
    {
        unsigned vecSz =  tgIntsVec->size( );
        vector< r_Minterval >* tgIntsVec2;
        tgIntsVec2 = new vector<r_Minterval>(vecSz);
        unsigned tix = i;
        for( i = 0; i < vecSz; i++ )
        {
            tix = rand( ) % tgIntsVec->size( );
            outRndPopFile <<"Tile :    "<< " "<< (*tgIntsVec)[tix]  << "; 0x0000" << endl;
            (*tgIntsVec2)[i] = (*tgIntsVec)[tix];
            tgIntsVec->erase( tgIntsVec->begin( ) + tix );
        }
        vector< r_Minterval >* tmpIntsVec;
        tmpIntsVec = tgIntsVec;
        tgIntsVec = tgIntsVec2;
        delete tmpIntsVec;
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

