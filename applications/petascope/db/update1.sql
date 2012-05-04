-- Add coverage type column
ALTER TABLE ps_coverage ADD COLUMN type varchar(30);

UPDATE ps_coverage SET type = 'RectifiedGridCoverage';
