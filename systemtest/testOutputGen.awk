BEGIN                           { testSection = 0; }
# ignore empty lines
/^$/                            { next; }

# ignore oid lines
/^ *Oid\.*:*/                   { next; }

# turn on flag 
/^-- Testbed start block.*/     { testSection = 1; }

# turn off flag 
/^-- Testbed end block.*/       { testSection = 0; }

# print lines in test section
testSection==1                  { print $0; next; }

# print lines starting with -- Testbed
/^-- Testbed .*/                 { print $0; }

