#!/bin/bash

# make non-matching patterns to an empty list
shopt -s nullglob

LICENSEPATH=/tmp/license.txt

function createIncludesFile(){
    # we are inside current
    current="$1"
    includefile="../$(basename $current).hpp"

    rm -f $includefile

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

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE"  ]; do # resolve $SOURCE until the file is no longer a symlink
    DIR="$( cd -P "$( dirname "$SOURCE"  )" && pwd  )"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /*  ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE"  )" && pwd  )"

cd "$DIR/../include"
recurseDirs ""
cd -
