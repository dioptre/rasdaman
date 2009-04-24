BEGIN                       { paren = 0; sec = 0 }
/\%\%/                      { sec += 1; next; }
sec!=1                      { next; }
# /^ *$/                      { next; }
/\/\// && !paren            { x=$0; sub(/\/\/.*/, "", x); print x; next; }
/{/                         { x=$0; sub(/\{.*/, "", x); if(!paren) print x; paren += 1; }
/(\/\*)/                    { paren += 1; }
/}|(\*\/)/                  { paren -= 1; next; }
!paren                      { print $0; }
