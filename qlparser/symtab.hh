#ifndef _SYMTAB_HH_
#define _SYMTAB_HH_

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
 *   store symbols (identifiers) during the parse process
 *
 *
 * COMMENTS:
 *   none
 *
 ***********************************************************************/

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "raslib/rmdebug.hh"

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  This class represents a generic symbol table. The operations on
  symbols in the symbol table are: putSymbol, getSymbol, lookupSymbol.
  The operations on scopes are: initScope, exitScope, outScope, clearScope,
  wipe.

*/

template <class T>
class SymbolTable
{
public:
    /// This vector stores keys available in the map.
    std::vector<std::string > keys;
    /// This is an iterator for the vector storing the keys available in the map.
    std::vector<std::string >::iterator keyIterator;

    /// default constructor creates an empty symbol table, calls initScope(), clears local symbols.
    SymbolTable();

    /// default destructor, calls exitScope().
    ~SymbolTable();

    //@Man: Methods for symbol manipulation
    //@{
    /// Puts value at position symbol in the table. Returns true if it succeeded, otherwise false.
    bool    putSymbol( const std::string& symbol, T value );
    /// Get value at position symbol from the table. If symbol doesn't exist, it returns NULL.
    T       getSymbol( const std::string& symbol );
    /// Returns true if symbol is in table.
    bool lookupSymbol( const std::string& symbol );
    //@}

    //@Man: Methods for scope manipulation
    //@{
    /// Enter new scope.
    void  initScope();
    /// Exit current scope.
    void  exitScope();
    /// Output current scope to RMInit::logOut.
    void   outScope();
    /// Init scope by clearing inner symbols.
    void clearScope();
    /// Clear all symbols in all scopes.
    void       wipe();
    //@}

private:
    /// Store symbol in map.
    void storeSymbol( const std::string& symbol, T value ); // put only in the hash_map

    /// Stores local variables.
    std::map<std::string , T>   STVars;

    /// Stores scopes.
    std::vector<std::map<std::string , T> > STScopes;
};

#ifdef EARLY_TEMPLATE
#ifdef __EXECUTABLE__
#include "symtab.cc"
#endif
#endif

#endif
