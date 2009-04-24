// This test evaluates the effort for internal operations.
// Run on different servers with optimization of operations and 
// multidimensional operations turned on and off.

// no operation

select img[0:799,0:799]
from earth1 as img

// one MDD operation

select img[0:799,0:799] + img[0:799,0:799]
from earth1 as img

// two MDD operations

select img[0:799,0:799] + img[0:799,0:799] + img[0:799,0:799]
from earth1 as img

// three MDD operations

select img[0:799,0:799] + img[0:799,0:799] + img[0:799,0:799] + img[0:799,0:799]
from earth1 as img

// four MDD operations

select img[0:799,0:799] + img[0:799,0:799] + img[0:799,0:799] + img[0:799,0:799] + img[0:799,0:799]
from earth1 as img

// five MDD operations

select img[0:799,0:799] + img[0:799,0:799] + img[0:799,0:799] + img[0:799,0:799] + img[0:799,0:799] + img[0:799,0:799]
from earth1 as img

// no operation

select img[0:799,0:799]
from earth1 as img

// one unary operation

select not(img[0:799,0:799])
from earth1 as img

// no operation

select img[0:799,0:799]
from earth1 as img

// one binary operation with a constant (+)

select img[0:799,0:799] + 10c
from earth1 as img

// two binary operations with a constant (+, *)

select (img[0:799,0:799] + 10c) * 10c
from earth1 as img

// three binary operations with a constant (+, *, -)

select ((img[0:799,0:799] + 10c) * 10c) - 10c
from earth1 as img

// four binary operations with a constant (+, *, -, /)

select (((img[0:799,0:799] + 10c) * 10c) - 10c) / 10c
from earth1 as img
