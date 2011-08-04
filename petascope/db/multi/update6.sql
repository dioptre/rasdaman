-- insert mr2 metadata
INSERT INTO ps_coverage(id,name,nulldefault,interpolationtypedefault,nullresistancedefault,type) VALUES (100, 'mr2', '0', 5, 2, 'GridCoverage');

INSERT INTO ps_celldomain(coverage, i, lo, hi) VALUES (100, 0, 0, 255);
INSERT INTO ps_celldomain(coverage, i, lo, hi) VALUES (100, 1, 0, 210);

INSERT INTO ps_domain(coverage,i,name,type,numlo,numhi,strlo,strhi) VALUES (100, 0, 'x', 1, 0, 1, NULL, NULL);
INSERT INTO ps_domain(coverage,i,name,type,numlo,numhi,strlo,strhi) VALUES (100, 1, 'y', 2, 0, 1, NULL, NULL);

INSERT INTO ps_range(coverage,i,name,type) VALUES (100, 0, 'grey', 7);

INSERT INTO ps_interpolationset(coverage,interpolationtype,nullresistance) VALUES (100, 5, 2);
INSERT INTO ps_nullset(coverage,nullvalue) VALUES (100, '0');

INSERT INTO ps_descriptions VALUES (100, 100, 'Coverage "mr2"', 'Abstract for coverage "mr2"', '{gray}');
