// This test evaluates performance of binary operations on parts 
// of tomo. Selectivity fixed at 10%

// cubed, no operation

select img[0:117,0:117,0:69]
from tomo_cubed as img

// cubed, one MDD operation

select img[0:117,0:117,0:69] + img[0:117,0:117,0:69]
from tomo_cubed as img

// cubed, two MDD operations

select img[0:117,0:117,0:69] + img[0:117,0:117,0:69] + img[0:117,0:117,0:69]
from tomo_cubed as img

// cubed, three MDD operations

select img[0:117,0:117,0:69] + img[0:117,0:117,0:69] + img[0:117,0:117,0:69] + img[0:117,0:117,0:69]
from tomo_cubed as img

// cubed, four MDD operations

select img[0:117,0:117,0:69] + img[0:117,0:117,0:69] + img[0:117,0:117,0:69] + img[0:117,0:117,0:69] + img[0:117,0:117,0:69]
from tomo_cubed as img
