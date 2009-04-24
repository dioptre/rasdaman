-- Testbed: result_type=set <marray <char, [0:10,0:10]>>
-- Testbed: result_elements=2

select a.red + a.green + a.blue 
from   ImgRGBA as a
where  some_cell( a.red > 0 )
