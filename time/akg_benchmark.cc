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

/*************************************************************************
 *
 *
 * PURPOSE:
 *   
 *    
 *
 * COMMENTS:
 *   none
 *
 ***********************************************************************/

#include "akgtime.hh"
#include<iomanip>

using namespace akg;

    
BenchmarkTimer::BenchmarkTimer(const char *lText, bool highPrecision) throw()
  {
    text = (char*)lText;
    
    precHigh = highPrecision;
   }
BenchmarkTimer::~BenchmarkTimer() throw()
  {
   }

void BenchmarkTimer::reset() throw()
  {
    tvStart.tv_sec  = 0;
    tvStart.tv_usec = 0;
    tvEnd.tv_sec    = 0;
    tvEnd.tv_usec   = 0;
   }
   
void BenchmarkTimer::start() throw()
  { 
    gettimeofday(&tvStart,NULL);
   }

void BenchmarkTimer::stop() throw()
  {
    gettimeofday(&tvEnd,NULL);
    timeval_subtract(&tvResult,&tvEnd,&tvStart);
   }

void BenchmarkTimer::setPrecision(bool high) throw()
  {
    precHigh = high;
   }

bool BenchmarkTimer::getPrecision() const throw()
  {
    return precHigh;
   }
      
int BenchmarkTimer::timeval_subtract(timeval *lResult,timeval *x,timeval *y) throw()
 {
       /* Perform the carry for the later subtraction by updating Y. */
       if (x->tv_usec < y->tv_usec) {
         int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
         y->tv_usec -= 1000000 * nsec;
         y->tv_sec += nsec;
       }
       if (x->tv_usec - y->tv_usec > 1000000) {
         int nsec = (x->tv_usec - y->tv_usec) / 1000000;
         y->tv_usec += 1000000 * nsec;
         y->tv_sec -= nsec;
       }
     
       /* Compute the time remaining to wait.
          `tv_usec' is certainly positive. */
       lResult->tv_sec = x->tv_sec - y->tv_sec;
       lResult->tv_usec = (x->tv_usec - y->tv_usec);
     
       /* Return 1 if result is negative. */
       return x->tv_sec < y->tv_sec;
  }


std::ostream& akg::operator<<(std::ostream &os, BenchmarkTimer &bm)
  {
    if(bm.precHigh == true)
      { 
        unsigned int msec = bm.tvResult.tv_sec*1000 + bm.tvResult.tv_usec / 1000;
        unsigned int usec = bm.tvResult.tv_usec % 1000;
    
        os<<bm.text<<"="<<msec<<'.'<<std::setfill('0')<<std::setw(3)<<usec<<"ms";    
       }
    else
      {
      	unsigned int msec = bm.tvResult.tv_usec / 1000;
        unsigned int sec  = bm.tvResult.tv_sec;

        os<<bm.text<<"="<<sec<<'.'<<std::setfill('0')<<std::setw(3)<<msec<<"s";       
       }   
    return os;
   }

