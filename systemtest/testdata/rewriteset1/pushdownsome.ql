-- some_cell( a or b ) -> some_cell( a ) or some_cell( b )

SELECT a+b   
FROM   ImgCharA as a, ImgCharB as b 
WHERE  some_cell ( a > 0 OR b > 10 ) 


