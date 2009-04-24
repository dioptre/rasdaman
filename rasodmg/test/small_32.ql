// Selection of images out of a collection with 15 800x800x8bit 
// images. There is one image each with only 255, 254, 253 as a
// value. And there is one image each with one value each out of
// 250, 251, 252. No other image contains value greater than 249.

// one img with some_cell

SELECT img[0:252,0:252]
FROM   earth_32 as img
WHERE  some_cell ( img[700:799,300:399] = 252c )

// two imgs with some_cell

SELECT img[0:252,0:252]
FROM   earth_32 as img
WHERE  some_cell ( img[700:799,300:399] = 252c OR img[700:799,300:399] = 251c )

// three imgs with some_cell

SELECT img[0:252,0:252]
FROM   earth_32 as img
WHERE  some_cell ( img[700:799,300:399] = 252c OR img[700:799,300:399] = 251c 
                                               OR img[700:799,300:399] = 250c )

// one img with all_cell

SELECT img[0:252,0:252]
FROM   earth_32 as img
WHERE  all_cell ( img[700:799,300:399] >= 255c )

// two imgs with all_cell

SELECT img[0:252,0:252]
FROM   earth_32 as img
WHERE  all_cell ( img[700:799,300:399] >= 254c )

// three imgs with all_cell

SELECT img[0:252,0:252]
FROM   earth_32 as img
WHERE  all_cell ( img[700:799,300:399] >= 253c )

