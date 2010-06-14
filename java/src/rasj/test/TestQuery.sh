#!/bin/bash

# TestQuery.sh: test rasj query interface
# where rasj.jar is expected:
export RASJ=$RMANBASE/java/rasj.jar
#export RASJ=~rasdaman/jlib/rasj.jar

java -DRMANPROTOCOL=RNP -cp .:$RASJ:$CLASSPATH TestQuery --query "SELECT jpeg(( marray x in [0:599,0:599] values {255c,255c,0c} ) overlay scale(img0[2011:2315,2543:2847],[1:600,1:600]) * { 1c, 1c, 1c}) FROM vat_rgbdop2_8 AS img0" $* 2>&1 

