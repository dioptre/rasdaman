select marray n in [0:1] values
         condense + over x in sdom(a) where a[x] = n[0] using 1c
from   ImgCharA as a

