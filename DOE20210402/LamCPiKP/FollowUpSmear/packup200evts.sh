#!/usr/bin/bash
if [[ ! -d batch200evts ]]; then
  mkdir batch200evts
fi
if [[ ! -d batch200evts/log ]]; then
  mkdir batch200evts/log
fi
if [[ ! -d batch200evts/out ]]; then
  mkdir batch200evts/out
fi

tar zcf batch200evts/tarball.tar.gz .rootlogon.C *.C *.h lists/

cp run_200evts.sh batch200evts
