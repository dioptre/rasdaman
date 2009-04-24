#! /bin/gawk -f
##
## This AWK-script generates some example queries, when applied on "testcases",
## as command line argument. When you need to add new test cases, edit the 
## file "testcases". Each line in this file should be of the form:
##  
## filename @ query @ -- comment @ -- comment
##
## For example the line:
##
## mm.ql @ select 2- -1 from ImgCharA as a @ -- caution: -- introduces comments.
##
## should generate a test file named mm.ql with the folowing content:
##
## --  caution: -- introduces comments.
##
## select 2- -1 from ImgCharA as a
##
## The char @ is somewhat unusual for SQL and I chose it as field separator.

BEGIN {FS = "@"} ; 

{ 
    for (i = 3; i < NF; i++) {
	printf "%s\n", $i > $1
    }
    printf "\n" >> $1
    printf "%s\n", $2 >> $1
}
