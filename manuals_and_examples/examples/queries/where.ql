-- rview-Query
select mr
from mr
where some_cells( mr[ 120:160, 55:75 ] > 110 )
