-- Testbed: result_type=set <marray <struct { char red, char green, char blue }, [0:10,0:10]>>
-- Testbed: result_elements=2

select a
from   ImgRGBA as a
where  oid(a) > 0 
