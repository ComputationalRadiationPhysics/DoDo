#!/bin/bash

# make non-matching patterns to an empty list
shopt -s nullglob

# DIR will be the directory where this script is stored
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE"  ]; do # resolve $SOURCE until the file is no longer a symlink
    DIR="$( cd -P "$( dirname "$SOURCE"  )" && pwd  )"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /*  ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE"  )" && pwd  )"



LICENSEPATH=$DIR/autoHeaderLicense.txt

function createIncludesFile(){
    # we are inside current
    current="$1"
    includefile="../$(basename $current).hpp"

    rm -f $includefile
    OLDIFS=$IFS

    if [ -f $LICENSEPATH ] ; then
        while IFS= read LINE
        do
            echo "$LINE" >> $includefile
        done < $LICENSEPATH
    fi

    IFS=$OLDIFS

    echo -e "
/**
 * This is an auto-generated file
 */\n" >> $includefile


    echo "#pragma once" >> $includefile
    echo "" >> $includefile

    for header in *.hpp ; do
        echo "#include <${current}${header}>" >> $includefile               
    done
}

function recurseDirs(){
    for folder in */ ; do
        cd $folder
        recurseDirs $1$folder
        cd ..
    done

    if [ "$1"x != ""x ]
    then
        createIncludesFile $1
    fi
}


cd "$DIR/../include"
recurseDirs ""
cd -
