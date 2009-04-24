#ifndef _QTMDD_
#define _QTMDD_

#include "qlparser/qtdata.hh"
#include "qlparser/qtscalardata.hh"
#include "catalogmgr/typefactory.hh"
#include "raslib/minterval.hh"

#include <string>
#include <list>

// forward declarations
class MDDObj;
class QtOperation;

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
/*************************************************************
 *
 *
 * COMMENTS:
 *
 ************************************************************/

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

  The class encapsulates a pointer to an object of type {\tt MDDObj}.
  Its purpose is to have the {\tt MDDObj} object available as a descendant
  of class {\tt QtData}. The class does not care of the memory management
  of the {\tt MDDObj} object.

  Note: The attribute lifetime of the superclass QtData ist set implicit
  in the constructor functions!
*/

class QtMDD : public QtData
{
  public:
    // list of lists of \Ref{QtScalarData} objects
    // typedef list< list<QtScalarData*>* > QtDimensionList;

    /// constructor getting the pointer to the MDDObj object
    QtMDD( MDDObj* ptr );

    /// constructor getting the pointer to the MDDObj object and an iterator name
    QtMDD( MDDObj* ptr, std::string name );

    /// constructor getting a domain expression and a literal list for initializing a transient constant object
    QtMDD( QtOperation* mintervalOp, std::list<QtScalarData*>* literalList );
    /**
      The list of literal objects is not deleted.
    */

    /// constructor getting the number of an mdd constant
    QtMDD( int constantNo );

    /// copy constructor
    QtMDD( const QtMDD& obj );

    /// destructor
    virtual ~QtMDD();

    //@Man: Read/Write methods:
    //@{
    ///
      /// gets the pointer to the MDDObj object
      inline MDDObj* getMDDObject() const;
      /// sets the pointer to the MDDObj object and it does not care about the old pointer
      inline void setMDDObject( MDDObj* ptr );
      /// read method for loadDomain
      inline const r_Minterval& getLoadDomain() const;
      /// write method for loadDomain
      inline void setLoadDomain( r_Minterval& newLoadDomain );

      /// returns a null-terminated string describing the type structure
      virtual char* getTypeStructure() const;
      /**
        The string pointer has to be free using free() by the caller.
      */
    ///
    ///
    //@}

    /// returns a pointer to the cell type
    BaseType* getCellType() const;

    /// returns size of one cell in bytes
    unsigned long getCellSize() const;

    /// returns {\tt QT_MDD}
    virtual QtDataType getDataType() const;

    /// compares data content
    virtual bool equal( const QtData* obj ) const;

    /// returns content dependent string representation 
    virtual std::string getSpelling() const;

    /// print status of the object to the specified stream
    virtual void printStatus( std::ostream& stream = std::cout ) const;

  private:
    /// prevents from using the default constructor
    QtMDD(){};

    /// pointer to the MDDObj object
    MDDObj* mddObject;

    /// load domain
    r_Minterval loadDomain;
};

#include "qlparser/qtmdd.icc"

#endif

