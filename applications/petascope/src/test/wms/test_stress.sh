#!/bin/bash

# test_stress.sh - flood rasdaman WMS server with requests
#       without parameter: run one request
#       with (any) parameter: run large number of requests concurrently

ME=`basename $0 .sh`

# one item per available server process;
# to really challenge the system, take one more than rasdaman processes available.
SERVERS="1 2 3"

WGET=/usr/bin/wget

# test request
REQUEST='http://localhost:8080/petascope/wms?REQUEST=GetMap&FORMAT=image/jpeg&WIDTH=400&HEIGHT=400&LAYERS=Hakoon_Dive_1&SRS=EPSG:32633&BBOX=489835,7988690,491154,7990009&STYLES=standard&VERSION=1.1.0&TRANSPARENT=false&BGCOLOR=0xff0000'

# be nice, give some seconds rest after $SERVERS attack
SLEEP=3

# where to put images retrieved
IMAGEDIR=Images
mkdir $IMAGEDIR || exit 1

echo $0: stress testing WMS

if [ $# -lt 1 ]
then
        $WGET $REQUEST -O $IMAGEDIR/$ME.jpg > $ME.log 2>&1
else
        for i in 1 2 3 4 5 6 7 8 9 0 a b c d e f g h i j k l m n o p q r s t u v w x y z
        do
                mkdir $IMAGEDIR/$i
                for j in 1 2 3 4 5 6 7 8 9 0 a b c d e f g h i j k l m n o p q r s t u v w x y z
                do
                        for k in 1 2 3 4 5 6 7 8 9 0 a b c d e f g h i j k l m n o p q r s t u v w x y z
                        do
                                for s in $SERVERS
                                do
					echo $WGET $REQUEST -O $IMAGEDIR/$i/$j$k$s.jpg
                                        ( $WGET $REQUEST -O $IMAGEDIR/$i/$j$k$s.jpg >> $ME.log 2>&1) &
                                        echo -n "."
                                done
                                echo -n " | "
                                sleep $SLEEP
                        done
                        echo
                done
                echo "===================================="
        done
        echo
fi

# see how many images are retrieved, i.e., non-XML
TMPFILE=/tmp/$0

ls -lR Images/ > $TMPFILE

IMG=.jpg
XML_SIZE=335
IMG_SIZE=5732

echo `grep $IMG $TMPFILE | grep $IMG_SIZE | wc -l` ok
echo `grep $IMG $TMPFILE | grep $XML_SIZE | wc -l` errors
echo -n `grep $IMG $TMPFILE | wc -l` "total = "
echo $(( `grep $IMG $TMPFILE | grep $IMG_SIZE | wc -l` * 100 / `grep $IMG $TMPFILE | wc -l` )) percent success

echo $0: done.
