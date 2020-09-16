#!/bin/bash

FILE=""
REV=""

function extract_revision()
{
   local file=$1
   cat $1 | grep attribute | grep REVISION  | grep -v layer | grep -v cons | sed s/.*value=\"// | sed s/\".*// | tail -1
}

function get_revision()
{
   if [ -e "$FILE" ]; then
      REV=$(extract_revision $FILE)
      echo "$REV"
   else
      echo "$0 file.sch"
   fi
}

function set_revision()
{
    local rev=$(get_revision)
    local DIR=`dirname $FILE`
    local name=`basename $FILE .sch`

    if [ -z "$rev" ]; then
       echo "No current rev!"
       exit 1
    fi
    if [ -z "$REV" ]; then
       echo "No new rev!"
       exit 2
    fi
    if [ ! -e "$DIR/${name}.sch" ]; then
       echo "$DIR/${name}.sch does not exist."
       echo "Did you use the sch file?"
       exit 3
    fi
    if [ ! -e "$DIR/${name}.brd" ]; then
       echo "${DIR}/${name}.brd does not exist!"
       exit 4
    fi
    echo "$rev -> $REV"
    while true; do
      read -p "Yes/No?" yn
      case $yn in
        [Yy]* ) break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no.";;
      esac
    done
    echo "Modifying $name.brd"
    sed -i "s/$rev/$REV/" $DIR/${name}.brd
    echo "Modifying $name.sch"
    sed -i "s/$rev/$REV/" $DIR/${name}.sch
}

function release()
{
     local DIR=`dirname $FILE`
     local rev=$(get_revision)
     local name=`basename $FILE .sch`
     local time=`date +%F`

     if [ -z "$rev" ]; then
        echo "No current rev!"
        exit 1
     fi
     if [ ! -e "$DIR/${name}.sch" ]; then
        echo "$DIR/${name}.sch does not exist."
        echo "Did you use the sch file?"
        exit 3
     fi
     if [ ! -e "$DIR/${name}.brd" ]; then
        echo "${DIR}/${name}.brd does not exist!"
        exit 4
     fi
     mkdir $rev
     if [ $? -eq 1 ]; then
        echo "$rev exists. Aborting."
        exit 5
     fi
     echo "In case it does not work type script and select release.scr"
     eagle -S`pwd`/release.scr $DIR/${name}.brd
     convert -flip -rotate 180 $DIR/b-negative.png $DIR/b-flip-negative.png
     convert -negate $DIR/t-negative.png $rev/t-$rev.png
     convert -negate $DIR/b-flip-negative.png $rev/b-$rev.png
     rm -f $DIR/t-negative.png
     rm -f $DIR/b-negative.png
     rm -f $DIR/b-flip-negative.png
     echo "Run CAM"
     cp release.cam $rev/
     sed -i "s/#NAME/$name/" $rev/release.cam
     sed -i "s/#REV/$rev/" $rev/release.cam
     eagle -X -dCAMJOB -j$rev/release.cam  -o"$(pwd)/$rev" $DIR/${name}.brd
     if [ $? -eq 0 ]; then
       (cd $rev;zip -9r ${name}_${rev}_${time}.zip CAMOutputs)
       cp -f $rev/CAMOutputs/Assembly/${name}.txt $rev/BOM.txt
     fi
}

case "$1" in
    get)
       FILE=$2
       shift 1
       get_revision
       ;;
    release)
       FILE=$2
       shift 1
       release
       ;;
    set)
       FILE=$2
       REV=$3
       shift 2
       set_revision
       ;;
esac
