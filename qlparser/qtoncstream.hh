#ifndef _QTONCSTREAM_
#define _QTONCSTREAM_

#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
#endif

#include "qlparser/qtnode.hh"

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
/************************************************************
 *
 *
 * COMMENTS:
 *
 ************************************************************/


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

This class is super class for every class supporting the ONC protocol
which means that, first, a stream has to be opened with open(),
then the elements are received through invoking next(),
and at the end, the stream is closed with the close() method. The
next() method returns a tupel of QtData or any subclass of QtData.

*/

class QtONCStream : public QtNode
{
  public:
    /// default constructor
    QtONCStream();

    /// constructor getting a pointer to the parent
    QtONCStream( QtNode* node );

    //@Man: Operations of the ONC protocol
    //@{
    ///
    virtual void        open()=0;
    ///
    virtual QtDataList* next()=0;
    ///
    virtual void        close()=0;
    ///
    virtual void        reset()=0;
    //@}

    /// type checking of the subtree
    virtual const QtTypeTuple& checkType() = 0;
    /**
      The method triggers type checking of the node's subtree. If an error occurs, an exception
      is raised. 
    */

    //@Man: Read/Write methods
    //@{
    ///
      /// 
      inline void               setDataStreamType( const QtTypeTuple& type );
      ///
      inline const QtTypeTuple& getDataStreamType() const;
    ///
    //@}

  protected:
    /// result type of the node
    QtTypeTuple dataStreamType;

  private:
	/// atribute for identification of nodes
	static const QtNodeType nodeType;	
};

#endif

