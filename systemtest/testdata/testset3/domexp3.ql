-- Testbed: result_type=set <marray <char, [0:10,0:5]>>
-- Testbed: result_elements=2

select a[*:*, *:sdom(a)[1].hi/2]
from ImgCharA as a
