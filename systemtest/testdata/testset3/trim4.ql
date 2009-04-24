-- Testbed: result_type=set <marray <char, [6:10,0:10]>>
-- Testbed: result_elements=2

select a[(5:6).hi : 10, *:*]
from ImgCharA as a

