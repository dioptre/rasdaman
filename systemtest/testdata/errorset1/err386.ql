-- Testbed: error_no=386

select a
from   ImgCharA as a
where  oid(a) = <"test50|this should no be a valid base|1"> 
