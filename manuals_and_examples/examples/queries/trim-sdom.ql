-- rview-Query
select rgb[ sdom(rgb)[0].hi - 99 : sdom(rgb)[0].hi,
            sdom(rgb)[1].hi - 99 : sdom(rgb)[1].hi ]
from   rgb
