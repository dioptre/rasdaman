-- Testbed: result_type=set <marray <char, [2:5,5:8]>>
-- Testbed: result_elements=4

select (a+b)[2:5,5:8]
from   ImgCharA as a, ImgCharB as b
