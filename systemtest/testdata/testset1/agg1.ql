-- Testbed: result_type=set <marray <char, [0:10,0:10]>>
-- Testbed: result_elements=1

select a 
from   ImgCharA as a
where  some_cell( a > 0 )
