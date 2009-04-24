-- sum cell values and add result by induction

select a + condense + over x in sdom(a) using a[x] 
from   ImgCharA as a
