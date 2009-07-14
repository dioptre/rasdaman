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

INSERT INTO ps_axistype (id, axistype) VALUES (1, 'x');
INSERT INTO ps_axistype (id, axistype) VALUES (2, 'y');
INSERT INTO ps_axistype (id, axistype) VALUES (3, 'temporal');
INSERT INTO ps_axistype (id, axistype) VALUES (4, 'elevation');
INSERT INTO ps_axistype (id, axistype) VALUES (5, 'other');
SELECT pg_catalog.setval('ps_axistype_id_seq', 5, true);

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
SELECT pg_catalog.setval('ps_datatype_id_seq', 13, true);

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
SELECT pg_catalog.setval('ps_interpolationtype_id_seq', 5, true);

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

SELECT pg_catalog.setval('ps_nullresistance_id_seq', 4, true);

-- This is a static table, describing the coordinate reference systems known by WCPS. Don't touch it.
-- Whether it's actually present in the table or not, the CRS called "CRS:1" is always supported. CRS:1 is the image CRS, i.e. addressable by pixels.
-- The current implementation does not support CRSs other than CRS:1, and does not use the contents of the table.
CREATE TABLE ps_crs (
    id serial NOT NULL,
    name text NOT NULL,
	primary key (id)
);

INSERT INTO ps_crs (id, name) VALUES (5, 'asdf');
INSERT INTO ps_crs (id, name) VALUES (6, 'no crs');
SELECT pg_catalog.setval('ps_crs_id_seq', 6, true);


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

INSERT INTO ps_format (id, name, mimetype) VALUES (1, 'csv', 'text/plain');
INSERT INTO ps_format (id, name, mimetype) VALUES (2, 'jpg', 'image/jpeg');
INSERT INTO ps_format (id, name, mimetype) VALUES (3, 'jpeg', 'image/jpeg');
INSERT INTO ps_format (id, name, mimetype) VALUES (4, 'png', 'image/png');
INSERT INTO ps_format (id, name, mimetype) VALUES (5, 'tif', 'image/tiff');
INSERT INTO ps_format (id, name, mimetype) VALUES (6, 'tiff', 'image/tiff');
INSERT INTO ps_format (id, name, mimetype) VALUES (7, 'raw', 'application/x-octet-stream');

SELECT pg_catalog.setval('ps_format_id_seq', 7, true);


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
    name text NOT NULL,
    nullvalue text,
    crs text,
    nulldefault text,
    interpolationtypedefault integer,
    nullresistancedefault integer,
	primary key (id),
    foreign key (interpolationTypeDefault) references ps_interpolationType (id),
    foreign key (nullResistanceDefault) references ps_nullResistance (id)
);

INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (1, 'rgb', '', '', '{0,0,0}', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (6, 'Boonsville_SEGY', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (7, 'Waha_SEGY', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (3, 'mr', NULL, '', '{0,0}', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (2, 'nrg', NULL, '', '{0,0,0}', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (8, 'Modis', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (4, 'mowglie', NULL, '', '{0,0,0}', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (16, 'whatevereeeee', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (17, 'climate_temperature', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (18, 'NN3_1', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (19, 'NN3_2', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (20, 'NN3_3', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (21, 'NN3_4', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (22, 'NN3_5', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (23, 'NN3_6', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (24, 'NN3_7', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (25, 'NN3_8', NULL, '', '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (26, 'NN3_9', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (27, 'NN3_10', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (28, 'NN3_11', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (29, 'TP', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (30, 'climate_clouds', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (31, 'climate_earth', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage (id, name, nullvalue, crs, nulldefault, interpolationtypedefault, nullresistancedefault) VALUES (32, 'NIR', NULL, NULL, '{0,0,0}', 5, 2);

SELECT pg_catalog.setval('ps_coverage_id_seq', 32, true);


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

INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (1, 1, 0, 0, 399);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (2, 1, 1, 0, 343);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (3, 2, 0, 0, 1959);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (4, 2, 1, 0, 1075);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (5, 3, 0, 0, 255);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (6, 3, 1, 0, 210);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (12, 5, 2, 0, 1950);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (13, 6, 0, 0, 134);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (16, 6, 1, 0, 94);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (17, 6, 2, 0, 2000);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (18, 7, 0, 0, 201);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (19, 7, 1, 0, 258);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (20, 7, 2, 0, 1000);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (21, 8, 0, 0, 7);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (22, 8, 1, 0, 1199);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (23, 8, 2, 0, 1199);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (7, 4, 0, 0, 119);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (8, 4, 1, 0, 158);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (9, 4, 2, 0, 118);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (28, 16, 0, 0, 699);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (29, 17, 0, 0, 719);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (30, 17, 1, 0, 360);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (31, 17, 2, 0, 60);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (32, 18, 0, 0, 125);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (33, 19, 0, 0, 125);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (34, 20, 0, 0, 125);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (35, 21, 0, 0, 125);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (36, 22, 0, 0, 125);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (37, 23, 0, 0, 125);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (38, 24, 0, 0, 125);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (39, 25, 0, 0, 125);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (40, 26, 0, 0, 125);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (41, 27, 0, 0, 125);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (42, 28, 0, 0, 125);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (43, 29, 0, 0, 719);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (44, 29, 1, 0, 369);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (46, 29, 2, 0, 30);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (47, 29, 3, 0, 25);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (48, 30, 0, 0, 719);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (49, 30, 1, 0, 360);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (50, 30, 2, 0, 59);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (51, 31, 0, 0, 719);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (52, 31, 1, 0, 360);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (53, 31, 2, 0, 60);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (54, 32, 0, 0, 1915);
INSERT INTO ps_celldomain (id, coverage, i, lo, hi) VALUES (55, 32, 1, 0, 1075);

SELECT pg_catalog.setval('ps_celldomain_id_seq', 55, true);

-- Addressing by pixels is good enough for some things, but sometimes you need to address a coverage via its geo coordinates. The geographic extent of each coverage is similar to the cell domain, but in geo coordinates.
-- For each coverage, the number of entries must be the same as in the CellDomain table.
-- The coverage is the id of the coverage a given entry describes.
-- The i is the number of axis for that coverage. Axes are ordered, so be careful.
-- The name is the name of a given axis. For example, a horizontal axis could have the name "x", and the a vertical one could have "y". You can then ask WCPS to scale x by 2, etc.
-- The type is one of the axes types in the static table above.
-- If the type is not temporal, numLo and numHi must be the lowest and highest addressable points in geo coordinates, and strLo and strHi must be left null.
-- If the type is temporal, numLo and numHi must be left null, and strLo and strHi must be timestamps, specifying the extent.
-- Because WCPS currently does not support geo coordinates, you can use dummy values for numLo and numHi, e.g. 0 and 1.
-- Because the current implementation does not currently support temporal axes, you can use "other" as the type and specify dummy values for numLo and numHi.
-- Fanally, geocoordinates are dependent on the CRS, so this table doesn't currently make much sense. An entry should be per coverage, axis, and CRS, rather than just per coverage and axis as it is now.
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

INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (1, 1, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (2, 1, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (3, 2, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (4, 2, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (5, 3, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (6, 3, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (8, 4, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (9, 4, 2, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (12, 5, 2, 'z', 4, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (13, 6, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (14, 6, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (15, 6, 2, 'z', 4, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (16, 7, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (17, 7, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (18, 7, 2, 'z', 4, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (20, 8, 1, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (21, 8, 2, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (22, 8, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (28, 16, 0, 'd0', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (7, 4, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (29, 17, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (30, 17, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (31, 17, 2, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (32, 18, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (33, 19, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (34, 20, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (35, 21, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (36, 22, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (37, 23, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (38, 24, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (39, 25, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (40, 26, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (41, 27, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (42, 28, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (43, 29, 0, 'x', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (44, 29, 1, 'y', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (45, 29, 2, 'z', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (46, 29, 3, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (47, 30, 0, 'x', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (48, 30, 1, 'y', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (49, 30, 2, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (50, 31, 0, 'x', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (52, 31, 1, 'y', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (53, 31, 2, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (54, 32, 0, 'x', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (55, 32, 1, 'y', 5, 0, 1, NULL, NULL);

SELECT pg_catalog.setval('ps_domain_id_seq', 55, true);

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

INSERT INTO ps_range (id, coverage, i, name, type) VALUES (1, 1, 0, 'red', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (2, 1, 1, 'green', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (3, 1, 2, 'blue', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (4, 2, 0, 'nir', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (5, 2, 1, 'red', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (6, 2, 2, 'green', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (7, 3, 0, 'green', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (8, 3, 1, 'grey', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (13, 6, 0, 'density', 10);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (14, 7, 0, 'density', 4);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (15, 8, 0, 'fire', 2);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (9, 4, 0, 'red', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (10, 4, 1, 'green', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (11, 4, 2, 'blue', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (20, 16, 0, 'r0', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (21, 17, 0, 'temperature', 10);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (22, 18, 0, 'value', 5);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (23, 19, 0, 'value', 5);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (24, 20, 0, 'value', 5);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (25, 21, 0, 'value', 5);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (26, 22, 0, 'value', 5);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (27, 23, 0, 'value', 5);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (28, 24, 0, 'value', 5);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (29, 26, 0, 'value', 5);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (30, 27, 0, 'value', 5);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (31, 28, 0, 'value', 5);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (32, 25, 0, 'value', 5);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (44, 29, 0, 'value', 10);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (45, 30, 0, 'value', 10);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (46, 31, 0, 'value', 10);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (47, 32, 0, 'red', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (48, 32, 1, 'green', 7);
INSERT INTO ps_range (id, coverage, i, name, type) VALUES (49, 32, 2, 'blue', 7);

SELECT pg_catalog.setval('ps_range_id_seq', 49, true);


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

INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (1, 1, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (2, 2, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (3, 3, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (6, 6, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (7, 7, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (8, 8, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (4, 4, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (10, 16, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (11, 17, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (12, 18, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (13, 19, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (14, 20, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (15, 21, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (16, 22, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (17, 23, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (18, 24, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (19, 25, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (20, 26, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (21, 27, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (22, 28, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (23, 29, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (24, 30, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (25, 31, 5, 2);
INSERT INTO ps_interpolationset (id, coverage, interpolationtype, nullresistance) VALUES (26, 32, 5, 2);

SELECT pg_catalog.setval('ps_interpolationset_id_seq', 26, true);


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

INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (6, 6, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (7, 7, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (3, 3, '{0,0}');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (1, 1, '{0,0,0}');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (2, 2, '{0,0,0}');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (8, 8, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (4, 4, '{0,0,0}');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (10, 16, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (11, 17, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (12, 18, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (13, 19, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (14, 20, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (15, 21, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (16, 22, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (17, 23, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (18, 24, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (19, 25, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (20, 26, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (21, 27, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (22, 28, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (23, 29, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (24, 30, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (26, 31, '0');
INSERT INTO ps_nullset (id, coverage, nullvalue) VALUES (27, 32, '{0,0,0}');

SELECT pg_catalog.setval('ps_nullset_id_seq', 27, true);


-- Each axis of a coverage has a set of allowed coordinate reference systems. Here, "CRS:1" is once again assumed for each axis, whether present or not.
-- The current implementation does not use the contents of this table, but it does ensure that the above constraints are met. Because of this, if the PS_CrsSet is left empty, this one should be empty too.
CREATE TABLE ps_crsset (
    id serial NOT NULL,
    axis integer NOT NULL,
    crs integer NOT NULL,
	primary key (id),
	unique (axis, crs),
    foreign key (axis) references ps_domain (id) on delete cascade,
    foreign key (crs) references ps_crs (id) on delete cascade
);

INSERT INTO ps_crsset (id, axis, crs) VALUES (1, 28, 6);
INSERT INTO ps_crsset (id, axis, crs) VALUES (2, 18, 5);

SELECT pg_catalog.setval('ps_crsset_id_seq', 2, true);

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

