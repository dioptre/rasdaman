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
 * INCLUDE: hierindexds.hh
 *
 * MODULE:  indexmgr
 * CLASS:   HierIndexDS
 *
 * COMMENTS:
 *
*/

#ifndef _HIERINDEXDS_HH_
#define _HIERINDEXDS_HH_

#include "indexmgr/indexds.hh"

/**
 *  @file hierindexds.hh
 *
 *  @ingroup indexmgr
 *
 *
 * @Doc:
 *  Interface class for Data Structure classes of Hierarchical Indexes.
 *  Classes which wish to use the RPTreeIndexLogic must implement this
 *  class.
*/

class HierIndexDS   :   public IndexDS
{
public:
    HierIndexDS():IndexDS() {}

    HierIndexDS(const OId& id):IndexDS(id) {}

    virtual double getOccupancy() const = 0;
    /*@Doc:
        Return the relative occupancy of this index.
        For flat indexes return 0.
        Not implemented.
    */

    virtual HierIndexDS* getParent() const = 0;
    /*@Doc:
        Returns a newly constructed HierIndexDS pointer to this
        object`s parent.
    */

    virtual void setParent(const HierIndexDS* newPa) = 0;
    /*@Doc:
        Sets the parent node of this object to newPa.
    */

    virtual void setIsNode(bool beNode) = 0;
    /*@Doc:
        When set to false, this index will behave as a leaf.
        When set to true, this index will behave as a node.
    */

    virtual bool isLeaf() const = 0;
    /*@Doc:
        If true this index contains objects which are stored in the index.
        If false this index contains index nodes.
    */

    virtual bool isRoot() const = 0;
    /*@Doc:
        If true this object is the root node.
    */

    virtual unsigned int getHeight() const = 0;
    /*@Doc:
        Returns the height of this subtree.
        Returns the complete height when called by the root node.
    */

    virtual unsigned int getTotalEntryCount() const = 0;
    /*@Doc:
        Get number of entries in leafs of the subtree.
        Returns the number of all entries in the tree
        when called by the root node.
    */

    virtual unsigned int getTotalNodeCount() const = 0;
    /*@Doc:
        Get number of nodes in the tree.
        Returns the number of all nodes in the tree
        when called by the root node.
        I do not count myself.
    */

    virtual unsigned int getTotalLeafCount() const = 0;
    /*@Doc:
        Get number of leafs in this subtree.
        Returns the number of all leafes in the tree
        when called by the root node.
        I do count myself -> I am a leaf, I return 1.
    */

};

#endif
