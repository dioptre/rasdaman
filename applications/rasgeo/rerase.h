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
 * Copyright 2003 - 2011 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */

/*
 * Contributed to rasdaman by Alexander Herzig, Landcare Research New Zealand
 */

/*!
 * \brief rerase deletes images and collections from the rasdaman
 *   data base including meta data created by rimport (i.e.
 *   nm_meta information, raster attribute tables, and petascope
 *   meta data)
 */

#ifndef RERASE_H_
#define RERASE_H_
#define ctxRerase "ctxRerase"

/// RASDAMAN includes
#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#ifdef __GNUG__
#include "raslib/template_inst.hh"
#include "rasgeo_template_inst.hh"
#endif
#endif

/// postgresql
#include "libpq-fe.h"

/// RasdamanHelper & RasdamanConnector
#include "RasdamanHelper2.h"
#include "RasdamanConnector.h"

// required variables, if rasdaman is compiled with DEBUG enabled
#ifdef RMANDEBUG
int indentLevel;
bool debugOutput;
#endif

void showEraseHelp();
int erasePSMetaData(RasdamanHelper2& helper);
int eraseNMMetaData(RasdamanHelper2& helper);


#endif /* RERASE_H_ */
