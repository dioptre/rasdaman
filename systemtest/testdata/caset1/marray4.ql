-- mirror mdd objects with marray

select marray x in sdom(a) values a[ sdom(a)[0].hi-x[0], x[1] ] 
from   ImgULongC as a
