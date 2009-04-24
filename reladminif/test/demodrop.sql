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
--
-- $Header: /home/rasdev/CVS-repository/rasdaman/reladminif/test/demodrop.sql,v 1.3 2003/12/27 23:11:46 rasdev Exp $
-- Copyright (c) Oracle Corporation 1988, 1993.  All Rights Reserved.
--
--
--   This script drops the SQL*Plus demonstration tables.
--
--   It should be STARTed by each owner of the tables.

set termout on
prompt Dropping demonstration tables.  Please wait.
set termout off

DROP TABLE EMP;
DROP TABLE DEPT;
DROP TABLE BONUS;
DROP TABLE SALGRADE;
DROP TABLE DUMMY;

EXIT;
