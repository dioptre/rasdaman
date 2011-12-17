-- unit of measures, used by ps_range and ps_domain
-- either uom or link should be specified
CREATE TABLE ps_uom (
  id serial NOT NULL,
  uom text UNIQUE,
  link text UNIQUE,
	primary key (id)
);


-- Add uom code column
ALTER TABLE ps_range ADD COLUMN uom integer;
ALTER TABLE ps_range ADD CONSTRAINT uom_fk FOREIGN KEY (uom) REFERENCES ps_uom (id) ON UPDATE NO ACTION ON DELETE CASCADE;

-- Add uom column
ALTER TABLE ps_domain ADD COLUMN uom integer;
ALTER TABLE ps_domain ADD CONSTRAINT uom_fk FOREIGN KEY (uom) REFERENCES ps_uom (id) ON UPDATE NO ACTION ON DELETE CASCADE;
