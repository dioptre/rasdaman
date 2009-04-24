select marray x in [0:1,0:1] values
         condense + over y in [0:6] using a[x[0]*7+y[0],x[1]]*1l
from   OlapCharA as a

