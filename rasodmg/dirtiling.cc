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
 * SOURCE: dirtiling.cc
 *
 * MODULE: rasodmg
 * CLASS:  r_DirTiling
 *
 * COMMENTS:
 *			None
*/

#ifdef __VISUALC__
// Diable warning for signed/unsigned mismatch.
#pragma warning( disable : 4018 )
#endif  

#include "rasodmg/dirtiling.hh"
#include "rasodmg/dirdecompose.hh"
#include "rasodmg/alignedtiling.hh"
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
// #define _DEBUG_DIRTILING_

#ifdef _DEBUG_DIRTILING_
#define DEBUG_DIR(msg)      cout << "[" << __LINE__ << "] " << msg;
#else
#define DEBUG_DIR(msg)
#endif


#ifdef _VISUALIZE_2D_DECOMP_
#include "tools/visualtiling2d.hh"
#endif

const char*
r_Dir_Tiling::description = "dimensions, decomposision patterns, tile size(in bytes) and subtiling [SUBTILING|NOSUBTILING] (ex: \"3;[0,2,4,5],[*],[0,10,15];100;NOSUBTILING\")";

const char* r_Dir_Tiling::subtiling_name_withoutsubtiling = "NOSUBTILING";
const char* r_Dir_Tiling::subtiling_name_withsubtiling    = "SUBTILING";
const char* r_Dir_Tiling::all_subtiling_names[r_Dir_Tiling::NUMBER]={
                   subtiling_name_withoutsubtiling,
                   subtiling_name_withsubtiling
                   };
                   
r_Dir_Tiling::SubTiling
r_Dir_Tiling::get_subtiling_from_name(const char* name)
{
  if(!name) {
    RMInit::logOut << "r_Dir_Tiling::get_subtiling_from_name(" << (name?name:"NULL") << ")." << endl;  
    return r_Dir_Tiling::NUMBER;
  }

  unsigned int i=r_Dir_Tiling::NUMBER;
   
  for (i=0; i<(unsigned int)r_Dir_Tiling::NUMBER; i++)
  {
   if (strcasecmp(name, all_subtiling_names[i]) == 0)
      break;
  }
  
  return (r_Dir_Tiling::SubTiling)i;
}
               
const char*
r_Dir_Tiling::get_name_from_subtiling(SubTiling tsl)
{
 static const char* unknown="UNKNOWN";
 unsigned int idx = (unsigned int)tsl;
                    
 if (idx >= (unsigned int)r_Dir_Tiling::NUMBER)
    return unknown;
                            
 return all_subtiling_names[idx];
}                   

r_Dir_Tiling::r_Dir_Tiling(const char* encoded) throw (r_Error)
	:	r_Dimension_Tiling(0, 0)
{
 if(!encoded)
 {
 RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << (encoded?encoded: "NULL") << ")." << endl;
 throw r_Error(TILINGPARAMETERNOTCORRECT);
 }

 r_Dimension tileD=0, dirIndex=0;
 r_Range decomp=0;
 std::vector<r_Dir_Decompose> vectDirDecomp;
 r_Bytes tileS=0, lenToConvert=0, lenDirToConvert=0, lenDecomp=0;
 r_Dir_Tiling::SubTiling subTiling;
 const char *pStart=NULL, *pEnd=NULL, *pRes=NULL, *pTemp=NULL, *pToConvertEnd=NULL;
 char* pToConvert=NULL;
 const char *pDirRes=NULL, *pDirEnd=NULL, *pDirTemp=NULL, *pDirStart=NULL;
 char* pDirToConvert=NULL; 
 char* pDecomp=NULL;
 
 //initialisation
 pStart=encoded;
 pTemp=pStart;
 pEnd=pTemp+strlen(pStart);
 
 //deal with dimension
 pRes=strstr(pTemp, TCOLON);
 if(!pRes)
 {
 RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding tile dimension." << endl;
 throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 
 lenToConvert=pRes-pTemp;
 pToConvert=new char[lenToConvert+1];
 memcpy(pToConvert, pTemp, lenToConvert);
 pToConvert[lenToConvert]='\0';
 
 tileD=strtol(pToConvert, (char**)NULL, DefaultBase);
 if(!tileD)
 { 
 RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding tile dimension \"" << pToConvert << "\" is not a number." << endl;
  delete[] pToConvert;
 throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 if(tileD<0)
 { 
 RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding tile dimension \"" << pToConvert << "\" is negative." << endl;
  delete[] pToConvert;
 throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 
 vectDirDecomp=std::vector<r_Dir_Decompose>(tileD);
 
 delete[] pToConvert;
 if(pRes==(pEnd-1))
 {
 RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding directional decompose, end of stream." << endl;
 throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 else
  pRes++;
 pTemp=pRes; 
 
 //deal with directional decompose
 pRes=strstr(pTemp, TCOLON);
 if(!pRes)
 {
 RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding directional decompose." << endl;
 throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
                 
 lenToConvert=pRes-pTemp;
 pToConvert=new char[lenToConvert+1];
 memcpy(pToConvert, pTemp, lenToConvert);
 pToConvert[lenToConvert]='\0';
 pToConvertEnd=pToConvert+strlen(pToConvert);
 pDirTemp=pToConvert;
 
 while(dirIndex < tileD)
 {
  pDirRes=strstr(pDirTemp, LSQRBRA);
  if(!pDirRes)
  {
     RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding directional decompose for dimension " << dirIndex+1 << " from \"" << pToConvert << "\"." << endl;
    delete[] pToConvert;
    throw r_Error(TILINGPARAMETERNOTCORRECT);
   }
  pDirStart=pDirRes;
  pDirRes=strstr(pDirTemp, RSQRBRA);
  if(!pDirRes)
  {
   RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding directional decompose for dimension " << dirIndex+1 << " from \"" << pToConvert << "\"." << endl;
   delete[] pToConvert; 
   throw r_Error(TILINGPARAMETERNOTCORRECT);
   }
  pDirEnd=pDirRes;
  
  lenDirToConvert=pDirEnd-pDirStart; 
  if(lenDirToConvert == 1)
  {
   RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding directional decompose for dimension " << dirIndex+1 << " from \"" << pToConvert << "\"." << endl;
   delete[] pToConvert;
   throw r_Error(TILINGPARAMETERNOTCORRECT);
  }
  
  pDirToConvert=new char[lenDirToConvert];
  memcpy(pDirToConvert, pDirStart+1, lenDirToConvert-1);
  pDirToConvert[lenDirToConvert-1]='\0';
  pDirTemp=pDirToConvert;
  pDirStart=pDirEnd;
  pDirEnd=pDirToConvert+strlen(pDirToConvert);
  
  if(*pDirToConvert != *ASTERIX)
  {
   pDirRes=strstr(pDirToConvert, TCOMMA);
   if(!pDirRes)
   {
     RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding directional decompose for dimension " << dirIndex+1 << " from \"" << pToConvert << "\"." << endl;
     delete[] pDirToConvert;
     delete[] pToConvert;
     throw r_Error(TILINGPARAMETERNOTCORRECT);
   }
   while(pDirRes)
   {
    lenDecomp=pDirRes-pDirTemp;
    pDecomp=new char[lenDecomp+1];
    memcpy(pDecomp, pDirTemp, lenDecomp);
    pDecomp[lenDecomp]='\0';
    
    decomp=strtoul(pDecomp, (char**)&pDirTemp, DefaultBase);

    if(*pDirTemp !='\0')
    {
     RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding decompose \"" << pDecomp << "\" from directional decompose \"" << pDirToConvert << "\", is not a number." << endl;
     delete[] pDecomp;
     delete[] pDirToConvert;
     delete[] pToConvert;
     throw r_Error(TILINGPARAMETERNOTCORRECT);
    }
    
    vectDirDecomp[dirIndex] << decomp;
    
    //skip COMMA & free buffer
    delete[] pDecomp;
    if(pDirRes==(pDirEnd-1))
    {
     RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding directional decompose \"" << pDirToConvert << "\", end of stream." << endl;
     delete[] pDirToConvert;
     delete[] pToConvert;
     throw r_Error(TILINGPARAMETERNOTCORRECT);
     }
    else
     pDirRes++;
    pDirTemp=pDirRes;
    
    //next decomp
    pDirRes=strstr(pDirTemp, TCOMMA);
    if(!pDirRes)
    {
      decomp=strtoul(pDirTemp, (char**)&pDirRes, DefaultBase);
      if(*pDirRes !='\0')
      {
       RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding decompose \"" << pDirTemp << "\" from directional decompose \"" << pDirToConvert << "\", is not a number." << endl;
       delete[] pDirToConvert;
       delete[] pToConvert;
       throw r_Error(TILINGPARAMETERNOTCORRECT);
      }                                                          
      vectDirDecomp[dirIndex]<<decomp;
      break;
     }//end if
    }//end while
  } //end if
  
  delete[] pDirToConvert; 
  dirIndex++;
  if(dirIndex < tileD)
  {
   if(pDirStart == (pToConvertEnd-1))
   {
    RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding directional decompose for dimension " << dirIndex << " from \"" << pToConvert << "\", end of stream." << endl;
    delete[] pToConvert;
    throw r_Error(TILINGPARAMETERNOTCORRECT);
   }
   else
    pDirStart++;
 
   pDirRes=strstr(pDirStart, TCOMMA);
   if(!pDirRes)
   {
    RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding directional decompose for dimension " << dirIndex+1 << " from \"" << pToConvert << "\", end of stream." << endl;
    delete[] pToConvert;
    throw r_Error(TILINGPARAMETERNOTCORRECT);
   }

   if(pDirRes == (pToConvertEnd-1))
   {
    RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding directional decompose for dimension " << dirIndex << " from \"" << pToConvert << "\", end of stream." << endl;
    delete[] pToConvert;
    throw r_Error(TILINGPARAMETERNOTCORRECT);
   }
   else
    pDirRes++;

   pDirTemp=pDirRes;
   }//end if(dirIndex<tileD)
 }//end while

 //skip COLON & free buffer 
 delete[] pToConvert;
 if(pRes==(pEnd-1))
 {
  RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding tile size from \"" << pStart << "\", end of stream." << endl;
  throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 else
  pRes++;
 pTemp=pRes;
 
 //deal with tilesize
 pRes=strstr(pTemp, TCOLON);
 if(!pRes)
 {
  RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding tile size from \"" << pTemp << "\", end of stream." << endl;
  throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
                 
 lenToConvert=pRes-pTemp;
 pToConvert=new char[lenToConvert+1];
 memcpy(pToConvert, pTemp, lenToConvert);
 pToConvert[lenToConvert]='\0';
 
 tileS=strtol(pToConvert, (char**)NULL, DefaultBase);

 if(!tileS)
 { 
  RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding tile size from \"" << pToConvert << "\", is not a number." << endl;
  delete[] pToConvert;
  throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 if(tileS<0)
 { 
  RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding tile size from \"" << pToConvert << "\", is negative." << endl;
  delete[] pToConvert;
  throw r_Error(TILINGPARAMETERNOTCORRECT);
 }

 //skip COLON & free buffer 
 delete[] pToConvert;
 if(pRes==(pEnd-1))
 {
  RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding subtiling from \"" << pStart << "\", end of stream." << endl;
  throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 else
  pRes++;
 pTemp=pRes; 
 //deal with subtilig
 subTiling=r_Dir_Tiling::get_subtiling_from_name(pTemp);
 if(subTiling==r_Dir_Tiling::NUMBER)
 {
  RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << encoded << "): Error decoding subtiling from \"" << pTemp << "\"." << endl;
  throw r_Error(TILINGPARAMETERNOTCORRECT);
 }
 dimension = tileD;
 dim_decomp = vectDirDecomp;
 sub_tile = subTiling;
 tile_size = tileS;
}

r_Dir_Tiling::r_Dir_Tiling(r_Dimension dims, const std::vector<r_Dir_Decompose>& decomp, r_Bytes ts, SubTiling sub) throw (r_Error)
	:	r_Dimension_Tiling(dims, ts),
		dim_decomp(decomp),
		sub_tile(sub)
{
  if (dim_decomp.size() != dimension)
    {
    RMInit::logOut << "r_Dir_Tiling::r_Dir_Tiling(" << dims << ", " << decomp << ", " << ts << ", " << sub << ") number of dimensions (" << dimension << ") does not match number of decomposition entries (" << decomp.size() << ")" << endl;
      throw r_Edim_mismatch(dimension, dim_decomp.size());
    }
}

r_Dir_Tiling::~r_Dir_Tiling()
{
}

r_Tiling_Scheme
r_Dir_Tiling::get_tiling_scheme() const
{
  return r_DirectionalTiling;
}

r_Tiling* r_Dir_Tiling::clone() const
{
  r_Tiling* copy = new r_Dir_Tiling(dimension, dim_decomp, tile_size, sub_tile);
  return copy;
}

void r_Dir_Tiling::print_status(std::ostream& os) const
{
  os << "r_Dir_Tiling[ ";
  r_Dimension_Tiling::print_status(os);
  os << " sub tiling = " << sub_tile << " decompose = { ";

  for (r_Dimension i = 0; i < dim_decomp.size(); i++)
    os << "dim #" << i << " : " << dim_decomp[i] << " ";
  os << "} ";

}

std::vector<r_Minterval>*  
r_Dir_Tiling::compute_tiles(const r_Minterval& domain, r_Bytes typelen) const throw (r_Error)
{
  // Aux variable
  r_Dimension i = 0;
  // The result
  std::vector<r_Minterval>* decomp_result = new std::vector<r_Minterval>();
  // An alias to result
  std::vector<r_Minterval>& result = *decomp_result;
  std::vector<r_Dir_Decompose> temp_dim_decomp = dim_decomp;


  // Check dims
  if (dimension != domain.dimension())
    {
    
    delete decomp_result;
    
    RMInit::logOut << "r_Dir_Tiling::compute_tiles(" << domain << ", " << typelen << ") dimensions of domain (" << domain.dimension() << ") do not match dimensions of tiling strategy (" << dimension << ")" << endl;
    throw r_Edim_mismatch(dimension, domain.dimension());
    }

  // Undefined dims
  bool* undef_dim = new bool[dimension];
  // Count of undef dims
  r_Dimension total_undef = 0;

  // Check if limits ok
  for (i = 0; i < dimension; i++)
  {
    // Restric defined
    if (temp_dim_decomp[i].get_num_intervals() > 0)
    {
      undef_dim[i] = false;
      r_Range lim1 = 0;
      r_Range lim2 = 0;

      lim1 = domain[i].high();
      lim2 = temp_dim_decomp[i].get_partition(temp_dim_decomp[i].get_num_intervals() - 1);
      if (lim1 != lim2)
        {
        delete[] undef_dim;
        delete decomp_result;
        
        RMInit::logOut << "r_Dir_Tiling::compute_tiles(" << domain << ", " << typelen << ") upper limit of domain (" << domain.dimension() << ") at dimension " << i << " (" << domain[i] << ") does not partition " << temp_dim_decomp[i].get_partition(temp_dim_decomp[i].get_num_intervals() - 1) << endl;
	throw r_Elimits_mismatch(lim1, lim2);
        }

      lim1 = domain[i].low();
      lim2 = temp_dim_decomp[i].get_partition(0);
      if (lim1 != lim2)
        {
        delete[] undef_dim;
        delete decomp_result;
        
        RMInit::logOut << "r_Dir_Tiling::compute_tiles(" << domain << ", " << typelen << ") lower limit of domain (" << domain.dimension() << ") at dimension " << i << " (" << domain[i] << ") does not partition " << temp_dim_decomp[i].get_partition(0) << endl;
	throw r_Elimits_mismatch(lim1, lim2);
        }
    }
    else                                               // Restric not defined
    {
      // Dim unspecified
      undef_dim[i] = true;
      total_undef++;
      temp_dim_decomp[i] << domain[i].low() << domain[i].high();
    }
  }

  // Create a counter for each dimension
  r_Dimension* dim_counter = new r_Dimension[dimension];
  memset(dim_counter, 0, sizeof(r_Dimension) * dimension);

#ifdef _VISUALIZE_2D_DECOMP_
  // User wants a visual of the 2D decomp.
  // Number of times a decomp has been made
  static int count;
  ++count;                                             // Update decomp count

  Visual_Tiling_2D* vis;
  if (dimension == 2)
  {
    // Create an object for visualization
    char fname[80];
    sprintf(fname, "2D_decomp_dir_%d.ppm", count);
    vis = new Visual_Tiling_2D(domain, fname);
  }
#endif

  // Iterate over the all space

  bool done = false;
  while (!done)
  {
    // Determine tile coordinates

    r_Minterval tile(dimension);

    for (i = 0; i < dimension; i++)                         
    {
	r_Range origin = temp_dim_decomp[i].get_partition(dim_counter[i]);
	if (dim_counter[i] != 0)
	  origin++;

	r_Range limit = temp_dim_decomp[i].get_partition(dim_counter[i]+1);

	tile << r_Sinterval(origin, limit);
    }
    
    // Do something with tile coordinates (decompose big object)

#ifdef _DEBUG_DIRTILING_
    cout << "(DirTiling::compute_tiles(): Tile: " << tile << endl;
#endif    

    // Check if sub-tiling should be done and calculate edgesize

    if ((tile.cell_count() * typelen > tile_size) && (sub_tile == WITH_SUBTILING))
    {
      // Create a specification for the partition      

      r_Minterval partition(dimension);
      
      if (total_undef == 0)
      {
        // No unspecified dimensions --- create block cross sections
        
        for (i=0; i<dimension; i++)
          partition << r_Sinterval((r_Range) 0, tile[i].high()-tile[i].low());
      }
      else
      {
        // Some unspecified dimensions

        // Compute edgesize
	r_Range edgesize = tile_size;

        for (i = 0; i < dimension; i++)
        {
          if (!undef_dim[i])
            edgesize/= (tile[i].high()-tile[i].low()+1) * typelen;
        }
        
	edgesize = floor(pow(((double)edgesize)/((double)typelen), (1.0/((double)total_undef))));

        // Create specification
        
        for (i = 0; i < dimension; i++)
        {
          if (undef_dim[i])
            partition << r_Sinterval((r_Range) 0, (edgesize-1));
          else
            partition << r_Sinterval((r_Range) 0, tile[i].high()-tile[i].low());
        }        
      }

#ifdef _DEBUG_DIRTILING_
      cout << "(DirTiling::compute_tiles(): Tile size = " << get_tile_size()
           << " Specs = " << partition << endl;
#endif

      // Create subtiles and insert them in the result

      r_Aligned_Tiling subtiling(partition, get_tile_size());
      std::vector<r_Minterval>* subtiles = subtiling.compute_tiles(tile, typelen);
      std::vector<r_Minterval>::iterator it = subtiles->begin();
      for (; it != subtiles->end(); it++)
      {
        result.push_back(*it);

#ifdef _VISUALIZE_2D_DECOMP_                          // If we are visualizing
	if (domain.dimension() == 2)
	{
	  vis->set_pen(0, 255, 0);
	  if (dimension == 2)
	    (*vis) << (*it);
	}
#endif
      }
      delete subtiles;
    }
    else
      result.push_back(tile);

#ifdef _VISUALIZE_2D_DECOMP_                          // If we are visualizing
    if (domain.dimension() == 2)
    {
      vis->set_pen(255, 255, 0);
      if (dimension == 2)
	(*vis) << tile;
    }
#endif
    
    // Update dimension counters

    for (i=0; i < dimension; i++)
    {
      dim_counter[i]++;
      if (dim_counter[i] == (temp_dim_decomp[i].get_num_intervals()-1))
        dim_counter[i] = 0;
      else
	break;
    }

    // See if we are done
    done = 1;
    for (i=0; i<dimension; i++)
      if (dim_counter[i] != 0)
      {
	done = 0;
	break;
      }
  }

  delete [] dim_counter;

#ifdef _VISUALIZE_2D_DECOMP_
  if (dimension == 2)
    delete vis;
#endif
  
  delete [] undef_dim;
  return &result;
}

bool
r_Dir_Tiling::is_compatible(const r_Minterval& domain, r_Bytes type_len) const
{
  bool retval = true;
  if (!r_Dimension_Tiling::is_compatible(domain, type_len))
    retval = false;
  else	{

  // Aux variable
  r_Dimension i = 0;

  for (i = 0; i < dimension; i++)                           // Check if limits ok
  {
    if (dim_decomp[i].get_num_intervals() > 0)         // Restric defined
    {
      r_Range lim1, lim2;

      lim1 = domain[i].high();
      lim2 = dim_decomp[i].get_partition(dim_decomp[i].get_num_intervals()-1);
      if (lim1 != lim2)
        {
	retval = false;
        break;
        }

      lim1 = domain[i].low();
      lim2 = dim_decomp[i].get_partition(0);
      if (lim1 != lim2)
        {
	retval = false;
        break;
        }
    }
  }
  }
  return retval;
}

