#!/usr/bin/bash
if [[ ! -d batchNoETL ]]; then
  mkdir batchNoETL
fi
if [[ ! -d batchNoETL/log ]]; then
  mkdir batchNoETL/log
fi
if [[ ! -d batchNoETL/out ]]; then
  mkdir batchNoETL/out
fi

tar zcf batchNoETL/tarball.tar.gz .rootlogon.C *.C *.h lists/

cp runNoETL.sh batchNoETL
