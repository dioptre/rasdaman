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
SELECT pg_catalog.setval('ps_axistype_id_seq', 6, true);

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

INSERT INTO ps_crs VALUES (9, 'urn:ogc:def:crs:OGC:1.3:CRS84');
INSERT INTO ps_crs VALUES (8, 'CRS:1');
SELECT pg_catalog.setval('ps_crs_id_seq', 9, true);


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

INSERT INTO ps_coverage VALUES (30, 'climate_clouds', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (31, 'climate_earth', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (8, 'Modis', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (29, 'TP', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (27, 'NN3_10', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (2, 'nrg', NULL, '', '{0,0,0}', 5, 2);
INSERT INTO ps_coverage VALUES (28, 'NN3_11', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (32, 'NIR', NULL, NULL, '{0,0,0}', 5, 2);
INSERT INTO ps_coverage VALUES (7, 'Waha_SEGY', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (3, 'mr', NULL, '', '{0,0}', 5, 2);
INSERT INTO ps_coverage VALUES (1, 'rgb', NULL, NULL, '{0,0,0}', 5, 2);
INSERT INTO ps_coverage VALUES (17, 'climate_temperature', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (16, 'whatevereeeee', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (6, 'Boonsville_SEGY', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (4, 'mowglie', NULL, '', '{0,0,0}', 5, 2);
INSERT INTO ps_coverage VALUES (19, 'NN3_2', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (20, 'NN3_3', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (18, 'NN3_1', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (23, 'NN3_6', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (24, 'NN3_7', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (21, 'NN3_4', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (22, 'NN3_5', NULL, NULL, '0', 5, 2);
INSERT INTO ps_coverage VALUES (25, 'NN3_8', NULL, '', '0', 5, 2);
INSERT INTO ps_coverage VALUES (26, 'NN3_9', NULL, NULL, '0', 5, 2);

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

INSERT INTO ps_celldomain VALUES (147, 30, 0, 0, 719);
INSERT INTO ps_celldomain VALUES (148, 30, 1, 0, 360);
INSERT INTO ps_celldomain VALUES (149, 30, 2, 0, 59);
INSERT INTO ps_celldomain VALUES (152, 31, 0, 0, 719);
INSERT INTO ps_celldomain VALUES (153, 31, 1, 0, 360);
INSERT INTO ps_celldomain VALUES (154, 31, 2, 0, 60);
INSERT INTO ps_celldomain VALUES (155, 8, 0, 0, 7);
INSERT INTO ps_celldomain VALUES (156, 8, 1, 0, 1199);
INSERT INTO ps_celldomain VALUES (157, 8, 2, 0, 1199);
INSERT INTO ps_celldomain VALUES (158, 29, 0, 0, 719);
INSERT INTO ps_celldomain VALUES (159, 29, 1, 0, 369);
INSERT INTO ps_celldomain VALUES (160, 29, 2, 0, 30);
INSERT INTO ps_celldomain VALUES (161, 29, 3, 0, 25);
INSERT INTO ps_celldomain VALUES (162, 27, 0, 0, 125);
INSERT INTO ps_celldomain VALUES (163, 2, 0, 0, 1959);
INSERT INTO ps_celldomain VALUES (164, 2, 1, 0, 1075);
INSERT INTO ps_celldomain VALUES (165, 28, 0, 0, 125);
INSERT INTO ps_celldomain VALUES (166, 32, 0, 0, 1915);
INSERT INTO ps_celldomain VALUES (167, 32, 1, 0, 1075);
INSERT INTO ps_celldomain VALUES (168, 7, 0, 0, 201);
INSERT INTO ps_celldomain VALUES (169, 7, 1, 0, 258);
INSERT INTO ps_celldomain VALUES (170, 7, 2, 0, 1000);
INSERT INTO ps_celldomain VALUES (171, 3, 0, 0, 255);
INSERT INTO ps_celldomain VALUES (172, 3, 1, 0, 210);
INSERT INTO ps_celldomain VALUES (173, 1, 0, 0, 399);
INSERT INTO ps_celldomain VALUES (174, 1, 1, 0, 343);
INSERT INTO ps_celldomain VALUES (175, 17, 0, 0, 719);
INSERT INTO ps_celldomain VALUES (176, 17, 1, 0, 360);
INSERT INTO ps_celldomain VALUES (177, 17, 2, 0, 60);
INSERT INTO ps_celldomain VALUES (178, 16, 0, 0, 699);
INSERT INTO ps_celldomain VALUES (179, 6, 0, 0, 134);
INSERT INTO ps_celldomain VALUES (180, 6, 1, 0, 94);
INSERT INTO ps_celldomain VALUES (181, 6, 2, 0, 2000);
INSERT INTO ps_celldomain VALUES (182, 4, 0, 0, 119);
INSERT INTO ps_celldomain VALUES (183, 4, 1, 0, 158);
INSERT INTO ps_celldomain VALUES (184, 4, 2, 0, 118);
INSERT INTO ps_celldomain VALUES (185, 19, 0, 0, 125);
INSERT INTO ps_celldomain VALUES (186, 20, 0, 0, 125);
INSERT INTO ps_celldomain VALUES (187, 18, 0, 0, 125);
INSERT INTO ps_celldomain VALUES (188, 23, 0, 0, 125);
INSERT INTO ps_celldomain VALUES (189, 24, 0, 0, 125);
INSERT INTO ps_celldomain VALUES (190, 21, 0, 0, 125);
INSERT INTO ps_celldomain VALUES (191, 22, 0, 0, 125);
INSERT INTO ps_celldomain VALUES (192, 25, 0, 0, 125);
INSERT INTO ps_celldomain VALUES (193, 26, 0, 0, 125);

SELECT pg_catalog.setval('ps_celldomain_id_seq', 193, true);

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

INSERT INTO ps_domain VALUES (104, 30, 0, 'x', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (105, 30, 1, 'y', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (106, 30, 2, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (109, 31, 0, 'x', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (110, 31, 1, 'y', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (111, 31, 2, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (112, 8, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (113, 8, 1, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (114, 8, 2, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (115, 29, 0, 'x', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (116, 29, 1, 'y', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (117, 29, 2, 'z', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (118, 29, 3, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (119, 27, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (120, 2, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (121, 2, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (122, 28, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (123, 32, 0, 'x', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (124, 32, 1, 'y', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (125, 7, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (126, 7, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (127, 7, 2, 'z', 4, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (128, 3, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (129, 3, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (130, 1, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (131, 1, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (132, 17, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (133, 17, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (134, 17, 2, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (135, 16, 0, 'd0', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (136, 6, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (137, 6, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (138, 6, 2, 'z', 4, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (139, 4, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (140, 4, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (141, 4, 2, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (142, 19, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (143, 20, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (144, 18, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (145, 23, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (146, 24, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (147, 21, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (148, 22, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (149, 25, 0, 't', 5, 0, 1, NULL, NULL);
INSERT INTO ps_domain VALUES (150, 26, 0, 't', 5, 0, 1, NULL, NULL);

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

INSERT INTO ps_range VALUES (165, 30, 0, 'value', 10);
INSERT INTO ps_range VALUES (169, 31, 0, 'value', 10);
INSERT INTO ps_range VALUES (170, 8, 0, 'fire', 2);
INSERT INTO ps_range VALUES (171, 29, 0, 'value', 10);
INSERT INTO ps_range VALUES (172, 27, 0, 'value', 5);
INSERT INTO ps_range VALUES (173, 2, 0, 'nir', 7);
INSERT INTO ps_range VALUES (174, 2, 1, 'red', 7);
INSERT INTO ps_range VALUES (175, 2, 2, 'green', 7);
INSERT INTO ps_range VALUES (176, 28, 0, 'value', 5);
INSERT INTO ps_range VALUES (177, 32, 0, 'red', 7);
INSERT INTO ps_range VALUES (178, 32, 1, 'green', 7);
INSERT INTO ps_range VALUES (179, 32, 2, 'blue', 7);
INSERT INTO ps_range VALUES (180, 7, 0, 'density', 4);
INSERT INTO ps_range VALUES (181, 3, 0, 'green', 7);
INSERT INTO ps_range VALUES (182, 3, 1, 'grey', 7);
INSERT INTO ps_range VALUES (183, 1, 0, 'red', 7);
INSERT INTO ps_range VALUES (184, 1, 1, 'green', 7);
INSERT INTO ps_range VALUES (185, 1, 2, 'blue', 7);
INSERT INTO ps_range VALUES (186, 17, 0, 'temperature', 10);
INSERT INTO ps_range VALUES (187, 16, 0, 'r0', 7);
INSERT INTO ps_range VALUES (188, 6, 0, 'density', 10);
INSERT INTO ps_range VALUES (189, 4, 0, 'red', 7);
INSERT INTO ps_range VALUES (190, 4, 1, 'green', 7);
INSERT INTO ps_range VALUES (191, 4, 2, 'blue', 7);
INSERT INTO ps_range VALUES (192, 19, 0, 'value', 5);
INSERT INTO ps_range VALUES (193, 20, 0, 'value', 5);
INSERT INTO ps_range VALUES (194, 18, 0, 'value', 5);
INSERT INTO ps_range VALUES (195, 23, 0, 'value', 5);
INSERT INTO ps_range VALUES (196, 24, 0, 'value', 5);
INSERT INTO ps_range VALUES (197, 21, 0, 'value', 5);
INSERT INTO ps_range VALUES (198, 22, 0, 'value', 5);
INSERT INTO ps_range VALUES (199, 25, 0, 'value', 5);
INSERT INTO ps_range VALUES (200, 26, 0, 'value', 5);

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

INSERT INTO ps_interpolationset VALUES (69, 30, 5, 2);
INSERT INTO ps_interpolationset VALUES (71, 31, 5, 2);
INSERT INTO ps_interpolationset VALUES (72, 8, 5, 2);
INSERT INTO ps_interpolationset VALUES (73, 29, 5, 2);
INSERT INTO ps_interpolationset VALUES (74, 27, 5, 2);
INSERT INTO ps_interpolationset VALUES (75, 2, 5, 2);
INSERT INTO ps_interpolationset VALUES (76, 28, 5, 2);
INSERT INTO ps_interpolationset VALUES (77, 32, 5, 2);
INSERT INTO ps_interpolationset VALUES (78, 7, 5, 2);
INSERT INTO ps_interpolationset VALUES (79, 3, 5, 2);
INSERT INTO ps_interpolationset VALUES (80, 1, 5, 2);
INSERT INTO ps_interpolationset VALUES (81, 17, 5, 2);
INSERT INTO ps_interpolationset VALUES (82, 16, 5, 2);
INSERT INTO ps_interpolationset VALUES (83, 6, 5, 2);
INSERT INTO ps_interpolationset VALUES (84, 4, 5, 2);
INSERT INTO ps_interpolationset VALUES (85, 19, 5, 2);
INSERT INTO ps_interpolationset VALUES (86, 20, 5, 2);
INSERT INTO ps_interpolationset VALUES (87, 18, 5, 2);
INSERT INTO ps_interpolationset VALUES (88, 23, 5, 2);
INSERT INTO ps_interpolationset VALUES (89, 24, 5, 2);
INSERT INTO ps_interpolationset VALUES (90, 21, 5, 2);
INSERT INTO ps_interpolationset VALUES (91, 22, 5, 2);
INSERT INTO ps_interpolationset VALUES (92, 25, 5, 2);
INSERT INTO ps_interpolationset VALUES (93, 26, 5, 2);

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

INSERT INTO ps_nullset VALUES (69, 30, '0');
INSERT INTO ps_nullset VALUES (71, 31, '0');
INSERT INTO ps_nullset VALUES (72, 8, '0');
INSERT INTO ps_nullset VALUES (73, 29, '0');
INSERT INTO ps_nullset VALUES (74, 27, '0');
INSERT INTO ps_nullset VALUES (75, 2, '{0,0,0}');
INSERT INTO ps_nullset VALUES (76, 28, '0');
INSERT INTO ps_nullset VALUES (77, 32, '{0,0,0}');
INSERT INTO ps_nullset VALUES (78, 7, '0');
INSERT INTO ps_nullset VALUES (79, 3, '{0,0}');
INSERT INTO ps_nullset VALUES (80, 1, '{0,0,0}');
INSERT INTO ps_nullset VALUES (81, 17, '0');
INSERT INTO ps_nullset VALUES (82, 16, '0');
INSERT INTO ps_nullset VALUES (83, 6, '0');
INSERT INTO ps_nullset VALUES (84, 4, '{0,0,0}');
INSERT INTO ps_nullset VALUES (85, 19, '0');
INSERT INTO ps_nullset VALUES (86, 20, '0');
INSERT INTO ps_nullset VALUES (87, 18, '0');
INSERT INTO ps_nullset VALUES (88, 23, '0');
INSERT INTO ps_nullset VALUES (89, 24, '0');
INSERT INTO ps_nullset VALUES (90, 21, '0');
INSERT INTO ps_nullset VALUES (91, 22, '0');
INSERT INTO ps_nullset VALUES (92, 25, '0');
INSERT INTO ps_nullset VALUES (93, 26, '0');

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

INSERT INTO ps_crsset VALUES (40, 104, 8);
INSERT INTO ps_crsset VALUES (41, 105, 8);
INSERT INTO ps_crsset VALUES (42, 106, 8);
INSERT INTO ps_crsset VALUES (45, 109, 8);
INSERT INTO ps_crsset VALUES (46, 110, 8);
INSERT INTO ps_crsset VALUES (47, 111, 8);
INSERT INTO ps_crsset VALUES (48, 112, 8);
INSERT INTO ps_crsset VALUES (49, 113, 8);
INSERT INTO ps_crsset VALUES (50, 114, 8);
INSERT INTO ps_crsset VALUES (51, 115, 8);
INSERT INTO ps_crsset VALUES (52, 116, 8);
INSERT INTO ps_crsset VALUES (53, 117, 8);
INSERT INTO ps_crsset VALUES (54, 118, 8);
INSERT INTO ps_crsset VALUES (55, 119, 8);
INSERT INTO ps_crsset VALUES (56, 120, 8);
INSERT INTO ps_crsset VALUES (57, 121, 8);
INSERT INTO ps_crsset VALUES (58, 122, 8);
INSERT INTO ps_crsset VALUES (59, 123, 8);
INSERT INTO ps_crsset VALUES (60, 124, 8);
INSERT INTO ps_crsset VALUES (61, 125, 8);
INSERT INTO ps_crsset VALUES (62, 126, 8);
INSERT INTO ps_crsset VALUES (63, 127, 8);
INSERT INTO ps_crsset VALUES (64, 128, 8);
INSERT INTO ps_crsset VALUES (65, 129, 8);
INSERT INTO ps_crsset VALUES (66, 130, 8);
INSERT INTO ps_crsset VALUES (67, 131, 8);
INSERT INTO ps_crsset VALUES (68, 132, 8);
INSERT INTO ps_crsset VALUES (69, 133, 8);
INSERT INTO ps_crsset VALUES (70, 134, 8);
INSERT INTO ps_crsset VALUES (71, 135, 8);
INSERT INTO ps_crsset VALUES (72, 136, 8);
INSERT INTO ps_crsset VALUES (73, 137, 8);
INSERT INTO ps_crsset VALUES (74, 138, 8);
INSERT INTO ps_crsset VALUES (75, 139, 8);
INSERT INTO ps_crsset VALUES (76, 140, 8);
INSERT INTO ps_crsset VALUES (77, 141, 8);
INSERT INTO ps_crsset VALUES (78, 142, 8);
INSERT INTO ps_crsset VALUES (79, 143, 8);
INSERT INTO ps_crsset VALUES (80, 144, 8);
INSERT INTO ps_crsset VALUES (81, 145, 8);
INSERT INTO ps_crsset VALUES (82, 146, 8);
INSERT INTO ps_crsset VALUES (83, 147, 8);
INSERT INTO ps_crsset VALUES (84, 148, 8);
INSERT INTO ps_crsset VALUES (85, 149, 8);
INSERT INTO ps_crsset VALUES (86, 150, 8);

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

INSERT INTO ps_descriptions VALUES (10, 30, '', '', '');
INSERT INTO ps_descriptions VALUES (12, 31, '', '', '');
INSERT INTO ps_descriptions VALUES (13, 8, '', '', '');
INSERT INTO ps_descriptions VALUES (14, 29, '', '', '');
INSERT INTO ps_descriptions VALUES (15, 27, '', '', '');
INSERT INTO ps_descriptions VALUES (16, 2, '', '', '');
INSERT INTO ps_descriptions VALUES (17, 28, '', '', '');
INSERT INTO ps_descriptions VALUES (18, 32, '', '', '');
INSERT INTO ps_descriptions VALUES (19, 7, '', '', '');
INSERT INTO ps_descriptions VALUES (20, 3, '', '', '');
INSERT INTO ps_descriptions VALUES (22, 17, '', '', '');
INSERT INTO ps_descriptions VALUES (23, 16, '', '', '');
INSERT INTO ps_descriptions VALUES (24, 6, '', '', '');
INSERT INTO ps_descriptions VALUES (25, 4, '', '', '');
INSERT INTO ps_descriptions VALUES (26, 19, '', '', '');
INSERT INTO ps_descriptions VALUES (27, 20, '', '', '');
INSERT INTO ps_descriptions VALUES (28, 18, '', '', '');
INSERT INTO ps_descriptions VALUES (29, 23, '', '', '');
INSERT INTO ps_descriptions VALUES (30, 24, '', '', '');
INSERT INTO ps_descriptions VALUES (31, 21, '', '', '');
INSERT INTO ps_descriptions VALUES (32, 22, '', '', '');
INSERT INTO ps_descriptions VALUES (33, 25, '', '', '');
INSERT INTO ps_descriptions VALUES (34, 26, '', '', '');
INSERT INTO ps_descriptions VALUES (21, 1, 'Coverage "rgb"', 'Abstract for coverage "rgb"', '{red,green,blue}');

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

INSERT INTO ps_crsdetails VALUES (2, 1, -30.0, 65.0, -35.0, 80.0, NULL, NULL);
