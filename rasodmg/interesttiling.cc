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
 * SOURCE: interesttiling.cc
 *
 * MODULE: rasodmg
 * CLASS:  r_Interest_Tiling
 *
 * COMMENTS:
 *		None
*/

#include "rasodmg/interesttiling.hh"
#include "rasodmg/alignedtiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "rasodmg/dirtiling.hh"
#include "raslib/rminit.hh"

#include <assert.h>
#include <string.h>
#include <math.h>
#include <cstdlib>

#ifdef AIX
#include <strings.h>
#endif

// Uncoment the _VISUALIZE_2D_DECOMP_ line to generate ppm files the  
// visualization of the domain decomposition done by the algoritm
// #define _VISUALIZE_2D_DECOMP_

// Uncoment the folling line for some debug printfs on this class
// #define _DEBUG_INTERESTTILING_

#ifdef _VISUALIZE_2D_DECOMP_
#include "tools/visualtiling2d.hh"
#endif

// This is a data structure for internal use within interesttiling.cc.
// It is defined here because of problems with ptrepository.
// -------------
// This structure consists of a r_Minterval and a counter. Also, it has
// some auxiliary constructors.

class Classified_Block
	{
	public:
		int intersection_count;                         // Intersection count
		r_Minterval block;                              // The actual block

		// Default constructor
		Classified_Block(int count = 0);

		// Constructor with the actual block and the counter
		Classified_Block(const r_Minterval& b, int count = 0);

		// Same data structure (this operator is needed because of dlist)
		bool operator==(const Classified_Block& other) const;

		// Different data structure (this operator is needed because of dlist)
		bool operator!=(const Classified_Block& other) const;

		// Friend of std::ostream (this operator is needed because of dlist)
		friend std::ostream& operator<<(std::ostream& os, const Classified_Block& block);
	};

const char*
r_Interest_Tiling::description = "dimensions, areas of interest, tile size (in bytes) and tile size limit [NOLIMIT|REGROUP|SUBTILING|REGROUPSUBTILING] (ex: \"2;[0:9,0:9];[100:109,0:9];100;REGROUPSUBTILING\")";
	
const char* r_Interest_Tiling::tilesizelimit_name_nolimit		="NOLIMIT";
const char* r_Interest_Tiling::tilesizelimit_name_regroup		="REGROUP";   
const char* r_Interest_Tiling::tilesizelimit_name_subtiling		="SUBTILING";   
const char* r_Interest_Tiling::tilesizelimit_name_regroupandsubtiling	="REGROUPSUBTILING";   
const char* r_Interest_Tiling::all_tilesizelimit_names[r_Interest_Tiling::NUMBER]={
		   tilesizelimit_name_nolimit,
		   tilesizelimit_name_regroup,
		   tilesizelimit_name_subtiling,
		   tilesizelimit_name_regroupandsubtiling
		   };
		   
r_Interest_Tiling::Tilesize_Limit 
r_Interest_Tiling::get_tilesize_limit_from_name(const char* name)
{

 if(!name) {
   RMInit::logOut << "r_Interest_Tiling::get_tilesize_limit_from_name(" << (name?name: "NULL") << ")" << endl;
   return r_Interest_Tiling::NUMBER;
 }

 unsigned int i=r_Interest_Tiling::NUMBER;
 
 for (i=0; i<(unsigned int)r_Interest_Tiling::NUMBER; i++)
 {
  if (strcasecmp(name, all_tilesizelimit_names[i]) == 0)
     break;
 }
 return (r_Interest_Tiling::Tilesize_Limit)i;
}

const char*
r_Interest_Tiling::get_name_from_tilesize_limit(Tilesize_Limit tsl)
{
  static const char* unknown="UNKNOWN";
  unsigned int idx = (unsigned int)tsl;
    
  if (idx >= (unsigned int)r_Interest_Tiling::NUMBER)
     return unknown;
          
  return all_tilesizelimit_names[idx];
}

r_Interest_Tiling::r_Interest_Tiling(const char* encoded) throw (r_Error)
	:	r_Dimension_Tiling(0,0)
{
 if(!encoded)
 {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << (encoded?encoded: "NULL") << ")" << endl;
 throw r_Error(TILINGPARAMETERNOTCORRECT);
 }

 std::vector<r_Minterval> vectInterestAreas;
 r_Interest_Tiling::Tilesize_Limit tileSizeLimit;
 r_Dimension tileD=0;
 r_Bytes tileS=0, lenToConvert=0;
 const char *pStart=NULL, *pRes=NULL, *pTemp=NULL, *pEnd=NULL;
 char *pToConvert=NULL;
 

 pStart=encoded;
 pEnd=pStart+strlen(pStart);
 pTemp=pStart;
 pRes=strstr(pTemp,COLON);
 
 if(!pRes)
 {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << encoded << "): Error decoding tile dimension from \"" << pTemp << "\"." << endl;
 throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 
 //deal with dimension
 lenToConvert=pRes-pTemp;
 pToConvert=new char[lenToConvert+1];
 memcpy(pToConvert,pTemp, lenToConvert);
 pToConvert[lenToConvert]='\0';

 tileD=strtol(pToConvert, (char**)NULL, DefaultBase);
 if (!tileD)
 {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << encoded << "): Error decoding tile dimension from \"" << pToConvert << "\"." << endl;
  delete[] pToConvert;
 throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 if (tileD < 0)
 {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << encoded << "): Error decoding tile dimension from \"" << pToConvert << "\", is negative." << endl;
  delete[] pToConvert;
 throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 
 //skip COLON && free buffer
 delete[] pToConvert;
 if(pRes != (pEnd-1))
   pRes++;
 else
 {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << encoded << "): Error decoding interest areas, end of stream." << endl;
 throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 
 //parse interest areas
 pTemp=pRes;
 pRes=strstr(pTemp,COLON);
 
 if(!pRes)
 {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << encoded << "): Error decoding interest areas." << endl;
 throw r_Error(TILINGPARAMETERNOTCORRECT);
 }

 while(pRes)
 {  
  //is interest areas?
  if(*pTemp != *LSQRBRA)
    break;
  
  //copy parsed interest area
  lenToConvert=pRes-pTemp;
  pToConvert=new char[lenToConvert+1];
  memcpy(pToConvert, pTemp, lenToConvert);
  pToConvert[lenToConvert]='\0';
  
  //add to vector
  try
  {
   r_Minterval a(pToConvert);
   vectInterestAreas.push_back(a);
  }
  catch(r_Error& err)
  {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << encoded << "): Error decoding interest area from \"" << pToConvert << "\"." << endl;
   RMInit::logOut << "Error " << err.get_errorno() << " : " << err.what() << endl;
   delete [] pToConvert;
   throw r_Error(TILINGPARAMETERNOTCORRECT);
  }

  //skip COLON
  delete[] pToConvert;
  if(pRes != (pEnd-1))
   pRes++;
  else
  {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << encoded << "): Error decoding interest areas, end of stream." << endl;
   throw r_Error(TILINGPARAMETERNOTCORRECT);
  }

  //try next item
  pTemp=pRes;  
  pRes=strstr(pTemp, COLON);
 }
 
 if(vectInterestAreas.empty())
 {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << encoded << "): Error decoding interest areas, no interest areas specified." << endl;
   throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 
 //deal with tile size
 lenToConvert=pRes-pTemp;
 pToConvert=new char[lenToConvert+1];
 memcpy(pToConvert, pTemp, lenToConvert);
 pToConvert[lenToConvert]='\0';

 tileS=strtol(pToConvert, (char**)NULL, DefaultBase);
 if (!tileS)
 {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << encoded << "): Error decoding tile size from \"" << pToConvert << "\"." << endl;
  delete[] pToConvert;
   throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 if (tileS < 0)
 {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << encoded << "): Error decoding tile size from \"" << pToConvert << "\", is negative." << endl;
  delete[] pToConvert;
   throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 
 //skip COLON && free buffer
 delete[] pToConvert;
 if(pRes != (pEnd-1))
   pRes++;
 else
 {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << encoded << "): Error decoding tile size limit, end of stream." << endl;
   throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 
 pTemp=pRes;

 tileSizeLimit=r_Interest_Tiling::get_tilesize_limit_from_name(pTemp);
 if(tileSizeLimit==r_Interest_Tiling::NUMBER)
 {
 RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << encoded << "): Error decoding tile size limit from \"" << pTemp << "\"." << endl;
   throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 
 iareas = vectInterestAreas;
 ts_strat = tileSizeLimit;
 dimension = tileD;
 tile_size = tileS;
}

r_Interest_Tiling::r_Interest_Tiling(r_Dimension dim, const std::vector<r_Minterval>& interest_areas, r_Bytes ts, Tilesize_Limit strat) throw (r_Error)
	:	r_Dimension_Tiling(dim, ts),
		iareas(interest_areas),
		ts_strat(strat)
{
  for (std::vector<r_Minterval>::iterator it = iareas.begin(); it != iareas.end(); it++)
    if (it->dimension() != dimension)
      {
      RMInit::logOut << "r_Interest_Tiling::r_Interest_Tiling(" << dim << ", " << interest_areas << ", " << ts << ", " << strat << ") the interest area domain " << *it << " does not match the dimension of this tiling scheme (" << dimension << ")" << endl;
      throw r_Edim_mismatch(dimension, it->dimension());
      }
}

r_Interest_Tiling::~r_Interest_Tiling()
{
}

r_Tiling* r_Interest_Tiling::clone() const
{
  r_Tiling* copy = new r_Interest_Tiling(dimension, iareas, tile_size, ts_strat);
  return copy;
}

void r_Interest_Tiling::print_status(std::ostream& os) const
{
  os << "r_Interest_Tiling[ ";
  r_Dimension_Tiling::print_status(os);
  os << " interest areas = " << iareas << ", tiling strategy = " << ts_strat << " ]";
}

r_Tiling_Scheme
r_Interest_Tiling::get_tiling_scheme() const
	{
	return r_InterestTiling;
	}

static int r_Range_comp(const void *elem1, const void *elem2)
{
  r_Range e1 = *((r_Range*) elem1);
  r_Range e2 = *((r_Range*) elem2);

  if (e1 == e2)
    return 0;
  
  if (e1 < e2)
    return -1;
  else
    return +1;
}

std::vector<r_Dir_Decompose>*
r_Interest_Tiling::make_partition(const r_Minterval& domain) const
{
  r_Dimension dim = domain.dimension();
  int total = 2 * iareas.size();

  // We need one decomp from each dimension
  std::vector<r_Dir_Decompose>* part = new std::vector<r_Dir_Decompose>(dim);

  // We have at most (number of interest areas + 2) intervals
  r_Range* intervals = new r_Range[total + 2];

  // Create iterator for interest areas
  std::vector<r_Minterval>::const_iterator it = iareas.begin();
  int total_iareas = iareas.size();                     


  // For all dimensions
  for (r_Dimension i = 0; i < dim; i++)
  {
    it = iareas.begin();                              // Reset iterator
    intervals[0] = domain[i].low();               // Input lower domain limit 
    intervals[total+1] = domain[i].high();        // Input higher domain limit


    for (int j = 1; j < total + 1; j += 2, ++it)          // For all possible intervals
    {
      if ((*it)[i].low()-1 <= domain[i].low())    // Input low iarea limit
	intervals[j] = domain[i].low();
      else
	intervals[j] = (*it)[i].low()-1;        

      intervals[j+1] = (*it)[i].high();           // Input higher iarea limit
    }

    // Sort the table
    qsort((void*) intervals, total+2, sizeof(r_Range), r_Range_comp);

    // Create partition using the limits table
    for (int k=0; k<total+2; k++)               // all limits must be checked
    {
      if (k == total+1)                         // if on the last limit...
	((*part)[i]) << intervals[k];                //   input it
      else                                      // else
	if (intervals[k] != intervals[k+1])     //   if it is unique
	  ((*part)[i]) << intervals[k];              //     input it
    }
  }

  // Free memory
  delete [] intervals;

  // Return result
  return part;
}

std::vector<r_Minterval>* 
r_Interest_Tiling::group(std::vector<r_Minterval>& blocks, r_Bytes typelen, Blocks_Type btype) const
{
  r_Bytes tilesize = get_tile_size();
  int joins = 0;
  bool group_blocks = true;

  // The list of threated blocks
  std::vector<r_Minterval>* treated = new std::vector<r_Minterval>;

  // An iterator for the blocks list
  std::vector<r_Minterval>::iterator blocks_it = blocks.begin();

  // The current block
  r_Minterval current_block;

  // For all the blocks in list
  while (!blocks.empty())
  {
    // Get first block from list
    current_block = blocks.back();
    blocks.pop_back();
    
    //this is neccessary when the compiler optimizes out the .end() check
    unsigned int numberOfLevels = blocks.size();
    for (blocks_it = blocks.begin(); blocks_it != blocks.end(); blocks_it++)
    {
      if (numberOfLevels == 0)
          {
          RMInit::logOut << "r_Interest_Tiling::group() the for loop was incorrectly optimized.  breaking the loop." << endl;
          break;
          }
      r_Minterval aux = *blocks_it;

      // In principle two blocks can't be merged
      group_blocks = false;
      
      // If they can be merged
      if (current_block.is_mergeable(aux))
      {
        std::vector<r_Minterval>::iterator ia_it = blocks.begin();

        switch (btype)
        {
          case BLOCKS_A:

            group_blocks = true;

            // Check if the two blocks belong exaclty to the same iareas
            for (; ia_it != blocks.end(); ia_it++)
            {
              if (aux.intersects_with(*ia_it) != current_block.intersects_with(*ia_it))
              {
                group_blocks = false;
                break;
              }
            }

            break;

          case BLOCKS_B:
            
            for (; ia_it != blocks.end(); ia_it++) // For all iareas
            {
              // Find the one this block intersects
              if (current_block.intersects_with(*ia_it))
              {
                // Check if the other area intersects it
                if (!aux.intersects_with(*ia_it))
                  group_blocks = false;
                else
                  group_blocks = true;

                break;
              }
            }

            if (!group_blocks)
              break;

            group_blocks = false;

          case BLOCKS_C: // Falls in (this is, also applies to B);

            // Only on this two strategies, tilesize should be looked at
            if ((ts_strat == REGROUP) || (ts_strat == REGROUP_AND_SUBTILING))
            {
              // If the resulting size isn't larger than tilesize
              if ((current_block.cell_count()+aux.cell_count()) * typelen
                  < get_tile_size())
                group_blocks = true;
            }
            else
              group_blocks = true;

            break;
	}
      }

      // take care of the iterator advance, if is possible
      if (group_blocks)
      {
	// Merge them
	blocks.erase(blocks_it); // Automatically advances to next block
        // take care of the size of the blocks
        numberOfLevels--;
	current_block.closure_with(aux);
	++joins;
      }
      else     
        numberOfLevels--;
    }

    // Update the treated list with the current block
    treated->push_back(current_block);
  }

  std::vector<r_Minterval>* result = 0;

  // If there was joins, the algoritm must be repeted
  if (joins != 0)
  {
    result = group(*treated, typelen, btype);
    delete treated;
  }
  else
  {
    result = treated;
  }

  // Return the result
  return result;
}

std::vector<r_Minterval>*  
r_Interest_Tiling::compute_tiles(const r_Minterval& domain, r_Bytes typelen) const throw (r_Error)
{
  r_Dimension num_dims = domain.dimension();                   // Dimensionality of dom
if (domain.dimension() != dimension)
	{
	RMInit::logOut << "r_Interest_Tiling::compute_tiles(" << domain << ", " << typelen << ") dimension (" << dimension << ") does not match dimension of object to tile (" << num_dims << ")" << endl;
	throw r_Edim_mismatch(dimension, num_dims);
	}
if (typelen > tile_size)
	{
	RMInit::logOut << "r_Interest_Tiling::compute_tiles(" << domain << ", " << typelen << ") tile size (" << tile_size << ") is smaller than type length (" << typelen << ")" << endl;
	throw r_Error(TILESIZETOOSMALL);
	}

#ifdef _VISUALIZE_2D_DECOMP_                           // User wants a visual 
  static int count;                                    // Number of decomps
  ++count;                                             // Update num decomps
                                                       // of the 2D decomp.
  Visual_Tiling_2D* vis;
  if (domain.dimension() == 2)
  {
    // Create an object for visualization
    char fname[80];
    sprintf(fname, "2D_decomp_int_%d.ppm", count);
    vis = new Visual_Tiling_2D(domain, fname);
  }

#endif


  // *** Main algoritm ***

  // The result
  std::vector<r_Minterval>* result = new std::vector<r_Minterval>; 

  // Create a partition for dir tiling
  std::vector<r_Dir_Decompose>* part = make_partition(domain);

  // Perform dirtiling
  r_Dir_Tiling *dir_tiling= NULL;
  std::vector<r_Minterval>* dir_domain=NULL;
  
  try
  {
   dir_tiling=new r_Dir_Tiling(num_dims, *part, tile_size, r_Dir_Tiling::WITHOUT_SUBTILING);

   // Compute all tiles (directional tiles)
   dir_domain = dir_tiling->compute_tiles(domain, typelen);
   
   delete dir_tiling;
  }
  catch(r_Error& err)
  {
   delete result;
   delete part;
   if(dir_tiling)
     delete dir_tiling;
   throw;
  }
  
  // Get an interator for interest areas
  //std::vector<r_Minterval>::iterator interest_area;

  // Create a list for holding the classifed blocks
  std::vector<Classified_Block> part_domain;

  // Finds how many intersections exist between a block an the interest areas
  for (std::vector<r_Minterval>::iterator dir_block = dir_domain->begin(); dir_block != dir_domain->end(); dir_block++)
  {
    Classified_Block b(*dir_block, 0);
    
    for (std::vector<r_Minterval>::const_iterator interest_area = iareas.begin(); interest_area != iareas.end(); interest_area++)
    {
      if (b.block.intersects_with(*interest_area))
	++b.intersection_count;
    }

    part_domain.push_back(b);
  }

  // Lists used for grouping blocks
  std::vector<r_Minterval> Out;
  std::vector<r_Minterval> In_Unique;
  std::vector<r_Minterval> In_Common;
  
  // Divide blocks into lists according to their number of intersections
  for (std::vector<Classified_Block>::iterator class_block = part_domain.begin(); class_block != part_domain.end(); class_block++)
  {
    switch ((*class_block).intersection_count)
    {
      case 0:
	Out.push_back((*class_block).block);
	break;
      case 1:
	In_Unique.push_back((*class_block).block);
	break;
      default:
	In_Common.push_back((*class_block).block);
	break;
    }
  }
  

  // Group blocks
  std::vector<r_Minterval>* Blocks_A = group(In_Common, typelen, BLOCKS_A);
  std::vector<r_Minterval>* Blocks_B = group(In_Unique, typelen, BLOCKS_B);
  std::vector<r_Minterval>* Blocks_C = group(Out, typelen, BLOCKS_C);


  std::vector<r_Minterval>::iterator it_A = Blocks_A->begin();
  std::vector<r_Minterval>::iterator it_B = Blocks_B->begin();
  std::vector<r_Minterval>::iterator it_C = Blocks_C->begin();

  // If may be necessary to perform sub-tiling
  if ((ts_strat == SUB_TILING) || (ts_strat == REGROUP_AND_SUBTILING))
  {
    // Variable to hold result of sub-tiling
    std::vector<r_Minterval>* subtiles = 0;

    // We need an array to hold the 3 iterators of the resulting blocks
    std::vector<r_Minterval>::iterator* blocks_it[3];
    std::vector<r_Minterval>* blocks_vec[3];

    // Put iterators on the list
    blocks_it[0] = &it_A;
    blocks_it[1] = &it_B;
    blocks_it[2] = &it_C;

    blocks_vec[0] = Blocks_A;
    blocks_vec[1] = Blocks_B;
    blocks_vec[2] = Blocks_C;

    // For all the lists (Blocs_A, Blocks_B and Blocks_C)
    for (int j=0; j<3; j++)
    {
      // Reset iterator to the begin
      (*blocks_it[j]) = blocks_vec[j]->begin();

      // Tile each block if necessary
      for (; *blocks_it[j] != blocks_vec[j]->end(); (*blocks_it[j])++)
      {
	if ((**blocks_it[j]).cell_count()*typelen > get_tile_size())
	{
    // Create a specification of a regular n-dim cube grid
          r_Minterval specs(num_dims);
          for (r_Dimension i=0; i<num_dims; i++)
          {
            specs << r_Sinterval((r_Range)0, (**blocks_it[j])[i].high() - (**blocks_it[j])[i].low());
          }

          // Class for performing sub-tiling
          r_Aligned_Tiling subtiling(specs, get_tile_size());

          subtiles = subtiling.compute_tiles(**blocks_it[j], typelen);
	  for (std::vector<r_Minterval>::iterator it = subtiles->begin(); it != subtiles->end(); it++)
	  {
	    result->push_back(*it);

#ifdef _VISUALIZE_2D_DECOMP_
	    if (domain.dimension() == 2)
	      (*vis) << (*it);
#endif
	  }
	
	  delete subtiles;	
	}
	else    // No subtiling needed
	{
	  // Insert block as it is
	  result->push_back(**blocks_it[j]);

#ifdef _VISUALIZE_2D_DECOMP_
	  if (domain.dimension() == 2)
	    (*vis) << (**blocks_it[j]);
#endif
	}
      }
    }

#ifdef _VISUALIZE_2D_DECOMP_
    if (domain.dimension() == 2)
    {
      vis->set_pen(255, 255, 0);

      interest_area.seek_begin();
      while (interest_area.not_done())
      {
	(*vis) << (*interest_area);
	++interest_area;
      }
    }
#endif

  }
  else
  {
    // The result is just the sum of all blocks

    result->insert(result->end(), Blocks_A->begin(), Blocks_A->end());
    result->insert(result->end(), Blocks_B->begin(), Blocks_B->end());
    result->insert(result->end(), Blocks_C->begin(), Blocks_C->end());

    // Visualization

#ifdef _VISUALIZE_2D_DECOMP_
    if (domain.dimension() == 2)
    {
      vis->set_pen(255, 255, 0);
      for (it_A.seek_begin(); it_A.not_done(); it_A++)
	(*vis) << *it_A;

      vis->set_pen(0, 255, 0);
      for (it_B.seek_begin(); it_B.not_done(); it_B++)
	(*vis) << *it_B;

      vis->set_pen(0, 0, 255);
      for (it_C.seek_begin(); it_C.not_done(); it_C++)
	(*vis) << *it_C;
    }
#endif
  }

  // *** End of the main algorithm ***

  // *** Clean up ***

  delete part;
  delete dir_domain;
  delete Blocks_A;
  delete Blocks_B;
  delete Blocks_C;

#ifdef _VISUALIZE_2D_DECOMP_
  if (domain.dimension() == 2)
    delete vis;
#endif

  // Return result

  return result;
}



std::ostream& operator<<(std::ostream& os, const Classified_Block block);


Classified_Block::Classified_Block(int count)
	:	intersection_count(count)
	{
	}

Classified_Block::Classified_Block(const r_Minterval& b, int count)
	:	intersection_count(count),
		block(b)
	{
	}


bool Classified_Block::operator==(const Classified_Block& other) const
{
  return (this->block == other.block)
      && (this->intersection_count == other.intersection_count);
}

bool Classified_Block::operator!=(const Classified_Block& other) const
{
  return !(*this == other);
}


std::ostream& operator<<(std::ostream& os, const Classified_Block block)
{
  os << "CBlock(" << block.intersection_count << "x: " << block.block << ")";

  return os;
}
