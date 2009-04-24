-- Testbed: result_type=set <marray <char, [0:10,0:10]>>
-- Testbed: result_elements=2

select a * b
from   ImgCharA as a, ImgCharB as b
where  some_cell( a*b > 0 )
