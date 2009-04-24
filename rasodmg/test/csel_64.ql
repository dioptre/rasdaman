// This query tests different selectivities a query box starting at
// the origin on comp_cubed_64.

// [0.5] cubed_64, selectivity 0,5%

select img[0:44,0:44,0:26]
from comp_cubed_64 as img

// [1] cubed_64, selectivity 1%

select img[0:55,0:55,0:33]
from comp_cubed_64 as img

// [2] cubed_64, selectivity 2%

select img[0:69,0:69,0:42]
from comp_cubed_64 as img

// [5] cubed_64, selectivity 5%

select img[0:94,0:94,0:56]
from comp_cubed_64 as img

// [10] cubed_64, selectivity 10%

select img[0:118,0:118,0:71]
from comp_cubed_64 as img

// [20] cubed_64, selectivity 0%

select img[0:149,0:149,0:89]
from comp_cubed_64 as img

// [50] cubed_64, selectivity 50%

select img[0:202,0:202,0:121]
from comp_cubed_64 as img

// [100] cubed_64, selectivity 100%

select img[0:255,0:255,0:153]
from comp_cubed_64 as img

