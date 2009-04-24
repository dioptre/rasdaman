-- Testbed: result_type=set <marray <char, [-2:2,-1:1]>>
-- Testbed: result_elements=1

select a + < [-2:2,-1:1] 1c, 2c, 3c; 1c, 2c, 3c; 1c, 2c, 3c; 1c, 2c, 3c; 1c, 2c, 3c >
from   ImgCharMask as a
