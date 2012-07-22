#ifndef _QTMARRAYOP2_
#define _QTMARRAYOP2_

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

  The class represents the root of a fake marray expression.

*/

#include "raslib/minterval.hh"

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

#include <vector>
#include "qlparser/qtoperation.hh"
#include "qlparser/qtdomainoperation.hh"
#include "qlparser/qtvariable.hh"
#include "qlparser/qtatomicdata.hh"
#include "qlparser/qtpointop.hh"
#include "qlparser/qtconst.hh"

class QtMarrayOp2
{
public:
    /// variables to pass to old marray
    r_Dimension greatDimension;
    r_Minterval greatDomain;
    std::string      greatIterator;

    /// pair (Identifier, Interval)
    typedef struct
    {
        std::string       variable;
        QtOperation* tree;
        ParseInfo    parseInfo;
    } mddIntervalType;

    /// list storing pairs (Identifier, Interval)
    typedef vector<mddIntervalType> mddIntervalListType;

    /// constructor getting iterator, minterval expression, and cell expression
    QtMarrayOp2( mddIntervalListType* & aList, QtOperation* & cellExp );

    /// destructor
    virtual ~QtMarrayOp2();

    /// optimizing load access
    virtual bool concatenateIntervals();
    virtual void rewriteVars();
    inline QtOperation *getInput() const;
    ///
    inline const ParseInfo&     getParseInfo();
    ///
    inline void                 setParseInfo( const ParseInfo &info );
    ///
    inline void                 setOldMarray( bool value );

protected:
    /// attribute for parser info
    ParseInfo parseInfo;

private:
    /// attribute storing the iterators
    mddIntervalListType iterators;

    /// attribute storing the cellExp
    QtOperation *qtOperation;

    /// tree traversal
    virtual void traverse(QtOperation *&node);

    /// replace Iterator name if this is false
    bool oldMarray;

};

#include "qlparser/qtmarrayop2.icc"

#endif

