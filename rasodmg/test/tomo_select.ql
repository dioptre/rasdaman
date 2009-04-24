// This test evaluates different selectivities in tomo stored both
// in slices and cubes.

// sliced, selectivity 0,5%

select img[0:43,0:43,0:25]
from tomo_sliced as img

// sliced, selectivity 1%

select img[0:55,0:55,0:32]
from tomo_sliced as img

// sliced, selectivity 2%

select img[0:68,0:68,0:41]
from tomo_sliced as img

// sliced, selectivity 5%

select img[0:91,0:91,0:54]
from tomo_sliced as img

// sliced, selectivity 10%

select img[0:117,0:117,0:69]
from tomo_sliced as img

// sliced, selectivity 50%

select img[0:201,0:201,0:121]
from tomo_sliced as img

// sliced, selectivity 100%

select img[0:255,0:255,0:153]
from tomo_sliced as img

// cubed, selectivity 0,5%

select img[0:43,0:43,0:25]
from tomo_cubed as img

// cubed, selectivity 1%

select img[0:55,0:55,0:32]
from tomo_cubed as img

// cubed, selectivity 2%

select img[0:68,0:68,0:41]
from tomo_cubed as img

// cubed, selectivity 5%

select img[0:91,0:91,0:54]
from tomo_cubed as img

// cubed, selectivity 10%

select img[0:117,0:117,0:69]
from tomo_cubed as img

// cubed, selectivity 50%

select img[0:201,0:201,0:121]
from tomo_cubed as img

// cubed, selectivity 100%

select img[0:255,0:255,0:153]
from tomo_cubed as img
