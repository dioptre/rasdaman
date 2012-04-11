---------------------------
-- Create tables for WMS --
---------------------------

-- WMS Service 
-- NOTE: petascope.wms.DatabaseConfigAdapter.java#67: --       
--	log.warn("Finding service...");	// NB: Only 1 service supported currently!
--	Table_Service serviceInfo = new Table_Service();
--	...
create table ps_services
(	serviceId	 SERIAL NOT NULL UNIQUE,
	contactPerson	 varchar(200),
	contactOrganization  varchar(200),
	addressType      varchar(200),
	address          varchar(200),
	city             varchar(200),
	stateorprovince  varchar(200),
	postcode         varchar(200),
	country          varchar(200),
	contactvoicetelephone   varchar(200),
	contactfacsimiletelephone varchar(200),
	contactelectronicmailaddress varchar(200),
	updateSequence	 varchar(100),
	availability	 int NOT NULL,
	name		 varchar(200) NOT NULL,
	title		 varchar(200) NOT NULL,
	abstract	 varchar(100),
	keywords	 varchar(100),
	fees		 varchar(100),
	accessConstraints	 varchar(300),
	hostName	 varchar(200) NOT NULL,
	port		 int NOT NULL,
	path		 varchar(200) NOT NULL,
	formats		 varchar(200) NOT NULL,
	baseLayerName	 varchar(200),
	vendorCapabilities	 varchar(200),
	PRIMARY KEY ( serviceId ),
	UNIQUE( hostName, port, path ),
	CHECK( length(hostname) != 0 AND port > 0 AND length(formats) != 0 )
);

-- Layer of a WMS service
create table ps_layers
(	layerId		 SERIAL NOT NULL UNIQUE,
	name		 varchar(200),
	title		 varchar(200),
	srs		 varchar(200) NOT NULL,
	authority	 varchar(200),
	latLonBoxXmin	 double precision, latLonBoxXmax	 double precision, latLonBoxYmin	 double precision, latLonBoxYmax	 double precision,
	bBoxXmin	 double precision, bBoxXmax	 double precision, bBoxYmin	 double precision, bBoxYmax	 double precision,
	attributionURL	 varchar(300),
	attributionTitle	 varchar(200),
	logoWidth	 int,
	logoHeight	 int,
	logoFormat	 varchar(200),
	logoURL		 varchar(300),
	featureURL	 varchar(300),
	resolution	 double precision,
	maptype		 varchar(200),
	PRIMARY KEY ( layerId ),
	CHECK( length(srs) != 0
	   AND logoWidth >= 0 AND logoHeight >= 0
	   AND latLonBoxXmin < latLonBoxXmax AND latLonBoxYmin < latLonBoxYmax
	   AND bBoxXmin < bBoxXmax AND bBoxYmin < bBoxYmax )
);

create table ps_serviceLayer
(	serviceId        int NOT NULL,
	layerId          int NOT NULL,
	layerSeq         int NOT NULL,
	FOREIGN KEY ( serviceId ) REFERENCES PS_Services,
	FOREIGN KEY ( layerId ) REFERENCES PS_Layers,
	UNIQUE( serviceId, layerId ),
	CHECK( layerSeq >= 0 )
);

-- A layer can be associated with N styles.
-- Must be compatible with a RGB set, e.g.:
-- Negative filter: name = 'negative', rasqlOp = '{255c,255c,255c} - negative*{1c,1c,1c}'
create table ps_styles
(	styleId		 SERIAL NOT NULL,
	layerId		 int NOT NULL,
	name		 varchar(200) NOT NULL,
	title		 varchar(200) NOT NULL,
	abstract	 varchar( 4096 ),
	legendWidth	 int,
	legendHeight	 int,
	legendURL	 varchar(300),
	sheetURL	 varchar(300),
	rasqlOp		 text,
	PRIMARY KEY ( styleId, layerId ),
	FOREIGN KEY ( layerId ) REFERENCES PS_Layers,
	UNIQUE( layerId, name ),
	CHECK( length(name) != 0
	   AND legendWidth >= 0 AND legendHeight >= 0
	   )
);

-- A WMS layer is usually made up of a pyramid of images
-- One actual collection in Rasdaman for each scale step.
create table ps_pyramidLevels
(	levelId		 SERIAL NOT NULL,
	layerId		 int NOT NULL,
	collectionName	 varchar(200) NOT NULL,
	scaleFactor	 float NOT NULL,
	PRIMARY KEY( levelId ), 
	FOREIGN KEY ( layerId ) REFERENCES PS_Layers,
	CHECK( scaleFactor >= 1
	   AND length(collectionName) != 0 )
);
