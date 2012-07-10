-- Creates Tables for Non-Raster Coverages: MultiPoint, MultiCurve, MultiSurface, MultiSolid
-- @Creator: Vaibhav Bajpai

-- DROP TABLE ps_multipoint;
-- DROP TABLE ps_multicurve;
-- DROP TABLE ps_multisurface;
-- DROP TABLE ps_multisolid;

--CREATE TABLE ps_multipoint (
--	id serial NOT NULL,
--        coverage integer NOT NULL,
--	x float,
--        y float,
--        z float,
--        w float, 
--        primary key(id),
--        foreign key(coverage) references ps_coverage(id) on delete cascade 
--);

--CREATE TABLE ps_multicurve (
--	id serial NOT NULL,
--        coverage integer NOT NULL,
--        data bytea,
--        primary key(id),
--	foreign key(coverage) references ps_coverage(id) on delete cascade 
--); 

--CREATE TABLE ps_multisurface (
--	id serial NOT NULL,
--        coverage integer NOT NULL,
--        data bytea,
--        primary key(id),
--	foreign key(coverage) references ps_coverage(id) on delete cascade 
--);

--CREATE TABLE ps_multisolid (
--	id serial NOT NULL,
--        coverage integer NOT NULL,
--        data bytea,
--        primary key(id),
--	foreign key(coverage) references ps_coverage(id) on delete cascade 
--); 
