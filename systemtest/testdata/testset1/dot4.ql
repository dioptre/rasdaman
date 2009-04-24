-- Testbed: result_type=set <marray <long, [0:10,0:10]>>
-- Testbed: result_elements=2

select a * ({ 1l, { 8l, 10l, 10l }}.1).2
from   ImgCharA as a
