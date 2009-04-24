-- all_cell( a and b ) -> all_cell( a ) and all_cell( b )

SELECT a+b   
FROM   ImgCharA as a, ImgCharB as b 
WHERE  all_cell ( a > 0 and b > 10 ) 


