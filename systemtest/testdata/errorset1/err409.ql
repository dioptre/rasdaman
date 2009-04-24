-- Testbed: error_no=409

select shift( a, [5,5] )[5:10,*:*]
from ImgCharD as a
