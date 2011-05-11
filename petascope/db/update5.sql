-- Insert Metadata for test_multipoint 
-- @Creator: Vaibhav Bajpai


-- Each coverage has a number of axes, called its dimension. Each axis must have an entry here.
-- The coverage is the id of the coverage a given entry describes.
-- The i is the number of axis for that coverage. Axes are ordered, so be careful.
-- The lo is the lowest addressable pixel, usually 0.
-- The hi is the highest addressable pixel, usually the total number of pixels on the axis minus 1.

INSERT INTO ps_celldomain (coverage, i, lo, hi) VALUES (33, 0, -4000, 4000);    -- FIXME: lo, hi
INSERT INTO ps_celldomain (coverage, i, lo, hi) VALUES (33, 1, -4000, 4000);    -- FIXME: lo, hi
INSERT INTO ps_celldomain (coverage, i, lo, hi) VALUES (33, 2, -4000, 4000);    -- FIXME: lo, hi
INSERT INTO ps_celldomain (coverage, i, lo, hi) VALUES (33, 3, -4000, 4000);    -- FIXME: lo, hi

 
-- The range is the datatype of the coverage cell values.
-- The coverage is the id of the coverage a given entry describes.
-- The i is the number of the structure component. Because cells can have composite types, you could have multiple entries for each coverage. Entries are ordered.
-- The name is a handle for that component. Names for the components for a RGB coverage, for example, could be "red", "green", and "blue".
-- You can then ask WCPS for the blue channel of a coverage, and it will know what you're talking about.
-- The type is the datatype of the given component. Note that while in principle each component could have a different datatype, having that might cause problems.

INSERT INTO ps_range (coverage, i, name, type) VALUES (33, 0, 'red', 7);
INSERT INTO ps_range (coverage, i, name, type) VALUES (33, 1, 'green', 7);
INSERT INTO ps_range (coverage, i, name, type) VALUES (33, 2, 'blue', 7);


-- Each coverage allows a set of interpolation methods. 
-- An interpolation method is a pair of an interpolation type and a null resistance.
-- Each coverage is required to have at least one entry in that table, and the defaults in PS_Coverage must be present here.
-- The current implementation does not use the contents of this table, but it does ensure that the above constraints are met.

INSERT INTO ps_interpolationset (coverage, interpolationtype, nullresistance) VALUES (33, 5, 2);


-- Each coverage allows a set of null values. If the range is non-composite, a null value is the value of the single component, e.g. "0".
-- If the range is composite, a null value is of the form "{c1,c2,c3}", e.g. "{0,0,0}" for a RGB coverage.
-- Each coverage is required to have at least one entry in that table, and the defaults in PS_Coverage must be present here.
-- The current implementation does not use the contents of this table, but it does ensure that the above constraints are met.

INSERT INTO ps_nullset (coverage, nullvalue) VALUES (33, '{0,0,0}');

-- Addressing by pixels is good enough for some things, but sometimes you need to address a coverage via its geo coordinates. 
-- The geographic extent of each coverage is similar to the cell domain, but in geo coordinates.
-- For each coverage, the number of entries must be the same as in the CellDomain table.
-- The coverage is the id of the coverage a given entry describes.
-- The i is the number of axis for that coverage. Axes are ordered, so be careful.
-- The name is the name of a given axis. 
-- For example, a horizontal axis could have the name "x", and the a vertical one could have "y". You can then ask WCPS to scale x by 2, etc.
-- The type is one of the axes types in the static table above.
-- If the type is not temporal, numLo and numHi must be the lowest and highest addressable points in geo coordinates, and strLo and strHi must be left null.
-- If the type is temporal, numLo and numHi must be left null, and strLo and strHi must be timestamps, specifying the extent.
-- Because WCPS currently does not support geo coordinates, you can use dummy values for numLo and numHi, e.g. 0 and 1.
-- Because the current implementation does not currently support temporal axes, you can use "other" as the type and specify dummy values for numLo and numHi.
-- Fanally, geocoordinates are dependent on the CRS, so this table doesn't currently make much sense.
-- An entry should be per coverage, axis, and CRS, rather than just per coverage and axis as it is now.


INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (151, 33, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (152, 33, 1, 'y', 2, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (153, 33, 2, 'z', 4, 0, 1, NULL, NULL);
INSERT INTO ps_domain (id, coverage, i, name, type, numlo, numhi, strlo, strhi) VALUES (154, 33, 3, 'w', 5, 0, 1, NULL, NULL);

-- Each axis of a coverage has a set of allowed coordinate reference systems. Here, "CRS:1" is once again assumed for each axis, whether present or not.
-- The current implementation does not use the contents of this table, but it does ensure that the above constraints are met.
-- Because of this, if the PS_CrsSet is left empty, this one should be empty too.

INSERT INTO ps_crsset (axis, crs) VALUES (151, 8);
INSERT INTO ps_crsset (axis, crs) VALUES (152, 8);
INSERT INTO ps_crsset (axis, crs) VALUES (153, 8);
INSERT INTO ps_crsset (axis, crs) VALUES (154, 8);


-- This table contains textual descriptions of the available coverages.

INSERT INTO ps_descriptions (coverage, title, abstract, keywords) VALUES (33, '', '', '');

-- This table contains metadata for CRS transformations: WGS84 bounding box limits and offsets for the X and Y axis.


INSERT INTO ps_crsdetails (coverage, low1, high1, low2, high2, offset1, offset2) VALUES (33, -4000, 4000, -4000, 4000, 0, 0);  -- FIX ME

 
