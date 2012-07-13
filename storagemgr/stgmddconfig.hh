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


#ifndef _STORAGE_MDD_CONFIG_HH_
#define _STORAGE_MDD_CONFIG_HH_

#include <vector>
#include "rasodmg/dirdecompose.hh"
#include "raslib/minterval.hh"

/****************************************************************************
 *
 *
 * INCLUDE: stgmddconfig.hh
 *
 * MODULE:  qlparser
 * CLASS:   StgMddConfig
 *
 * CHANGE HISTORY (append further entries):
 * when         who        what
 * -----------------------------------------------------------------------
 * 17-Aug-09	aaiordachi	create class (for adding rasql tiling options)
 * COMMENTS:
 *
 ****************************************************************************/

class StgMddConfig
	{
	private:
		std::vector<r_Minterval> bboxes;
		double interestThreshold;
		unsigned int borderThreshold;
		bool subTiling;
		int cellSize;
		std::vector<r_Dir_Decompose> dirDecomp;

	public:

		StgMddConfig();
		~StgMddConfig();

		// Setter functions 
		void setBBoxes(const std::vector<r_Minterval>& input);
		void setSubTiling();
		void resetSubTiling();
		void setInterestThreshold(double i);
		void setBorderThreshold(unsigned int b);
		void setCellSize(int size);
		void setDirDecompose(const std::vector<r_Dir_Decompose>& input);

		// Getter functions 
		std::vector<r_Minterval>& getBBoxes();
		bool getSubTiling();
		double getInterestThreshold();
		unsigned int getBorderThreshold();
		int getCellSize();
		std::vector<r_Dir_Decompose>& getDirDecompose();
                
	};
#endif
