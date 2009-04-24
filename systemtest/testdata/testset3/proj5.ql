-- Testbed: result_type=set <marray <char, [0:10]>>
-- Testbed: result_elements=2

select (a[*:*,2:8])[*:*,5]
from ImgCharA as a

