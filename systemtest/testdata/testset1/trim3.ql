-- Testbed: result_type=set <marray <char, [2:5,5:8]>>
-- Testbed: result_elements=4

select a[2:5,5:8]+b[2:5,5:8]
from   ImgCharA as a, ImgCharB as b
