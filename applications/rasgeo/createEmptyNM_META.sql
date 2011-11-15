-- This file is part of rasdaman community.
--
-- Rasdaman community is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- Rasdaman community is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
--
-- Copyright 2003 - 2011 Peter Baumann / rasdaman GmbH.
--
-- For more information please see <http://www.rasdaman.org>
-- or contact Peter Baumann via <baumann@rasdaman.com>.
-- 
--
-- Contributed to rasdaman by Alexander Herzig, Landcare Research New Zealand
--
-- ---------------------------------------------------------------
--
-- purpose: creation of the nm_meta metadata table
--          for describing CRS-related attributes 
--					of a rasdaman collections
--
--			The metadata stored in this table are partly
--			redundant to the metadata stored in  
--          petascope ps_* tables of the WCPS reference
--          implementation. However, it stores as well reference
--          information for attribute tables associated with
--          certain images
--
-- ---------------------------------------------------------------

-- creation of the nm_meta table
CREATE TABLE nm_meta (
	img_id int unique NOT NULL,
	coll_name character varying(254) NOT NULL,
	epsg_code int,
	crs_name text,
	minx double precision,
	maxx double precision,
	miny double precision,
	maxy double precision,
	minz double precision,
	maxz double precision,
	cellsize_x double precision,
	cellsize_y double precision,
	cellsize_z double precision,
	pixeltype character varying(50),
	stats_min double precision,
	stats_max double precision,
	stats_mean double precision,
	stats_stddev double precision,
	attrtable_name character varying(64),
	primary key (img_id)
);
	
	
