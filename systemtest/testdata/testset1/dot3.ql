-- Testbed: result_type=set <marray <octet, [0:10,0:10]>>
-- Testbed: result_elements=2

select a * ({1l,2o}+{3l,4o}).1
from   ImgCharA as a
