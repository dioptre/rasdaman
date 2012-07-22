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
 *  PURPOSE:
 *
 *  The label manager class is used to lookup label:value pairs
 *  stored in a text file. The constructor reads in the file whose
 *  name is passed to it and sorts the non-empty, non-commentary
 *  lines (commentary lines have a '#' as the first non-white
 *  character in a line) in ascending order to make binary searching
 *  possible. The binary search is performed by the lookup member
 *  function.
 *
 *  COMMENTS:  None
 */

/**
*   @defgroup Applications Applications
*
*   @file labelManager.hh
*
*   @ingroup Applications
*/

#ifndef _LABEL_MANAGER_H_
#define _LABEL_MANAGER_H_



class labelManager
{
public:

    labelManager(const char *resourceFile);
    ~labelManager(void);
    char *lookup(const char *symbol);
    // These two should be handled with care.
    int numberOfLabels(void);
    char *returnLabelNumber(unsigned int index);


private:

    void sortResources(int from, int to);

    char *buffer;
    char **lineTable;
    unsigned int lines;
    char badSymbol[4];
};



#endif
