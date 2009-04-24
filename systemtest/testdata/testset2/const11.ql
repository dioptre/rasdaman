-- Testbed: result_type=set <marray <struct { long 0, float 1, long 2 }, [-2:2,-1:1]>>
-- Testbed: result_elements=1

select a + { -1, 1.0, 1 }
from   ImgCharMask as a
