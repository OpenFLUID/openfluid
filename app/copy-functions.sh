#!/bin/sh


#$1 = path to dirs to get function list
#$2 = binary functions source dir
#$3 = binary functions extension
#$4 = dest dir

FILELIST=`ls -1 $1 | awk '{ ORS=" "; print; }'`
FILELIST=${FILELIST%% } # trim left

for item in $FILELIST
do
  cp  $2/$item.$3 $4/$item.$3
done


