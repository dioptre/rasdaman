#!/bin/ksh
#constants sections
TEST_TIME=$(date +%d.%b.%Y_%H:%M) 
TEST_DIR=$RMANHOME/rasodmg/test
SYSTEM_INSERT=$TEST_DIR/system_insert
SYSTEM_QUERY=$TEST_DIR/system_query
DEFDIFF=$TEST_DIR/defdiff
QUERYFILE=/tmp/query.ql
LOGFILE=$PWD/$0_log
IMG_DIR=$TEST_DIR/images
INIMGFILE=$IMG_DIR/x1
OUTIMGFILE=$IMG_DIR/out
FAILIMGFILE=$IMG_DIR/fails/out
REALIMGFILE="$OUTIMGFILE"1
set -A CONVERSION Array TIFF JPEG HDF PNG BMP VFF
set -A CONVERSIONEXT raw tif jpg hdf png bmp vff
#AutoCompression not implemented ask Andeas?
COMPRESSION="Array ZLib RLE HaarWavelet DaubechiesWavelet SepZLib \
	SepRLE Daubechies6Wavelet Daubechies8Wavelet Daubechies10Wavelet Daubechies12Wavelet \
	Daubechies14Wavelet Daubechies16Wavelet Daubechies18Wavelet Daubechies20Wavelet \
	LeastAsym8Wavelet LeastAsym10Wavelet LeastAsym12Wavelet LeastAsym14Wavelet \
	LeastAsym16Wavelet LeastAsym18Wavelet LeastAsym20Wavelet Coiflet6Wavelet \
	Coiflet12Wavelet Coiflet18Wavelet Coiflet24Wavelet Coiflet30Wavelet QHaarWavelet"
DEBUG=0
SERVER=$HOSTNAME
DATABASE=RASBASE
USER=rasguest
PASSWD=rasguest
COLLNAME=pipi
SETTYPE=BoolSet
MDDTYPE=BoolImage
MDDDOMAIN=[0:624,0:899]

#function used
error() {
        echo "$1 !"
        exit 1
}

log() {
        echo "$1" >>$LOGFILE
}

print_options() {
echo "RasDaMan (s)erver host ..... $SERVER"
echo "RasDaMan (d)atabase    ..... $DATABASE"
echo "RasDaMan (u)ser        ..... $USER"
echo "Test Colle(c)tion      ..... $COLLNAME"
echo "Test Set T(y)pe        ..... $SETTYPE"
echo "Test MDD (T)ype        ..... $MDDTYPE"
echo "Test MDD D(o)main      ..... $MDDDOMAIN"
echo "Start Con(v)ersion Test"
echo "Start Com(p)ression Test"
echo "(E)xit Test"
}

check_options() {
 option=""
 if(test "$1" = "s"); then
   while(test -z "$option"); do
    echo "RasDaMan Server Host: \c"
    read option
   done
   SERVER=$option
 elif (test "$1" = "d"); then
   while(test -z "$option"); do
    echo "RasDaMan Database: \c"
    read option
   done
   DATABASE=$option
 elif (test "$1" = "u"); then
   while(test -z "$option"); do
    echo "RasDaMan User: \c"
    read option
   done
   USER=$option
   option=""
   while(test -z "$option"); do
    echo "Password: \c"
    read option
   done
   PASSWD=$option
 elif (test "$1" = "c"); then
   while(test -z "$option"); do
    echo "Test Collection: \c"
    read option
   done
   COLLNAME=$option
 elif (test "$1" = "y"); then
   while(test -z "$option"); do
    echo "Test SetType: \c"
    read option
   done
   SETTYPE=$option
 elif (test "$1" = "t"); then
   while(test -z "$option"); do
    echo "MDD Type: \c"
    read option
   done
   MDDTYPE=$option
 elif (test "$1" = "o"); then
   while(test -z "$option"); do
    echo "MDD Domain: \c"
    read option
   done
   MDDDOMAIN=$option
 elif (test "$1" = "p"); then
   echo "All set, get ready for compression test, go!"
   execute_compression_test
 elif (test "$1" = "v"); then
   echo "All set, get ready for conversion test, go!"
   execute_conversion_test
 else
    echo "Please choose a valid option!"
 fi
}




execute_compression_test() {
SELECTQUERY="select a from $COLLNAME  as a"
CREATEQUERY="create collection $COLLNAME $SETTYPE"
DELETEQUERY="delete from $COLLNAME where true"

SYSTEM_INSERT_ARGS="-s $SERVER -u $USER -p $PASSWD --dbname $DATABASE --collection $COLLNAME --mddtype $MDDTYPE --mdddomain $MDDDOMAIN" 
SYSTEM_QUERY_ARGS="-s $SERVER -u $USER -p $PASSWD --dbname $DATABASE" 

#creates collection
rm -rf $LOGFILE
log "Compression Test stated on $TEST_TIME"
log "-creating collection $COLLNAME SETTYPE"
echo "$CREATEQUERY" > $QUERYFILE 
if (test $DEBUG -ne 0); then
 log "$SYSTEM_QUERY $SYSTEM_QUERY_ARGS -r $QUERYFILE"
fi
$SYSTEM_QUERY $SYSTEM_QUERY_ARGS -r $QUERYFILE 1>>$LOGFILE 2>&1
if ( test $? -ne 0 ); then
    #deletes all mdd from collection from current collection
    log "-deleting all image from $COLLNAME collection"
    echo "$DELETEQUERY" > $QUERYFILE 
    if (test $DEBUG -ne 0); then
    	log "$SYSTEM_QUERY $SYSTEM_QUERY_ARGS -r $QUERYFILE"
    fi
    $SYSTEM_QUERY $SYSTEM_QUERY_ARGS -r $QUERYFILE 1>>$LOGFILE 2>&1 
    if ( test $? -ne 0 ); then
      error "An Error occured please check the log file $LOGFILE"
    fi
fi

k=1
informat=${CONVERSION[0]}
informatext=${CONVERSIONEXT[0]}

for storage in $COMPRESSION; do
  for transfer in $COMPRESSION; do
   #insert the data in db
   log "--inserting $INIMGFILE.$informatext with tranfer $transfer mode and storage $storage mode in $COLLNAME"
   if (test $DEBUG -ne 0); then
   	log "$SYSTEM_INSERT $SYSTEM_INSERT_ARGS \n--storageformat $storage --transformat $transfer \
	--inputformat $informat -r $INIMGFILE.$informatext"
   fi
   $SYSTEM_INSERT $SYSTEM_INSERT_ARGS --storageformat $storage --transformat $transfer \
	--inputformat $informat -r $INIMGFILE.$informatext 1>>$LOGFILE 2>&1 
   if ( test $? -eq 0 ); then
        #checks if what is in db is correct compared with DEF input
  	outformat=$informat
	outformatext=$informatext
    	log "---retriving data for tranfer $transfer mode in output $outformat mode"
    	echo "$SELECTQUERY" > $QUERYFILE
    	if (test $DEBUG -ne 0); then
           log "$SYSTEM_QUERY $SYSTEM_QUERY_ARGS --transformat $transfer --outputformat $outformat \
        	--outputfilename $OUTIMGFILE -r $QUERYFILE"
	fi
    	$SYSTEM_QUERY $SYSTEM_QUERY_ARGS --transformat $transfer  --outputformat $outformat \
                --outputfilename $OUTIMGFILE -r $QUERYFILE 1>>$LOGFILE 2>&1
    	if ( test $? -ne 0 ); then
             error "An Error occured please check the log file $LOGFILE"
    	fi

 	#comparing files src.outformatext with realfile.outformatext
	$DEFDIFF --srcfilename $INIMGFILE.$informatext --srcformat $informat \
		--destfilename $REALIMGFILE.$outformatext --destformat $outformat 1>>$LOGFILE 2>&1
	if (test $? -eq 0); then
            echo "$k-DEFIN=$informat TRANSFER=$transfer STORAGE=$storage DEFOUT=$outformat OK"
            log "---$k-DEFIN=$informat TRANSFER=$transfer STORAGE=$storage DEFOUT=$outformat OK"
    	else
            echo "$k-DEFIN=$informat TRANSFER=$transfer STORAGE=$storage DEFOUT=$outformat FAILED"
            log  "---$k-DEFIN=$informat TRANSFER=$transfer STORAGE=$storage DEFOUT=$outformat FAILED"
	    #keep the file for futher analysis
            mv $REALIMGFILE.$outformatext $FAILIMGFILE$k.$outformatext
	fi

	#deletes all mdd from collection
	log "--deleting current image"
	echo "$DELETEQUERY" > $QUERYFILE 
        if (test $DEBUG -ne 0); then
   	   log "$SYSTEM_QUERY $SYSTEM_QUERY_ARGS --transformat $transfer -r $QUERYFILE"
	fi
   	$SYSTEM_QUERY $SYSTEM_QUERY_ARGS --transformat $transfer -r $QUERYFILE 1>>$LOGFILE 2>&1 
	if ( test $? -ne 0 ); then    
	     error "An Error occured please check the log file $LOGFILE"
	fi
   else
     error "An Error occured please check the log file $LOGFILE"
   fi
   k=$(($k+1)) 
  done
done
}

execute_conversion_test() {
SELECTQUERY="select a from $COLLNAME  as a"
CREATEQUERY="create collection $COLLNAME $SETTYPE"
DELETEQUERY="delete from $COLLNAME where true"

SYSTEM_INSERT_ARGS="-s $SERVER -u $USER -p $PASSWD --dbname $DATABASE --collection $COLLNAME --mddtype $MDDTYPE --mdddomain $MDDDOMAIN" 
SYSTEM_QUERY_ARGS="-s $SERVER -u $USER -p $PASSWD --dbname $DATABASE" 

#creates collection
rm -rf $LOGFILE
log "Conversion Test stated on $TEST_TIME"
log "-creating collection $COLLNAME SETTYPE"
echo "$CREATEQUERY" > $QUERYFILE 
if (test $DEBUG -ne 0); then
 log "$SYSTEM_QUERY $SYSTEM_QUERY_ARGS -r $QUERYFILE"
fi
$SYSTEM_QUERY $SYSTEM_QUERY_ARGS -r $QUERYFILE 1>>$LOGFILE 2>&1
if ( test $? -ne 0 ); then
    #deletes all mdd from collection from current collection
    log "-deleting all image from $COLLNAME collection"
    echo "$DELETEQUERY" > $QUERYFILE 
    if (test $DEBUG -ne 0); then
    	log "$SYSTEM_QUERY $SYSTEM_QUERY_ARGS -r $QUERYFILE"
    fi
    $SYSTEM_QUERY $SYSTEM_QUERY_ARGS -r $QUERYFILE 1>>$LOGFILE 2>&1 
    if ( test $? -ne 0 ); then
      error "An Error occured please check the log file $LOGFILE"
    fi
fi

i=0
k=1
n=${#CONVERSION[*]}
transfer=Array
storage=Array

while (test i -lt n); do
   informat=${CONVERSION[$i]}
   informatext=${CONVERSIONEXT[$i]}
   #insert the data in db
   log "--inserting $INIMGFILE.$informatext with tranfer $transfer mode and storage $storage mode in $COLLNAME"
   if (test $DEBUG -ne 0); then
   	log "$SYSTEM_INSERT $SYSTEM_INSERT_ARGS \n--storageformat $storage --transformat $transfer \
	--inputformat $informat -r $INIMGFILE.$informatext"
   fi
   $SYSTEM_INSERT $SYSTEM_INSERT_ARGS --storageformat $storage --transformat $transfer \
	--inputformat $informat -r $INIMGFILE.$informatext 1>>$LOGFILE 2>&1 
   if ( test $? -eq 0 ); then
      #checks if what is in db is correct compared with DEF input
      j=0
      while (test j -lt n); do
    	outformat=${CONVERSION[$j]}
    	outformatext=${CONVERSIONEXT[$j]}
	log "---retriving data for tranfer $transfer mode in output $outformat mode"
	echo "$SELECTQUERY" > $QUERYFILE 
	if (test $DEBUG -ne 0); then
 	   log "$SYSTEM_QUERY $SYSTEM_QUERY_ARGS --transformat $transfer  --outputformat $outformat \
		--outputfilename $OUTIMGFILE -r $QUERYFILE" 
     	fi
	$SYSTEM_QUERY $SYSTEM_QUERY_ARGS --transformat $transfer  --outputformat $outformat \
		--outputfilename $OUTIMGFILE -r $QUERYFILE 1>>$LOGFILE 2>&1 
        if ( test $? -ne 0 ); then    
	     error "An Error occured please check the log file $LOGFILE"
        fi
        #comparing files src.outformatext with realfile.outformatext
	$DEFDIFF --srcfilename $INIMGFILE.$informatext --srcformat $informat \
		--destfilename $REALIMGFILE.$outformatext --destformat $outformat 1>>$LOGFILE 2>&1
	if (test $? -eq 0); then
	   echo "$k-DEFIN=$informat TRANSFER=$transfer STORAGE=$storage DEFOUT=$outformat OK"
	   log "---$k-DEFIN=$informat TRANSFER=$transfer STORAGE=$storage DEFOUT=$outformat OK"
        else
           echo "$k-DEFIN=$informat TRANSFER=$transfer STORAGE=$storage DEFOUT=$outformat FAILED"
	   log  "---$k-DEFIN=$informat TRANSFER=$transfer STORAGE=$storage DEFOUT=$outformat FAILED"
 	   #keep the file for futher analysis
	   mv $REALIMGFILE.$outformatext $FAILIMGFILE$k.$outformatext
	fi
        j=$(($j+1)) 
        k=$(($k+1)) 
      done
      #deletes all mdd from collection
      log "--deleting current image"
      echo "$DELETEQUERY" > $QUERYFILE 
      if (test $DEBUG -ne 0); then
     	log "$SYSTEM_QUERY $SYSTEM_QUERY_ARGS --transformat $transfer -r $QUERYFILE"
      fi
      $SYSTEM_QUERY $SYSTEM_QUERY_ARGS --transformat $transfer -r $QUERYFILE 1>>$LOGFILE 2>&1 
      if ( test $? -ne 0 ); then    
         error "An Error occured please check the log file $LOGFILE"
      fi
   else
     error "An Error occured please check the log file $LOGFILE"
   fi
   i=$(($i+1)) 
done
}

echo "Conversion & Compression Test - Main Menu"

opt="s"
while ( test "$opt" != "e"); do
 print_options
 echo "Change settings/Start Test/Exit?\c"
 read opt
 if (test "$opt" = "E"); then
  opt="e"
 fi
 if (test "$opt" = "e"); then
   echo "Exiting ..."
   exit 0
 else
  check_options $opt
 fi
done

