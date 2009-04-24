-- Testbed: result_type=set <marray <char, [0:5,0:10]>>
-- Testbed: result_elements=2

select a[0:5,sdom(a)[1]]
from ImgCharA as a
