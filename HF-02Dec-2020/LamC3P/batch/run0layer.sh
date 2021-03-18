#!/usr/bin/bash
filename=$(basename -- $1)
echo $filename
myfun="${filename%_*}"

tar zxf tarball.tar.gz
mv lists/* .

echo $3
echo $4

root -b <<- _EOF_
.L $1+
${myfun}("$2")
.q
_EOF_
ls -lst
