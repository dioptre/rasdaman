BEGIN                       { paren = 0; sec = 0; comm = 0 }
/^ *$/                      { next; }
/\%\%/                      { sec += 1; next; }
sec!=1                      { next; }
/^\/\*$/                    { comm = 1; next; } 
/^\*\//                     { comm = 0; next; }
comm == 1                   { next; }
/^\/\*.*\*\//               { next; }
/{/                         { x=$0; sub(/\{.*/, "", x); if(!paren) print x; paren += 1; }
/}/                         { paren -= 1; next; }
!paren                      { print $0; }
