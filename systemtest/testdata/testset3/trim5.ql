-- Testbed: result_type=set <marray <char, [0:10,4:6]>>
-- Testbed: result_elements=2

select (a[*:*,2:8])[*:*,4:6]
from ImgCharA as a

