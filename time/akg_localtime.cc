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

/*
 * 2007-feb-07  PB          #include <akgtime.hh> -> "akgtime.hh"
 */

#include "akgtime.hh"
#include <iomanip>

using namespace akg;


LocalTime::LocalTime() throw()
  {
    now();
   }
      
void LocalTime::now() throw()
  {
    //gettimeofday(&value,NULL);
    value = time(NULL);
   }
      
std::ostream& akg::operator<<(std::ostream &os, const LocalTime &lt)
  {
    struct tm broken;
    
    localtime_r(&lt.value, &broken);
    
    os<<std::setw(2)<<broken.tm_mday<<'.';
    os<<std::setw(2)<<std::setfill('0')<<broken.tm_mon+1;
    os<<'.'<<broken.tm_year+1900<<' ';
    os<<std::setw(2)<<std::setfill('0')<<broken.tm_hour<<':';
    os<<std::setw(2)<<std::setfill('0')<<broken.tm_min<<':';
    os<<std::setw(2)<<std::setfill('0')<<broken.tm_sec;

    return os;
   }

