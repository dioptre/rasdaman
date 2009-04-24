-- Testbed: error_no=356

select marray x in [ 0:sdom(a)[0].hi+1, 0:sdom(a)[1].hi ] values a[x]
from   ImgCharA as a
