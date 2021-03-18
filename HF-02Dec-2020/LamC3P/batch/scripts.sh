#!/usr/bin/bash
if [[ ! -d log ]]; then
  echo "mkdir log"
  mkdir log
fi

if [[ ! -d out ]]; then
  echo "mkdir out"
  mkdir out
fi

tar zcf tarball.tar.gz ../lists/ .rootlogon.C ../*.h ../*.C test.list
echo "tarball.tar.gz has been created"

echo "Writing submission script ..."
./write_submit.py -b
echo "Finished writing submission script."
