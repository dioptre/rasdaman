-- fix the name of the EPSG:4326 CRS
update ps_crs set name = 'http://www.opengis.net/def/crs/EPSG/0/4326' where id = 9;