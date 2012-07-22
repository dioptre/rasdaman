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
 * SOURCE: test_fastscale.cc
 *
 * MODULE: rasodmg
 *
 * COMMENTS:
 *      None
*/


#include <iostream>

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#include "raslib/template_inst.hh"
#endif

#include "rasodmg/fastscale.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/database.hh"
#include "rasodmg/marray.hh"
#include "raslib/odmgtypes.hh"



template class r_Fast_Scale<r_Char>;

template<class T>
void fast_scale_resample_array(T *dest, const T *src, const r_Minterval &destIv, const r_Minterval &srcIv, const r_Minterval &srcIter, unsigned int type_len, unsigned int length);

static void test_resample(unsigned int extent, unsigned int shrink)
{
    r_Minterval siv(2);
    r_Minterval div(2);
    unsigned int shrunken;
    r_Char *srcData;
    r_Char *destData;

    cout << "Test resampling, size " << extent
         << ", shrink factor " << shrink << endl;

    shrunken = (extent / shrink);
    if (shrunken * shrink < extent) shrunken++;

    siv << r_Sinterval((r_Range)0, (r_Range)extent-1)
        << r_Sinterval((r_Range)0, (r_Range)extent-1);

    div << r_Sinterval((r_Range)0, (r_Range)shrunken-1)
        << r_Sinterval((r_Range)0, (r_Range)shrunken-1);

    srcData = new r_Char[extent*extent];
    destData = new r_Char[shrunken*shrunken];

    unsigned int i, j;

    for (i=0; i<extent; i++)
        for (j=0; j<extent; j++)
            srcData[i*extent + j] = (r_Char)(i+j);

    cout << "go... " << flush;
    fast_scale_resample_array(destData, srcData, div, siv, siv, 1, shrink);
    cout << "OK... " << flush;

    delete [] srcData;
    delete [] destData;

    cout << "done." << endl;
}


int main(int argc, char *argv[])
{
    char servername[256] = "sunwibas0";
    char collname[256] = "nopscale";
    char database[256] = "NorbertBase";
    r_Range imgSize = 1000;
    r_Range updtSize = 100;
    int dostuff=3;

#if 0
    // test the actual resampling code...
    unsigned int shrink;
    for (shrink=2; shrink<50; shrink++)
    {
        test_resample(1000, shrink);
    }
    exit(0);
#endif

    int i;

    i = 1;
    while (i < argc)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 's':
                strcpy(servername, argv[++i]);
                break;
            case 'd':
                strcpy(database, argv[++i]);
                break;
            case 'c':
                strcpy(collname, argv[++i]);
                break;
            case 'i':
                imgSize = atoi(argv[++i]);
                break;
            case 'n':   // no create
                dostuff &= ~1;
                break;
            case 'N':   // no update
                dostuff &= ~2;
                break;
            case 'h':
                cout << argv[0] << " [-s servername -d dbname -c collname -i imgsize -n -N -h]" << endl;
                exit(0);
            default:
                cerr << "Bad switch " << argv[i] << endl;
                exit(-1);
            }
        }
        else
        {
            cerr << "unknown option " << argv[i] << endl;
            exit(-1);
        }
        i++;
    }

    r_Database db;
    r_Transaction ta;
    r_Minterval tileDom(2);
    r_Domain_Storage_Layout *myStorageLayout;
    r_Aligned_Tiling *myAlignedTiling;
    tileDom << r_Sinterval((r_Range)0, (r_Range)100)
            << r_Sinterval((r_Range)0, (r_Range)100);
    myAlignedTiling = new r_Aligned_Tiling( tileDom, tileDom.cell_count() );
    myStorageLayout = new r_Domain_Storage_Layout( tileDom, myAlignedTiling );
    r_Minterval imgDom(2);
    imgDom << r_Sinterval((r_Range)0, (r_Range)imgSize)
           << r_Sinterval((r_Range)0, (r_Range)imgSize);

    r_Fast_Scale<r_Char>* myScale;

    cout << "Server = " << servername << ", Database = " << database << ", collection = " << collname << ", Image size = " << imgSize << endl;

    try
    {
        db.set_servername(servername);
        db.open(database);
    }
    catch(r_Error &err)
    {
        cerr << err.what() << endl;
        exit(-1);
    }

    if ((dostuff & 1) != 0)
    {
        try
        {
            ta.begin();
            myScale = new r_Fast_Scale<r_Char>(collname, imgDom, "GreySet", "GreyImage", myStorageLayout);
            ta.commit();
            delete myScale;
            cout << "created OK" << endl;
        }
        catch(r_Error &err)
        {
            ta.abort();
            cerr << err.what() << endl;
        }
    }

    delete myStorageLayout;

    // read a small object for identification
    try
    {
        ta.begin(r_Transaction::read_only);
        r_Ref<r_GMarray> mddObj = r_Fast_Base_Scale::get_minimal_array(collname);
        ta.commit();
        cout << "Object read: ";
        mddObj->print_status();
        mddObj.destroy();
    }
    catch(r_Error &err)
    {
        ta.abort();
        cerr << err.what() << endl;
    }

    try
    {
        ta.begin(r_Transaction::read_only);
        myScale = new r_Fast_Scale<r_Char>(collname);
        cout << "Read domain: " << myScale->get_full_domain() << endl;
        ta.commit();
        cout << "domain OK" << endl;
    }
    catch(r_Error &err)
    {
        ta.abort();
        cerr << err.what() << endl;
    }

    if ((dostuff & 2) != 0)
    {
        // update a diagonal strip
        for (i=0; i<=imgSize-updtSize; i+=updtSize/2)
        {
            try
            {
                r_Minterval uiv(2);

                uiv << r_Sinterval((r_Range)i, (r_Range)i+updtSize-1)
                    << r_Sinterval((r_Range)i, (r_Range)i+updtSize-1);

                cout << "Update " << uiv << endl;
                r_Marray<r_Char> *updtArray = new ("GreyImage") r_Marray<r_Char>(uiv);
                memset(updtArray->get_array(), 255, updtSize * updtSize);
                cout << "Start insert... " << flush;
                ta.begin();
                myScale->insert_array(*updtArray);
                ta.commit();
                delete updtArray;
                cout << "OK" << endl;
            }
            catch (r_Error &err)
            {
                ta.abort();
                cerr << err.what();
            }
        }
    }

    double scaleFactor;

    for (scaleFactor=1.0; scaleFactor>1e-6; scaleFactor-=0.05)
    {
        try
        {
            r_Minterval scaledIv;

            if (myScale->get_scaled_domain(myScale->get_full_domain(), scaledIv, scaleFactor) != 0)
            {
                cout << "Read " << scaledIv << ", scaled " << scaleFactor << "..." << flush;
                ta.begin(r_Transaction::read_only);
                r_Ref<r_Marray<r_Char> > result;
                result = myScale->get_scaled_image(scaledIv, scaleFactor);
                ta.commit();
                cout << "OK" << endl;
                result.destroy();
            }
        }
        catch(r_Error &err)
        {
            ta.abort();
            cerr << err.what() << endl;
        }
    }

    delete myScale;

    db.close();

    return 0;
}
