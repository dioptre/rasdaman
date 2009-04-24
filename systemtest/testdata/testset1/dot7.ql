select ( a.green > 0 ) * { 255c, 0c, 0c } + ( a.red > 0 ) * { 0c, 127c, 0c }
from   ImgRGBA as a
