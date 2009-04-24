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
 * SOURCE: rasmgr_protocol.hh
 *
 * MODULE: rasmgr
 * CLASS:  none
 *
 * PURPOSE:
 * Centralize all keywords of the rasmgr c/s protocol
 *    
 * COMMENTS:
 * - to be completed
 *
*/

#ifndef RASMGR_PROTOCOL_HH
#define RASMGR_PROTOCOL_HH

#define RASMGRCMD_HELLO		"Hello"

#define RASMGRCMD_LICENCE	"licence"
#define RASMGRCMD_LICENSE	"license"

#define RASMGRCMD_EXIT		"exit"
#define RASMGRCMD_QUIT		"quit"
#define RASMGRCMD_BYE		"bye"

#define RASMGRCMD_HELP		"help"
#define RASMGRCMD_USER		"user"
#define RASMGRCMD_HOST		"host"
#define RASMGRCMD_SRV		"srv"
#define RASMGRCMD_DATABASE	"database"
#define RASMGRCMD_LIST		"list"
#define RASMGRCMD_DEFINE	"define"
#define RASMGRCMD_REMOVE	"remove"
#define RASMGRCMD_CHECK		"check"
#define RASMGRCMD_CHANGE	"change"
#define RASMGRCMD_SAVE		"save"
#define RASMGRCMD_RESET		"reset"
#define RASMGRCMD_GRANT		"grant"
#define RASMGRCMD_REVOKE	"revoke"

#define RASMGRCMD_UP		"up"
#define RASMGRCMD_DOWN		"down"


#endif // RASMGR_PROTOCOL_HH
