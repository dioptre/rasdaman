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
 * INCLUDE: miter.hh
 *
 * MODULE:  raslib
 * CLASS:   r_MiterFloat
 *
*/

#ifndef _D_MITERF_
#define _D_MITERF_

class r_Minterval;


#include "raslib/mddtypes.hh"

class r_FixedPointNumber
  {
    public:
      inline r_FixedPointNumber();
      inline r_FixedPointNumber(const double&);
      
      inline r_FixedPointNumber& operator=(const r_FixedPointNumber&);
      inline r_FixedPointNumber& operator=(const double&);
      
      // returns intPart_new - intPart_old -- used for tests
      inline r_Range stepForward(const r_FixedPointNumber&);
      
      // returns carry of fracPart
      inline bool    stepForwardFlag(const r_FixedPointNumber&);
      
      inline r_Range getIntPart();
      
    private:
      inline void init(const double&);
      
      r_Range intPart;  
      r_Range fracPart; 
      
      static const int FIXPREC;
      static const r_Range carryPos;
      static const r_Range fracMask;     
      static const double fixOne;
      
      friend std::ostream& operator<<(std::ostream&,r_FixedPointNumber&);   
   };
   


//@ManMemo: Module: {\bf raslib}

/*@Doc:
  r_MiterFloat is used for iterating through parts of 
  multidimensional intervals with arbitrary stepping size using
  nearest neighbours. It is given the tile, the source domain 
  and the destination domain
  Apart from that behaviour is exactly as in r_Miter.

*/

class r_MiterFloat
  {
    public:
      /// Constructor getting the source tile, the source domain and the destination domain
      inline r_MiterFloat(Tile *sourceTile, r_Minterval& sourceDomain, r_Minterval& destDomain);
      
      /// destructor
      inline ~r_MiterFloat();
      
      /// iterator reset
      inline void reset();
      
      /// get the next cell
      inline char* nextCell();
      
      /// true if done
      inline bool isDone();
    
    protected:
      struct iter_desc
        {
	  r_FixedPointNumber min;
	  r_FixedPointNumber pos;
	  r_FixedPointNumber step;
	  
	  r_Range countSteps;
	  r_Range maxSteps;
	  
	  r_Range dimStep;
	  r_Range scaleStep;
	  char    *cell;
	 };  
	 
      r_Dimension dim;
      char        *currentCell;
      const char  *firstCell;
      
      iter_desc   *iterDesc;
      
      bool done;
   };


#include "miterf.icc"

#endif
