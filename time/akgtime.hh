#ifndef AKGTIME_HH
#define AKGTIME_HH
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
 * COMMENTS:
 * Namespace akg
 * 
 ****************************************************************************/

#include <iostream>


#include <sys/time.h>
#include <time.h>

namespace akg
{

class LocalTime
  {
    public:
      LocalTime() throw();
      
      void now() throw();
            
    private:
      time_t value;
      
      friend std::ostream& operator<<(std::ostream&, const LocalTime&);
      
   };


class BenchmarkTimer
  {
    public:
      BenchmarkTimer(const char *text, bool highPrecision = false) throw();
      ~BenchmarkTimer() throw();
      
      void reset() throw();
      void start() throw();
      void stop()  throw();
      
      void setPrecision(bool high) throw();
      bool getPrecision()    const throw();
      
      friend std::ostream& operator<<(std::ostream&, BenchmarkTimer&);
    private:
      int  timeval_subtract(timeval *result,timeval *x,timeval *y) throw();

      struct timeval tvStart;
      struct timeval tvEnd;
      struct timeval tvResult;

      char* text;
      bool  precHigh;
   };

} // namespace

#endif
