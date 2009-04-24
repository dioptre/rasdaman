-- Testbed: error_no=413

select marray n in [0:1] values
         condense + over x in sdom(a) where 1c using 1c
from   ImgCharA as a

