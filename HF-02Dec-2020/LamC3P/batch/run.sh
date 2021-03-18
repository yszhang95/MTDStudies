#!/usr/bin/bash
filename=$(basename -- $1)
echo $filename
myfun="${filename%_*}"

tar zxf tarball.tar.gz
mv lists/* .

if [[ $filename == *"1Layer"* ]]; then
  root -b <<- _EOF_
  .L $1+
  ${myfun}("$2", $4)
  .q
_EOF_
else
  root -b <<- _EOF_
  .L $1+
  ${myfun}("$2", $3, $4)
  .q
_EOF_
fi
ls -lst
