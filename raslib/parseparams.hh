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
 * INCLUDE: parseparams.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Parse_Params
 *
 * COMMENTS:
 *
*/

#ifndef _PARSE_PARAMS_HH_
#define _PARSE_PARAMS_HH_

#include <iosfwd>

//@ManMemo: Module {\bf raslib}

/*@Doc:
  Class for parsing a string consisting of key=value pairs separated by ",".
*/

class r_Parse_Params
{
public:

    /// the possible parameter types used for add()
    enum parse_param_type
    {
        param_type_int,
        param_type_double,
        param_type_string
    };
    /**
       Possible parameter types and their corresponding C types are

       \begin{tabular}{ll}
       param_type_int && int\\
       param_type_double && double\\
       param_type_string && char*\\
       \end{tabular}
    */

    struct parse_params_s;

    /// default constructor, should not be used
    r_Parse_Params( void );
    /// constructor, gets descriptor of the values to scan for
    r_Parse_Params( unsigned int num );
    /// destructor
    ~r_Parse_Params( void );
    /// add parameters to the list
    int add( const char *key, void *store, parse_param_type type );
    /**
       Add a parameter to the list. key is the keyword, e.g. ``quality'', type
       is one of the available types and describes the data type of the parameter
       and store is a pointer to a variable of this type that will be updated
       by process() if the parameter is encountered there. The variable pointed
       to by store must not be initalized except for param_type_string where it
       must be set to NULL before calling process() for the first time. The contents
       of a string variable must be freed by the caller by first checking whether
       the variable is NULL and if not doing a delete [] <var>.
     */
    /// process parameter string
    int process( const char *str ) const;
    /**
       process the parameter string. Returns the number of keywords found
       or -1 for error.
    */


protected:
    //@Man: The parameter descriptor
    //@{
    typedef struct parse_params_s
    {
        const char *key;
        void *store;
        parse_param_type type;
    } parse_params_t;
    //@}

    parse_params_t *params;
    unsigned int maxnum;
    unsigned int number;

    static const unsigned int granularity;
};

//@ManMemo: Module: {\bf raslib}
/**
  Output stream operator for objects of type {\tt const} \Ref{r_Parse_Params::parse_param_type}.
  */
extern std::ostream& operator<<( std::ostream& s, const r_Parse_Params::parse_param_type& d );


#endif
