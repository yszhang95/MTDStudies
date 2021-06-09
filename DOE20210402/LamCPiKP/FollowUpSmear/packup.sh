#!/usr/bin/bash
if [[ ! -d batch ]]; then
  mkdir batch
fi
if [[ ! -d batch/log ]]; then
  mkdir batch/log
fi
if [[ ! -d batch/out ]]; then
  mkdir batch/out
fi

tar zcf batch/tarball.tar.gz .rootlogon.C *.C *.h lists/

cp run.sh batch
