-- rview-Query
select 

(marray x in [0:6, 0:6, 0:6] 
values (char)(216-x[0]*x[1]*x[2] ) ) 

overlay

(marray x in [0:6, 0:6, 0:6] 
values (char)(x[0]*x[1]*x[2] ) ) 

from ImgCharA as a 