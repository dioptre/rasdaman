select marray x in [0:6,0:1] values
          condense + over y in [0:1] using (a[x[0]+y[0]*7,x[1]])
from   OlapCharA as a

