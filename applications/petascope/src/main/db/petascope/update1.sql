-- Add coverage type column
ALTER TABLE ps_coverage ADD COLUMN type varchar(30);

-- UPDATE ps_coverage SET type = 'GridCoverage' WHERE id = 1;
-- UPDATE ps_coverage SET type = 'RectifiedGridCoverage' WHERE id = 2;
