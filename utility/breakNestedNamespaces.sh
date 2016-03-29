#!/bin/bash

cd ../include

#find . -name \*.\*pp -exec sed -i 's/ { namespace \([a-zA-Z]\+\)/\r{\rnamespace \1/g' \{\} \;
find . -name \*.\*pp -exec sed -i 's/namespace \([a-zA-Z]\+\) {/\rnamespace \1\r{/g' \{\} \;

find . -name \*.\*pp -exec sed -i 's/namespace \([a-zA-Z]*\)\n{\n/namespace \1 { /g' \{\} \;
