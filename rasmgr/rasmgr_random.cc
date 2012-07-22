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
 * SOURCE: rasmgr_master.hh
 *
 * MODULE: rasmgr
 * CLASS:  RandomGenerator
 *
 * PURPOSE:
 *   Own "random number generator", used to scramble the authorization file
 *
 * COMMENTS:
 *          None
 *
*/
#include "rasmgr_users.hh"
#include "raslib/rminit.hh"

RandomGenerator::RandomGenerator()
{
    // if somebody want's seed=1 inside it's seed =0, due to table generation way
    seed=0;
    fileVersion=-1;
}

bool RandomGenerator::setFileVersion(long version)
{
    static bool firstCall=true;
    fileVersion = version;
    if(version == 2 ) return true; // the actual version

    if(version == 1 )
    {
        if(firstCall) RMInit::logOut<<"Authorization file will be migrated to the new version"<<std::endl;
        firstCall=false;
        return true;
    }
    return false;
}

void
RandomGenerator::init(unsigned int newSeed)
{
    switch(fileVersion)
    {
    case 1:
        srand(newSeed);
        break;

    case 2:
        seed = (newSeed-1)%1000;
        break;
    }
}

unsigned char
RandomGenerator::operator()()
{
    unsigned char rasp = 0;
    switch(fileVersion)
    {
    case 1:
        rasp=rand();
        break;

    case 2:
        rasp= randomTable[seed];
        seed=(seed+1)%1000;
        break;
    }
    return rasp;
}

bool RandomGenerator::insideTest()
{
    //performs a test to see if rand() functions like the original one.
    // usefull to see if we were paranoic for nothing

    unsigned int oldSeed=seed;
    init(1);
    srand(1); // with other than 1 it breaks

    bool rasp=true;
    for(int i=0; i<40; i++)
    {
        for(int j=0; j<25; j++)
        {
            unsigned char b=rand();
            if( b!= this->operator()() )
            {
                rasp=false;

            }
        }
    }

    seed=oldSeed;

    return rasp;
}


/* do not change this table by any means and any reason!!
We used for scrambling the authorization file the standard rand() function, but we are paranoic
that someone could upgrade some system and change the rand function, breaking the decoding of the file
To avoid such trouble, we put a table here generated with:

   srand(1);

     for(int i=0;i<40;i++)
      {
       for(int j=0;j<25;j++)
        {
      unsigned char b=rand();
      RMInit::logOut<<(unsigned int)b<<',';
     }
       RMInit::logOut<<std::endl;
      }
    return 0;

*/


unsigned char RandomGenerator::randomTable[1000]=
{
    103,198,105,115,81,255,74,236,41,205,186,171,242,251,227,70,124,194,84,248,27,232,231,141,118,
    90,46,99,51,159,201,154,102,50,13,183,49,88,163,90,37,93,5,23,88,233,94,212,171,178,
    205,198,155,180,84,17,14,130,116,65,33,61,220,135,112,233,62,161,65,225,252,103,62,1,126,
    151,234,220,107,150,143,56,92,42,236,176,59,251,50,175,60,84,236,24,219,92,2,26,254,67,
    251,250,170,58,251,41,209,230,5,60,124,148,117,216,190,97,137,249,92,187,168,153,15,149,177,
    235,241,179,5,239,247,0,233,161,58,229,202,11,203,208,72,71,100,189,31,35,30,168,28,123,
    100,197,20,115,90,197,94,75,121,99,59,112,100,36,17,158,9,220,170,212,172,242,27,16,175,
    59,51,205,227,80,72,71,21,92,187,111,34,25,186,155,125,245,11,225,26,28,127,35,248,41,
    248,164,27,19,181,202,78,232,152,50,56,224,121,77,61,52,188,95,78,119,250,203,108,5,172,
    134,33,43,170,26,85,162,190,112,181,115,59,4,92,211,54,148,179,175,226,240,228,158,79,50,
    21,73,253,130,78,169,8,112,212,178,138,41,84,72,154,10,188,213,14,24,168,68,172,91,243,
    142,76,215,45,155,9,66,229,6,196,51,175,205,163,132,127,45,173,212,118,71,222,50,28,236,
    74,196,48,246,32,35,133,108,251,178,7,4,244,236,11,185,32,186,134,195,62,5,241,236,217,
    103,51,183,153,80,163,227,20,211,217,52,247,94,160,242,16,168,246,5,148,1,190,180,188,68,
    120,250,73,105,230,35,208,26,218,105,106,126,76,126,81,37,179,72,132,83,58,148,251,49,153,
    144,50,87,68,238,155,188,233,229,37,207,8,245,233,226,94,83,96,170,210,178,208,133,250,84,
    216,53,232,212,102,130,100,152,217,168,135,117,101,112,90,138,63,98,128,41,68,222,124,165,137,
    78,87,89,211,81,173,172,134,149,128,236,23,228,133,241,140,12,102,241,124,192,124,187,34,252,
    228,102,218,97,11,99,175,98,188,131,180,105,47,58,255,175,39,22,147,172,7,31,184,109,17,
    52,45,141,239,79,137,212,182,99,53,193,199,228,36,131,103,216,237,150,18,236,69,57,2,216,
    229,10,248,157,119,9,209,165,150,193,244,31,149,170,130,202,108,73,174,144,205,22,104,186,172,
    122,166,242,180,168,202,153,178,194,55,42,203,8,207,97,201,195,128,94,110,3,40,218,76,215,
    106,25,237,210,211,153,76,121,139,0,34,86,154,212,24,209,254,228,217,205,69,163,145,198,1,
    255,201,42,217,21,1,67,47,238,21,2,135,97,124,19,98,158,105,252,114,129,205,113,101,166,
    62,171,73,207,113,75,206,58,117,167,79,118,234,126,100,255,129,235,97,253,254,195,155,103,191,
    13,233,140,126,78,50,189,249,124,140,106,199,91,164,60,2,244,178,237,114,22,236,243,1,77,
    240,0,16,139,103,207,153,80,91,23,159,142,212,152,10,97,3,209,188,167,13,190,155,191,171,
    14,213,152,1,214,229,242,214,246,125,62,197,22,142,33,46,45,175,2,198,185,99,201,138,31,
    112,151,222,12,86,137,26,43,33,27,1,7,13,216,253,139,22,194,161,164,227,207,210,146,210,
    152,75,53,97,213,85,209,108,51,221,194,188,247,237,222,19,239,229,32,199,226,171,221,164,77,
    129,136,28,83,26,238,235,102,36,76,59,121,30,168,172,251,106,104,243,88,70,6,71,43,38,
    14,13,210,235,178,31,108,58,59,192,84,42,171,186,78,248,246,199,22,158,115,17,8,219,4,
    96,34,10,167,77,49,181,91,3,160,13,34,13,71,93,205,155,135,120,86,213,112,76,156,134,
    234,15,152,242,235,156,83,13,167,250,90,216,176,181,219,80,194,253,93,9,90,42,165,226,163,
    251,183,19,71,84,154,49,99,50,35,78,206,118,91,117,113,182,77,33,107,40,113,46,37,207,
    55,128,249,220,98,156,215,25,176,30,109,74,79,209,124,115,31,74,233,123,192,90,49,13,123,
    156,54,237,202,91,188,2,219,181,222,61,82,182,87,2,212,196,76,36,149,200,151,181,18,128,
    48,210,219,97,224,86,253,22,67,200,113,255,202,77,181,168,138,7,94,225,9,51,166,85,87,
    59,29,238,240,47,110,32,2,73,129,226,160,127,248,227,71,105,227,17,182,152,185,65,159,24,
    34,168,75,200,253,162,4,26,144,244,73,254,21,75,72,150,45,232,21,37,203,92,143,174,109
};
