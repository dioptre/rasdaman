#ifndef _QTCONVERSION_HH__
#define _QTCONVERSION_HH___

#include "qlparser/qtunaryoperation.hh"

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

  The class represents a format conversion. The conversion is described by
  an enumerator.

*/

class QtConversion : public QtUnaryOperation
{
  public:
    enum QtConversionType
    {
      QT_UNKNOWN,
      QT_TOTIFF,
      QT_TOBMP,
      QT_TOHDF,
      QT_TOCSV,
      QT_TOJPEG,
      QT_TOPNG,
      QT_TOVFF,
      QT_TOTOR,
      QT_TODEM,
      QT_FROMTIFF,
      QT_FROMBMP,
      QT_FROMHDF,
      QT_FROMCSV,
      QT_FROMJPEG,
      QT_FROMPNG,
      QT_FROMVFF,
      QT_FROMTOR,
      QT_FROMDEM      
    };

    /// constructor getting operand and format conversion type
    QtConversion( QtOperation* newInput, QtConversionType newConversionType, const char* = NULL );

    /// set the format conversion type by name
    void setConversionTypeByName( std::string formatName );

    /// look up the format conversion type by name
    static bool lookupConversionTypeByName( std::string formatName );

    /// test if the two nodes have an equal meaning in a subtree
    virtual bool equalMeaning( QtNode* node );

    /// evaluates the node
    QtData* evaluate( QtDataList* inputList );

    /// prints the tree 
    virtual void printTree( int tab, std::ostream& s = std::cout, QtChildType mode = QT_ALL_NODES );

    /// prints the algebraic expression
    virtual void printAlgebraicExpression( std::ostream& s = std::cout );

    /// method for identification of nodes
    inline virtual const QtNodeType getNodeType() const;

    /// type checking of the subtree
    virtual const QtTypeElement& checkType( QtTypeTuple* typeTuple = NULL );

  private:
    /// attribute storing conversion type
    QtConversionType conversionType;

    /// attribute for identification of nodes
    static const QtNodeType nodeType;

    const char* paramStr;
};

extern std::ostream& operator<<(std::ostream& os, QtConversion::QtConversionType t);

#include "qlparser/qtconversion.icc"

#endif

