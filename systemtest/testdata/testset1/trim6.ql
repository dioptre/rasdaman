-- Testbed: result_type=set <marray <char, [0:10]>>
-- Testbed: result_elements=4

select (a * b).green [5,*:*]
from   ImgRGBA as a, ImgRGBA as b
