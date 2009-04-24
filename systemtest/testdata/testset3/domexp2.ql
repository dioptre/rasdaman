-- Testbed: result_type=set <marray <char, [0:10,0:9]>>
-- Testbed: result_elements=2

select a [sdom(a)[0], *:sdom(a)[1].hi - 1]
from ImgCharA as a

