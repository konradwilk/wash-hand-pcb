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
     echo "script ./release.scr"
     eagle $DIR/${name}.brd
     convert -flip -rotate 180 $DIR/b-negative.png $DIR/b-flip-negative.png
     convert -negate $DIR/t-negative.png $rev/t-$rev.png
     convert -negate $DIR/b-flip-negative.png $rev/b-$rev.png
     convert -negate $DIR/h-negative.bmp $rev/h-$rev.bmp
     potrace -s -r 300 $rev/h-$rev.bmp -o $rev/h-$rev.svg
     convert $rev/b-$rev.png -resize 3300x2550 -units 'PixelsPerInch' -density 300 $DIR/4.pdf
     convert $rev/t-$rev.png -resize 3300x2550 -units 'PixelsPerInch' -density 300 $DIR/5.pdf

     rm -f $DIR/t-negative.png
     rm -f $DIR/b-negative.png
     rm -f $DIR/b-flip-negative.png
     rm -f $DIR/h-$rev.bmp
     rm -f $DIR/h-negative.bmp
     pdfunite $DIR/1.pdf $DIR/2.pdf $DIR/3.pdf $DIR/4.pdf $DIR/5.pdf $name.pdf
     rm $DIR/?.pdf
     echo "Run CAM"
     cp release.cam $rev/
     sed -i "s/#NAME/$name/" $rev/release.cam
     sed -i "s/#REV/$rev/" $rev/release.cam
     eagle -X -dCAMJOB -j$rev/release.cam  -o"$(pwd)/$rev" $DIR/${name}.brd
     if [ $? -eq 0 ]; then
       (cd $rev;zip -9r ${name}_${rev}_${time}.zip CAMOutputs)
       cp BOM.csv $rev/BOM.csv
     fi
     ./create_pnp.py
     git add $rev/*.csv
     git add $rev/*.png
     git add $rev/*.svg
     git add $name.pdf
     git add $DIR/${name}.brd
     git add $DIR/${name}.sch
     rev=`echo $rev | sed s/v//`
     git commit -s -m "RELEASE-${rev}"
}

case "$1" in
    get)
       FILE=$2
       if [ -z "$FILE" ]; then
           FILE=Hardware/Wash_Hand.sch
       fi
       shift 1
       get_revision
       ;;
    release)
       FILE=$2
       if [ -z "$FILE" ]; then
           FILE=Hardware/Wash_Hand.sch
       fi
       shift 1
       release
       ;;
    set)
       FILE=$2
       if [ ! -e "$FILE" ]; then
           FILE=Hardware/Wash_Hand.sch
       fi
       REV=$3
       if [ -z "$REV" ]; then
           REV=$2
       fi
       shift 2
       set_revision
       ;;
esac
