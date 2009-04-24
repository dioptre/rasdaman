-- Testbed: result_type=set <marray <char, [0:10,4:7]>>
-- Testbed: result_elements=2

select (a[sdom(a)[0],2:8])[*:*,4:sdom(a)[1].hi - 3]
from ImgCharA as a

