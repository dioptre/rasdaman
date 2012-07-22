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
 * INCLUDE: stdexcept.h
 *
 * CLASSES:  exception
 *             logic_error
 *               domain_error
 *               invalid_argument
 *               length_error
 *               out_of_range
 *             runtime_error
 *               range_error
 *               overflow_error
 *
 * PURPOSE:
 * This file implements the exception class hierarchy of
 * Standard C++. It is thought to be substituted by a
 * compiler package implementation in future.
 *
 * COMMENTS:
 *
*/

#ifndef _D_EXCEPTION_
#define _D_EXCEPTION_

class exception
{
public:
    exception() throw() : errorString(0) {};

    exception( const char* s ) throw()
    {
        errorString = new char[strlen(s)+1];
        strcpy( errorString, s );
    };

    exception( const exception& ex ) throw()
    {
        errorString = new char[strlen(ex.what())+1];
        strcpy( errorString, ex.what() );
    };

    ~exception() throw()
    {
        if( errorString ) delete[] errorString;
    };

    exception& operator=( const exception& ex ) throw()
    {
        if( this != &ex )
        {
            if( errorString ) delete[] errorString;

            errorString = new char[strlen(ex.what())+1];
            strcpy( errorString, ex.what() );
        }
        return *this;
    };

    inline virtual const char* what() const throw()
    {
        return errorString;
    };

private:
    char* errorString;
};


class logic_error : public exception
{
public:
    logic_error( const char* what_arg ) throw() : exception( what_arg ) {};
};


class domain_error : public logic_error
{
public:
    domain_error( const char* what_arg ) throw() : logic_error( what_arg ) {};
};


class invalid_argument : public logic_error
{
public:
    invalid_argument( const char* what_arg ) throw() : logic_error( what_arg ) {};
};


class length_error : public logic_error
{
public:
    length_error( const char* what_arg ) throw() : logic_error( what_arg ) {};
};


class out_of_range : public logic_error
{
public:
    out_of_range( const char* what_arg ) throw() : logic_error( what_arg ) {};
};


class runtime_error : public exception
{
public:
    runtime_error( const char* what_arg ) throw() : exception( what_arg ) {};
};


class range_error : public runtime_error
{
public:
    range_error( const char* what_arg ) throw() : runtime_error( what_arg ) {};
};


class overflow_error : public runtime_error
{
public:
    overflow_error( const char* what_arg ) throw() : runtime_error( what_arg ) {};
};

#endif
