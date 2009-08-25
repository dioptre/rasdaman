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
 * CHANGE HISTORY (append further entries):
 * when         who        	 	what
 * ----------------------------------------------------------
 * 10-11-08     Shams      Created the class
 * COMMENTS:
 *
 ************************************************************/
#ifndef _QTMDDCFGOP_HH_
#define _QTMDDCFGOP_HH_

#include <iostream>
#ifndef CPPSTDLIB
#include <ospace/string.h> // STL<ToolKit>
#else
#include <string>
using namespace std;
#endif
#include<fstream>
#include "qlparser/qtconst.hh"
#include "qlparser/qtmddconfig.hh"
#include "rasodmg/interesttiling.hh"


class QtMddCfgOp: public QtOperation
{
private:
	QtMDDConfig* mddCfgObj;
	QtOperation* input;
	
public:

	QtMddCfgOp();
	~QtMddCfgOp();

	QtMddCfgOp( QtOperation* inputInit );

	QtMddCfgOp( int tilingType, int tileSize, int borderThreshold,
				double interestThreshold, QtOperation* tileCfg, QtNode::QtOperationList* box,std::vector<r_Dir_Decompose>* dDecomp,
				int indexType);

	QtMddCfgOp( int tilingType, int tileSize, int borderThreshold,
				double interestThreshold, QtOperation* tileCfg, QtNode::QtOperationList* box,std::vector<r_Dir_Decompose>* dDecomp);

	QtMddCfgOp(int index);

	void optimizeLoad( QtTrimList* trimList );

	QtMDDConfig* evaluate(QtDataList* inputList);

	QtMDDConfig* getMddConfig();
};

#endif
