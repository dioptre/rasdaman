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

//**************************************************************
// Symbol Table class
//**************************************************************

//**************************************************************
// Implementation
//**************************************************************

#include "symtab.hh"

// constructor
template <class T>
SymbolTable<T>::SymbolTable() {
  initScope();
  STVars.clear();
}

// destructor
template <class T>
SymbolTable<T>::~SymbolTable() {
  exitScope();
}

// put symbol in keys and in the hash-table
template <class T>
bool SymbolTable<T>::putSymbol(const std::string& symbol, T value) {
  bool retVal = false;
  // if not locally declared
  if ( (STVars.find(symbol) == STVars.end()) ) {
    if (getSymbol(symbol) == 0) 
      keys.push_back(symbol);
    STVars[symbol] = value;
    retVal = true;
  } else 
    retVal = false;
  return retVal;  
}

// store symbol into hash-table
template <class T>
void SymbolTable<T>::storeSymbol( const std::string& symbol, T value ) {
  STVars[symbol] = value;
}

// get symbol from table
template <class T>
T SymbolTable<T>::getSymbol( const std::string& symbol ) {
  T retVal=NULL;

  // find if locally declared
  if ( !(STVars.find(symbol) == STVars.end()) )
  {
    retVal=STVars[symbol];
  } 
  else
  {
    // find if declared in outer scopes
    size_t idx=STScopes.size();  
    while (idx > 0) {
      idx--;
      if ( !(STScopes[idx].find(symbol) == STScopes[idx].end()) )
        retVal=STScopes[idx][symbol];      
    }
  }
  return retVal;
}

// lookup symbol in table
template <class T>
bool SymbolTable<T>::lookupSymbol( const std::string& symbol ) {
  bool retVal=false;

  // find if locally declared
  if ( !(STVars.find(symbol) == STVars.end()) )
  {
    retVal=true;
  }
  else
  {  
    // find if declared in outer scopes
    size_t idx = STScopes.size();  
    while (idx > 0) {
      idx--;
      if ( !(STScopes[idx].find(symbol) == STScopes[idx].end()) )
        retVal=true;      
    }
  }
  return retVal;
}

// output the current scope
template <class T>
void SymbolTable<T>::outScope() {
  RMInit::logOut << "Scope: " << STScopes.size() << endl;
}

// init scope by clearing inner symbols
template <class T>
void SymbolTable<T>::clearScope() {
  STVars.clear();
}

// enter new scope
template <class T>
void SymbolTable<T>::initScope() {
  STScopes.push_back(STVars);
  STVars.clear();
}

// exit current scope to previous one
template <class T>
void SymbolTable<T>::exitScope() {
  if (!STScopes.empty()) {
    STVars = STScopes[STScopes.size() - 1];
    STScopes.pop_back();
  }
}

// wipe out everything from symbol table
template <class T>
void SymbolTable<T>::wipe() {
  keys.clear();  
  STVars.clear();
  STScopes.clear();
}
