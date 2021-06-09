#!/usr/bin/bash
if [[ ! -d batchScan ]]; then
  mkdir batchScan
fi
if [[ ! -d batchScan/log ]]; then
  mkdir batchScan/log
fi
if [[ ! -d batchScan/out ]]; then
  mkdir batchScan/out
fi

tar zcf batchScan/tarball.tar.gz .rootlogon.C *.C *.h lists/

cp run_scan.sh batchScan
