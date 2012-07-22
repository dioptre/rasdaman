/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
*/

/**
 * SOURCE: sinterval.cc
 *
 * MODULE: raslib
 * CLASS:  r_Sinterval
 *
 * COMMENTS:
 *
*/

static const char rcsid[] = "@(#)raslib, r_Sinterval: $Id: sinterval.cc,v 1.29 2002/08/19 11:11:25 coman Exp $";

#include "sinterval.hh"

#include <string>
#include <cstring>

#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <strstream>
#endif

// for min and max
#include <algorithm>

using namespace std;


#include "raslib/error.hh"
#include "raslib/rminit.hh"

r_Sinterval::r_Sinterval()
    : lower_bound(0),
      upper_bound(0),
      low_fixed(false),
      high_fixed(false)
{
}


r_Sinterval::r_Sinterval( char* stringRep ) throw(r_Eno_interval)
    : lower_bound(0),
      upper_bound(0),
      low_fixed(false),
      high_fixed(false)
{
    if(!stringRep)
    {
        RMInit::dbgOut << "r_Sinterval::r_Sinterval(" << (stringRep?stringRep: "NULL") << ")" << std::endl;
        throw r_Eno_interval();
    }

    char    charToken = 0;
    r_Range valueToken = 0;

    // for parsing the string
    std::istrstream str(stringRep, strlen(stringRep) + 1);

    str >> charToken;
    if(charToken == '*')
        set_low('*');
    else
    {
        str.putback(charToken);
        str >> valueToken;
        set_low(valueToken);
    }

    str >> charToken;
    if(charToken != ':')
    {
        // error
        lower_bound=0;
        upper_bound=0;
        low_fixed=false;
        high_fixed=false;

        RMInit::dbgOut << "r_Sinterval::r_Sinterval(" << stringRep << ") string doesn't have the pattern a:b" << endl;
        throw r_Eno_interval();
    }

    str >> charToken;
    if(charToken == '*')
        set_high('*');
    else
    {
        str.putback(charToken);
        str >> valueToken;
        set_high(valueToken);
    }
}


r_Sinterval::r_Sinterval( r_Range low, r_Range high ) throw( r_Eno_interval )
    : lower_bound(low),
      upper_bound(high),
      low_fixed(true),
      high_fixed(true)
{
    if( low > high )
    {
        RMInit::dbgOut << "r_Sinterval::r_Sinterval(" << low << ", " << high << ") not a interval" << endl;
        throw r_Eno_interval();
    }
}


r_Sinterval::r_Sinterval( char, r_Range high )
    : lower_bound(0),
      upper_bound(high),
      low_fixed(false),
      high_fixed(true)
{
}


r_Sinterval::r_Sinterval( r_Range low, char )
    : lower_bound(low),
      upper_bound(0),
      low_fixed(true),
      high_fixed(false)
{
}


r_Sinterval::r_Sinterval( char, char)
    : lower_bound(0),
      upper_bound(0),
      low_fixed(false),
      high_fixed(false)
{
}


bool
r_Sinterval::operator==( const r_Sinterval& interval ) const
{
    bool returnValue=true;

    if (low_fixed)
        returnValue = interval.low_fixed && lower_bound == interval.lower_bound;
    else
        returnValue = !interval.low_fixed;//other is fixed -> false
    if (returnValue)
    {
        if( high_fixed )
            returnValue = interval.high_fixed && upper_bound == interval.upper_bound;
        else
            returnValue = !interval.high_fixed;
    }

    return returnValue;
}



bool
r_Sinterval::operator!=( const r_Sinterval& interval ) const
{
    return !operator==( interval );
}

r_Range
r_Sinterval::get_extent() const throw(r_Error)
{
    r_Range ext;

    if(!low_fixed || !high_fixed)
    {
        RMInit::dbgOut << "r_Sinterval::get_extent() low or high are not fixed (" << *this << ")" << std::endl;
        throw r_Error(INTERVALOPEN);
    }

    ext = upper_bound - lower_bound + 1;

    return ext;
}

void
r_Sinterval::set_low ( r_Range low  ) throw( r_Eno_interval )
{
    if( high_fixed && low > upper_bound )
    {
        RMInit::dbgOut << "r_Sinterval::set_low(" << low << ") not an interval (" << *this << ")" << endl;
        throw r_Eno_interval();
    }

    lower_bound = low;
    low_fixed = true;
}


void
r_Sinterval::set_high( r_Range high ) throw( r_Eno_interval )
{
    if( low_fixed && high < lower_bound )
    {
        RMInit::dbgOut << "r_Sinterval::set_high(" << high << ") not an interval (" << *this << ")" << endl;
        throw r_Eno_interval();
    }

    upper_bound = high;
    high_fixed = true;
}


void
r_Sinterval::set_interval( r_Range low, r_Range high ) throw( r_Eno_interval )
{
    if( low > high )
    {
        RMInit::dbgOut << "r_Sinterval::set_interval(" << low << ", " << high << ") not an interval (" << *this << ")" << endl;
        throw r_Eno_interval();
    }

    lower_bound = low;
    upper_bound = high;
    low_fixed  = true;
    high_fixed = true;
}


void
r_Sinterval::set_interval( char, r_Range high )
{
    lower_bound = 0;
    upper_bound = high;
    low_fixed  = false;
    high_fixed = true;
}


void
r_Sinterval::set_interval( r_Range low, char )
{
    lower_bound = low;
    upper_bound = 0;
    low_fixed  = true;
    high_fixed = false;
}


void
r_Sinterval::set_interval( char, char )
{
    lower_bound = 0;
    upper_bound = 0;
    low_fixed  = false;
    high_fixed = false;
}


bool
r_Sinterval::intersects_with( const r_Sinterval& interval ) const
{
    int classnr = classify( *this, interval );

    return classnr !=  1 && classnr !=  6 && classnr != 16 && classnr != 21 &&
           classnr != 26 && classnr != 31 && classnr != 34 && classnr != 37;
}


r_Sinterval&
r_Sinterval::union_of( const r_Sinterval& interval1, const r_Sinterval& interval2  ) throw( r_Eno_interval )
{
    *this = calc_union( interval1, interval2 );

    return *this;
}


r_Sinterval&
r_Sinterval::union_with( const r_Sinterval& interval ) throw( r_Eno_interval )
{
    *this = calc_union( interval, *this );

    return *this;
}


r_Sinterval&
r_Sinterval::operator+=( const r_Sinterval& interval ) throw( r_Eno_interval )
{
    *this = calc_union( interval, *this );

    return *this;
}


r_Sinterval
r_Sinterval::create_union( const r_Sinterval& interval ) const throw( r_Eno_interval )
{
    r_Sinterval result;

    result = calc_union( interval, *this );

    return result;
}


r_Sinterval
r_Sinterval::operator+( const r_Sinterval& interval ) const throw( r_Eno_interval )
{
    r_Sinterval result;

    result = calc_union( interval, *this );

    return result;
}


r_Sinterval&
r_Sinterval::difference_of( const r_Sinterval& interval1, const r_Sinterval& interval2  ) throw( r_Eno_interval )
{
    *this = calc_difference( interval1, interval2 );

    return *this;
}


r_Sinterval&
r_Sinterval::difference_with( const r_Sinterval& interval ) throw( r_Eno_interval )
{
    *this = calc_difference( interval, *this );

    return *this;
}



r_Sinterval&
r_Sinterval::operator-=( const r_Sinterval& interval ) throw( r_Eno_interval )
{
    *this = calc_difference( interval, *this );

    return *this;
}


r_Sinterval
r_Sinterval::create_difference( const r_Sinterval& interval ) const throw( r_Eno_interval )
{
    r_Sinterval result;

    result = calc_difference( interval, *this );

    return result;
}


r_Sinterval
r_Sinterval::operator-( const r_Sinterval& interval ) const throw( r_Eno_interval )
{
    r_Sinterval result;

    result = calc_difference( interval, *this );

    return result;
}


r_Sinterval&
r_Sinterval::intersection_of( const r_Sinterval& interval1, const r_Sinterval& interval2  ) throw( r_Eno_interval )
{
    *this = calc_intersection( interval1, interval2 );

    return *this;
}


r_Sinterval&
r_Sinterval::intersection_with( const r_Sinterval& interval ) throw( r_Eno_interval )
{
    *this = calc_intersection( interval, *this );

    return *this;
}


r_Sinterval&
r_Sinterval::operator*=( const r_Sinterval& interval )  throw( r_Eno_interval )
{
    *this = calc_intersection( interval, *this );

    return *this;
}


r_Sinterval
r_Sinterval::create_intersection( const r_Sinterval& interval ) const throw( r_Eno_interval )
{
    r_Sinterval result;

    result = calc_intersection( interval, *this );

    return result;
}


r_Sinterval
r_Sinterval::operator*( const r_Sinterval& interval ) const throw( r_Eno_interval )
{
    r_Sinterval result;

    result = calc_intersection( interval, *this );

    return result;
}


r_Sinterval&
r_Sinterval::closure_of( const r_Sinterval& interval1, const r_Sinterval& interval2  ) throw( r_Eno_interval )
{
    *this = calc_closure( interval1, interval2 );

    return *this;
}


r_Sinterval&
r_Sinterval::closure_with( const r_Sinterval& interval ) throw( r_Eno_interval )
{
    *this = calc_closure( interval, *this );

    return *this;
}


r_Sinterval
r_Sinterval::create_closure( const r_Sinterval& interval ) const throw( r_Eno_interval )
{
    r_Sinterval result;

    result = calc_closure( interval, *this );

    return result;
}


void
r_Sinterval::print_status( std::ostream& s ) const
{
    if( low_fixed )
        s << lower_bound;
    else
        s << "*";

    s << ":";

    if( high_fixed )
        s << upper_bound;
    else
        s << "*";
}


r_Bytes
r_Sinterval::get_storage_size( ) const
{
    return ( 2 * ( sizeof( r_Range ) + sizeof(bool) ) );
}

r_Sinterval
r_Sinterval::calc_union( const r_Sinterval& a, const r_Sinterval& b ) const throw( r_Eno_interval )
{
    r_Sinterval result;

    switch( classify( a, b ) )
    {
    case  2:
    case  7:
    case  9:
    case 12:
    case 22:
    case 23:
    case 27:
    case 28:
    case 35:
    case 36:
        // result = [a1:b2]

        if( a.is_low_fixed() )
            result.set_low( a.low() );
        else
            result.set_low('*');

        if( b.is_high_fixed() )
            result.set_high( b.high() );
        else
            result.set_high('*');

        break;

    case  4:
    case  8:
    case 10:
    case 13:
    case 17:
    case 18:
    case 32:
    case 33:
    case 38:
    case 39:
        // result = [b1:a2]

        if( b.is_low_fixed() )
            result.set_low( b.low() );
        else
            result.set_low('*');

        if( a.is_high_fixed() )
            result.set_high( a.high() );
        else
            result.set_high('*');

        break;

    case  3:
    case 11:
    case 14:
    case 15:
    case 19:
    case 20:
    case 41:
    case 42:
    case 43:
    case 44:
    case 46:
    case 48:
    case 49:
    case 52:
        result = a;
        break;

    case  5:
    case 24:
    case 25:
    case 29:
    case 30:
    case 40:
    case 45:
    case 47:
    case 50:
    case 51:
        result = b;
        break;

    default: // case in { 1, 6, 16, 21, 26, 31, 34, 37 }
    {
        RMInit::dbgOut << "r_Sinterval::calc_union(" << a << ", " << b << ") not an interval" << endl;
        throw r_Eno_interval();
    }
    }

    return result;
}


r_Sinterval
r_Sinterval::calc_difference( const r_Sinterval& a, const r_Sinterval& b ) const throw( r_Eno_interval )
{
    r_Sinterval result;

    switch( classify( a, b ) )
    {
    case  2:
    case  9:
    case 20:
    case 23:
    case 28:
    case 36:
    case 39:
    case 43:
    case 49:
        // result = [a1:b1]

        if( a.is_low_fixed() )
            result.set_low( a.low() );
        else
            result.set_low('*');

        if( b.is_low_fixed() )
            result.set_high( b.low() );
        else
            result.set_high('*');

        break;

    case  1:
    case  6:
    case  7:
    case  8:
    case 16:
    case 17:
    case 21:
    case 22:
    case 26:
    case 27:
    case 31:
    case 32:
    case 34:
    case 35:
    case 37:
    case 38:
        result = a;
        break;

    case  4:
    case 10:
    case 15:
    case 18:
    case 33:
    case 42:
    case 48:
        // result = [b2:a2]

        if( b.is_high_fixed() )
            result.set_low( b.high() );
        else
            result.set_low('*');

        if( a.is_high_fixed() )
            result.set_high( a.high() );
        else
            result.set_high('*');

        break;

    default: // case in { 3, 5, 11, 12, 13, 14, 19, 24, 25, 29, 30, 40, 41, 44, 45, 46, 47, 50, 51, 52 }
    {
        RMInit::dbgOut << "r_Sinterval::calc_difference(" << a << ", " << b << ") not an interval" << endl;
        throw r_Eno_interval();
    }
    }

    return result;
}


r_Sinterval
r_Sinterval::calc_intersection( const r_Sinterval& a, const r_Sinterval& b ) const throw( r_Eno_interval )
{
    r_Sinterval result;

    switch( classify( a, b ) )
    {
    case  4:
    case 18:
    case 33:
    case 39:
        // result = [a1:b2]

        if( a.is_low_fixed() )
            result.set_low( a.low() );
        else
            result.set_low('*');

        if( b.is_high_fixed() )
            result.set_high( b.high() );
        else
            result.set_high('*');

        break;

    case  2:
    case 23:
    case 28:
    case 36:
        // result = [b1:a2]

        if( b.is_low_fixed() )
            result.set_low( b.low() );
        else
            result.set_low('*');

        if( a.is_high_fixed() )
            result.set_high( a.high() );
        else
            result.set_high('*');

        break;

    case  5:
    case 11:
    case 12:
    case 13:
    case 24:
    case 25:
    case 29:
    case 30:
    case 40:
    case 41:
    case 44:
    case 45:
    case 47:
    case 50:
    case 51:
    case 52:
        result = a;
        break;

    case  3:
    case  9:
    case 10:
    case 14:
    case 15:
    case 19:
    case 20:
    case 42:
    case 43:
    case 46:
    case 48:
    case 49:
        result = b;
        break;

    case  7:
    case 22:
    case 27:
    case 35:
        // result = [a2:a2]

        if( a.is_high_fixed() )
            result.set_interval( a.high(), a.high() );
        else
            result.set_interval( '*', '*' );

        break;

    case  8:
    case 17:
    case 32:
    case 38:
        // result = [b2:b2]

        if( b.is_high_fixed() )
            result.set_interval( b.high(), b.high() );
        else
            result.set_interval( '*', '*' );

        break;

    default: // case in { 1, 6, 16, 21, 26, 31, 34, 37 }
        RMInit::dbgOut << "r_Sinterval::calc_intersection(" << a << ", " << b << ") not an interval" << endl;
        throw r_Eno_interval();
    }

    return result;
}


r_Sinterval
r_Sinterval::calc_closure( const r_Sinterval& a, const r_Sinterval& b ) const throw( r_Eno_interval )
{
    r_Sinterval closure;

    if( !a.is_low_fixed() || !b.is_low_fixed() )
        closure.set_low('*');
    else
        closure.set_low( std::min( a.low(), b.low() ) );

    if( !a.is_high_fixed() || !b.is_high_fixed() )
        closure.set_high('*');
    else
        closure.set_high( std::max( a.high(), b.high() ) );

    return closure;
}


/*************************************************************
 * Method name...: classify
 *
 * Arguments.....: Two intervals for the classification.
 * Return value..: The classification class number (1..52).
 * Description...: The method classifies the two intervals into
 *                 one of 13 classes according to their spatial
 *                 relationship. Based on the classification, the
 *                 result of the operations union, difference,
 *                 and intersection can be calculated as shown
 *                 in the table in file sinterval.hh:
 ************************************************************/

int
r_Sinterval::classify( const r_Sinterval& a, const r_Sinterval& b ) const
{
    int classification = 0;

    if( a.is_low_fixed() && a.is_high_fixed() && b.is_low_fixed() && b.is_high_fixed() )
    {
        // classification 1..13

        if( a.low() < b.low() )
        {
            if( a.high() < b.high() )
            {
                if( a.high() < b.low() )
                    classification = 1;
                else if( a.high() == b.low() )
                    classification = 7;
                else
                    classification = 2;
            }
            else  if( a.high() == b.high() )
                classification = 9;
            else
                classification = 3;
        }
        else if( a.low() == b.low() )
        {
            if( a.high() < b.high() )
                classification = 12;
            else  if( a.high() == b.high() )
                classification = 11;
            else
                classification = 10;
        }
        else if( a.high() < b.high() )
            classification = 5;
        else if( a.high() == b.high() )
            classification = 13;
        else
        {
            if( a.low() < b.high() )
                classification = 4;
            else if( a.low() == b.high() )
                classification = 8;
            else
                classification = 6;
        }
    }
    else if( a.is_low_fixed() && !a.is_high_fixed() && b.is_low_fixed() && b.is_high_fixed() )
    {
        // classification 14..18

        if( a.low() < b.low() )
            classification = 14;
        else if( a.low() == b.low() )
            classification = 15;
        else
        {
            if( b.high() < a.low() )
                classification = 16;
            else if( b.high() == a.low() )
                classification = 17;
            else
                classification = 18;
        }
    }
    else if( !a.is_low_fixed() && a.is_high_fixed() && b.is_low_fixed() && b.is_high_fixed() )
    {
        // classification 19..23

        if( a.high() > b.high() )
            classification = 19;
        else if( a.high() == b.high() )
            classification = 20;
        else
        {
            if( a.high() < b.low() )
                classification = 21;
            else if( a.high() == b.low() )
                classification = 22;
            else
                classification = 23;
        }
    }
    else if( a.is_low_fixed() && a.is_high_fixed() && b.is_low_fixed() && !b.is_high_fixed() )
    {
        // classification 24..28

        if( b.low() < a.low() )
            classification = 24;
        else if( b.low() == a.low() )
            classification = 25;
        else
        {
            if( a.high() < b.low() )
                classification = 26;
            else if( a.high() == b.low() )
                classification = 27;
            else
                classification = 28;
        }
    }
    else if( a.is_low_fixed() && a.is_high_fixed() && !b.is_low_fixed() && b.is_high_fixed() )
    {
        // classification 29..33

        if( b.high() > a.high() )
            classification = 29;
        else if( b.high() == a.high() )
            classification = 30;
        else
        {
            if( b.high() < a.low() )
                classification = 31;
            else if( b.high() == a.low() )
                classification = 32;
            else
                classification = 33;
        }
    }
    else if( !a.is_low_fixed() && a.is_high_fixed() && b.is_low_fixed() && !b.is_high_fixed() )
    {
        // classification 34..36

        if( a.high() < b.low() )
            classification = 34;
        else if( a.high() == b.low() )
            classification = 35;
        else
            classification = 36;
    }
    else if( a.is_low_fixed() && !a.is_high_fixed() && !b.is_low_fixed() && b.is_high_fixed() )
    {
        // classification 37..39

        if( b.high() < a.low() )
            classification = 37;
        else if( b.high() == a.low() )
            classification = 38;
        else
            classification = 39;
    }
    else if( !a.is_low_fixed() && a.is_high_fixed() && !b.is_low_fixed() && b.is_high_fixed() )
    {
        // classification 40..42

        if( a.high() < b.high() )
            classification = 40;
        else if( a.high() == b.high() )
            classification = 41;
        else
            classification = 42;
    }
    else if( a.is_low_fixed() && !a.is_high_fixed() && b.is_low_fixed() && !b.is_high_fixed() )
    {
        // classification 43..45

        if( a.low() < b.low() )
            classification = 43;
        else if( a.low() == b.low() )
            classification = 44;
        else
            classification = 45;
    }
    else  if( !a.is_low_fixed() && !a.is_high_fixed() &&  b.is_low_fixed() &&  b.is_high_fixed() )
        classification = 46;
    else  if(  a.is_low_fixed() &&  a.is_high_fixed() && !b.is_low_fixed() && !b.is_high_fixed() )
        classification = 47;
    else  if( !a.is_low_fixed() && !a.is_high_fixed() && !b.is_low_fixed() &&  b.is_high_fixed() )
        classification = 48;
    else  if( !a.is_low_fixed() && !a.is_high_fixed() &&  b.is_low_fixed() && !b.is_high_fixed() )
        classification = 49;
    else  if( !a.is_low_fixed() &&  a.is_high_fixed() && !b.is_low_fixed() && !b.is_high_fixed() )
        classification = 50;
    else  if(  a.is_low_fixed() && !a.is_high_fixed() && !b.is_low_fixed() && !b.is_high_fixed() )
        classification = 51;
    else //   !a.is_low_fixed() && !a.is_high_fixed() && !b.is_low_fixed() && !b.is_high_fixed()
        classification = 52;

    return classification;
}



char*
r_Sinterval::get_string_representation() const
{
    unsigned int bufferSize = 128;  // should be enough

    // allocate buffer and initialize string stream
    char* buffer = new char[bufferSize];
    std::ostrstream domainStream( buffer, bufferSize );

    // write into string stream
    domainStream << (*this) << ends;

    // allocate memory taking the final string
    char* returnString = strdup(buffer);

    // delete buffer
    delete[] buffer;

    return returnString;
}



/*************************************************************
 * Method name...: operator<<( std::ostream& s, r_Sinterval& d )
 ************************************************************/
std::ostream& operator<<( std::ostream& s, const r_Sinterval& d )
{
    d.print_status( s );
    return s;
}

