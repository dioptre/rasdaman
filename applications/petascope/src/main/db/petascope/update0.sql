-- These SQL statements will create the metadata database required by WCPS. Each coverage defined here must have a rasdaman collection by the same name in order to work.

-- There are three types of tables.
-- The "static" fixed tables are the ones for which you see "insert" statements in this file. They should generally never be modified. Any modifications of these tables will require to be reflected in the WCPS source code, so a code review must be done if anything is inserted or deleted there. The values of these tables are specified by the WCPS standard.
-- The "service" tables contain various things that are not per-coverage. These are modifiable, but see the comments for each before you change them.
-- The "coverage" tables contain information for each coverage. They can be modified freely, as long as the end result makes sense, as described below.

-----------------------------------------------------------------------------------------
-- STATIC TABLES. Once again, modifying any of these will break WCPS.
-----------------------------------------------------------------------------------------

-- This is a static table, describing the type of axes WCPS knows about. Don't touch it.
CREATE TABLE ps_axistype (
  id serial NOT NULL,
  axistype character varying(9) UNIQUE NOT NULL,
	primary key (id)
);

INSERT INTO ps_axistype VALUES (1, 'x');
INSERT INTO ps_axistype VALUES (2, 'y');
INSERT INTO ps_axistype VALUES (3, 'temporal');
INSERT INTO ps_axistype VALUES (4, 'elevation');
INSERT INTO ps_axistype VALUES (5, 'other');
INSERT INTO ps_axistype VALUES (6, 't');
-- SELECT pg_catalog.setval('ps_axistype_id_seq', 6, true);

-- This is a static table, describing the range datatypes WCPS knows about. Don't touch it.
CREATE TABLE ps_datatype (
  id serial NOT NULL,
  datatype text NOT NULL,
	primary key (id)
);

INSERT INTO ps_datatype (id, datatype) VALUES (1, 'boolean');
INSERT INTO ps_datatype (id, datatype) VALUES (2, 'char');
INSERT INTO ps_datatype (id, datatype) VALUES (3, 'unsigned char');
INSERT INTO ps_datatype (id, datatype) VALUES (4, 'short');
INSERT INTO ps_datatype (id, datatype) VALUES (5, 'unsigned short');
INSERT INTO ps_datatype (id, datatype) VALUES (6, 'int');
INSERT INTO ps_datatype (id, datatype) VALUES (7, 'unsigned int');
INSERT INTO ps_datatype (id, datatype) VALUES (8, 'long');
INSERT INTO ps_datatype (id, datatype) VALUES (9, 'unsigned long');
INSERT INTO ps_datatype (id, datatype) VALUES (10, 'float');
INSERT INTO ps_datatype (id, datatype) VALUES (11, 'double');
INSERT INTO ps_datatype (id, datatype) VALUES (12, 'complex');
INSERT INTO ps_datatype (id, datatype) VALUES (13, 'complex2');
-- SELECT pg_catalog.setval('ps_datatype_id_seq', 13, true);

-- This is a static table, describing the interpolation types WCPS knows about. Don't touch it.
CREATE TABLE ps_interpolationtype (
  id serial NOT NULL,
  interpolationtype text NOT NULL,
	primary key (id)
);

INSERT INTO ps_interpolationtype (id, interpolationtype) VALUES (1, 'nearest');
INSERT INTO ps_interpolationtype (id, interpolationtype) VALUES (2, 'linear');
INSERT INTO ps_interpolationtype (id, interpolationtype) VALUES (3, 'cubic');
INSERT INTO ps_interpolationtype (id, interpolationtype) VALUES (4, 'quadratic');
INSERT INTO ps_interpolationtype (id, interpolationtype) VALUES (5, 'none');
-- SELECT pg_catalog.setval('ps_interpolationtype_id_seq', 5, true);

-- This is a static table, describing the null resistances WCPS knows about. Don't touch it.
CREATE TABLE ps_nullresistance (
  id serial NOT NULL,
  nullresistance text NOT NULL,
	primary key (id)
);

INSERT INTO ps_nullresistance (id, nullresistance) VALUES (1, 'full');
INSERT INTO ps_nullresistance (id, nullresistance) VALUES (2, 'none');
INSERT INTO ps_nullresistance (id, nullresistance) VALUES (3, 'half');
INSERT INTO ps_nullresistance (id, nullresistance) VALUES (4, 'other');
-- SELECT pg_catalog.setval('ps_nullresistance_id_seq', 4, true);

-- This is a static table, describing the coordinate reference systems known by WCPS. Don't touch it.
-- Whether it's actually present in the table or not, the CRS called "CRS:1" is always supported. CRS:1 is the image CRS, i.e. addressable by pixels.
-- The current implementation does not support CRSs other than CRS:1, and does not use the contents of the table.
CREATE TABLE ps_crs (
  id serial NOT NULL,
  name text UNIQUE NOT NULL,
	primary key (id)
);

INSERT INTO ps_crs VALUES (9, 'http://www.opengis.net/def/crs/EPSG/0/4326');
INSERT INTO ps_crs VALUES (8, 'CRS:1');
-- SELECT pg_catalog.setval('ps_crs_id_seq', 9, true);


-----------------------------------------------------------------------------------------
-- SERVICE TABLES.
-----------------------------------------------------------------------------------------

-- This table describes the encoding formats known to WCPS, as well as their mappings to mimetypes. WCPS doesn't care about these, but if you add any, make sure that rasdaman can encode in the format specified by 'name', or encoding to that format won't work.
CREATE TABLE ps_format (
  id serial NOT NULL,
  name character varying(64) NOT NULL,
  mimetype character varying(128) NOT NULL,
	primary key (id)
);

INSERT INTO ps_format VALUES (1, 'csv', 'text/plain');
INSERT INTO ps_format VALUES (2, 'jpg', 'image/jpeg');
INSERT INTO ps_format VALUES (3, 'jpeg', 'image/jpeg');
INSERT INTO ps_format VALUES (4, 'png', 'image/png');
INSERT INTO ps_format VALUES (5, 'tif', 'image/tiff');
INSERT INTO ps_format VALUES (6, 'tiff', 'image/tiff');
INSERT INTO ps_format VALUES (7, 'raw', 'application/x-octet-stream');

-- SELECT pg_catalog.setval('ps_format_id_seq', 7, true);


-----------------------------------------------------------------------------------------
-- COVERAGE TABLES. At this point, you need to have read the beginning of the WCPS standard, which describes the different metadata a coverage may have.
-- When entering a new coverage, it's best to fill the tables in the order listed here, to satisfy foreign key constraints.
-----------------------------------------------------------------------------------------

-- A coverage must be introduced here.
-- The name must correspond to the rasdaman collection for that data. It must also be present in PS_NullSet below.
-- The nullDefault must be the default null value for the coverage. It must match the range type. If the range type is a structure, it must look like this: {c1,c2,c3...} where cn is the nth component of the value.
-- The interpolationTypeDefault is the default interpolation type used in operations that require one, but don't have one specified. It must point to an entry in the static table above.
-- The nullResistanceDefault is the default null resistance used in operations that require one, but don't have one specified. It must point to an entry in the static table above.
CREATE TABLE ps_coverage (
  id serial NOT NULL,
  name text UNIQUE NOT NULL,
  nullvalue text,
  crs text,
  nulldefault text,
  interpolationtypedefault integer,
  nullresistancedefault integer,
	primary key (id),
  foreign key (interpolationTypeDefault) references ps_interpolationType (id),
  foreign key (nullResistanceDefault) references ps_nullResistance (id)
);

-- INSERT INTO ps_coverage VALUES (1, 'NIR', NULL, NULL, '{0,0,0}', 5, 2);
-- INSERT INTO ps_coverage VALUES (2, 'mean_summer_airtemp', NULL, NULL, '0', 5, 2);

-- SELECT pg_catalog.setval('ps_coverage_id_seq', 2, true);


-- Each coverage has a number of axes, called its dimension. Each axis must have an entry here.
-- The coverage is the id of the coverage a given entry describes.
-- The i is the number of axis for that coverage. Axes are ordered, so be careful.
-- The lo is the lowest addressable pixel, usually 0.
-- The hi is the highest addressable pixel, usually the total number of pixels on the axis minus 1.
CREATE TABLE ps_celldomain (
  id serial NOT NULL,
  coverage integer NOT NULL,
  i integer NOT NULL,
  lo integer NOT NULL,
  hi integer NOT NULL,
	primary key (id),
	unique (coverage, i),
	foreign key (coverage) references ps_coverage (id) on delete cascade
);

-- INSERT INTO ps_celldomain VALUES (1, 1, 0, 0, 1915);
-- INSERT INTO ps_celldomain VALUES (2, 1, 1, 0, 1075);
-- INSERT INTO ps_celldomain VALUES (3, 2, 0, 0, 885);
-- INSERT INTO ps_celldomain VALUES (4, 2, 1, 0, 710);

-- SELECT pg_catalog.setval('ps_celldomain_id_seq', 4, true);

-- Addressing by pixels is good enough for some things, but sometimes you need to address a coverage via its geo coordinates. The geographic extent of each coverage is similar to the cell domain, but in geo coordinates.
-- For each coverage, the number of entries must be the same as in the CellDomain table.
-- The coverage is the id of the coverage a given entry describes.
-- The i is the number of axis for that coverage. Axes are ordered, so be careful.
-- The name is the name of a given axis. For example, a horizontal axis could have the name "x", and the a vertical one could have "y". You can then ask WCPS to scale x by 2, etc.
-- The type is one of the axes types in the static table above.
-- If the type is not temporal, numLo and numHi must be the lowest and highest addressable points in geo coordinates, and strLo and strHi must be left null.
-- If the type is temporal, numLo and numHi must be left null, and strLo and strHi must be timestamps, specifying the extent.
-- Because the current implementation does not currently support temporal axes, you can use "other" as the type and specify dummy values for numLo and numHi.
-- Finally, geocoordinates are dependent on the CRS, so this table doesn't currently make much sense. An entry should be per coverage, axis, and CRS, rather than just per coverage and axis as it is now.
CREATE TABLE ps_domain (
  id serial NOT NULL,
  coverage integer NOT NULL,
  i integer NOT NULL,
  name text NOT NULL,
  type integer NOT NULL,
  numlo double precision,
  numhi double precision,
  strlo text,
  strhi text,
	primary key (id),
	unique (coverage, i),
  foreign key (coverage) references ps_coverage (id) on delete cascade,
  foreign key (type) references ps_axisType (id) on delete cascade
);

-- INSERT INTO ps_domain VALUES (1, 1, 0, 'x', 1, 0, 1, NULL, NULL);
-- INSERT INTO ps_domain VALUES (2, 1, 1, 'y', 2, 0, 1, NULL, NULL);
-- INSERT INTO ps_domain VALUES (3, 2, 0, 'x', 1, 111.975, 156.275, NULL, NULL);
-- INSERT INTO ps_domain VALUES (4, 2, 1, 'y', 2, -44.525, -8.975, NULL, NULL);
-- SELECT pg_catalog.setval('ps_domain_id_seq', 4, true);

-- The range is the datatype of the coverage cell values.
-- The coverage is the id of the coverage a given entry describes.
-- The i is the number of the structure component. Because cells can have composite types, you could have multiple entries for each coverage. Entries are ordered.
-- The name is a handle for that component. Names for the components for a RGB coverage, for example, could be "red", "green", and "blue". You can then ask WCPS for the blue channel of a coverage, and it will know what you're talking about.
-- The type is the datatype of the given component. Note that while in principle each component could have a different datatype, having that might cause problems.
CREATE TABLE ps_range (
  id serial NOT NULL,
  coverage integer NOT NULL,
  i integer NOT NULL,
  name text NOT NULL,
  type integer NOT NULL,
	primary key (id),
	unique (coverage, i),
  foreign key (coverage) references ps_coverage (id) on delete cascade,
  foreign key (type) references ps_dataType (id) on delete cascade
);

-- INSERT INTO ps_range VALUES (1, 1, 0, 'red', 7);
-- INSERT INTO ps_range VALUES (2, 1, 1, 'green', 7);
-- INSERT INTO ps_range VALUES (3, 1, 2, 'blue', 7);
-- INSERT INTO ps_range VALUES (4, 2, 0, 'pan', 7);

-- SELECT pg_catalog.setval('ps_range_id_seq', 4, true);


-- Each coverage allows a set of interpolation methods. An interpolation method is a pair of an interpolation type and a null resistance. Each coverage is required to have at least one entry in that table, and the defaults in PS_Coverage must be present here.
-- The current implementation does not use the contents of this table, but it does ensure that the above constraints are met.
CREATE TABLE ps_interpolationset (
  id serial NOT NULL,
  coverage integer NOT NULL,
  interpolationtype integer NOT NULL,
  nullresistance integer NOT NULL,
	primary key (id),
	unique (coverage, interpolationType, nullResistance),
	foreign key (coverage) references ps_coverage (id) on delete cascade,
  foreign key (interpolationType) references ps_interpolationType (id) on delete cascade,
  foreign key (nullResistance) references ps_nullResistance (id) on delete cascade
);

-- INSERT INTO ps_interpolationset VALUES (1, 1, 5, 2);
-- INSERT INTO ps_interpolationset VALUES (2, 2, 5, 2);

-- SELECT pg_catalog.setval('ps_interpolationset_id_seq', 1, true);


-- Each coverage allows a set of null values. If the range is non-composite, a null value is the value of the single component, e.g. "0". If the range is composite, a null value is of the form "{c1,c2,c3}", e.g. "{0,0,0}" for a RGB coverage. Each coverage is required to have at least one entry in that table, and the defaults in PS_Coverage must be present here.
-- The current implementation does not use the contents of this table, but it does ensure that the above constraints are met.
CREATE TABLE ps_nullset (
  id serial NOT NULL,
  coverage integer NOT NULL,
  nullvalue text NOT NULL,
	primary key (id),
	unique (coverage, nullValue),
  foreign key (coverage) references ps_coverage (id) on delete cascade
);

-- INSERT INTO ps_nullset VALUES (1, 1, '{0,0,0}');
-- INSERT INTO ps_nullset VALUES (2, 2, '0');

-- SELECT pg_catalog.setval('ps_nullset_id_seq', 1, true);


-- Each axis of a coverage has a set of allowed coordinate reference systems.
-- CRS:1 is used for non-georeferenced coverages.
CREATE TABLE ps_crsset (
  id serial NOT NULL,
  axis integer NOT NULL,
  crs integer NOT NULL,
	primary key (id),
	unique (axis, crs),
  foreign key (axis) references ps_domain (id) on delete cascade,
  foreign key (crs) references ps_crs (id) on delete cascade
);

-- INSERT INTO ps_crsset VALUES (1, 1, 8);
-- INSERT INTO ps_crsset VALUES (2, 2, 8);
-- INSERT INTO ps_crsset VALUES (3, 3, 9);
-- INSERT INTO ps_crsset VALUES (4, 4, 9);

-- SELECT pg_catalog.setval('ps_crsset_id_seq', 4, true);

-- This table contains textual descriptions of the available coverages.
CREATE TABLE ps_descriptions (
    id serial NOT NULL,
    coverage integer NOT NULL,
	title text NOT NULL,
    abstract text,
    keywords text,
    primary key (id),
    unique (coverage),
    foreign key (coverage) references ps_coverage (id) on delete cascade
);

-- INSERT INTO ps_descriptions VALUES (1, 1, 'NIR', 'Abstract for coverage NIR', 'keywords');
-- INSERT INTO ps_descriptions VALUES (2, 2, 'mean_summer_airtemp', 'This coverage shows the mean summer air temperatures over Australia in the period 2001-2007', 'summer temperature, australia');

-- This table contains metadata for CRS transformations: WGS84 bounding box limits and offsets for the X and Y axis.
CREATE TABLE ps_crsdetails (
	id serial NOT NULL,
	coverage integer NOT NULL,
	low1 float NOT NULL,
	high1 float NOT NULL,
	low2 float NOT NULL,
	high2 float NOT NULL,
	offset1 float,
	offset2 float,
	unique (coverage),
	foreign key (coverage) references ps_coverage (id) on delete cascade
);

-- INSERT INTO ps_crsdetails VALUES (1, 1, 0, 1, 0, 1, NULL, NULL);
-- INSERT INTO ps_crsdetails VALUES (2, 2, 111.975, 156.275, -44.525, -8.975, NULL, NULL);
