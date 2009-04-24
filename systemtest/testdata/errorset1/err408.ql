-- Testbed: error_no=408

select shift( a, [0,sdom(a)[0].hi] )
from ImgCharA as a
