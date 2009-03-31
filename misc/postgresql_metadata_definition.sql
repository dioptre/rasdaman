-- These SQL statements will create the metadata database required by WCPS. Each coverage defined here must have a rasdaman collection by the same name in order to work.

-- There are three types of tables.
-- The "static" fixed tables are the ones for which you see "insert" statements in this file. They should generally never be modified. Any modifications of these tables will require to be reflected in the WCPS source code, so a code review must be done if anything is inserted or deleted there. The values of these tables are specified by the WCPS standard.
-- The "service" tables contain various things that are not per-coverage. These are modifiable, but see the comments for each before you change them.
-- The "coverage" tables contain information for each coverage. They can be modified freely, as long as the end result makes sense, as described below.

-- STATIC TABLES. Once again, modifying any of these will break WCPS, destroy your computer, burn your house down, and possibly kill your family.

-- This is a static table, describing the type of axes WCPS knows about. Don't touch it.
create table WCPS_AxisType (
    id serial not null,
    axisType text unique not null,
    primary key (id)
);

insert into WCPS_AxisType (axisType) values ('x');
insert into WCPS_AxisType (axisType) values ('y');
insert into WCPS_AxisType (axisType) values ('z');
insert into WCPS_AxisType (axisType) values ('time');
insert into WCPS_AxisType (axisType) values ('abstract');

-- This is a static table, describing the range datatypes WCPS knows about. Don't touch it.
create table WCPS_Datatype (
    id serial not null,
    datatype text unique not null,
    primary key (id)
);

insert into WCPS_Datatype (datatype) values ('boolean');
insert into WCPS_Datatype (datatype) values ('char');
insert into WCPS_Datatype (datatype) values ('unsigned char');
insert into WCPS_Datatype (datatype) values ('short');
insert into WCPS_Datatype (datatype) values ('unsigned short');
insert into WCPS_Datatype (datatype) values ('int');
insert into WCPS_Datatype (datatype) values ('unsigned int');
insert into WCPS_Datatype (datatype) values ('long');
insert into WCPS_Datatype (datatype) values ('unsigned long');
insert into WCPS_Datatype (datatype) values ('float');
insert into WCPS_Datatype (datatype) values ('double');
insert into WCPS_Datatype (datatype) values ('complex');
insert into WCPS_Datatype (datatype) values ('complex2');

-- This is a static table, describing the interpolation types WCPS knows about. Don't touch it.
create table WCPS_InterpolationType (
    id serial not null,
    interpolationType text unique not null,
    primary key (id)
);

insert into WCPS_InterpolationType (interpolationType) values ('nearest');
insert into WCPS_InterpolationType (interpolationType) values ('linear');
insert into WCPS_InterpolationType (interpolationType) values ('cubic');
insert into WCPS_InterpolationType (interpolationType) values ('quadratic');
insert into WCPS_InterpolationType (interpolationType) values ('none');

-- This is a static table, describing the null resistances WCPS knows about. Don't touch it.
create table WCPS_NullResistance (
    id serial not null,
    nullResistance text unique not null,
    primary key (id)
);

insert into WCPS_NullResistance (nullResistance) values ('full');
insert into WCPS_NullResistance (nullResistance) values ('none');
insert into WCPS_NullResistance (nullResistance) values ('half');
insert into WCPS_NullResistance (nullResistance) values ('other');

-- This is a static table, describing the coordinate reference systems known by WCPS. Don't touch it.
-- Whether it's actually present in the table or not, the CRS called "CRS:1" is always supported. CRS:1 is the image CRS, i.e. addressable by pixels.
-- The current implementation does not support CRSs other than CRS:1, so you should leave this empty until it does.
create table WCPS_Crs (
    id serial not null,
    name text unique not null,
    primary key (id)
);

-- SERVICE TABLES.

-- This table describes the encoding formats known to WCPS, as well as their mappings to mimetypes. WCPS doesn't care about these, but if you add any, make sure that rasdaman can encode in the format specified by 'name', or encoding to that format won't work.
create table WCPS_Format (
    id serial not null,
    name text unique not null,
    mimetype text not null,
    primary key (id)
);

insert into WCPS_Format (name, mimetype) values ('jpg', 'image/jpeg');
insert into WCPS_Format (name, mimetype) values ('jpeg', 'image/jpeg');
insert into WCPS_Format (name, mimetype) values ('png', 'image/png');
insert into WCPS_Format (name, mimetype) values ('tif', 'image/tiff');
insert into WCPS_Format (name, mimetype) values ('tiff', 'image/tiff');

-- COVERAGE TABLES. At this point, you need to have read the beginning of the WCPS standard, which describes the different metadata a coverage may have.
-- When entering a new coverage, it's best to fill the tables in the order listed here, to satisfy foreign key constraints.

-- A coverage must be introduced here.
-- The name must correspond to the rasdaman collection for that data. It must also be present in WCPS_NullSet below.
-- The nullDefault must be the default null value for the coverage. It must match the range type. If the range type is a structure, it must look like this: {c1,c2,c3...} where cn is the nth component of the value.
-- The interpolationTypeDefault is the default interpolation type used in operations that require one, but don't have one specified. It must point to an entry in the static table above.
-- The nullResistanceDefault is the default null resistance used in operations that require one, but don't have one specified. It must point to an entry in the static table above.
create table WCPS_Coverage (
    id serial not null,
    name text unique not null,
    nullDefault text not null,
    interpolationTypeDefault integer not null,
    nullResistanceDefault integer not null,
    primary key (id),
    foreign key (interpolationTypeDefault) references WCPS_InterpolationType (id),
    foreign key (nullResistanceDefault) references WCPS_NullResistance (id)
);

-- Each coverage has a number of axes, called its dimension. Each axis must have an entry here.
-- The coverage is the id of the coverage a given entry describes.
-- The i is the number of axis for that coverage. Axes are ordered, so be careful.
-- The lo is the lowest addressable pixel, usually 0.
-- The hi is the highest addressable pixel, usually the total number of pixels on the axis minus 1.
create table WCPS_CellDomain (
    id serial not null,
    coverage integer not null,
    i integer not null,
    lo integer not null,
    hi integer not null,
    primary key (id),
    unique (coverage, i),
    foreign key (coverage) references WCPS_Coverage (id) on delete cascade
);

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
create table WCPS_Domain (
    id serial not null,
    coverage integer not null,
    i integer not null,
    name text not null,
    type integer not null,
    numLo double precision,
    numHi double precision,
    strLo text,
    strHi text,
    primary key (id),
    unique (coverage, i),
    foreign key (coverage) references WCPS_Coverage (id) on delete cascade,
    foreign key (type) references WCPS_AxisType (id) on delete cascade
);

-- The range is the datatype of the coverage cell values.
-- The coverage is the id of the coverage a given entry describes.
-- The i is the number of the structure component. Because cells can have composite types, you could have multiple entries for each coverage. Entries are ordered.
-- The name is a handle for that component. Names for the components for a RGB coverage, for example, could be "red", "green", and "blue". You can then ask WCPS for the blue channel of a coverage, and it will know what you're talking about.
-- The type is the datatype of the given component. Note that while in principle each component could have a different datatype, having that might cause problems.
create table WCPS_Range (
    id serial not null,
    coverage integer not null,
    i integer not null,
    name text not null,
    type integer not null,
    primary key (id),
    unique (coverage, i),
    foreign key (coverage) references WCPS_Coverage (id) on delete cascade,
    foreign key (type) references WCPS_Datatype (id) on delete cascade
);

-- Each coverage allows a set of interpolation methods. An interpolation method is a pair of an interpolation type and a null resistance. Each coverage is required to have at least one entry in that table, and the defaults in WCPS_Coverage must be present here.
-- The current implementation does not use the contents of this table, but it does ensure that the above constraints are met.
create table WCPS_InterpolationSet (
    id serial not null,
    coverage integer not null,
    interpolationType integer not null,
    nullResistance integer not null,
    primary key (id),
    unique (coverage, interpolationType, nullResistance),
    foreign key (coverage) references WCPS_Coverage (id) on delete cascade,
    foreign key (interpolationType) references wCPS_InterpolationType (id) on delete cascade,
    foreign key (nullResistance) references WCPS_NullResistance (id) on delete cascade
);

-- Each coverage allows a set of null values. If the range is non-composite, a null value is the value of the single component, e.g. "0". If the range is composite, a null value is of the form "{c1,c2,c3}", e.g. "{0,0,0}" for a RGB coverage. Each coverage is required to have at least one entry in that table, and the defaults in WCPS_Coverage must be present here.
-- The current implementation does not use the contents of this table, but it does ensure that the above constraints are met.
create table WCPS_NullSet (
    id serial not null,
    coverage integer not null,
    nullValue text not null,
    primary key (id),
    unique (coverage, nullValue),
    foreign key (coverage) references WCPS_Coverage (id) on delete cascade
);

-- Each axis of a coverage has a set of allowed coordinate reference systems. Here, "CRS:1" is once again assumed for each axis, whether present or not.
-- The current implementation does not use the contents of this table, but it does ensure that the above constraints are met. Because of this, if the WCPS_CrsSet is left empty, this one should be empty too.
create table WCPS_CrsSet (
    id serial not null,
    axis integer not null,
    crs integer not null,
    primary key (id),
    unique (axis, crs),
    foreign key (axis) references WCPS_Domain (id) on delete cascade,
    foreign key (crs) references WCPS_Crs (id) on delete cascade
);
