select marray n in [0:1] values
         condense + over x in sdom(a) using (a[x] = n[0])*1c
from   ImgCharA as a
