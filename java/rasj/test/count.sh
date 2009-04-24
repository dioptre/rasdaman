#!/bin/bash


awk < $1 '
BEGIN	{ COUNT=0; MIN=1000; MAX=0; SMALL=0; LARGE=0;}
	{ COUNT++;
          if ($6>1000) LARGE++; else SMALL++;
          if ($6>MAX) MAX = $6;
	  if ($6<MIN) MIN = $6;
	}
END	{ printf("count=%d, small=%d, large=%d, min=%d, max=%d, percent=%f\n", COUNT, SMALL, LARGE, MIN, MAX, LARGE/COUNT*100 ); } '
