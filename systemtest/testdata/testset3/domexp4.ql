-- Testbed: result_type=set <marray <char, [0:10]>>
-- Testbed: result_elements=2

select a[*:*, (sdom(a)[1].hi-sdom(a)[1].lo)/2 ]
from ImgCharA as a
