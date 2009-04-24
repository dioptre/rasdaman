-- Testbed: result_type=set <marray <long, [0:10,0:10]>>
-- Testbed: result_elements=4

select (a+2)*(b+1)
from   ImgCharA as a, ImgCharB as b
where  some_cell( (a+2)*(b+1) > 0 )
