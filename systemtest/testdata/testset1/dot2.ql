-- Testbed: result_type=set <marray <float, [0:10,0:10]>>
-- Testbed: result_elements=2

select a * ({ 10l, { 8ul, 10f, 10.0 }}.1).2
from   ImgCharA as a
