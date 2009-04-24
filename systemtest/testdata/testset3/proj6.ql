-- Testbed: result_type=set <marray <ulong, [0:10,0:10]>>
-- Testbed: result_elements=2

select a[5,0] * b
from   ImgULongC as a, ImgCharA as b
